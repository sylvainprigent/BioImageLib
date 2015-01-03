/// \file blBilateralFilter.h
/// \brief blBilateralFilter class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include "blFilter.h"
#include "itkBilateralImageFilter.h"

/// \class blBilateralFilter
/// \brief Blurs an image while preserving edges.
/// Bilateral Filtering for Gray and ColorImages. IEEE ICCV. 1998.
class blBilateralFilter : public blFilter{

public:
    /// \fn blBilateralFilter();
    /// \brief Constructor
    blBilateralFilter();
    /// \fn virtual ~blBilateralFilter();
    /// \brief Destructor
    virtual ~blBilateralFilter();

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
    /// \fn void setDomainSigma(float domainSigma);
    /// \param[in] domainSigma Parameter sigma for domain
    void setDomainSigma(float domainSigma);
    /// \fn void setRangeSigma(float rangeSigma);
    /// \param[in] rangeSigma Parameter sigma for range
    void setRangeSigma(float rangeSigma);

private:
    float m_domainSigma;
    float m_rangeSigma;

private:
    blImage* bilateralOnOneComponent(blImage* input, unsigned int index);

    template<typename InputImageType, typename OutputImageType>
    blImage* bilateral(typename InputImageType::Pointer image){
        typedef itk::BilateralImageFilter<
                InputImageType, OutputImageType >  FilterType;

        typename FilterType::Pointer filter = FilterType::New();
        filter->SetInput(image);
        filter->SetDomainSigma(m_domainSigma);
        filter->SetRangeSigma(m_rangeSigma);
        filter->Update();
        return new blImage(filter->GetOutput());
    }
};
