#include "file_mapper_.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

file_mapper::file_mapper(const char *path, Access access)
    : _data(nullptr), _path(path), _length(0), _descriptor(0), _error(0), _access(access), _message(nullptr)
{
}

bool file_mapper::open()
{
    // Return safe values on error.
    _error = 0;
    _data = nullptr;
    _length = 0;
    _message = nullptr;
 
    // figure out the flags
    int flagsOpen = O_RDONLY, flagsMap = PROT_READ;
    if (_access == ACCESS_WRITE) {
        flagsOpen = O_WRONLY;
        flagsMap = PROT_WRITE;
    } else if (_access == ACCESS_READ_WRITE) {
        flagsOpen = O_RDWR;
        flagsMap = PROT_READ | PROT_WRITE;
    }

    // Open the file.
    _descriptor = ::open(_path.c_str(), flagsOpen, 0);
    if (_descriptor < 0) {
        _error = errno;
        _message = "from open()";
        return false;
    }
    
    struct stat info;

    // We now know the file exists. Retrieve the file size.
    if (fstat(_descriptor, &info) != 0) {
        _error = errno;
        _message = "from fstat()";
        return false;
    }

    _data = mmap(nullptr, info.st_size, flagsMap, MAP_FILE | MAP_SHARED, _descriptor, 0);
    if (_data == MAP_FAILED) {
        _data = nullptr;
        _length = 0;
        _error = errno;
        _message = "from mmap()";
        return false;
    }
    
    _length = info.st_size;

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

void file_mapper::close()
{
    munmap(_data, _length);
    _data = nullptr;
    _length = 0;
}

void test_file_mapper()
{
    char path[1024] = {0};
    getcwd(path, 1024);
    sprintf(path, "%s/file.map", path);
    //const char *filename = "/Users/paul/paul_file.map";
    FILE *file = fopen(path, "w");
    if (!file) {
        printf("Failed to create file %s\n", path);
        return;
    }
    printf("\nCreated or opened file %s\n", path);
    char buffer[1024] = {0};
    fwrite(buffer, sizeof(buffer), 1, file);
    fclose(file);
    file_mapper *mapper = new file_mapper(path, file_mapper::ACCESS_READ_WRITE);
    if (!mapper->open()) {
        printf("Failed to open file '%s' due to error %d at %s\n", mapper->path(), mapper->error(), mapper->message());
    } else {
        uint32_t *p = mapper->as_uint32();
        for (uint32_t i = 0; i < 10000000; i++) {
            (*p)++;
        }
        mapper->close();
    }
}
