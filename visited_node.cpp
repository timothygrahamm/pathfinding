#include "visited_node.hpp"

VisitedNode::VisitedNode(int _cost, std::string _prev_node, std::string _node_key){
    this->cost = _cost;
    this->prev_node = _prev_node;
    this->node_key = _node_key;
}