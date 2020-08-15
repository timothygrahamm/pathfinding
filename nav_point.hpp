#ifndef NAVPOINT_H 
#define NAVPOINT_H

#include "config.hpp"
#include "point_element.hpp"
#include "region_element.hpp"

enum Direction
{
    NORTH,
    NORTH_EAST,
    EAST,
    SOUTH_EAST,
    SOUTH,
    SOUTH_WEST,
    WEST,
    NORTH_WEST,
    LAST
};

class NavPoint : public PointElement

{
    public:

        NavPoint(sf::Vector2f _position, float _size, sf::Color _color, std::string _id);

        std::string get_lowest_cost_key();

        std::string get_adj_key(Direction dir);

        void calculate_cost(sf::Vector2f destination, std::map<std::string, RegionElement> * obstacle_map);

        int cost;

};

#endif

