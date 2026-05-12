#pragma once

#include <cstdint>
#include <cstdio>
#include "binary_stream.h"

struct file_in_stream : public binary_in_stream
{
    inline file_in_stream(FILE *file) : _file(file) {}
    file_in_stream(const char *path);

    std::size_t read(void *u, std::size_t bytes);
    void        close();
    
private:
    
    FILE *_file;
};

struct file_out_stream : public binary_out_stream
{
    inline file_out_stream(FILE *file) : _file(file) {}
    file_out_stream(const char *path);

    std::size_t write(const void *u, std::size_t bytes);
    void        close();
    void        flush();
    
private:
    
    FILE *_file;
};
