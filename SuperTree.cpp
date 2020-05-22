//
// Created by muzamil on 1/31/20.
//

#include "SuperTree.h"

#include <cmath>

#include "MiddleTree.h"
#include "Tree.h"

using namespace std;

SuperTree::SuperTree(const int& height) { superTree.resize(height); }

SuperTree::SuperTree(const int& height, const std::vector<double>& xValues)
{
    superTree.resize(height);

    // construct the leaves of the superTree
    unsigned int superNodesTotal = ceil(xValues.size() / VAL_SIZE);
    SuperTree::superTree[height-1].resize(superNodesTotal);
    for (unsigned int i = 0; i < superNodesTotal; i++)
    {
        for (unsigned int j = 0; j < VAL_SIZE; j++)
        {
            if (i*VAL_SIZE + j < xValues.size())
            {
                SuperTree::superTree[height-1][i].setIthVal(xValues[i * VAL_SIZE + j], j);
            }
        }
    }
    //fill the last node with infinities if needed
    if (xValues.size() % VAL_SIZE != 0)
    {
        int unfilled = VAL_SIZE - (xValues.size() % VAL_SIZE);
        for (int i = 0; i < unfilled; i++)
        {
            SuperTree::superTree[height-1][superNodesTotal-1].setIthVal(
                INFTY, VAL_SIZE - i - 1);
        }
    }

    // go over every level in the tree
    for (int i = height - 2; i > -1; --i)
    {
        int size = SuperTree::superTree[i + 1].size() % CHILD_SIZE == 0
                       ? SuperTree::superTree[i + 1].size() / CHILD_SIZE
                       : (SuperTree::superTree[i + 1].size() / CHILD_SIZE) + 1;

        SuperTree::superTree[i].resize(size);
        // go over every node in each level
        for (int j = 0; j < SuperTree::superTree[i].size(); j++)
        {
            // set up the children
            for (int k = 0; k < CHILD_SIZE; k++)
            {
                if (j * CHILD_SIZE + k < SuperTree::superTree[i + 1].size())
                {
                    SuperTree::superTree[i][j].setIthChild(
                        SuperTree::superTree[i + 1][j * CHILD_SIZE + k], k);

                    // set up the values (skip every node with index multiple of CHILD_SIZE)
                    if (k - 1 > -1  )
                    {
                        SuperTree::superTree[i][j].setIthVal(
                            SuperTree::superTree[i + 1][j * CHILD_SIZE + k - 1]
                                .getVal()[VAL_SIZE - 1],
                            k-1);
                    }
                }
            }
        }
        //fill the last node with infinities if needed
        if (SuperTree::superTree[i + 1].size() % CHILD_SIZE != 0)
        {
            int filled = SuperTree::superTree[i + 1].size() % CHILD_SIZE ;
            if (filled - 1 > -1 )
            {
                for (int j = filled -1 ; j < VAL_SIZE; j++)
                {
                    SuperTree::superTree[i][size - 1].setIthVal(INFTY, j);
                }
            }
        }
    }
}

// check superRoot is not null

void partitionLineSegments(SuperNode& superRoot,
                           vector<LineSegment*>& lineSegments,
                           vector<vector<LineSegment>*>* left,
                           vector<vector<LineSegment>*>* right,
                           vector<LineSegment>* middle,
                           vector<vector<LineSegment*>*>* remainingLineSegments)
{
    for (int i = 0; i < superRoot.getValSize(); i++)
    {
        // for loop that scan every line segment
        for (auto& lineSegment : lineSegments)
        {
            // if the lineSegment crosses or touches boundary i
            if (lineSegment->getXLeft() <= superRoot.getIthVal(i)
                && lineSegment->getXRight() >= superRoot.getIthVal(i))
            {
                // to the left of first boundary
                if (i == 0 && lineSegment->getXLeft() < superRoot.getIthVal(i))
                {
                    (*left)[i]->push_back(*lineSegment);
                }
                // starts at slab i-1
                if (i > 0
                    && superRoot.getIthVal(i - 1) < lineSegment->getXLeft()
                    && lineSegment->getXLeft() < superRoot.getIthVal(i))
                {
                    (*left)[i]->push_back(*lineSegment);
                }
                // to the right of last boundary

                if (i == superRoot.getValSize() - 1
                    && lineSegment->getXRight() > superRoot.getIthVal(i))
                {
                    (*right)[i]->push_back(*lineSegment);
                }

                // ends at slab i
                if (i < superRoot.getValSize() - 1
                    && lineSegment->getXRight() < superRoot.getIthVal(i + 1)
                    && lineSegment->getXRight() > superRoot.getIthVal(i))
                {
                    (*right)[i]->push_back(*lineSegment);
                }
                // case lineSegment crosses slab i
                if (i < superRoot.getValSize() - 1
                    && lineSegment->getXRight() >= superRoot.getIthVal(i + 1))
                {
                    // check this for pointers
                    if (find(middle->begin(), middle->end(), *lineSegment)
                        == middle->end())
                    {
                        middle->push_back(*lineSegment);
                    }
                }
            }
            // case it does not cross any boundary
            else
            {
                // case it ends before first boundary
                if (i == 0 && lineSegment->getXRight() < superRoot.getIthVal(i))
                {
                    (*remainingLineSegments)[i]->push_back(lineSegment);
                }
                // case it starts after last boundary
                if (i == superRoot.getValSize() - 1
                    && lineSegment->getXLeft() > superRoot.getIthVal(i))
                {
                    (*remainingLineSegments)[i + 1]->push_back(lineSegment);
                }
                // case it is between two boundaries i and i +1 i.e. b_i < left
                // x < b_i+1

                if (i > 0
                    && lineSegment->getXLeft() > superRoot.getIthVal(i - 1)
                    && lineSegment->getXRight() < superRoot.getIthVal(i))
                {
                    (*remainingLineSegments)[i]->push_back(lineSegment);
                }
            }
        }
    }
}

void constructLeftTrees(SuperNode& superRoot,
                        vector<vector<LineSegment>*>* left)
{
    for (int k = 0; k < superRoot.getValSize(); k++)
    {
        // case there are some lineSegments in the set left[k]
        if (!(*left)[k]->empty())
        {
            unsigned int height =
                ceil(log2((*left)[k]->size()) / log2(CHILD_SIZE)) + 1;
            Tree* tree = new Tree(height,  *(*left)[k], true);
            Node* root = tree->getRoot();
            superRoot.setIthLeftSemiLines(*root, k);
        }
    }
}

void constructRightTrees(SuperNode& superRoot,
                         vector<vector<LineSegment>*>* right)
{
    for (int k = 0; k < superRoot.getValSize(); k++)
    {
        // case there are some lineSegments in the set right[k]
        if (!(*right)[k]->empty())
        {
            // create the leaves nodes first
            unsigned int height =
                ceil(log2((*right)[k]->size()) / log2(CHILD_SIZE)) + 1;
            Tree* tree = new Tree(height, *(*right)[k], false);
            Node* root = tree->getRoot();
            superRoot.setIthRightSemiLines(*root, k);
        }
    }
}

void constructMiddleTree(SuperNode& superRoot, vector<LineSegment>* middle)
{
    unsigned int middleNodesTotal = ceil(middle->size() * 1.0 / VAL_SIZE);

    unsigned int height =
        ceil(log2(middle->size()) / log2(CHILD_SIZE)) + 1;
    MiddleTree* middleTree = new MiddleTree(height, superRoot,*middle);
    MiddleNode* middleRoot = middleTree->getRoot();
    superRoot.setMiddle(middleRoot);
}

void fillSuperTree(SuperNode& superRoot, vector<LineSegment*>& lineSegments)
{
    auto* left = new vector<vector<LineSegment>*>(superRoot.getValSize());
    auto* right = new vector<vector<LineSegment>*>(superRoot.getValSize());
    // initialize the middle in the heap. Middle is connected to SuperNode
    // parent.
    auto* middle = new vector<LineSegment>();
    // initialize the remainingLineSegments that left out from left, right, and
    // middle
    auto* remainingLineSegments =
        new vector<vector<LineSegment*>*>(superRoot.getValSize() + 1);

    for (int i = 0; i < superRoot.getValSize(); i++)
    {
        (*left)[i] = new vector<LineSegment>();
        (*right)[i] = new vector<LineSegment>();
        (*remainingLineSegments)[i] = new vector<LineSegment*>();
    }

    // initialize the last child to an empty vector
    (*remainingLineSegments)[superRoot.getValSize()] =
        new vector<LineSegment*>();

    //partition lineSegments into left, middle and right and remainingLineSegments
    partitionLineSegments(
        superRoot, lineSegments, left, right, middle, remainingLineSegments);

    // construct the left B-trees for each value of superRoot
    if (!left->empty())
    {
        constructLeftTrees(superRoot, left);
    }
    // construct the right B-trees for each value of superRoot
    if (!right->empty())
    {
        constructRightTrees(superRoot, right);
    }
    // construct the middle B-trees
    if (!middle->empty())
    {
        constructMiddleTree(superRoot, middle);  // ERROR HERE
    }
    // recursive call for fill in the tree with remaining lineSegments
    for (int i = 0; i < remainingLineSegments->size(); i++)
    {
        if (!(*remainingLineSegments)[i]->empty()
            && superRoot.getIthChild(i) != nullptr)
        {
            fillSuperTree(*superRoot.getIthChild(i),
                          *((*remainingLineSegments)[i]));
        }
    }
}

const int SuperTree::size() const { return SuperTree::superTree.size(); }

const SuperNode& SuperTree::getRoot() const { return superTree[0][0]; }

const LineSegment* pointLocationQuery(SuperNode& superRoot, Point& point)
{
    const LineSegment* middleSegment = new LineSegment();
    const LineSegment* leftSegment = new LineSegment();
    const LineSegment* rightSegment = new LineSegment();
    const LineSegment* recursiveSegment = new LineSegment();

    //get the lower bound of x-values
    auto low = lower_bound(
        superRoot.getVal().begin(), superRoot.getVal().end(), point.getX());

    int index;
    if (low != superRoot.getVal().end())
        index = low - superRoot.getVal().begin();
    else
        index = superRoot.getValSize();

    //cout << "\nlower bound: " << *low << " at position: " << index;

    // get the middleSegment if found
    if (superRoot.getMiddle() != nullptr)
    {
        middleSegment = pointLocationMiddle(superRoot.getMiddle(), point , index);
    }

    // case the x value equals a boundary
    if (superRoot.getIthVal(index) == point.getX())
    {
        return middleSegment;
    }
    else
    {
        // get the leftSegment
        if (index < superRoot.getValSize()
            && superRoot.getIthLeftSemiLines(index) != nullptr)
        {
            leftSegment =
                pointLocationLeft(superRoot.getIthLeftSemiLines(index), point);
        }
        // get the rightSegment from previous index if not right border
        if (0 < index < superRoot.getValSize()
            && superRoot.getIthRightSemiLines(index) != nullptr)
        {
            index--;
            rightSegment =
                pointLocationRight(superRoot.getIthRightSemiLines(index), point);
            index++;
        }
        else  // if right border
        {
            rightSegment =
                pointLocationRight(superRoot.getIthRightSemiLines(index), point);
        }
        // get the recursive segment
        if (superRoot.getIthChild(index) != nullptr)
        {
            recursiveSegment =
                pointLocationQuery(*superRoot.getIthChild(index), point);
        }
    }
    return &max(*leftSegment,
                max(*middleSegment,
                    max(*rightSegment, *recursiveSegment, YLeftLessThan()),
                    YLeftLessThan()),
                YLeftLessThan());
}
//return the lineSegment that intersects point
const LineSegment* pointLocationLeft(const Node* root, Point& point)
{
    const LineSegment* segment = new LineSegment();
    if (root == nullptr)
        return segment;
    //Dummy lineSegment to facilitate checking. Can be improved by a function call
    LineSegment pointLineSegment(point.getX(), point.getX(), point.getY(), point.getY());
    auto up = upper_bound(
        root->getVal().begin(), root->getVal().end(), pointLineSegment, YLeftLessThan());
    //get the index of where this up lineSegment is located
    int index;
    if (up != root->getVal().end())
        index = up - root->getVal().begin();
    else
        index = root->getValSize();
    //find a
    while(root->getMinMaxX()[index] < point.getX() && index > 0)
        index--;
    //case there is an LineSegment intersecting the ray down
    if(root->getMinMaxX()[index] >= point.getX())
    {
        //check the subTree to my right
        segment = &max(*segment, *pointLocationLeft(root->getIthChild(index), point), YLeftLessThan());
        //case nothing intersect within the sub-tree
        if (segment->getYLeft() == -1  && root->getIthVal(index).getXLeft() <= point.getX())
        {
            segment = &root->getIthVal(index);
        }
    }
    return segment;
}

const LineSegment* pointLocationRight(const Node* root, Point& point)
{
    const LineSegment* segment = new LineSegment();
    if (root == nullptr)
        return segment;
    LineSegment pointLineSegment(point.getX(), point.getX(), point.getY(), point.getY());
    auto up = upper_bound(
        root->getVal().begin(), root->getVal().end(), pointLineSegment, YLeftLessThan());
    int index;
    if (up != root->getVal().end())
        index = up - root->getVal().begin();
    else
        index = root->getValSize();
    while(root->getMinMaxX()[index] > point.getX() && index > 0)
        index--;
    //case there is an LineSegment intersecting the ray down
    if(root->getMinMaxX()[index] <= point.getX())
    {
        //check the subTree to my right
        segment = &max(*segment, *pointLocationRight(root->getIthChild(index), point), YLeftLessThan());
        //case nothing intersect within the sub-tree
        if (segment->getYLeft() == -1  && root->getIthVal(index).getXRight() >= point.getX())
        {
            segment = &root->getIthVal(index);
        }
    }
    return segment;
}


// TO DO LIST Finish middle point location, and also finish the pointers usage from main.
const LineSegment* pointLocationMiddle(const MiddleNode* root, Point& point, int& slabIndex)
{
    const LineSegment* segment = new LineSegment();
    //lower bound will return the val.size() if no element in the vector is less than point.x
    if (root == nullptr || slabIndex == root->getValSize() )
        return segment;
    //get the slab index
    unsigned int spanningFlag = 1 << slabIndex;
    //get the upper bound for y
    LineSegment pointLineSegment(point.getX(), point.getX(), point.getY(), point.getY());
    auto up = upper_bound(
        root->getVal().begin(), root->getVal().end(), pointLineSegment, YLeftLessThan());
    int index;
    if (up != root->getVal().end())
        index = up - root->getVal().begin();
    else
        index = root->getValSize();
    while(root->getIthChild(index)!= nullptr && root->getIthChild(index)->getSpannedSlabs() & spanningFlag != 0)
        index--;

    //case there is an LineSegment intersecting the ray down
    if(root->getIthChild(index)!= nullptr && root->getIthChild(index)->getSpannedSlabs() & spanningFlag == 0)
    {
        //check the subTree to my right
        segment = &max(*segment, *pointLocationMiddle(root->getIthChild(index), point, slabIndex), YLeftLessThan());
        //case nothing intersect within the sub-tree
        if (segment->getYLeft() == -1  && root->getIthVal(index).getXLeft() <= point.getX() <= root->getIthVal(index).getXRight() )
        {
            segment = &root->getIthVal(index);
        }
    }
    return segment;
}
