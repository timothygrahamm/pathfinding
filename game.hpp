#ifndef GAME_H 
#define GAME_H

#include "SFML/Graphics.hpp"
#include "point_element.hpp"
#include "nav_point.hpp"
#include "region_element.hpp"
#include "config.hpp"
#include <iostream>
#include <unordered_map>
#include <vector>

class Game

{
    public:

        Game(sf::RenderWindow * _window, sf::Font _font);

        void run();

        std::unordered_map<std::string, std::unordered_map<std::string, RegionElement> *> regions;
        std::unordered_map<std::string, std::unordered_map<std::string, PointElement> *> pieces;

    protected:
    
        void draw();

        void logic();

        void inputs();

        std::unordered_map<std::string, PointElement> * GeneratePath(sf::Vector2f origin, sf::Vector2f goal);

        std::unordered_map<std::string, PointElement> * GenerateNavMap(int width, int height);

        std::string find_closest_nav_point_key(float _x, float _y);

        bool game_running = false;
        sf::RenderWindow * window;
        sf::Font font;
        int path_step;

};

#endif

