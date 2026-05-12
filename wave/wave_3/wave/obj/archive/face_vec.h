#pragma once

#include "face.h"
#include <vector>

struct face_vec : public face
{
    void          push_back(const vertex &u)       { _vertices.push_back(u); };
    const vertex &at(std::size_t i)          const { return _vertices[i]; }
    std::size_t   size()                     const { return _vertices.size(); };
    
private:

    std::vector<vertex> _vertices;
};
