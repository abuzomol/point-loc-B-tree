//
// Created by muzamil on 1/14/20.
//

#include "SuperNode.h"
SuperNode::SuperNode(const std::vector<double>& val): val(val) {}

const std::vector<double>& SuperNode::getVal() const { return val; }
void SuperNode::setVal(const std::vector<double>& val) { SuperNode::val = val; }
Node* SuperNode::getLeftSemiLines() const { return leftSemiLines; }
void SuperNode::setLeftSemiLines(Node* leftSemiLines)
{
  SuperNode::leftSemiLines = leftSemiLines;
}
Node* SuperNode::getRightSemiLines() const { return rightSemiLines; }
void SuperNode::setRightSemiLines(Node* rightSemiLines)
{
  SuperNode::rightSemiLines = rightSemiLines;
}
MiddleNode* SuperNode::getMiddle() const { return middle; }
void SuperNode::setMiddle(MiddleNode* middle) { SuperNode::middle = middle; }
