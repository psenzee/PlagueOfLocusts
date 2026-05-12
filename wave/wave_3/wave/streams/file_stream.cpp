#include "file_stream.h"

#include <cstdio>
#include <string>
#include <exception>
#include <stdexcept>

file_in_stream::file_in_stream(const char *path) : _file(0)
{
    _file = ::fopen(path, "rb");
    if (!_file) {
        throw std::runtime_error("error: can't open file.");
    }
}

void file_in_stream::close()
{
    if (_file) {
        ::fclose(_file);
    }
    _file = 0;
}

std::size_t file_in_stream::read(void *u, std::size_t bytes)
{
    return !_file ? 0 : (std::size_t)::fread(u, 1, bytes, _file);
}

file_out_stream::file_out_stream(const char *path) : _file(0)
{
    _file = ::fopen(path, "wb");
    if (!_file) {
        throw std::runtime_error("error: can't open file.");
    }
}

void file_out_stream::close()
{
    if (_file) {
        ::fclose(_file);
    }
    _file = 0;
}

void file_out_stream::flush()
{
    if (_file) {
        ::fflush(_file);
    }
}

std::size_t file_out_stream::write(const void *u, std::size_t bytes)
{
    return !_file ? 0 : (std::size_t)::fwrite(u, 1, bytes, _file);
}
