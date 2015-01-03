/// \file blCurvatureFlowFilter.h
/// \brief blCurvatureFlowFilter class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include "blFilter.h"
#include "itkCurvatureFlowImageFilter.h"

/// \class blCurvatureFlowFilter
/// \brief Denoise an image using curvature driven flow.
class blCurvatureFlowFilter : public blFilter{

public:
    /// \fn blCurvatureFlowFilter();
    /// \brief Constructor
    blCurvatureFlowFilter();
    /// \fn virtual ~blCurvatureFlowFilter();
    /// \brief Destructor
    virtual ~blCurvatureFlowFilter();

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
    /// \fn void setNumberOfIterations(int iterations);
    /// \param[in] iterations Number of iterations
    void setNumberOfIterations(int iterations);
    /// \fn void setTimeStep(float timeStep);
    /// \param[in] timeStep Time step (default 0.125)
    void setTimeStep(float timeStep);

private:
    int m_iterations;
    float m_timeStep;

private:
    blImage* curvatureOnOneComponent(blImage* input, unsigned int index);

private:
    template<typename InputImageType, typename OutputImageType>
    blImage* curvature(typename InputImageType::Pointer image){
        typedef itk::CurvatureFlowImageFilter< InputImageType, OutputImageType >CurvatureFlowImageFilterType;

        typename CurvatureFlowImageFilterType::Pointer smoothing = CurvatureFlowImageFilterType::New();

        smoothing->SetInput( image );
        smoothing->SetNumberOfIterations( m_iterations );
        smoothing->SetTimeStep( m_timeStep );
        smoothing->Update();
        return new blImage(smoothing->GetOutput());
    }
};
