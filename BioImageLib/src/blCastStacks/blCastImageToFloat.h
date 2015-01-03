/// \file blCastImageToFloat.h
/// \brief blCastImageToFloat class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include <blCore>
#include "blCastImage.h"

/// \class blCastImageToFloat
/// \brief Cast en image to float
class blCastImageToFloat : public blCastImage{

public:
    /// \fn blCastImageToFloat();
    /// \brief Constructor
    blCastImageToFloat();
    /// \fn virtual ~blCastImageToFloat();
    /// \brief Destructor
    virtual ~blCastImageToFloat();

public:
    /// \fn virtual std::string name();
    /// \brief Reimplemented from blProcess
    virtual std::string name();

public:
    /// \fn virtual void run();
    /// \brief Implement from blCastImage
    virtual void run();

private:
    /// \fn blImage* castOnOneComponent(blImage* input, unsigned int index);
    /// \brief run the cast filter in one component of rgb image
    /// \return the casted component
    /// \param[in] input Image to cast
    /// \param[in] index Index of the layer to cast
    blImage* castOnOneComponent(blImage* input, unsigned int index);
};
