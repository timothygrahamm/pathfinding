#include "nav_point.hpp"

NavPoint::NavPoint(sf::Vector2f _position, float _size, sf::Color _color){
    this->pos = _position;
    this->size = _size;
    this->color = _color;
    this->shape = sf::CircleShape(this->size);
    this->shape.setPosition(this->pos);
    this->shape.setFillColor(this->color);
    this->score = 0;
}
