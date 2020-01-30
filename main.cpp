#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

#include "MiddleNode.h"
#include "Node.h"
#include "Point.h"
#include "SuperNode.h"
#include "Tree.h"

using namespace std;

int main()
{
  ifstream fin("line-seg.txt");
  // freopen("output.txt", "w", stdout);

  unsigned int n;
  double xLeft, xRight, yLeft, yRight;
  fin >> n;
  // read lines segments unsorted from a file
  vector<LineSegment> lineSegs(n);
  // lineSegs.reserve(n);
  for (unsigned int i = 0; i < n; i++)
  {
    fin >> xLeft >> yLeft >> xRight >> yRight;
    lineSegs[i].setXLeft(xLeft);
    lineSegs[i].setXRight(xRight);
    lineSegs[i].setYLeft(yLeft);
    lineSegs[i].setYRight(yRight);
  }
  // sort these lines by x-left
  sort(lineSegs.begin(), lineSegs.end(), XLeftLessThan());

  // construct a set of lineSegments ordered by y
  set<LineSegment, YLeftLessThan> lineSegmentsY;
  for (int i = 0; i < lineSegs.size(); i++)
  {
    lineSegmentsY.insert(lineSegs[i]);
  }

  cout << n;

  vector<Node> nodes;
  unsigned int nodesTotal = ceil(n/VAL_SIZE);
  nodes.resize(nodesTotal);
  for (unsigned int i = 0; i < nodesTotal; i++)
  {
    for(unsigned int j = 0; j < VAL_SIZE; j++)
    {
      nodes[i].setIthVal(lineSegs[i*VAL_SIZE + j] , j);
      nodes[i].setIthMinMaxX(lineSegs[i*VAL_SIZE + j].getXLeft(), j);
    }
  }
  for (int i = 0; i < nodesTotal; i++)
  {
    cout << "\n" << nodes[i];
  }

  unsigned int height = ceil(log2(nodes.size()) / log2(CHILD_SIZE)) + 1;
  Tree tree(height);
  tree.buildBottomUpBTree(nodes, true);
  Node root = tree.getRoot();
  cout << "\n:" <<  root;

  return 0;
}
