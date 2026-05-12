#pragma once

#include "file.h"

#include <map>
#include <string>
#include <string_view>

class bundle
{
public:
    
    typedef std::map<std::string, const uint8_t *> index_t;

    ~bundle();

    bool             load(std::string_view filename);
    std::string_view get_file_name() const;
    void             unload();

    const uint8_t   *get_file(std::string_view filename) const;
    ssize_t          get_size(std::string_view filename) const;

    // temp
    void             test();

private:

    bool             _read_index();

    file::buffer_t   _data;
    index_t          _index;
    std::string      _filename;
};
