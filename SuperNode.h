//
// Created by muzamil on 1/14/20.
//

#ifndef POINT_LOC_B_TREE_SUPERNODE_H
#define POINT_LOC_B_TREE_SUPERNODE_H

#include <iostream>

#include "MiddleNode.h"
#include "Node.h"

class SuperNode
{
 private:
  std::vector<double> val;
  std::vector<SuperNode*> child;
  std::vector<Node*> leftSemiLines;
  std::vector<Node*> rightSemiLines;
  MiddleNode* middle;

 public:
  SuperNode();

  SuperNode(const std::vector<double>& val);

  const std::vector<Node*>& getLeftSemiLines() const;

  void setLeftSemiLines(const std::vector<Node*>& leftSemiLines);

  const std::vector<Node*>& getRightSemiLines() const;

  void setRightSemiLines(const std::vector<Node*>& rightSemiLines);

  const std::vector<SuperNode*>& getChild() const;

  void setChild(const std::vector<SuperNode*>& child);

  const std::vector<double>& getVal() const;

  void setVal(const std::vector<double>& val);

  MiddleNode* getMiddle() const;

  void setMiddle(MiddleNode* middle);

  void setIthVal(const double& x, const int& i);

  const double& getIthVal(const int& i) const;

  void setIthChild(SuperNode& superNode, const int& i);

  SuperNode* getIthChild(const int& i);

  friend std::ostream& operator<<(std::ostream& os, const SuperNode& superNode);

  const unsigned int getValSize() const;

  const Node* getIthLeftSemiLines(int& i);

  void setIthLeftSemiLines(Node& root, int& i);

  const Node* getIthRightSemiLines(int& i);

  void setIthRightSemiLines(Node& root, int& i);
};

std::ostream& operator<<(std::ostream& os, const SuperNode& superNode);

#endif  // POINT_LOC_B_TREE_SUPERNODE_H
