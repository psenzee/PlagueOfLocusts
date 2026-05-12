#pragma once

#include <stddef.h>
#include <stdint.h>
#include <string>

class FileMapper
{
public:

    enum Access
    {
        ACCESS_READ       = 1,
        ACCESS_WRITE      = 2,
        ACCESS_READ_WRITE = ACCESS_READ | ACCESS_WRITE
    };

    FileMapper(const char *path, Access access = ACCESS_READ);
    
    inline int             error()     const { return _error; }
    inline const char     *message()   const { return _message; }
    inline void           *data()            { return _data; }
    inline char           *as_char()         { return reinterpret_cast<char *>(_data); }
    inline uint32_t       *as_uint32()       { return reinterpret_cast<uint32_t *>(_data); }
    inline const void     *data()      const { return _data; }
    inline const char     *as_char()   const { return reinterpret_cast<const char *>(_data); }
    inline const uint32_t *as_uint32() const { return reinterpret_cast<const uint32_t *>(_data); }
    inline const char     *path()      const { return _path.c_str(); }
    
    bool open();
    void close();

private:

    std::string  _path;
    void        *_data;
    size_t       _length;
    int          _descriptor;
    int          _error;
    const char * _message;
    Access       _access;
};

extern void test_file_mapper();
