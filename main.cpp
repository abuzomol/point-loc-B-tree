#include <algorithm>
#include <chrono>
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
    /*
    if (argc < 2 || argc > 6) {
        cout << "Usage: " << argv[0] << "<number_of_segments> <file_name>" <<
    endl; cout.flush(); return -1;
    }
    unsigned int n = stoi(argv[1]);
    string fileName = argv[2];
    */
    string fileName = "objects.1K.1583107550.long";
    // unsigned int n = 10000000;
    unsigned int n = 10000000;

    // double xLeft, xRight, yLeft, yRight;
    char* memBlock = new char[sizeof(double) * 3];

    vector<LineSegment> lineSegs(n);
    // lineSegs.reserve(n);

    auto start = std::chrono::high_resolution_clock::now();
    ifstream fin(fileName, ios::binary);
    for (unsigned int i = 0; i < n; i++)
    {
        // fin >> xLeft >>  xRight >> yLeft;

        double* doubleBlock = (double*)memBlock;

        fin.read(memBlock, sizeof(double) * 3);
        lineSegs[i].setXLeft(doubleBlock[0]);
        lineSegs[i].setXRight(doubleBlock[1]);
        lineSegs[i].setYLeft(doubleBlock[2]);
        lineSegs[i].setYRight(doubleBlock[2]);
    }
    fin.close();
    auto finish = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Time to read File        : " << elapsed.count() / 1000
              << " ms\n";

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
    vector<SuperNode*> superNodes;
    unsigned int superNodesTotal = ceil(2 * n / VAL_SIZE);
    superNodes.resize(superNodesTotal);
    for (unsigned int i = 0; i < superNodesTotal; i++)
    {
        superNodes[i] = new SuperNode();
        for (unsigned int j = 0; j < VAL_SIZE; j++)
        {
            superNodes[i]->setIthVal(xValues[i * VAL_SIZE + j], j);
        }
    }

    unsigned int superHeight =
        ceil(log2(superNodes.size()) / log2(CHILD_SIZE)) + 1;
    SuperTree superTree(superHeight, superNodes);
    SuperNode superRoot = superTree.getRoot();
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << "Constructing the tree    : " << elapsed.count() / 1000
              << " ms\n";

    // fill in itermediate <lineSeg*>
    vector<LineSegment*> lineSegments;
    for (auto& lineSeg : lineSegs)
    {
        lineSegments.push_back(&lineSeg);
    }

    start = std::chrono::high_resolution_clock::now();

    // fillSuperTree(superRoot, lineSegments);

    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;

    std::cout << "Filling the tree         : " << elapsed.count() / 1000
              << " ms\n";
    cout << "**************************************" << endl;

    // Print information
    int zero = 0, one = 1, two = 2, three = 3, four = 4;
    cout << "Number of lineSegments   : " << lineSegs.size();
    cout << "\nBlock Size               : " << CHILD_SIZE;
    cout << "\nNumber of super leaves   : " << nodesTotal;
    cout << "\nHeight of SuperTree      : " << superTree.size();
    cout << "\nRoot values              : " << superRoot;
    /* cout << "\nLeftTree[0] root         : "
     <<*superRoot.getIthLeftSemiLines(zero); cout << "\nLeftTree[1] root : "
     <<*superRoot.getIthLeftSemiLines(one); cout << "\nLeftTree[2] root : "
     <<*superRoot.getIthLeftSemiLines(two); cout << "\nLeftTree[3] root : "
     <<*superRoot.getIthLeftSemiLines(three); cout << "\nRightTree[0] root : "
     <<*superRoot.getIthRightSemiLines(zero); cout << "\nRightTree[1] root : "
     <<*superRoot.getIthRightSemiLines(one); cout << "\nRightTree[2] root : "
     <<*superRoot.getIthRightSemiLines(two); cout << "\nRightTree[3] root : "
     <<*superRoot.getIthRightSemiLines(three);
   */
    cout << "\n**************************************" << endl;
    Point point;
    point.setX(4211);
    point.setY(90000);
    cout << "point: " << point;
    auto ans = pointLocationQuery(superRoot, point);
    cout << "\nLineSegment: " << *ans;
    return 0;
}
