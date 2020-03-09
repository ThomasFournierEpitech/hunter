//
//  Duck.hpp
//  hunter_network
//
//  Created by Thomas Fournier on 05/03/2020.
//
//

#ifndef Duck_hpp
#define Duck_hpp


#include <stdio.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <list>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include "NetworkConnection.hpp"


class Duck {
private:
public:
    Duck(sf::Texture &tex);
    ~Duck();

    void Fly(sf::Time dt);

    sf::Sprite _sp;
    unsigned int _id;
    float _animeSpeed;
    sf::Time _moveDuckClock;
    sf::Time _animeDuckClock;
protected:

};


#endif /* Duck_hpp */
