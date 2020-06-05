//
// Created by muzamil on 1/14/20.
//

#include "Node.h"

#include <utility>

Node::Node() {
    Node::val.resize(VAL_SIZE);
    /*for(int i = 0; i < VAL_SIZE ; i++)
    {
        Node::val[i] = *new LineSegment();
    }*/
    //TODO() change resize into reserve, and use emplace_back instead!
    Node::child.resize(CHILD_SIZE);
    Node::minMaxX.resize(CHILD_SIZE);
}

Node::Node(std::vector<LineSegment> val) : val(std::move(val)) {
    //TODO() change resize into reserve, and use emplace_back instead!
    Node::child.resize(CHILD_SIZE);
    Node::minMaxX.resize(CHILD_SIZE);
}

Node::Node(std::vector<LineSegment> val,
           std::vector<Node *> child,
           std::vector<double>& minMaxX)
        : val(std::move(val)), child(std::move(child)), minMaxX(minMaxX) {
}

const std::vector<LineSegment> &Node::getVal() const { return val; }

void Node::setVal(const std::vector<LineSegment> &val) { Node::val = val; }

const std::vector<Node *> &Node::getChild() const { return child; }

void Node::setChild(const std::vector<Node *> &child) { Node::child = child; }

// NEED to check this later with indices instead of push_back
void Node::setIthVal(const LineSegment &lineSegment, const int &i) {
    Node::val[i] = lineSegment;
}

const LineSegment &Node::getIthVal(const int &i) const { return Node::val[i]; }

void Node::setValSize(const unsigned int &valSize) {
    Node::val.resize(valSize);
}

const std::vector<double> &Node::getMinMaxX() const { return minMaxX; }

void Node::setMinMaxX(const std::vector<double> &minMaxX) {
    Node::minMaxX = minMaxX;
}

void Node::setIthMinMaxX(const double &minMax, const int &i) {
    Node::minMaxX[i] = minMax;
}

const Node *Node::getIthChild(int &i) const { return child[i]; }

void Node::setIthChild(Node &node, const int &i) { Node::child[i] = &node; }

const bool Node::underflow() const { return Node::getValSize() < MIN_VAL; }

const unsigned int Node::getValSize() const { return Node::val.size(); }

std::ostream &operator<<(std::ostream &os, const Node &node) {
    for (int i = 0; i < node.val.size(); ++i) {
        os << node.val[i] << " ";
    }
    return os;
}
