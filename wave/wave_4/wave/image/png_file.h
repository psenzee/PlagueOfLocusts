#pragma once

#include <cstdio>
#include <cstring>
#include <cstdlib>

extern bool write_png__rgba32(FILE *fp, const uint32_t *data, std::size_t width, std::size_t height);
extern bool write_png__rgba32(const char *filename, const uint32_t *data, std::size_t width, std::size_t height);
extern bool write_png__grayscale16(FILE* fp, const uint16_t *data, std::size_t width, std::size_t height);
extern bool write_png__grayscale16(const char *filename, const uint16_t *data, std::size_t width, std::size_t height);
extern bool read_png__rgba32_libpng(FILE *fp, uint32_t **data, std::size_t *width, std::size_t *height);
extern bool read_png__rgba32(const char *filename, uint32_t **data, std::size_t *width, std::size_t *height);
extern void release_read_png_data(uint32_t **data);

extern int test_read_png_libpng(const char *filename);
extern int test_read_png_lodepng(const char *filename);
