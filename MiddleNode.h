//
// Created by muzamil on 1/17/20.
//

#ifndef POINT_LOC_B_TREE_MIDDLENODE_H
#define POINT_LOC_B_TREE_MIDDLENODE_H

#include <vector>
#include <ostream>
#include "LineSegment.h"


class MiddleNode
{
private:
  // Val can store a double for y-value, but the total ordering of lineSegments
  // make it easier to use the lineSegment itself.
  // Hence, we can save the size of double for each node.
  std::vector<LineSegment> val;
  std::vector<MiddleNode*> child;
  std::vector<double> minX;
  std::vector<double> maxX;
public:
  MiddleNode();
  MiddleNode(const std::vector<LineSegment> &val);
  MiddleNode(const std::vector<LineSegment> &val, const std::vector<MiddleNode*> &child,
       std::vector<double> &minX, std::vector<double> &maxX);
  const std::vector<LineSegment> &getVal() const;
  void setVal(const std::vector<LineSegment> &val);
  const std::vector<MiddleNode *> &getChild() const;
  void setChild(const std::vector<MiddleNode *> &child);
  const std::vector<double> &getMinX() const;
  const std::vector<double> &getMaxX() const;
  void setMinX(const std::vector<double> &minX);
  void setMaxX(const std::vector<double> &maxX);

  const MiddleNode* getIthChild(int &i) const;
  void setIthChild(MiddleNode &node,int &i) ;
  friend std::ostream &operator<<(std::ostream &os, const MiddleNode &node);
};

std::ostream &operator<<(std::ostream &os, const MiddleNode &node) ;

#endif // POINT_LOC_B_TREE_MIDDLENODE_H
