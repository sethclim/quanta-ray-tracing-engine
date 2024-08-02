#pragma once;

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

        Vector3() {}

        Vector3(T _x, T _y, T _z)
        {
            x = _x;
            y = _y;
            z = _z;
        }

        Vector3<T> operator+(Vector3<T> const &obj)
        {
            Vector3 res = Vector3();
            res.x = x + obj.x;
            res.y = y + obj.y;
            res.z = z + obj.z;
            return res;
        }

        Vector3 operator-(Vector3 const &obj)
        {
            Vector3 res = Vector3();
            res.x = x - obj.x;
            res.y = y - obj.y;
            res.z = z - obj.z;
            return res;
        }

        Vector3 operator*(Vector3 const &obj)
        {
            Vector3 res = Vector3();
            res.x = x * obj.x;
            res.y = y * obj.y;
            res.z = z * obj.z;
            return res;
        }

        Vector3 operator/(Vector3 const& obj)
        {
            Vector3 res = Vector3();
            res.x = x / obj.x;
            res.y = y / obj.y;
            res.z = z / obj.z;
            return res;
        }

        Vector3<T> operator*(T const &scale)
        {
            Vector3<T> res = Vector3<T>();
            res.x = x * scale;
            res.y = y * scale;
            res.z = z * scale;
            return res;
        }

        Vector3 operator/(T const &scale)
        {
            Vector3 res = Vector3();
            res.x = x / scale;
            res.y = y / scale;
            res.z = z / scale;
            return res;
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
    Vector3<T> Clamp(Vector3<T> x,T minVal, T maxVal) { return Min(Max(x, minVal), maxVal); }
}
