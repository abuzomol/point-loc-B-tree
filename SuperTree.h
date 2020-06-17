//
// Created by muzamil on 1/31/20.
//

#ifndef POINT_LOC_B_TREE_SUPERTREE_H
#define POINT_LOC_B_TREE_SUPERTREE_H

#include "Point.h"
#include "SuperNode.h"

class SuperTree
{
 private:
  std::vector<std::vector<SuperNode> > superTree;

 public:
  SuperTree(const int& height);
  SuperTree(const int& height, const std::vector<double>& xValues);
  const vector<std::vector<SuperNode>>& getSuperTree() const;
  const SuperNode& getRoot() const;
  const int size() const;
};

void fillSuperTree(SuperNode& superRoot,
                   std::vector<LineSegment>& lineSegments);

const LineSegment& pointLocationQuery(SuperNode& superRoot, Point& point);
const LineSegment& pointLocationMiddle(const MiddleNode* middleRoot, Point& point, int& index);
const LineSegment& pointLocationLeft(const Node* root, Point& point);
const LineSegment& pointLocationRight(const Node* root, Point& point);
#endif  // POINT_LOC_B_TREE_SUPERTREE_H
