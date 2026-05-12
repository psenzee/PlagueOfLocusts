#pragma once

#include "vertex.h"
#include "face.h"
#include "face_n.h"
#include "face_vec.h"
#include <vector>

struct face_list
{

    inline void push_back(const face &u)
    {
        if (u.size() > 4) {
            copy_face(u, _fv);
        } else {
            copy_face(u, _fn);
        }
    }
    
    inline void push_back(const face_n<4> &u) { push_face(u, _fn); }
    inline void push_back(const face_vec &u)  { push_face(u, _fv); }

private:
    
    template <typename T> inline void push_face(const T &u, std::vector<T> &vec)
    {
        vec.push_back(u);
        list_count_t lc(vec.size(), u.size());
        _faces.push_back(lc);
    }

    template <typename T> inline void copy_face(const face &u, std::vector<T> &vec)
    {
        vec.push_back(T());
        T &face = vec.back();
        face.normal = u.normal;
        std::size_t sz = u.size();
        list_count_t lc(vec.size(), sz);
        for (std::size_t i = 0; i != sz; ++i) {
            face.push_back(u.at(i));
        }
        _faces.push_back(lc);
    }

    typedef std::pair<uint32_t, uint32_t> list_count_t;

    std::vector< face_n<4> >    _fn;
    std::vector< face_vec >     _fv;
    std::vector< list_count_t > _faces;
};

