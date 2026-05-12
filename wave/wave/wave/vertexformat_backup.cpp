//
//  main.cpp
//  wave
//
//  Created by paul on 12/27/23.
//

#include <random>
#include "world.h"
#include "std.h"
#include "u5.h"
#include "core_bits.h"

#include <string>
#include <string_view>
#include <algorithm>
#include <utility>
#include <array>
#include <iostream>
#include <span>

struct Element
{
    uint8_t offset, count;
    inline Element(uint8_t offset = 0, uint8_t count = 0) : offset(offset), count(count) {}
};

inline std::ostream &operator<<(std::ostream &os, const Element &e)
{
    return os << "offset " << int(e.offset) << " count " << int(e.count) << " bytes " << sizeof(float) * e.count;
}

typedef std::string_view::const_iterator sv_iter_t;

inline sv_iter_t        _consume_space(sv_iter_t b, sv_iter_t e) { for (; b != e && isspace(*b); ++b) {} return b; }
inline std::string_view consume_space(sv_iter_t b, sv_iter_t e) { return std::string_view(_consume_space(b, e)); }
inline std::string_view consume_space(std::string_view s) { return consume_space(s.begin(), s.end()); }

/* format
 
 non case sensitve
 current:
 starts with
 
 I      - structure: Interleaved - currently the only option, required
 
 then
 
 L or S - triangle set types: List (3 vertices per) or Strips (2 initial then 1 per subsequent)
 
 the following can occur in any order:
 
 Pn     - Position - where n is 1-4.  Only 1
 Nn     - Normal - where n is 1-4.  Only 1?
 Tn     - Texture - coor where n is 1-4.  Repeatable up to the max textures
 Cn     - Color - where n is 1-4.  Repeatable number up to the max textures
 
 */
struct VertexFormat
{
    enum            { MAX_ITEMS = 16 };

    enum Type       { TRI_NONE = 0, TRI_LIST, TRI_STRIP };
    enum FieldOrder { ORDER_POSITION = 0, ORDER_NORMAL, ORDER_TEXTURE, ORDER_COLOR, ORDER_MAX };
    enum Error      {
        NO_ERROR = 0,
        ERROR_PARSE_EMPTY_TEXT,
        ERROR_PARSE_UNEXPECTED_END,
        ERROR_PARSE_STRUCTURE,
        ERROR_PARSE_TRIANGLE_TYPE,
        ERROR_PARSE_UNRECOGNIZED_FIELD_NAME,
        ERROR_PARSE_POSITION,
        ERROR_PARSE_POSITION_COUNT,
        ERROR_PARSE_POSITION_TOO_MANY,
        ERROR_PARSE_NORMAL,
        ERROR_PARSE_NORMAL_COUNT,
        ERROR_PARSE_NORMAL_TOO_MANY,
        ERROR_PARSE_COLOR,
        ERROR_PARSE_COLOR_COUNT,
        ERROR_PARSE_COLOR_TOO_MANY,
        ERROR_PARSE_TEXTURE,
        ERROR_PARSE_TEXTURE_COUNT,
        ERROR_PARSE_TEXTURE_TOO_MANY
    };

    typedef std::array<Element, MAX_ITEMS>   element_array_t;
    typedef std::string_view::const_iterator const_iter_t;
    typedef std::array<uint8_t, ORDER_MAX>   field_array_t;

    Type             type;
    element_array_t  position,
                     normal,
                     colors,
                     textures;
    uint8_t          stride;
    field_array_t    counts;

    Error            error;
    std::string      format, error_message;
    
    std::ostream &print(std::ostream &os) const
    {
        os << "vertex format [" << format << "]";
        if (error) {
            return os << " " << error_message << std::endl;
        }
        os << std::endl;
        os << "  interleaved" << std::endl;
        os << "  type " << _type_name(type) << std::endl;
        os << "  stride " << size_t(stride) << " bytes" << std::endl;
        const element_array_t *arrays[] = { &position, &normal, &textures, &colors };
        const char *title[] = { "position", "normal", "texture", "color", 0 };
        for (size_t i = 0; title[i]; ++i) {
            if (counts[i] > 0) {
                os << "  " << title[i] << " count " << size_t(counts[i]) << ":" << std::endl;
                for (size_t j = 0; j < counts[i]; ++j) {
                    os << "    " << (*arrays[i])[j] << std::endl;
                }
            }
        }
        return os;
    }

    inline VertexFormat() : type(TRI_NONE), stride(0), counts{ 0 }, error(NO_ERROR) { _clear(); }
    
    typedef std::pair<Error, const_iter_t> parse_result_t;
    
    bool parse(std::string_view s)
    {
        format = s;
        parse_result_t pr = _parse(s);
        error = pr.first;
        std::string_view at(pr.second, s.end());
        if (pr.first == NO_ERROR) {
            error_message = "";
            return true;
        }
        _clear();
        char buffer[1024] = "";
        snprintf(buffer, sizeof(buffer) - 1, "%s at \'%s\'", _error_messages(error), at.data());
        error_message = buffer;
        return false;
    }

private:
    
    static constexpr Error _count_errors[] = {
        ERROR_PARSE_POSITION_COUNT, ERROR_PARSE_NORMAL_COUNT,
        ERROR_PARSE_TEXTURE_COUNT,  ERROR_PARSE_COLOR_COUNT
    };
    static constexpr Error _too_many_errors[] = {
        ERROR_PARSE_POSITION_TOO_MANY, ERROR_PARSE_NORMAL_TOO_MANY,
        ERROR_PARSE_TEXTURE_TOO_MANY,  ERROR_PARSE_COLOR_TOO_MANY
    };
    static constexpr uint8_t _max_items[] = { 1, 1, MAX_ITEMS, MAX_ITEMS };
    
    inline static const char *_error_messages(Error e)
    {
        // in the order of the Error enum
        static const char *text[] = {
            nullptr,
            "parse error: empty text",
            "parse error: unexpected end of text",
            "parse error: bad structure value, expected I for interleaved",
            "parse error: bad triangle type, expected L for list or S for strip",
            "parse error: unrecognized field name, expecting P, N, T or C",
            "parse error: position (P...) error",
            "parse error: position with count should be P1, P2, P3 or P4",
            "parse error: too many positions given",
            "parse error: normal (N...) error",
            "parse error: normal with count should be N1, N2, N3 or N4",
            "parse error: too many normals given",
            "parse error: texture coords (T...) error",
            "parse error: texture coords with count should be T1, T2, T3 or T4",
            "parse error: too many texture coords given",
            "parse error: color (C...) error",
            "parse error: color with count should be C1, C2, C3 or C4",
            "parse error: too many colors given",
        };
        return text[e];
    }
    
    inline static const char *_type_name(Type t)
    {
        static const char *text[] = {
            "none",
            "triangle list",
            "triangle strip",
        };
        return text[t];
    }
    
    void _clear_error()
    {
        error = NO_ERROR;
        if (!error_message.empty()) {
            error_message = "";
        }
    }
    
    void _clear()
    {
        type = TRI_NONE;
        stride = 0;
        std::fill(counts.begin(), counts.end(), 0);
        element_array_t *arrays[] = { &position, &normal, &textures, &colors };
        for (auto arr: std::span<element_array_t *>(arrays)) {
            *arr = element_array_t();
        }
    }
    
    parse_result_t _parse(std::string_view s)
    {
        using namespace std::string_view_literals;
        
        _clear();
        _clear_error();
        s = consume_space(s);
        if (s.empty()) { return _result(ERROR_PARSE_EMPTY_TEXT, s, s.begin()); }
        const_iter_t b = s.begin(), e = s.end();
        char ch = toupper(*b++);
        if (ch != 'I') { return _result(ERROR_PARSE_STRUCTURE, s, b - 1); }
        s = consume_space(b, e);
        if (s.empty()) { return _result(ERROR_PARSE_UNEXPECTED_END, s, b - 1); }
        ch = toupper(*b++);
        type = TRI_NONE;
        switch (ch) {
            case 'L': type = TRI_LIST;  break;
            case 'S': type = TRI_STRIP; break;
            default: return _result(ERROR_PARSE_TRIANGLE_TYPE, s, b - 1);
        }
        // for field lookup arrays below the order is always PNTC: 0:P 1:N 2:T 3:C
        // in the order of the FieldOrder enum
        while (b != e) {
            b = _consume_space(b, e);
            if (b == e) {
                break;
            }
            Element elem(stride, 0);
            auto checkpoint = b;
            char c = toupper(*b++);
            if (c == '#') { b = e; break; } // # is a comment, finish up
            size_t field = "PNTC"sv.find(c);
            if (field == std::string_view::npos) {
                return _result(ERROR_PARSE_UNRECOGNIZED_FIELD_NAME, s, checkpoint);
            }
            if (b == e) {
                return _result(ERROR_PARSE_UNEXPECTED_END, s, checkpoint);
            }
            size_t count = "1234"sv.find(*b++);
            if (count == std::string_view::npos) {
                return _result(_count_errors[field], s, checkpoint);
            }
            elem.count = count + 1;
            stride += elem.count * sizeof(float); // the size of all our vector elements
            element_array_t *arrays[] = { &position, &normal, &textures, &colors };
            if (counts[field] >= _max_items[field]) {
                return _result(_too_many_errors[field], s, checkpoint);
            }
            (*arrays[field])[counts[field]] = elem;
            counts[field]++;
        }
        return _result(NO_ERROR, s, s.begin());
    }
    
    inline parse_result_t _result(Error e, std::string_view s, const_iter_t b)
    {
        return parse_result_t(e, std::max(b, s.begin()));
    }
};

inline std::ostream &operator<<(std::ostream &os, const VertexFormat &vf) { return vf.print(os);; }

int main(int argc, const char * argv[])
{
    const char *tests[] = { "IL P3 N3 T2 C3 C3 T2", "ILP3N3T2C3C3T2", "seveh", "ISP4T2 C3", "ISP4T2 C3", 0 };
    for (const char **t = tests; *t; t++) {
        VertexFormat vf;
        if (!vf.parse(*t)) {
            std::cout << "failed" << std::endl;
        }
        std::cout << vf << std::endl << "--------" << std::endl << std::endl;
    }
    
    return 0;
    
    test_bit_reverse();

    const char *PATH = "/Users/Paul/Desktop/Projects/ultima_generate";

    std::mt19937 mt(100);

    size2u sz(2048, 2048);
    point2i p(0, 0);

    double scale = 1.;//0.5;
    auto w = wave::world(sz, p, mt, scale);

    w.create_world();

    u5::u5(w._elevation, PATH);

    auto range = w.elevation_range();

    std::cout << "range " << range[0] << " - " << range[1] << std::endl;
    auto fn(filename(PATH, "nu5map", "png")), fn_gs(filename(PATH, "nu5map_gs", "png"));
    
    w.save_rgba_png(fn.c_str());
    w.save_height_map_png(fn_gs.c_str());

    return 0;
}
