#include "readpng.h"

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