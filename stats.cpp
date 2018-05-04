


#include "stats.h"
#include <cmath>

long stats::getSum(char channel, pair<int, int> ul, pair<int, int> lr) {
    long sum;
    if (channel == 'r') {
        if (ul.first == 0 && ul.second == 0) {
            sum = sumRed[lr.first][lr.second];
        } else if (ul.first == 0) {
            sum = sumRed[lr.first][lr.second] - sumRed[lr.first][ul.second - 1];
        } else if (ul.second == 0) {
            sum = sumRed[lr.first][lr.second] - sumRed[ul.first - 1][lr.second];
        } else
            sum = sumRed[lr.first][lr.second] - sumRed[lr.first][ul.second - 1]
                  - sumRed[ul.first - 1][lr.second] + sumRed[ul.first - 1][ul.second - 1];
    } else if (channel == 'g') {
        if (ul.first == 0 && ul.second == 0) {
            sum = sumGreen[lr.first][lr.second];
        } else if (ul.first == 0) {
            sum = sumGreen[lr.first][lr.second] - sumGreen[lr.first][ul.second - 1];
        } else if (ul.second == 0) {
            sum = sumGreen[lr.first][lr.second] - sumGreen[ul.first - 1][lr.second];
        } else {
            sum = sumGreen[lr.first][lr.second] - sumGreen[lr.first][ul.second - 1]
                  - sumGreen[ul.first - 1][lr.second] + sumGreen[ul.first - 1][ul.second - 1];
        }
    } else {
        if (ul.first == 0 && ul.second == 0) {
            sum = sumBlue[lr.first][lr.second];
        } else if (ul.first == 0) {
            sum = sumBlue[lr.first][lr.second] - sumBlue[lr.first][ul.second - 1];
        } else if (ul.second == 0) {
            sum = sumBlue[lr.first][lr.second] - sumBlue[ul.first - 1][lr.second];
        } else {
            sum = sumBlue[lr.first][lr.second] - sumBlue[lr.first][ul.second - 1]
                  - sumBlue[ul.first - 1][lr.second] + sumBlue[ul.first - 1][ul.second - 1];
        }
    }
    return sum;
}

long stats::getSumSq(char channel, pair<int, int> ul, pair<int, int> lr) {
    long sumSq;
    if (channel == 'r') {
        if (ul.first == 0 && ul.second == 0) {
            sumSq = sumsqRed[lr.first][lr.second];
        } else if (ul.first == 0) {
            sumSq = sumsqRed[lr.first][lr.second] - sumsqRed[lr.first][ul.second - 1];
        } else if (ul.second == 0) {
            sumSq = sumsqRed[lr.first][lr.second] - sumsqRed[ul.first - 1][lr.second];
        } else {
            sumSq = sumsqRed[lr.first][lr.second] - sumsqRed[lr.first][ul.second - 1]
                    - sumsqRed[ul.first - 1][lr.second] + sumsqRed[ul.first - 1][ul.second - 1];
        }
    } else if (channel == 'g') {
        if (ul.first == 0 && ul.second == 0) {
            sumSq = sumsqGreen[lr.first][lr.second];
        } else if (ul.first == 0) {
            sumSq = sumsqGreen[lr.first][lr.second] - sumsqGreen[lr.first][ul.second - 1];
        } else if (ul.second == 0) {
            sumSq = sumsqGreen[lr.first][lr.second] - sumsqGreen[ul.first - 1][lr.second];
        } else {
            sumSq = sumsqGreen[lr.first][lr.second] - sumsqGreen[lr.first][ul.second - 1]
                    - sumsqGreen[ul.first - 1][lr.second] + sumsqGreen[ul.first - 1][ul.second - 1];
        }
    } else {
        if (ul.first == 0 && ul.second == 0) {
            sumSq = sumsqBlue[lr.first][lr.second];
        } else if (ul.first == 0) {
            sumSq = sumsqBlue[lr.first][lr.second] - sumsqBlue[lr.first][ul.second - 1];
        } else if (ul.second == 0) {
            sumSq = sumsqBlue[lr.first][lr.second] - sumsqBlue[ul.first - 1][lr.second];
        } else {
            sumSq = sumsqBlue[lr.first][lr.second] - sumsqBlue[lr.first][ul.second - 1]
                    - sumsqBlue[ul.first - 1][lr.second] + sumsqBlue[ul.first - 1][ul.second - 1];
        }
    }
    return sumSq;
}

stats::stats(PNG &im) {
    sumRed.resize(im.width(), vector<long>(im.height()));
    sumGreen.resize(im.width(), vector<long>(im.height()));
    sumBlue.resize(im.width(), vector<long>(im.height()));
    sumsqRed.resize(im.width(), vector<long>(im.height()));
    sumsqGreen.resize(im.width(), vector<long>(im.height()));
    sumsqBlue.resize(im.width(), vector<long>(im.height()));
    for (unsigned int x = 0; x < im.width(); x++) {
        for (unsigned int y = 0; y < im.height(); y++) {
            RGBAPixel *pxlptr = im.getPixel(x, y);
            //(0,0) case
            if (x == 0 && y == 0) {
                sumRed[x][y] = pxlptr->r;
                sumGreen[x][y] = pxlptr->g;
                sumBlue[x][y] = pxlptr->b;
                sumsqRed[x][y] = pow(pxlptr->r, 2);
                sumsqGreen[x][y] = pow(pxlptr->g, 2);
                sumsqBlue[x][y] = pow(pxlptr->b, 2);
            } else if (x == 0) {
                // 1st column case
                sumRed[x][y] = sumRed[x][y - 1] + pxlptr->r;
                sumGreen[x][y] = sumGreen[x][y - 1] + pxlptr->g;
                sumBlue[x][y] = sumBlue[x][y - 1] + pxlptr->b;
                sumsqRed[x][y] = sumsqRed[x][y - 1] + pow(pxlptr->r, 2);
                sumsqGreen[x][y] = sumsqGreen[x][y - 1] + pow(pxlptr->g, 2);
                sumsqBlue[x][y] = sumsqBlue[x][y - 1] + pow(pxlptr->b, 2);
            } else if (y == 0) {
                // 1st row case
                sumRed[x][y] = sumRed[x - 1][y] + pxlptr->r;
                sumGreen[x][y] = sumGreen[x - 1][y] + pxlptr->g;
                sumBlue[x][y] = sumBlue[x - 1][y] + pxlptr->b;
                sumsqRed[x][y] = sumsqRed[x - 1][y] + pow(pxlptr->r, 2);
                sumsqGreen[x][y] = sumsqGreen[x - 1][y] + pow(pxlptr->g, 2);
                sumsqBlue[x][y] = sumsqBlue[x - 1][y] + pow(pxlptr->b, 2);
            } else {
                // every other case
                sumRed[x][y] = sumRed[x - 1][y] + sumRed[x][y - 1] - sumRed[x - 1][y - 1] + pxlptr->r;
                sumGreen[x][y] = sumGreen[x - 1][y] + sumGreen[x][y - 1] - sumGreen[x - 1][y - 1] + pxlptr->g;
                sumBlue[x][y] = sumBlue[x - 1][y] + sumBlue[x][y - 1] - sumBlue[x - 1][y - 1] + pxlptr->b;
                sumsqRed[x][y] = sumsqRed[x - 1][y] + sumsqRed[x][y - 1] - sumsqRed[x - 1][y - 1] + pow(pxlptr->r, 2);
                sumsqGreen[x][y] =
                        sumsqGreen[x - 1][y] + sumsqGreen[x][y - 1] - sumsqGreen[x - 1][y - 1] + pow(pxlptr->g, 2);
                sumsqBlue[x][y] =
                        sumsqBlue[x - 1][y] + sumsqBlue[x][y - 1] - sumsqBlue[x - 1][y - 1] + pow(pxlptr->b, 2);
            }
        }
    }
}

long stats::getScore(pair<int, int> ul, pair<int, int> lr) {
    long score;
    long area = rectArea(ul, lr);
    score = getSumSq('r', ul, lr) + getSumSq('g', ul, lr) + getSumSq('b', ul, lr)
            - pow(getSum('r', ul, lr), 2) / area - pow(getSum('g', ul, lr), 2) / area
            - pow(getSum('b', ul, lr), 2) / area;
    return score;
}

RGBAPixel stats::getAvg(pair<int, int> ul, pair<int, int> lr) {
    RGBAPixel pixel;
    long area = rectArea(ul, lr);
    pixel.r = getSum('r', ul, lr) / area;
    pixel.g = getSum('g', ul, lr) / area;
    pixel.b = getSum('b', ul, lr) / area;
    return pixel;
}

long stats::rectArea(pair<int, int> ul, pair<int, int> lr) {
    long area;
    area = (lr.first + 1 - ul.first) * (lr.second + 1 - ul.second);
    return area;
}