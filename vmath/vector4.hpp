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

    namespace detail
    {
        struct alignas(16) Vector4Simd
        {
            union
            {
                __m128 data;
                struct
                {
                    float x, y, z, w;
                };
            };

            Vector4Simd() : data(_mm_setzero_ps()) {}

            Vector4Simd(float x, float y, float z, float w)
                : data(_mm_set_ps(w, z, y, x)) {}

            // Loads from array or other floats
            static Vector4Simd FromArray(const float *ptr)
            {
                return Vector4Simd{_mm_loadu_ps(ptr)};
            }

        public:
            explicit Vector4Simd(__m128 v) : data(v) {}
            // Operator overloading
            Vector4Simd operator+(const Vector4Simd &other) const
            {
                return Vector4Simd(_mm_add_ps(data, other.data));
            }

            void operator+=(const Vector4Simd &other)
            {
                this->data = _mm_add_ps(this->data, other.data);
            }

            Vector4Simd operator-(const Vector4Simd &other) const
            {
                return Vector4Simd(_mm_sub_ps(data, other.data));
            }

            void operator-=(const Vector4Simd &other)
            {
                this->data = _mm_sub_ps(this->data, other.data);
            }

            Vector4Simd operator*(float scalar) const
            {
                return Vector4Simd(_mm_mul_ps(data, _mm_set1_ps(scalar)));
            }

            Vector4Simd operator*(const Vector4Simd &other) const
            {
                return Vector4Simd(_mm_mul_ps(this->data, other.data));
            }

            void operator*=(const Vector4Simd &obj)
            {
                this->data = _mm_mul_ps(this->data, obj.data);
            }

            Vector4Simd operator-(float scalar) const
            {
                __m128 scalarVec = _mm_set1_ps(scalar); // broadcast scalar to all components
                return Vector4Simd(_mm_sub_ps(this->data, scalarVec));
            }

            Vector4Simd operator/(float scalar) const
            {
                __m128 scalarVec = _mm_set1_ps(scalar);                // Broadcast scalar
                return Vector4Simd(_mm_div_ps(this->data, scalarVec)); // SIMD divide
            }

            void operator/=(float scalar)
            {
                __m128 scalarVec = _mm_set1_ps(scalar);         // Broadcast scalar
                this->data = _mm_div_ps(this->data, scalarVec); // SIMD divide in-place
            }

            float Dot(const Vector4Simd &other) const
            {
                __m128 mul = _mm_mul_ps(data, other.data);
                // Horizontal add: x*y + y*y + z*z
                __m128 shuf1 = _mm_movehdup_ps(mul);      // (y, y, z, z)
                __m128 sum1 = _mm_add_ps(mul, shuf1);     // (x+y, y+z, ..., ...)
                __m128 shuf2 = _mm_movehl_ps(sum1, sum1); // (y+z, ..., ...)
                __m128 sum2 = _mm_add_ss(sum1, shuf2);
                return _mm_cvtss_f32(sum2);
            }

            __m128 GetData() const { return this->data; }

            // Vector4Simd Normalize() const
            // {
            //     float len_sq = Dot(*this);
            //     float inv_len = 1.0f / std::sqrt(len_sq);
            //     return *this * inv_len;
            // }

            // float LengthSquared() const
            // {
            //     __m128 mul = _mm_mul_ps(data, data); // x*x, y*y, z*z, w*w (w is padding)
            //     // Shuffle and add: x� + y� + z�
            //     __m128 shuf1 = _mm_movehdup_ps(mul);      // (y�, y�, z�, z�)
            //     __m128 sum1 = _mm_add_ps(mul, shuf1);     // (x�+y�, y�+z�, ..., ...)
            //     __m128 shuf2 = _mm_movehl_ps(sum1, sum1); // (y�+z�, ..., ...)
            //     __m128 sum2 = _mm_add_ss(sum1, shuf2);    // (x�+y�+z�, ...)
            //     return _mm_cvtss_f32(sum2);               // Extract float
            // }

            // bool near_zero()
            // {
            //     return Dot(*this) < 1e-16f;
            // }

            bool operator==(const Vector4Simd &other) const
            {
                __m128 cmp = _mm_cmpeq_ps(this->data, other.data); // compare all elements
                // Move the comparison results into a mask
                int mask = _mm_movemask_ps(cmp);
                // Check if the first three bits (x, y, z) are all 1 (equal)
                return (mask & 0b0111) == 0b0111;
            }

            std::string ToString()
            {
                std::ostringstream os;
                os << "{x: " << x << " y: " << y << " z: " << z << " w: " << w << " }\n";
                return os.str();
            }
        };
    }

    template <typename T>
    struct Vector4Impl
    {
        using type = Vector4Scalar<T>; // fallback
    };

    template <>
    struct Vector4Impl<float>
    {
        using type = detail::Vector4Simd; // optimized SIMD
    };

    template <typename T>
    using Vector4 = typename Vector4Impl<T>::type;

    template <typename T>
    constexpr Vector4<T> Min(const Vector4<T> &a, T minVal) { return Vector4<float>(std::min(a.x, minVal), std::min(a.y, minVal), std::min(a.z, minVal), std::min(a.w, minVal)); }

    template <typename T>
    constexpr Vector4<T> Max(const Vector4<T> &a, T maxVal) { return Vector4<float>(std::max(a.x, maxVal), std::max(a.y, maxVal), std::max(a.z, maxVal), std::max(a.w, maxVal)); }

    template <typename T>
    constexpr Vector4<T> Clamp(const Vector4<T> &x, T minVal, T maxVal) { return Min(Max(x, minVal), maxVal); }

    // SIMD VERSIONS
    template <typename T>
    constexpr detail::Vector4Simd min_with_scalar(const detail::Vector4Simd &v, float scalar)
    {
        return detail::Vector4Simd{_mm_min_ps(v.GetData(), _mm_set1_ps(scalar))};
    }

    template <typename T>
    constexpr detail::Vector4Simd max_with_scalar(const detail::Vector4Simd &v, float scalar)
    {
        return detail::Vector4Simd{_mm_max_ps(v.GetData(), _mm_set1_ps(scalar))};
    }

    template <typename T>
    constexpr detail::Vector4Simd Clamp(const detail::Vector4Simd &v, T minVal, T maxVal)
    {
        __m128 clamped = _mm_min_ps(_mm_max_ps(v.GetData(), _mm_set1_ps(minVal)),
                                    _mm_set1_ps(maxVal));
        return detail::Vector4Simd{clamped};
    }
}

#endif
