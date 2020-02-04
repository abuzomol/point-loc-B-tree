//
// Created by muzamil on 1/14/20.
//

#ifndef POINT_LOC_B_TREE_SUPERNODE_H
#define POINT_LOC_B_TREE_SUPERNODE_H
#include <iostream>
#include <vector>
#include <set>
#include "MiddleNode.h"
#include "Node.h"
#include "MiddleNode.h"
#include "Node.h"

class SuperNode
{
 private:
  std::vector<double> val;
  std::vector<SuperNode*> child;
  Node* leftSemiLines ;
  Node *rightSemiLines;
  MiddleNode *middle;

 public:
  SuperNode ();
  SuperNode(const std::vector<double>& val);
  const std::vector<SuperNode*>& getChild() const;
  void setChild(const std::vector<SuperNode*>& child);
  const std::vector<double>& getVal() const;
  void setVal(const std::vector<double>& val);

  Node* getLeftSemiLines() const;
  void setLeftSemiLines(Node* leftSemiLines);
  Node* getRightSemiLines() const;
  void setRightSemiLines(Node* rightSemiLines);
  MiddleNode* getMiddle() const;
  void setMiddle(MiddleNode* middle);
  void setIthVal(const double & x, const int& i);
  const double& getIthVal(const int & i)const;

  void setIthChild(SuperNode& superNode, const int & i );
  const SuperNode* getIthChild(const int & i);

  friend std::ostream& operator<<(std::ostream& os, const SuperNode& superNode);
  const unsigned int getValSize() const;
};

std::ostream& operator<<(std::ostream& os, const SuperNode& superNode);

#endif // POINT_LOC_B_TREE_SUPERNODE_H
