//
//  Player.hpp
//  hunter_network
//
//  Created by Thomas Fournier on 26/02/2020.
//
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <list>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include "NetworkConnection.hpp"


class Player {
private:
public:
    Player();
    ~Player();

    sf::Sprite _crosshair;

protected:

};


#endif /* Player_hpp */
