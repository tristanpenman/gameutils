#include <iostream>

#include "gameutils/math.h"

//----------------------------------------------------------------------------
//
// Operators for floats
//
//----------------------------------------------------------------------------

template<>
std::ostream& operator<<(std::ostream &ostream, const gameutils::Vec2<float> &t)
{
    ostream << "<" << t.x << ", " << t.y << ">";

    return ostream;
}

template<>
std::ostream& operator<<(std::ostream &ostream, const gameutils::Vec3<float> &t)
{
    ostream << "<" << t.x << ", " << t.y << ", " << t.z << ">";

    return ostream;
}

template<>
std::ostream& operator<<(std::ostream &ostream, const gameutils::Vec4<float> &t)
{
    ostream << "<" << t.x << ", " << t.y << ", " << t.z << ", " << t.w << ">";

    return ostream;
}

template<>
std::ostream& operator<<(std::ostream &ostream, const gameutils::Mat3<float> &m)
{
    ostream
        << m.m00 << ", " << m.m10 << ", " << m.m20 << ", "
        << m.m01 << ", " << m.m11 << ", " << m.m21 << ", "
        << m.m02 << ", " << m.m12 << ", " << m.m22;

    return ostream;
}

template<>
std::ostream& operator<<(std::ostream &ostream, const gameutils::Mat4<float> &m)
{
    ostream
        << m.m00 << ", " << m.m10 << ", " << m.m20 << ", " << m.m30 << ", "
        << m.m01 << ", " << m.m11 << ", " << m.m21 << ", " << m.m31 << ", "
        << m.m02 << ", " << m.m12 << ", " << m.m22 << ", " << m.m32 << ", "
        << m.m03 << ", " << m.m13 << ", " << m.m23 << ", " << m.m33;

    return ostream;
}

template<>
std::ostream& operator<<(std::ostream &ostream, const gameutils::Quat<float> &t)
{
    ostream
        << t.scalar << " "
        << ((t.x >= 0) ? "+ " : "- ")
        << std::abs(t.x) << "i "
        << ((t.y >= 0) ? "+ " : "- ")
        << std::abs(t.y) << "j "
        << ((t.z >= 0) ? "+ " : "- ")
        << std::abs(t.z) << "k";

    return ostream;
}

//----------------------------------------------------------------------------
//
// Operators for doubles
//
//----------------------------------------------------------------------------

template<>
std::ostream& operator<<(std::ostream &ostream, const gameutils::Vec2<double> &t)
{
    ostream << "<" << t.x << ", " << t.y << ">";

    return ostream;
}

template<>
std::ostream& operator<<(std::ostream &ostream, const gameutils::Vec3<double> &t)
{
    ostream << "<" << t.x << ", " << t.y << ", " << t.z << ">";

    return ostream;
}

template<>
std::ostream& operator<<(std::ostream &ostream, const gameutils::Vec4<double> &t)
{
    ostream << "<" << t.x << ", " << t.y << ", " << t.z << ", " << t.w << ">";

    return ostream;
}

template<>
std::ostream& operator<<(std::ostream &ostream, const gameutils::Mat3<double> &m)
{
    ostream
        << m.m00 << ", " << m.m10 << ", " << m.m20 << ", "
        << m.m01 << ", " << m.m11 << ", " << m.m21 << ", "
        << m.m02 << ", " << m.m12 << ", " << m.m22;

    return ostream;
}

template<>
std::ostream& operator<<(std::ostream &ostream, const gameutils::Mat4<double> &m)
{
    ostream
        << m.m00 << ", " << m.m10 << ", " << m.m20 << ", " << m.m30 << ", "
        << m.m01 << ", " << m.m11 << ", " << m.m21 << ", " << m.m31 << ", "
        << m.m02 << ", " << m.m12 << ", " << m.m22 << ", " << m.m32 << ", "
        << m.m03 << ", " << m.m13 << ", " << m.m23 << ", " << m.m33;

    return ostream;
}

template<>
std::ostream& operator<<(std::ostream &ostream, const gameutils::Quat<double> &t)
{
    ostream
        << t.scalar << " "
        << ((t.x >= 0) ? "+ " : "- ")
        << std::abs(t.x) << "i "
        << ((t.y >= 0) ? "+ " : "- ")
        << std::abs(t.y) << "j "
        << ((t.z >= 0) ? "+ " : "- ")
        << std::abs(t.z) << "k";

    return ostream;
}
