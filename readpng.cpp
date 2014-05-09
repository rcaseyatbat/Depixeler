#include "readpng.h"

/* Added so we can read EITHER 8-bit or 16-bit png files! */
bool loadPngImage(char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData) {
    png_structp png_ptr;
    png_infop info_ptr;
    unsigned int sig_read = 0;
    int color_type, interlace_type;
    FILE *fp;
 
    if ((fp = fopen(name, "rb")) == NULL)
        return false;
 
    /* Create and initialize the png_struct
     * with the desired error handler
     * functions.  If you want to use the
     * default stderr and longjump method,
     * you can supply NULL for the last
     * three parameters.  We also supply the
     * the compiler header file version, so
     * that we know if the application
     * was compiled with a compatible version
     * of the library.  REQUIRED
     */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                     NULL, NULL, NULL);
 
    if (png_ptr == NULL) {
        fclose(fp);
        return false;
    }
 
    /* Allocate/initialize the memory
     * for image information.  REQUIRED. */
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return false;
    }
 
    /* Set error handling if you are
     * using the setjmp/longjmp method
     * (this is the normal method of
     * doing things with libpng).
     * REQUIRED unless you  set up
     * your own error handlers in
     * the png_create_read_struct()
     * earlier.
     */
    if (setjmp(png_jmpbuf(png_ptr))) {
        /* Free all of the memory associated
         * with the png_ptr and info_ptr */
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        /* If we get here, we had a
         * problem reading the file */
        return false;
    }
 
    /* Set up the output control if
     * you are using standard C streams */
    png_init_io(png_ptr, fp);
 
    /* If we have already
     * read some of the signature */
    png_set_sig_bytes(png_ptr, sig_read);
 
    /*
     * If you have enough memory to read
     * in the entire image at once, and
     * you need to specify only
     * transforms that can be controlled
     * with one of the PNG_TRANSFORM_*
     * bits (this presently excludes
     * dithering, filling, setting
     * background, and doing gamma
     * adjustment), then you can read the
     * entire image (including pixels)
     * into the info structure with this
     * call
     *
     * PNG_TRANSFORM_STRIP_16 |
     * PNG_TRANSFORM_PACKING  forces 8 bit
     * PNG_TRANSFORM_EXPAND forces to
     *  expand a palette into RGB
     */
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);
 
    png_uint_32 width, height;
    int bit_depth;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_type, NULL, NULL);
    outWidth = width;
    outHeight = height;

    
        switch(color_type)
        {
                case PNG_COLOR_TYPE_RGBA:
                        outHasAlpha = true;
                        break;
                case PNG_COLOR_TYPE_RGB:
                        outHasAlpha = false;
                        break;
                default:
                        printf("%s: color type: %d not supported\n", __FILE__, color_type);
                        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                        fclose(fp);
                        return false;
        }


 
    unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    *outData = (unsigned char*) malloc(row_bytes * outHeight);
 
    png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);
 
    for (int i = 0; i < outHeight; i++) {
        // note that png is ordered top to
        // bottom, but OpenGL expect it bottom to top
        // so the order or swapped
        memcpy(*outData+(row_bytes * (outHeight-1-i)), row_pointers[i], row_bytes);
    }
 
    /* Clean up after the read,
     * and free any memory allocated */
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
 
    /* Close the file */
    fclose(fp);
 
    /* That's it */
    return true;
}

/* Ripped from the libpng manual */
png_bytepp readpng(const char *filename, int* width, int* height) {
   FILE *fp = fopen(filename, "rb");
   char header[8];
   png_structp png_ptr;
   png_infop info_ptr, end_info;

   if (!fp) {
      fprintf(stderr, "%s ", filename);
      perror("fopen");
      return NULL;
   }
   fread(header, 1, 8, fp);
   if(png_sig_cmp((png_byte*)header, 0, 8))
   {
      fprintf(stderr, "%s: Not a PNG image!\n", filename);
      return NULL;
   }

   png_ptr = png_create_read_struct(
      PNG_LIBPNG_VER_STRING,
      NULL, NULL, NULL);
   if(!png_ptr)
      return NULL;

   info_ptr = png_create_info_struct(png_ptr);
   if(!info_ptr) {
      png_destroy_read_struct(&png_ptr, NULL, NULL);
      return NULL;
   }

   end_info = png_create_info_struct(png_ptr);
   if(!end_info) {
      png_destroy_read_struct(&png_ptr, NULL, NULL);
      return NULL;
   }

   /* Set up jump target for libpng errors */
   if (setjmp(png_jmpbuf(png_ptr)))
   {
      png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
      fprintf(stderr, "libpng error!\n");
      fclose(fp);
      return NULL;
   }

   png_init_io(png_ptr, fp);
   png_set_sig_bytes(png_ptr, 8);
   png_read_png(png_ptr, info_ptr, 0, NULL);

   /* Make sure the image is in the format we want */
   *width = png_get_image_width(png_ptr, info_ptr);
   *height = png_get_image_height(png_ptr, info_ptr);
   if(png_get_bit_depth(png_ptr, info_ptr) != 8 ||
      png_get_color_type(png_ptr, info_ptr) != PNG_COLOR_TYPE_RGB)
      fprintf(stderr, "Need an 8 bit/color RGB image!!\n");

   fclose(fp);  // close the file so we don't have too many open

   /* Warning!  We leak these data structures!!! */
   return png_get_rows(png_ptr, info_ptr);
}

#ifdef PNGDEBUG
int main(int argc, char *argv[]) {
   int w, h, y;
   if(argc != 2) {
      fprintf(stderr, "wrong number of args\n");
      return 1;
   }
   png_bytepp p = readpng(argv[1], &w, &h);
   printf("P6\n%i %i\n255\n", w, h);
   for(y=0; y<h; y++)
      fwrite(p[y], 3, w, stdout);
   return 0;
}
#endif