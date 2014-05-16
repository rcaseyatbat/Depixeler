#ifndef __READPNG_H
#define __READPNG_H
#include <png.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGl/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

  png_bytepp readpng(const char *filename, int* width, int* height);
  bool loadPngImage(char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData);

#ifdef __cplusplus
}
#endif

#endif