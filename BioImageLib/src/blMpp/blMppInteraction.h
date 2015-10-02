/// \file blMppInteraction.h
/// \brief blMppInteraction class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once

#include <blCore>
#include "blMppShape.h"
#include "blMppExport.h"

/// \class blMppInteraction
/// \brief Class that define the prototype of a Marked Point Process
/// interaction between a pair of shape
class BLMPP_EXPORT blMppInteraction : public blProcess{

public:
    /// \fn blMppInteraction();
    /// \brief Constructor
    blMppInteraction();

public:
    // blProcess methods
    /// \fn virtual std::string name();
    /// \brief reimplemented from blProcess
    virtual std::string name();

public:
    //setters
    /// \fn void setGeneratorName(std::string name);
    /// \param[in] name Name of the generator. It can be use in the
    /// case where the interaction is calculated differently depending
    /// on the generator
    void setGeneratorName(std::string name);

    /// \fn void setGeneratorType(bool is3D);
    /// \param[in] is3D True if the generator generate 3D silhouettes shapes
    void setGeneratorType(bool is3D);

public:
    // virtual methods
    /// \fn virtual void initialize();
    /// \brief Initialize the internal variable depending on the
    /// generator and/or parameters
    virtual void initialize();
    /// \fn virtual bool isCompatible(std::string generatorShapeName) = 0;
    /// \return true if the interaction is compatible with the generator
    /// named generatorShapeName
    /// \param[in] generatorShapeName Generator to test the compatibility
    virtual bool isCompatible(std::string generatorShapeName) = 0;
    /// \fn virtual float compute(blMppShape* shape1, blMppShape* shape2) = 0;
    /// \brief Compute the interaction between a pair of points
    /// \param[in] shape1 First shape to evaluate
    /// \param[in] shape2 Second shape to evaluate
    virtual float compute(blMppShape* shape1, blMppShape* shape2) = 0;

protected:
    std::string m_generatorName;
    bool m_isGenerator3D;
};
