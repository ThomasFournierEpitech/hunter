//
//  Duck.cpp
//  hunter_network
//
//  Created by Thomas Fournier on 05/03/2020.
//
//

#include "Duck.hpp"

Duck::Duck(sf::Texture &tex) {
    _sp.setTexture(tex);
    _sp.setTextureRect (sf::IntRect(0, 0, 60, 45));
    _animeSpeed = 0; //value between 0.01 to 1


}
Duck::~Duck() {}

void Duck::Fly(sf::Time dt) {
    static float maxSpeedAnimOffset = 100;
    static float maxSpeedOffset = 5;
    sf::IntRect rectangleDuck(_sp.getTextureRect());
    float speed = 1 + maxSpeedOffset * _animeSpeed;
    float offset = 0;

    _moveDuckClock += dt;
    _animeDuckClock += dt;
    while (_moveDuckClock.asMilliseconds () >= 17) {
        offset += speed;
        _moveDuckClock -= sf::milliseconds(17);
    }
    _sp.move(offset, 0);
    while (_animeDuckClock.asMilliseconds () >= (110 + (maxSpeedAnimOffset * _animeSpeed))) {
            rectangleDuck.left += rectangleDuck.width;
            if (rectangleDuck.left >= 180)
                rectangleDuck.left = 0;
        _sp.setTextureRect (rectangleDuck);
        _animeDuckClock -= sf::milliseconds((110 + (maxSpeedAnimOffset * _animeSpeed)));
    }
}
