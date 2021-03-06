#ifndef POINTELEMENT_H 
#define POINTELEMENT_H

#include "SFML/Graphics.hpp"
#include <iostream>
#include <unordered_map>
#include <vector>

class PointElement

{
    public:

        PointElement(){}

        PointElement(sf::Vector2f _position, float _size, sf::Color _color, std::string _id);

        sf::CircleShape shape;

        void move_to(float x, float y);

        std::string id;

    protected:

        sf::Vector2f pos;
        float size;
        sf::Color color;

};

#endif

