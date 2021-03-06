#include "game.hpp"
#include "windows.h" 


Game::Game(sf::RenderWindow * _window, sf::Font _font){
    this->window = _window;
    this->font = _font;
    this->pieces["NAVMAP"] = GenerateNavMap(_window->getSize().x,_window->getSize().y);
    this->path_vector = std::vector<std::string>();
    std::vector<std::string> nav_map_keys = std::vector<std::string>();
}



void Game::run(){
    this->game_running = true;
    while(this->game_running){
        this->inputs();
        this->logic();
        this->draw();
        Sleep(10);
    }
}

void Game::logic(){
     if ( this->path_step < this->path_vector.size() ){

        PointElement * subject = &(*this->pieces["SUBJECT"])["0"];
        float destX = subject->shape.getPosition().x;
        float destY = subject->shape.getPosition().y;

        std::string path_step_id = std::to_string(this->path_step);

        NavPoint * next_nav_point = static_cast<NavPoint*>(&(*this->pieces["NAVMAP"])[this->path_vector.at(this->path_step)]);

        if (destX < next_nav_point->shape.getPosition().x){
            destX += MOVE_SPEED;
        }
        else{
            destX -= MOVE_SPEED;
        }
        if (destY <  next_nav_point->shape.getPosition().y){
            destY += MOVE_SPEED;
        }
        else{
            destY -= MOVE_SPEED;
        }
        if (std::abs(destX - next_nav_point->shape.getPosition().x) <= 10 && std::abs(destY - next_nav_point->shape.getPosition().y) <= 10){
            next_nav_point->shape.setFillColor(sf::Color::White);
            this->path_step++;
        }
        subject->move_to(destX,destY);
    }
    this->check_collisions();
}

void Game::check_collisions(){
    bool collided = false;
    PointElement * subject = &(*this->pieces["SUBJECT"])["0"];
    for( const auto& key_value : this->regions ) {
        std::map<std::string,RegionElement> * region_map = key_value.second;
        for ( const auto& key_value : *region_map){
            RegionElement element = key_value.second;
            if (subject->shape.getPosition().x + (subject->shape.getRadius()) > element.shape.getPosition().x && subject->shape.getPosition().x - (subject->shape.getRadius()) < element.shape.getPosition().x + element.shape.getSize().x &&
            subject->shape.getPosition().y + (subject->shape.getRadius()) > element.shape.getPosition().y && subject->shape.getPosition().y - (subject->shape.getRadius()) < element.shape.getPosition().y + element.shape.getSize().y){
                this->path_step = 0;
                NavPoint * old_point;
                for( const auto& key : this->path_vector ) {
                    old_point = static_cast<NavPoint*>(&(*this->pieces["NAVMAP"])[key]);
                    old_point->shape.setFillColor(sf::Color::White);
                }
                this->path_vector.clear();
                collided = true;
                break;
            }
        }
        if (collided==true){
            break;
        }
        
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

    for( const auto& key_value : this->regions ) {

        std::map<std::string,RegionElement> * region_map = key_value.second;

        for ( const auto& key_value : *region_map){
            RegionElement element = key_value.second;
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
            
            if (event.mouseButton.button == sf::Mouse::Left){

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
                (*this->pieces["NAVMAP"])[nav_map_key].shape.setFillColor(sf::Color::Green);
            
                this->path_step = 0;

                for ( std::string key_value : this->nav_map_keys){
                    NavPoint * element = static_cast<NavPoint*>(&(*this->pieces["NAVMAP"])[key_value]);
                    element->calculate_cost((*this->pieces["ORDER"])["0"].shape.getPosition(), this->regions["OBSTACLE"]);
                }

                //this->path_vector = GeneratePath((*this->pieces["SUBJECT"])["0"].shape.getPosition(), (*this->pieces["ORDER"])["0"].shape.getPosition());
                this->path_vector = GeneratePathDijkstraWithAstar((*this->pieces["SUBJECT"])["0"].shape.getPosition(), (*this->pieces["ORDER"])["0"].shape.getPosition());
            }

            else if (event.mouseButton.button == sf::Mouse::Right) {
                std::string new_obstacle_id = std::to_string((*this->regions["OBSTACLE"]).size());
                sf::Vector2f pos(event.mouseButton.x,event.mouseButton.y);
                sf::Vector2f dim(50.f,50.f);
                (*this->regions["OBSTACLE"])[new_obstacle_id] = (RegionElement(pos,dim,sf::Color::White, new_obstacle_id));
            }

        }

        /*else if (event.type == sf::Event::MouseButtonReleased){
            std::cout << "RELEASED" << std::endl;
            for ( const auto& key_value : *this->regions["OBSTACLE"]){
                (*this->regions["OBSTACLE"])[key_value.first] = (RegionElement(key_value.second.pos,key_value.second.dim,sf::Color::White, new_obstacle_id));
        }*/
        
    }
}

std::vector<std::string> Game::GeneratePath(sf::Vector2f origin, sf::Vector2f goal){

    bool goal_reached = false;
    NavPoint * old_point;
    for( const auto& key : this->path_vector ) {
        old_point = static_cast<NavPoint*>(&(*this->pieces["NAVMAP"])[key]);
        old_point->shape.setFillColor(sf::Color::White);
    }
    this->path_vector.clear();

    std::vector<std::string> path_vector = std::vector<std::string>();

    std::string origin_key = find_closest_nav_point_key(origin.x, origin.y);
    std::string goal_key = find_closest_nav_point_key(goal.x, goal.y);

    NavPoint * current_point = static_cast<NavPoint*>(&(*this->pieces["NAVMAP"])[origin_key]);

    std::vector<std::string> visited_keys = std::vector<std::string>();
    
    while (!goal_reached){

        std::string dir_key;
        std::string best_cost_key;
        int current_cost = INT32_MAX;

        for ( int dir = NORTH; dir != LAST; dir++ ){

            dir_key = current_point->get_adj_key(Direction(dir));
            if (dir_key==""){
                continue;
            }
            NavPoint * adjacent_point = static_cast<NavPoint*>(&(*this->pieces["NAVMAP"])[dir_key]);

            bool visited = false;
            for ( std::string key_value : visited_keys){
                if (adjacent_point->id == key_value){
                    visited = true;
                }
            }

            if (adjacent_point->cost < current_cost && !visited){
                
                best_cost_key = adjacent_point->id;
                current_cost = adjacent_point->cost;
            }
        }
        visited_keys.push_back(best_cost_key);
        current_point = static_cast<NavPoint*>(&(*this->pieces["NAVMAP"])[best_cost_key]);
        current_point->shape.setFillColor(sf::Color::Green);
        path_vector.push_back(best_cost_key);

        if (best_cost_key==goal_key){
            goal_reached = true;
        }
        
    }

    return path_vector;
}

std::vector<std::string> Game::GeneratePathDijkstraWithAstar(sf::Vector2f origin, sf::Vector2f goal){

    bool goal_reached = false;
    NavPoint * old_point;
    for( const auto& key : this->path_vector ) {
        old_point = static_cast<NavPoint*>(&(*this->pieces["NAVMAP"])[key]);
        old_point->shape.setFillColor(sf::Color::White);
    }
    this->path_vector.clear();
    
    std::vector<std::string> path_vector = std::vector<std::string>();

    auto compare_node_cost = [](VisitedNode left, VisitedNode right) { return (left.cost ^ 1) > (right.cost ^ 1); };
    
    std::priority_queue<VisitedNode, std::vector<VisitedNode>, decltype(compare_node_cost)> fringe(compare_node_cost);

    std::map<std::string, VisitedNode> expanded_nodes = std::map<std::string, VisitedNode>();

    std::string origin_key = find_closest_nav_point_key(origin.x, origin.y);
    std::string goal_key = find_closest_nav_point_key(goal.x, goal.y);

    NavPoint * current_navpoint = static_cast<NavPoint*>(&(*this->pieces["NAVMAP"])[origin_key]);

    VisitedNode current_point = VisitedNode(0, "START", origin_key);

    expanded_nodes[origin_key] = VisitedNode(0, "START", origin_key);
    
    while (true){

        std::string dir_key;

        int current_cost = INT32_MAX;

        for ( int dir = NORTH; dir != LAST; dir++ ){

            dir_key = current_navpoint->get_adj_key(Direction(dir));

            if (dir_key==""){
                continue;
            }

            NavPoint * adjacent_point = static_cast<NavPoint*>(&(*this->pieces["NAVMAP"])[dir_key]);

            VisitedNode v_node = VisitedNode(adjacent_point->cost + current_point.cost, current_point.node_key, dir_key);

            fringe.push(v_node);

        }

        bool valid_point_not_found = true;
        while(valid_point_not_found){
            current_point = fringe.top();
            if (expanded_nodes.count(current_point.node_key)==0){
                valid_point_not_found = false;
            }
            fringe.pop();
            if (fringe.size()==0){
                break;
            }
        }
        
        current_navpoint = static_cast<NavPoint*>(&(*this->pieces["NAVMAP"])[current_point.node_key]);

        current_navpoint->shape.setFillColor(sf::Color::Blue);

        this->draw();

        Sleep(PATH_CALCULATION_DELAY);

        expanded_nodes[current_point.node_key] = VisitedNode(current_point.cost, current_point.prev_node, current_point.node_key);

        if (fringe.size()==0){

            for ( auto& key_value : *this->pieces["NAVMAP"]){
                PointElement * element = &key_value.second;
                element->shape.setFillColor(sf::Color::White);
            }
            this->draw();
            VisitedNode resolved_point = expanded_nodes[goal_key];
            while(true){
                //std::cout << resolved_point.node_key << std::endl;
                resolved_point = expanded_nodes[resolved_point.prev_node];
                current_navpoint = static_cast<NavPoint*>(&(*this->pieces["NAVMAP"])[resolved_point.node_key]);
                current_navpoint->shape.setFillColor(sf::Color::Green);

                path_vector.insert(path_vector.begin(), resolved_point.node_key);
                if (resolved_point.node_key==origin_key){
                    break;
                }

            }
            return path_vector;
        }
        
    }

    return std::vector<std::string>();
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

    if (closest_y==WINDOW_Y){
        closest_y--;
    }
    if (closest_x==WINDOW_X){
        closest_x--;
    }
    
    return std::to_string(closest_x) + "," + std::to_string(closest_y);
    
}

std::map<std::string, PointElement> * Game::GenerateNavMap(int width, int height){


    std::map<std::string, PointElement> * node_map = new std::map<std::string, PointElement>();
    for(int x = 0; x <= width; x+=NAV_MAP_GAP){
        if (x % NAV_MAP_GAP !=0){
            //std::cout << "failed x: " + std::to_string(x) << std::endl;
        }
        if (x==width){
            x=width-1;
        }
        for(int y = 0; y <= height; y+=NAV_MAP_GAP){
            if (y % NAV_MAP_GAP !=0){
                //std::cout << "failed y: " + std::to_string(y) << std::endl;
            }
            if(y==height){
                y=y-1;
            }
            std::string id = std::to_string(x) + ',' + std::to_string(y);
            //std::cout << id << std::endl;
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
    //std::cout << "END MAP" << std::endl;
    return node_map;
}

