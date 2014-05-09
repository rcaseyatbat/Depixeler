#include <iostream>
#include <cassert>
#include <vector>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include "data.h"
#include "matrix.h"
#include "readpng.h"

int getUpNeighbor(int x, int y, int imageWidth, int imageHeight) {
    // assume gData stores RGB of each pixel

    if (y == imageHeight - 1) {
        return -1;
    }

    int newY = y + 1;
    int index = 4 * x + (4 * imageWidth * newY);

    return index;
}

int getDownNeighbor(int x, int y, int imageWidth, int imageHeight) {
    // assume gData stores RGB of each pixel

    if (y == 0) {
        return -1;
    }

    int newY = y - 1;
    int index = 4 * x + (4 * imageWidth * newY);

    return index;
}

int getLeftNeighbor(int x, int y, int imageWidth, int imageHeight) {
    // assume gData stores RGB of each pixel

    if (x == 0) {
        return -1;
    }

    int newX = x - 1;
    int index = 4 * newX + (4 * imageWidth * y);

    return index;
}

int getRightNeighbor(int x, int y, int imageWidth, int imageHeight) {
    // assume gData stores RGB of each pixel

    if (x == imageWidth - 1) {
        return -1;
    }

    int newX = x + 1;
    int index = 4 * newX + (4 * imageWidth * y);

    return index;
}


int getUpLeftNeighbor(int x, int y, int imageWidth, int imageHeight) {
    // assume gData stores RGB of each pixel

    if (x == 0 || y == imageHeight - 1) {
        return -1;
    }

    int newX = x - 1;
    int newY = y + 1;
    int index = 4 * newX + (4 * imageWidth * newY);

    return index;
}

int getUpRightNeighbor(int x, int y, int imageWidth, int imageHeight) {
    // assume gData stores RGB of each pixel

    if (x == imageWidth - 1 || y == imageHeight - 1) {
        return -1;
    }

    int newX = x + 1;
    int newY = y + 1;
    int index = 4 * newX + (4 * imageWidth * newY);

    return index;
}

int getDownLeftNeighbor(int x, int y, int imageWidth, int imageHeight) {
    // assume gData stores RGB of each pixel

    if (x == 0 || y == 0) {
        return -1;
    }

    int newX = x - 1;
    int newY = y - 1;
    int index = 4 * newX + (4 * imageWidth * newY);

    return index;
}

int getDownRightNeighbor(int x, int y, int imageWidth, int imageHeight) {
    // assume gData stores RGB of each pixel

    if (x == imageWidth - 1 || y == 0) {
        return -1;
    }

    int newX = x + 1;
    int newY = y - 1;
    int index = 4 * newX + (4 * imageWidth * newY);

    return index;
}
