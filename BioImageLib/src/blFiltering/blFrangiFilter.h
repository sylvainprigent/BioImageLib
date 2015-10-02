/*
/// \file blFrangiFilter.h
/// \brief blFrangiFilter class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include "blFilter.h"
#include "itkHessianToObjectnessMeasureImageFilter.h"
#include "itkMultiScaleHessianBasedMeasureImageFilter.h"
#include "blFilteringExport.h"

/// \class blFrangiFilter
/// \brief Enhance the tubular structures in an image using the grangi filter
class BLFILTERING_EXPORT blFrangiFilter : public blFilter{

public:
    /// \fn blFrangiFilter();
    /// \brief Constructor
    blFrangiFilter();
    /// \fn virtual ~blFrangiFilter();
    /// \brief Destructor
    virtual ~blFrangiFilter();

public:
    /// \fn virtual std::string name();
    /// \brief Reimplement from blProcess
    virtual std::string name();

public:
    /// \fn virtual std::string name();
    /// \brief Reimplement from blProcess
    void run();

public:
    // parameters
    /// \fn void setAlpha( float alpha);
    /// \param[in] alpha Alpha parameter (see Hessian filtering)
    void setAlpha( float alpha);
    /// \fn void setBeta( float beta);
    /// \param[in] beta Beta parameter (see Hessian filtering)
    void setBeta( float beta);
    /// \fn void setGamma( float gamma);
    /// \param[in] gamma Gamma parameter (see Hessian filtering)
    void setGamma( float gamma);
    /// \fn void setBrightObject(bool brightObject);
    /// \param[in] brightObject set to true to enhance bright objects
    /// and to false to enhance dark objects
    void setBrightObject(bool brightObject);
    /// \fn void setSigmaMinimum( float sigmaMinimum );
    /// \param[in] sigmaMinimum Minimum sigma value for the multi level analysis
    void setSigmaMinimum( float sigmaMinimum );
    /// \fn void setSigmaMaximum( float sigmaMaximum );
    /// \param[in] sigmaMaximum Maximum sigma value for the multi level analysis
    void setSigmaMaximum( float sigmaMaximum );
    /// \fn void setNumberOfSigmaSteps( unsigned int numberOfSigmaSteps );
    /// \param[in] numberOfSigmaSteps Number of sigma steps for the multi level analysis
    void setNumberOfSigmaSteps( unsigned int numberOfSigmaSteps );

private:
    float m_alpha;
    float m_beta;
    float m_gamma;
    bool m_brightObject;
    float m_sigmaMinimum;
    float m_sigmaMaximum;
    unsigned int m_numberOfSigmaSteps;

private:
    blImage* frangiOnOneComponent(blImage* input, unsigned int index);


    typedef itk::SymmetricSecondRankTensor< float, 2 > Hessian2DPixelType;
    typedef itk::Image< Hessian2DPixelType, 2 >           Hessian2DImageType;

    typedef itk::SymmetricSecondRankTensor< float, 3 > Hessian3DPixelType;
    typedef itk::Image< Hessian3DPixelType, 3 >           Hessian3DImageType;


    template<typename InputImageType, typename HessianImageType, typename OutputImageType>
    blImage* frangiFilter(typename InputImageType::Pointer image){

        typedef itk::HessianToObjectnessMeasureImageFilter< HessianImageType, InputImageType >
                ObjectnessFilterType;
        typename ObjectnessFilterType::Pointer objectnessFilter = ObjectnessFilterType::New();
        objectnessFilter->SetBrightObject( m_brightObject );
        objectnessFilter->SetScaleObjectnessMeasure( false );
        objectnessFilter->SetAlpha( m_alpha );
        objectnessFilter->SetBeta( m_beta);
        objectnessFilter->SetGamma( m_gamma );

        typedef itk::MultiScaleHessianBasedMeasureImageFilter< InputImageType, HessianImageType, OutputImageType >
                MultiScaleEnhancementFilterType;
        typename MultiScaleEnhancementFilterType::Pointer multiScaleEnhancementFilter =
                MultiScaleEnhancementFilterType::New();
        multiScaleEnhancementFilter->SetInput( image );
        multiScaleEnhancementFilter->SetHessianToMeasureFilter( objectnessFilter );
        multiScaleEnhancementFilter->SetSigmaStepMethodToLogarithmic();
        multiScaleEnhancementFilter->SetSigmaMinimum( m_sigmaMinimum );
        multiScaleEnhancementFilter->SetSigmaMaximum( m_sigmaMaximum );
        multiScaleEnhancementFilter->SetNumberOfSigmaSteps( m_numberOfSigmaSteps );
        multiScaleEnhancementFilter->Update();
        return new blImage(multiScaleEnhancementFilter->GetOutput());
    }
};
*/
