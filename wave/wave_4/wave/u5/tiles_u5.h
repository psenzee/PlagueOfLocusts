#pragma once

#include "grid.h"
#include "multigradient.h"
#include "any_of.h"

namespace u5 {

#include "std.h"

using namespace wave; // TODO remove when namespaces are cleaned up

struct tile_types
{
    size_t water_count,
    grass_count,
    trees_count,
    hills_count,
    mountains_count,
    peaks_count;
    
    inline tile_types() : water_count(3), grass_count(3), trees_count(3), hills_count(2), mountains_count(2), peaks_count(4) {}
};

namespace {
template <typename T, typename U> inline T umod(U v, T d) { return (v % d + d) % d; }
}

template <size_t ID_COUNT, typename InitFunctor>
class rotation_map
{
    typedef std::array<size_t, ID_COUNT> map_t;
    typedef InitFunctor                  initializer_t;
    
    static map_t         _cw[4];
    static bool          _cw_inited;
    static initializer_t _initializer;
    
public:

    inline static size_t rotate_cw(size_t id, int angle_cw90) { return rotate_cw_map(angle_cw90)[id % ID_COUNT]; }
    
private:

    static const map_t &rotate_cw_map(int angle)
    {
        size_t a = umod(angle, 4);
        if (_cw_inited) {
            return _cw[a];
        }
        map_t m;
        for (size_t i = 0; i < m.size(); i++)
            m[i] = i;
        _cw[0] = m;
        
        _initializer(m);
        
        _cw[1] = m;
        
        map_t mm;
        for (size_t i = 0; i < m.size(); i++)
            mm[i] = m[m[i]];
        _cw[2] = mm;

        for (size_t i = 0; i < m.size(); i++)
            mm[i] = m[m[m[i]]];
        _cw[3] = mm;
        
        _cw_inited = true;
        
        return _cw[a];
    }
};

template <size_t ID_COUNT, typename InitFunctor>
bool rotation_map<ID_COUNT, InitFunctor>::_cw_inited = false;

template <size_t ID_COUNT, typename InitFunctor>
typename rotation_map<ID_COUNT, InitFunctor>::map_t rotation_map<ID_COUNT, InitFunctor>::_cw[4];

template <size_t ID_COUNT, typename InitFunctor>
InitFunctor rotation_map<ID_COUNT, InitFunctor>::_initializer;

struct tiles_u5_constants
{
    typedef knot<double> knot_t;
    
    enum {
        
        _MIN_BASIC_WATER        = 1,
         SEA_ROUGH              = 1,
         SEA_CALM               = 2,
         SEA_SHOALS             = 3,
        _MAX_BASIC_WATER        = 3,
        
        _MIN_BASIC_LAND         = 4,
         _MIN_BASIC_GRASS       = 4,
          LAND_SWAMP            = 4,
          LAND_GRASS            = 5,
          LAND_SHRUBS           = 6,
         _MAX_BASIC_GRASS       = 6,

         LAND_DESERT            = 7,

         _MIN_BASIC_FOREST      = 8,
          LAND_TREES            = 8,
          LAND_WOODLANDS        = 9,
          LAND_FOREST           = 10,
         _MAX_BASIC_FOREST      = 10,

         _MIN_BASIC_MOUNTAIN    = 10,
          LAND_FOOTHILLS        = 11,
          LAND_LOW_MOUNTAINS    = 12,
          LAND_MOUNTAINS        = 13,
          LAND_FOOTHILLS_LEFT   = 14,
          LAND_FOOTHILLS_RIGHT  = 15,
         _MAX_BASIC_MOUNTAIN    = 15,
        _MAX_BASIC_LAND         = 15,
        
        _MIN_HABITATS           = 16,
         HABITATS_HUT           = 16,
         HABITATS_CODEX         = 17,
         HABITATS_KEEP          = 18,
         HABITATS_VILLAGE       = 19,
         HABITATS_TOWN          = 20,
         HABITATS_CASTLE        = 21,
         HABITATS_CAVE          = 22,
         HABITATS_MINE          = 23,
         HABITATS_DUNGEON       = 24,
         HABITATS_SHRINE        = 25,
         HABITATS_SHRINE_RUIN   = 26,
         HABITATS_LIGHTHOUSE    = 27,
        _MAX_HABITATS           = 27,
        
        DESERT_OASIS            = 28,
        PATH_BRIDGE_EW          = 29,
        DESERT_EDGE_W           = 30,
        DESERT_EDGE_E           = 31,

        _MIN_PATH               = 32,
         PATH_NS                = 32,
         PATH_EW                = 33,
         PATH_SW_CORNER         = 34,
         PATH_NW_CORNER         = 35,
         PATH_NE_CORNER         = 36,
         PATH_SE_CORNER         = 37,
         PATH_INTERSECT_PATH    = 38,
        _MAX_PATH               = 38,
        
        CASTLE_ROOF_1           = 39,
        CASTLE_ROOF_2           = 40,
        CRYSTAL_BALL            = 41,
        PERPETUAL_FLAME         = 42,
        
        LAND_DEAD_TREE          = 43,
        LAND_FARM_FURROWED      = 44,
        LAND_FARM_HARVEST       = 45,
        LAND_TOWN_TREE          = 46,
        DESERT_SAGUARO          = 47,

        _MIN_SHORE              = 48,
         _MIN_SHORE_CARDINAL    = 48,
          SHORE_S               = 48,
          SHORE_W               = 49,
          SHORE_N               = 50,
          SHORE_E               = 51,
         _MAX_SHORE_CARDINAL    = 51,

         _MIN_SHORE_ORDINAL     = 52,
          SHORE_SE              = 52,
          SHORE_SW              = 53,
          SHORE_NW              = 54,
          SHORE_NE              = 55,
         _MAX_SHORE_ORDINAL     = 55,
        _MAX_SHORE              = 55,
        
        _MIN_RIVER              = 96,
         RIVER_NS               = 96,
         RIVER_EW               = 97,
         RIVER_SW_BEND          = 98,
         RIVER_NW_BEND          = 99,
         RIVER_NE_BEND          = 100,
         RIVER_SE_BEND          = 101,
         RIVER_T_W              = 102,
         RIVER_T_N              = 103,
         RIVER_T_E              = 104,
         RIVER_T_S              = 105,
         RIVER_BRIDGE_EW        = 106,
         RIVER_BRIDGE_NS        = 107,
         RIVER_END_N            = 108,
         RIVER_END_E            = 109,
         RIVER_END_S            = 110,
         RIVER_END_W            = 111,
        _MAX_RIVER              = 111,
        
        _MIN_WATERFALL          = 212,
         WATERFALL_1            = 212,
         WATERFALL_2            = 213,
         WATERFALL_3            = 214,
         WATERFALL_4            = 215,
        _MAX_WATERFALL          = 215,
        
        _MAX_TILES              = 216
    };
private:
    struct _initializer
    {
        void operator()(typename std::array<size_t, _MAX_TILES> &m)
        {
            m[PATH_NS]         = PATH_EW;            m[PATH_EW]         = PATH_NS;
            m[PATH_NW_CORNER]  = PATH_NE_CORNER;     m[PATH_NE_CORNER]  = PATH_SE_CORNER;
            m[PATH_SE_CORNER]  = PATH_SW_CORNER;     m[PATH_SW_CORNER]  = PATH_NW_CORNER;
            
            m[RIVER_NS]        = RIVER_EW;           m[RIVER_EW]        = RIVER_NS;
            m[RIVER_BRIDGE_NS] = RIVER_BRIDGE_EW;    m[RIVER_BRIDGE_EW] = RIVER_BRIDGE_NS;
            m[RIVER_NW_BEND]   = RIVER_NE_BEND;      m[RIVER_NE_BEND]   = RIVER_SE_BEND;
            m[RIVER_SE_BEND]   = RIVER_SW_BEND;      m[RIVER_SW_BEND]   = RIVER_NW_BEND;
            m[RIVER_END_N]     = RIVER_END_E;        m[RIVER_END_E]     = RIVER_END_S;
            m[RIVER_END_S]     = RIVER_END_W;        m[RIVER_END_W]     = RIVER_END_N;
            m[RIVER_T_N]       = RIVER_T_E;          m[RIVER_T_E]       = RIVER_T_S;
            m[RIVER_T_S]       = RIVER_T_W;          m[RIVER_T_W]       = RIVER_T_N;
            
            m[SHORE_N]         = SHORE_E;            m[SHORE_E]         = SHORE_S;
            m[SHORE_S]         = SHORE_W;            m[SHORE_W]         = SHORE_N;
            m[SHORE_NW]        = SHORE_NE;           m[SHORE_NE]        = SHORE_SE;
            m[SHORE_SE]        = SHORE_SW;           m[SHORE_SW]        = SHORE_NW;
        }
    };
public:
    typedef rotation_map<_MAX_TILES, _initializer> rotator_t;
    
    constexpr static const size_t ELEVATION_IDS[] = {
        SEA_ROUGH,           SEA_CALM,           SEA_SHOALS,
        LAND_SWAMP,          LAND_GRASS,         LAND_SHRUBS,         LAND_TREES,    LAND_WOODLANDS,     LAND_FOREST,  LAND_WOODLANDS, LAND_TREES,
        LAND_FOOTHILLS_LEFT, LAND_FOOTHILLS,     LAND_LOW_MOUNTAINS,  LAND_MOUNTAINS
    };
       
    inline static bool is_basic_water(size_t id)    { return id >= _MIN_BASIC_WATER && id <= _MAX_BASIC_WATER; }
    inline static bool is_basic_land(size_t id)     { return id >= _MIN_BASIC_LAND && id <= _MAX_BASIC_LAND; }
    inline static bool is_land_misc(size_t id)      { return any_of(id, LAND_DEAD_TREE, LAND_FARM_FURROWED, LAND_FARM_HARVEST, LAND_TOWN_TREE, DESERT_SAGUARO); }
    inline static bool is_habitat(size_t id)        { return id >= _MIN_HABITATS && id <= _MAX_HABITATS; }
    inline static bool is_shore_cardinal(size_t id) { return id >= _MIN_SHORE_CARDINAL && id <= _MAX_SHORE_CARDINAL; }
    inline static bool is_shore_ordinal(size_t id)  { return id >= _MIN_SHORE_ORDINAL && id <= _MAX_SHORE_ORDINAL; }
    inline static bool is_shore(size_t id)          { return id >= _MIN_SHORE && id <= _MAX_SHORE; }
    inline static bool is_basic_river(size_t id)    { return id >= _MIN_RIVER && id <= _MAX_RIVER; }
    inline static bool is_waterfall(size_t id)      { return id >= _MIN_WATERFALL && id <= _MAX_WATERFALL; }
    inline static bool is_river(size_t id)          { return is_basic_river(id) || is_waterfall(id); }
    inline static bool is_desert(size_t id)         { return any_of(id, LAND_DESERT, DESERT_SAGUARO, DESERT_OASIS, DESERT_EDGE_W, DESERT_EDGE_E); }
    inline static bool is_basic_path(size_t id)     { return id >= _MIN_PATH && id <= _MAX_PATH; }
    inline static bool is_bridge(size_t id)         { return any_of(id, RIVER_BRIDGE_EW, RIVER_BRIDGE_NS, PATH_BRIDGE_EW); }
    inline static bool is_path(size_t id)           { return is_basic_path(id) || is_bridge(id); }
    inline static bool is_land(size_t id)           { return is_basic_land(id) || is_shore_cardinal(id) || is_path(id) || is_desert(id) || is_land_misc(id); }
    inline static bool is_water(size_t id)          { return is_basic_water(id) || is_river(id) || is_shore_ordinal(id); }
    
    static size_t tile_from_elevation(double e, const point2i &p)
    {
        // TODO add dithering -- x2 or x4 the indices then use % to select the dither pattern! ?
        // not with the ocean/shore tho!
        constexpr static const multigradient<double, 7> gradient = {
            knot_t({ 0.00,  0. }), knot_t({ 0.25,  1. }), knot_t({ 0.38,  2. }), knot_t({ 0.40,  3. }),
            knot_t({ 0.90,  9. }), knot_t({ 0.95, 10. }), knot_t({ 0.99, 13. })
        };
        double v = gradient.value_at(e);
        size_t vi = size_t(v);
        if (vi > 3) {
            // dither
            size_t vn(v * 4), vf(vn & 3), vx(p.x & 1), vy(p.y & 1);
            if        (vf == 1) {
                vi += (vx & vy);
            } else if (vf == 2) {
                vi += (vx ^ vy);
            } else if (vf == 3) {
                vi += (vx | vy);
            }
        }
        return ELEVATION_IDS[vi];
    }
};

template <typename T>
struct kernel5x5
{
    typedef point2i pt;
    typedef grid::grid<T> grid_t;
    
    size_t    _rotation;
    grid_t   &_grid;
    const pt &_p;
    
    constexpr static const pt
            NW_NW = pt(-2, -2), NW_N = pt(-1, -2), N_N = pt( 0, -2), NE_N = pt( 1, -2), NE_NE = pt( 2, -2),
            NW_W  = pt(-2, -1), NW   = pt(-1, -1), N   = pt( 0, -1), NE   = pt( 1, -1), NE_E  = pt( 2, -1),
            W_W   = pt(-2,  0), W    = pt(-1,  0), C   = pt( 0,  0), E    = pt( 1,  0), E_E   = pt( 2,  0),
            SW_W  = pt(-2,  1), SW   = pt(-1,  1), S   = pt( 0,  1), SE   = pt( 1,  1), SE_E  = pt( 2,  1),
            SW_SW = pt(-2,  2), SW_S = pt(-1,  2), S_S = pt( 0,  2), SE_S = pt( 1,  2), SE_SE = pt( 2,  2);
    
    kernel5x5(grid_t &grid, const pt &p, int rotation) : _grid(grid), _p(p), _rotation(rotation) {}
    
    inline size_t nw_nw()  const { return _rotate_at(_p + NW_NW); }
    inline size_t nw_n()   const { return _rotate_at(_p + NW_N);  }
    inline size_t n_n()    const { return _rotate_at(_p + N_N);   }
    inline size_t ne_n()   const { return _rotate_at(_p + NE_N);  }
    inline size_t ne_ne()  const { return _rotate_at(_p + NE_NE); }
    
    inline size_t nw_w()   const { return _rotate_at(_p + NW_W);  }
    inline size_t nw()     const { return _rotate_at(_p + NW);    }
    inline size_t n()      const { return _rotate_at(_p + N);     }
    inline size_t ne()     const { return _rotate_at(_p + NE);    }
    inline size_t ne_e()   const { return _rotate_at(_p + NE_E);  }
    
    inline size_t w_w()    const { return _rotate_at(_p + W_W);   }
    inline size_t w()      const { return _rotate_at(_p + W);     }
    inline size_t c()      const { return _rotate_at(_p + C);     }
    inline size_t e()      const { return _rotate_at(_p + E);     }
    inline size_t e_e()    const { return _rotate_at(_p + E_E);   }

    inline size_t sw_w()   const { return _rotate_at(_p + SW_W);  }
    inline size_t sw()     const { return _rotate_at(_p + SW);    }
    inline size_t s()      const { return _rotate_at(_p + S);     }
    inline size_t se()     const { return _rotate_at(_p + SE);    }
    inline size_t se_e()   const { return _rotate_at(_p + SE_E);  }

    inline size_t sw_sw()  const { return _rotate_at(_p + SW_SW); }
    inline size_t sw_s()   const { return _rotate_at(_p + SW_S);  }
    inline size_t s_s()    const { return _rotate_at(_p + S_S);   }
    inline size_t se_s()   const { return _rotate_at(_p + SE_S);  }
    inline size_t se_se()  const { return _rotate_at(_p + SE_SE); }
    
private:
    
    inline pt _rotate(const pt &p) const
    {
        pt a[] = { p, pt(-p.y,  p.x), pt(-p.x, -p.y), pt( p.y, -p.x) };
        return a[_rotation];
    }
    
    inline pt _rotate_inv(const pt &p) const
    {
        pt a[] = { p, pt(-p.y,  p.x), pt(-p.x, -p.y), pt( p.y, -p.x) };
        return a[4 - _rotation];
    }
    
    inline size_t _rotate_at(const pt &p)     const { return _rotated_tile(_rotate(p)); }
    inline size_t _rotate_inv_at(const pt &p) const { return _rotated_inv_tile(_rotate_inv(p)); }
    
    inline size_t _rotated_tile(const pt &p)  const
    {
        typedef tiles_u5_constants::rotator_t rotator_t;
        return rotator_t::rotate_cw(_grid.get(p), _rotation);
    }
    
    inline size_t _rotated_inv_tile(const pt &p)  const
    {
        typedef tiles_u5_constants::rotator_t rotator_t;
        return rotator_t::rotate_cw(_grid.get(p), _rotation);
    }
};

template <typename T>
struct tiles_u5 : public tiles_u5_constants
{
    typedef grid::readable<T>  readable_t;
    typedef grid::grid<T>      grid_t;
    
    inline static bool is_basic_water(size_t id)
    {
        return tiles_u5_constants::is_basic_water(id);
    }
    
    inline static bool is_basic_water(const readable_t &grid, const point2i &p, size_t offset_index = directions::CENTER)
    {
        return is_basic_water(at_offset(grid, p, offset_index));
    }
    
    inline static bool is_basic_land(size_t id)
    {
        return tiles_u5_constants::is_basic_land(id);
    }
    
    inline static bool is_basic_land(const readable_t &grid, const point2i &p, size_t offset_index = directions::CENTER)
    {
        return is_basic_land(at_offset(grid, p, offset_index));
    }
    
    inline static bool is_land(size_t id)
    {
        return tiles_u5_constants::is_land(id);
    }
    
    inline static bool is_land(const readable_t &grid, const point2i &p, size_t offset_index = directions::CENTER)
    {
        return is_land(at_offset(grid, p, offset_index));
    }
    
    inline static bool is_shore(const readable_t &grid, const point2i &p, size_t offset_index = directions::CENTER)
    {
        return tiles_u5_constants::is_shore(at_offset(grid, p, offset_index));
    }
    
    inline static bool is_river(const readable_t &grid, const point2i &p, size_t offset_index = directions::CENTER)
    {
        return tiles_u5_constants::is_river(at_offset(grid, p, offset_index));
    }

    inline static bool is_any_cardinal_neighbor_shore(const readable_t &grid, const point2i &p)
    {
        return is_shore(grid, p, directions::NORTH) || is_shore(grid, p, directions::SOUTH)
            || is_shore(grid, p, directions::EAST) || is_shore(grid, p, directions::WEST);
    }

    inline static bool is_any_ordinal_neighbor_shore(const readable_t &grid, const point2i &p)
    {
        return is_shore(grid, p, directions::NORTHEAST) || is_shore(grid, p, directions::NORTHWEST)
            || is_shore(grid, p, directions::SOUTHEAST) || is_shore(grid, p, directions::SOUTHWEST);
    }

    inline static bool is_any_neighbor_shore(const readable_t &grid, const point2i &p)
    {
        return is_any_cardinal_neighbor_shore(grid, p) || is_any_ordinal_neighbor_shore(grid, p);
    }
    
    inline static bool is_north_shore(const readable_t &grid, const point2i &p)
    {
        return is_land(grid, p) && is_basic_water(grid, p, directions::SOUTH);
    }
    
    inline static bool is_south_shore(const readable_t &grid, const point2i &p)
    {
        return is_land(grid, p) && is_basic_water(grid, p, directions::NORTH);
    }
    
    inline static bool is_east_shore(const readable_t &grid, const point2i &p)
    {
        return is_land(grid, p) && is_basic_water(grid, p, directions::WEST);
    }
    
    inline static bool is_west_shore(const readable_t &grid, const point2i &p)
    {
        return is_land(grid, p) && is_basic_water(grid, p, directions::EAST);
    }
    
    inline static bool is_northeast_shore(const readable_t &grid, const point2i &p)
    {
        return is_north_shore(grid, p) && is_east_shore(grid, p);
    }
    
    inline static bool is_northwest_shore(const readable_t &grid, const point2i &p)
    {
        return is_north_shore(grid, p) && is_west_shore(grid, p);
    }
    
    inline static bool is_southeast_shore(const readable_t &grid, const point2i &p)
    {
        return is_south_shore(grid, p) && is_east_shore(grid, p);
    }
    
    inline static bool is_southwest_shore(const readable_t &grid, const point2i &p)
    {
        return is_south_shore(grid, p) && is_west_shore(grid, p);
    }
    
    typedef point2i pt;
    
    constexpr static const pt
    NW_NW = pt(-2, -2), NW_N = pt(-1, -2), N_N = pt( 0, -2), NE_N = pt( 1, -2), NE_NE = pt( 2, -2),
    NW_W  = pt(-2, -1), NW   = pt(-1, -1), N   = pt( 0, -1), NE   = pt( 1, -1), NE_E  = pt( 2, -1),
    W_W   = pt(-2,  0), W    = pt(-1,  0), C   = pt( 0,  0), E    = pt( 1,  0), E_E   = pt( 2,  0),
    SW_W  = pt(-2,  1), SW   = pt(-1,  1), S   = pt( 0,  1), SE   = pt( 1,  1), SE_E  = pt( 2,  1),
    SW_SW = pt(-2,  2), SW_S = pt(-1,  2), S_S = pt( 0,  2), SE_S = pt( 1,  2), SE_SE = pt( 2,  2);
    
    static void _puddle(grid_t &m, const point2i &p)
    {
        size_t a = m.get(p + NW), b = m.get(p + N), c = m.get(p + NE),
        d = m.get(p +  W), e = m.get(p + C), f = m.get(p +  E),
        g = m.get(p + SW), h = m.get(p + S), i = m.get(p + SE);
        if (is_land(a) && is_land(b)        && is_land(c) &&
            is_land(d) && is_basic_water(e) && is_land(f) &&
            is_land(g) && is_land(h)        && is_land(i)) {
            m.set(p + N, SHORE_N);
            m.set(p + S, SHORE_S);
            m.set(p + E, SHORE_E);
            m.set(p + W, SHORE_W);
        }
    }
    
    static void _grass(grid_t &m, const point2i &p)
    {
        size_t a = m.get(p + NW), b = m.get(p + N), c = m.get(p + NE),
               d = m.get(p +  W), e = m.get(p + C), f = m.get(p +  E),
               g = m.get(p + SW), h = m.get(p + S), i = m.get(p + SE);
        if ((b == LAND_FOREST && e == LAND_FOREST && any_of(h, LAND_GRASS, LAND_SHRUBS, LAND_TREES )) ||
            (h == LAND_FOREST && e == LAND_FOREST && any_of(b, LAND_GRASS, LAND_SHRUBS, LAND_TREES )) ||
            (f == LAND_FOREST && e == LAND_FOREST && any_of(d, LAND_GRASS, LAND_SHRUBS, LAND_TREES )) ||
            (d == LAND_FOREST && e == LAND_FOREST && any_of(f, LAND_GRASS, LAND_SHRUBS, LAND_TREES ))) {
            m.set(p, LAND_WOODLANDS);
        } else if ((b == LAND_WOODLANDS && e == LAND_WOODLANDS && any_of(h, LAND_GRASS, LAND_SHRUBS)) ||
                   (h == LAND_WOODLANDS && e == LAND_WOODLANDS && any_of(b, LAND_GRASS, LAND_SHRUBS)) ||
                   (f == LAND_WOODLANDS && e == LAND_WOODLANDS && any_of(d, LAND_GRASS, LAND_SHRUBS)) ||
                   (d == LAND_WOODLANDS && e == LAND_WOODLANDS && any_of(f, LAND_GRASS, LAND_SHRUBS))) {
            m.set(p, LAND_TREES);
        }
    }
    
    static void _river2(grid_t &m, const point2i &p)
    {
        return;
        for (int r = 0; r < 4; r++) {
            kernel5x5<size_t> k(m, p, r);
            if (is_land(k.nw()) && is_basic_water(k.n()) && is_land(k.ne()) &&
                is_land(k.w())  && is_basic_water(k.c()) && is_land(k.e())  &&
                is_land(k.sw()) && is_basic_water(k.s()) && is_land(k.se())) {
                m.set(p, RIVER_NS);
            }
        }
    }
    
    static void _river(grid_t &m, const point2i &p)
    {
        size_t a = m.get(p + NW), b = m.get(p + N), c = m.get(p + NE),
               d = m.get(p +  W), e = m.get(p + C), f = m.get(p +  E),
               g = m.get(p + SW), h = m.get(p + S), i = m.get(p + SE);
        bool w_e = is_basic_water(e);
        if (w_e) {
            bool 
                wr_b_ns = is_basic_water(b) || any_of(b, RIVER_NS, RIVER_END_N, RIVER_END_S, RIVER_SE_BEND, RIVER_SW_BEND),
                wr_h_ns = is_basic_water(h) || any_of(h, RIVER_NS, RIVER_END_N, RIVER_END_S),
                wr_g_ns = is_basic_water(g) || any_of(g, RIVER_NS, RIVER_END_N, RIVER_END_S, RIVER_NE_BEND, RIVER_NW_BEND),
                wr_d_ew = is_basic_water(d) || any_of(d, RIVER_EW, RIVER_END_E, RIVER_END_W),
                wr_f_ew = is_basic_water(f) || any_of(f, RIVER_EW, RIVER_END_E, RIVER_END_W),
                wr_i_ns = is_basic_water(i) || any_of(i, RIVER_NS, RIVER_END_N, RIVER_END_S, RIVER_NE_BEND, RIVER_NW_BEND);
            
            bool a_land = is_land(a), b_land = is_land(b), c_land = is_land(c),
                 d_land = is_land(d), e_land = is_land(e), f_land = is_land(f),
                 g_land = is_land(g), h_land = is_land(h), i_land = is_land(i);
            
            if        (a_land        && wr_b_ns           && c_land &&
                       d_land        && w_e               && f_land &&
                       g_land        && wr_h_ns           && i_land) {
                m.set(p, RIVER_NS);
            } else if (a_land        && b_land            && c_land  &&
                       wr_d_ew       && w_e               && wr_f_ew &&
                       g_land        && h_land            && i_land) {
                m.set(p, RIVER_EW);
            } else if (a_land        && wr_b_ns           && c_land  &&
                       d_land        && w_e               && f_land  &&
                       g_land        && h_land            && i_land) {
                m.set(p, RIVER_END_S);
            } else if (a_land        && b_land            && c_land  &&
                       d_land        && w_e               && f_land  &&
                       g_land        && wr_h_ns           && i_land) {
                m.set(p, RIVER_END_N);
            } else if (a_land        && b_land            && c_land  &&
                       d_land        && w_e               && wr_f_ew &&
                       g_land        && h_land            && i_land) {
                m.set(p, RIVER_END_W);
            } else if (a_land        && b_land            && c_land &&
                       wr_d_ew       && w_e               && f_land &&
                       g_land        && wr_h_ns           && i_land) {
                m.set(p, RIVER_END_E);
            } else if (a_land        && b_land            && c_land &&
                       wr_d_ew       && w_e               && f_land &&
                       g_land        && wr_h_ns           && i_land) {
                m.set(p, RIVER_END_E);
            } else if (a_land        && wr_b_ns           && c_land &&
                       d_land        && w_e               && f_land &&
                       wr_g_ns       && h_land            && i_land) {
                m.set(p,      RIVER_SE_BEND);
                m.set(p + W,  RIVER_NW_BEND);
                if (is_basic_water(b)) {
                    m.set(p + N, RIVER_NS);
                }
                if (is_basic_water(g)) {
                    m.set(p + SW, RIVER_NS);
                }
            } else if (a_land        && wr_b_ns           && c_land &&
                       d_land        && w_e               && f_land &&
                       g_land        && h_land            && wr_i_ns) {
                m.set(p,      RIVER_SW_BEND);
                m.set(p + E,  RIVER_NE_BEND);
                if (is_basic_water(b)) {
                    m.set(p + N, RIVER_NS);
                }
                if (is_basic_water(g)) {
                    m.set(p + SW, RIVER_NS);
                }
            }
        }
    }
    
    static void update(grid_t &m)
    {
        size2u sz(m.size());
        for (size_t i = 0; i < 3; i++) {
            for (point2i p(1, 1); p.y < sz.h - 1; p.y++)
                for (p.x = 1; p.x < sz.w - 1; p.x++)
                    _grass(m, p);
        }
        for (point2i p(1, 1); p.y < sz.h - 1; p.y++)
            for (p.x = 1; p.x < sz.w - 1; p.x++)
                _puddle(m, p);
        for (size_t i = 0; i < 2; i++) {
            for (point2i p(1, 1); p.y < sz.h - 1; p.y++)
                for (p.x = 1; p.x < sz.w - 1; p.x++)
                    _river2(m, p);
        }
    }
    
    static T get_shore_tile(const readable_t &m, const point2i &p)
    {
        T tile = m.get(p);
        
        bool water = tiles_u5_constants::is_basic_water(tile);
        bool land  = tiles_u5_constants::is_basic_land(tile);
        
        if (false && !water) {
            if      (is_northeast_shore(m, p)) tile = SHORE_NE;
            else if (is_southeast_shore(m, p)) tile = SHORE_SE;
            else if (is_northwest_shore(m, p)) tile = SHORE_NW;
            else if (is_southwest_shore(m, p)) tile = SHORE_SW;
            else if (is_north_shore(m, p))     tile = SHORE_N;
            else if (is_south_shore(m, p))     tile = SHORE_S;
            else if (is_east_shore(m, p))      tile = SHORE_E;
            else if (is_west_shore(m, p))      tile = SHORE_W;
        }

        /*
        typedef point2i pt;
         
        std::array<int, 2>
            NW_NW = p + pt(-2, -2), NW_N = p + pt(-1, -2), N_N = p + pt( 0, -2), NE_N = p + pt( 1, -2), NE_NE = p + pt( 2, -2),
            NW_W  = p + pt(-2, -1), NW   = p + pt(-1, -1), N   = p + pt( 0, -1), NE   = p + pt( 1, -1), NE_E  = p + pt( 2, -1),
            W_W   = p + pt(-2,  0), W    = p + pt(-1,  0), C   = p + pt( 0,  0), E    = p + pt( 1,  0), E_E   = p + pt( 2,  0),
            SW_W  = p + pt(-2, -1), SW   = p + pt(-1, -1), S   = p + pt( 0, -1), SE   = p + pt( 1, -1), SE_E  = p + pt( 2, -1),
            SW_SW = p + pt(-2, -2), SW_N = p + pt(-1, -2), S_S = p + pt( 0, -2), SE_S = p + pt( 1, -2), SE_SE = p + pt( 2, -2);
         */

        bool water_n   = is_basic_water(m, p + N),    water_s   = is_basic_water(m, p + S),
             water_e   = is_basic_water(m, p + E),    water_w   = is_basic_water(m, p + W),

             land_n    = is_basic_land(m,  p + N),    land_s    = is_basic_land(m,  p + S),
             land_e    = is_basic_land(m,  p + E),    land_w    = is_basic_land(m,  p + W),
             land_nn   = is_basic_land(m,  p + N_N),  land_ss   = is_basic_land(m,  p + S_S),
             land_ee   = is_basic_land(m,  p + E_E),  land_ww   = is_basic_land(m,  p + W_W),
             land_ne   = is_basic_land(m,  p + NE),   land_sw   = is_basic_land(m,  p + SW),
             land_se   = is_basic_land(m,  p + SE),   land_nw   = is_basic_land(m,  p + NW),
             land_ne_n = is_basic_land(m,  p + NE_N), land_sw_s = is_basic_land(m,  p + SW_S),
             land_se_s = is_basic_land(m,  p + SE_S), land_nw_n = is_basic_land(m,  p + NW_N),
        
             river_n   = is_river(m,       p + N),    river_s   = is_river(m,       p + S),
             river_e   = is_river(m,       p + E),    river_w   = is_river(m,       p + W),

             wr_n     = water_n || river_n,       wr_s      = water_s || river_s,
             wr_e     = water_e || river_e,       wr_w      = water_w || river_w;
        
        /*
        if (land) {
            
            bool ls_w = land_w || is_east_shore(m.get(W)),
                 ls_e = land_e || is_west_shore(m.get(E));
                                                                    
            if (land_w && land_e && land_se && land_sw && land_ss && land_sw_s && land_se_s && water_s)
                tile = SHORE_N;
            if (land_w && land_e && land_se && land_sw && land_ss && land_sw_s && land_se_s && water_s)
                tile = SHORE_N;
        }*/
        
        if (water) {
            
            //if      (land_e && land_w && land_n  && land_s ) tile = RIVER_END_N;

            // is river end
            if      (land_e && land_w && land_n  && wr_s   ) tile = RIVER_END_N;
            else if (land_e && land_w && wr_n    && land_s ) tile = RIVER_END_S;
            else if (land_e && wr_w   && land_n  && land_s ) tile = RIVER_END_E;
            else if (wr_e   && land_w && land_n  && land_s ) tile = RIVER_END_W;
            
          //  if      (land_e && land_w && wr_n    && wr_s   ) tile = RIVER_NS;
          //  else if (wr_e   && wr_w   && land_n  && land_s ) tile = RIVER_EW;
        }
         
        /*
        if      (land_e  && wr_w && land_n  && wr_s) tile = RIVER_NE_BEND;
        else if (water_e && land_w  && wr_n && land_s ) tile = RIVER_SW_BEND;
        else if (water_e && land_w  && land_n  && wr_s) tile = RIVER_NW_BEND;
        else if (land_e  && wr_w && wr_n && land_s ) tile = RIVER_SE_BEND;

        if (tile == SHORE_NE && water_n && water_e) tile = SEA_SHOALS;
        if (tile == SHORE_NW && water_n && water_w) tile = SEA_SHOALS;
        if (tile == SHORE_SE && water_s && water_e) tile = SEA_SHOALS;
        if (tile == SHORE_SW && water_s && water_w) tile = SEA_SHOALS;

        if (tile == LAND_MOUNTAINS && is_any_neighbor_shore(m, p)) tile = LAND_FOOTHILLS;
         */

        return tile;
    }
    
private:

    inline static T at_offset(const readable_t &grid, const point2i &p, size_t offset_index)
    {
        return grid.get(directions::offset(p, offset_index));
    }
};

}
