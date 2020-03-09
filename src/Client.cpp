//
//  Client.cpp
//  hunter_network
//
//  Created by Thomas Fournier on 25/02/2020.
//
//


#include "Client.hpp"
#include "ProtocolManager.hpp"
#include "Game.hpp"

void Client::UpdateNetwork()
{
    // Make the selector wait for data on any socket
    if (_selector.wait(sf::microseconds(1)))
    {

        //UDP
        if (_selector.isReady(client.udp_socket))
        {
            // The server has sent some data, we can receive it
            sf::Packet packet;
            sf::Socket::Status status = client.udp_socket.receive(packet, client._ip, client._port);

            if (status == sf::Socket::Disconnected)
            {
                std::cout << "UdpSocket disconnected to the server"  << std::endl;
            }

            if (status == sf::Socket::Done)
            {
                int protocol;
                if (packet >> protocol) {
                    auto fptr = _protocolManager._cfptr.at(static_cast<ProtocolManager::Protocol>(protocol));
                    (_protocolManager.*fptr)(*this, packet);
                }
            }
            else
                std::cout << "read fail" << std::endl;
        }

        //TCP
        if (_selector.isReady(client.tcp_socket))
        {
            // The server has sent some data, we can receive it
            sf::Packet packet;
            sf::Socket::Status status = client.tcp_socket.receive(packet);

            if (status == sf::Socket::Disconnected)
            {
                std::cout << "Disconnected to the server"  << std::endl;
                _window.close();
            }
            if (status == sf::Socket::Done)
            {
                int protocol;
                if (packet >> protocol) {
                    auto fptr = _protocolManager._cfptr.at(static_cast<ProtocolManager::Protocol>(protocol));
                    (_protocolManager.*fptr)(*this, packet);
                }
            }
            else
                std::cout << "read fail" << std::endl;
        }
    }
}



Client::Client(unsigned short port, const std::string &ip) :
_game(),
_protocolManager()
{
    sf::Socket::Status status;
    client._ip = sf::IpAddress(ip);
    client._port = port;

    while (client.tcp_socket.connect(ip, port) != sf::Socket::Done) {
        std::cout << "Can't connect..." << std::endl;
        sleep(3);
        std::cout << "Reconnection..." << std::endl;
    }

    if (client.udp_socket.bind(55001) == sf::Socket::Done || client.udp_socket.bind(55000) == sf::Socket::Done)
    {
    } else
        std::cout << "Can't bind udp socket" << std::endl;


    _selector.add(client.tcp_socket);
    _selector.add(client.udp_socket);


    _game._texCrosshair.loadFromFile("Assets/crosshair.png");
    _game._texDuck.loadFromFile("Assets/duck.png");
    _game._texBg.loadFromFile("Assets/bg.jpeg");

    _game._spBg.setTexture(_game._texBg);

    _window.create(sf::VideoMode(1080, 720), "LAN HUNTER");
    _window.setFramerateLimit(144);
    // _window.setMouseCursorVisible(false);
    _game._clock.restart();
    while (_window.isOpen())
    {
        while (_window.pollEvent(_event))
        {
            // Close window: exit
            if (_event.type == sf::Event::Closed)
                _window.close();
            if (_event.type == sf::Event::MouseMoved) {
                sf::Packet p;
                p << ProtocolManager::CLIENT_MOUSEMOVE_SEND;
                p << _event.mouseMove.x;
                p << _event.mouseMove.y;
                p << client._id;

                if (client._id == 0 && client.udp_socket.send(p, client.tcp_socket.getRemoteAddress(), 55002) != sf::Socket::Done)
                    std::cout << "Fail send protocol CLIENT_MOUSEMOVE" << std::endl;
                if (client._id == 1 && client.udp_socket.send(p, client.tcp_socket.getRemoteAddress(), 55003) != sf::Socket::Done)
                    std::cout << "Fail send protocol CLIENT_MOUSEMOVE" << std::endl;


            }
            if (_event.type == sf::Event::MouseButtonPressed && _game._state == Game::RUN) {
                sf::Packet p2;

                p2 << ProtocolManager::CLIENT_SHOT_SEND;
                p2 << _event.mouseButton.x;
                p2 << _event.mouseButton.y;
                p2 << client._id;
                if (client._id == 0 && client.udp_socket.send(p2, client.tcp_socket.getRemoteAddress(), 55002) != sf::Socket::Done)
                    std::cout << "Fail send protocol CLIENT_SHOT" << std::endl;
                if (client._id == 1 && client.udp_socket.send(p2, client.tcp_socket.getRemoteAddress(), 55003) != sf::Socket::Done)
                    std::cout << "Fail send protocol CLIENT_SHOT" << std::endl;

            }
        }
        UpdateNetwork();
        _game.UpdateClient(_window);
        _game._dt = _game._clock.getElapsedTime();
        _game._clock.restart();
    }

}
Client::~Client() {
    std::cout << "disconnect client" << std::endl;
    client.udp_socket.unbind();
    client.tcp_socket.disconnect();
}
