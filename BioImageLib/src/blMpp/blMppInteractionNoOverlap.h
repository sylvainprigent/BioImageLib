/// \file blMppInteractionNoOverlap.h
/// \brief blMppInteractionNoOverlap class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once

#include "blMppInteraction.h"

/// \class blMppInteractionNoOverlap
/// \brief Class that define the interaction between a pair of
/// shape as non overlapping
class blMppInteractionNoOverlap : public blMppInteraction{

public:
    /// \fn blMppInteractionNoOverlap();
    /// \brief reimplemented from blMppInteraction
    blMppInteractionNoOverlap();

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
    /// \fn void setMinZDistance(int value);
    /// \param[in] value Minimum z distance for 2D shapes in 3D image
    void setMinZDistance(int value);

public:
    /// \fn virtual void initialize();
    /// \brief reimplemented from blMppInteraction
    virtual void initialize();
    /// \fn virtual bool isCompatible(std::string generatorShapeName);
    /// \brief reimplemented from blMppInteraction
    virtual bool isCompatible(std::string generatorShapeName);
    /// \fn virtual float compute(blMppShape* shape1, blMppShape* shape2);
    /// \brief reimplemented from blMppInteraction
    virtual float compute(blMppShape* shape1, blMppShape* shape2);

private:
    float computeCircleOverlap(blMppShape* shape1, blMppShape* shape2);
    float computeSphereOverlap(blMppShape* shape1, blMppShape* shape2);

protected:
    // parameters
    float m_overlapCoef;
    int m_minZDistance;

    // internal
    unsigned int m_generatorType;
};
