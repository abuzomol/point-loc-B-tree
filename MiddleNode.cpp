//
// Created by muzamil on 1/17/20.
//

#include "MiddleNode.h"

#include "Node.h"
MiddleNode::MiddleNode() {
  MiddleNode::val.reserve(Val_SIZE);
  MiddleNode::child.reserve(CHILD_SIZE);
  MiddleNode::minX.reserve(CHILD_SIZE);
  MiddleNode::maxX.reserve(CHILD_SIZE);
}
MiddleNode::MiddleNode(const std::vector<LineSegment> &val) : val(val) {
  MiddleNode::child.reserve(CHILD_SIZE);
  MiddleNode::minX.reserve(CHILD_SIZE);
  MiddleNode::maxX.reserve(CHILD_SIZE);
}
MiddleNode::MiddleNode(const std::vector<LineSegment> &val,
                       const std::vector<MiddleNode *> &child,
                       std::vector<double> &minX, std::vector<double> &maxX)
    : val(val), child(child), minX(minX), maxX(maxX) {}
const std::vector<LineSegment> &MiddleNode::getVal() const { return val; }
void MiddleNode::setVal(const std::vector<LineSegment> &val) {
  MiddleNode::val = val;
}
const std::vector<MiddleNode *> &MiddleNode::getChild() const { return child; }
void MiddleNode::setChild(const std::vector<MiddleNode *> &child) {
  MiddleNode::child = child;
}
const std::vector<double> &MiddleNode::getMinX() const { return minX; }
void MiddleNode::setMinX(const std::vector<double> &minX) {
  MiddleNode::minX = minX;
}
const std::vector<double> &MiddleNode::getMaxX() const { return maxX; }
void MiddleNode::setMaxX(const std::vector<double> &maxX) {
  MiddleNode::maxX = maxX;
}
const MiddleNode *MiddleNode::getIthChild(int &i) const { return child[i]; }
void MiddleNode::setIthChild(MiddleNode &node, int &i) {
  MiddleNode::child[i] = &node;
}

std::ostream &operator<<(std::ostream &os, const MiddleNode &node) {
  for (int i = 0; i < node.val.size(); ++i) {
    os << node.val[i] << " ";
  }
}