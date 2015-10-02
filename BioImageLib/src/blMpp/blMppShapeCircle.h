/// \file blMppShapeCircle.h
/// \brief blMppShapeCircle class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once

#include <vector>
#include <string>

#include "blMppShape2D.h"
#include "blMppExport.h"

/// \class blMppShapeCircle
/// \brief define a blMppShape circle
class BLMPP_EXPORT blMppShapeCircle : public blMppShape2D{

public:
    /// \fn blMppShapeCircle();
    /// \brief reimplement from blMppShape
    blMppShapeCircle();
    /// \fn virtual ~blMppShapeCircle();
    /// \brief reimplement from blMppShape
    virtual ~blMppShapeCircle();

    /// \fn std::string name();
    /// \return the name of the shape
    virtual std::string name();

public:
    /// \fn void setParameters(int radius);
    /// \param[in] r Radius of the circle
    void setParameters(int radius);
    /// \fn virtual std::vector<float> getParameters();
    /// \brief reimplement from blMppShape
    virtual std::vector<float> getParameters();
    /// \fn virtual std::vector<std::string>& parametersDescription()
    /// \brief reimplement from blMppShape
    virtual std::vector<std::string> parametersDescription();
    /// \fn int radius();
    /// \return the radius if the circle
    int radius();


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
    virtual blMppShape2D* copy();

protected:
    float m_radius;
};
