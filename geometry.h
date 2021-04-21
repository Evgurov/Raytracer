#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>
#include <cassert>
#include <iostream>

template <size_t dim, typename T> struct vec {
    vec() { for (size_t i = 0; i < dim; i++) data[i] = T(); }
    T&       operator[](const size_t i)       { assert(i < dim); return data[i]; }
    const T& operator[](const size_t i) const { assert(i < dim); return data[i]; }
private:
    T data[dim];
};

typedef vec<2, float> vec2f;
typedef vec<2, unsigned> vec2u;
typedef vec<3, float> vec3f;
typedef vec<3, unsigned> vec3u;

template <typename T> struct vec<2, T> {
    vec() : x(), y() {} 
    vec(const T X, const T Y) : x(X), y(Y) {}
    T&       operator[](const size_t i)       { assert(i < 2); return i <= 0 ? x : y; }
    const T& operator[](const size_t i) const { assert(i < 2); return i <= 0 ? x : y; }
    void operator=(const vec<2,T>& rhs) {x = rhs.x; y = rhs.y;}
    T x, y; 
};

template <typename T> struct vec<3, T> {
    vec() : x(), y(), z() {} 
    vec(const T X, const T Y, const T Z) : x(X), y(Y), z(Z) {}
    T&       operator[](const size_t i)       { assert(i < 3); return i <= 0 ? x : (1 == i ? y : z); }
    const T& operator[](const size_t i) const { assert(i < 3); return i <= 0 ? x : (1 == i ? y : z); }
    void operator=(const vec<2,T>& rhs) {x = rhs.x; y = rhs.y; z = rhs.z;}
    float norm() const {return std::sqrt(x*x + y*y + z*z);}
    vec<3, T>& normalize() { *this = (*this) * (1 / norm()); return *this; }
    T x, y, z; 
};

template <size_t dim, typename T> T operator*(const vec<dim, T>& lhs, const vec<dim, T>& rhs) {
    T res = T();
    for (size_t i = 0; i < dim; i++)
        res += lhs[i] * rhs[i];
    return res;
}

template <size_t dim, typename T> vec<dim, T> operator+(vec<dim, T> lhs, const vec<dim,T>& rhs) {
    for (size_t i = 0; i < dim; i++)
        lhs[i] += rhs[i];
    return lhs;
}

template <size_t dim, typename T> vec<dim, T> operator-(vec<dim, T> lhs, const vec<dim, T>& rhs) {
    for (size_t i = 0; i < dim; i++)
        lhs[i] -= rhs[i];
    return lhs;
}

template <size_t dim,typename T,typename U> vec<dim,T> operator*(const vec<dim,T>& lhs, const U& rhs) {
    vec<dim,T> res;
    for (size_t i = 0; i < dim; i++) 
        res[i] = lhs[i] * rhs;
    return res;
}

template <size_t dim,typename T> vec<dim,T> operator-(const vec<dim,T> &lhs) {
    return lhs * T(-1);
}

template <typename T> vec<3,T> cross(vec<3,T> v1, vec<3,T> v2) {
    return vec<3,T>(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}

template <size_t dim, typename T> std::ostream& operator<<(std::ostream& out, const vec<dim,T>& v) {
    for(size_t i = 0; i < dim; i++)
        out << v[i] << " " ;
    return out;
}

#endif 