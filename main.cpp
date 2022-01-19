#include <iostream>
#include <fstream>
#include "vec.hpp"
#include <cmath>

vec unit_vector(vec v) {
    return v / v.norm();
}

color ray_color(const ray& r) {
    vec unit_direction = unit_vector(r.getDirection());
    //std::cout << unit_direction;
    auto t = 0.5*(unit_direction.getY() + 1.0);
    //std::cout << t;
    color c=color(1.0, 1.0, 1.0)*(1.0-t) + color(0.5, 0.7, 1.0)*t;
    //std::cout << c;
    return c;
}

void write_color(std::ofstream &out, color pixel_color) {
    // Write the translated [0,255] value of each color component.
    out << (int)(255.999 * pixel_color.getX()) << ' '
        << (int)(255.999 * pixel_color.getY()) << ' '
        << (int)(255.999 * pixel_color.getZ()) << '\n';
}

int main(){
    //remove("image.ppm");
    double ratio = 16.0/9.0;
    int width = 400;
    int height = (int) width/ratio;

    double view_height = 2.0;
    double view_width = ratio*view_height;
    double focal = 1.0;

    point origin = point(0,0,0);
    vec horizontal = vec(view_width, 0, 0);
    vec vertical = vec(0, view_height, 0);
    point lw_lft_corner = origin - horizontal/2 - vertical/2 - vec(0, 0, focal);
    //std::cout << lw_lft_corner;
    std::ofstream ofs("image.ppm", std::ios::out|std::ios::binary);

    ofs << "P3\n" << width << " " << height << "\n255\n";


    for (int j = height-1; j>=0; --j){
        //std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i=0; i<width; ++i){
            double u = double(i) / (width-1);
            double v = double(j) / (height-1);
            vec dir = (vec) lw_lft_corner + horizontal*u + vertical*v - origin;
            //std::cout << dir;
            ray r(origin, dir, 1);
            color pixel_color = ray_color(r);
            //std::cout << pixel_color;
            write_color(ofs, pixel_color);
        }
    }
    //std::cerr << "\nDone.\n";
}