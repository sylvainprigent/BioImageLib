/// \file blMathLinearAlgebra.cpp
/// \brief blMathLinearAlgebra class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blMathLinearAlgebra.h"

#include <cmath>
#include <algorithm>

double blMathLinearAlgebra::hypot2(const double& x, const double& y)
{
    return sqrt(x*x+y*y);
}

void blMathLinearAlgebra::tred2(double V[3][3], double d[3], double e[3]) {

    /*  This is derived from the Algol procedures tred2 by */
    /*  Bowdler, Martin, Reinsch, and Wilkinson, Handbook for */
    /*  Auto. Comp., Vol.ii-Linear Algebra, and the corresponding */
    /*  Fortran subroutine in EISPACK. */
    int i, j, k;
    double scale;
    double f, g, h;
    double hh;
    int n=3;
    for (j = 0; j < n; j++)
    {
        d[j] = V[n-1][j];
    }

    /* Householder reduction to tridiagonal form. */
    for (i = n-1; i > 0; i--) {
        /* Scale to avoid under/overflow. */
        scale = 0.0;
        h = 0.0;
        for (k = 0; k < i; k++)
        {
            scale = scale + std::abs(d[k]);
        }
        if (scale == 0.0)
        {
            e[i] = d[i-1];
            for (j = 0; j < i; j++)
            {
                d[j] = V[i-1][j];
                V[i][j] = 0.0;
                V[j][i] = 0.0;
            }
        }
        else
        {
            /* Generate Householder vector. */
            for (k = 0; k < i; k++)
            {
                d[k] /= scale; h += d[k] * d[k];
            }
            f = d[i-1];
            g = sqrt(h);
            if (f > 0)
            {
                g = -g;
            }
            e[i] = scale * g;
            h = h - f * g;
            d[i-1] = f - g;
            for (j = 0; j < i; j++)
            {
                e[j] = 0.0;
            }

            /* Apply simirity transformation to remaining columns. */
            for (j = 0; j < i; j++)
            {
                f = d[j];
                V[j][i] = f;
                g = e[j] + V[j][j] * f;
                for (k = j+1; k <= i-1; k++)
                {
                    g += V[k][j] * d[k];
                    e[k] += V[k][j] * f;
                }
                e[j] = g;
            }
            f = 0.0;
            for (j = 0; j < i; j++)
            {
                e[j] /= h;
                f += e[j] * d[j];
            }
            hh = f / (h + h);
            for (j = 0; j < i; j++)
            {
                e[j] -= hh * d[j];
            }
            for (j = 0; j < i; j++)
            {
                f = d[j];
                g = e[j];
                for (k = j; k <= i-1; k++)
                {
                    V[k][j] -= (f * e[k] + g * d[k]);
                }
                d[j] = V[i-1][j];
                V[i][j] = 0.0;
            }
        }
        d[i] = h;
    }

    /* Accumute transformations. */
    for (i = 0; i < n-1; i++)
    {
        V[n-1][i] = V[i][i];
        V[i][i] = 1.0;
        h = d[i+1];
        if (h != 0.0)
        {
            for (k = 0; k <= i; k++)
            {
                d[k] = V[k][i+1] / h;
            }
            for (j = 0; j <= i; j++)
            {
                g = 0.0;
                for (k = 0; k <= i; k++)
                {
                    g += V[k][i+1] * V[k][j];
                }
                for (k = 0; k <= i; k++)
                {
                    V[k][j] -= g * d[k];
                }
            }
        }
        for (k = 0; k <= i; k++)
        {
            V[k][i+1] = 0.0;
        }
    }
    for (j = 0; j < n; j++)
    {
        d[j] = V[n-1][j]; V[n-1][j] = 0.0;
    }
    V[n-1][n-1] = 1.0;
    e[0] = 0.0;
}

void blMathLinearAlgebra::tql2(double V[3][3], double d[3], double e[3]) {

/*  This is derived from the Algol procedures tql2, by */
/*  Bowdler, Martin, Reinsch, and Wilkinson, Handbook for */
/*  Auto. Comp., Vol.ii-Linear Algebra, and the corresponding */
/*  Fortran subroutine in EISPACK. */
    int i, j, k, l, m;
    int n=3;
    double f;
    double tst1;
    double eps;
    int iter;
    double g, p, r;
    double dl1, h, c, c2, c3, el1, s, s2;

    for (i = 1; i < n; i++) { e[i-1] = e[i]; }
    e[n-1] = 0.0;

    f = 0.0;
    tst1 = 0.0;
    eps = pow(2.0, -52.0);
    for (l = 0; l < n; l++)
    {
        /* Find small subdiagonal element */
        tst1 = std::max(tst1, std::abs(d[l]) + std::abs(e[l]));
        m = l;
        while (m < n)
        {
            if (std::abs(e[m]) <= eps*tst1)
            {
                break;
            }
            m++;
        }

        /* If m == l, d[l] is an eigenvalue, */
        /* otherwise, iterate. */
        if (m > l)
        {
            iter = 0;
            do {
                iter = iter + 1;  /* (Could check iteration count here.) */
                /* Compute implicit shift */
                g = d[l];
                p = (d[l+1] - g) / (2.0 * e[l]);
                r = hypot2(p, 1.0);
                if (p < 0)
                {
                    r = -r;
                }
                d[l] = e[l] / (p + r);
                d[l+1] = e[l] * (p + r);
                dl1 = d[l+1];
                h = g - d[l];
                for (i = l+2; i < n; i++)
                {
                    d[i] -= h;
                }
                f = f + h;
                /* Implicit QL transformation. */
                p = d[m];
                c = 1.0;
                c2 = c;
                c3 = c;
                el1 = e[l+1];
                s = 0.0;
                s2 = 0.0;
                for (i = m-1; i >= l; i--)
                {
                    c3 = c2;
                    c2 = c;
                    s2 = s;
                    g = c * e[i];
                    h = c * p;
                    r = hypot2(p, e[i]);
                    e[i+1] = s * r;
                    s = e[i] / r;
                    c = p / r;
                    p = c * d[i] - s * g;
                    d[i+1] = h + s * (c * g + s * d[i]);
                    /* Accumute transformation. */
                    for (k = 0; k < n; k++)
                    {
                        h = V[k][i+1];
                        V[k][i+1] = s * V[k][i] + c * h;
                        V[k][i] = c * V[k][i] - s * h;
                    }
                }
                p = -s * s2 * c3 * el1 * e[l] / dl1;
                e[l] = s * p;
                d[l] = c * p;

                /* Check for convergence. */
            } while (std::abs(e[l]) > eps*tst1);
        }
        d[l] = d[l] + f;
        e[l] = 0.0;
    }

    /* Sort eigenvalues and corresponding vectors. */
    for (i = 0; i < n-1; i++)
    {
        k = i;
        p = d[i];
        for (j = i+1; j < n; j++)
        {
            if (d[j] < p)
            {
                k = j;
                p = d[j];
            }
        }
        if (k != i)
        {
            d[k] = d[i];
            d[i] = p;
            for (j = 0; j < n; j++)
            {
                p = V[j][i];
                V[j][i] = V[j][k];
                V[j][k] = p;
            }
        }
    }
}

void blMathLinearAlgebra::eigen_decomposition(double A[3][3], double V[3][3], double d[3])
{
    int n = 3;
    double e[3];
    double da[3];
    double dt, dat;
    double vet[3];
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            V[i][j] = A[i][j];
        }
    }
    tred2(V, d, e);
    tql2(V, d, e);

    /* Sort the eigen values and vectors by abs eigen value */
    da[0]=abs(d[0]);
    da[1]=abs(d[1]);
    da[2]=abs(d[2]);
    if((da[0]>=da[1])&&(da[0]>da[2]))
    {
        dt=d[2];   dat=da[2];    vet[0]=V[0][2];    vet[1]=V[1][2];    vet[2]=V[2][2];
        d[2]=d[0]; da[2]=da[0];  V[0][2] = V[0][0]; V[1][2] = V[1][0]; V[2][2] = V[2][0];
        d[0]=dt;   da[0]=dat;    V[0][0] = vet[0];  V[1][0] = vet[1];  V[2][0] = vet[2];
    }
    else if((da[1]>=da[0])&&(da[1]>da[2]))
    {
        dt=d[2];   dat=da[2];    vet[0]=V[0][2];    vet[1]=V[1][2];    vet[2]=V[2][2];
        d[2]=d[1]; da[2]=da[1];  V[0][2] = V[0][1]; V[1][2] = V[1][1]; V[2][2] = V[2][1];
        d[1]=dt;   da[1]=dat;    V[0][1] = vet[0];  V[1][1] = vet[1];  V[2][1] = vet[2];
    }
    if(da[0]>da[1])
    {
        dt=d[1];   dat=da[1];    vet[0]=V[0][1];    vet[1]=V[1][1];    vet[2]=V[2][1];
        d[1]=d[0]; da[1]=da[0];  V[0][1] = V[0][0]; V[1][1] = V[1][0]; V[2][1] = V[2][0];
        d[0]=dt;   da[0]=dat;    V[0][0] = vet[0];  V[1][0] = vet[1];  V[2][0] = vet[2];
    }
}