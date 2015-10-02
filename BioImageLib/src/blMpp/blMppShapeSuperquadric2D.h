/// \file blMppShapeSuperquadric2D.h
/// \brief blMppShapeSuperquadric2D class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once

#include <vector>
#include <string>

#include "blMppShape2D.h"
#include "blMppExport.h"

/// \class blMppShapeSuperquadric2D
/// \brief define a blMppShape superquadric 2D by the equation
///  |X-Xc|^m / |a|^m + |Y-Yc|^n / |b|^n = 1
class BLMPP_EXPORT blMppShapeSuperquadric2D : public blMppShape2D{

public:
    /// \fn blMppShapeSuperquadric2D();
    /// \brief reimplement from blMppShape
    blMppShapeSuperquadric2D();
    /// \fn virtual ~blMppShapeSuperquadric2D();
    /// \brief reimplement from blMppShape
    virtual ~blMppShapeSuperquadric2D();

    /// \fn std::string name();
    /// \return the name of the shape
    virtual std::string name();

public:
    /// \fn void setParameters(int a, int b, float m, float n, float theta);
    /// \param[in] a parameter a of the superquadric
    /// \param[in] b parameter b of the superquadric
    /// \param[in] m parameter m of the superquadric
    /// \param[in] n parameter n of the superquadric
    /// \param[in] theta parameter theta of the superquadric
    void setParameters(int a, int b, float m, float n, float theta);
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
    /// \fn int m();
    /// \return the parameter m
    float m();
    /// \fn int n();
    /// \return the parameter n
    float n();
    /// \fn int theta();
    /// \return the parameter theta
    float theta();

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
    virtual blMppShapeSuperquadric2D* copy();

protected:
    int m_a; int m_b;
    float m_m; float m_n;
    float m_theta;
};
