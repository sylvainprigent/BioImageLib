/// \file blMathLinearAlgebra.h
/// \brief blMathLinearAlgebra class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once
#include "blMathExport.h"

/// \class blMathLinearAlgebra
/// \brief Implement linear algebra algorithms
class BLMATH_EXPORT blMathLinearAlgebra
{
public:
    /// \fn static double hypot2(const double& x, const double& y);
    /// \brief hupot2 (cf numerical recipe)
    static double hypot2(const double& x, const double& y);

    /// \fn static void tred2(double V[3][3], double d[3], double e[3]);
    /// \brief Symmetric Householder reduction to tridiagonal form
    static void tred2(double V[3][3], double d[3], double e[3]);

    /// \fn static void tql2(double V[3][3], double d[3], double e[3]);
    /// \brief Symmetric tridiagonal QL algorithm.
    static void tql2(double V[3][3], double d[3], double e[3]);

    /// \fn static void eigen_decomposition(double A[3][3], double V[3][3], double d[3]);
    /// \brief calculate matrix eigen decomposition
    /// \param[in] A Matrix to decompose
    /// \param[out] V Eigen vectors
    /// \param[out] d Eigen values
    static void eigen_decomposition(double A[3][3], double V[3][3], double d[3]);
};
