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

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

void scale2x(int gHeight, int gWidth, int h, int w, int xRes, int yRes, int max, unsigned char gData[])
{
    GLubyte color[2*gHeight][2*gWidth][4];

    glClearColor(0.0, 0.0, 0.0, 0.0);
    //glVertex3f(0.95,-0.95,0);

    //GLubyte color[gHeight][gWidth][4];
    for(int y = 0; y < gHeight; y++) {
        for (int x = 0; x < 3 * gWidth; x += 3) {
            int index = x + (3 * w * y);

            color[2*y][2*x/3][0] = int(gData[index]);
            color[2*y][2*x/3][1] = int(gData[index + 1]);
            color[2*y][2*x/3][2] = int(gData[index + 2]);
            color[2*y][2*x/3][3] = 0;

            color[2*y][2*x/3 + 1][0] = int(gData[index]);
            color[2*y][2*x/3 + 1][1] = int(gData[index + 1]);
            color[2*y][2*x/3 + 1][2] = int(gData[index + 2]);
            color[2*y][2*x/3 + 1][3] = 0;

            color[2*y + 1][2*x/3][0] = int(gData[index]);
            color[2*y + 1][2*x/3][1] = int(gData[index + 1]);
            color[2*y + 1][2*x/3][2] = int(gData[index + 2]);
            color[2*y + 1][2*x/3][3] = 0;

            color[2*y + 1][2*x/3 + 1][0] = int(gData[index]);
            color[2*y + 1][2*x/3 + 1][1] = int(gData[index + 1]);
            color[2*y + 1][2*x/3 + 1][2] = int(gData[index + 2]);
            color[2*y + 1][2*x/3 + 1][3] = 0;

            int upIndex = getUpNeighbor(x/3,y, gWidth, gHeight);
            int downIndex = getDownNeighbor(x/3,y, gWidth, gHeight);
            int leftIndex = getLeftNeighbor(x/3,y, gWidth, gHeight);
            int rightIndex = getRightNeighbor(x/3,y, gWidth, gHeight);

            // this is for the corner cases
            if ((upIndex == -1 && leftIndex == -1) ||
                (upIndex == -1 && rightIndex == -1) ||
                (downIndex == -1 && leftIndex == -1) ||
                (downIndex == -1 && rightIndex == -1))
            {
                color[2*y][2*x/3][0] = int(gData[index]);
                color[2*y][2*x/3][1] = int(gData[index + 1]);
                color[2*y][2*x/3][2] = int(gData[index + 2]);
                color[2*y][2*x/3][3] = 0;

                color[2*y][2*x/3 + 1][0] = int(gData[index]);
                color[2*y][2*x/3 + 1][1] = int(gData[index + 1]);
                color[2*y][2*x/3 + 1][2] = int(gData[index + 2]);
                color[2*y][2*x/3 + 1][3] = 0;

                color[2*y + 1][2*x/3][0] = int(gData[index]);
                color[2*y + 1][2*x/3][1] = int(gData[index + 1]);
                color[2*y + 1][2*x/3][2] = int(gData[index + 2]);
                color[2*y + 1][2*x/3][3] = 0;

                color[2*y + 1][2*x/3 + 1][0] = int(gData[index]);
                color[2*y + 1][2*x/3 + 1][1] = int(gData[index + 1]);
                color[2*y + 1][2*x/3 + 1][2] = int(gData[index + 2]);
                color[2*y + 1][2*x/3 + 1][3] = 0;
            }

            else
            {
                Vec3 up;
                Vec3 down;
                Vec3 right;
                Vec3 left;
                up.x = gData[upIndex];
                up.y = gData[upIndex+1];
                up.z = gData[upIndex+2];
                down.x = gData[downIndex];
                down.y = gData[downIndex+1];
                down.z = gData[downIndex+2];
                right.x = gData[rightIndex];
                right.y = gData[rightIndex+1];
                right.z = gData[rightIndex+2];
                left.x = gData[leftIndex];
                left.y = gData[leftIndex+1];
                left.z = gData[leftIndex+2];

                bool upLeft = (up == left);
                bool upRight = (up == right);

                bool downLeft = (down == left);
                bool downRight = (down == right);

                bool updown = (up == down);
                bool leftright = (left == right);

                if (!(updown) && !(leftright)) {
                    if (downLeft) {
                        color[2*y][2*x/3][0] = int(left.x);
                        color[2*y][2*x/3][1] = int(left.y);

                        color[2*y][2*x/3][2] = int(left.z);
                        color[2*y][2*x/3][3] = 0;
                    } 

                    else if (downRight) {
                        color[2*y][2*x/3 + 1][0] = int(right.x);
                        color[2*y][2*x/3 + 1][1] = int(right.y);

                        color[2*y][2*x/3 + 1][2] = int(right.z);
                        color[2*y][2*x/3 + 1][3] = 0;
                    } 

                    else if (upLeft) {
                        color[2*y + 1][2*x/3][0] = int(left.x);
                        color[2*y + 1][2*x/3][1] = int(left.y);

                        color[2*y + 1][2*x/3][2] = int(left.z);
                        color[2*y + 1][2*x/3][3] = 0;
                    }

                    else if (upRight) {
                        color[2*y + 1][2*x/3 + 1][0] = int(right.x);
                        color[2*y + 1][2*x/3 + 1][1] = int(right.y);

                        color[2*y + 1][2*x/3 + 1][2] = int(right.z);
                        color[2*y + 1][2*x/3 + 1][3] = 0;
                    }
                }

                else {
                    color[2*y][2*x/3][0] = int(gData[index]);
                    color[2*y][2*x/3][1] = int(gData[index + 1]);
                    color[2*y][2*x/3][2] = int(gData[index + 2]);
                    color[2*y][2*x/3][3] = 0;

                    color[2*y][2*x/3 + 1][0] = int(gData[index]);
                    color[2*y][2*x/3 + 1][1] = int(gData[index + 1]);
                    color[2*y][2*x/3 + 1][2] = int(gData[index + 2]);
                    color[2*y][2*x/3 + 1][3] = 0;

                    color[2*y + 1][2*x/3][0] = int(gData[index]);
                    color[2*y + 1][2*x/3][1] = int(gData[index + 1]);
                    color[2*y + 1][2*x/3][2] = int(gData[index + 2]);
                    color[2*y + 1][2*x/3][3] = 0;

                    color[2*y + 1][2*x/3 + 1][0] = int(gData[index]);
                    color[2*y + 1][2*x/3 + 1][1] = int(gData[index + 1]);
                    color[2*y + 1][2*x/3 + 1][2] = int(gData[index + 2]);
                    color[2*y + 1][2*x/3 + 1][3] = 0;
                }
            }
        }
    }
    glPixelZoom(floor(xRes/max)/2, floor(yRes/max)/2);

    glRasterPos2d(-1.0, -1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels( 2*gWidth, 2*gHeight, GL_RGBA, GL_UNSIGNED_BYTE, color );
    glFlush();
}
