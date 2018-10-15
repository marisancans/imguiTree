#include "node.h"


Node::Node(int id, Node& parent) {
    _id = id;
    _status = MIDDLE;
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