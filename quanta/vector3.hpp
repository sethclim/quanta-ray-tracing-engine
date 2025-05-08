#pragma once

#include <xmmintrin.h> // SSE
#include <string>
#include <pmmintrin.h>

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

    namespace detail
    {
        struct alignas(16) Vector3Simd
        {
            union
            {
                __m128 data;
                struct
                {
                    float x, y, z, w;
                }; // `w` is padding
            };

            Vector3Simd() : data(_mm_setzero_ps()) {}

            Vector3Simd(float x, float y, float z)
                : data(_mm_set_ps(0.0f, z, y, x)) {}

            // Loads from array or other floats
            static Vector3Simd FromArray(const float *ptr)
            {
                return Vector3Simd{_mm_loadu_ps(ptr)};
            }

        private:
            explicit Vector3Simd(__m128 v) : data(v) {}

        public:
            // Operator overloading
            Vector3Simd operator+(const Vector3Simd &other) const
            {
                return Vector3Simd(_mm_add_ps(data, other.data));
            }

            void operator+=(const Vector3Simd &other)
            {
                this->data = _mm_add_ps(this->data, other.data);
            }

            Vector3Simd operator-(const Vector3Simd &other) const
            {
                return Vector3Simd(_mm_sub_ps(data, other.data));
            }

            void operator-=(const Vector3Simd &other)
            {
                this->data = _mm_sub_ps(this->data, other.data);
            }

            Vector3Simd operator*(float scalar) const
            {
                return Vector3Simd(_mm_mul_ps(data, _mm_set1_ps(scalar)));
            }

            Vector3Simd operator*(const Vector3Simd &other) const
            {
                return Vector3Simd(_mm_mul_ps(this->data, other.data));
            }

            void operator*=(const Vector3Simd &obj)
            {
                this->data = _mm_mul_ps(this->data, obj.data);
            }

            Vector3Simd operator-(float scalar) const
            {
                __m128 scalarVec = _mm_set1_ps(scalar); // broadcast scalar to all components
                return Vector3Simd(_mm_sub_ps(this->data, scalarVec));
            }

            Vector3Simd operator/(float scalar) const
            {
                __m128 scalarVec = _mm_set1_ps(scalar);                // Broadcast scalar
                return Vector3Simd(_mm_div_ps(this->data, scalarVec)); // SIMD divide
            }

            float Dot(const Vector3Simd &other) const
            {
                __m128 mul = _mm_mul_ps(data, other.data);
                // Horizontal add: x*y + y*y + z*z
                __m128 shuf1 = _mm_movehdup_ps(mul);      // (y, y, z, z)
                __m128 sum1 = _mm_add_ps(mul, shuf1);     // (x+y, y+z, ..., ...)
                __m128 shuf2 = _mm_movehl_ps(sum1, sum1); // (y+z, ..., ...)
                __m128 sum2 = _mm_add_ss(sum1, shuf2);
                return _mm_cvtss_f32(sum2);
            }

            Vector3Simd Normalize() const
            {
                float len_sq = Dot(*this);
                float inv_len = 1.0f / std::sqrt(len_sq);
                return *this * inv_len;
            }

            float LengthSquared() const
            {
                __m128 mul = _mm_mul_ps(data, data); // x*x, y*y, z*z, w*w (w is padding)
                // Shuffle and add: x� + y� + z�
                __m128 shuf1 = _mm_movehdup_ps(mul);      // (y�, y�, z�, z�)
                __m128 sum1 = _mm_add_ps(mul, shuf1);     // (x�+y�, y�+z�, ..., ...)
                __m128 shuf2 = _mm_movehl_ps(sum1, sum1); // (y�+z�, ..., ...)
                __m128 sum2 = _mm_add_ss(sum1, shuf2);    // (x�+y�+z�, ...)
                return _mm_cvtss_f32(sum2);               // Extract float
            }

            bool near_zero()
            {
                return Dot(*this) < 1e-16f;
            }

            bool operator==(const Vector3Simd &other) const
            {
                __m128 cmp = _mm_cmpeq_ps(this->data, other.data); // compare all elements
                // Move the comparison results into a mask
                int mask = _mm_movemask_ps(cmp);
                // Check if the first three bits (x, y, z) are all 1 (equal)
                return (mask & 0b0111) == 0b0111;
            }
        };
    }

    template <typename T>
    class Vector3Scalar
    {
    public:
        T x, y, z;

        constexpr Vector3Scalar() : x{}, y{}, z{} {}

        constexpr Vector3Scalar(T x, T y, T z) : x(x), y(y), z(z) {}

        Vector3Scalar<T> operator+(Vector3Scalar<T> const &other) const
        {
            return Vector3Scalar<T>{x + other.x, y + other.y, z + other.z};
        }

        void operator+=(Vector3Scalar<T> const &obj)
        {
            x = x + obj.x;
            y = y + obj.y;
            z = z + obj.z;
        }

        constexpr Vector3Scalar<T> operator-(Vector3Scalar const &other) const
        {
            return Vector3Scalar<T>{x - other.x, y - other.y, z - other.z};
        }

        constexpr Vector3Scalar<T> operator*(Vector3Scalar const &other) const
        {
            return Vector3Scalar<T>{x * other.x, y * other.y, z * other.z};
        }

        void operator*=(Vector3Scalar const &obj)
        {
            x = x * obj.x;
            y = y * obj.y;
            z = z * obj.z;
        }

        constexpr Vector3Scalar operator/(Vector3Scalar const &other) const
        {
            return Vector3Scalar<T>{x / other.x, y / other.y, z / other.z};
        }

        constexpr Vector3Scalar<T> operator*(T const &scale) const
        {
            return Vector3Scalar<T>{x * scale, y * scale, z * scale};
        }

        constexpr Vector3Scalar operator/(T const &scale) const
        {
            return Vector3Scalar<T>{x / scale, y / scale, z / scale};
        }

        constexpr Vector3Scalar operator+(T const &scale) const
        {
            return Vector3Scalar<T>{x + scale, y + scale, z + scale};
        }

        constexpr Vector3Scalar operator-(T const &scale) const
        {
            return Vector3Scalar<T>{x - scale, y - scale, z - scale};
        }

        bool operator==(Vector3Scalar<T> const &obj) const
        {
            return x == obj.x && y == obj.y && z == obj.z;
        }

        std::string ToString()
        {
            std::ostringstream os;
            os << "{x: " << x << " y: " << y << " z: " << z << " }\n";
            return os.str();
        }

        constexpr Vector3Scalar<T> Normalize() const { return *this / Length(*this); }

        constexpr T LengthSquared() const { return Sqr(x) + Sqr(y) + Sqr(z); }

        constexpr Vector3Scalar<T> Dot(const Vector3Scalar<T> &a, const Vector3Scalar<T> &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

        bool near_zero()
        {
            // Return true if the vector is close to zero in all dimensions.
            auto s = 1e-8;
            return (std::fabs(x) < s) && (std::fabs(y) < s) && (std::fabs(z) < s);
        }
    };

    template <typename T>
    struct Vector3Impl
    {
        using type = Vector3Scalar<T>; // fallback
    };

    template <>
    struct Vector3Impl<float>
    {
        using type = detail::Vector3Simd; // optimized SIMD
    };

    template <typename T>
    using Vector3 = typename Vector3Impl<T>::type;

    template <typename T>
    using ScalarType = typename TupleLength<T>::type;

    template <typename T>
    constexpr ScalarType<T> Length(const Vector3<T> &v)
    {
        using std::sqrt;
        return sqrt(LengthSquared(v));
    }

    // template <typename T>
    // constexpr Vector3<T> Normalize(const Vector3<T> &v) { return v / Length(v); }

    // template <typename T>
    // constexpr T LengthSquared(const Vector3<T> &v) { return Sqr(v.x) + Sqr(v.y) + Sqr(v.z); }

    template <typename T>
    constexpr T Sqr(T v) { return v * v; }

    // template <typename T>
    // constexpr ScalarType<T> Dot(const Vector3<T> &a, const Vector3<T> &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

    template <typename T>
    constexpr Vector3<T> Min(const Vector3<T> &a, T minVal) { return Vector3(std::min(a.x, minVal), std::min(a.y, minVal), std::min(a.z, minVal)); }

    template <typename T>
    constexpr Vector3<T> Max(const Vector3<T> &a, T maxVal) { return Vector3(std::max(a.x, maxVal), std::max(a.y, maxVal), std::max(a.z, maxVal)); }

    template <typename T>
    constexpr Vector3<T> Clamp(const Vector3<T> &x, T minVal, T maxVal) { return Min(Max(x, minVal), maxVal); }

    // template <typename T>
    // bool near_zero(const Vector3Scalar<T> &v)
    //{
    //     // Return true if the vector is close to zero in all dimensions.
    //     auto s = 1e-8;
    //     return (std::fabs(v.x) < s) && (std::fabs(v.y) < s) && (std::fabs(v.z) < s);
    // }

    /*bool near_zero(const detail::Vector3Simd& v) {
        return v.Dot(v) < 1e-16f;
    }*/
}
