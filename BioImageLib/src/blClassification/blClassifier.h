/// \file blClassifier.h
/// \brief blClassifier class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2013

#pragma once

#include <blCore>
#include "itkConvolutionImageFilter.h"
#include "blClassificationExport.h"

/// \class blClassifier
/// \brief Class that define an interface for
/// an image classification
class BLCLASSIFICATION_EXPORT blClassifier : public blProcess{

public:
    /// \fn blClassifier();
    /// \brief Constructor
    blClassifier();
    /// \fn virtual ~blClassifier();
    /// \brief Destructor
    virtual ~blClassifier();

public:
    /// \fn virtual std::string name();
    /// \brief Reimplement from blProcess
    virtual std::string name();

public:
    // IO
    /// \fn void setInput(blImage* inputImage);
    /// \brief Set the input
    /// \param[in] inputImage Image to process
    void setInput(blImage* inputImage);
    /// \fn virtual void run() = 0;
    /// \brief Implement this run function to
    /// make the filtering calculation
    virtual void run() = 0;
    /// \fn blImage* output();
    /// \return filtered image
    blImage* output();

public:
    blImage* m_inputImage; ///< input image
    blImage* m_outputImage; ///< output image
};
