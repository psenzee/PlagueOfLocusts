#include <png.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include "lodepng.h"

bool write_png__rgba32(FILE *fp, const uint32_t *data, size_t width, size_t height)
{
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;

    /* Create and initialize the png_struct with the desired error handler
     * functions.  If you want to use the default stderr and longjump method,
     * you can supply NULL for the last three parameters.  We also check that
     * the library version is compatible with the one used at compile time,
     * in case we are using dynamically linked libraries.  REQUIRED.
     */
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        return false;
    }

    /* Allocate/initialize the image information data.  REQUIRED. */
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        fclose(fp);
        png_destroy_write_struct(&png_ptr,  NULL);
        return false;
    }
    png_set_IHDR(png_ptr, info_ptr, png_uint_32(width), png_uint_32(height),
        8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_set_sRGB(png_ptr, info_ptr, PNG_sRGB_INTENT_ABSOLUTE);

    /* Set up error handling.  REQUIRED if you aren't supplying your own
    * error handling functions in the png_create_write_struct() call.
    */
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return false;
    }

    uint8_t **row_pointers = (uint8_t **)png_malloc(png_ptr, height * sizeof(uint8_t *));
    for (size_t i = 0; i < height; i++) {
        uint8_t *row = (uint8_t *)png_malloc(png_ptr, width * sizeof(uint32_t) * height);
        std::memcpy(row, data + width * i, width * sizeof(uint32_t));
        row_pointers[i] = row;
    }
    png_set_rows(png_ptr, info_ptr, row_pointers);

    /* One of the following I/O initialization functions is REQUIRED. */

    /* Set up the output control if you are using standard C streams. */
    png_init_io(png_ptr, fp);

    /* This is the easy way.  Use it if you already have all the
    * image info living in the structure.  You could "|" many
    * PNG_TRANSFORM flags into the png_transforms integer here.
    */
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
    
    for (size_t i = 0; i < height; i++) {
        png_free(png_ptr, row_pointers[i]);
    }
    
    png_free(png_ptr, row_pointers);
    row_pointers = NULL;

    /* Clean up after the write, and free any allocated memory. */
    png_destroy_write_struct(&png_ptr, &info_ptr);

    /* That's it! */
    return true;
}

bool write_png__rgba32(const char *filename, const uint32_t *data, size_t width, size_t height)
{
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return false;
    }
    write_png__rgba32(fp, data, width, height);
    fclose(fp);
    return true;
}

bool write_png__grayscale16(FILE* fp, const uint16_t *data, size_t width, size_t height)
{
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_bytepp row_pointers = NULL;

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        return false;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_write_struct(&png_ptr, NULL);
        return false;
    }

     if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return false;
    }

    png_set_IHDR(png_ptr, info_ptr,
                 png_uint_32(width), png_uint_32(height), // width and height
                 16, // bit depth
                 PNG_COLOR_TYPE_GRAY, // color type
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    /* Initialize rows of PNG. */
    row_pointers = (uint8_t **)png_malloc(png_ptr, height * sizeof(uint8_t *));
    for (size_t i = 0; i < height; i++) {
        uint8_t *row = (uint8_t *)png_malloc(png_ptr, width * sizeof(uint16_t) * height);
        for (size_t x = 0; x < width; x++) {
            uint16_t u = data[x + width * i];
            row[x * 2 + 0] = u >> 8;
            row[x * 2 + 1] = u & 0xff;
        }
        row_pointers[i] = row;
    }

    /* Actually write the image data. */
    png_init_io(png_ptr, fp);
    png_set_rows(png_ptr, info_ptr, row_pointers);
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    /* Cleanup. */
    for (size_t i = 0; i < height; i++) {
        png_free(png_ptr, row_pointers[i]);
    }
    png_free(png_ptr, row_pointers);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return true;
}

bool write_png__grayscale16(const char *filename, const uint16_t *data, size_t width, size_t height)
{
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return false;
    }
    write_png__grayscale16(fp, data, width, height);
    fclose(fp);
    return true;
}

bool read_png__rgba32(FILE *fp, uint32_t **data, size_t *width, size_t *height)
{
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return false;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return false;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return false;
    }

    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);

    *width = png_get_image_width(png_ptr, info_ptr);
    *height = png_get_image_height(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    
    printf("PNG ver %s width %zu height %zu depth %u color type %u (=%u)\n", PNG_LIBPNG_VER_STRING,
           *width, *height, bit_depth, color_type, PNG_COLOR_TYPE_RGBA);
    
    if (color_type == PNG_COLOR_TYPE_RGBA && bit_depth == 16) {
        png_set_strip_16(png_ptr);
    } else if (color_type != PNG_COLOR_TYPE_RGBA || bit_depth != 8) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return false;
    }

    *data = (uint32_t *)malloc(sizeof(uint32_t) * (*width) * (*height));
    if (!*data) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return false;
    }

    png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * (*height));
    if (!row_pointers) {
        free(*data);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return false;
    }

    for (size_t i = 0; i < *height; i++) {
        row_pointers[i] = (png_byte *)malloc(sizeof(uint32_t) * (*width));
        if (!row_pointers[i]){
            // TODO Handle memory allocation failure
            for(size_t j = 0; j < i; j++){
                free(row_pointers[j]);
            }
            free(row_pointers);
            free(*data);
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            return false;
        }
    }

    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY | PNG_TRANSFORM_EXPAND, NULL);

    for (size_t i = 0; i < *height; i++) {
        memcpy(*data + (*width) * i, row_pointers[i], sizeof(uint32_t) * (*width));
        free(row_pointers[i]);
    }

    free(row_pointers);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return true;
}

bool read_png__rgba32_libpng(const char *filename, uint32_t **data, size_t *width, size_t *height)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        throw std::runtime_error("failed to open png file");
    }

    if (!read_png__rgba32(fp, data, width, height)) {
        throw std::runtime_error("failed to read png file");
    }
    
    fclose(fp);
    return true;
}

bool read_png__rgba32(const char *filename, uint32_t **data, size_t *width, size_t *height)
{
    unsigned char *image;
    unsigned uwidth, uheight;
    unsigned error = lodepng_decode32_file(&image, &uwidth, &uheight, filename);
    *data = reinterpret_cast<uint32_t *>(image);
    *width = uwidth;
    *height = uheight;

    if (error) {
        printf("lodepng error %u: %s\n", error, lodepng_error_text(error));
        return false;
    }

    printf("lodepng read successfully: width=%u, height=%u\n", uwidth, uheight);

    free(image);
    return true;
}

void release_read_png_data(uint32_t **data)
{
    free(*data);
    *data = nullptr;
}


// -------

int test_read_png_libpng(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("Error opening PNG file");
        return 1;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return 1;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return 1;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return 1;
    }

    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY | PNG_TRANSFORM_EXPAND, NULL);

    printf("PNG read successfully (if this prints, and there are no other errors)\n");

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    return 0;
}

int test_read_png_lodepng(const char *filename)
{
    unsigned char *image;
    unsigned width, height;
    unsigned error = lodepng_decode32_file(&image, &width, &height, filename);

    if (error) {
        printf("lodepng error %u: %s\n", error, lodepng_error_text(error));
        return 1;
    }

    printf("lodepng read successfully: width=%u, height=%u\n", width, height);

    free(image);
    return 0;
}
