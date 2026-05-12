#pragma once

#include <iostream>

#include "renderer.h"
#include "voronoi.h"
#include "eroder.h"
#include "octave_noise.h"

struct vertex
{
    typedef std::array<double, 3> pos_t;
    typedef std::array<double, 2> uv_t;
    
    pos_t position;
    uv_t  uv;
};

struct elevation_obj
{
    typedef grid::readable<double> grid_t;
    typedef std::array<double, 2>  vec3_t;
    typedef std::array<double, 2>  range_t;

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
        for (point2i p { 0, 0 }; p.y < sz.h; p.y++) {
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
        for (point2i p { 0, 0 }; p.y < sz.h; p.y++) {
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
       // smooth();
    }

private:
};

}
