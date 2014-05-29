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

double CubicInterpolate(double y0,double y1, double y2,double y3, double mu)
{
   double a0,a1,a2,a3,mu2;

   mu2 = mu * mu;
   a0 = y3 - y2 - y0 + y1;
   a1 = y0 - y1 - a0;
   a2 = y2 - y0;
   a3 = y1;

   return(a0 * mu * mu2 + a1 * mu2 + a2 * mu + a3);
}

Vec3 CubicInterpolateVec(Vec3 y0,Vec3 y1, Vec3 y2,Vec3 y3, double mu)
{
   double a0x,a1x,a2x,a3x;
   double a0y,a1y,a2y,a3y;
   double a0z,a1z,a2z,a3z;

   double mu2 = mu * mu;

   // Catmull-Rom cubic splines!
   a0x = -0.5 * y0.x + 1.5 * y1.x - 1.5 * y2.x + 0.5 * y3.x;
   a1x = y0.x - 2.5 * y1.x + 2 * y2.x - 0.5 * y3.x;
   a2x = -0.5 * y0.x + 0.5 * y2.x;
   a3x = y1.x;

   a0y = -0.5 * y0.y + 1.5 * y1.y - 1.5 * y2.y + 0.5 * y3.y;
   a1y = y0.y - 2.5 * y1.y + 2 * y2.y - 0.5 * y3.y;
   a2y = -0.5 * y0.y + 0.5 * y2.y;
   a3y = y1.y;

   a0z = -0.5 * y0.z + 1.5 * y1.z - 1.5 * y2.z + 0.5 * y3.z;
   a1z = y0.z - 2.5 * y1.z + 2 * y2.z - 0.5 * y3.z;
   a2z = -0.5 * y0.z + 0.5 * y2.z;
   a3z = y1.z;

   Vec3 returnVec;
   returnVec.x = a0x * mu * mu2 + a1x * mu2 + a2x * mu + a3x;
   returnVec.y = a0y * mu * mu2 + a1y * mu2 + a2y * mu + a3y;
   returnVec.z = a0z * mu * mu2 + a1z * mu2 + a2z * mu + a3z;

   return returnVec;
}

void bicubic2x(int gHeight, int gWidth, int h, int w, int xRes, int yRes, int max, unsigned char gData[])
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

                Vec3 current;
                current.x = int(gData[index]);
                current.y = int(gData[index+1]);
                current.z = int(gData[index+2]);
                

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

                We do a cubic interpolation in each direction (horiztonal and vertical) according to 
                the 2 neighbors on either side.

                Order is important as CubicInterpolateVec(y0, y1, y2, y3, m) interpolates and returns
                the value m fraction of the way between y1 and y2.

                We use Catmoll-Rom spline interpolation, and then we make sure to clamp our RGB
                values between 0 and 255 before assigning the new colors. Note that we don't 
                really care about alpha values here.
                */



                // P2
                Vec3 P2Left = CubicInterpolateVec(left, left, current, right, 0.5);
                Vec3 P2Up = CubicInterpolateVec(up, current, down, down, 0.5);

                int finalP2X = min(std::max((int)(P2Left.x + P2Up.x)/2, 0), 255);
                int finalP2Y = min(std::max((int)(P2Left.y + P2Up.y)/2, 0), 255);
                int finalP2Z = min(std::max((int)(P2Left.z + P2Up.z)/2, 0), 255);

                color[2*y][2*x][0] = finalP2X;
                color[2*y][2*x][1] = finalP2Y;
                color[2*y][2*x][2] = finalP2Z;
                color[2*y][2*x][3] = 255;

                // P4
                Vec3 P4Left = CubicInterpolateVec(left, current, right, right, 0.5);
                Vec3 P4Up = CubicInterpolateVec(up, current, down, down, 0.5);

                int finalP4X = min(std::max((int)(P4Left.x + P4Up.x)/2, 0), 255);
                int finalP4Y = min(std::max((int)(P4Left.y + P4Up.y)/2, 0), 255);
                int finalP4Z = min(std::max((int)(P4Left.z + P4Up.z)/2, 0), 255);

                color[2*y][2*x + 1][0] = finalP4X;
                color[2*y][2*x + 1][1] = finalP4Y;
                color[2*y][2*x + 1][2] = finalP4Z;
                color[2*y][2*x + 1][3] = 255;

                // P1
                Vec3 P1Left = CubicInterpolateVec(left, left, current, right, 0.5);
                Vec3 P1Up = CubicInterpolateVec(up, up, current, down, 0.5);

                int finalP1X = min(std::max((int)(P1Left.x + P1Up.x)/2, 0), 255);
                int finalP1Y = min(std::max((int)(P1Left.y + P1Up.y)/2, 0), 255);
                int finalP1Z = min(std::max((int)(P1Left.z + P1Up.z)/2, 0), 255);

                color[2*y + 1][2*x][0] = finalP1X;
                color[2*y + 1][2*x][1] = finalP1Y;
                color[2*y + 1][2*x][2] = finalP1Z;
                color[2*y + 1][2*x][3] = 255;

                // P3
                Vec3 P3Left = CubicInterpolateVec(left, current, right, right, 0.5);
                Vec3 P3Up = CubicInterpolateVec(up, up, current, down, 0.5);

                int finalP3X = min(std::max((int)(P3Left.x + P3Up.x)/2, 0), 255);
                int finalP3Y = min(std::max((int)(P3Left.y + P3Up.y)/2, 0), 255);
                int finalP3Z = min(std::max((int)(P3Left.z + P3Up.z)/2, 0), 255);
                color[2*y + 1][2*x + 1][0] = finalP3X;
                color[2*y + 1][2*x + 1][1] = finalP3Y;
                color[2*y + 1][2*x + 1][2] = finalP3Z;
                color[2*y + 1][2*x + 1][3] = 255;

            }
        }
    }
    glPixelZoom(floor(xRes/max)/2, floor(yRes/max)/2);

    glRasterPos2d(-1.0, -1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels( 2*gWidth, 2*gHeight, GL_RGBA, GL_UNSIGNED_BYTE, color );
    glFlush();
}
