#include <iostream>
#include <vector>
#include <cmath>
#include "data.h"
#include "matrix.h"
#include "readpng.h"
#include "neighbors.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

void bilinear2x(int gHeight, int gWidth, int h, int w, int xRes, int yRes, int max, unsigned char gData[])
{
    GLubyte color[2*gHeight][2*gWidth][4];

    glClearColor(0.0, 0.0, 0.0, 0.0);

    //GLubyte color[gHeight][gWidth][4];
    for(int y = 0; y < gHeight; y++) {
        for (int x = 0; x < gWidth; x ++) {
            int index = 4 * x + (4 * w * y);

            for (int xPixel = 0; xPixel < 2; xPixel++) {
                for (int yPixel = 0; yPixel < 2; yPixel++) {
                    color[2*y + yPixel][2*x + xPixel][0] = int(gData[index]);
                    color[2*y + yPixel][2*x + xPixel][1] = int(gData[index + 1]);
                    color[2*y + yPixel][2*x + xPixel][2] = int(gData[index + 2]);
                    color[2*y + yPixel][2*x + xPixel][3] = int(gData[index + 3]);
                }
            }

            int upIndex = getUpNeighbor(x,y, gWidth, gHeight);
            int downIndex = getDownNeighbor(x,y, gWidth, gHeight);
            int leftIndex = getLeftNeighbor(x,y, gWidth, gHeight);
            int rightIndex = getRightNeighbor(x,y, gWidth, gHeight);

            // this is for the edge cases
            if (upIndex == -1 || downIndex == -1 || leftIndex == -1 ||  rightIndex == -1) {
                // we'll just keep the same color for the pixels on the the border (already set)
                // note that we could change the interpolation to take the missing interpolatoin into
                // account..but this would be a lot of casework for little benefit
            }
            else
            {    
                Vec3 up, down, right, left;

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

                int upAlpha = int(gData[upIndex+3]);
                int downAlpha = int(gData[downIndex+3]);
                int leftAlpha = int(gData[leftIndex+3]);
                int rightAlpha = int(gData[rightIndex+3]);

                int currentLeftX = int(gData[index]);
                int currentLeftY = int(gData[index+1]);
                int currentLeftZ = int(gData[index+2]);
                int currentLeftA = int(gData[index+3]);
                

                // we expand each pixel into 4 pixels, and then for each of the new pixels, 
                // we interpolate with each neighbor in a 2x2 block.

                // originally, we have pixel P:
                /*

                 |U|                     U1   U2
                -----                  ----------
                L|P|R    ---->     L1  | P1 | P3 |   R1
                -----              L2  | P2 | P4 |   R2
                 |D|                   ----------
                                        D1   D2

                Let U1 = U2 = U, L1 = L2 = L, R1 = R2 = R, D1 = D2 = D (we just expanded U, L, R, D as well)
                Let P = P1 = P2 = P3 = P4 (we just expanded pixel P)
                Now, we interpolate each of the P_i according to its 4 neigbors.

                P1 = (L1 + U1 + P3 + P2) / 4 = (L + U + 2*P) / 4
                P2 = (L2 + P1 + P4 + D1) / 4 = (L + D + 2*P) / 4
                P3 = (P1 + U2 + R1 + P4) / 4 = (R + U + 2*P) / 4
                P4 = (P2 + P3 + R2 + D2) / 4 = (R + D + 2*P) / 4
                */

                color[2*y][2*x][0] = int((2*currentLeftX + left.x + down.x)/4.0);
                color[2*y][2*x][1] = int((2*currentLeftY + left.y + down.y)/4.0);
                color[2*y][2*x][2] = int((2*currentLeftZ + left.z + down.z)/4.0);
                color[2*y][2*x][3] = int((2*currentLeftA + leftAlpha + downAlpha)/4.0);

                color[2*y][2*x + 1][0] = int((2*currentLeftX + right.x + down.x)/4.0);
                color[2*y][2*x + 1][1] = int((2*currentLeftY + right.y + down.y)/4.0);
                color[2*y][2*x + 1][2] = int((2*currentLeftZ + right.z + down.z)/4.0);
                color[2*y][2*x + 1][3] = int((2*currentLeftA + rightAlpha + downAlpha)/4.0);

                color[2*y + 1][2*x][0] = int((2*currentLeftX + left.x + up.x)/4.0);
                color[2*y + 1][2*x][1] = int((2*currentLeftY + left.y + up.y)/4.0);
                color[2*y + 1][2*x][2] = int((2*currentLeftZ + left.z + up.z)/4.0);
                color[2*y + 1][2*x][3] = int((2*currentLeftA + leftAlpha + upAlpha)/4.0);

                color[2*y + 1][2*x + 1][0] = int((2*currentLeftX + right.x + up.x)/4.0);
                color[2*y + 1][2*x + 1][1] = int((2*currentLeftY + right.y + up.y)/4.0);
                color[2*y + 1][2*x + 1][2] = int((2*currentLeftZ + right.z + up.z)/4.0);
                color[2*y + 1][2*x + 1][3] = int((2*currentLeftA + rightAlpha + upAlpha)/4.0); 
            }
        }
    }
    glPixelZoom(floor(xRes/max)/2, floor(yRes/max)/2);

    glRasterPos2d(-1.0, -1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels( 2*gWidth, 2*gHeight, GL_RGBA, GL_UNSIGNED_BYTE, color );
    glFlush();
}
