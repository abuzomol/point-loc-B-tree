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
    unsigned int linesSegSize = stoi(argv[1]);
    string fileName = argv[2];
    */
    string fileName = "objects.1K.1583107550.long";
     unsigned int linesSegSize = 1000000;
    //unsigned int linesSegSize = 501;

    // double xLeft, xRight, yLeft, yRight;
    char* memBlock = new char[sizeof(double) * 3];

    vector<LineSegment> lineSegs(linesSegSize);
    // lineSegs.reserve(linesSegSize);

    auto start = std::chrono::high_resolution_clock::now();
    ifstream fin(fileName, ios::binary);

    //read lineSegments from file
    for (unsigned int i = 0; i < linesSegSize; i++)
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

    // get all x coordinates from lineSegments
    vector<double> xValues;
    xValues.resize(2 * linesSegSize);
    for (int i = 0; i < linesSegSize; i++)
    {
        xValues[i << 1] = lineSegs[i].getXLeft();
        xValues[(i << 1) + 1] = lineSegs[i].getXRight();
    }
    // sort xValues.
    sort(xValues.begin(), xValues.end());

    // sort lineSegments ordered by y
    sort(lineSegs.begin(), lineSegs.end(), YLeftLessThan());

    start = std::chrono::high_resolution_clock::now();

    unsigned int superNodesTotal = ceil(xValues.size() / VAL_SIZE);
    unsigned int superHeight =
        ceil(log2(superNodesTotal) / log2(CHILD_SIZE)) + 1;
    SuperTree superTree(superHeight, xValues);
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

    fillSuperTree(superRoot, lineSegments);

    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;

    std::cout << "Filling the tree         : " << elapsed.count() / 1000
              << " ms\n";

    Point point;
    point.setX(4211);
    point.setY(90000);
    start = std::chrono::high_resolution_clock::now();
    auto ans = pointLocationQuery(superRoot, point);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;

    std::cout << "Querying the tree        : " << elapsed.count() / 1000
              << " ms";


    cout << "\n**************************************" << endl;

    // Print information
    int zero = 0, one = 1, two = 2, three = 3, four = 4;
    cout << "Number of lineSegments   : " << lineSegs.size();
    cout << "\nBlock Size               : " << CHILD_SIZE;
    cout << "\nNumber of super leaves   : " << superNodesTotal;
    cout << "\nHeight of SuperTree      : " << superTree.size();
    cout << "\nRoot values              : " << superRoot;
    cout << "\npoint                    : " << point;
    cout << "\nLineSegment              : " << *ans;
    cout << "\n**************************************" << endl;


    return 0;
}
/*
 unsigned int linesSegSize = 501;
point: (4211,90000)
lower bound: 9.0072e+15 at position: 1
lower bound: 5855.59 at position: 0
lower bound: 5013.31 at position: 0
lower bound: 4883.09 at position: 0
lower bound: 4853 at position: 0
LineSegment: (3437.35,6102.96)--(6086.25,6102.96)
*/