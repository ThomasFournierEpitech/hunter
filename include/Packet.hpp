//
//  Packet.hpp
//  hunter_network
//
//  Created by Thomas Fournier on 04/03/2020.
//
//

#ifndef Packet_hpp
#define Packet_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

class Packet : public sf::Packet {
private:
public:
    Packet();
    ~Packet();

    virtual void Open() = 0;
protected:

};


#endif /* Packet_hpp */
