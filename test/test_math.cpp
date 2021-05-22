#include "gameutils/math.h"

#include "gtest/gtest.h"

using gameutils::Vec2;
using gameutils::Vec3;
using gameutils::Mat3;
using gameutils::Mat4;
using gameutils::Quat;

class MathTest : public testing::Test
{

};

//----------------------------------------------------------------------------
//
// Vec2
//
//----------------------------------------------------------------------------

TEST_F(MathTest, Vec2_arithmetic)
{
    const Vec2<double> v1(1.0, 1.0);

    Vec2<double> v2(1.0, 2.0);

    v2 += v1;
    EXPECT_EQ(2.0, v2.x);
    EXPECT_EQ(3.0, v2.y);

    v2 = v2 + v1;
    EXPECT_EQ(3.0, v2.x);
    EXPECT_EQ(4.0, v2.y);

    v2 -= v1;
    EXPECT_EQ(2.0, v2.x);
    EXPECT_EQ(3.0, v2.y);

    v2 = v2 - v1;
    EXPECT_EQ(1.0, v2.x);
    EXPECT_EQ(2.0, v2.y);

    v2 *= 2.0;
    EXPECT_EQ(2.0, v2.x);
    EXPECT_EQ(4.0, v2.y);

    v2 = v2 * 2.0;
    EXPECT_EQ(4.0, v2.x);
    EXPECT_EQ(8.0, v2.y);

    v2 = 2.0 * v2;
    EXPECT_EQ(8.0, v2.x);
    EXPECT_EQ(16.0, v2.y);

    v2 /= 2.0;
    EXPECT_EQ(4.0, v2.x);
    EXPECT_EQ(8.0, v2.y);

    v2 = v2 / 2.0;
    EXPECT_EQ(2.0, v2.x);
    EXPECT_EQ(4.0, v2.y);
}

TEST_F(MathTest, Vec2_assignment)
{
    const Vec2<double> v1(3.0, 4.0);

    Vec2<double> v2(10.0, 5.0);
    v2 = v1;
    EXPECT_EQ(3.0, v2.x);
    EXPECT_EQ(4.0, v2.y);
}

TEST_F(MathTest, Vec2_comparison)
{
    const Vec2<double> v0(3.0, 4.0);
    const Vec2<double> v1(3.0, 4.0);
    const Vec2<double> v2(3.0, 5.0);
    const Vec2<double> v3(4.0, 5.0);

    EXPECT_TRUE(v0.equalTo(v0, 5));
    EXPECT_TRUE(v0.equalTo(v1, 5));
    EXPECT_FALSE(v0.equalTo(v2, 5));
    EXPECT_FALSE(v0.equalTo(v3, 5));

    EXPECT_TRUE(v1.equalTo(v0, 5));
    EXPECT_TRUE(v1.equalTo(v1, 5));
    EXPECT_FALSE(v1.equalTo(v2, 5));
    EXPECT_FALSE(v1.equalTo(v3, 5));

    EXPECT_FALSE(v2.equalTo(v0, 5));
    EXPECT_FALSE(v2.equalTo(v1, 5));
    EXPECT_TRUE(v2.equalTo(v2, 5));
    EXPECT_FALSE(v2.equalTo(v3, 5));

    EXPECT_FALSE(v3.equalTo(v0, 5));
    EXPECT_FALSE(v3.equalTo(v1, 5));
    EXPECT_FALSE(v3.equalTo(v2, 5));
    EXPECT_TRUE(v3.equalTo(v3, 5));
}

TEST_F(MathTest, Vec2_construction)
{
    const Vec2<double> v1(3.0, 4.0);
    EXPECT_EQ(3.0, v1.x);
    EXPECT_EQ(4.0, v1.y);

    const Vec2<double> v2;
    EXPECT_EQ(0.0, v2.x);
    EXPECT_EQ(0.0, v2.y);
}

TEST_F(MathTest, Vec2_dot)
{
    const Vec2<double> v1(3.0, 4.0);
    const Vec2<double> v2(2.0, 5.0);

    double dotProduct = v1.dot(v2);

    EXPECT_EQ(26.0, dotProduct);
}

TEST_F(MathTest, Vec2_length)
{
    const Vec2<double> v1(3.0, 4.0);
    const double epsilon = 0.00001;
    const double expectedLength = sqrt(3.0 * 3.0 + 4.0 * 4.0);
    const double actualLength = v1.length();

    EXPECT_TRUE(abs(expectedLength - actualLength) < epsilon);
}

TEST_F(MathTest, Vec2_perp)
{
    const Vec2<double> v1(3.0, 4.0);
    const Vec2<double> v2 = v1.perp();

    EXPECT_EQ(-v1.y, v2.x);
    EXPECT_EQ( v1.x, v2.y);
}

//----------------------------------------------------------------------------
//
// Mat3
//
//----------------------------------------------------------------------------

TEST_F(MathTest, Mat3_inverse)
{
    // Ensure that multiplying a vector by a matrix followed by the matrix
    // inverse results in the original vector.
    Vec3<float> v(1, 2, 3);
    Mat3<float> m(
        2.0,  4.0, 9.0,
        3.0, -1.0, 1.0,
        0.0, 10.0, 1.0);
    Vec3<float> actual = m.inverse() * m * v;
    EXPECT_TRUE(actual.equalTo(v, 5));
}

TEST_F(MathTest, Mat3_invert)
{
    // Ensure that multiplying a vector by a matrix followed by the matrix
    // inverse results in the original vector, when the in-place invert()
    // method is used.
    Vec3<float> v(1, 2, 3);
    Mat3<float> m(
        1.0,  3.0, 3.0,
        3.0, -1.0, 1.3,
        0.0, 10.0, 1.0);
    Vec3<float> actual = m * v;  // Transform vector
    m.invert();
    actual = m * actual;  // Reverse transformation
    EXPECT_TRUE(actual.equalTo(v, 5));
}

//----------------------------------------------------------------------------
//
// Quat
//
//----------------------------------------------------------------------------

TEST_F(MathTest, Quat_multiplication)
{
    // Ensure that quaternion multiplication produces the correct result for
    // an example pulled from a Wolfram Alpha query:
    //   quaternion -Sin[Pi]+3i+4j+3k multiplied by -1j+3.9i+4-3k
    Quat<float> a(-std::sin(M_PI), 3, 4, 3);
    Quat<float> b(4, 3.9, -1, -3);
    Quat<float> actual = a * b;
    Quat<float> expected(1.3, 3, 36.7, -6.6);
    EXPECT_TRUE(actual.equalTo(expected, 5));

    // Ensure that the same result is produced using in-place multiplication
    actual = a;
    actual *= b;
    EXPECT_TRUE(actual.equalTo(expected, 5));

    // Ensure that quaternion multiplication produces the correct result when
    // multiplying a quaternion by itself.
    Quat<float> c = Quat<float>::rotation(M_PI / 4, 1, 0, 0);
    actual = c * c;
    expected = Quat<float>::rotation(M_PI / 2, 1, 0, 0);
    EXPECT_TRUE(actual.equalTo(expected, 5));

    // Ensure that the same result is produced using in-place multiplication
    actual = c;
    actual *= c;
    EXPECT_TRUE(actual.equalTo(expected, 5));
}

TEST_F(MathTest, Quat_rotation)
{
    Quat<float> a = Quat<float>::rotation(M_PI / 2, 1, 0, 0);

    // Ensure that the inverse of this quaternion is equal to its transpose,
    // since rotation should produce an orthogonal matrix.
    Mat4<float> b = Mat4<float>(a.makeMat3().inverse());
    Mat4<float> c = a.makeMat4().transpose();
    EXPECT_TRUE(b.equalTo(c, 5));
}
