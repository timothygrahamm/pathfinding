#include "point_element.hpp"

PointElement::PointElement(sf::Vector2f _position, float _size, sf::Color _color, std::string _id){
    this->pos = _position;
    this->size = _size;
    this->color = _color;
    this->shape = sf::CircleShape(this->size);
    this->shape.setPosition(this->pos);
    this->shape.setFillColor(this->color);
    this->id = _id;
}

void PointElement::move_to(float x, float y){
    this->pos.x = x;
    this->pos.y = y;
    this->shape.setPosition(this->pos);
}