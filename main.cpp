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

#include <chrono>
#include "Tree.h"

using namespace std;

int main()
{
  ifstream fin("objects.1K.1583107550.long" , ios::binary);

  unsigned int n=1000;
  double xLeft, xRight, yLeft, yRight;
  char* memBlock  = new char[sizeof(double) *3];
  //fin >> n;
  // read lines segments unsorted from a file

  vector<LineSegment> lineSegs(n);
  // lineSegs.reserve(n);

  auto start = std::chrono::high_resolution_clock::now();

  for (unsigned int i = 0; i < n; i++)
  {
    //fin >> xLeft >>  xRight >> yLeft;
    fin.read( (char*) memBlock, sizeof(double)*3 );
    double* doubleBlock = (double*) memBlock;
    lineSegs[i].setXLeft(doubleBlock[0]);
    lineSegs[i].setXRight(doubleBlock[1]);
    lineSegs[i].setYLeft(doubleBlock[2]);
    lineSegs[i].setYRight(doubleBlock[2]);
  }

  auto finish = std::chrono::high_resolution_clock::now();
  fin.close();

  std::chrono::duration<double> elapsed = finish - start;
  std::cout << "Time to read File        : " << elapsed.count() << " s\n";

  // sort these lines by x-left
  sort(lineSegs.begin(), lineSegs.end(), XLeftLessThan());

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

  // sort lineSegments ordered by y
  sort(lineSegs.begin(), lineSegs.end(), YLeftLessThan());

  // create the leaves nodes first, N/B nodes
  start = std::chrono::high_resolution_clock::now();
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
  /*
  for (int i = 0; i < nodesTotal; i++)
  {
    cout << "\n" << nodes[i];
  }*/
  // construct the leaves of the superTree;
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
  SuperTree superTree(superHeight, superNodes);
  SuperNode superRoot = superTree.getRoot();
  finish = std::chrono::high_resolution_clock::now();
  elapsed = finish - start;
  std::cout << "Constructing the tree    : " << elapsed.count() << " s\n";

  start = std::chrono::high_resolution_clock::now();
  fillSuperTree(superRoot, lineSegs);
  finish = std::chrono::high_resolution_clock::now();
  elapsed = finish - start;
  std::cout << "Filling the tree         : " << elapsed.count() << " s\n";
  cout <<"**************************************" <<endl;
  //Print information
  int zero = 0, one = 1, two = 2 ,three = 3, four = 4;
  cout << "Number of lineSegments   : " << lineSegs.size();
  cout << "\nBlock Size               : " << CHILD_SIZE;
  cout << "\nNumber of leaves         : " << nodesTotal;
  cout << "\nHeight of SuperTree      : " << superTree.size();
  cout << "\nRoot values              : " <<superRoot;
 /* cout << "\nLeftTree[0] root         : " <<*superRoot.getIthLeftSemiLines(zero);
  cout << "\nLeftTree[1] root         : " <<*superRoot.getIthLeftSemiLines(one);
  cout << "\nLeftTree[2] root         : " <<*superRoot.getIthLeftSemiLines(two);
  cout << "\nLeftTree[3] root         : " <<*superRoot.getIthLeftSemiLines(three);
  cout << "\nRightTree[0] root         : " <<*superRoot.getIthRightSemiLines(zero);
  cout << "\nRightTree[1] root         : " <<*superRoot.getIthRightSemiLines(one);
  cout << "\nRightTree[2] root         : " <<*superRoot.getIthRightSemiLines(two);
  cout << "\nRightTree[3] root         : " <<*superRoot.getIthRightSemiLines(three);
*/
  return 0;
}
