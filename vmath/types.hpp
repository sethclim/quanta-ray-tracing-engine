#ifndef TYPES_H
#define TYPES_H

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
}

#endif