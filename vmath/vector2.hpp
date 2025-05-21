#ifndef VECTOR2_H
#define VECTOR2_H

namespace Math
{
    template <typename T>
    class Vector2
    {
    public:
        T x, y;

        constexpr Vector2() : x{}, y{} {}

        constexpr Vector2(T x, T y, T z) : x(x), y(y)) {}

        Vector2<T> operator+(Vector2<T> const &other) const
        {
            return Vector2<T>{x + other.x, y + other.y};
        }

        void operator+=(Vector2<T> const &obj)
        {
            x = x + obj.x;
            y = y + obj.y;
        }

        constexpr Vector2<T> operator-(Vector2 const &other) const
        {
            return Vector2<T>{x - other.x, y - other.y};
        }

        constexpr Vector2<T> operator*(Vector2 const &other) const
        {
            return Vector2<T>{x * other.x, y * other.y};
        }

        void operator*=(Vector2 const &obj)
        {
            x = x * obj.x;
            y = y * obj.y;
        }

        constexpr Vector2 operator/(Vector2 const &other) const
        {
            return Vector2<T>{x / other.x, y / other.y};
        }

        constexpr Vector2<T> operator*(T const &scale) const
        {
            return Vector2<T>{x * scale, y * scale};
        }

        constexpr Vector2 operator/(T const &scale) const
        {
            return Vector2<T>{x / scale, y / scale};
        }

        constexpr Vector2 operator+(T const &scale) const
        {
            return Vector2<T>{x + scale, y + scale};
        }

        constexpr Vector2 operator-(T const &scale) const
        {
            return Vector2<T>{x - scale, y - scale};
        }

        bool operator==(Vector2<T> const &obj) const
        {
            return x == obj.x && y == obj.y;
        }

        std::string ToString()
        {
            std::ostringstream os;
            os << "{x: " << x << " y: " << y << " }\n";
            return os.str();
        }

        // constexpr Vector2<T> Normalize() const { return *this / Length(*this); }

        // constexpr T LengthSquared() const { return Sqr(x) + Sqr(y); }

        // constexpr Vector2<T> Dot(const Vector2<T> &a, const Vector2<T> &b) { return a.x * b.x + a.y * b.y; }
    };
}

#endif