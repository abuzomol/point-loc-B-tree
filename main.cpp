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

using namespace std;

using namespace std;

void BuildBottomUpBTree(vector<vector<Node>>& tree, const vector<Node>& nodes);
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
    }
  }
  for (int i = 0; i < nodesTotal; i++)
  {
    cout << "\n" << nodes[i];
  }

  unsigned int height = ceil(log2(nodes.size()) / log2(CHILD_SIZE)) + 1;
  vector<vector<Node>> tree (height) ;

  BuildBottomUpBTree(tree, nodes);

  return 0;
}

void BuildBottomUpBTree(vector<vector<Node>>& tree, const vector<Node>& nodes)
{
  int height = tree.size();
  cout << "\n height: " << height;
  tree[height - 1 ] = nodes;

  for(int i = height - 2; i > -1 ; --i)
  {
    int size = tree[i+1].size() % CHILD_SIZE == 0? tree[i+1].size() / CHILD_SIZE:
               (tree[i+1].size() / CHILD_SIZE )+ 1 ;

    cout << "\nsize: " << size;
    tree[i].resize(size);
  }

}
