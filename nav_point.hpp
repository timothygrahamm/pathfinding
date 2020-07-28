#ifndef NAVPOINT_H 
#define NAVPOINT_H

#include "point_element.hpp"

class NavPoint : public PointElement

{
    public:

        NavPoint(sf::Vector2f _position, float _size, sf::Color _color);

        int score;

};

#endif

