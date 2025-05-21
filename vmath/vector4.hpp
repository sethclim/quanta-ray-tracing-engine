#ifndef VECTOR4_H
#define VECTOR4_H

#include "types.hpp"

namespace Math
{

    template <typename T>
    class Vector4Scalar
    {
    public:
        T x, y, z, w;

        constexpr Vector4Scalar() : x{}, y{}, z{}, w{} {}

        constexpr Vector4Scalar(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

        Vector4Scalar<T> operator+(Vector4Scalar<T> const &other) const
        {
            return Vector4Scalar<T>{x + other.x, y + other.y, z + other.z, w + other.w};
        }

        void operator+=(Vector4Scalar<T> const &obj)
        {
            x = x + obj.x;
            y = y + obj.y;
            z = z + obj.z;
            w = w + obj.w;
        }

        constexpr Vector4Scalar<T> operator-(Vector4Scalar const &other) const
        {
            return Vector3Scalar<T>{x - other.x, y - other.y, z - other.z, w - other.w};
        }

        constexpr Vector4Scalar<T> operator*(Vector4Scalar const &other) const
        {
            return Vector3Scalar<T>{x * other.x, y * other.y, z * other.z, w * other.w};
        }

        void operator*=(Vector4Scalar const &obj)
        {
            x = x * obj.x;
            y = y * obj.y;
            z = z * obj.z;
            w = w * obj.w;
        }

        constexpr Vector4Scalar operator/(Vector4Scalar const &other) const
        {
            return Vector3Scalar<T>{x / other.x, y / other.y, z / other.z, w / other.w};
        }

        constexpr Vector4Scalar<T> operator*(T const &scale) const
        {
            return Vector3Scalar<T>{x * scale, y * scale, z * scale, w * scale};
        }

        constexpr Vector4Scalar operator/(T const &scale) const
        {
            return Vector3Scalar<T>{x / scale, y / scale, z / scale, w / scale};
        }

        void operator/=(Vector4Scalar const &other)
        {
            x /= other.x;
            y /= other.y;
            z /= other.z;
            w /= other.w;
        }

        void operator/=(T const &scale)
        {
            x /= scale;
            y /= scale;
            z /= scale;
            w /= scale;
        }

        constexpr Vector4Scalar operator+(T const &scale) const
        {
            return Vector3Scalar<T>{x + scale, y + scale, z + scale, w + scale};
        }

        constexpr Vector4Scalar operator-(T const &scale) const
        {
            return Vector3Scalar<T>{x - scale, y - scale, z - scale, w - scale};
        }

        bool operator==(Vector4Scalar<T> const &obj) const
        {
            return x == obj.x && y == obj.y && z == obj.z && w == obj.w;
        }

        std::string ToString()
        {
            std::ostringstream os;
            os << "{x: " << x << " y: " << y << " z: " << z << " w: " << w << " }\n";
            return os.str();
        }

        // constexpr Vector4Scalar<T> Normalize() const { return *this / Length(*this); }

        // constexpr T LengthSquared() const { return Sqr(x) + Sqr(y) + Sqr(z); }

        // constexpr Vector4Scalar<T> Dot(const Vector4Scalar<T> &a, const Vector4Scalar<T> &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

        // bool near_zero()
        // {
        //     // Return true if the vector is close to zero in all dimensions.
        //     auto s = 1e-8;
        //     return (std::fabs(x) < s) && (std::fabs(y) < s) && (std::fabs(z) < s);
        // }
    };

    template <typename T>
    struct Vector4Impl
    {
        using type = Vector4Scalar<T>; // fallback
    };

    template <typename T>
    using Vector4 = typename Vector4Impl<T>::type;


    template <typename T>
    constexpr Vector4<T> Min(const Vector4<T> &a, T minVal) { return Vector4<float>(std::min(a.x, minVal), std::min(a.y, minVal), std::min(a.z, minVal), std::min(a.w, minVal)); }

    template <typename T>
    constexpr Vector4<T> Max(const Vector4<T> &a, T maxVal) { return Vector4<float>(std::max(a.x, maxVal), std::max(a.y, maxVal), std::max(a.z, maxVal), std::max(a.w, maxVal)); }

    template <typename T>
    constexpr Vector4<T> Clamp(const Vector4<T> &x, T minVal, T maxVal) { return Min(Max(x, minVal), maxVal); }
}

#endif
