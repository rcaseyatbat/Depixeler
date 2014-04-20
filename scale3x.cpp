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

void scale3x(int gHeight, int gWidth, int h, int w, int xRes, int yRes, int max, unsigned char gData[])
{
    GLubyte color[3*gHeight][3*gWidth][4];

    glClearColor(0.0, 0.0, 0.0, 0.0);
    //glVertex3f(0.95,-0.95,0);

    //GLubyte color[gHeight][gWidth][4];
    for(int y = 0; y < gHeight; y++) {
        for (int x = 0; x < 3 * gWidth; x += 3) {
            int index = x + (3 * w * y);

            for (int xPixel = 0; xPixel < 3; xPixel++) {
                for (int yPixel = 0; yPixel < 3; yPixel++) {
                    color[3*y + yPixel][3*x/3 + xPixel][0] = int(gData[index]);
                    color[3*y + yPixel][3*x/3 + xPixel][1] = int(gData[index + 1]);
                    color[3*y + yPixel][3*x/3 + xPixel][2] = int(gData[index + 2]);
                    color[3*y + yPixel][3*x/3 + xPixel][3] = 0;
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

            int upIndex = getUpNeighbor(x/3,y, gWidth, gHeight);
            int downIndex = getDownNeighbor(x/3,y, gWidth, gHeight);
            int leftIndex = getLeftNeighbor(x/3,y, gWidth, gHeight);
            int rightIndex = getRightNeighbor(x/3,y, gWidth, gHeight);
        
            int upleftIndex = getUpLeftNeighbor(x/3, y, gWidth, gHeight);
            int uprightIndex = getUpRightNeighbor(x/3, y, gWidth, gHeight);
            int downleftIndex = getDownLeftNeighbor(x/3, y, gWidth, gHeight);
            int downrightIndex = getDownRightNeighbor(x/3, y, gWidth, gHeight);

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
                for (int xPixel = 0; xPixel < 3; xPixel++) {
                    for (int yPixel = 0; yPixel < 3; yPixel++) {
                        color[3*y + yPixel][3*x/3 + xPixel][0] = int(gData[index]);
                        color[3*y + yPixel][3*x/3 + xPixel][1] = int(gData[index + 1]);
                        color[3*y + yPixel][3*x/3 + xPixel][2] = int(gData[index + 2]);
                        color[3*y + yPixel][3*x/3 + xPixel][3] = 0;
                    }
                }
            }

            else if ((B != H) && (D != F)) {

                //color[3*y + 1][3*x/3 + 1][0] = int(E.x);
                //color[3*y + 1][3*x/3 + 1][1] = int(E.y);

                //color[3*y + 1][3*x/3 + 1][2] = int(E.z);
                //color[3*y + 1][3*x/3 + 1][3] = 0;

                if (D == H) {
                    color[3*y][3*x/3][0] = int(D.x);
                    color[3*y][3*x/3][1] = int(D.y);

                    color[3*y][3*x/3][2] = int(D.z);
                    color[3*y][3*x/3][3] = 0;
                }

                if ((D == H && E != J) || (H == F && E != G)) {
                    color[3*y][3*x/3 + 1][0] = int(H.x);
                    color[3*y][3*x/3 + 1][1] = int(H.y);

                    color[3*y][3*x/3 + 1][2] = int(H.z);
                    color[3*y][3*x/3 + 1][3] = 0;
                }

                if (H == F) {
                    color[3*y][3*x/3 + 2][0] = int(F.x);
                    color[3*y][3*x/3 + 2][1] = int(F.y);

                    color[3*y][3*x/3 + 2][2] = int(F.z);
                    color[3*y][3*x/3 + 2][3] = 0;
                }

                if ((D == B && E != G) || (D == H && E != A)) {
                    color[3*y + 1][3*x/3][0] = int(D.x);
                    color[3*y + 1][3*x/3][1] = int(D.y);

                    color[3*y + 1][3*x/3][2] = int(D.z);
                    color[3*y + 1][3*x/3][3] = 0;
                }

                if ((B == F && E != J) || (H == F && E != C)) {
                    color[3*y + 1][3*x/3 + 2][0] = int(F.x);
                    color[3*y + 1][3*x/3 + 2][1] = int(F.y);

                    color[3*y + 1][3*x/3 + 2][2] = int(F.z);
                    color[3*y + 1][3*x/3 + 2][3] = 0;
                }

                if (D == B) {
                    color[3*y + 2][3*x/3][0] = int(D.x);
                    color[3*y + 2][3*x/3][1] = int(D.y);

                    color[3*y + 2][3*x/3][2] = int(D.z);
                    color[3*y + 2][3*x/3][3] = 0;
                } 

                if ((D == B && E != C) || (B == F && E != A)) {
                    color[3*y + 2][3*x/3 + 1][0] = int(B.x);
                    color[3*y + 2][3*x/3 + 1][1] = int(B.y);

                    color[3*y + 2][3*x/3 + 1][2] = int(B.z);
                    color[3*y + 2][3*x/3 + 1][3] = 0;
                } 

                if (B == F) {
                    color[3*y + 2][3*x/3 + 2][0] = int(F.x);
                    color[3*y + 2][3*x/3 + 2][1] = int(F.y);

                    color[3*y + 2][3*x/3 + 2][2] = int(F.z);
                    color[3*y + 2][3*x/3 + 2][3] = 0;
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
