#pragma once

#include <string>

namespace Math
{
    template <typename T>
    struct TupleLength
    {
        using type = float;
    };

    template <>
    struct TupleLength<double>
    {
        using type = double;
    };

    template <typename T>
    class Vector3
    {
    public:
        T x, y, z;

        constexpr Vector3() : x{}, y{}, z{} {}

        constexpr Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

        Vector3<T> operator+(Vector3<T> const &other) const
        {
            return Vector3<T>{x + other.x, y + other.y, z + other.z};
        }

        void operator+=(Vector3<T> const &obj)
        {
            x = x + obj.x;
            y = y + obj.y;
            z = z + obj.z;
        }

        constexpr Vector3<T> operator-(Vector3 const &other) const
        {
            return Vector3<T>{x - other.x, y - other.y, z - other.z};
        }

        constexpr Vector3<T> operator*(Vector3 const &other) const
        {
            return Vector3<T>{x* other.x, y* other.y, z* other.z};
        }

        void operator*=(Vector3 const &obj)
        {
            x = x * obj.x;
            y = y * obj.y;
            z = z * obj.z;
        }

        constexpr Vector3 operator/(Vector3 const &other) const
        {
            return Vector3<T>{x / other.x, y / other.y, z / other.z};
        }

        constexpr Vector3<T> operator*(T const &scale) const
        {
            return Vector3<T>{x* scale, y* scale, z* scale};
        }

        constexpr Vector3 operator/(T const &scale) const
        {
            return Vector3<T>{x / scale, y / scale, z / scale};
        }

        constexpr Vector3 operator+(T const &scale) const
        {
            return Vector3<T>{x + scale, y + scale, z + scale};
        }

        constexpr Vector3 operator-(T const &scale) const
        {
            return Vector3<T>{x - scale, y - scale, z - scale};
        }

        bool operator==(Vector3<T> const &obj) const
        {
            return x == obj.x && y == obj.y && z == obj.z;
        }

        std::string ToString()
        {
            std::ostringstream os;
            os << "{x: " << x << " y: " << y << " z: " << z << " }\n";
            return os.str();
        }
    };

    template <typename T>
    auto Length(Vector3<T> v) -> typename TupleLength<T>::type
    {
        using std::sqrt;
        return sqrt(LengthSquared(v));
    }

    template <typename T>
    Vector3<T> Normalize(Vector3<T> v) { return v / Length(v); }

    template <typename T>
    T LengthSquared(Vector3<T> v) { return Sqr(v.x) + Sqr(v.y) + Sqr(v.z); }

    template <typename T>
    constexpr T Sqr(T v) { return v * v; }

    template <typename T>
    T Dot(Vector3<T> a, Vector3<T> b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

    template <typename T>
    Vector3<T> Min(Vector3<T> a, T minVal) { return Vector3(std::min(a.x, minVal), std::min(a.y, minVal), std::min(a.z, minVal)); }

    template <typename T>
    Vector3<T> Max(Vector3<T> a, T maxVal) { return Vector3(std::max(a.x, maxVal), std::max(a.y, maxVal), std::max(a.z, maxVal)); }

    template <typename T>
    Vector3<T> Clamp(Vector3<T> x, T minVal, T maxVal) { return Min(Max(x, minVal), maxVal); }

    template <typename T>
    bool near_zero(Vector3<T> v)
    {
        // Return true if the vector is close to zero in all dimensions.
        auto s = 1e-8;
        return (std::fabs(v.x) < s) && (std::fabs(v.y) < s) && (std::fabs(v.z) < s);
    }
}
