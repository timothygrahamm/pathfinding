#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "SFML/Graphics.hpp"
#include <ctime>
#include "game.hpp"
#include "point_element.hpp"
#include "region_element.hpp"
#include "config.hpp"

int WinMain()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_X, WINDOW_Y), "pathfinding");

    sf::Font font;
    //if (!font.loadFromFile("resources/sansation.ttf"))
    //    return EXIT_FAILURE;
    
    Game game = Game(&window,font);

    std::map<std::string, PointElement> * subject_map = new std::map<std::string, PointElement>();
    game.pieces["SUBJECT"] = subject_map;
    sf::Vector2f subject_pos(300.f,600.f);
    (*subject_map)["0"] = (PointElement(subject_pos,20.f,sf::Color::Green, "0"));

    std::map<std::string, PointElement> * order_map = new std::map<std::string, PointElement>();
    game.pieces["ORDER"] = order_map;
    
    
    std::map<std::string, RegionElement> * obstacle_map = new std::map<std::string, RegionElement>();
    game.regions["OBSTACLE"] = obstacle_map;
    sf::Vector2f test_pos(1000,300.f);
    sf::Vector2f test_dim(100.f,100.f);
    (*obstacle_map)["0"] = (RegionElement(test_pos,test_dim,sf::Color::White, "0"));

    game.run();

    return 0;

}

