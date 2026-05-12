#include "file_map.h"

#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

file_map::file_map(const char *path, Access access)
    : _data(nullptr), _path(path), _length(0), _descriptor(0), _error(0), _access(access), _message(nullptr), _file_size(0)
{
}

bool file_map::open(size_t offset, size_t length)
{
    // Return safe values on error.
    _error = 0;
    _data = nullptr;
    _length = 0;
    _message = nullptr;

    // offset for mmap() must be page aligned
    size_t pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);
 
    // figure out the flags
    int flags_open = O_RDONLY, flags_map = PROT_READ;
    if (_access == ACCESS_WRITE) {
        flags_open = O_WRONLY;
        flags_map = PROT_WRITE;
    } else if (_access == ACCESS_READ_WRITE) {
        flags_open = O_RDWR;
        flags_map = PROT_READ | PROT_WRITE;
    }

    // Open the file.
    std::cout << "file " << _path;
    _descriptor = ::open(_path.c_str(), flags_open, 0);
    if (_descriptor < 0) {
        _error = errno;
        _message = "from open()";
        std::cout << _message << " error " << _error << std::endl;
        return false;
    }
    
    struct stat info;

    // We now know the file exists. Retrieve the file size.
    if (::fstat(_descriptor, &info) != 0) {
        _error = errno;
        _message = "from fstat()";
        return false;
    }

    // can't display bytes past end of file
    if (offset > info.st_size) {
        _message = "offset starts after end";
        return false;
    }
    _file_size = info.st_size;
    if (!length || offset + length > _file_size)
        length = _file_size - offset;

    _data = ::mmap(nullptr, length + offset - pa_offset, flags_map, MAP_FILE | MAP_SHARED/*MAP_PRIVATE*/, _descriptor, 0);
    if (_data == MAP_FAILED) {
        _data = nullptr;
        _length = 0;
        _error = errno;
        _message = "from mmap()";
        return false;
    }
    
    _length = length;

    // Now close the file. The kernel doesn’t use our file descriptor.
    if (::close(_descriptor) != 0) {
        _data = nullptr;
        _length = 0;
        _error = errno;
        _message = "from close()";
        return false;
    }
    _descriptor = 0;
 
    return true;
}

void file_map::close()
{
    if (_data) {
        ::munmap(_data, _length);
        _data = nullptr;
        _length = 0;
        _file_size = 0;
    }
}
/*
void test_file_map()
{
    const char *filename = "/Users/paul/paul_file.map";
    FILE *file = fopen(filename, "w");
    char buffer[128] = {0};
    fwrite(buffer, 1, sizeof(buffer), file);
    fclose(file);
    file_map *mapper = new file_map(filename, file_map::ACCESS_READ_WRITE);
    if (!mapper->open()) {
        printf("Failed to open file '%s' due to error %d at %s\n", mapper->path(), mapper->error(), mapper->message());
    } else {
        uint32_t *p = mapper->as_uint32();
        for (uint32_t i = 0; i < 10000000; i++)
            (*p)++;
        mapper->close();
    }
}
*/
