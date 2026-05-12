#pragma once

#include "grid.h"
#include "grid_extra.h"
#include "std_array.h"

namespace wave
{

typedef std::array<double, 3> vec3_t;
typedef std::array<double, 2> vec2_t;

struct eroder
{
    typedef grid::grid<double>  grid_t;
    typedef grid::basic<double> basic_t;

    grid_t      &_elevation;
    basic_t      _erosion;
    size2u       _size;
    double       _sea_level;

    inline eroder(grid_t &elevation, double sea_level = 0.0) :
        _elevation(elevation), _erosion(elevation.size()), _size(elevation.size()), _sea_level(sea_level) {}
    
    inline size2u size()                            const { return _size; }
    inline double elevation(const point2i &p)       const { return _elevation.get(p); }
    inline double altitude(const point2i &p)        const { return _elevation.get(p) + _erosion.get(p); }
    inline void   erode(const point2i &p, double w)       { _erosion.set(p, _erosion.get(p) + w); }
    
    void commit()
    {
        basic_t original(_erosion);
        gaussian_smooth_3x3(_erosion);
        gaussian_smooth_3x3(_erosion);
        for (point2i p { 0, 0 }; p[1] < _size[1]; p[1]++) {
            for (p[0] = 0; p[0] < _size[0]; p[0]++) {
                if (_erosion.get(p) < 0.) {
                    double v = elevation(p);
                    //_elevation.set(p, v + _erosion.get(p));
                    _elevation.set(p, v - cbrt(abs(_erosion.get(p))));
                    //_elevation.set(p, _sea_level);
                }
            }
        }
    }

    double calculate_erodable_density() const
    {
        size_t erodable = 0;
        for (point2i p { 0, 0 }; p[1] < _size[1]; p[1]++) {
            for (p[0] = 0; p[0] < _size[0]; p[0]++) {
                erodable += (elevation(p) >= _sea_level);
            }
        }
        return double(erodable) / double(area(_size));
    }
};

struct rain_drop
{
    typedef std::array<double, 2> vec2_t;

    vec2_t   _start, _at;
    size_t   _life;
    eroder  &_eroder;

    inline rain_drop(eroder &eroder, const point2i &at, size_t life = 1000) : _eroder(eroder), _life(life)
    {
        _at[0] = double(at[0]); _at[1] = double(at[1]);
        _start = _at;
    }

    bool update()
    {
        if (!_life) {
            return false;
        }
        point2i at(_point());
        if (_eroder.elevation(at) < 0.40) {
            _life = 0;
            return false;
        }
        std::array<double, 9> elevs = {
            _elev(directions::NORTHWEST), _elev(directions::NORTH),  _elev(directions::NORTHEAST),
            _elev(directions::WEST),      _elev(directions::CENTER), _elev(directions::EAST),
            _elev(directions::SOUTHWEST), _elev(directions::SOUTH),  _elev(directions::SOUTHEAST)
        };
        auto v = grid::vector3x3(elevs);
        auto len = std_array::magnitude(v);
        if (len > 0) {
            auto dir = v * std::array<double, 2> { 1. / len, 1. / len };
            auto prev = _point();
            _at += dir;
            at = _point();
            if (at != prev && std_array::distance(_at, _start) > 5.0) {
                _eroder.erode(at, -0.001);
            }
        }
        _life--;
        return _life > 0;
    }

private:
    
    inline point2i        _point()                const { return point2i(int(_at[0]), int(_at[1])); }
    inline static point2i _point(const vec2_t &o)       { return point2i(int(o[0]), int(o[1])); }
    
    inline double _elev(directions::Direction dir)
    {
        point2i p(directions::offset(_point(), dir));
        return _eroder.elevation(p);
        //return _eroder.altitude(p);
    }
};

struct rainer
{
    typedef grid::grid<double> grid_t;
    
    eroder       _eroder;
    std::mt19937 _mt;
    size_t       _particle_count;
    double       _sea_level;
    
    inline rainer(grid_t &elevation, const std::mt19937 &mt)
        : _eroder(elevation), _mt(mt), _particle_count(100000), _sea_level(0.40) {}
    
    void rain()
    {
        std::cout << "raining..." << std::endl;
        size_t particles = size_t(_eroder.calculate_erodable_density() * _particle_count);
        for (size_t i = 0; i < particles; i++) {
            run_whole();
        }
        _eroder.commit();
    }

private:

    void run_whole()
    {
        point2i at = _get_point();
        rain_drop drop(_eroder, at);
        while (drop.update()) {}
    }
    
    point2i _get_point()
    {
        point2i at(-1, -1);
        double v = 0.0;
        size2u sz(_eroder.size());
        for (size_t i = 0; v < _sea_level && i < 64; i++) {
            at = point2i(_mt() % sz[0], _mt() % sz[1]);
            v = _eroder._elevation.get(at);
        }
        return at;
    }
};

}
