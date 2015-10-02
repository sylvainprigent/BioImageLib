/// \file blMppInteractionAttraction.h
/// \brief blMppInteractionAttraction class
/// \author Yellamraju Tarun (yellamraju.tarun@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once

#include "blMppInteraction.h"
#include "blMppExport.h"

/// \class blMppInteractionAttraction
/// \brief Class that define the interaction between a pair of
/// ellipse based on attraction between the shapes
class BLMPP_EXPORT blMppInteractionAttraction : public blMppInteraction{

public:
    /// \fn blMppInteractionAttraction();
    /// \brief reimplemented from blMppInteraction
    blMppInteractionAttraction();

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
