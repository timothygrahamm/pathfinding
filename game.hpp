#ifndef GAME_H 
#define GAME_H

#include "SFML/Graphics.hpp"
#include "point_element.hpp"
#include "nav_point.hpp"
#include "region_element.hpp"
#include <iostream>
#include <unordered_map>
#include <vector>

class Game

{
    public:

        Game(sf::RenderWindow * _window, sf::Font _font);

        void run();

        std::unordered_map<std::string, std::vector<RegionElement> *> regions;
        std::unordered_map<std::string, std::vector<PointElement> *> pieces;

    protected:
    
        void draw();

        void logic();

        void inputs();

        std::vector<PointElement> * GeneratePath(sf::Vector2f origin, sf::Vector2f goal);

        std::vector<PointElement> * GenerateNavMap(int width, int height);

        bool game_running = false;
        sf::RenderWindow * window;
        sf::Font font;
        int path_step;

};

#endif

