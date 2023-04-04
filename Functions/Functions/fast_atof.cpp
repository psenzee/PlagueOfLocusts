#include "fast_atof.h"

#include <stdlib.h>

namespace
{
const float atof_table[]  = {
                                0.f,
                                0.1f,
                                0.01f,
                                0.001f,
                                0.0001f,
                                0.00001f,
                                0.000001f,
                                0.0000001f,
                                0.00000001f,
                                0.000000001f,
                                0.0000000001f,
                                0.00000000001f,
                                0.000000000001f,
                                0.0000000000001f,
                                0.00000000000001f,
                                0.000000000000001f
                            };
}

namespace parse
{

const char *parse_float(const char *c, float *out)
{
    bool inv = false;
    char *t = 0;
    float f;

    if (*c == '-')
    {
        c++;
        inv = true;
    }

    f = (float)strtol(c, &t, 10);
    
    if (!t)
        return 0;

    c = t;

    if (*c == '.')
    {
        c++;

        float pl = (float)strtol(c, &t, 10);
        pl *= atof_table[t - c];

        f += pl;

        c = t;

        if (*c == 'e')
        {
            ++c;
            float exp = (float)strtol(c, &t, 10);
            f *= (float)pow(10.0f, exp);
        }
    }

    if (inv)
        f *= -1.0f;
    
    *out = f;
    return c;
}

}