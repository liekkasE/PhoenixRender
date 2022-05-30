#pragma once
#include "stdlib.h"
#include <algorithm>

namespace FX
{
    namespace Math
    {
        static float RandF()
        {
            return (float)(rand()) / (float)RAND_MAX;
        }

        // Returns random float in [a, b).
        static float RandF(float a, float b)
        {
            return a + RandF() * (b - a);
        }

        template<typename T>
        static T Min(const T& a, const T& b)
        {
            return a < b ? a : b;
        }

        template<typename T>
        static T Max(const T& a, const T& b)
        {
            return a > b ? a : b;
        }

        template<typename T>
        static T Lerp(const T& a, const T& b, float t)
        {
            return a + (b - a) * t;
        }

        template<typename T>
        static T Clamp(const T& x, const T& low, const T& high)
        {
            return x < low ? low : (x > high ? high : x);
        }


    }





}

//#endif

