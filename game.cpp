#include "game.hpp"

Game::Game(sf::RenderWindow * _window, sf::Font _font){
    this->window = _window;
    this->font = _font;
    this->pieces["NAVMAP"] = GenerateNavMap(_window->getSize().x,_window->getSize().y);
    std::vector<std::string> nav_map_keys = std::vector<std::string>();
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

        std::map<std::string, PointElement> * path = this->pieces["PATH"];

        PointElement * subject = &(*this->pieces["SUBJECT"])["0"];
        float destX = subject->shape.getPosition().x;
        float destY = subject->shape.getPosition().y;

        std::string path_step_id = std::to_string(this->path_step);

        if (destX < (*path)[path_step_id].shape.getPosition().x){
            destX += 0.1;
        }
        else{
            destX -= 0.1;
        }
        if (destY <  (*path)[path_step_id].shape.getPosition().y){
            destY += 0.1;
        }
        else{
            destY -= 0.1;
        }
        if (std::abs(destX - (*path)[path_step_id].shape.getPosition().x) <= 10 && std::abs(destY - (*path)[path_step_id].shape.getPosition().y) <= 10){
            this->path_step++;
        }
        subject->move_to(destX,destY);
    }
}
    
void Game::draw(){

    this->window->clear();
    for( const auto& key_value : this->pieces ) {

        std::map<std::string,PointElement> * piece_map = key_value.second;

        for ( const auto& key_value : *piece_map){
            PointElement element = key_value.second;
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
                (*this->pieces["ORDER"])["0"] = PointElement(order_pos,5.f,sf::Color::Red,"0");
            }
            else{
                std::string nav_map_key_old = find_closest_nav_point_key((*this->pieces["ORDER"])["0"].shape.getPosition().x,(*this->pieces["ORDER"])["0"].shape.getPosition().y);

                (*this->pieces["NAVMAP"])[nav_map_key_old].shape.setFillColor(sf::Color::White);

                (*this->pieces["ORDER"])["0"].move_to(event.mouseButton.x,event.mouseButton.y);
            }

            std::string nav_map_key = find_closest_nav_point_key(event.mouseButton.x,event.mouseButton.y);
            (*this->pieces["NAVMAP"])[nav_map_key].shape.setFillColor(sf::Color::Yellow);
           
            this->pieces["PATH"]->clear();
            this->path_step = 0;

            std::cout << "navmap size before: " + std::to_string(this->pieces["NAVMAP"]->size()) << std::endl;

            for ( std::string key_value : this->nav_map_keys){
                NavPoint * element = static_cast<NavPoint*>(&(*this->pieces["NAVMAP"])[key_value]);
                element->calculate_cost((*this->pieces["ORDER"])["0"].shape.getPosition());
            }

            std::cout << "navmap size after: " + std::to_string(this->pieces["NAVMAP"]->size()) << std::endl;

            this->pieces["PATH"] = GeneratePath((*this->pieces["SUBJECT"])["0"].shape.getPosition(), (*this->pieces["ORDER"])["0"].shape.getPosition());
        }
    }
}

std::map<std::string, PointElement> * Game::GeneratePath(sf::Vector2f origin, sf::Vector2f goal){

    bool goal_reached = false;

    std::map<std::string, PointElement> * path_map = new std::map<std::string, PointElement>();
    int id = 0;
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
        (*path_map)[std::to_string(id)] = PointElement(sf::Vector2f(origin.x, origin.y), 2.f, sf::Color::White, std::to_string(id));
        if (std::abs(origin.x - goal.x) <= 20 && std::abs(origin.y - goal.y) <= 20){
            goal_reached = true;
        }
        id++;
    }

    return path_map;
}



std::string Game::find_closest_nav_point_key(float _x, float _y){
    int x_remainder = int(_x) % NAV_MAP_GAP;
    int mod_nav_map_gap_x = int(_x) - x_remainder;
    int closest_x;
    if (x_remainder <= NAV_MAP_GAP/2){
        closest_x = int(_x) - x_remainder;
    }
    else{
        closest_x = int(_x) + (NAV_MAP_GAP - x_remainder);
    }

    int y_remainder = int(_y) % NAV_MAP_GAP;
    int mod_nav_map_gap_y = int(_y) - y_remainder;
    int closest_y;
    if (y_remainder <= NAV_MAP_GAP/2){
        closest_y = int(_y) - y_remainder;
    }
    else{
        closest_y = int(_y) + (NAV_MAP_GAP - y_remainder);
    }
    return std::to_string(closest_x) + "," + std::to_string(closest_y);
    
}

std::map<std::string, PointElement> * Game::GenerateNavMap(int width, int height){


    std::map<std::string, PointElement> * node_map = new std::map<std::string, PointElement>();
    for(int x = 0; x <= width; x+=NAV_MAP_GAP){
         if (x % NAV_MAP_GAP !=0){
                std::cout << "failed x: " + std::to_string(x) << std::endl;
        }
        if (x==width){
            x=width-1;
        }
        for(int y = 0; y <= height; y+=NAV_MAP_GAP){
            if (y % NAV_MAP_GAP !=0){
                std::cout << "failed y: " + std::to_string(y) << std::endl;
            }
            if(y==height){
                y=y-1;
            }
            std::string id = std::to_string(x) + ',' + std::to_string(y);
            std::cout << id << std::endl;
            (*node_map)[id]= (NavPoint(sf::Vector2f(x, y), 2.f, sf::Color::White, id));
            nav_map_keys.push_back(id);
            if (y==height-1){
                y = height;
            }
        }
        if (x==width-1){
            x = width;
        }
        
    }
    std::cout << "END MAP" << std::endl;
    return node_map;
}

