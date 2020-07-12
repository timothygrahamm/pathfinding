#include <iostream>
#include <vector>
#include <string>
#include "SFML/Graphics.hpp"
#include <ctime>
#include "test.hpp"

int WinMain()
{
    sf::RenderWindow window(sf::VideoMode(1600, 900), "SFML works!");
    sf::CircleShape order(10.f);
    sf::Vector2f order_pos(300.f,600.f);
    order.setPosition(order_pos);

    sf::CircleShape shape(50.f);
    sf::Vector2f shape_pos(100.f,100.f);
    shape.setPosition(shape_pos);

    sf::Font font;
    if (!font.loadFromFile("resources/sansation.ttf"))
        return EXIT_FAILURE;

    // Initialize the pause message
    sf::Text fps;
    fps.setFont(font);
    fps.setCharacterSize(40);
    fps.setPosition(1500.f, 800.f);
    fps.setFillColor(sf::Color::White);
    fps.setString("0");

    shape.setFillColor(sf::Color::Blue);
    order.setFillColor(sf::Color::Green);
    
    std::clock_t start;
    double duration;

    start = std::clock();
    int frames = 0;
    while (window.isOpen())
    {
        sf::Event event;
        TestClass * messager = new TestClass();
        
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
             if (event.type == sf::Event::MouseButtonPressed){
                 order_pos.x = event.mouseButton.x;
                 order_pos.y = event.mouseButton.y;
                 order.setPosition(order_pos);
             }
        }

        if (shape_pos.x < order_pos.x){
            shape_pos.x += 0.01;
        }
        else{
            shape_pos.x -= 0.01;
        }
        if (shape_pos.y < order_pos.y){
            shape_pos.y += 0.01;
        }
        else{
            shape_pos.y -= 0.01;
        }

        
        shape.setPosition(shape_pos);
        duration += ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
        window.clear();
        window.draw(order);
        window.draw(shape);
        window.draw(fps);
        if (duration > 1000) {
            std::cout << duration << std::endl;
            start = std::clock();
            fps.setString(std::to_string(frames));
            frames=0;
            duration = 0;
        }
        else{
            frames++;
        }
        
        window.display();
        
    }

    return 0;
}