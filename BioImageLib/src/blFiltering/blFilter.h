/// \file blFilter.h
/// \brief blFilter class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include <blCore>
#include "itkConvolutionImageFilter.h"
#include "blFilteringExport.h"

/// \class blFilter
/// \brief Class that define an interface for
/// an image filter
class BLFILTERING_EXPORT blFilter : public blProcess{

public:
    /// \fn blFilter();
    /// \brief Constructor
    blFilter();
    /// \fn virtual ~blFilter();
    /// \brief Destructor
    virtual ~blFilter();

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

protected:
    blImage* m_inputImage; ///< pointer to the input image
    blImage* m_outputImage; ///< pointer to the output image
};
