/// \file blMppInteractionCircle.h
/// \brief blMppInteractionCircle class
/// \author Emmanuel Soubies (Emmanuel.Soubies@inria.fr)
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once

#include "blMppInteraction.h"
#include "blMppExport.h"

/// \class blMppInteractionCircle
/// \brief Class that define the interaction between a pair of
/// superquadric circle that does not overlap and are close on to each other
class BLMPP_EXPORT blMppInteractionCircle : public blMppInteraction{

public:
    /// \fn blMppInteractionCircle();
    /// \brief reimplemented from blMppInteraction
    blMppInteractionCircle();

public:
    // blProcess methods
    /// \fn virtual std::string name();
    /// \brief /// \brief reimplemented from blMppInteraction
    virtual std::string name();

public:
    // setters parameters
    /// \fn void setOverlapCoef(float value);
    /// \param[in] value Purcentage of allowed overlaping between shapes
    void setOverlapCoef(float value);

public:
    /// \fn virtual bool isCompatible(std::string generatorShapeName);
    /// \brief reimplemented from blMppInteraction
    virtual bool isCompatible(std::string generatorShapeName);
    /// \fn virtual float compute(blMppShape* shape1, blMppShape* shape2);
    /// \brief reimplemented from blMppInteraction
    virtual float compute(blMppShape* shape1, blMppShape* shape2);

protected:
    // parameters
    float m_overlapCoef;
};
