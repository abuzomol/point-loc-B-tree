#include <algorithm>
#include <iostream>
#include <set>
#include <vector>
#include <fstream>
#include <cmath>
#include "Node.h"
#include "Point.h"
#include "SuperNode.h"
#include "MiddleNode.h"

using namespace std;

int main() {
  ifstream fin("line-seg.txt");
  // freopen("output.txt", "w", stdout);

  unsigned int n;
  double xLeft, xRight, yLeft, yRight;
  fin >> n;
  // read lines segments unsorted from a file
  vector<LineSegment> lineSegs(n);
  //lineSegs.reserve(n);
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

  cout << n ;
  for(int i = 0 ; i < n ; i++)
  {
    cout <<"\n" << lineSegs[i];
  }

  vector<Node> nodes;
  nodes.reserve(n);
  for(int i = 0 ; i< n; i++)
  {
    nodes[i].setIthChild();
  }

  return 0;
}
