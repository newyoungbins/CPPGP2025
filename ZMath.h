// ZMath.h
// ���� ��ƿ��Ƽ �Լ� �� ��� ����
// Mathematical utility functions and constants

#pragma once
#include <math.h>

// ������ ��� (float ���е�)
// PI constant (float precision)
constexpr float PI = 3.14159265f;

// ������ ��� (double ���е�)
// PI constant (double precision)
constexpr double PI_D = 3.1415926535897932;

// ���� �Լ�
// Square function
// @param x: ������ �� (value to square)
// @return: x�� ���� (x squared)
template <typename T>
constexpr auto sqroot(const T& x)
{
    return x * x;
}

// ������ [-PI, PI] ������ ����ȭ
// Normalize angle to [-PI, PI] range
// @param theta: ����ȭ�� ���� (����) (angle to normalize in radians)
// @return: [-PI, PI] ������ ����ȭ�� ���� (normalized angle in [-PI, PI] range)
template<typename T>
T wrap_angle(T theta)
{
    const T modded = fmod(theta, (T)2.0 * (T)PI_D);
    return (modded > (T)PI_D) ?
        (modded - (T)2.0 * (T)PI_D) :
        modded;
}

// ���� ���� �Լ� (Linear Interpolation, LERP)
// Linear interpolation function (LERP)
// @param src: ���� �� (source value)
// @param dst: ��ǥ �� (destination value)
// @param alpha: ���� ��� [0.0, 1.0] (interpolation factor [0.0, 1.0])
// @return: src�� dst ������ ������ �� (interpolated value between src and dst)
template<typename T>
constexpr T lerp(const T& src, const T& dst, float alpha)
{
    return src + (dst - src) * alpha;
}

// ������ ��(degree)���� ����(radian)���� ��ȯ
// Convert angle from degrees to radians
// @param deg: �� ���� ���� (angle in degrees)
// @return: ���� ���� ���� (angle in radians)
template<typename T>
constexpr T to_rad(T deg)
{
    return deg * PI / (T)180.0;
}