//
//  ProtocolManager.cpp
//  hunter_network
//
//  Created by Thomas Fournier on 26/02/2020.
//
//'

#include "ProtocolManager.hpp"
#include "NetworkConnection.hpp"
#include "Game.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Duck.hpp"



ProtocolManager::ProtocolManager()
{
    _sfptr.insert( std::make_pair(ProtocolManager::CLIENT_MOUSEMOVE_SEND, &ProtocolManager::ClientMouseMoveSend));
    _sfptr.insert( std::make_pair(ProtocolManager::CLIENT_SHOT_SEND, &ProtocolManager::ClientShotSend));

    _cfptr.insert( std::make_pair(ProtocolManager::SERVER_ID_RESPONSE, &ProtocolManager::ServerIdResponse));
    _cfptr.insert( std::make_pair(ProtocolManager::SERVER_UPDATESCORE_RESPONSE, &ProtocolManager::ServerUpdateScoreResponse));
    _cfptr.insert( std::make_pair(ProtocolManager::SERVER_NEWPLAYER_SEND, &ProtocolManager::ServerNewPlayerSend));
    _cfptr.insert( std::make_pair(ProtocolManager::SERVER_PLAYERLEFT_SEND, &ProtocolManager::ServerPlayerLeftSend));
    _cfptr.insert( std::make_pair(ProtocolManager::SERVER_BIRDDIE_SEND, &ProtocolManager::ServerBirdDieSend));
    _cfptr.insert( std::make_pair(ProtocolManager::SERVER_BIRDSPAWN_SEND, &ProtocolManager::ServerBirdSpawnSend));
    _cfptr.insert( std::make_pair(ProtocolManager::SERVER_PAUSE_SEND, &ProtocolManager::ServerPauseGameSend));
    _cfptr.insert( std::make_pair(ProtocolManager::SERVER_STARTGAME_SEND, &ProtocolManager::ServerStartGameSend));
    _cfptr.insert( std::make_pair(ProtocolManager::SERVER_ENDGAME_SEND, &ProtocolManager::ServerEndGameSend));
    _cfptr.insert( std::make_pair(ProtocolManager::SERVER_MOUSEMOVE_RESPONSE, &ProtocolManager::ServerMouseMoveResponse));

}
ProtocolManager::~ProtocolManager() {}


void ProtocolManager::ClientMouseMoveSend(Server &s, sf::Packet &p)
{
    // std::cout << "ClientMouseMoveSend" << std::endl;

    int x;
    int y;
    int id;

    p >> x;
    p >> y;
    p >> id;
    p.clear();
    p << static_cast<int>(ProtocolManager::SERVER_MOUSEMOVE_RESPONSE);
    p << x;
    p << y;
    p << id;
    s._udp_sender.send(p, sf::IpAddress::Broadcast, 55001);
    s._udp_sender.send(p, sf::IpAddress::Broadcast, 55000);
}

void ProtocolManager::ClientShotSend(Server &s, sf::Packet &p)
{

    int x;
    int y;
    int id;
    int score = 1;
    p >> x;
    p >> y;
    p >> id;


    p.clear();
    sf::Vector2f m_pos(x, y);



    std::vector<Duck *> &ducks = s._game._ducks;
    for (std::vector<Duck *>::iterator it = ducks.begin(); it != ducks.end(); ++it) {
        Duck *duck = *it;
        if (!duck)
            continue;
        sf::FloatRect f_rect = duck->_sp.getGlobalBounds();
        if (f_rect.contains(m_pos)) {
            p << ProtocolManager::SERVER_BIRDDIE_SEND;
            p << duck->_id;
            s._udp_sender.send(p, sf::IpAddress::Broadcast, 55001);
            s._udp_sender.send(p, sf::IpAddress::Broadcast, 55000);
            p.clear();
            p << ProtocolManager::SERVER_UPDATESCORE_RESPONSE;
            p << id;
            p << score;
            s._udp_sender.send(p, sf::IpAddress::Broadcast, 55001);
            s._udp_sender.send(p, sf::IpAddress::Broadcast, 55000);
            delete (duck);
            it = ducks.erase(it);
            std::cout << "bird alive: " << ducks.size() << std::endl;
            break;
        }
    }

}



void ProtocolManager::ServerUpdateScoreResponse(Client &c, sf::Packet &p)
{

    int id;
    int score;
    p >> id;
    p >> score;

    for (auto elem : c._game._scores)

    std::cout << "id: " << elem->getString().toAnsiString() << std::endl;

    std::string s1 = c._game._scores[id]->getString();
    int m_score = std::atoi(s1.c_str());
    m_score += score;

    c._game._scores[id]->setString(std::to_string(m_score).c_str());

    // std::sort(c._game._scores.begin(), c._game._scores.end(), scoreGreater());
}

void ProtocolManager::ServerIdResponse(Client &c, sf::Packet &p)
{

    int id;
    int maxPlayer;
    p >> id;
    p >> maxPlayer;

    c._game._scores = std::vector<sf::Text *>(maxPlayer, nullptr);
    c._game._players = std::vector<Player *>(maxPlayer, nullptr);
    c._game._maxPlayer = maxPlayer;


    Player *player = new Player();
    sf::Text *text = new sf::Text();

    player->_crosshair.setTexture(c._game._texCrosshair);
    player->_crosshair.setColor(sf::Color::Green);
    c._game._scores[id] = text;
    c._game._scores[id]->setFont(c._game._font);
    c._game._scores[id]->setString("0");
    c._game._scores[id]->setFillColor(sf::Color::Green);
    c._game._players[id] = player;
    std::cout << "player: "<< player->_crosshair.getPosition().x << std::endl;
    c.client._id = id;
}

void ProtocolManager::ServerNewPlayerSend(Client &c, sf::Packet &p)
{
    int id;
    sf::Text *text = new sf::Text();


    p >> id;

    std::cout << "ServerNewPlayerSend: " << id<< std::endl;
    if (c.client._id != id) {
        Player *player = new Player();
        player->_crosshair.setTexture(c._game._texCrosshair);
        player->_crosshair.setColor(sf::Color::Blue);
        c._game._scores[id] = text;

        c._game._scores[id]->setFont(c._game._font);
        c._game._scores[id]->setString("0");
        c._game._scores[id]->setFillColor(sf::Color::Blue);
        c._game._players[id] = player;
    }
}

void ProtocolManager::ServerPlayerLeftSend(Client &c, sf::Packet &p)
{
    int id;

    p >> id;
    std::cout << "ServerPlayerLeftSend: " << id<< std::endl;
    delete c._game._players[id];
    c._game._players[id] = nullptr;
    std::cout << "is null ?: " << c._game._players[id] << '\n';
    c._game._players[id] = nullptr;

}

void ProtocolManager::ServerBirdSpawnSend(Client &c, sf::Packet &p)
{
    // std::cout << "ServerBirdSpawnSend" << std::endl;

    float x = 42;
    float y = 42;
    float animeSpeed = 42;
    unsigned int id = 42;

    p >> x;
    p >> y;
    p >> animeSpeed;
    p >> id;
    Duck *duck = new Duck(c._game._texDuck);

    duck->_sp.setPosition(sf::Vector2f(x, y));
    duck->_animeSpeed = animeSpeed;
    duck->_id = id;
    c._game._ducks.push_back(duck);

}


void ProtocolManager::ServerBirdDieSend(Client &c, sf::Packet &p)
{

    unsigned int id;

    std::vector<Duck *> &ducks = c._game._ducks;
    p >> id;
    for (std::vector<Duck *>::iterator it = ducks.begin(); it != ducks.end(); ++it) {
            if ((*it)->_id == id) {
                delete (*it);
                it = ducks.erase(it);
                break;
            }
    }
}

void ProtocolManager::ServerPauseGameSend(Client &c, sf::Packet &p)
{
    // std::cout << "ServerPauseGameSend" << std::endl;
    c._game._state = Game::PAUSED;

}


void ProtocolManager::ServerStartGameSend(Client &c, sf::Packet &p)
{
    c._game._state = Game::RUN;
}

void ProtocolManager::ServerEndGameSend(Client &c, sf::Packet &p)
{
    // std::cout << "ServerEndGameSend" << std::endl;

}

//TODO

void ProtocolManager::ServerMouseMoveResponse(Client &c, sf::Packet &p)
{
    // std::cout << "ServerMouseMoveResponse" << std::endl;

    int x;
    int y;
    int id;



    p >> x;
    p >> y;
    p >> id;

    // std::cout << "c._game._players.size(): " << c._game._players.size() << std::endl;
    // std::cout << "id: " << id << std::endl;

    sf::FloatRect g =c._game._players[id]->_crosshair.getGlobalBounds();
    if (id < c._game._players.size())
        c._game._players[id]->_crosshair.setPosition(sf::Vector2f(static_cast<float>(x - g.width / 2), static_cast<float>(y - g.height / 2)));
}
