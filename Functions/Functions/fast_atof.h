#ifndef _FAST_ATOF_H
#define _FAST_ATOF_H

namespace parse
{

const char *parse_float(const char *c, float *out);

inline float fast_atof(const char *c)
{
    float ret = 0.f;
    parse_float(c, &ret);
    return ret;
}

}

#endif  // _FAST_ATOF_H

