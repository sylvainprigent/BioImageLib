/// \file blBinomialBlurFilter.h
/// \brief blBinomialBlurFilter class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include "blFilter.h"
#include "itkBinomialBlurImageFilter.h"

/// \class blBinomialBlurFilter
/// \brief The binomial blur consists of a nearest neighbor
/// average along each image dimension. The net result after
/// n-iterations approaches convultion with a gaussian.
class blBinomialBlurFilter : public blFilter{

public:
    /// \fn blBinomialBlurFilter();
    /// \brief Constructor
    blBinomialBlurFilter();
    /// \fn virtual ~blBinomialBlurFilter();
    /// \brief Destructor
    virtual ~blBinomialBlurFilter();

public:
    /// \fn virtual std::string name();
    /// \brief Reimplement from blProcess
    virtual std::string name();

public:
    // IO
    /// \fn void run();
    /// \brief Implement from blFilter
    void run();

    // parameters
    /// \fn void setRepetitions(int repetitions);
    /// \brief Set the repetition of the filter
    /// \param[in] repetitions Number of repetition of the filtering
    void setRepetitions(int repetitions);

private:
    int m_repetitions;

    blImage* binomialBlurOnOneComponent(blImage* input, unsigned int index);

    template<typename InputImageType>
    blImage* binomialBlur(typename InputImageType::Pointer image){
        typedef itk::BinomialBlurImageFilter<
          InputImageType, InputImageType >  FilterType;

        typename FilterType::Pointer filter = FilterType::New();
        filter->SetInput(image);
        filter->SetRepetitions(m_repetitions);
        filter->Update();
        return new blImage(filter->GetOutput());
    }
};
