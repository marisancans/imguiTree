#include "node.h"


Node::Node(Node const* parent){

}


void Node::setHighlighted(bool b) {
    if(b) {
        for (auto &p : parentNodes)
            p->setHighlighted(true);
        selected = true;
    } else {
        selected = false;
    }
}



