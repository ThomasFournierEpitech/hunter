//
//  Server.cpp
//  hunter_network
//
//  Created by Thomas Fournier on 25/02/2020.
//
//

#include "Server.hpp"
#include "ProtocolManager.hpp"


Server::Server(unsigned short port, int maxPlayer) :
_game(),
_protocolManager(),
_clients(maxPlayer, nullptr)
 {
    _listener.listen(port);
    _selector.add(_listener);
    _running = true;
    _maxPlayer = maxPlayer;
    _game._clock.restart();
    std::cout << "maxPlayer: " << maxPlayer << std::endl;
    std::cout << "clientsSize: " << _clients.size() << std::endl;
    while (_running)
    {
        UpdateNetwork();
        _game.UpdateServer(_udp_sender);

        _game._dt = _game._clock.restart();
    }

}


void Server::ClientConnect()
{
    int id = 0;
    for (auto &client : _clients) {
        if (client == nullptr)
            break;
        id++;
    }
    std::cout << "id disponible: " << id << std::endl;

    // if (id == _maxPlayer) {
    //     std::cout << "The game is limited to " << _maxPlayer << " players, sorry!" <<std::endl;
    //     return;
    // }
    NetworkConnection *client = new NetworkConnection;
    if (_listener.accept(client->tcp_socket) == sf::Socket::Done)
    {
        client->_ip = client->tcp_socket.getRemoteAddress();
        client->_port = client->tcp_socket.getRemotePort();
        if (client->udp_socket.bind(55002) == sf::Socket::Done || client->udp_socket.bind(55003) == sf::Socket::Done)
        {

            std::cout << "(tcp) Connected to remote addresse: " << client->tcp_socket.getRemoteAddress () << " wich is using port: " << client->tcp_socket.getRemotePort () << std::endl;
            client->_id = id;
            std::cout << "id assigned: " << id << std::endl;
            _clients[id] = client;
            _selector.add(client->tcp_socket);
            _selector.add(client->udp_socket);


            sf::Packet p;
            p << static_cast<int>(ProtocolManager::SERVER_ID_RESPONSE);
            p << client->_id;
            p << _maxPlayer;
            client->tcp_socket.send(p);


            std::cout << "clients size: " << _clients.size() << std::endl;

            for (auto &c : _clients) {
                if (c == nullptr)
                    continue;
                p.clear();
                p << static_cast<int>(ProtocolManager::SERVER_NEWPLAYER_SEND);
                p << c->_id;

                client->tcp_socket.send(p);
            }

            for (auto &c : _clients) {
                if (c == nullptr)
                    continue;
                p.clear();
                p << static_cast<int>(ProtocolManager::SERVER_NEWPLAYER_SEND);
                p << client->_id;

                c->tcp_socket.send(p);
            }

            int check = 0;
            for (auto &client : _clients) {
                if (client == nullptr)
                    break;
                check++;
            }
            if (check == _maxPlayer) {
                sf::Packet p2;
                int prot = 0;
                p2 << ProtocolManager::SERVER_STARTGAME_SEND;
                _udp_sender.send(p2, sf::IpAddress::Broadcast, 55001);
                _udp_sender.send(p2, sf::IpAddress::Broadcast, 55000);
                _game._state = Game::RUN;

            }
        }
        else
            std::cout << "Can't bind udp socket" << std::endl;
    }
    else
        delete client;
}

Server::~Server() {}

void Server::UpdateNetwork()
{
    if (_selector.wait(sf::microseconds(1)))
    {
        // Test the listener
        if (_selector.isReady(_listener))
            ClientConnect();
        else
        {

            // The listener socket is not ready, test all other sockets (the clients)
            for (std::vector<NetworkConnection *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
            {
                if (*it == nullptr)
                    continue;
                //TCP
                NetworkConnection& client = **it;
                if (_selector.isReady(client.tcp_socket))
                {

                    sf::Packet packet;
                    sf::Socket::Status status = client.tcp_socket.receive(packet);


                    if (status == sf::Socket::Disconnected)
                    {

                        _selector.remove(client.tcp_socket);
                        _selector.remove(client.udp_socket);
                        delete *it;
                        *it = nullptr;
                        sf::Packet p2;
                        p2 << ProtocolManager::SERVER_PAUSE_SEND;
                        _udp_sender.send(p2, sf::IpAddress::Broadcast, 55001);
                        _udp_sender.send(p2, sf::IpAddress::Broadcast, 55000);
                        _game._state = Game::PAUSED;
                    }

                    if (status == sf::Socket::Done)
                    {

                        int protocol;
                        if (packet >> protocol) {
                            auto fptr = _protocolManager._sfptr.at(static_cast<ProtocolManager::Protocol>(protocol));
                            (_protocolManager.*fptr)(*this, packet);
                        }
                    }

                }

                //UDP
                if (_selector.isReady(client.udp_socket)) {
                    sf::Packet packet;
                    unsigned short remote_port;
                    sf::IpAddress remote_ip;
                    sf::Socket::Status status = client.udp_socket.receive(packet, remote_ip, remote_port);


                    if (status == sf::Socket::Disconnected)
                    {
                        std::cout << "UdpSocket disconnected to the server"  << std::endl;
                    }

                    if (status == sf::Socket::Done)
                    {

                        int protocol;
                        if (packet >> protocol) {
                            auto fptr = _protocolManager._sfptr[static_cast<ProtocolManager::Protocol>(protocol)];
                            (_protocolManager.*fptr)(*this, packet);
                        }

                    }
                }
            }
        }
    }
}
