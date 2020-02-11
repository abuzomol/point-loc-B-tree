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
#include "SuperTree.h"
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

  // get all x coordinates from lineSegments
  vector<double> xValues;
  xValues.resize(2 * n);
  for (int i = 0; i < n; i++)
  {
    xValues[i << 1] = lineSegs[i].getXLeft();
    xValues[(i << 1) + 1] = lineSegs[i].getXRight();
  }
  // sort xValues.
  sort(xValues.begin(), xValues.end());
  // create the leaves nodes first
  vector<Node> nodes;
  unsigned int nodesTotal = ceil(n / VAL_SIZE);
  nodes.resize(nodesTotal);
  for (unsigned int i = 0; i < nodesTotal; i++)
  {
    for (unsigned int j = 0; j < VAL_SIZE; j++)
    {
      nodes[i].setIthVal(lineSegs[i * VAL_SIZE + j], j);
      nodes[i].setIthMinMaxX(lineSegs[i * VAL_SIZE + j].getXLeft(), j);
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
  cout << "\n:" << root;

  // construct the leaves of the superTree;
  vector<SuperNode> superNodes;
  unsigned int superNodesTotal = ceil(2 * n / VAL_SIZE);
  superNodes.resize(superNodesTotal);
  for (unsigned int i = 0; i < superNodesTotal; i++)
  {
    for (unsigned int j = 0; j < VAL_SIZE; j++)
    {
      superNodes[i].setIthVal(xValues[i * VAL_SIZE + j], j);
    }
  }

  unsigned int superHeight =
      ceil(log2(superNodes.size()) / log2(CHILD_SIZE)) + 1;
  SuperTree superTree(superHeight);
  superTree.buildBottomUpBTree(superNodes);
  SuperNode superRoot = superTree.getRoot();
  cout << "\n:" << superRoot;
  fillSuperTree(superRoot, lineSegmentsY);
       return 0;
}
