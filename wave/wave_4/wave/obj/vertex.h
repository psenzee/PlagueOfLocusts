#pragma once

#include <array>

struct vertex
{
    typedef std::array<float, 3> vec3_t;
    typedef std::array<float, 2> vec2_t;

    vec3_t position,
           normal;
    vec2_t uv;
    
    inline vertex(const vec3_t &pos, const vec2_t &uv, const vec3_t &normal) : position(pos), uv(uv), normal(normal) {}
    inline vertex(const vec3_t &pos, const vec2_t &uv) : position(pos), uv(uv), normal({ 0. }) {}
    inline vertex(const vec3_t &pos) : position(pos), uv({ 0. }), normal({ 0. }) {}
    inline vertex() : position({ 0. }), uv({ 0. }), normal({ 0. }) {}
};
