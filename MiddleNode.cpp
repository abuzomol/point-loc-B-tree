//
// Created by muzamil on 1/17/20.
//

#include "MiddleNode.h"

MiddleNode::MiddleNode()
{
  MiddleNode::val.resize(VAL_SIZE);
  MiddleNode::child.resize(CHILD_SIZE);
}
MiddleNode::MiddleNode(const std::vector<LineSegment>& val) : val(val)
{
  MiddleNode::child.resize(CHILD_SIZE);
}
MiddleNode::MiddleNode(const std::vector<LineSegment>& val,
                       const std::vector<MiddleNode*>& child,
                       const unsigned int& spannedSlabs)
    : val(val), child(child), spannedSlabs(spannedSlabs)
{
}
const std::vector<LineSegment>& MiddleNode::getVal() const { return val; }
void MiddleNode::setVal(const std::vector<LineSegment>& val)
{
  MiddleNode::val = val;
}
const std::vector<MiddleNode*>& MiddleNode::getChild() const { return child; }
void MiddleNode::setChild(const std::vector<MiddleNode*>& child)
{
  MiddleNode::child = child;
}

void MiddleNode::setIthVal(const LineSegment& lineSegment, const int& i)
{
  MiddleNode::val[i] = lineSegment;
}

const MiddleNode* MiddleNode::getIthChild(int& i) const { return child[i]; }
void MiddleNode::setIthChild(MiddleNode& node, int& i)
{
  MiddleNode::child[i] = &node;
}

const bool MiddleNode::underflow() const
{
  return MiddleNode::getValSize() < MIN_VAL;
}
const unsigned int MiddleNode::getValSize() const
{
  return MiddleNode::val.size();
}
const LineSegment& MiddleNode::getIthVal(const int& i) const
{
  return MiddleNode::val[i];
}
void MiddleNode::setValSize(const unsigned int& valSize)
{
  MiddleNode::val.resize(valSize);
}

void MiddleNode::setSpannedSlabs(unsigned int spannedSlabs) {MiddleNode::spannedSlabs = spannedSlabs; }
std::ostream& operator<<(std::ostream& os, const MiddleNode& node)
{
  for (int i = 0; i < node.val.size(); ++i)
  {
    os << node.val[i] << " ";
  }
  return os;
}

