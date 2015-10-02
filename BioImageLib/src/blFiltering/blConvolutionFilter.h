/// \file blConvolutionFilter.h
/// \brief blConvolutionFilter class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include "blFilter.h"
#include "itkConvolutionImageFilter.h"
#include "blFilteringExport.h"

/// \class blConvolutionFilter
/// \brief Apply a convolution filter to an image
/// this filter needs a kernel as an input
/// For vector image, it applie the convolution to each layer independently
class BLFILTERING_EXPORT blConvolutionFilter : public blFilter{

public:
    /// \fn blConvolutionFilter();
    /// \brief Constructor
    blConvolutionFilter();
    /// \fn virtual ~blConvolutionFilter();
    /// \brief Destructor
    virtual ~blConvolutionFilter();

public:
    /// \fn virtual std::string name();
    /// \brief Reimplement from blProcess
    virtual std::string name();

public:
    // IO
    /// \fn void setKernel(blImage* kernel);
    /// \brief Set the convolution kernel
    /// \param[in] kernel Convolution kernel
    void setKernel(blImage* kernel);

    /// \fn void run();
    /// \brief Implement from blFilter
    void run();

    // parameters
    /// \fn void setNormalizeKernel(bool value);
    /// \brief Normalize the kernel by making it values
    /// to sum to one
    /// \param[in] value set to true to normalize the kernel
    void setNormalizeKernel(bool value);

public:
    blImage* m_kernel; ///< pointer to the kernel
    bool m_normalizeKernel; ///< true if the kernel is normalized (sum to 1)

private:
    blImage* convolutionOnOneComponent(blImage* input, unsigned int index);

    template<typename InputImageType>
    blImage* convolution(typename InputImageType::Pointer image, typename InputImageType::Pointer kernel){
        typedef itk::ConvolutionImageFilter<InputImageType> FilterType;
        typename FilterType::Pointer convolutionFilter = FilterType::New();
        convolutionFilter->SetInput(image);
        convolutionFilter->SetKernelImage(kernel);
        if (m_normalizeKernel){
            convolutionFilter->NormalizeOn();
        }
        else{
            convolutionFilter->NormalizeOff();
        }
        convolutionFilter->Update();
        return new blImage( convolutionFilter->GetOutput() );
    }
};
