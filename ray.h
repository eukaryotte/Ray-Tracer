#ifndef RAY_H
#define RAY_H
#include <iostream>
#include <cmath>
#include "vec3.h"

class Ray {
    public:
    Vec3 origin;
    Vec3 direction;

    Ray(const Vec3& _origin, const Vec3& _direction)
    : origin(_origin), direction(_direction) {};

    // ray(t) で ray を係数 t 進めた位置ベクトルを出力する
    Vec3 operator()(double t) const {
        return origin + t*direction;
    };
};

std::ostream& operator<<(std::ostream& stream, const Ray& ray) {
    stream << "origin: " << ray.origin << " direction: "
    << ray.direction;
    return stream;
}



#endif