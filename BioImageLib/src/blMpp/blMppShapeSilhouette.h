/// \file blMppShapeSilhouette.h
/// \brief blMppShapeSilhouette class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once

#include <vector>
#include <string>

#include "blMppShape2D.h"
#include "blMppExport.h"

/// \class blMppShapeSilhouette
/// \brief define a blMppShape as a any shape loaded from a file
/// and defined by a silhouette
class BLMPP_EXPORT blMppShapeSilhouette : public blMppShape2D{

public:
    /// \fn blMppShapeSilhouette();
    /// \brief reimplement from blMppShape
    blMppShapeSilhouette();
    /// \fn virtual ~blMppShapeSilhouette();
    /// \brief reimplement from blMppShape
    virtual ~blMppShapeSilhouette();

    /// \fn std::string name();
    /// \return the name of the shape
    virtual std::string name();

public:
    /// \fn void setParameters(int a, int b, float m, float n, float theta);
    /// \param[in] pos Position of the silhouette in the file containing the
    /// file library
    void setParameters(int pos);
    /// \fn virtual std::vector<float> getParameters();
    /// \brief reimplement from blMppShape
    virtual std::vector<float> getParameters();
    /// \fn virtual std::vector<std::string>& parametersDescription();
    /// \brief reimplement from blMppShape
    virtual std::vector<std::string> parametersDescription();
    /// \fn int position();
    /// \return the position of the silhouette in the file containing the
    /// file library
    int position();

public:
    // silhouette
    /// \fn virtual void computeSilhouette();
    /// \brief reimplement from blMppShape
    virtual void computeSilhouette();
    /// \fn virtual void computeMaxDistanceToBarycenter();
    /// \brief reimplement from blMppShape
    virtual void computeMaxDistanceToBarycenter();
    /// \fn virtual blMppShapeSilhouette* copy();
    /// \brief reimplement from blMppShape
    virtual blMppShape2D* copy();

protected:
    int m_position;
};
