//
//  NetworkConnection.cpp
//  hunter_network
//
//  Created by Thomas Fournier on 25/02/2020.
//
//

#include "NetworkConnection.hpp"

NetworkConnection::NetworkConnection()  {}
NetworkConnection::~NetworkConnection() {
    udp_socket.unbind();
    tcp_socket.disconnect();
}
