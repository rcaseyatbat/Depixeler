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

void eagle(int gHeight, int gWidth, int h, int w, int xRes, int yRes, int max, unsigned char gData[])
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

            // this is for the corner cases
            
            if ((upIndex == -1 && leftIndex == -1) ||
                (upIndex == -1 && rightIndex == -1) ||
                (downIndex == -1 && leftIndex == -1) ||
                (downIndex == -1 && rightIndex == -1))
            {
            
            //if (upIndex == -1 || leftIndex == -1 || rightIndex == -1 || downIndex == -1) {
                for (int xPixel = 0; xPixel < 2; xPixel++) {
                    for (int yPixel = 0; yPixel < 2; yPixel++) {
                        color[2*y + yPixel][2*x + xPixel][0] = int(gData[index]);
                        color[2*y + yPixel][2*x + xPixel][1] = int(gData[index + 1]);
                        color[2*y + yPixel][2*x + xPixel][2] = int(gData[index + 2]);
                        color[2*y + yPixel][2*x + xPixel][3] = int(gData[index + 3]);
                    }
                }
            }

            else //if ((B != H) && (D != F))
            {    

                //if (!(updown) && !(leftright)) {
                if ((D == G) && (G == H)) {
                    color[2*y][2*x][0] = int(G.x);
                    color[2*y][2*x][1] = int(G.y);
                    color[2*y][2*x][2] = int(G.z);
                    color[2*y][2*x][3] = int(gData[leftIndex+3]);
                } 

                if ((F == J) && (J == H)) {
                    color[2*y][2*x + 1][0] = int(J.x);
                    color[2*y][2*x + 1][1] = int(J.y);
                    color[2*y][2*x + 1][2] = int(J.z);
                    color[2*y][2*x + 1][3] = int(gData[rightIndex+3]);
                } 

                if ((B == A) && (A == D)) {
                    color[2*y + 1][2*x][0] = int(A.x);
                    color[2*y + 1][2*x][1] = int(A.y);
                    color[2*y + 1][2*x][2] = int(A.z);
                    color[2*y + 1][2*x][3] = int(gData[rightIndex+3]);
                }

                if ((B == C) && (C == F)) {
                    color[2*y + 1][2*x + 1][0] = int(C.x);
                    color[2*y + 1][2*x + 1][1] = int(C.y);
                    color[2*y + 1][2*x + 1][2] = int(C.z);
                    color[2*y + 1][2*x + 1][3] = int(gData[leftIndex+3]);
                }
                //}
            }
        }
    }
    glPixelZoom(floor(xRes/max)/2, floor(yRes/max)/2);

    glRasterPos2d(-1.0, -1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels( 2*gWidth, 2*gHeight, GL_RGBA, GL_UNSIGNED_BYTE, color );
    glFlush();
}
