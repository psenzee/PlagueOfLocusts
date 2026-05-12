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

/*
typedef std::string_view::const_iterator const_iter_t;

int parseOneOf(const_iter_t b, const_iter_t e, std::span<const char *> options)
{
    int i = 0;
    for (auto p : options) {
        size_t n = strlen(p), sz(e - b);
        if (sz >= n && strncasecmp(b, p, std::min(sz, n)) == 0) {
            return i;
        }
        i++;
    }
    return -1;
}

std::pair<int, int> parseCharacterN(const_iter_t b, const_iter_t e, std::string_view chars)
{
    char *p = strcasechr(chars, *b);
    std::pair<int, int> result(-1, -1);
    if (p) {
        result.first = *p;
        // now parse number -- one hex digit?
        strtol(b + 1)
        
    }
    return result;
}
*/
struct Element
{
    uint8_t offset, count;
    inline Element(uint8_t offset = 0, uint8_t count = 0) : offset(offset), count(count) {}
};

inline std::ostream &operator<<(std::ostream &os, const Element &elem) { return std::cout << "(offset " << int(elem.offset) << " count " << int(elem.count) << ")"; }

typedef std::string_view::const_iterator sv_iter_t;

inline sv_iter_t        consume_space(sv_iter_t b, sv_iter_t e) { for (; b != e && isspace(*b); ++b) {} return b; }
inline std::string_view consume_space(const_iter_t b, const_iter_t e) { return std::string_view(consume_space(b, e)); }
inline std::string_view consume_space(std::string_view s) { return consume_space(s.begin(), s.end()); }

struct VertexFormat
{
    enum            { MAX_ITEMS = 16 };

    enum Type       { TRI_NONE = 0, TRI_LIST, TRI_STRIP };
    enum FieldOrder { ORDER_POSITION = 0, ORDER_NORMAL, ORDER_TEXTURE, ORDER_COLOR, ORDER_MAX };
    enum Error      {
        PARSE_SUCCESS = 0,
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
    
    static const Error _count_errors[] = {
        ERROR_PARSE_POSITION_COUNT, ERROR_PARSE_NORMAL_COUNT,
        ERROR_PARSE_TEXTURE_COUNT,  ERROR_PARSE_COLOR_COUNT
    };
    static const Error _too_many_errors[] = {
        ERROR_PARSE_POSITION_TOO_MANY, ERROR_PARSE_NORMAL_TOO_MANY,
        ERROR_PARSE_TEXTURE_TOO_MANY,  ERROR_PARSE_COLOR_TOO_MANY
    };
    static const uint8_t _max_items[] = { 1, 1, MAX_ITEMS, MAX_ITEMS };
    
    const char *_error_messages(Error e)
    {
        // in the order of the Error enum
        static const char *messages[] = {
            "success",
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
        return messages[e];
    }

    Type            type;
    element_array_t position,
                    normal,
                    colors,
                    textures;
    uint8_t         stride;
    field_array_t   counts;

    inline VertexFormat() : type(TRI_NONE), stride(0), counts{ 0 } {}
    
    typedef std::pair<Error, const_iter_t> parse_result_t;
    
    /*
    std::ostream &print(std::ostream &os, const Element &elem)
    {
        return std::cout << "(offset " << int(elem.offset) << " count " << int(elem.count) << ")";
    }
    */
    
    parse_result_t _parse(std::string_view s)
    {
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
        stride = 0;
        counts = field_array_t{ 0 };
        while (b != e) {
            b = consume_space(b, e);
            if (b == e) {
                break;
            }
            Element elem(stride, 0);
            char c = toupper(*b++);
            if (c == '#') { b = e; break; } // # is a comment, finish up
            size_t field = "PNTC"sv.find(c);
            if (field == std::string_view::npos) {
                return _result(ERROR_PARSE_UNRECOGNIZED_FIELD_NAME, s, b - 1);
            }
            if (b == e) {
                return _result(ERROR_PARSE_UNEXPECTED_END, s, b - 1);
            }
            size_t count = "1234"sv.find(*b++);
            if (count == std::string_view::npos) {
                return _result(_count_errors[field], s, b - 2 /* one for field name, one for count */);
            }
            elem.count = count + 1;
            stride += elem.count * sizeof(float); // the size of all our vector elements
            element_array_t &arrays[] = { position, normal, texture, color };
            if (counts[field] + 1 >= _max_items[field]) {
                return _result(_too_many_errors[field], s, s.begin());
            }
            (arrays[field])[counts[field]] = elem;
            counts[field]++;
        }
        return _result(PARSE_SUCCESS, s, s.begin());
    }
    
    inline parse_result_t _result(Error e, std::string_view s, const_iter_t b)
    {
        return parse_result_t(e, std::max(b, s.begin()));
    }

    /*

    Error parse(std::string_view s)
    {
        const_iter_t i = _parse(s.begin(), s.end(), "XYZW");
        
        State { TYPE, POSITION, OTHER };
        State state = TYPE;
        uint8_t offset = 0, cindex = 0, tindex = 0;
        for (const_iterator i = s.begin(), e = e.end(); i != e; ++i) {
            for (; i != e && isspace(*i); ++i) {}
            if (state == TYPE) {
                type = TRI_NONE;
                switch (toupper(*i)) {
                case 'S': type  = TRI_STRIP; type_parsed = true; break;
                case 'L': type  = TRI_LIST;  type_parsed = true; break;
                default:
                    return ERROR_PARSE_TRI_TYPE;
                }
                state = POSITION;
                continue;
            }
            if (state == POSITION) {
                Element element(offset);
                const_iter_t ni = _parse(i, e, "XYZW", element);
                if (element.count >= 2) {
                    i = ni;
                    position = element;
                    state = OTHER;
                    offset += element.count;
                    continue;
                } else if (element.count) {
                    return ERROR_PARSE_POSITION;
                }
            }
            if (state == OTHER) {
                Element element(offset);

                ni = _parse(i, e, "NXYZ", element);
                element = Element(offset);
                if (element.count >= 3) {
                    i = ni;
                    normal = element;
                    offset += element.count - 1;
                    continue;
                } else if (element.count) {
                    return ERROR_PARSE_NORMAL;
                }
                char ch = toupper(*i);
                i += (ch == 'N' || ch == 'T' || ch == 'C');
                switch (ch)
                {
                case 'N':
                case 'T':
                case 'C':
                    const_iter_t ni = _parse(i, e, "RGBA", element);
                    if (element.count >= 3) {
                        i = ni;
                        colors[cindex++] = element;
                        offset += element.count;
                        continue;
                    } else if (element.count) {
                        return ERROR_PARSE_COLOR;
                    }
                    break;
                }

                ni = _parse(i, e, "TSTU", element);
                element = Element(offset);
                if (element.count >= 3) {
                    i = ni;
                    normal = element;
                    offset += element.count - 1;
                    continue;
                } else if (element.count) {
                    // error
                }
            }
        }
        return *this;
    }
    */
    
    /*
    const_iter_t _parse(const_iter_t i, const_iter_t e, const char *word, Element &element)
    {
        for (; i != e && isspace(*i); ++i) {}
        element = Element(0, 0);
        for (const char *p = word; *p; ++p) {
            if (isupper(*i++) != isupper(*p)) {
                return i;
            }
            element.count++;
        }
        return i;
    }
     */
};

/*
int test_vf()
{
    std::string s("XYZW ");
    std::string_view sv(s);
    Element element;
    VertexFormat vf;
    vf._parse(sv.begin(), sv.end(), "XYZW", element);
    std::cout << "elem " << element << std::endl;
    return 0;
}
 */

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
        PARSE_SUCCESS = 0,
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
    
    static const Error _count_errors[] = {
        ERROR_PARSE_POSITION_COUNT, ERROR_PARSE_NORMAL_COUNT,
        ERROR_PARSE_TEXTURE_COUNT,  ERROR_PARSE_COLOR_COUNT
    };
    static const Error _too_many_errors[] = {
        ERROR_PARSE_POSITION_TOO_MANY, ERROR_PARSE_NORMAL_TOO_MANY,
        ERROR_PARSE_TEXTURE_TOO_MANY,  ERROR_PARSE_COLOR_TOO_MANY
    };
    static const uint8_t _max_items[] = { 1, 1, MAX_ITEMS, MAX_ITEMS };

    Type            type;
    element_array_t position,
                    normal,
                    colors,
                    textures;
    uint8_t         stride;
    field_array_t   counts;

    inline VertexFormat() : type(TRI_NONE), stride(0), counts{ 0 } {}
    
    typedef std::pair<Error, const_iter_t> parse_result_t;
    
    std::ostream &print(std::ostream &os, const Element &elem)
    {
        return std::cout << "(offset " << int(elem.offset) << " count " << int(elem.count) << ")";
    }
    
    parse_result_t _parse(std::string_view s)
    {
        s = consume_space(s);
        if (s.empty()) { return result(ERROR_PARSE_EMPTY_TEXT, s, s.begin()); }
        const_iter_t b = s.begin(), e = s.end();
        char ch = toupper(*b++);
        if (ch != 'I') { return result(ERROR_PARSE_STRUCTURE, s, b - 1); }
        s = consume_space(b, e);
        if (s.empty()) { return result(ERROR_PARSE_UNEXPECTED_END, s, b - 1); }
        ch = toupper(*b++);
        type = TRI_NONE;
        switch (ch) {
            case 'L': type = TRI_LIST;  break;
            case 'S': type = TRI_STRIP; break;
            default: return result(ERROR_PARSE_TRIANGLE_TYPE, s, b - 1);
        }
        // for field lookup arrays below the order is always PNTC: 0:P 1:N 2:T 3:C
        // in the order of the FieldOrder enum
        stride = 0;
        counts = field_array_t{ 0 };
        while (b != e) {
            b = consume_space(b, e);
            if (b == e) {
                break;
            }
            Element elem(stride, 0);
            char c = toupper(*b++);
            if (c == '#') { b = e; break; } // # is a comment, finish up
            size_t field = "PNTC"sv.find(c);
            if (field == std::string_view::npos) {
                return result(ERROR_PARSE_UNRECOGNIZED_FIELD_NAME, s, b - 1);
            }
            if (b == e) {
                return result(ERROR_PARSE_UNEXPECTED_END, s, b - 1);
            }
            size_t count = "1234"sv.find(*b++);
            if (count == std::string_view::npos) {
                return result(_count_errors[field], s, b - 2 /* one for field name, one for count */);
            }
            elem.count = count + 1;
            stride += elem.count * sizeof(float); // the size of all our vector elements
            element_array_t &arrays[] = { position, normal, texture, color };
            if (counts[field] + 1 >= _max_items[field]) {
                return result(_too_many_errors[field], s, s.begin());
            }
            (arrays[field])[counts[field]] = elem;
            counts[field]++;
        }
        return result(PARSE_SUCCESS, s, s.begin());
    }
    
    inline parse_result_t _result(Error e, std::string_view s, const_iter_t b)
    {
        return parse_result_t(e, std::max(b, s.begin()));
    }
    
    inline const_iter_t    _consume_space(const_iter_t b, const_iter_t e)
    {
        for (; b != e && isspace(*b); ++b) {}
        return b;
    }
    
    inline std::string_view _consume_space(const_iter_t b, const_iter_t e)
    {
        return std::string_view(consume_space(b, e));
    }
            
    inline std::string_view consume_space(std::string_view s)
    {
        return consume_space(s.begin(), s.end());
    }

    /*

    Error parse(std::string_view s)
    {
        const_iter_t i = _parse(s.begin(), s.end(), "XYZW");
        
        State { TYPE, POSITION, OTHER };
        State state = TYPE;
        uint8_t offset = 0, cindex = 0, tindex = 0;
        for (const_iterator i = s.begin(), e = e.end(); i != e; ++i) {
            for (; i != e && isspace(*i); ++i) {}
            if (state == TYPE) {
                type = TRI_NONE;
                switch (toupper(*i)) {
                case 'S': type  = TRI_STRIP; type_parsed = true; break;
                case 'L': type  = TRI_LIST;  type_parsed = true; break;
                default:
                    return ERROR_PARSE_TRI_TYPE;
                }
                state = POSITION;
                continue;
            }
            if (state == POSITION) {
                Element element(offset);
                const_iter_t ni = _parse(i, e, "XYZW", element);
                if (element.count >= 2) {
                    i = ni;
                    position = element;
                    state = OTHER;
                    offset += element.count;
                    continue;
                } else if (element.count) {
                    return ERROR_PARSE_POSITION;
                }
            }
            if (state == OTHER) {
                Element element(offset);

                ni = _parse(i, e, "NXYZ", element);
                element = Element(offset);
                if (element.count >= 3) {
                    i = ni;
                    normal = element;
                    offset += element.count - 1;
                    continue;
                } else if (element.count) {
                    return ERROR_PARSE_NORMAL;
                }
                char ch = toupper(*i);
                i += (ch == 'N' || ch == 'T' || ch == 'C');
                switch (ch)
                {
                case 'N':
                case 'T':
                case 'C':
                    const_iter_t ni = _parse(i, e, "RGBA", element);
                    if (element.count >= 3) {
                        i = ni;
                        colors[cindex++] = element;
                        offset += element.count;
                        continue;
                    } else if (element.count) {
                        return ERROR_PARSE_COLOR;
                    }
                    break;
                }

                ni = _parse(i, e, "TSTU", element);
                element = Element(offset);
                if (element.count >= 3) {
                    i = ni;
                    normal = element;
                    offset += element.count - 1;
                    continue;
                } else if (element.count) {
                    // error
                }
            }
        }
        return *this;
    }
    */
    
    /*
    const_iter_t _parse(const_iter_t i, const_iter_t e, const char *word, Element &element)
    {
        for (; i != e && isspace(*i); ++i) {}
        element = Element(0, 0);
        for (const char *p = word; *p; ++p) {
            if (isupper(*i++) != isupper(*p)) {
                return i;
            }
            element.count++;
        }
        return i;
    }
     */
};

/*
int test_vf()
{
    std::string s("XYZW ");
    std::string_view sv(s);
    Element element;
    VertexFormat vf;
    vf._parse(sv.begin(), sv.end(), "XYZW", element);
    std::cout << "elem " << element << std::endl;
    return 0;
}
 */

int test_oneof()
{
    std::string s("hello ");
    std::string_view sv(s);
    const char *options[] = { "what", "is", "hello", "hell", "what", 0 };
    int id = parseOneOf(sv.begin(), sv.end(), options);
}

int main(int argc, const char * argv[])
{
    std::string s("hello ");
    std::string_view sv(s);
    const char *options[] = { "what", "is", "hello", "hell", "what", 0 };
    int id = parseOneOf(sv.begin(), sv.end(), options);
    
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
