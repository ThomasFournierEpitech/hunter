//
//  Game.cpp
//  hunter_network
//
//  Created by Thomas Fournier on 25/02/2020.
//
//

#include "Game.hpp"
#include "ProtocolManager.hpp"
#include <stdlib.h>

Game::Game()
{
    _dt = sf::microseconds(0);
    _state = PAUSED;
    std::srand(std::time(nullptr));
}
Game::~Game() {
}

void Game::CreateDuck(sf::UdpSocket &s, float x, float y)
{

    static unsigned int id = 0;
    if (id >= 0) {

        Duck *duck = new Duck(_texDuck);
        sf::Packet p;
        duck->_animeSpeed = ((std::rand() % 100) + 1) / 100.f; //value between 0.01 to 1

        p << static_cast<int>(ProtocolManager::SERVER_BIRDSPAWN_SEND);
        p << x;
        p << y;
        p << duck->_animeSpeed;
        p << id;
        s.send(p, sf::IpAddress::Broadcast, 55001);
        s.send(p, sf::IpAddress::Broadcast, 55000);
        duck->_id = id;
        duck->_sp.setPosition(sf::Vector2f(x, y));
        if (id < _ducks.size())
            _ducks.insert(_ducks.begin() + id, duck);
        else
            _ducks.push_back(duck);
    }
    id++;
}

//SERVER
void Game::UpdateServer(sf::UdpSocket &s)
{
    static sf::Time spawnDuckClock = sf::microseconds(0);


    if (_state == RUN) {
        spawnDuckClock += _dt;
        while (spawnDuckClock.asSeconds () >= 1) {
            CreateDuck(s, - (std::rand() % 100 + 60), std::rand() % (720 - 45));
            spawnDuckClock -= sf::seconds(1);
        }
        for (std::vector<Duck *>::iterator it = _ducks.begin(); it != _ducks.end(); ++it) {
            Duck *duck = *it;
            if (!duck)
                continue;

            duck->Fly(_dt);
            if (duck->_sp.getPosition().x > 1500) {
                sf::Packet p;
                p << ProtocolManager::SERVER_BIRDDIE_SEND;
                p << duck->_id;
                s.send(p, sf::IpAddress::Broadcast, 55001);
                s.send(p, sf::IpAddress::Broadcast, 55000);
                p.clear();
                delete (duck);
                it = _ducks.erase(it);
            }
        }
    }

}

void Game::UpdateClient(sf::RenderWindow &window)
{
    if (_state == RUN)
        for (auto &duck : _ducks)
            if (duck)
                duck->Fly(_dt);
    window.clear(sf::Color::White);
    window.draw(_spBg);
    for (std::vector<Duck *>::iterator it = _ducks.begin(); it != _ducks.end(); ++it)
        if (*it)
            window.draw((*it)->_sp);
    for (std::vector<Player *>::iterator it = _players.begin(); it != _players.end(); ++it)
        if (*it)
            window.draw((*it)->_crosshair);
    window.display();
}
