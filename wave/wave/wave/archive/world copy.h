//
//  main.cpp
//  wave
//
//  Created by paul on 12/27/23.
//

#include "noise.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <deque>
#include <ctime>
#include <queue>
#include <random>
#include <algorithm>
#include <cstring>
#include <ctime>
#include "bits.h"
#include "bit_array.h"
#include "grid.h"
#include "base_grid.h"
#include "bool_grid.h"
#include "rules.h"
#include "patch.h"
#include "std_array.h"
#include "noise.h"
#include "std.h"
#include "color.h"
#include "piecewise_spline.h"

#include "bit_container.h"
#include "bit_array_core.h"

#include "tiles_u5.h"
#include <png.h>
#include "write_png.h"
#include "gaussian.h"

namespace wave
{

size_t quantize(const std::array<double, 2> &range, double value, size_t count)
{
    size_t c = count - 1;
    double t = round(((value - range[0]) / (range[1] - range[0])) * c);
    return t < 0 ? 0 : (t > c ? c : size_t(t));
}

template <typename T>
struct octave
{
    double frequency, amplitude;
};

template <typename T>
void octave_noise(
                  grid<T> &grid,
                  size_t octaves,
                  T frequency,
                  std::mt19937 mt,
                  T amplitude = 1.0,
                  T frequency_multiplier_per_octave = 2.0,
                  T amplitude_multiplier_per_octave = 0.5
                  ) {
    for (size_t i = 0; i < octaves; i++) {
        noise_filler<T> nf(mt(), noise::SIMPLEX, vec3_t({ frequency, frequency }), amplitude);
        fill(grid, nf);
        frequency *= frequency_multiplier_per_octave;
        amplitude *= amplitude_multiplier_per_octave;
    }
}

template <typename T, size_t N>
void octave_noise(
                  wave::grid<T> &grid,
                  const std::array<T, N> &frequencies,
                  const std::array<T, N> &amplitudes,
                  std::mt19937 mt
                  ) {
    for (size_t i = 0; i < N; i++) {
        noise_filler<T> nf(mt(), noise::SIMPLEX, vec3_t({ frequencies[i], frequencies[i] }), amplitudes[i]);
        fill(grid, nf);
    }
}

template <typename T>
void octave_noise(wave::grid<T> &grid, const std::vector< octave<T> > &octaves, std::mt19937 mt)
{
    for (const auto &octave : octaves) {
        noise_filler<T> nf(mt(), noise::SIMPLEX, vec3_t({ octave.frequency }), octave.amplitude);
        fill(grid, nf);
    }
}

constexpr color_rgba_f  color__indigo           ({ 0.00, 0.00, 0.20, 1.0 }),
                        color__rich_blue        ({ 0.05, 0.05, 0.30, 1.0 }),
                        color__sea_foam         ({ 0.30, 0.30, 0.60, 1.0 }),
                        color__sea_foam_dark    ({ 0.20, 0.20, 0.20, 1.0 }),
                        color__sea_foam_white   ({ 0.50, 0.50, 0.80, 1.0 }),
                        color__dark_earth       ({ 0.12, 0.17, 0.04, 1.0 }),
                        color__dark_grass       ({ 0.07, 0.23, 0.03, 1.0 }),
                        color__forest_green     ({ 0.10, 0.23, 0.00, 1.0 }),
                        color__meadow_green     ({ 0.25, 0.60, 0.25, 1.0 }),
                        color__iron_stone       ({ 0.20, 0.15, 0.10, 1.0 }),
                        color__stone_snowline   ({ 0.20, 0.20, 0.20, 1.0 }),
                        color__blue_snow        ({ 0.40, 0.40, 0.45, 1.0 }),
                        color__white_snow       ({ 0.80, 0.80, 0.98, 1.0 }),
                        color__stone_bright     ({ 0.90, 0.90, 0.70, 1.0 }),
                        color__brighter         ({ 1.00, 1.00, 0.50, 1.0 }),
                        color__darker           ({ 0.10, 0.10, 0.40, 1.0 });

typedef knot<color_rgba_f> color_knot_t;

constexpr piecewise_spline<color_rgba_f, 7> landscape_gradient_0 = {
    
    color_knot_t({ 0.00, color__indigo           }),
    color_knot_t({ 0.25, color__indigo           }),
    color_knot_t({ 0.39, color__rich_blue        }),
    color_knot_t({ 0.40, color__sea_foam_dark    }),
    color_knot_t({ 0.43, color__dark_earth       }),
  //color_knot_t({ 0.45, color__dark_grass       }),
    color_knot_t({ 0.50, color__iron_stone       }),
    color_knot_t({ 0.85, color__iron_stone       }),
};

constexpr piecewise_spline<color_rgba_f, 8> landscape_gradient_1 = {
    
    color_knot_t({ 0.00, color__indigo           }),
    color_knot_t({ 0.25, color__indigo           }),
    color_knot_t({ 0.38, color__rich_blue        }),
    color_knot_t({ 0.41, color__sea_foam_white   }),
    color_knot_t({ 0.43, color__dark_grass       }),
    color_knot_t({ 0.50, color__forest_green     }),
    color_knot_t({ 0.65, color__stone_snowline   }),
//  color_knot_t({ 0.50, color__iron_stone       }),
    color_knot_t({ 0.95, color__stone_snowline   }),
};

constexpr piecewise_spline<color_rgba_f, 5> landscape_gradient_snowy = {
    
    color_knot_t({ 0.00, color__indigo           }),
    color_knot_t({ 0.25, color__indigo           }),
    color_knot_t({ 0.38, color__rich_blue        }),
    color_knot_t({ 0.41, color__sea_foam_white   }),
    color_knot_t({ 0.95, color__stone_snowline   }),
};

constexpr piecewise_spline<color_rgba_f, 7> landscape_brighter = {
    
    color_knot_t({ 0.00, color__brighter         }),
    color_knot_t({ 0.25, color__brighter         }),
    color_knot_t({ 0.39, color__brighter         }),
    color_knot_t({ 0.40, color__brighter         }),
    color_knot_t({ 0.43, color__meadow_green     }),
    color_knot_t({ 0.45, color__meadow_green     }),
    color_knot_t({ 0.85, color__stone_bright     }),
};

constexpr piecewise_spline<color_rgba_f, 7> landscape_darker = {
    
    color_knot_t({ 0.00, color__darker           }),
    color_knot_t({ 0.25, color__darker           }),
    color_knot_t({ 0.39, color__darker           }),
    color_knot_t({ 0.40, color__darker           }),
    color_knot_t({ 0.43, color__darker           }),
    color_knot_t({ 0.45, color__darker           }),
    color_knot_t({ 0.85, color__darker           }),
};

/*
void islandify(wave::base_grid<double> &map)
{
    size sz(map.size());
    point mid(int(sz.w / 2), int(sz.h / 2));
    for (point p(0, 0); p.y < sz.h; p.y++) {
        for (p.x = 0; p.x < sz.w; p.x++) {
            wave::point q(abs(p.x - mid.x), abs(p.y - mid.y));
            map.set(p, map.get(p) - clamp((sqrt(q.x * q.x + q.y * q.y)) * 0.2, 0., 1.));
        }
    }
}

void render_png(const char *file, const wave::base_grid<double> &map)
{
    const size &sz = map.size();
    uint32_t *data = new uint32_t [sz.area()];
    std::array<double, 2> sun({ -1., -1. });
    color_rgba_f darker({ 0.1, 0.1, 0.4, 1.0 }), brighter({ 1.0, 1.0, 0.5, 1.0 });
    noise_filler<double> modulator(10, noise::SIMPLEX, 0.001, 1.);
    for (point p(0, 0); p.y < sz.h; p.y++) {
        for (p.x = 0; p.x < sz.w; p.x++) {
            auto v = map.get(p);
            auto color1 = landscape_gradient_0.value_at(v), color2 = landscape_gradient_1.value_at(v);
            auto color = lerp(color1, color2, modulator.at(p) / 2. + 0.5);
            auto d = std_array::dot(wave::vector_3x3(map, p), sun);
            auto light = (v < 0.40 || d == 0.) ? color : (d > 0 ? darker : brighter);
            double t = clamp(fabs(d) * v, 0., 1.);
            color = lerp(color, light, t);
            color = color * light + color * 0.5;
            ((uint32_t *)data)[p.y * sz.w + p.x] = color_to_u32(clamp(color));
        }
    }
    write_png(file, (uint8_t *)data, sz.w, sz.w * sizeof(uint32_t), sz.h);
    delete [] data;
    
}

std::string filename(const char *path, const char *base_file, const char *extension, int appendable = -1)
{
    std::ostringstream s;
    s << path << "/" << base_file;
    if (appendable != -1)
        s << "_" << appendable;
    if (extension) {
        s << "." << extension;
    }
    return s.str();
}
*/

struct layers
{
    typedef grid<double>      grid_t;
    typedef base_grid<double> base_grid_t;

    grid_t      &_elevation;
    base_grid_t  _erosion,
                 _fill;
    
    layers(grid_t &elevation) : _elevation(elevation), _erosion(elevation.size()), _fill(elevation.size()) {}
    
    inline double altitude(const point &p) const  { return _elevation.get(p) + _erosion.get(p) + _fill.get(p); }
    inline void   erode(const point &p, double w) { _erosion.set(p, _erosion.get(p) + w); }
    inline void   fill(const point &p, double w)  { _fill.set(p, _fill.get(p) + w); }
    
    void commit()
    {
        size sz(_erosion.size());
        for (point p(0, 0); p.y < sz.h; p.y++) {
            for (p.x = 0; p.x < sz.w; p.x++) {
                double el = _elevation.get(p);
                if (_erosion.get(p) < 0.) {
                    //_elevation.set(p, _erosion.get(p) + el);
                    _elevation.set(p, 0.);
                }
                if (_fill.get(p) > 0.) {
                    _elevation.set(p, 0.);
                }
            }
        }
    }
};

struct rain_drop
{
    typedef std::array<double, 2> vec2_t;

    vec2_t   _at;
    size_t   _life;
    size_t   _moves;
    layers  &_layers;

    inline rain_drop(layers &layers, const point &at, size_t life = 1000) : _layers(layers), _life(life), _moves(0)
    {
        _at[0] = double(at.x); _at[1] = double(at.y);
    }

    void update()
    {
        if (!_life) {
            return;
        }
        point at(_point());
        if (false && _layers._elevation.get(at) < 0.40) {
            _life = 0;
            return;
        }
        //std::cout << "point ";
        //at.print(std::cout);
        //_layers.fill(at, -0.001); // remove from where it is currently at
        std::array<double, 9> alts = {
            _alt(point::NORTHWEST), _alt(point::NORTH),  _alt(point::NORTHEAST),
            _alt(point::WEST),      _alt(point::CENTER), _alt(point::EAST),
            _alt(point::SOUTHWEST), _alt(point::SOUTH),  _alt(point::SOUTHEAST)
        };
        std::array<double, 9> elevs = {
            _elev(point::NORTHWEST), _elev(point::NORTH),  _elev(point::NORTHEAST),
            _elev(point::WEST),      _elev(point::CENTER), _elev(point::EAST),
            _elev(point::SOUTHWEST), _elev(point::SOUTH),  _elev(point::SOUTHEAST)
        };
        auto v = vector_3x3(elevs);
        //auto v = vector_3x3(alts);
        auto len = std_array::magnitude(v);
        auto dir = v * (len == 0 ? 0 : (1. / len));
        if (len > 0) {
            auto prev = _point();
            _at += dir;
            at = _point();
            if (at != prev) {
                //double t = _layers.altitude(at);
                _layers.erode(at, -0.002);
                //_layers.fill(at, 0.001); // fill back in the new spot
                _moves++;
            }
        }
        _life--;
    }

private:
    
    inline point        _point() const { return point(int(_at[0]), int(_at[1])); }
    inline static point _point(const vec2_t &o) { return point(int(o[0]), int(o[1])); }

    inline double _alt(point::Direction dir)
    {
        return _layers.altitude(point(_at[0], _at[1]).direction_offset(dir));
    }
    
    inline double _elev(point::Direction dir)
    {
        return _layers._elevation.get(point(_at[0], _at[1]).direction_offset(dir));
    }
};

struct world
{
    typedef std::array<double, 2> vec2_t;
    typedef std::array<double, 2> range_t;

    base_grid<double>    _elevation;
    base_grid<double>    _dots;
    base_grid<vec2_t>    _gradient_field;
    base_grid<uint32_t>  _ids;
    std::mt19937         _mt;
    double               _sea_level;
    double               _scale;
    vec2_t               _sun;
    noise_filler<double> _color_modulator;
    layers               _layers;
    
    world(const size &s, const std::mt19937 &mt, double scale = 1.0) : _elevation(s), _dots(s), _ids(s), _gradient_field(s), _mt(mt), _sea_level(0.40), _sun({ -1., -1. }),
         _color_modulator(10, noise::SIMPLEX, 0.001, 1.0), _layers(_elevation), _scale(scale)
    {
        clear(_elevation, 0.);
        clear(_dots, 0.);
        clear(_gradient_field, vec2_t({ 0., 0. }));
        clear(_ids, uint32_t(0));
    }
    
    size size() const { return _elevation.size(); }
    
    range_t elevation_range() const { return range(_elevation); }
    range_t dot_range()       const { return range(_dots); }
    
    void add_rivers(size_t octaves = 5, double frequency = 0.005, const range_t &river_range = { 0.1, 0.5 })
    {
        octave_noise(_elevation, octaves, frequency / _scale, _mt);
        normalize(_elevation);
        auto sz = _elevation.size();
        for (point p(0, 0); p.y < sz.h; p.y++) {
            for (p.x = 0; p.x < sz.w; p.x++) {
                double v = _elevation.get(p),
                       u = clamp(((v > river_range[0] && v < river_range[1]) ? -v : 0.), -1., 0.);
                _elevation.set(p, u);
            }
        }
    }
    
    void smooth()
    {
        gaussian_smooth_3x3(_elevation);
        gaussian_smooth_3x3(_elevation);
    }
    
    void add_landscape(size_t octaves = 8, double frequency = 0.0012)
    {
        octave_noise(_elevation, octaves, frequency / _scale, _mt);
        normalize(_elevation);
    }
    
    void rain()
    {
        std::cout << "raining..." << std::endl;
        std::vector<rain_drop *> drops;
        struct size sz(_elevation.size());
        for (size_t i = 0; i < 10000; i++) {
            point at;
            while (true) {
                at = point(_mt() % sz.w, _mt() % sz.h);
                double v = _elevation.get(at);
                if (true || v > 0.4) {
                    break;
                }
            }
            drops.push_back(new rain_drop(_layers, at));
        }
        for (size_t c = 0; c < 1000; c++) {
            for (size_t i = 0; i < drops.size(); i++)
                drops[i]->update();
        }
        for (size_t i = 0; i < drops.size(); i++) {
            //std::cout << "moves " << drops[i]->_moves << std::endl;
            delete drops[i];
        }
        drops.resize(0);
        _layers.commit();
    }

    void islandify()
    {
        struct size sz(_elevation.size());
        point mid(int(sz.w / 2), int(sz.h / 2));
        for (point p(0, 0); p.y < sz.h; p.y++) {
            for (p.x = 0; p.x < sz.w; p.x++) {
                wave::point q(abs(p.x - mid.x), abs(p.y - mid.y));
                _elevation.set(p, _elevation.get(p) - clamp((sqrt(q.x * q.x + q.y * q.y)) * 0.2, 0., 1.));
            }
        }
    }
    
    void calculate_gradients()
    {
        struct size sz(_elevation.size());
        for (point p(0, 0); p.y < sz.h; p.y++) {
            for (p.x = 0; p.x < sz.w; p.x++) {
                auto v = wave::vector_3x3(_elevation, p);
                _gradient_field.set(p, v);
                _dots.set(p, std_array::dot(v, _sun));
            }
        }
    }
    
    color_rgba_f get_color(const point &p)
    {
        auto v = _elevation.get(p);
        auto color1 = landscape_gradient_0.value_at(v), color2 = landscape_gradient_1.value_at(v);
        auto brighter = landscape_brighter.value_at(v), darker = landscape_darker.value_at(v);
        auto color = lerp(color1, lerp(color2, color1 * 2.0, v), _color_modulator.at(p) / 2. + 0.5);
        auto d = _dots.get(p);
        auto light = (v < _sea_level || d == 0.) ? color : (d > 0 ? darker : brighter);
        double t = clamp(fabs(d) * v, 0., 1.);
        color = lerp(color, light, t);
        color = color * light + color * 0.5;
        color = color + color_rgba_f({ 0.05, 0.05, 0.1, 1.0 }); // atmospheric attenuation/desaturation
        return color;
    }

    void render_png(const char *file)
    {
        struct size sz(_elevation.size());
        uint32_t *data = new uint32_t [sz.area()];
        calculate_gradients();
        for (point p(0, 0); p.y < sz.h; p.y++) {
            for (p.x = 0; p.x < sz.w; p.x++) {
                auto color = get_color(p);
                ((uint32_t *)data)[p.y * sz.w + p.x] = color_to_u32(clamp(color));
            }
        }
        write_png(file, (uint8_t *)data, sz.w, sz.w * sizeof(uint32_t), sz.h);
        delete [] data;
    }
    
    void normalize_elevation()
    {
        normalize(_elevation);
    }
    
    void create_world()
    {
        add_rivers();
        add_landscape();
        //islandify();
        normalize_elevation();
        smooth();
        normalize_elevation();
        rain();
        smooth();
    }
};

static std::string filename(const char *path, const char *base_file, const char *extension, int appendable = -1)
{
    std::ostringstream s;
    s << path << "/" << base_file;
    if (appendable != -1)
        s << "_" << appendable;
    if (extension) {
        s << "." << extension;
    }
    return s.str();
}

void u5(wave::grid<double> &grid, const char *path)
{
    auto range = wave::range(grid);
    
    std::cout << "range " << range[0] << " - " << range[1] << std::endl;
    
    wave::tile_types tt;
    size_t gradient[256] = { 0 },
    gradient_sz = wave::tiles_u5<size_t>::elevation_gradient(gradient, tt);
    wave::size sz(grid.size());
    wave::base_grid<size_t> quantized(sz);
    
    for (wave::point p(0, 0); p.y < sz.h; p.y++)
        for (p.x = 0; p.x < sz.w; p.x++)
            quantized.set(p, gradient[quantize(range, grid.get(p), gradient_sz)]);
    for (size_t passes = 0; passes < 2; passes++)
        for (wave::point p(0, 0); p.y < sz.h - 0; p.y++)
            for (p.x = 0; p.x < sz.w - 0; p.x++)
                quantized.set(p, wave::tiles_u5<size_t>::get_shore_tile(quantized, p));
    
    std::ostringstream ostr;
    ostr << "function getMapData() {" << std::endl <<
    "    return [[" << sz.w << ", " << sz.h << ", " << sz.w << "], [" << std::endl;
    wave::print_as_json(ostr, quantized, 2);
    ostr << "]];" << std::endl << "}" << std::endl;
    
    time_t t;
    time(&t);
    std::array<std::string, 2> fns({ filename(path, "nu5map", "js", t), filename(path, "nu5map", "js") });
    for (const auto &fn : fns) {
        std::fstream s;
        s.open(fn.c_str(), std::ios::out);
        s << ostr.str();
        s.close();
        std::cout << "wrote to " << fn << std::endl;
    }
}

}
