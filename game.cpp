#include "game.hpp"

Game::Game(sf::RenderWindow * _window, sf::Font _font){
    this->window = _window;
    this->font = _font;
    this->pieces["NAVMAP"] = GenerateNavMap(_window->getSize().x,_window->getSize().y);
}

void Game::run(){
    this->game_running = true;
    while(this->game_running){
        this->inputs();
        this->logic();
        this->draw();
    }
}
/*
* Currently moves subject along path, needs collision detecting code
* make each Element handle their own logic checks?
*/
void Game::logic(){
     if (this->path_step < this->pieces["PATH"]->size()){

        std::vector<PointElement> * path = this->pieces["PATH"];

        PointElement * subject = &this->pieces["SUBJECT"]->at(0);
        float destX = subject->shape.getPosition().x;
        float destY = subject->shape.getPosition().y;

        if (destX < path->at(this->path_step).shape.getPosition().x){
            destX += 0.1;
        }
        else{
            destX -= 0.1;
        }
        if (destY <  path->at(this->path_step).shape.getPosition().y){
            destY += 0.1;
        }
        else{
            destY -= 0.1;
        }
        if (std::abs(destX -  path->at(this->path_step).shape.getPosition().x) <= 20 && std::abs(destY -  path->at(this->path_step).shape.getPosition().y) <= 20){
            this->path_step++;
        }
        subject->move_to(destX,destY);
    }
}
    
void Game::draw(){

    this->window->clear();
    for( const auto& key_value : this->pieces ) {

        std::vector<PointElement> * piece_vector = key_value.second;

        for (PointElement element: *piece_vector){
            this->window->draw(element.shape);
        }
    }

    this->window->display();
    
}

void Game::inputs(){
    sf::Event event;
    while (this->window->pollEvent(event)){
        
        if (event.type == sf::Event::Closed)
            this->window->close();
        
        if (event.type == sf::Event::MouseButtonPressed){
            if (this->pieces["ORDER"]->size()==0){
                sf::Vector2f order_pos(event.mouseButton.x,event.mouseButton.y);
                this->pieces["ORDER"]->push_back(PointElement(order_pos,10.f,sf::Color::Red));
            }
            else{
                this->pieces["ORDER"]->at(0).move_to(event.mouseButton.x,event.mouseButton.y);
            }
            std::cout << "clearing old path" << std::endl;
            this->pieces["PATH"]->clear();
            this->path_step = 0;
            this->pieces["PATH"] = GeneratePath(this->pieces["SUBJECT"]->at(0).shape.getPosition(), this->pieces["ORDER"]->at(0).shape.getPosition());
        }
    }
}

std::vector<PointElement> * Game::GeneratePath(sf::Vector2f origin, sf::Vector2f goal){
    std::cout << "entering generate path" << std::endl;
    bool goal_reached = false;

    std::vector<PointElement> * path_vector = new std::vector<PointElement>();

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
        path_vector->push_back(PointElement(sf::Vector2f(origin.x, origin.y), 2.f, sf::Color::White));
        if (std::abs(origin.x - goal.x) <= 20 && std::abs(origin.y - goal.y) <= 20){
            goal_reached = true;
        }
    }
    std::cout << "leaving generate path" << std::endl;
    return path_vector;
}

std::vector<PointElement> * Game::GenerateNavMap(int width, int height){

    std::cout << std::to_string(width) << std::endl;
    std::vector<PointElement> * node_map_vector = new std::vector<PointElement>();

    for(int x = 0; x <= width; x+=50){
        for(int y = 0; y <= height; y+=50){
            std::cout << std::to_string(x) << std::endl;
            node_map_vector->push_back(NavPoint(sf::Vector2f(x, y), 2.f, sf::Color::White));
        }
    }
    
    return node_map_vector;
}

