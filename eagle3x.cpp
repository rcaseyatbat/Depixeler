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

void eagle3x(int gHeight, int gWidth, int h, int w, int xRes, int yRes, int max, unsigned char gData[])
{
    GLubyte color[3*gHeight][3*gWidth][4];

    glClearColor(0.0, 0.0, 0.0, 0.0);

    for(int y = 0; y < gHeight; y++) {
        for (int x = 0; x < gWidth; x ++) {
            int index = 4 * x + (4 * w * y);


            // first set all pixels to original
            for (int xPixel = 0; xPixel < 3; xPixel++) {
                for (int yPixel = 0; yPixel < 3; yPixel++) {
                    color[3*y + yPixel][3*x + xPixel][0] = int(gData[index]);
                    color[3*y + yPixel][3*x + xPixel][1] = int(gData[index + 1]);
                    color[3*y + yPixel][3*x + xPixel][2] = int(gData[index + 2]);
                    color[3*y + yPixel][3*x + xPixel][3] = int(gData[index + 3]);
                }
            }

            /*
            A|B|C
            -----
            D|E|F
            -----
            G|H|J

            E0|E1|E2
            --------
            E3|E4|E5
            --------
            E6|E7|E8
            */

            int upIndex = getUpNeighbor(x,y, gWidth, gHeight);
            int downIndex = getDownNeighbor(x,y, gWidth, gHeight);
            int leftIndex = getLeftNeighbor(x,y, gWidth, gHeight);
            int rightIndex = getRightNeighbor(x,y, gWidth, gHeight);
        
            int upleftIndex = getUpLeftNeighbor(x, y, gWidth, gHeight);
            int uprightIndex = getUpRightNeighbor(x, y, gWidth, gHeight);
            int downleftIndex = getDownLeftNeighbor(x, y, gWidth, gHeight);
            int downrightIndex = getDownRightNeighbor(x, y, gWidth, gHeight);

            Vec3 A, B, C, D, E, F, G, H, J;

            B.x = gData[upIndex];
            B.y = gData[upIndex+1];
            B.z = gData[upIndex+2];
            D.x = gData[leftIndex];
            D.y = gData[leftIndex+1];
            D.z = gData[leftIndex+2];
            F.x = gData[rightIndex];
            F.y = gData[rightIndex+1];
            F.z = gData[rightIndex+2];
            H.x = gData[downIndex];
            H.y = gData[downIndex+1];
            H.z = gData[downIndex+2];

            E.x = gData[index];
            E.y = gData[index + 1];
            E.z = gData[index + 2];

            A.x = gData[upleftIndex];
            A.y = gData[upleftIndex + 1];
            A.z = gData[upleftIndex + 2];
            C.x = gData[uprightIndex];
            C.y = gData[uprightIndex + 1];
            C.z = gData[uprightIndex + 2];
            G.x = gData[downleftIndex];
            G.y = gData[downleftIndex + 1];
            G.z = gData[downleftIndex + 2];
            J.x = gData[downrightIndex];
            J.y = gData[downrightIndex + 1];
            J.z = gData[downrightIndex + 2];


            if (leftIndex != -1 && upIndex != -1 && downIndex != -1 && rightIndex != -1) {
                // if we are not on a border, we can do the algorithm

                // if bottom left 3 pixels are equal
                if (D == G && G == H) {
                    // set bottom left pixel to that color
                    color[3*y][3*x][0] = int(D.x);
                    color[3*y][3*x][1] = int(D.y);

                    color[3*y][3*x][2] = int(D.z);
                    color[3*y][3*x][3] = int(gData[leftIndex+3]);
                }

                // if bottom right 3 pixels are equal
                if (F == J && F == H) {
                    // set bottom right pixel to that color
                    color[3*y][3*x + 2][0] = int(F.x);
                    color[3*y][3*x + 2][1] = int(F.y);

                    color[3*y][3*x + 2][2] = int(F.z);
                    color[3*y][3*x + 2][3] = int(gData[rightIndex+3]);
                }

                // if top right 3 pixels are equal
                if (B == C && C == F) {
                    // set top right pixel to that color
                    color[3*y + 2][3*x + 2][0] = int(F.x);
                    color[3*y + 2][3*x + 2][1] = int(F.y);

                    color[3*y + 2][3*x + 2][2] = int(F.z);
                    color[3*y + 2][3*x + 2][3] = int(gData[rightIndex+3]);
                }


                // if top left 3 pixels are equal
                if (B == A && B == D) {
                    // set top left pixel to that color
                    color[3*y + 2][3*x][0] = int(D.x);
                    color[3*y + 2][3*x][1] = int(D.y);

                    color[3*y + 2][3*x][2] = int(D.z);
                    color[3*y + 2][3*x][3] = int(gData[leftIndex+3]);
                }

                // if bottom 5 pixels are equal
                if (D == G && G == H && H == J && J == F) {
                    // set bottom pixel to that color
                    color[3*y][3*x + 1][0] = int(D.x);
                    color[3*y][3*x + 1][1] = int(D.y);

                    color[3*y][3*x + 1][2] = int(D.z);
                    color[3*y][3*x + 1][3] = int(gData[leftIndex+3]);
                }

                // if top 5 pixels are equal
                if (D == A && A == B && B == C && C == F) {
                    // set top pixel to that color
                    color[3*y + 2][3*x + 1][0] = int(D.x);
                    color[3*y + 2][3*x + 1][1] = int(D.y);

                    color[3*y + 2][3*x + 1][2] = int(D.z);
                    color[3*y + 2][3*x + 1][3] = int(gData[leftIndex+3]);
                }

                // if right 5 pixels are equal
                if (B == C && C == F && F == J && J == H) {
                    // set right pixel to that color
                    color[3*y + 1][3*x + 2][0] = int(F.x);
                    color[3*y + 1][3*x + 2][1] = int(F.y);

                    color[3*y + 1][3*x + 2][2] = int(F.z);
                    color[3*y + 1][3*x + 2][3] = int(gData[rightIndex+3]);
                }

                // if left 5 pixels are equal
                if (B == A && A == D && D == G && G == H) {
                    // set right pixel to that color
                    color[3*y + 1][3*x][0] = int(D.x);
                    color[3*y + 1][3*x][1] = int(D.y);

                    color[3*y + 1][3*x][2] = int(D.z);
                    color[3*y + 1][3*x][3] = int(gData[leftIndex+3]);
                }
            }
        }
    }
    glPixelZoom(floor(xRes/max)/3, floor(yRes/max)/3);

    glRasterPos2d(-1.0, -1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels( 3*gWidth, 3*gHeight, GL_RGBA, GL_UNSIGNED_BYTE, color );
    glFlush();
}
