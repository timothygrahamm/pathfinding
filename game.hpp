#ifndef GAME_H 
#define GAME_H

#include "SFML/Graphics.hpp"
#include "point_element.hpp"
#include "nav_point.hpp"
#include "region_element.hpp"
#include "config.hpp"
#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <functional>

class Game

{
    public:

        Game(sf::RenderWindow * _window, sf::Font _font);

        void run();

        std::map<std::string, std::map<std::string, RegionElement> *> regions;
        std::map<std::string, std::map<std::string, PointElement> *> pieces;

    protected:
    
        void draw();

        void logic();

        void inputs();

        void check_collisions();

        std::vector<std::string> * GeneratePath(sf::Vector2f origin, sf::Vector2f goal);

        std::vector<std::string> * GeneratePathDijkstraWithAstar(sf::Vector2f origin, sf::Vector2f goal);

        std::map<std::string, PointElement> * GenerateNavMap(int width, int height);

        std::string find_closest_nav_point_key(float _x, float _y);

        bool game_running = false;
        sf::RenderWindow * window;
        sf::Font font;
        int path_step;

        std::vector<std::string> path_vector;
        std::vector<std::string> nav_map_keys;

};

class VisitedNode {

    public:

        VisitedNode();

        VisitedNode(int _cost, std::string _prev_node, std::string _node_key){
            this->cost = _cost;
            this->prev_node = _prev_node;
            this->node_key = node_key;
        }

        int cost;
        std::string prev_node;
        std::string node_key;
} ;

#endif

