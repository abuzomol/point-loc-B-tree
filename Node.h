//
// Created by muzamil on 1/14/20.
//

#ifndef POINT_LOC_B_TREE_NODE_H
#define POINT_LOC_B_TREE_NODE_H

#include "LineSegment.h"
#include "global_const.h"
#include <algorithm>
#include <ostream>
#include <vector>

class Node
{
private:
  // Val can store a double for y-value, but the total ordering of lineSegments
  // make it easier to use the lineSegment itself.
  // Hence, we can save the size of double for each node.
  std::vector<LineSegment> val;
  std::vector<Node*> child;
  std::vector<double> minMaxX;
public:
  Node();
  Node(const std::vector<LineSegment> &val);
  const std::vector<LineSegment> &getVal() const;
  void setVal(const std::vector<LineSegment> &val);
  const std::vector<Node *> &getChild() const;
  void setChild(const std::vector<Node *> &child);
  const std::vector<double> &getMinMaxX() const;
  void setMinMaxX(const std::vector<double> &minMaxX);

  const Node* getIthChild(int &i) const;
  void setIthChild(Node &node,int &i) ;
  friend std::ostream &operator<<(std::ostream &os, const Node &node);
};

std::ostream &operator<<(std::ostream &os, const Node &node) ;

#endif // POINT_LOC_B_TREE_NODE_H
