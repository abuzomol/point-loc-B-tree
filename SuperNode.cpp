//
// Created by muzamil on 1/14/20.
//

#include "SuperNode.h"

SuperNode::SuperNode()
{
  SuperNode::val.resize(VAL_SIZE);
  SuperNode::child.resize(CHILD_SIZE);
  SuperNode::leftSemiLines = nullptr;
  SuperNode::rightSemiLines = nullptr;
  SuperNode::middle = nullptr;
}
SuperNode::SuperNode(const std::vector<double>& val) : val(val) {}

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
const std::vector<SuperNode*>& SuperNode::getChild() const { return child; }
void SuperNode::setChild(const std::vector<SuperNode*>& child)
{
  SuperNode::child = child;
}

void SuperNode::setIthVal(const double& val, const int& i)
{
  SuperNode::val[i] = val;
}
const double& SuperNode::getIthVal(const int& i) const
{
  return SuperNode::val[i];
}

void SuperNode::setIthChild(SuperNode& superNode, const int& i)
{
  SuperNode::child[i] = &superNode;
}

const SuperNode* SuperNode::getIthChild(const int& i)
{
  return SuperNode::child[i];
}

const unsigned int SuperNode::getValSize() const
{
  return SuperNode::val.size();
}

std::ostream& operator<<(std::ostream& os, const SuperNode& superNode)
{
  for (int i = 0; i < superNode.val.size(); ++i)
  {
    os << superNode.val[i] << " ";
  }
}
