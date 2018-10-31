#include "node.h"


Node::Node(Node* parent): _status(MIDDLE){
    parentNodeID = parent->ID;
    P1Pos = parent->P1Pos;
    P2Pos = parent->P2Pos;
}


void Node::setHighlighted(bool b) {
//    if(b) {
//        for (auto &p : parentNodes)
//            p->setHighlighted(true);
//        selected = true;
//    } else {
//        selected = false;
//    }
}



