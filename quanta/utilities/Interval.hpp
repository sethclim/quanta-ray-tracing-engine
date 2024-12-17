#pragma once

#include <limits>

namespace Utilities
{
    class Interval
    {
    public:
        double min, max;

        Interval() : min(+std::numeric_limits<double>::infinity()), max(-std::numeric_limits<double>::infinity()) {} // Default interval is empty

        Interval(double min, double max) : min(min), max(max) {}

        double size() const
        {
            return max - min;
        }

        bool contains(double x) const
        {
            return min <= x && x <= max;
        }

        bool surrounds(double x) const
        {
            return min < x && x < max;
        }

        static const Interval empty, universe;
    };

    // const Interval Interval::empty = Interval(+std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity());
    // const Interval Interval::universe = Interval(-std::numeric_limits<double>::infinity(), +std::numeric_limits<double>::infinity());

}
