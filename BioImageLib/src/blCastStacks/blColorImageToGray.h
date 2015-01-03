/// \file blColorImageToGray.h
/// \brief blColorImageToGray class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include <blCore>
#include "itkVectorIndexSelectionCastImageFilter.h"

/// \class blColorImageToGray
/// \brief Extract a gray scale component (red, green or blue)
/// from a color image
class blColorImageToGray : public blProcess{
public:
    /// \fn blColorImageToGray();
    /// \brief Constructor
    blColorImageToGray();
    /// \fn virtual ~blColorImageToGray();
    /// \brief Destructor
    virtual ~blColorImageToGray();

public:
    static const std::string Luminance; ///< Luminance transformation
    static const std::string Red;       ///< Red transformation
    static const std::string Green;     ///< Green transformation
    static const std::string Blue;      ///< Blue transformation

public:
    /// \fn virtual std::string name();
    /// \brief Reimplement from blProcess
    virtual std::string name();

public:
    /// \fn void setInput(blImage* image);
    /// \param[in] image Input image
    void setInput(blImage* image);

    /// \fn void run();
    /// \brief Run the computation
    void run();

    /// \fn blImage* output();
    /// \return the extracted gray scale image
    blImage* output();

public:
    // parameters
    /// \fn void setTransformation(std::string transformationName);
    /// \param[in] transformationName Name of the transformation to use
    ///  see the list of transformation in the static const variables
    void setTransformation(std::string transformationName);

protected:
    blImage* m_inputImage; ///< input image
    blImage* m_outputImage; ///< output image
    std::string m_transformationName; ///< Name of the transformation to use
};
