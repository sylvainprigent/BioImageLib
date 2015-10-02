/// \file blMppShapeRectangle.h
/// \brief blMppShapeRectangle class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once

#include <vector>
#include <string>

#include "blMppShape2D.h"
#include "blMppExport.h"

/// \class blMppShapeRectangle
/// \brief define a blMppShape rectangle 2D by the equation
///  |X-Xc|^10 / |a|^10 + |Y-Yc|^10 / |b|^10 = 1
class BLMPP_EXPORT blMppShapeRectangle : public blMppShape2D{

public:
    /// \fn blMppShapeRectangle();
    /// \brief reimplement from blMppShape
    blMppShapeRectangle();
    /// \fn virtual ~blMppShapeSuperquadric2D();
    /// \brief reimplement from blMppShape
    virtual ~blMppShapeRectangle();

    /// \fn std::string name();
    /// \return the name of the shape
    virtual std::string name();

public:
    /// \fn void setParameters(int a, int b, float theta);
    /// \param[in] a parameter a of the superquadric
    /// \param[in] b parameter b of the superquadric
    /// \param[in] theta parameter theta of the superquadric
    void setParameters(int a, int b, float theta);
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
    /// \fn int theta();
    /// \return the parameter theta
    float theta();

public:
    // silhouette
    /// \fn virtual void computeSilhouette();
    /// \brief reimplement from blMppShape
    virtual void computeSilhouette();
    /// \fn virtual void computeNormalsBorder();
    /// \brief reimplement from blMppShape
    virtual void computeNormalsBorder();
    /// \fn virtual void computeMaxDistanceToBarycenter();
    /// \brief reimplement from blMppShape
    virtual void computeMaxDistanceToBarycenter();
    /// \fn virtual blMppShape* copy();
    /// \brief reimplement from blMppShape
    virtual blMppShape2D* copy();

protected:
    int m_a; int m_b;
    float m_theta;
    float m_m, m_n;
};
