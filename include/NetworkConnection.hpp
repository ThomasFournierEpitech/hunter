//
//  NetworkConnection.hpp
//  hunter_network
//
//  Created by Thomas Fournier on 25/02/2020.
//
//

#ifndef NetworkConnection_hpp
#define NetworkConnection_hpp

#include <stdio.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <list>
#include <iostream>
#include <SFML/Network.hpp>

class NetworkConnection {
    public:
        NetworkConnection();
        ~NetworkConnection();
        sf::TcpSocket tcp_socket;
        sf::UdpSocket udp_socket;

        sf::IpAddress _ip;
        unsigned short  _port;

        unsigned int _id;

};


#endif /* NetworkConnection_hpp */
