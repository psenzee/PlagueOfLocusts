class point_t
{
public:
    double x, y;

    inline point_t() : x(0), y(0) {}   
    inline point_t(const double &x, const double &y) : x(x), y(y) {}   
};

template <typename T>
inline T evaluate_bezier1(const T &a, const T &b, const T &c, const T &d, double t)
{
    T      u;
    double t_3 = t * 3,
        t2  = t * t,  t2_3 = t2 * 3, t2_6 = t2 * 6, 
        t3  = t2 * t, t3_3 = t3 * 3,

        p, q, r, s;

    p = a.x * (-t3 + t2_3 - t_3 + 1);
    q = b.x * (t3_3 - t2_6 + t_3);
    r = c.x * (-t3_3 + t2_3);
    s = d.x * t3;

    u.x  = p + q + r + s;

    p = a.y * (-t3 + t2_3 - t_3 + 1);
    q = b.y * (t3_3 - t2_6 + t_3);
    r = c.y * (-t3_3 + t2_3);
    s = d.y * t3;

    u.y  = p + q + r + s;

    return u;
}

template <typename T>
inline T evaluate_bezier2(const T &a, const T &b, const T &c, const T &d, double t)
{
    double e = t * 3, f = t * t, g = f * 3, h = f * 6, m = f * t, n = m * 3, 	       
           p = -m + g - e + 1, q = n - h + e, r = -n + g, s = m;
    return T(a.x * p + b.x * q + c.x * r + d.x * s, a.y * p + b.y * q + c.y * r + d.y * s);
}


/*

j1 = bezier.a.x * ((-tcu) + 3 * (tsq) - (3 * t) + 1);
j2 = bezier.b.x * ((3 * tcu) - (6 * tsq) + (3 * t));
j3 = bezier.c.x * ((-3 * tcu) + (3 * tsq));
j4 = bezier.d.x * tcu;

p.x  = j1 + j2 + j3 + j4;

j1 = bezier.a.y * ((-tcu) + 3 * (tsq) - (3 * t) + 1);
j2 = bezier.b.y * ((3 * tcu) - (6 * tsq) + (3 * t));
j3 = bezier.c.y * ((-3 * tcu) + (3 * tsq));
j4 = bezier.d.y * tcu;

p.y  = j1 + j2 + j3 + j4;

*/    	

#include <stdio.h>
#include <conio.h>
#include <string.h>

void print_curve(const point_t &b, const point_t &c)
{
    point_t a(0, 0), d(1, 1);

    char out[50][50];

    memset(out, ' ', 2500);

    point_t r, q;
    for (int i = 0; i < 50; i++)
    {
        out[i][i]               = 'o';
        out[(int)(b.y * 50)][i] = '.';
        out[(int)(c.y * 50)][i] = '.';
    }
    for (int i = 0; i < 50; i++)
    {
        r = evaluate_bezier1(a, b, c, d, i * 0.02);
        q = evaluate_bezier2(a, b, c, d, i * 0.02);

        if (r.x != q.x || r.y != q.y)
            printf("Error [%.04f, %.04f] != [%.04f, %.04f]\n", r.x, r.y, q.x, q.y);

        q.x *= 50; q.y *= 50;

        out[(int)q.x][(int)q.y] = 'x';
    }
    char line[51];
    for (int i = 0; i < 50; i++)
    {
        line[50] = 0;
        memcpy(line, out[49 - i], 50);
        printf("|%s|\n", line);
    }
    printf("|--------------------------------------------------|\n\n", line);
    getch();
}


void main()
{
    point_t b(0, 0.75), c(1, 0.25);
    for (int i = 0; i < 10; i++)
    {
        print_curve(b, c);
        c.x -= 0.1;
        b.x += 0.1; 
    }
}