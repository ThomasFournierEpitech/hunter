//
//  Game.hpp
//  hunter_network
//
//  Created by Thomas Fournier on 25/02/2020.
//
//

#ifndef Game_hpp
#define Game_hpp


#include <stdio.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <list>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include "NetworkConnection.hpp"
#include "Player.hpp"
#include "Duck.hpp"

class Game {
private:
public:
    enum State {
        RUN,
        PAUSED,
    };


    Game();
    ~Game();

    State _state;
    int _maxPlayer;

    sf::Texture _texBg;
    sf::Texture _texDuck;
    sf::Texture _texCrosshair;

    sf::Sprite _spBg;

    std::vector<Duck *> _ducks;


    std::vector<Player *> _players;

    sf::Clock _clock;
    sf::Time _dt;
    void CreateDuck(sf::UdpSocket &s, float x, float y);
    void UpdateClient(sf::RenderWindow &window);
    void UpdateServer(sf::UdpSocket &s);


protected:

};


#endif /* Game_hpp */
