
#include <iostream>



class VisitedNode {

    public:

        VisitedNode(){}
        VisitedNode(int _cost, std::string _prev_node, std::string _node_key);
        
        int cost;
        std::string prev_node;
        std::string node_key;
} ;