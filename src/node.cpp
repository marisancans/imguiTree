#include "node.h"


Node::Node(int id, Node* parent): _id(id), _status(MIDDLE){
    parentNodeID = parent->_id;
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



