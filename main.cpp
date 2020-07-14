#include <iostream>
#include <vector>
#include <string>
#include "SFML/Graphics.hpp"
#include <ctime>
#include "test.hpp"

std::vector<sf::Vector2f> GeneratePath(sf::Vector2f origin, sf::Vector2f goal){
    std::cout << "entering generate path" << std::endl;
    bool goal_reached = false;
    std::vector<sf::Vector2f> path = std::vector<sf::Vector2f>();
    while (!goal_reached){

        if (origin.x < goal.x && std::abs(origin.x - goal.x) >= 20){
            origin.x += 10;
        }
        else if (origin.x > goal.x && std::abs(origin.x - goal.x) >= 20){
            origin.x -= 10;
        }

        if (origin.y < goal.y && std::abs(origin.y - goal.y) >= 20){
            origin.y += 10;
        }
        else if (origin.y > goal.y && std::abs(origin.y - goal.y) >= 20){
            origin.y -= 10;
        }

        path.push_back(sf::Vector2f(origin.x, origin.y));
        if (std::abs(origin.x - goal.x) <= 20 && std::abs(origin.y - goal.y) <= 20){
            goal_reached = true;
        }
    }
    std::cout << "leaving generate path" << std::endl;
    return path;
}

int WinMain()
{
    sf::RenderWindow window(sf::VideoMode(1600, 900), "pathfinding");
    sf::CircleShape order(5.f);
    sf::Vector2f order_pos(300.f,600.f);
    order.setPosition(order_pos);
    sf::CircleShape shape(5.f);
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
    bool path_shapes_set = false;
    double duration;
    std::vector<sf::Vector2f> path;
    std::vector<sf::CircleShape> path_shapes;
    start = std::clock();
    int frames = 0;
    int path_step = 0;
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
                path = GeneratePath(shape_pos, order_pos);
                int i = 0;
                path_shapes = std::vector<sf::CircleShape>();
                std::cout << "cleared shapes" << std::endl;
                path_step = 0;
                for(sf::Vector2f point : path){
                    std::cout << point.x << std::endl;
                    sf::CircleShape temp_path_shape = sf::CircleShape(1.f);
                    temp_path_shape.setPosition(point.x,point.y);
                    temp_path_shape.setFillColor(sf::Color::White);
                    path_shapes.push_back(temp_path_shape);
                }
                path_shapes_set = true; 
             }
        }

        if (path_step < path.size()){
            if (shape_pos.x < path.at(path_step).x){
                shape_pos.x += 0.01;
            }
            else{
                shape_pos.x -= 0.01;
            }
            if (shape_pos.y <  path.at(path_step).y){
                shape_pos.y += 0.01;
            }
            else{
                shape_pos.y -= 0.01;
            }
            if (std::abs(shape_pos.x -  path.at(path_step).x) <= 20 && std::abs(shape_pos.y -  path.at(path_step).y) <= 20){
                path_step++;
            }
        }

        shape.setPosition(shape_pos);

        duration += ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
        window.clear();

        for(sf::CircleShape point : path_shapes){
            window.draw(point);
        } 

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

