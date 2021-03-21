#ifndef REGIONELEMENT_H 
#define REGIONELEMENT_H

#include "SFML/Graphics.hpp"
#include <iostream>
#include <unordered_map>
#include <vector>

class RegionElement

{
    public:

        RegionElement(){}

        RegionElement(sf::Vector2f _position, sf::Vector2f _dimensions, sf::Color _color, std::string _id);

        sf::RectangleShape shape;

        std::string id;

        sf::Vector2f pos;
        sf::Vector2f dimensions;
        sf::Color color;

    protected:
        
        
        

};

#endif

