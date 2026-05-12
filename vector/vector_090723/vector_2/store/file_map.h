#pragma once

#include <stddef.h>
#include <stdint.h>
#include <string>

class file_map
{
public:

    enum Access
    {
        ACCESS_READ       = 1,
        ACCESS_WRITE      = 2,
        ACCESS_READ_WRITE = ACCESS_READ | ACCESS_WRITE
    };

    file_map(const char *path, Access access = ACCESS_READ);

    inline int             error()     const { return _error; }
    inline const char     *message()   const { return _message; }
    inline void           *data()            { return _data; }
    inline const void     *data()      const { return _data; }
    inline const char     *path()      const { return _path.c_str(); }
    inline size_t          file_size() const { return _file_size; }
    
    bool open(size_t offset = 0, size_t length = 0);
    void close();

private:

    std::string  _path;
    void        *_data;
    size_t       _length;
    size_t       _file_size;
    int          _descriptor;
    int          _error;
    const char * _message;
    Access       _access;
};

extern void test_file_map();

