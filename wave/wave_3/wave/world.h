#pragma once

#include <iostream>

#include "renderer.h"
#include "voronoi.h"
#include "eroder.h"
#include "octave_noise.h"
#include "obj_file.h"
#include "vertex.h"
#include "std_array.h"

namespace wave
{

typedef std::array<double, 3> vec3_t;
typedef std::array<double, 2> vec2_t;

struct world
{
    typedef std::array<double, 2> vec2_t;
    typedef std::array<double, 2> range_t;
    
    typedef std::array<float, 2> fvec2_t;
    typedef std::array<float, 3> fvec3_t;
    typedef std::array<float, 4> fvec4_t;

    grid::basic<double>         _elevation;
    std::mt19937                _mt;
    point2i                     _offset;
    double                      _scale;
    renderer                    _renderer;
    
    world(const size2u &s, const point2i &offset, const std::mt19937 &mt, double scale = 1.0) :
        _elevation(s), _mt(mt), _scale(scale), _renderer(_elevation), _offset(offset)
    {
        clear(_elevation, 0.);
    }
    
    inline size2u size() const { return _elevation.size(); }
    
    inline range_t elevation_range() const { return range(_elevation); }
    
    void add_ridges(size_t octaves = 5, double frequency = 0.005, const range_t &slice_range = { 0.1, 0.5 })
    {
        noise::octave_noise(_elevation, _offset.array<double>(), octaves, frequency / _scale, _mt);
        auto sz = _elevation.size();
        for (point2i p(0, 0); p.y < sz.h; p.y++) {
            for (p.x = 0; p.x < sz.w; p.x++) {
                point2i q(p + _offset);
                double v = _elevation.get(q),
                       u = clamp(((v > slice_range[0] && v < slice_range[1]) ? -v : 0.), -1., 0.);
                _elevation.set(q, u);
            }
        }
    }
    
    void smooth(std::size_t iterations = 1)
    {
        for (size_t i = 0; i < iterations; i++) {
            gaussian_smooth_5x5(_elevation);
        }
    }
    
    void add_landscape(size_t octaves = 8, double frequency = 0.0012)
    {
        noise::octave_noise(_elevation, _offset.array<double>(), octaves, frequency / _scale, _mt);
    }
    
    void rain()
    {
        rainer rs(_elevation, _mt);
        rs.rain();
    }
    
    void add_voronoi()
    {
        noise::voronoi<double> w(12, _offset.array<double>(), 2048);
        size2u sz(_elevation.size());
        for (point2i p(0, 0); p.y < sz.h; p.y++) {
            for (p.x = 0; p.x < sz.w; p.x++) {
                point2i q(p + _offset);
                vec2_t v({ double(q.x), double(q.y) });
                _elevation.set(q, _elevation.get(q) + abs(w.evaluate(v) * 0.5));
            }
        }
    }
    
    void save_rgba_png(const char *file)
    {
        _renderer.prerender();
        _renderer.save_rgba_png(file);
    }
    
    void save_height_map_png(const char *file)
    {
        _renderer.save_height_map_png(file);
    }

    void normalize_elevation()
    {
        normalize(_elevation, 0., 1.);
    }
    
    void create_world()
    {
        add_ridges();
        add_landscape();
        add_voronoi();
        rain();
        smooth();
    }
    
    template <typename T, typename U> constexpr fvec3_t _vec(const point2i &p, T max_a, U max_b)
    {
        return fvec3_t ({ float(p.x), float(p.y), std::max(float(max_a), float(max_b)) });
    }
    
    constexpr fvec2_t _uv(const fvec3_t &v, const fvec2_t &scale)
    {
        return fvec2_t ({ v[0] * scale[0], 1.f - v[1] * scale[1] });
    }
    
    void _add_face(const point2i &p, const size2u &step, const fvec3_t &scale, const fvec2_t &uv_scale, face_list &list)
    {
        face f;
        point2i q(int(p.x + step.w), p.y),
                r(int(p.x + step.w), int(p.y + step.h)),
                s(p.x, int(p.y + step.h));
        fvec3_t a(_vec(p, _elevation.get(p), 0.)),
                b(_vec(q, _elevation.get(q), 0.)),
                c(_vec(r, _elevation.get(r), 0.)),
                d(_vec(s, _elevation.get(s), 0.));
        f.vertices.push_back(vertex(a * scale, _uv(a, uv_scale)));
        f.vertices.push_back(vertex(b * scale, _uv(b, uv_scale)));
        f.vertices.push_back(vertex(c * scale, _uv(c, uv_scale)));
        f.vertices.push_back(vertex(d * scale, _uv(d, uv_scale)));
        list.faces.push_back(f);
    }
    
    void save_obj(const char *file)
    {
        size2u step(16, 16), sz(_elevation.size());
        fvec2_t uv_scale { 1.f / sz.w, 1.f / sz.h };
        face_list faces;
        fvec3_t scale { 1.f, 1.f, 128.f };
        for (point2i p(0, 0); p.y < sz.h; p.y += step.h) {
            for (p.x = 0; p.x < sz.w; p.x += step.w) {
                _add_face(p, step, scale, uv_scale, faces);
            }
        }
        obj_file objf;
        objf.set_materials("nu5map.mtl");
        objf.add_group("numap", "nu5map", faces);
        objf.write_file(file);
    }

private:
};

}
