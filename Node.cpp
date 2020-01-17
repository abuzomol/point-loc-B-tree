//
// Created by muzamil on 1/14/20.
//

#include "Node.h"
Node::Node(){Node::val.reserve(B_SIZE);}
Node::Node(){Node::val.reserve(B_SIZE);}
Node::Node(const std::vector<LineSegment> &val) : val(val) {}
const std::vector<LineSegment> &Node::getVal() const { return val; }
void Node::setVal(const std::vector<LineSegment> &val) { Node::val = val; }
const std::vector<Node *> &Node::getChild() const { return child; }
void Node::setChild(const std::vector<Node *> &child) { Node::child = child; }
const std::vector<double> &Node::getMinMaxX() const { return minMaxX; }
void Node::setMinMaxX(const std::vector<double> &minMaxX) {
  Node::minMaxX = minMaxX;
}
const Node* Node::getIthChild(int &i) const
{
  return child[i];
}
void Node::setIthChild(Node &node , int &i){ Node::child[i] = &node; }

std::ostream &operator<<(std::ostream &os, const Node &node)
{
  for(int i = 0 ; i < node.val.size(); ++i)
  {
    os << node.val[i] << " ";
  }
}