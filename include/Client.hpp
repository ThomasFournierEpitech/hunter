//
//  Client.hpp
//  hunter_network
//
//  Created by Thomas Fournier on 25/02/2020.
//
//

#ifndef Client_hpp
#define Client_hpp

#include <stdio.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <list>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include "NetworkConnection.hpp"
#include "Game.hpp"
#include "ProtocolManager.hpp"

class Client {
private:
public:
    Client(unsigned short port, const std::string &ip);
    ~Client();

    void UpdateNetwork();

    NetworkConnection client;
    sf::SocketSelector _selector;
    sf::RenderWindow _window;
    sf::Event _event;
    Game _game;
    ProtocolManager _protocolManager;
protected:

};


#endif /* Client_hpp */
