#include "file.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <algorithm>

namespace file
{

enum { MAX_PATH = 1024 };

buffer_t read_file(const char *filename, bool add_null_terminator)
{
    buffer_t data;
    if (filename) {
	    int handle = ::open(filename, O_RDONLY /* | O_BINARY ; windows */);
	    if (handle >= 0) {
            data.size = file_size(handle);
            data.data = new char [data.size + (add_null_terminator ? 1 : 0)]; // the +1 we null terminate, convenient if it's a string.
            size_t sz = ::read(handle, data.data, (unsigned)data.size);
            if (sz != data.size) {
                delete [] (char *)data.data;
                data.data = 0;
                data.size = 0;
            }
            if (add_null_terminator)
                ((char *)data.data)[sz] = 0;
            ::close(handle);
		}
	}
	return data;
}

ssize_t write_file(const char *filename, const void *buffer, size_t size)
{
    auto handle = ::open(filename, O_WRONLY | O_TRUNC | O_CREAT /* | _O_BINARY ; windows */, S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH | S_IRUSR | S_IWUSR);
    if (handle < 0) {
        fprintf(stderr, "error: can't open output file %s\n\n", filename);
        return ssize_t(handle);
    }
    std::cout << "file writing " << size << " bytes" << std::endl;
    const uint8_t *p = reinterpret_cast<const uint8_t *>(buffer);
    size_t written = 0;
    while (size) {
        size_t chunk = std::min(size, size_t(0x7fffffff));
        std::cout << "writing chunk " << chunk << " of remaining bytes " << size << std::endl;
        ssize_t write_sz = !written ? ::write(handle, p, chunk) : ::pwrite(handle, p, chunk, written);
        size -= chunk;
        written += chunk;
        p += chunk;
        if (write_sz < chunk) {
            if (write_sz <  0) fprintf(stderr, "error: write returned %d for %s\n\n", int(write_sz), filename);
            if (write_sz == 0) fprintf(stderr, "error: can't write to output file %s\n\n", filename);
            else               fprintf(stderr, "error: only wrote %u bytes of %u to output file %s\n\n", unsigned(write_sz), unsigned(chunk), filename);
            ::close(handle);
            return write_sz;
        }
    }
    ::fsync(handle);
    ::close(handle);
    return 0;
}

bool stat(const char *fn, struct stat *st)
{
    if (!fn || !*fn || !st)
        return false;
    // see MSDN Q168439 - windows
    size_t len = strlen(fn);
    char str[MAX_PATH] = { 0 };
    strcpy(str, fn);
    if (str[len - 1] == '\\')
        str[len - 1] = 0;
    memset(st, 0, sizeof(struct stat));
    return ::stat(str, st) == 0;
}

bool stat(int fd, struct stat *st)
{
    if (!st) return false;
    memset(st, 0, sizeof(struct stat));
    return ::fstat(fd, st) == 0;
}

bool is_directory(const char *path)
{
    if (!path || !*path) return false;
    struct stat st;
    return ::stat(path, &st) && (st.st_mode & S_IFDIR) > 0;
}

bool is_directory(int fd)
{
    if (!fd) return false;
    struct stat st;
    return stat(fd, &st) && (st.st_mode & S_IFDIR) > 0;
}

time_t time_modified(const char *fn)
{
    if (!fn || !*fn) return 0;
    struct stat st;
    return !::stat(fn, &st) ? 0 : st.st_mtime;
}

time_t time_modified(int fd)
{
    if (!fd) return 0;
    struct stat st;
    return !stat(fd, &st) ? 0 : st.st_mtime;
}

bool path_exists(const char *path)
{
    return ::access(path, 0) == 0;
}

bool path_exists(int fd)
{
    struct stat st;
    return fd && stat(fd, &st);
}

bool read_only(const char *path)
{
    return ::access(path, 4) == 0;
}

ssize_t file_size(const char *fn)
{
    struct stat st;
    return fn && *fn && ::stat(fn, &st) ? st.st_size : 0;
}

ssize_t file_size(int fd)
{
    struct stat st;
    return fd && stat(fd, &st) ? st.st_size : 0;
}

}
