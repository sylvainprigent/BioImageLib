/// \file blMppShapeSuperquadric3D.h
/// \brief blMppShapeSuperquadric3D class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once

#include <vector>
#include <string>

#include "blMppShape3D.h"

/// \class blMppShapeSuperquadric3D
/// \brief define a blMppShape superquadric 3D by the equation
///  |X-Xc|^m / |a|^m + |Y-Yc|^n / |b|^n + |Z-Zc|^p / |c|^p = 1
///  The orientation of the superquadric is represented by 3 angles
///  (phi, theta, gamma)
class blMppShapeSuperquadric3D : public blMppShape3D{

public:
    /// \fn blMppShapeSuperquadric3D();
    /// \brief reimplement from blMppShape
    blMppShapeSuperquadric3D();
    /// \fn virtual ~blMppShapeSuperquadric3D();
    /// \brief reimplement from blMppShape
    virtual ~blMppShapeSuperquadric3D();

    /// \fn std::string name();
    /// \return the name of the shape
    virtual std::string name();

public:
    /// \fn void setParameters(int a, int b, float m, float n, float theta);
    /// \param[in] a parameter a of the superquadric
    /// \param[in] b parameter b of the superquadric
    /// \param[in] c parameter c of the superquadric
    /// \param[in] m parameter m of the superquadric
    /// \param[in] n parameter n of the superquadric
    /// \param[in] p parameter p of the superquadric
    /// \param[in] phi orientation of the superquadric along X
    /// \param[in] theta orientation of the superquadric along Y
    /// \param[in] gamma orientation of the superquadric along Z
    void setParameters(int a, int b, int c, float m, float n, float p, float phi, float theta, float gamma);
    /// \fn virtual std::vector<float> getParameters();
    /// \brief reimplement from blMppShape
    virtual std::vector<float> getParameters();
    /// \fn virtual std::vector<std::string>& parametersDescription()
    /// \brief reimplement from blMppShape
    virtual std::vector<std::string> parametersDescription();
    /// \fn int a();
    /// \return the parameter a
    int a();
    /// \fn int b();
    /// \return the parameter b
    int b();
    /// \fn int c();
    /// \return the parameter c
    int c();
    /// \fn int m();
    /// \return the parameter m
    float m();
    /// \fn int n();
    /// \return the parameter n
    float n();
    /// \fn int p();
    /// \return the parameter p
    float p();
    /// \fn int phi();
    /// \return the parameter phi (angle along x)
    float phi();
    /// \fn int theta();
    /// \return the parameter theta (angle along y)
    float theta();
    /// \fn int gamma();
    /// \return the parameter gamma (angle along z)
    float gamma();

    void printParam();

public:
    // silhouette
    /// \fn virtual void computeSilhouette();
    /// \brief reimplement from blMppShape
    virtual void computeSilhouette();
    /// \fn virtual void computeBorderPixels(int borderWidth);
    /// \brief reimplement from blMppShape
    //virtual void computeBorderPixels(int borderWidth);
    /// \fn virtual void computeNormalsBorder();
    /// \brief reimplement from blMppShape
    virtual void computeNormalsBorder();
    /// \fn virtual void computeMaxDistanceToBarycenter();
    /// \brief reimplement from blMppShape
    virtual void computeMaxDistanceToBarycenter();
    /// \fn virtual blMppShape* copy();
    /// \brief reimplement from blMppShape
    virtual blMppShape3D* copy();

protected:
    /// \brief calculate the rotation matrix given the 3
    /// angles (phi, theta, gamma)
    void getXYZRotationcoefs(float &a, float &b, float &c,
                             float &d, float &e, float &f,
                             float &g, float &h, float &i);

protected:
    int m_a; int m_b; int m_c;
    float m_m; float m_n; float m_p;
    float m_phi, m_theta, m_gam;
};
