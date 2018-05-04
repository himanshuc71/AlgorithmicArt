
/**
 *
 * twoDtree (pa3)
 * slight modification of a Kd tree of dimension 2.
 * twoDtree.cpp
 * This file will be used for grading.
 *
 */

#include "twoDtree.h"
#include <cmath>

/* given */
twoDtree::Node::Node(pair<int, int> ul, pair<int, int> lr, RGBAPixel a)
        : upLeft(ul), lowRight(lr), avg(a), left(NULL), right(NULL) {}

/* given */
twoDtree::~twoDtree() {
    clear();
}

/* given */
twoDtree::twoDtree(const twoDtree &other) {
    copy(other);
}

/* given */
twoDtree &twoDtree::operator=(const twoDtree &rhs) {
    if (this != &rhs) {
        clear();
        copy(rhs);
    }
    return *this;
}

twoDtree::twoDtree(PNG &imIn) {
    stats s(imIn);
    height = imIn.height();
    width = imIn.width();
    pair<int, int> ul = make_pair(0, 0);
    pair<int, int> lr = make_pair(imIn.width() - 1, imIn.height() - 1);

    root = buildTree(s, ul, lr);
}

twoDtree::Node *twoDtree::buildTree(stats &s, pair<int, int> ul, pair<int, int> lr) {
    RGBAPixel pxl = s.getAvg(ul, lr);
    if (ul == lr) {
        return new Node(ul, lr, pxl);
    }

    Node *subRoot = new Node(ul, lr, pxl);

    // knowing how to split
    long minScore = LONG_MAX;
    pair<int, int> newLr, newUl;

    // finding min vertical split
    for (int x = ul.first; x < lr.first; x++) {
        pair<int, int> p1 = make_pair(x, lr.second);
        pair<int, int> p2 = make_pair(x + 1, ul.second);
        long temp = s.getScore(ul, p1) + s.getScore(p2, lr);
        if (temp < minScore) {
            minScore = temp;
            newLr = p1;
            newUl = p2;
        }
    }
    // finding min horizontal split and overwriting the new coordinates if horizontal gives the least score
    for (int y = ul.second; y < lr.second; y++) {
        pair<int, int> p1 = make_pair(lr.first, y);
        pair<int, int> p2 = make_pair(ul.first, y + 1);
        long temp = s.getScore(ul, p1) + s.getScore(p2, lr);
        if (temp < minScore) {
            minScore = temp;
            newLr = p1;
            newUl = p2;
        }
    }
    // taking the minimum of the vertical and the horizontal split which the loops do

    subRoot->left = buildTree(s, ul, newLr);
    subRoot->right = buildTree(s, newUl, lr);
    return subRoot;
}

PNG twoDtree::render() {
    PNG img(width, height);
    render(root, img);
    return img;
}

void twoDtree::render(const Node *subRoot, PNG &image) {
    if (subRoot == NULL) {
        return;
    }
    if (subRoot->left == NULL && subRoot->right == NULL) {
        for (int x = subRoot->upLeft.first; x <= subRoot->lowRight.first; x++) {
            for (int y = subRoot->upLeft.second; y <= subRoot->lowRight.second; y++) {
                RGBAPixel *pixelptr = image.getPixel(x, y);
                *pixelptr = subRoot->avg;
            }
        }
        return;
    }

    render(subRoot->left, image);
    render(subRoot->right, image);

}

void twoDtree::prune(double pct, int tol) {
    prune(root, pct, tol);
}

int twoDtree::pruneDistance(Node *baseRoot, Node *leaf) {
    return pow((baseRoot->avg.r - leaf->avg.r), 2) + pow((baseRoot->avg.g - leaf->avg.g), 2) +
           pow((baseRoot->avg.b - leaf->avg.b), 2);
}

void twoDtree::prune(Node *subRoot, double pct, int tol) {
    if (subRoot == NULL)
        return;
    if (numberOfLeavesWithinTolerance(subRoot, subRoot, tol) >= pct * countLeaves(subRoot)) {
        clear(subRoot->left);
        clear(subRoot->right);
        subRoot->left = NULL;
        subRoot->right = NULL;
        return;
    }
    prune(subRoot->left, pct, tol);
    prune(subRoot->right, pct, tol);
}

int twoDtree::countLeaves(Node *subRoot) {
    if (subRoot == NULL)
        return 0;
    if (subRoot->left == NULL && subRoot->right == NULL)
        return 1;
    else return countLeaves(subRoot->left) + countLeaves(subRoot->right);
}

int twoDtree::numberOfLeavesWithinTolerance(Node *baseRoot, Node *subRoot, int tol) {
    if (subRoot == NULL)
        return 0;
    if (subRoot->left == NULL && subRoot->right == NULL) {
        if (pruneDistance(baseRoot, subRoot) <= tol)
            return 1;
        else
            return 0;
    } else {
        return numberOfLeavesWithinTolerance(baseRoot, subRoot->left, tol) +
               numberOfLeavesWithinTolerance(baseRoot, subRoot->right, tol);
    }

}

void twoDtree::clear() {
    clear(root);
    root = NULL;
    height = 0;
    width = 0;
}

void twoDtree::clear(Node *subRoot) {
    if (subRoot == NULL)
        return;
    clear(subRoot->left);
    clear(subRoot->right);
    delete subRoot;
}


void twoDtree::copy(const twoDtree &orig) {
    root = copy(orig.root);
    this->height = orig.height;
    this->width = orig.width;
}


twoDtree::Node *twoDtree::copy(const Node *subRoot) {
    if (subRoot == NULL)
        return NULL;
    Node *newTreeNode = new Node(subRoot->upLeft, subRoot->lowRight, subRoot->avg);
    newTreeNode->left = copy(subRoot->left);
    newTreeNode->right = copy(subRoot->right);
    return newTreeNode;
}



