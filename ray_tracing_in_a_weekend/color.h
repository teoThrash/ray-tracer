#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "rtweekend.h"
#include "interval.h"

using namespace std;

using color = vec3;

inline double linear_to_gamma(double linear_comp){
    if(linear_comp <= 0) return 0;
    return sqrt(linear_comp);
}

void write_color(ostream & out, const color& pixel_color){
    auto r=pixel_color.x();
    auto g=pixel_color.y();
    auto b=pixel_color.z();

    r=linear_to_gamma(r);
    g=linear_to_gamma(g);
    b=linear_to_gamma(b);

    static const interval intensity (0.000, 0.999);
    
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));



    out<<rbyte<<' '<<gbyte<<' '<<bbyte<<'\n';
}


#endif // COLOR_H
