/// \file blMeanFilter.h
/// \brief blMeanFilter class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include "blFilter.h"
#include "itkConvolutionImageFilter.h"
#include "itkMeanImageFilter.h"

/// \class blMeanFilter
/// \brief Blur an image by applying a mean filter
class blMeanFilter : public blFilter{

public:
    /// \fn blMeanFilter();
    /// \brief Constructor
    blMeanFilter();
    /// \fn virtual ~blMeanFilter();
    /// \brief Destructor
    virtual ~blMeanFilter();

public:
    /// \fn virtual std::string name();
    /// \brief Reimplement from blProcess
    virtual std::string name();

public:
    // IO
    /// \fn void run();
    /// \brief Implement from blFilter
    void run();

private:
    blImage* meanOnOneComponent(blImage* input, unsigned int index);

    template<typename InputImageType>
    blImage* meanFilter(typename InputImageType::Pointer image){
        typedef itk::MeanImageFilter<InputImageType, InputImageType > FilterType;
        typename FilterType::Pointer medianFilter = FilterType::New();
        medianFilter->SetInput( image );
        medianFilter->Update();
        return new blImage(medianFilter->GetOutput());
    }
};
