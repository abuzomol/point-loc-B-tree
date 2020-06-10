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
    string fileName = "/Users/muzamil/Desktop/point-location/objects.100M.1583107550.random";

     unsigned int linesSegSize;
     linesSegSize = 10000000;


    //unsigned int linesSegSize = 501;

    // double xLeft, xRight, yLeft, yRight;
    char* lineBlock = new char[sizeof(double) * 3];

    vector<LineSegment> lineSegs(linesSegSize);
    // lineSegs.reserve(linesSegSize);

    auto start = std::chrono::high_resolution_clock::now();
    ifstream finLines(fileName, ios::binary);

    //read lineSegments from file
    for (unsigned int i = 0; i < lineSegs.size(); i++)
    {
        // fin >> xLeft >>  xRight >> yLeft;

        double* doubleBlock = (double*)lineBlock;

        finLines.read(lineBlock, sizeof(double) * 3);
        lineSegs[i].setXLeft(doubleBlock[0]);
        lineSegs[i].setXRight(doubleBlock[1]);
        lineSegs[i].setYLeft(doubleBlock[2]);
        lineSegs[i].setYRight(doubleBlock[2]);
    }
    finLines.close();

    auto finish = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Time to read lines       : " << elapsed.count()
              << " s\n";

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
        ceil(log(superNodesTotal) / log(CHILD_SIZE)) + 1;
    SuperTree superTree(superHeight, xValues);
    SuperNode superRoot = superTree.getRoot();

    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;

    std::cout << "Constructing the super tree    : " << elapsed.count()
              << " s\n";

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

    std::cout << "Filling the super tree         : " << elapsed.count()
              << " s\n";

    unsigned int pointsSize ;
    //vector<int> pointsSizes ;
    vector<int> pointsSizes = {500000,1000000, 2000000,5000000,10000000};
    fileName = "/Users/muzamil/Desktop/point-location/objects.100M.1583107551.random";

    for(int t = 0 ; t < pointsSizes.size() ; t++)
    {
        // double xLeft, xRight, yLeft, yRight;
        char* pointBlock = new char[sizeof(double) * 2];

        pointsSize = pointsSizes[t];
        vector<Point> points(pointsSize);
        ifstream finPoints(fileName, ios::binary);

        start = std::chrono::high_resolution_clock::now();

        //read lineSegments from file
        for (unsigned int i = 0; i < points.size(); i++)
        {
            // fin >> xLeft >>  xRight >> yLeft;

            double* doubleBlock = (double*)pointBlock;

            finPoints.read(pointBlock, sizeof(double) * 2);
            points[i].setX(doubleBlock[1] * -1);
            points[i].setY(doubleBlock[0] * -1);
        }
        finPoints.close();

        finish = std::chrono::high_resolution_clock::now();
        elapsed = finish - start;
        std::cout << "\nTime to read points       : " << elapsed.count()
                  << " s";
        vector<LineSegment> ans(points.size());

        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < points.size(); i++)
        {
            ans[i] = pointLocationQuery(superRoot, points[i]);
        }
        finish = std::chrono::high_resolution_clock::now();
        elapsed = finish - start;
        std::cout << "\nQuerying the tree        : " << elapsed.count() << " s";
        cout << "\nNumber of queries        : " << points.size();
        //free(&points);
    }

    cout << "\n**************************************" ;

    cout << "\nNumber of lineSegments   : " << lineSegs.size();
    //cout << "\nNumber of queries        : " << points.size();
    cout << "\nBranching factor         : " << CHILD_SIZE;
    cout << "\nNumber of super leaves   : " << superNodesTotal;
    cout << "\nHeight of SuperTree      : " << superTree.size();
    cout << "\nRoot values              : " << superRoot;
    //cout << "\npoint                    : " << point;
    //cout << "\nLineSegment              : " << *ans;
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
