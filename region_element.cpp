#include "region_element.hpp"

RegionElement::RegionElement(sf::Vector2f _position, sf::Vector2f _dimensions, sf::Color _color, std::string _id){
    this->pos = _position;
    this->dimensions = _dimensions;
    this->color = _color;
    this->shape = sf::RectangleShape(this->dimensions);
    this->shape.setPosition(this->pos);
    this->shape.setOutlineColor(this->color);
    this->shape.setOutlineThickness(1);
    this->shape.setFillColor(sf::Color::Transparent);
    this->id = _id;
}

/*void RegionElement::move_to(float x, float y){
    this->pos.x = x;
    this->pos.y = y;
    this->shape.setPosition(this->pos);
}*/