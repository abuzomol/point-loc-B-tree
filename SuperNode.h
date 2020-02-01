//
// Created by muzamil on 1/14/20.
//

#ifndef POINT_LOC_B_TREE_SUPERNODE_H
#define POINT_LOC_B_TREE_SUPERNODE_H
#include <iostream>
#include <vector>

#include "MiddleNode.h"
#include "Node.h"
#include "MiddleNode.h"
#include "Node.h"

class SuperNode
{
 private:
  std::vector<double> val;
  Node *leftSemiLines ;
  Node *rightSemiLines;
  MiddleNode *middle;

 public:
  SuperNode(const std::vector<double>& val);
  const std::vector<double>& getVal() const;
  void setVal(const std::vector<double>& val);

  Node* getLeftSemiLines() const;
  void setLeftSemiLines(Node* leftSemiLines);
  Node* getRightSemiLines() const;
  void setRightSemiLines(Node* rightSemiLines);
  MiddleNode* getMiddle() const;
  void setMiddle(MiddleNode* middle);
};


#endif // POINT_LOC_B_TREE_SUPERNODE_H
