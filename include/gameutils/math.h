#pragma once

#include <algorithm>
#include <cmath>
#include <iosfwd>
#include <limits>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace gameutils {

template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
    almostEqual(T x, T y, int ulp)
{
    // The machine epsilon has to be scaled to the magnitude of the larger
    // value and multiplied by the desired precision in ULPs (units in the
    // last place).
    return std::abs(x-y) <= std::numeric_limits<T>::epsilon()
                          * std::max(std::abs(x), std::abs(y))
                          * ulp;
}

template<typename T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
    almostEqual(const T *a, const T *b, int count, int ulp)
{
    for (int i = 0; i < count; ++i) {
        if (!almostEqual(a[i], b[i], ulp)) {
            return false;
        }
    }

    return true;
}

//----------------------------------------------------------------------------
//
// Vec2
//
//----------------------------------------------------------------------------

template<typename T>
struct Vec2
{
    Vec2()
      : x(0)
      , y(0) { }

    Vec2(const Vec2 &r)
      : x(r.x)
      , y(r.y) { }

    Vec2(T x, T y)
      : x(x)
      , y(y) { }

    explicit Vec2(T r[])
      : x(r[0])
      , y(r[1]) { }

    Vec2 conjugate() const
    {
        return Vec2(x, -y);
    }

    bool equalTo(const Vec2 &r, int ulp) const
    {
        return almostEqual(d, r.d, 2, ulp);
    }

    const T* data() const
    {
        return d;
    }

    T* data()
    {
        return d;
    }

    T dot(const Vec2& r) const
    {
        return x * r.x + y * r.y;
    }

    T length() const
    {
        return std::sqrt(x * x + y * y);
    }

    void normalise()
    {
        *this = normalised();
    }

    Vec2 normalised() const
    {
        const T len = length();

        if (almostEqual(len, 0, 5)) {
            return Vec2(1, 0);
        }

        return Vec2(x / len, y / len);
    }

    Vec2 perp() const
    {
        return Vec2(-y, x);
    }

    Vec2 reflect(const Vec2& normal) const
    {
        return *this - (2 * dot(normal) * normal).normalised();
    }

    Vec2 operator-() const
    {
        return Vec2(-x, -y);
    }

    Vec2 operator+(const Vec2& r) const
    {
        return Vec2(x + r.x, y + r.y);
    }

    Vec2 operator-(const Vec2& r) const
    {
        return Vec2(x - r.x, y - r.y);
    }

    Vec2 operator*(double n) const
    {
        return Vec2(x * n, y * n);
    }

    Vec2 operator/(double n) const
    {
        return Vec2(x / n, y / n);
    }

    void operator+=(const Vec2& r)
    {
        x += r.x;
        y += r.y;
    }

    void operator-=(const Vec2& r)
    {
        x -= r.x;
        y -= r.y;
    }

    void operator*=(T n)
    {
        x *= n;
        y *= n;
    }

    void operator/=(T n)
    {
        x /= n;
        y /= n;
    }

    union
    {
        struct { T x, y; };
        struct { T u, v; };

        T d[2];
    };
};

template<typename T>
inline Vec2<T> operator*(T n, const Vec2<T>& v)
{
    return v * n;
}

template<typename T>
inline T dot(const Vec2<T> &l, const Vec2<T> &r)
{
    return l.dot(r);
}


//----------------------------------------------------------------------------
//
// Vec3
//
//----------------------------------------------------------------------------

template<typename T>
struct Vec3
{
    Vec3()
      : x(0)
      , y(0)
      , z(0) { }

    Vec3(const Vec2<T> &v2, T z)
      : x(v2.x)
      , y(v2.y)
      , z(z) { }

    Vec3(const Vec3& r)
      : x(r.x)
      , y(r.y)
      , z(r.z) { }

    Vec3(T x, T y, T z)
      : x(x)
      , y(y)
      , z(z) { }

    explicit Vec3(T r[])
      : x(r[0])
      , y(r[1])
      , z(r[2]) { }

    const Vec2<T>& asVec2() const
    {
        return *reinterpret_cast<Vec2<T>*>(this);
    }

    Vec2<T>& asVec2()
    {
        return *reinterpret_cast<Vec2<T>*>(this);
    }

    Vec3 cross(const Vec3 &r) const
    {
        return Vec3(
            y * r.z - r.y * z,
            z * r.x - r.z * x,
            x * r.y - r.x * y);
    }

    const T* data() const
    {
        return d;
    }

    T* data()
    {
        return d;
    }

    T dot(const Vec3 &r) const
    {
        return x * r.x + y * r.y + z * r.z;
    }

    bool equalTo(const Vec3 &r, int ulp) const
    {
        return almostEqual(d, r.d, 3, ulp);
    }

    T length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    void normalise()
    {
        *this = normalised();
    }

    Vec3 normalised() const
    {
        T len = length();
        if (almostEqual<T>(len, 0, 5)) {
            return Vec3(1, 0, 0);
        }

        len = 1 / len;
        return Vec3(x * len, y * len, z * len);
    }

    Vec3 reflect(const Vec3& normal) const
    {
        return *this - (2 * dot(normal) * normal).normalised();
    }

    Vec3 operator-() const
    {
        return Vec3(-x, -y, -z);
    }

    Vec3 operator+(const Vec3& r) const
    {
        return Vec3(x + r.x, y + r.y, z + r.z);
    }

    Vec3 operator-(const Vec3& r) const
    {
        return Vec3(x - r.x, y - r.y, z - r.z);
    }

    Vec3 operator*(T n) const
    {
        return Vec3(x * n, y * n, z * n);
    }

    Vec3 operator/(T n) const
    {
        n = 1 / n;

        return Vec3(x * n, y * n, z * n);
    }

    void operator+=(const Vec3& r)
    {
        x += r.x;
        y += r.y;
        z += r.z;
    }

    void operator-=(const Vec3& r)
    {
        x -= r.x;
        y -= r.y;
        z -= r.z;
    }

    void operator*=(T n)
    {
        x *= n;
        y *= n;
        z *= n;
    }

    void operator/=(T n)
    {
        n = 1 / n;
        x *= n;
        y *= n;
        z *= n;
    }

    union
    {
        struct { T x, y, z; };
        struct { T r, g, b; };

        T d[3];
    };
};

template<typename T>
inline Vec3<T> operator*(T n, const Vec3<T>& r)
{
    return Vec3<T>(r.x * n, r.y * n, r.z * n);
}

template<typename T>
inline Vec3<T> cross(const Vec3<T> &l, const Vec3<T> &r)
{
    return l.cross(r);
}

template<typename T>
inline T dot(const Vec3<T> &l, const Vec3<T> &r)
{
    return l.dot(r);
}


//----------------------------------------------------------------------------
//
// Vec4
//
//----------------------------------------------------------------------------

template<typename T>
struct Vec4
{
    Vec4()
      : x(0)
      , y(0)
      , z(0)
      , w(0) { }

    Vec4(const Vec2<T> &v2, T z, T w)
      : x(v2.x)
      , y(v2.y)
      , z(z)
      , w(w) { }

    Vec4(const Vec3<T> &v3, T w)
      : x(v3.x)
      , y(v3.y)
      , z(v3.z)
      , w(w) { }

    Vec4(const Vec4& r)
      : x(r.x)
      , y(r.y)
      , z(r.z)
      , w(r.w) { }

    Vec4(T x, T y, T z, T w)
      : x(x)
      , y(y)
      , z(z)
      , w(w) { }

    explicit Vec4(T r[])
      : x(r[0])
      , y(r[1])
      , z(r[2])
      , w(r[3]) { }

    const Vec2<T>& asVec2() const
    {
        return *reinterpret_cast<Vec2<T>*>(this);
    }

    Vec2<T>& asVec2()
    {
        return *reinterpret_cast<Vec2<T>*>(this);
    }

    const Vec3<T>& asVec3() const
    {
        return *reinterpret_cast<Vec3<T>*>(this);
    }

    Vec3<T>& asVec3()
    {
        return *reinterpret_cast<Vec3<T>*>(this);
    }

    const T* data() const
    {
        return d;
    }

    T* data()
    {
        return d;
    }

    T dot(const Vec4& r) const
    {
        return x * r.x + y * r.y + z * r.z + w * r.w;
    }

    bool equalTo(const Vec4& r, int ulp)
    {
        return almostEqual(d, r.d, 4, ulp);
    }

    T length() const
    {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    void normalise()
    {
        *this = normalised();
    }

    Vec4 normalised() const
    {
        const T len = length();
        if (almostEqual(len, 0, 5)) {
            return Vec4(1, 0, 0, 0);
        }

        len = 1 / len;
        return Vec4(x * len, y * len, z * len, w * len);
    }

    Vec4 operator-() const
    {
        return Vec4(-x, -y, -z, -w);
    }

    Vec4 operator+(const Vec4& r) const
    {
        return Vec4(x + r.x, y + r.y, z + r.z, w + r.w);
    }

    Vec4 operator-(const Vec4& r) const
    {
        return Vec4(x - r.x, y - r.y, z - r.z, w - r.w);
    }

    Vec4 operator*(T n) const
    {
        return Vec4(x * n, y * n, z * n, w * n);
    }

    Vec4 operator/(T n) const
    {
        n = 1 / n;
        return Vec4(x * n, y * n, z * n, w * n);
    }

    void operator+=(const Vec4& r)
    {
        x += r.x;
        y += r.y;
        z += r.z;
        w += r.w;
    }

    void operator-=(const Vec4& r)
    {
        x -= r.x;
        y -= r.y;
        z -= r.z;
        w -= r.w;
    }

    void operator*=(T n)
    {
        x *= n;
        y *= n;
        z *= n;
        w *= n;
    }

    void operator/=(T n)
    {
        n = 1 / n;
        x *= n;
        y *= n;
        z *= n;
        w *= n;
    }

    union
    {
        struct { T x, y, z, w; };
        struct { T r, g, b, a; };
        struct { T s, t, p, q; };

        T d[4];
    };
};

template<typename T>
inline Vec4<T> operator*(T n, const Vec4<T>& r)
{
    return r.mul(n);
}

template<typename T>
inline T dot(const Vec4<T> &l, const Vec4<T> &r)
{
    return l.dot(r);
}


//----------------------------------------------------------------------------
//
// Mat3
//
//----------------------------------------------------------------------------

template<typename T>
struct Mat3
{
    Mat3()
      : m00(0), m10(0), m20(0)  // Column 0
      , m01(0), m11(0), m21(0)  // Column 1
      , m02(0), m12(0), m22(0)  { }

    Mat3(const Mat3& r)
      : m00(r.m00), m10(r.m10), m20(r.m20)
      , m01(r.m01), m11(r.m11), m21(r.m21)
      , m02(r.m02), m12(r.m12), m22(r.m22) { }

    //
    // Constructor, column-major ordering
    //
    Mat3(T m00, T m10, T m20,  // Column 0
         T m01, T m11, T m21,  // Column 1
         T m02, T m12, T m22)  // Column 2
      : m00(m00), m10(m10), m20(m20)
      , m01(m01), m11(m11), m21(m21)
      , m02(m02), m12(m12), m22(m22) { }

    Mat3(Vec3<T> col0, Vec3<T> col1, Vec3<T> col2)
      : m00(col0.x), m10(col0.y), m20(col0.z)  // Column 0
      , m01(col1.x), m11(col1.y), m21(col1.z)  // Column 1
      , m02(col2.x), m12(col2.y), m22(col2.z) { }

    const T* data() const
    {
        return d;
    }

    T* data()
    {
        return d;
    }

    bool equalTo(const Mat3& r, int ulp) const
    {
        return almostEqual(d, r.d, 9, ulp);
    }

    static Mat3<T> identity()
    {
        return Mat3(
            1, 0, 0,
            0, 1, 0,
            0, 0, 1);
    }

    Mat3<T> inverse() const
    {
        Mat3<T> inv;

        inv.m00 = m11 * m22 - m21 * m12;
        inv.m01 = m21 * m02 - m01 * m22;
        inv.m02 = m01 * m12 - m11 * m02;

        const T det = m00 * inv.m00 + m10 * inv.m01 + m20 * inv.m02;
        const T invDet = 1 / det;

        inv.m10 = m20 * m12 - m10 * m22;
        inv.m20 = m10 * m21 - m20 * m11;
        inv.m11 = m00 * m22 - m20 * m02;
        inv.m21 = m20 * m01 - m00 * m21;
        inv.m12 = m10 * m02 - m00 * m12;
        inv.m22 = m00 * m11 - m10 * m01;

        return (invDet * inv);
    }

    void invert()
    {
        *this = inverse();
    }

    Mat3 transpose() const
    {
        return Mat3(
            m00, m01, m02,
            m10, m11, m12,
            m20, m21, m22);
    }

    Mat3 operator+(const Mat3& r) const
    {
        return Mat3(m00 + r.m00, m10 + r.m10, m20 + r.m20,
                    m01 + r.m01, m11 + r.m11, m21 + r.m21,
                    m02 + r.m02, m12 + r.m12, m22 + r.m22);
    }

    Mat3 operator-(const Mat3& r) const
    {
        return Mat3(m00 - r.m00, m10 - r.m10, m20 - r.m20,
                    m01 - r.m01, m11 - r.m11, m21 - r.m21,
                    m02 - r.m02, m12 - r.m12, m22 - r.m22);
    }

    Mat3 operator*(const Mat3& r) const
    {
        return Mat3(
            m00 * r.m00 + m01 * r.m10 + m02 * r.m20,
            m10 * r.m00 + m11 * r.m10 + m12 * r.m20,
            m20 * r.m00 + m21 * r.m10 + m22 * r.m20,
            m00 * r.m01 + m01 * r.m11 + m02 * r.m21,
            m10 * r.m01 + m11 * r.m11 + m12 * r.m21,
            m20 * r.m01 + m21 * r.m11 + m22 * r.m21,
            m00 * r.m02 + m01 * r.m12 + m02 * r.m22,
            m10 * r.m02 + m11 * r.m12 + m12 * r.m22,
            m20 * r.m02 + m21 * r.m12 + m22 * r.m22);
    }

    Mat3 operator*(T n) const
    {
        return Mat3(m00 * n, m10 * n, m20 * n,
                    m01 * n, m11 * n, m21 * n,
                    m02 * n, m12 * n, m22 * n);
    }

    Mat3 operator/(T n) const
    {
        n = 1 / n;
        return Mat3(m00 * n, m10 * n, m20 * n,
                    m01 * n, m11 * n, m21 * n,
                    m02 * n, m12 * n, m22 * n);
    }

    void operator+=(const Mat3& r)
    {
        *this = *this + r;
    }

    void operator-=(const Mat3& r)
    {
        *this = *this + r;
    }

    void operator*=(const Mat3& r)
    {
        *this = *this * r;
    }

    void operator*=(T n)
    {
        *this = *this * n;
    }

    void operator/=(T n)
    {
        *this = *this / n;
    }

    const T& operator[](int n) const
    {
        return d[n];
    }

    T& operator[](int n)
    {
        return d[n];
    }

    union
    {
        struct   // Column-major ordering (1st num => row, 2nd num => column)
        {
            T m00, m10, m20;   // Column 0
            T m01, m11, m21;   // Column 1
            T m02, m12, m22;   // Column 2
        };

        T d[9];  // Raw data
    };
};

template<typename T>
inline Mat3<T> operator*(T n, const Mat3<T>& r)
{
    return r * n;
}

template<typename T>
inline Vec3<T> operator*(const Mat3<T>& m, const Vec3<T>& v)
{
    return Vec3<T>(
        m.m00 * v.x + m.m01 * v.y + m.m02 * v.z,
        m.m10 * v.x + m.m11 * v.y + m.m12 * v.z,
        m.m20 * v.x + m.m21 * v.y + m.m22 * v.z);
}


//----------------------------------------------------------------------------
//
// Mat4
//
//----------------------------------------------------------------------------

template<typename T>
struct Mat4
{
    Mat4()
    : m00(0), m10(0), m20(0), m30(0)  // Column 0
    , m01(0), m11(0), m21(0), m31(0)  // Column 1
    , m02(0), m12(0), m22(0), m32(0)  // Column 2
    , m03(0), m13(0), m23(0), m33(0) { }

    //
    // Constructor using column-major ordering
    //
    Mat4(T m00, T m10, T m20, T m30,    // Column 0
         T m01, T m11, T m21, T m31,    // Column 1
         T m02, T m12, T m22, T m32,    // Column 2
         T m03, T m13, T m23, T m33)    // Column 3
    : m00(m00), m10(m10), m20(m20), m30(m30)
    , m01(m01), m11(m11), m21(m21), m31(m31)
    , m02(m02), m12(m12), m22(m22), m32(m32)
    , m03(m03), m13(m13), m23(m23), m33(m33) { }

    Mat4(Vec4<T> col0, Vec4<T> col1, Vec4<T> col2, Vec4<T> col3)
    : m00(col0.x), m10(col0.y), m20(col0.z), m30(col0.w)
    , m01(col1.x), m11(col1.y), m21(col1.z), m31(col1.w)
    , m02(col2.x), m12(col2.y), m22(col2.z), m32(col2.w)
    , m03(col3.x), m13(col3.y), m23(col3.z), m33(col3.w) { }

    Mat4(const Mat4& r)
    : m00(r.m00), m10(r.m10), m20(r.m20), m30(r.m30)
    , m01(r.m01), m11(r.m11), m21(r.m21), m31(r.m31)
    , m02(r.m02), m12(r.m12), m22(r.m22), m32(r.m32)
    , m03(r.m03), m13(r.m13), m23(r.m23), m33(r.m33) { }

    explicit Mat4(const Mat3<T> &r)
    : m00(r.m00), m10(r.m10), m20(r.m20), m30(0)
    , m01(r.m01), m11(r.m11), m21(r.m21), m31(0)
    , m02(r.m02), m12(r.m12), m22(r.m22), m32(0)
    , m03(0),     m13(0),     m23(0),     m33(1) { }

    const T* data() const
    {
        return d;
    }

    T* data()
    {
        return d;
    }

    bool equalTo(const Mat4 &r, int ulp) const
    {
        return almostEqual(d, r.d, 16, ulp);
    }

    static Mat4 identity()
    {
        return Mat4<T>(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);
    }

    Mat3<T> makeMat3() const
    {
        return Mat3<T>(
            m00, m10, m20,
            m01, m11, m21,
            m02, m12, m22);
    }

    static Mat4 orthogonal(T left,   T right,
                           T bottom, T top,
                           T zNear,  T zFar)
    {
        const T ral = right + left,
                rsl = right - left,
                tab = top + bottom,
                tsb = top - bottom,
                fan = zFar + zNear,
                fsn = zFar - zNear;

        const T nm00 =  2 / rsl,
                nm11 =  2 / tsb,
                nm22 = -2 / fsn,
                nm03 =  -ral / rsl,
                nm13 =  -tab / tsb,
                nm23 =  -fan / fsn;

        return Mat4(
            nm00, 0, 0, 0,
            0, nm11, 0, 0,
            0, 0, nm22, 0,
            nm03, nm13, nm23, 1);
    }

    static Mat4 perspective(T yFov, T aspect, T zNear, T zFar)
    {
        const T cotan = 1 / tan(yFov / 2 / 180 * M_PI);

        return Mat4(
            cotan / aspect, 0, 0, 0,
            0, cotan, 0, 0,
            0, 0, (zFar + zNear) / (zNear - zFar), -1,
            0, 0, (2 * zFar * zNear) / (zNear - zFar), 0);
    }

    static Mat4 translation(T tx, T ty, T tz)
    {
        return Mat4(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            tx, ty, tz, 1);
    }

    Mat4 transpose() const
    {
        return Mat4(
            m00, m01, m02, m03,
            m10, m11, m12, m13,
            m20, m21, m22, m23,
            m30, m31, m32, m33);
    }

    Mat4 operator+(const Mat4& r) const
    {
        return Mat4(
            m00 + r.m00, m10 + r.m10, m20 + r.m20, m30 + r.m30,
            m01 + r.m01, m11 + r.m11, m21 + r.m21, m31 + r.m31,
            m02 + r.m02, m12 + r.m12, m22 + r.m22, m32 + r.m32,
            m03 + r.m03, m13 + r.m13, m23 + r.m23, m33 + r.m33);
    }

    Mat4 operator-(const Mat4& r) const
    {
        return Mat4(
            m00 - r.m00, m10 - r.m10, m20 - r.m20, m30 - r.m30,
            m01 - r.m01, m11 - r.m11, m21 - r.m21, m31 - r.m31,
            m02 - r.m02, m12 - r.m12, m22 - r.m22, m32 - r.m32,
            m03 - r.m03, m13 - r.m13, m23 - r.m23, m33 - r.m33);
    }

    Mat4 operator*(const Mat4& r) const
    {
        return Mat4(
            m00 * r.m00 + m01 * r.m10 + m02 * r.m20 + m03 * r.m30,
            m10 * r.m00 + m11 * r.m10 + m12 * r.m20 + m13 * r.m30,
            m20 * r.m00 + m21 * r.m10 + m22 * r.m20 + m23 * r.m30,
            m30 * r.m00 + m31 * r.m10 + m32 * r.m20 + m33 * r.m30,
            m00 * r.m01 + m01 * r.m11 + m02 * r.m21 + m03 * r.m31,
            m10 * r.m01 + m11 * r.m11 + m12 * r.m21 + m13 * r.m31,
            m20 * r.m01 + m21 * r.m11 + m22 * r.m21 + m23 * r.m31,
            m30 * r.m01 + m31 * r.m11 + m32 * r.m21 + m33 * r.m31,
            m00 * r.m02 + m01 * r.m12 + m02 * r.m22 + m03 * r.m32,
            m10 * r.m02 + m11 * r.m12 + m12 * r.m22 + m13 * r.m32,
            m20 * r.m02 + m21 * r.m12 + m22 * r.m22 + m23 * r.m32,
            m30 * r.m02 + m31 * r.m12 + m32 * r.m22 + m33 * r.m32,
            m00 * r.m03 + m01 * r.m13 + m02 * r.m23 + m03 * r.m33,
            m10 * r.m03 + m11 * r.m13 + m12 * r.m23 + m13 * r.m33,
            m20 * r.m03 + m21 * r.m13 + m22 * r.m23 + m23 * r.m33,
            m30 * r.m03 + m31 * r.m13 + m32 * r.m23 + m33 * r.m33);
    }

    Mat4 operator*(T n) const
    {
        return Mat4(
            m00 * n, m01 * n, m02 * n, m03 * n,
            m10 * n, m11 * n, m12 * n, m13 * n,
            m20 * n, m21 * n, m22 * n, m23 * n,
            m30 * n, m31 * n, m32 * n, m33 * n);
    }

    Mat4 operator/(T n) const
    {
        n = 1 / n;
        return Mat4(
            m00 * n, m01 * n, m02 * n, m03 * n,
            m10 * n, m11 * n, m12 * n, m13 * n,
            m20 * n, m21 * n, m22 * n, m23 * n,
            m30 * n, m31 * n, m32 * n, m33 * n);
    }

    void operator+=(const Mat4& r)
    {
        *this = *this + r;
    }

    void operator-=(const Mat4& r)
    {
        *this = *this - r;
    }

    void operator*=(const Mat4& r)
    {
        *this = *this * r;
    }

    void operator*=(T n)
    {
        *this = *this * n;
    }

    void operator/=(T n)
    {
        *this = *this / n;
    }

    const T& operator[](int n) const
    {
        return d[n];
    }

    T& operator[](int n)
    {
        return d[n];
    }

    union
    {
        struct     // Column-major ordering (1st num => row, 2nd num => column)
        {
            T m00, m10, m20, m30;   // Column 0
            T m01, m11, m21, m31;   // Column 1
            T m02, m12, m22, m32;   // Column 2
            T m03, m13, m23, m33;   // Column 3
        };

        struct     // Column vectors
        {
            Vec4<T> col0, col1, col2, col3;
        };

        T d[16];   // Raw data
    };
};

template<typename T>
inline Mat4<T> operator*(T n, const Mat4<T>& r)
{
    return r * n;
}

template<typename T>
inline Vec4<T> operator*(const Mat4<T>& m, const Vec4<T>& v)
{
    return Vec4<T>(
        m.m00 * v.x + m.m01 * v.y + m.m02 * v.z + m.m03 * v.w,
        m.m10 * v.x + m.m11 * v.y + m.m12 * v.z + m.m13 * v.w,
        m.m20 * v.x + m.m21 * v.y + m.m22 * v.z + m.m23 * v.w,
        m.m30 * v.x + m.m31 * v.y + m.m32 * v.z + m.m33 * v.w);
}


//----------------------------------------------------------------------------
//
// Quat
//
//----------------------------------------------------------------------------

template<typename T>
struct Quat
{
    Quat()
      : scalar(1)
      , x(0)
      , y(0)
      , z(0) { }

    Quat(const Quat &r)
      : scalar(r.scalar)
      , x(r.x)
      , y(r.y)
      , z(r.z) { }

    Quat(T scalar, Vec3<T> q)
      : scalar(scalar)
      , x(q.x)
      , y(q.y)
      , z(q.z) { }

    Quat(T scalar, T x, T y, T z)
      : scalar(scalar)
      , x(x)
      , y(y)
      , z(z) { }

    Quat conjugate() const
    {
        return Quat(scalar, -x, -y, -z);
    }

    const T* data() const
    {
        return d;
    }

    T* data()
    {
        return d;
    }

    bool equalTo(const Quat& r, int ulp)
    {
        return almostEqual(d, r.d, 4, ulp);
    }

    static Quat<T> identity()
    {
        return Quat<T>(1, 0, 0, 0);
    }

    Mat3<T> makeMat3() const
    {
        const T n = x * x + y * y + z * z + scalar * scalar;
        const T s = (n > 0) ? (2 / n) : 0;

        const T xs = x * s,
                ys = y * s,
                zs = z * s;

        const T wx = scalar * xs,
                wy = scalar * ys,
                wz = scalar * zs;

        const T xx = x * xs,
                xy = x * ys,
                xz = x * zs;

        const T yy = y * ys,
                yz = y * zs;

        const T zz = z * zs;

        return Mat3<T>(
            1 - (yy + zz), xy + wz, xz - wy,
            xy - wz, 1 - (xx + zz), yz + wx,
            xz + wy, yz - wx, 1 - (xx + yy));
    }

    Mat4<T> makeMat4() const
    {
        return Mat4<T>(makeMat3());
    }

    void normalise()
    {
        *this = normalised();
    }

    Quat normalised() const
    {
        const T length = std::sqrt(x * x + y * y + z * z + scalar * scalar);
        if (length == 0) {
            return Quat();
        }

        const T scale = 1 / length;
        return Quat(scalar * scale, x * scale, y * scale, z * scale);
    }

    static Quat rotation(T angle, T x, T y, T z)
    {
        if (almostEqual<T>(angle, 0, 5)) {
            return Quat();
        }

        angle *= 0.5;
        T sintheta = std::sin(angle);
        return Quat(std::cos(angle), sintheta * Vec3<T>(x, y, z)).normalised();
    }

    static Quat<T> zero()
    {
        return Quat<T>(0, Vec3<T>());
    }

    Quat operator*(const Quat& r) const
    {
        const T& scalarA = scalar;
        const Vec3<T>& vectorA = *reinterpret_cast<const Vec3<T>*>(&x);

        const T& scalarB = r.scalar;
        const Vec3<T>& vectorB = *reinterpret_cast<const Vec3<T>*>(&r.x);

        return Quat<T>(
            scalarA * scalarB - dot(vectorA, vectorB),
            scalarA * vectorB + scalarB * vectorA + cross(vectorA, vectorB));
    }

    void operator*=(const Quat& r)
    {
        *this = *this * r;
    }

    union
    {
        struct       // Component representation
        {
            T scalar, x, y, z;
        };

        T d[4];      // Raw data
    };
};

} // end namespace gameutils

//----------------------------------------------------------------------------
//
// Operators
//
// Defined for `float` and `double` types in math.cpp
//
//----------------------------------------------------------------------------

template<typename T>
std::ostream& operator<<(std::ostream &out, const gameutils::Vec2<T> &t);

template<typename T>
std::ostream& operator<<(std::ostream &out, const gameutils::Vec3<T> &t);

template<typename T>
std::ostream& operator<<(std::ostream &out, const gameutils::Vec4<T> &t);

template<typename T>
std::ostream& operator<<(std::ostream &out, const gameutils::Mat3<T> &m);

template<typename T>
std::ostream& operator<<(std::ostream &out, const gameutils::Mat4<T> &m);

template<typename T>
std::ostream& operator<<(std::ostream &out, const gameutils::Quat<T> &t);
