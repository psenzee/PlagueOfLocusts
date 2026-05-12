#pragma once

#include <cstdint>
#include <cstdio>
#include <string>

// todo update to use ostringstream
struct string_out_stream : public binary_out_stream
{
    size_t             write(const void *u, std::size_t bytes)       { _str.append((const char *)u, bytes); return bytes; }
    void               close()                                       {}
    void               flush()                                       {}
    std::string_view   str()                                   const { return _str; }

private:

    std::string _str;
};
