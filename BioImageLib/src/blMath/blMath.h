/// \file blMath.h
/// \brief blMath class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

/// \class blMath
/// \brief Implement basic mathematical static methods
class blMath
{
public:
    /// \fn static bool isNan(const float& number);
    /// \brief test if a number is Nan
    /// \param[in] number Number to test
    /// \return true if number is Nan
    static bool isNan(const float& number);

    /// \fn float randSign();
    /// \brief Function that randomly generate -1 or 1
    static float randSign();

    /// \fn static float rand10_1();
    /// \brief randomly generate -1, 0, or 1
    static float rand10_1();

    // swap two values
    /// \fn template<typename T> static void Swap(T& a, T& b)
    /// \brief Swap the two input values
    /// \param[in] a value 1
    /// \param[in] b value 2
    template<typename T>
    static void Swap(T& a, T& b)
    {
        T temp = a;
        a = b;
        b = temp;
    }
};
