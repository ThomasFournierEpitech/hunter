//
//  Server.hpp
//  hunter_network
//
//  Created by Thomas Fournier on 25/02/2020.
//
//

#ifndef Server_hpp
#define Server_hpp

#include <stdio.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <list>
#include <iostream>
#include <SFML/Network.hpp>
#include "NetworkConnection.hpp"
#include "Game.hpp"
#include "ProtocolManager.hpp"

class Server {
public:
    Server(unsigned short port, int max_player);
    ~Server();

    void UpdateNetwork();
    void ClientConnect();

    bool _running;
    int _maxPlayer;
    Game _game;
    sf::TcpListener _listener;
    std::vector<NetworkConnection *> _clients;
    sf::SocketSelector _selector;
    ProtocolManager _protocolManager;
    sf::UdpSocket _udp_sender;

};


#endif /* Server_hpp */
