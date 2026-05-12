#pragma once

#include <iostream>
#include <png.h>

#include "noise.h"
#include "base_grid.h"
#include "color.h"
#include "multigradient.h"
#include "write_png.h"

namespace wave
{

typedef std::array<double, 3> vec3_t;
typedef std::array<double, 2> vec2_t;

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

struct renderer
{
    typedef knot<color_rgba_f> color_knot_t;
    
    const base_grid<double>    &_elevation;
    base_grid<double>           _dots;
    base_grid<vec2_t>           _gradient_field;
    double                      _sea_level;
    double                      _scale;
    vec2_t                      _sun;
    noise::noise_filler<double> _color_modulator;
    color_rgba_f                _atmospheric_attenuation; // atmospheric attenuation/desaturation
    
    renderer(const base_grid<double> &elevation, size_t seed = 10) :
        _elevation(elevation), _dots(elevation.size()), _gradient_field(elevation.size()), _sea_level(0.4), _sun({ 1., 1. }),
        _color_modulator(seed, 0.001, 1.0), _atmospheric_attenuation({ 0.05, 0.05, 0.1, 1.0 })
    {
    }
    
    void prerender()
    {
        _calculate_gradients();
    }
    
    void save_rgba_png(const char *file) const
    {
        size2u sz(_elevation.size());
        uint32_t *data = _render_rgba_bitmap();
        write_png__rgba32(file, data, sz.w, sz.h);
        delete [] data;
    }
    
    void save_height_map_png(const char *file) const
    {
        size2u sz(_elevation.size());
        uint16_t *data = _render_grayscale_height_map();
        write_png__grayscale16(file, data, sz.w, sz.h);
        delete [] data;
    }
    

private:
    
    constexpr static const multigradient<color_rgba_f, 7> _highlight = {
        
        color_knot_t({ 0.00, color__brighter         }),
        color_knot_t({ 0.25, color__brighter         }),
        color_knot_t({ 0.39, color__brighter         }),
        color_knot_t({ 0.40, color__brighter         }),
        color_knot_t({ 0.43, color__meadow_green     }),
        color_knot_t({ 0.45, color__meadow_green     }),
        color_knot_t({ 0.85, color__stone_bright     }),
    };
    
    constexpr static const multigradient<color_rgba_f, 7> _highlight_snow = {
        
        color_knot_t({ 0.00, color__brighter         }),
        color_knot_t({ 0.25, color__brighter         }),
        color_knot_t({ 0.39, color__brighter         }),
        color_knot_t({ 0.40, color__brighter         }),
        color_knot_t({ 0.43, color__meadow_green     }),
        color_knot_t({ 0.45, color__meadow_green     }),
        color_knot_t({ 0.85, color__stone_snowline   }),
    };

    constexpr static const multigradient<color_rgba_f, 7> _shadow = {
        
        color_knot_t({ 0.00, color__darker           }),
        color_knot_t({ 0.25, color__darker           }),
        color_knot_t({ 0.39, color__darker           }),
        color_knot_t({ 0.40, color__darker           }),
        color_knot_t({ 0.43, color__darker           }),
        color_knot_t({ 0.45, color__darker           }),
        color_knot_t({ 0.85, color__darker           }),
    };

    constexpr static const multigradient<color_rgba_f, 7> _terrain_0 = {
        
        color_knot_t({ 0.00, color__indigo           }),
        color_knot_t({ 0.25, color__indigo           }),
        color_knot_t({ 0.39, color__rich_blue        }),
        color_knot_t({ 0.40, color__sea_foam_dark    }),
        color_knot_t({ 0.43, color__dark_earth       }),
        color_knot_t({ 0.50, color__iron_stone       }),
        color_knot_t({ 0.85, color__iron_stone       }),
    };

    constexpr static const multigradient<color_rgba_f, 8> _terrain_1 = {
        
        color_knot_t({ 0.00, color__indigo           }),
        color_knot_t({ 0.25, color__indigo           }),
        color_knot_t({ 0.38, color__rich_blue        }),
        color_knot_t({ 0.41, color__sea_foam_white   }),
        color_knot_t({ 0.43, color__dark_grass       }),
        color_knot_t({ 0.50, color__forest_green     }),
        color_knot_t({ 0.65, color__stone_snowline   }),
        color_knot_t({ 0.95, color__stone_snowline   }),
    };

    constexpr static const multigradient<color_rgba_f, 5> _terrain_snowy = {
        
        color_knot_t({ 0.00, color__indigo           }),
        color_knot_t({ 0.25, color__indigo           }),
        color_knot_t({ 0.38, color__rich_blue        }),
        color_knot_t({ 0.41, color__sea_foam_white   }),
        color_knot_t({ 0.95, color__stone_snowline   }),
    };

    void _calculate_gradients()
    {
        clear(_dots, 0.);
        clear(_gradient_field, vec2_t({ 0., 0. }));
        size2u sz(_elevation.size());
        for (point2i p(0, 0); p.y < sz.h; p.y++) {
            for (p.x = 0; p.x < sz.w; p.x++) {
                auto v = wave::vector_3x3(_elevation, p);
                _gradient_field.set(p, v);
                _dots.set(p, std_array::dot(v, _sun));
            }
        }
    }
    
    color_rgba_f _get_lit(color_rgba_f color, double v, double dot, double modulate) const
    {
        //auto brighter0 = _highlight.value_at(v), brighter1 = _highlight_snow.value_at(v);
        //auto brighter = lerp(brighter0, brighter1, modulate);
        auto brighter = _highlight.value_at(v);
        auto darker = _shadow.value_at(v);
        auto light = (v < _sea_level || dot == 0.) ? color : (dot > 0 ? darker : brighter);
        double t = clamp(fabs(dot) * v, 0., 1.);
        color = lerp(color, light, t);
        color = color * light + color * 0.5;
        return color + _atmospheric_attenuation;
    }

    color_rgba_f _get_color(const point2i &p) const
    {
        auto v = _elevation.get(p);
        auto color1 = _terrain_0.value_at(v), color2 = _terrain_1.value_at(v);
        auto modulate = _color_modulator.at(p) / 2. + 0.5;
        auto color = lerp(color1, lerp(color2, color1 * 2.0, v), modulate);
        auto d = _dots.get(p);
        return _get_lit(color, v, d, modulate);
    }

    uint16_t *_render_grayscale_height_map() const
    {
        size2u sz(_elevation.size());
        uint16_t *hm = new uint16_t [sz.area()];
        for (point2i p(0, 0); p.y < sz.h; p.y++) {
            for (p.x = 0; p.x < sz.w; p.x++) {
                hm[p.y * sz.w + p.x] = uint16_t((_elevation.get(p) + 1) * 65536);
            }
        }
        return hm;
    }

    uint32_t *_render_rgba_bitmap() const
    {
        size2u sz(_elevation.size());
        uint32_t *data = new uint32_t [sz.area()];
        for (point2i p(0, 0); p.y < sz.h; p.y++) {
            for (p.x = 0; p.x < sz.w; p.x++) {
                auto color = _get_color(p);
                data[p.y * sz.w + p.x] = color_to_u32(clamp(color));
            }
        }
        return data;
    }
};

}
