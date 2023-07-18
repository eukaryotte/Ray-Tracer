/* #ifndef で指定したシンボルが定義されていない場合に下記を実行 */
#ifndef VEC3_H
/* マクロを定義する */
#define VEC3_H
#include <iostream>
#include <cmath>

class Vec3 {
    // 各成分を倍精度(64bit)浮動小数点数で表現
    public:
    double x;
    double y;
    double z;


    // Vec3() は零ベクトル
    // Vec3(t) は各成分が t のベクトル
    // Vec3(x, y, z) で 各成分が x, y, z のベクトル
    Vec3() { x = y = z = 0;} ;
    Vec3(double _x) { x = y = z = _x; };
    Vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {};

    // ベクトルの長さを各成分の二乗の総和の平方根で取得する
    double length() const {
        return std::sqrt(x*x + y*y + z*z);
    };

    double FastInverseSquareRoot(double x) const {
    double xhalf = 0.5 * x;
    long long i = *(long long*)&x;
    i = 0x5fe6ec85e7de30daLL - (i >> 1);
    x = *(double*)&i;
    x = x * (1.5 - xhalf * x * x);
    return x;
    };


    // ベクトルの長さの二乗
    double length2() const {
        return x*x + y*y + z*z;
    };
};

// ベクトル同士の演算をオーバーロードで定義する
Vec3 operator+(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vec3 operator-(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vec3 operator*(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

Vec3 operator/(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

// ベクトルとスカラーの演算
Vec3 operator+(const Vec3& v, double k) {
    return Vec3(v.x + k, v.y + k, v.z + k);
}

Vec3 operator+(double k, const Vec3& v) {
    return v + k;
}

Vec3 operator-(const Vec3& v, double k) {
    return Vec3(v.x - k, v.y - k, v.z - k);
}

Vec3 operator-(double k, const Vec3& v) {
    return v - k;
}

Vec3 operator*(const Vec3& v, double k) {
    return Vec3(v.x * k, v.y * k, v.z * k);
}

Vec3 operator*(double k, const Vec3& v) {
    return v * k;
}

Vec3 operator/(const Vec3& v, double k) {
    return Vec3(v.x / k, v.y / k, v.z / k);
}
// スカラーをベクトルで割る計算の定義が独特であることに注意
Vec3 operator/(double k, const Vec3& v) {
    return Vec3(k / v.x, k / v.y, k / v.z);
}

// 内積
double dot(const Vec3& v1, const Vec3& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// 外積
Vec3 cross(const Vec3& v1, const Vec3& v2) {
    return Vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
    v1.x * v2.y - v1.y * v2.x);
}

// 正規化
Vec3 normalize(const Vec3& v) {
    return v/v.length();
}

// コンソール出力
std::ostream& operator<<(std::ostream& stream, const Vec3& v) {
    stream << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return stream;
}

// 正規直交基底を作る
void orthonormalBasis(const Vec3& v1, Vec3& v2, Vec3& v3) {
    if(std::abs(v1.x) > 0.9) v2 = Vec3(0, 1, 0);
    else v2 = Vec3(1, 0, 0);

    v2 = normalize(v2 - dot(v1, v2)*v1);
    v3 = cross(v1, v2);
}

#endif
