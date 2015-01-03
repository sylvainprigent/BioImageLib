/// \file blMedianFilter.h
/// \brief blMedianFilter class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include "blFilter.h"
#include "itkConvolutionImageFilter.h"
#include "itkMedianImageFilter.h"

/// \class blMedianFilter
/// \brief Blur an image by applying a median filter
class blMedianFilter : public blFilter{

public:
    /// \fn blMedianFilter();
    /// \brief Constructor
    blMedianFilter();
    /// \fn virtual ~blMedianFilter();
    /// \brief Destructor
    virtual ~blMedianFilter();

public:
    /// \fn virtual std::string name();
    /// \brief Reimplement from blProcess
    virtual std::string name();

public:
    // IO
    /// \fn void setRadius(float radius);
    /// \brief Set the size of the filter*
    /// \param[in] radius Radius of the median kernel
    void setRadius(float radius);

    /// \fn void run();
    /// \brief Implement from blFilter
    void run();

public:
    float m_radius; ///< radius

private:
    blImage* medianOnOneComponent(blImage* input, unsigned int index);

    template<typename InputImageType>
    blImage* medianFilter(typename InputImageType::Pointer image){
        typedef itk::MedianImageFilter<InputImageType, InputImageType > FilterType;
        typename FilterType::Pointer medianFilter = FilterType::New();
        typename FilterType::InputSizeType radius;
        radius.Fill(m_radius);
        medianFilter->SetRadius( radius );
        medianFilter->SetInput( image );
        medianFilter->Update();
        return new blImage(medianFilter->GetOutput());
    }
};
