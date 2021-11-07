#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <random>
#include <numeric>
#include "SDL.h"

inline float randFloat()
{
    static std::uniform_real_distribution<float> dist;
    static std::mt19937_64 generator;
    return dist(generator);
}


inline double randomDouble() 
{
    static std::uniform_real_distribution<double> dist;
    static std::mt19937_64 generator;
    return dist(generator);
}

inline float sRGB(float u) { return (u <= 0.0031308f) ? 12.92f * u : 1.055f * std::pow(u, 0.416667f) - 0.055f; }

//use templates to make classes and functions attempt to work for multiple data types by substituting typename for whatever is used at construction time
template<typename T>
inline T min(T a, T b) { return (a < b) ? a : b; }
template<typename T>
inline T max(T a, T b) { return (a > b) ? a : b; }
template<typename T>
inline T clamp(T x, T a, T b) { return  max(a, min(b, x)); }

template<typename ve>
inline ve unit_vector(ve v) { return v / v.mag(); }

template <typename T> int sign(T val) { return (T(0) < val) - (val < T(0)); }

template <typename T> T abs(T v) { return v * ((v > 0) - (v < 0)); }

inline float ler(float a, float b, float f) { return ((float)a * (1.0 - f)) + ((float)b * f); }

template<int n, typename real>
class vec
{
public:
    //real x, y, z;

    real e[n];
    //bool useAlpha = false;
    vec() { } // default vector

    vec(const vec& v)  // copy vector
    {
        for (int i = 0; i < n; ++i) e[i] = v.e[i];
        //if (!useAlpha) e[3] = 1;
    }

    vec(const real c)  // set all to c
    {

        for (int i = 0; i < n; ++i) e[i] = c;
        //if (!useAlpha) e[3] = 1;
    }

    vec(const real x, const real y)  // set all to c
    {
        e[0] = x;
        e[1] = y;
        for (int i = 2; i < n; ++i) e[i] = y;
    }

    // Some brutal C++ hackery to enable initializer lists
    template<typename val, typename... vals, std::enable_if_t<(sizeof...(vals) > 0), int> = 0>
    inline vec(const val v, const vals... vs) : e{ (real)v , (real)vs... } { }

    inline real x() const { return e[0]; }
    inline real y() const { return e[1]; }
    inline real z() const { return e[2]; }
    inline real& x() { return e[0]; }
    inline real& y() { return e[1]; }
    inline real& z() { return e[2]; }

    inline real r() const { return e[0]; }
    inline real g() const { return e[1]; }
    inline real b() const { return e[2]; }
    inline real a() const { return e[3]; }
    inline real& r() { return e[0]; }
    inline real& g() { return e[1]; }
    inline real& b() { return e[2]; }
    inline real& a() { return e[3]; }

    inline static vec random()
    {
        vec result;
        for (int i = 0; i < n; ++i) result.e[i] = randomDouble(); // Error	C3861	'randomDouble': identifier not found

        return result;
    }

    inline static vec random(double low, double high)
    {
        vec result;
        for (int i = 0; i < n; ++i) result.e[i] = min(low, high) + randomDouble() * abs(low - high);
        return result;
    }

    int size() const { return n; }

    void operator= (const vec& rhs)
    {
        for (int i = 0; i < n; ++i) e[i] = rhs.e[i];
    }

    vec operator+   (const vec& rhs) const
    {
        vec result;
        for (int i = 0; i < n; ++i)  result.e[i] = (e[i] + rhs.e[i]);
        return result;
    }

    vec operator+= (const vec& rhs)
    {
        vec result;
        for (int i = 0; i < n; ++i) result.e[i] = (e[i] += rhs.e[i]);
        return result;
    }

    vec operator- (const vec rhs) const
    {
        vec result;
        for (int i = 0; i < n; ++i) { result.e[i] = (e[i] - rhs.e[i]); }
        return result;
    }
    vec operator*(const real rhs) const
    {
        vec result;
        for (int i = 0; i < n; ++i) { result.e[i] = (e[i] * rhs); }
        return result;
    }
    vec operator*(const vec rhs) const
    {
        vec result;
        for (int i = 0; i < n; ++i) { result.e[i] = (e[i] * rhs.e[i]); }
        return result;
    }
    vec operator/(const real rhs) const
    {
        vec result;
        const real div_rhs = 1 / rhs;
        for (int i = 0; i < n; ++i) result.e[i] = (e[i] * div_rhs);
        return result;
    }

    vec operator/(const vec rhs) const
    {
        vec result;
        for (int i = 0; i < n; ++i)
        {
            result.e[i] = (e[i] / rhs.e[i]);
        }
        return result;
    }

    vec operator>(const vec rhs) const
    {
        vec result;
        for (int i = 0; i < n; ++i)
        {
            result.e[i] = (e[i] > rhs.e[i]) ? 1 : 0;
        }
        return result;
    }

    vec operator<(const vec rhs) const
    {
        vec result;
        for (int i = 0; i < n; ++i)
        {
            result.e[i] = (e[i] < rhs.e[i]) ? 1 : 0;
        }
        return result;
    }

    vec f() const
    {
        vec result;
        for (int i = 0; i < n; ++i) { result.e[i] = static_cast<float>(e[i]); }
        return result;
    }

    vec d() const
    {
        vec result;
        for (int i = 0; i < n; ++i) { result.e[i] = static_cast<double>(e[i]); }
        return result;
    }

    vec i() const
    {
        vec result;
        for (int i = 0; i < n; ++i) { result.e[i] = static_cast<int>(e[i]); }
        return result;
    }

    vec Ceil() const
    {
        vec result;
        for (int i = 0; i < n; ++i)
        {
            result.e[i] = ceil(e[i]);
        }
        return result;
    }
    vec Floor() const
    {
        vec result;
        for (int i = 0; i < n; ++i)
        {
            result.e[i] = floor(e[i]);
        }
        return result;
    }

    vec lerpTo(vec b, float f)
    {
        vec result;
        for (int i = 0; i < n; ++i)
        {
            result.e[i] = ((float)e[i] * (1.0 - f)) + ((float)b.e[i] * f);  //floor(e[i]);

        }

        return result;
    }

    vec abso() const
    {
        vec result;
        for (int i = 0; i < n; ++i)
        {
            result.e[i] = abs(e[i]);
        }
        return result;
    }
    // user defined conversion from vector with 3 dimensions to pixelRGBA // e.g. vec<3 ,  double> to , vec<4, float> where channel 4 (alpha) is set to 1
    explicit operator vec<4, float>() const
    {
        vec<4, float> result;
        for (int i = 0; i < 3; ++i)
        {
            result.e[i] = static_cast<float>(e[i]);
        }
        result.e[3] = 1;
        return result;
    }

    vec rotate2d(const real a)
    {
        vec result;
        real ang = a;
        real cs = std::cos(a);
        real sn = std::sin(a);
        result.e[0] = e[0] * cs - e[1] * sn;
        result.e[1] = e[0] * sn + e[1] * cs;
        return result;
    }
    vec normal2D() const
    {
        vec result;
        result.x() = e[1];
        result.y() = e[0] * -1;
        return result;
    }
    real XxY() const
    {
        real result = e[0] * e[1];
        return result;
    }
    bool XisYmag() const
    {
        return (abs(e[0]) == abs(e[1]));
    }
    real dot(const vec& a, const vec& b) const
    {
        return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
    } //const because it only returns values, not affecting any values

    real dot(const vec& a) const
    {
        return a.x() * e[0] + a.y() * e[1] + a.z() * e[2];
    } //const because it only returns values, not affecting any values



    real distance2(const vec b) const
    {
        if (n == 3)
        {
            return  (e[0] - b.e[0]) * (e[0] - b.e[0]) +
                    (e[1] - b.e[1]) * (e[1] - b.e[1]) +
                    (e[2] - b.e[2]) * (e[2] - b.e[2]);
        }
        if (n == 2)
        {
            return  (e[0] - b.e[0]) * (e[0] - b.e[0]) +
                    (e[1] - b.e[1]) * (e[1] - b.e[1]);
        }
        else { return 0; }

    }

    real distance(const vec& b) const { return std::sqrt(distance2(b)); }

    real mag()                 const { return std::sqrt(dot(*this, *this)); }   // same as std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);    }
    real mag2(const vec& v)    const { return dot(*this, v); }
    real length(const vec& v)  const { return std::sqrt(dot(*this, v)); }   // same as std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);    }
    real length2()             const { return dot(*this, *this); }

    real luminance() const
    {
        return (0.299 * r() + 0.587 * g() + 0.114 * b());
    }

    vec vto8bit()
    {
        int i[n];
        for (int i = 0; i < n; ++i)
        {
            i[i] = to8bit(e[i]);
        }

        return i; //return { e[0] , e[1] , e[2] , e[3] }; //return  { to8bit(e[0]) , to8bit(e[1]) , to8bit(e[2]) , to8bit(e[3]) };
    }

    vec int_unit_vector() 
    {
        vec result;
        for (int i = 0; i < n; ++i)
        {
            result.e[i] = (e[i] == 0) ? 0 : 1 * sign(e[i]);
        }
        return result;
    }

    std::string toString(const bool vertex) const
    {
        std::string output = "";

        if (vertex) output.append("v { ");

        for (int i = 0; i < n; ++i)
        {
            output.append(std::to_string(e[i]));
            output.append(", ");
        }

        if (vertex) output.append(" } ");

        return output;
    }

    Uint32 vtoUint32() // convert 3 component vector to Uint32 for use with SDL2  https://youtu.be/JN5yUrJPThI?t=3258
    {
        unsigned char r = static_cast<unsigned char>(e[0]);
        unsigned char g = static_cast<unsigned char>(e[1]);
        unsigned char b = static_cast<unsigned char>(e[2]);
        //unsigned char a = static_cast<unsigned char>(e[3]);

        #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        Uint32 pixColour = (r << 24) + (g << 16) + (b << 8) + 255;
        #else
        Uint32 pixColour = (255 << 24) + (r << 16) + (g << 8) + b;
        #endif
        return pixColour;
    }

};

using vec3b = vec<3, bool>;
using vec3i = vec<3, int>;
using vec3f = vec<3, float>;
using vec3d = vec<3, double>;

using vec2i = vec<2, int>;
using vec2f = vec<2, float>;
using vec2d = vec<2, double>;

using pixel8bit = vec<4, uint8_t>;
using pixelRGBA = vec<4, float>;

template<typename ve>
inline double dot(const ve& a, const ve& b)
{
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}
/*
vec3d random_in_unit_sphere() {

    while (true) {
        vec3d p = vec3d::random();
        if (p.length2() >= 1) continue;
        return p;
    }
}
*/
inline uint8_t to8bit(float v)
{
    float v_sRGB = sRGB(v);
    int d = int(256 * v_sRGB);
    return clamp(d, 0, 255);
}