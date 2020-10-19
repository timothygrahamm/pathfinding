#include "nav_point.hpp"

NavPoint::NavPoint(sf::Vector2f _position, float _size, sf::Color _color, std::string _id){
    this->pos = _position;
    this->size = _size;
    this->color = _color;
    this->shape = sf::CircleShape(this->size);
    this->shape.setPosition(this->pos);
    this->shape.setFillColor(this->color);
    this->cost = 0;
    this->id = _id;
}

void NavPoint::calculate_cost(sf::Vector2f destination, std::map<std::string, RegionElement> * obstacle_map){
    int cost = std::abs(int(destination.y) - this->pos.y) + std::abs(int(destination.x) - this->pos.x);
    bool collided = false;
    for ( const auto& key_value : *obstacle_map){
        RegionElement element = key_value.second;
        if (this->shape.getPosition().x + (NAV_MAP_GAP) > element.shape.getPosition().x && this->shape.getPosition().x - (NAV_MAP_GAP) < element.shape.getPosition().x + element.shape.getSize().x &&
        this->shape.getPosition().y + (NAV_MAP_GAP) > element.shape.getPosition().y && this->shape.getPosition().y - (NAV_MAP_GAP) < element.shape.getPosition().y + element.shape.getSize().y){
            collided = true;
        }
        if (collided==true){
            cost = 9999999;
            break;
        }
    }
    this->cost = cost + 1;
}

std::string NavPoint::get_adj_key(Direction dir){

    int adj_x = this->pos.x;
    int adj_y = this->pos.y;

    if (adj_x==WINDOW_X-1){
        adj_x+=1;
    }

    if (adj_y==WINDOW_Y-1){
        adj_y+=1;
    }

    int x_change = 0;
    int y_change = 0;

    if (dir==NORTH_EAST || dir==EAST || dir==SOUTH_EAST){
        x_change = NAV_MAP_GAP;
    }
    else if(dir==SOUTH_WEST || dir==WEST || dir==NORTH_WEST){
        x_change = - NAV_MAP_GAP;
    }
    if (dir==NORTH || dir==NORTH_EAST || dir==NORTH_WEST){
        y_change = - NAV_MAP_GAP;
    }
    if (dir==SOUTH || dir==SOUTH_EAST || dir==SOUTH_WEST){
        y_change = NAV_MAP_GAP;
    }

    adj_x += x_change;
    adj_y += y_change;

    if (adj_x==WINDOW_X){
        adj_x-=1;
    }

    if (adj_y==WINDOW_Y){
        adj_y-=1;
    }

    if (adj_x<0 || adj_x > WINDOW_X-1){
        return "";
    }

    if (adj_y<0 || adj_y > WINDOW_Y-1){
        return "";
    }

    return std::to_string(adj_x) + "," + std::to_string(adj_y);

}
