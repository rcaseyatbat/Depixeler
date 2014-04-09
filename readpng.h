#ifndef __READPNG_H
#define __READPNG_H
#include <png.h>

#ifdef __cplusplus
extern "C" {
#endif

  png_bytepp readpng(const char *filename, int* width, int* height);

#ifdef __cplusplus
}
#endif

#endif