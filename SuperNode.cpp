//
// Created by muzamil on 1/14/20.
//

#include "SuperNode.h"

#include <utility>

SuperNode::SuperNode()
{
    SuperNode::val.resize(VAL_SIZE);
    SuperNode::child.resize(CHILD_SIZE);
    SuperNode::leftSemiLines.resize(VAL_SIZE);
    SuperNode::rightSemiLines.resize(VAL_SIZE);
    SuperNode::middle = nullptr;
}

SuperNode::SuperNode(std::vector<double>& val)
    : val(std::move(val)), middle(nullptr)
{
    SuperNode::child.resize(CHILD_SIZE);
    SuperNode::leftSemiLines.resize(VAL_SIZE);
    SuperNode::rightSemiLines.resize(VAL_SIZE);
}
SuperNode::SuperNode(std::vector<double>& val, std::vector<SuperNode*>& child)
    : val(std::move(val)), child(std::move(child)),middle(nullptr)
{
    SuperNode::leftSemiLines.resize(VAL_SIZE);
    SuperNode::rightSemiLines.resize(VAL_SIZE);
}

const std::vector<double>& SuperNode::getVal() const { return val; }

void SuperNode::setVal(const std::vector<double>& val) { SuperNode::val = val; }

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

SuperNode* SuperNode::getIthChild(const int& i) { return SuperNode::child[i]; }

const unsigned int SuperNode::getValSize() const
{
    return SuperNode::val.size();
}

const Node* SuperNode::getIthLeftSemiLines(int& i)
{
    return SuperNode::leftSemiLines[i];
}

void SuperNode::setIthLeftSemiLines(Node& root, int& i)
{
    SuperNode::leftSemiLines[i] = &root;
}

const Node* SuperNode::getIthRightSemiLines(int& i)
{
    return SuperNode::rightSemiLines[i];
}

void SuperNode::setIthRightSemiLines(Node& root, int& i)
{
    SuperNode::rightSemiLines[i] = &root;
}

std::ostream& operator<<(std::ostream& os, const SuperNode& superNode)
{
    for (int i = 0; i < superNode.val.size(); ++i)
    {
        os << superNode.val[i] << " ";
    }
    return os;
}

const std::vector<Node*>& SuperNode::getLeftSemiLines() const
{
    return SuperNode::leftSemiLines;
}

void SuperNode::setLeftSemiLines(const std::vector<Node*>& leftSemiLines)
{
    SuperNode::leftSemiLines = leftSemiLines;
}

const std::vector<Node*>& SuperNode::getRightSemiLines() const
{
    return SuperNode::rightSemiLines;
}

void SuperNode::setRightSemiLines(const std::vector<Node*>& rightSemiLines)
{
    SuperNode::rightSemiLines = rightSemiLines;
}
