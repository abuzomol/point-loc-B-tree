//
// Created by muzamil on 1/31/20.
//

#include "SuperTree.h"

#include <chrono>
#include <cmath>

#include "MiddleTree.h"
#include "Tree.h"

using namespace std;

SuperTree::SuperTree(const int& height) { superTree.resize(height); }

SuperTree::SuperTree(const int& height, const std::vector<double>& xValues)
{
    superTree.resize(height);

    // construct the leaves of the superTree
    unsigned int numOfLeaves = xValues.size() % VAL_SIZE == 0
                                   ? xValues.size() / VAL_SIZE
                                   : xValues.size() / VAL_SIZE + 1;

    auto start = std::chrono::high_resolution_clock::now();
    SuperTree::superTree.back().reserve(numOfLeaves);

    // fill all leaves but the last one
    for (unsigned int i = 0;
         i < SuperTree::superTree.back().capacity() - 1;
         i++)
    {
        vector<double> values;
        values.reserve(VAL_SIZE);
        for (unsigned int j = 0; j < VAL_SIZE; j++)
        {
            values.emplace_back(xValues[i * VAL_SIZE + j]);
        }
        SuperTree::superTree[height - 1].emplace_back(values);
    }

    // fill the last leaf
    vector<double> values;
    values.reserve(VAL_SIZE);
    int t;
    for (t = 0; (numOfLeaves - 1) * VAL_SIZE + t < xValues.size(); t++)
    {
        values.emplace_back(xValues[(numOfLeaves - 1) * VAL_SIZE + t]);
    }
    // pad last leaf with infinities if not full
    for (t; t < VAL_SIZE; t++)
    {
        values.emplace_back(INFTY);
    }
    SuperTree::superTree.back().emplace_back(values);

    auto finish = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = finish - start;
    cout << "constructing leaves nodes: " << elapsed.count() << endl;

    // To keep the invariant of B-tree satisfied, for binary tree, we pick the
    // parent from the right child.
    int binaryAdjustment;
    if (VAL_SIZE == 1)
        binaryAdjustment = 1;
    else
        binaryAdjustment = 0;

    // go over every level in the tree, fill values, and children bottom up
    start = std::chrono::high_resolution_clock::now();
    for (int i = height - 2; i > -1; --i)
    {
        // noNodes = ceil(level below/branching factor)
        int noNodes =
            (SuperTree::superTree[i + 1].size() % CHILD_SIZE == 0)
                ? SuperTree::superTree[i + 1].size() / CHILD_SIZE
                : (SuperTree::superTree[i + 1].size() / CHILD_SIZE) + 1;

        SuperTree::superTree[i].reserve(noNodes);
        // go over every node in each level

        for (int j = 0; j < SuperTree::superTree[i].capacity() - 1; j++)
        {
            // set up the children in each node
            vector<double> parent;
            vector<SuperNode*> child;
            parent.reserve(VAL_SIZE);
            child.reserve(CHILD_SIZE);
            // fill parent and child
            for (int l = 0; l < VAL_SIZE; l++)
            {
                double childMaxValue =
                    SuperTree::superTree[i + 1]
                                        [j * CHILD_SIZE + l + binaryAdjustment]
                                            .getIthVal(VAL_SIZE - 1);
                parent.emplace_back(childMaxValue);
                child.emplace_back(
                    &SuperTree::superTree[i + 1][j * CHILD_SIZE + l]);
            }
            // add the last child
            child.emplace_back(
                &SuperTree::superTree[i + 1][j * CHILD_SIZE + VAL_SIZE
                                             + binaryAdjustment]);
            SuperTree::superTree[i].emplace_back(parent, child);
        }
        // fill the last node
        vector<double> lastParent;
        vector<SuperNode*> lastChild;
        lastParent.reserve(VAL_SIZE);
        lastChild.reserve(CHILD_SIZE);
        // fill last node
        int s = 0;
        for (s; (SuperTree::superTree[i].capacity() - 1) * CHILD_SIZE + s
                    < SuperTree::superTree[i + 1].size()
                && s < VAL_SIZE;
             s++)
        {
            if ((SuperTree::superTree[i].capacity() - 1) * CHILD_SIZE + s
                    + binaryAdjustment
                < SuperTree::superTree[i + 1].capacity())
            {
                lastParent.emplace_back(
                    SuperTree::superTree
                        [i + 1]
                        [(SuperTree::superTree[i].capacity() - 1) * CHILD_SIZE
                         + s + binaryAdjustment]
                            .getIthVal(VAL_SIZE - 1));
            }
            else
            {
                lastParent.emplace_back(INFTY);
            }
            lastChild.emplace_back(
                &SuperTree::superTree[i + 1]
                                     [(SuperTree::superTree[i].capacity() - 1)
                                          * CHILD_SIZE
                                      + s]);
        }
        if ((SuperTree::superTree[i].capacity() - 1) * CHILD_SIZE + s
                + binaryAdjustment
            < SuperTree::superTree[i + 1].capacity())
        {
            lastChild.emplace_back(
                &SuperTree::superTree[i + 1]
                                     [(SuperTree::superTree[i].capacity() - 1)
                                          * CHILD_SIZE
                                      + s + binaryAdjustment]);
        }
        else
        {
            lastChild.emplace_back(nullptr);
        }
        // pad last node with infinities if not full
        // TODO() fix the binary version. Error on setting the children.
        for (s; s < VAL_SIZE; s++)
        {
            lastParent.emplace_back(INFTY);
            lastChild.emplace_back(nullptr);
            if (s == VAL_SIZE - 1) lastChild.emplace_back(nullptr);
        }

        SuperTree::superTree[i].emplace_back(lastParent, lastChild);
    }
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    cout << "Building internal nodes: " << elapsed.count() << endl;
}

// check superRoot is not null

void partitionLineSegments(SuperNode& superRoot,
                           vector<LineSegment>& lineSegments,
                           vector<vector<LineSegment>*>* left,
                           vector<vector<LineSegment>*>* right,
                           vector<LineSegment>* middle,
                           vector<vector<LineSegment>*>* remainingLineSegments)
{
    // TODO() remove if statements
    for (auto lineSegment : lineSegments)

    {
        // for loop that scan every line segment
        for (int i = 0; i < superRoot.getValSize(); i++)
        {
            // if the lineSegment crosses or touches boundary i
            if (lineSegment.getXLeft() <= superRoot.getIthVal(i)
                && lineSegment.getXRight() >= superRoot.getIthVal(i))
            {
                // to the left of first boundary
                if (i == 0 && lineSegment.getXLeft() < superRoot.getIthVal(i))
                {
                    (*left)[i]->push_back(lineSegment);
                }
                // starts at slab i-1
                if (i > 0 && superRoot.getIthVal(i - 1) < lineSegment.getXLeft()
                    && lineSegment.getXLeft() < superRoot.getIthVal(i))
                {
                    (*left)[i]->push_back(lineSegment);
                }
                // to the right of last boundary

                if (i == superRoot.getValSize() - 1
                    && lineSegment.getXRight() > superRoot.getIthVal(i))
                {
                    (*right)[i]->push_back(lineSegment);
                }

                // ends at slab i
                if (i < superRoot.getValSize() - 1
                    && lineSegment.getXRight() < superRoot.getIthVal(i + 1)
                    && lineSegment.getXRight() > superRoot.getIthVal(i))
                {
                    (*right)[i]->push_back(lineSegment);
                }
                // case lineSegment crosses slab i
                if (i < superRoot.getValSize() - 1
                    && lineSegment.getXRight() >= superRoot.getIthVal(i + 1))
                {
                    if (!middle->empty() && middle->back() == lineSegment)
                        continue;
                    middle->push_back(lineSegment);
                }
            }
            // case it does not cross any boundary
            else
            {
                // case it ends before first boundary
                if (i == 0 && lineSegment.getXRight() < superRoot.getIthVal(i))
                {
                    (*remainingLineSegments)[i]->push_back(lineSegment);
                }
                // case it starts after last boundary
                if (i == superRoot.getValSize() - 1
                    && lineSegment.getXLeft() > superRoot.getIthVal(i))
                {
                    (*remainingLineSegments)[i + 1]->push_back(lineSegment);
                }
                // case it is between two boundaries i and i +1 i.e. b_i < left
                // x < b_i+1

                if (i > 0 && lineSegment.getXLeft() > superRoot.getIthVal(i - 1)
                    && lineSegment.getXRight() < superRoot.getIthVal(i))
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
            unsigned int height;
            if ((*left)[k]->size() == 1)
                height = 1;
            else
                height = ceil(log((*left)[k]->size()) / log(CHILD_SIZE));
            Tree* tree = new Tree(height, *(*left)[k], true);
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
            unsigned int height;
            if ((*right)[k]->size() == 1)
                height = 1;
            else
                height = ceil(log((*right)[k]->size()) / log(CHILD_SIZE));
            Tree* tree = new Tree(height, *(*right)[k], false);
            Node* root = tree->getRoot();
            superRoot.setIthRightSemiLines(*root, k);
        }
    }
}

void constructMiddleTree(SuperNode& superRoot, vector<LineSegment>* middle)
{
    unsigned int middleNodesTotal = ceil(middle->size() * 1.0 / VAL_SIZE);

    unsigned int height;
    if (middle->size() == 1)
        height = 1;
    else
        height = ceil(log(middle->size()) / log(CHILD_SIZE));
    MiddleTree* middleTree = new MiddleTree(height, superRoot, *middle);
    MiddleNode* middleRoot = middleTree->getRoot();
    superRoot.setMiddle(middleRoot);
}

void fillSuperTree(SuperNode& superRoot, vector<LineSegment>& lineSegments)
{
    auto* left = new vector<vector<LineSegment>*>(superRoot.getValSize());
    auto* right = new vector<vector<LineSegment>*>(superRoot.getValSize());
    // initialize the middle in the heap. Middle is connected to SuperNode
    // parent.
    auto* middle = new vector<LineSegment>();
    // initialize the remainingLineSegments that left out from left, right, and
    // middle
    auto* remainingLineSegments =
        new vector<vector<LineSegment>*>(superRoot.getValSize() + 1);

    for (int i = 0; i < superRoot.getValSize(); i++)
    {
        (*left)[i] = new vector<LineSegment>();
        (*right)[i] = new vector<LineSegment>();
        (*remainingLineSegments)[i] = new vector<LineSegment>();
    }

    // initialize the last child to an empty vector
    (*remainingLineSegments)[superRoot.getValSize()] =
        new vector<LineSegment>();

    // partition lineSegments into left, middle and right and
    // remainingLineSegments

    // cout <<"partitioning the data" <<endl;
    partitionLineSegments(
        superRoot, lineSegments, left, right, middle, remainingLineSegments);
    // cout << "\nPartition of lineSegments into left, right, middle and
    // remaining ----> Done.";
    // construct the left B-trees for each cell of superRoot
    if (!left->empty())
    {
        constructLeftTrees(superRoot, left);
    }

    // cout <<"consrcuted left trees" <<endl;

    // cout << "\nConstructing Left tree ----> Done.";
    // construct the right B-trees for each cell of superRoot
    if (!right->empty())
    {
        constructRightTrees(superRoot, right);
    }
    // cout <<"consrcuted right trees" <<endl;
    // cout << "\nConstrcuting right tree ----> Done.";
    // construct the middle B-trees
    if (!middle->empty())
    {
        constructMiddleTree(superRoot, middle);  // ERROR HERE
    }

    // cout << "\nConstrcuting Middle tree ----> Done.";
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

    // cout <<"consrcuted middle trees" <<endl;
    delete left;
    delete right;
    delete middle;
    delete remainingLineSegments;
}

const int SuperTree::size() const { return SuperTree::superTree.size(); }

const SuperNode& SuperTree::getRoot() const { return superTree[0][0]; }
const vector<std::vector<SuperNode>>& SuperTree::getSuperTree() const
{
    return superTree;
}

const LineSegment& pointLocationQuery(SuperNode& superRoot, Point& point)
{
    LineSegment middleSegment;
    LineSegment leftSegment;
    LineSegment rightSegment;
    LineSegment recursiveSegment;

    // get the lower bound of x-values
    auto low = lower_bound(
        superRoot.getVal().begin(), superRoot.getVal().end(), point.getX());

    int index;
    if (low != superRoot.getVal().end())
        index = low - superRoot.getVal().begin();
    else
        index = superRoot.getValSize();

    // get the middleSegment if found
    if (superRoot.getMiddle() != nullptr)
    {
        middleSegment =
            pointLocationMiddle(superRoot.getMiddle(), point, index);
    }

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

    if (middleSegment.getYLeft() == INFTY) middleSegment.setAll(-1, -1, -1);
    if (leftSegment.getYLeft() == INFTY) leftSegment.setAll(-1, -1, -1);
    if (rightSegment.getYLeft() == INFTY) rightSegment.setAll(-1, -1, -1);
    if (recursiveSegment.getYLeft() == INFTY)
        recursiveSegment.setAll(-1, -1, -1);

    const LineSegment ans =
        max(leftSegment,
            max(middleSegment,
                max(rightSegment, recursiveSegment, YLeftLessThan()),
                YLeftLessThan()),
            YLeftLessThan());
    return ans;
}

/* This function queries the tree of line segments to the right of one of the
 * superNode boundaries. These lineSegments are sorted first by y-axis and then
 * built up bottom up approach. A min end point is maintained with each sub-tree
 * to speed up the search.
 */
const LineSegment& pointLocationLeft(const Node* root, Point& point)
{
    LineSegment segment;
    if (root == nullptr) return segment;
    // Dummy lineSegment to facilitate checking. Can be improved by a function
    // call
    LineSegment pointLineSegment(
        point.getX(), point.getX(), point.getY(), point.getY());
    auto up = upper_bound(root->getVal().begin(),
                          root->getVal().end(),
                          pointLineSegment,
                          YLeftLessThan());
    // get the index of where this up lineSegment is located
    int index;
    if (up != root->getVal().end())
        index = up - root->getVal().begin();
    else
        index = root->getValSize();
    while (index > -1 && segment.getYLeft() == INFTY)
    {
        /* ignores the subtrees with no line segment stabbed by point.
         TODO(): can we speed up this further to get  log B instead of B?
         */
        while (root->getMinMaxX()[index] > point.getX() && index > 0) index--;
        // case there is an LineSegment stabbed by the ray down
        if (root->getMinMaxX()[index] >= point.getX())
        {
            // check the subTree to my right
            segment = max(segment,
                          pointLocationLeft(root->getIthChild(index), point),
                          YLeftLessThan());
        }
        // case nothing intersect within the sub-tree i.e. root is a leaf
        if (segment.getYLeft() == INFTY
            && root->getIthVal(index).getXLeft() <= point.getX()
            && root->getIthVal(index).getYLeft() <= point.getY())
        {
            segment = root->getIthVal(index);
        }
        index--;
    }
    return segment;
}

/* This function queries the tree of line segments to the right of one of the
 * superNode boundaries. These lineSegments are sorted first by y-axis and then
 * built up bottom up approach. A max end point is maintained with each sub-tree
 * to speed up the search.
 */

const LineSegment& pointLocationRight(const Node* root, Point& point)
{
    LineSegment segment;
    if (root == nullptr) return segment;
    LineSegment pointLineSegment(
        point.getX(), point.getX(), point.getY(), point.getY());
    auto up = upper_bound(root->getVal().begin(),
                          root->getVal().end(),
                          pointLineSegment,
                          YLeftLessThan());
    int index;
    if (up != root->getVal().end())
        index = up - root->getVal().begin();
    else
        index = root->getValSize();
    /* ignores the subtrees with no line segment stabbed by point.
     TO DO: can we speed up this further to get  log B instead of B
     */
    while (index > -1 && segment.getYLeft() == INFTY)
    {
        while (root->getMinMaxX()[index] < point.getX() && index > 0) index--;
        // case there is an LineSegment stabbed by the ray down
        if (root->getMinMaxX()[index] <= point.getX())
        {
            // check the subTree to my right
            segment = max(segment,
                          pointLocationRight(root->getIthChild(index), point),
                          YLeftLessThan());
        }
        // case nothing intersect within the sub-tree i.e root is a leaf node
        if (segment.getYLeft() == INFTY
            && root->getIthVal(index).getXRight() >= point.getX()
            && root->getIthVal(index).getYLeft() <= point.getY())
        {
            segment = root->getIthVal(index);
        }
        index--;
    }
    return segment;
}

/* This function queries the middleTree associated with a superNode. Recall
 * that this middle tree contains all line segments that cross one or more
 * slabs. They are sorted first by y-axis and then built up bottom up approach.
 * A bitflag is maintained to indicate which slabs are crossed with line
 * segments stored within the subtree
 */

const LineSegment& pointLocationMiddle(const MiddleNode* root,
                                       Point& point,
                                       int& slabIndex)
{
    LineSegment segment;
    // lower bound will return the val.size() if no element in the vector is
    // less than point.x
    if (root == nullptr || slabIndex == root->getValSize()) return segment;

    // get the upper bound for y
    LineSegment pointLineSegment(
        point.getX(), point.getX(), point.getY(), point.getY());
    auto up = upper_bound(root->getVal().begin(),
                          root->getVal().end(),
                          pointLineSegment,
                          YLeftLessThan());
    int index;
    if (up != root->getVal().end())
    {
        index = up - root->getVal().begin();
    }
    else
    {
        index = root->getValSize();
    }

    /* ignores the super subtrees with no line segment crossing the slab
     slabIndex
     TODO(): can we speed up this further to get  log B instead of B
     */
    while (index > -1 && segment.getYLeft() == INFTY)
    {
        while (
            index > -1 && root->getIthChild(index) != nullptr
            && ((root->getIthChild(index)->getSpannedSlabs() >> slabIndex) & 1)
                   == 0)
            index--;

        // case there is a LineSegment intersecting the ray down
        if (index > -1 && root->getIthChild(index) != nullptr
            && ((root->getIthChild(index)->getSpannedSlabs() >> slabIndex) & 1)
                   == 1)
        {
            // check the subTree to my right
            segment = max(
                segment,
                pointLocationMiddle(root->getIthChild(index), point, slabIndex),
                YLeftLessThan());
        }
        // case nothing intersects within the sub-tree i.e. child[index] returns
        // empty lineSegment
        if (segment.getYLeft() == INFTY
            && root->getIthVal(index).getXLeft() <= point.getX()
            && point.getX() <= root->getIthVal(index).getXRight()
            && root->getIthVal(index).getYLeft() <= point.getY())
        {
            segment = root->getIthVal(index);
        }
        index--;
    }
    return segment;
}
