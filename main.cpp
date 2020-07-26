#include <iostream>
#include <vector>
#include <string>
#include "SFML/Graphics.hpp"
#include <ctime>
#include "game.hpp"
#include "point_element.hpp"

int WinMain()
{
    sf::RenderWindow window(sf::VideoMode(1600, 900), "pathfinding");

    sf::Font font;
    if (!font.loadFromFile("resources/sansation.ttf"))
        return EXIT_FAILURE;
    
    Game game = Game(&window,font);

    std::vector<PointElement> * subject_vector = new std::vector<PointElement>();
    game.pieces["SUBJECT"] = subject_vector;
    sf::Vector2f subject_pos(300.f,600.f);
    subject_vector->push_back(PointElement(subject_pos,20.f,sf::Color::Green));

    std::vector<PointElement> * order_vector = new std::vector<PointElement>();
    game.pieces["ORDER"] = order_vector;

    std::vector<PointElement> * path_vector = new std::vector<PointElement>();
    game.pieces["PATH"] = path_vector;

    game.run();

    return 0;

}

