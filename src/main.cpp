#include <iostream>
#include "Server.hpp"
#include "Client.hpp"

int main(int ac, char **av)
{
    if (ac == 4) {
        if (std::string(av[1]) == std::string("server")){
            std::cout << "server" << std::endl;
            Server server(atoi(av[2]), atoi(av[3]));
            return 0;
        }
        if (std::string(av[1]) == std::string("client")) {
            std::cout << "client" << std::endl;
            Client client(atoi(av[2]), av[3]);
            return 0;
        }
    // else if (ac == 4) {
    //     if (std::string(av[1]) == std::string("client")) {
    //         std::cout << "client" << std::endl;
    //         Client client(atoi(av[2]), av[3]);
    //         return 0;
    //     }
    }
    std::cout << "usage: hunter_network [client] [port] [ip] / [server] [port] [nb joueur]" << std::endl;
    return 84;
}
