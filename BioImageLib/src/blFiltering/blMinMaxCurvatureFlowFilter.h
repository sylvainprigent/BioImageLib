/// \file blMinMaxCurvatureFlowFilter.h
/// \brief blMinMaxCurvatureFlowFilter class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include "blFilter.h"
#include "itkMinMaxCurvatureFlowImageFilter.h"

/// \class blMinMaxCurvatureFlowFilter
/// \brief Denoise an image using min/max curvature flow.
class blMinMaxCurvatureFlowFilter : public blFilter{

public:
    /// \fn blMinMaxCurvatureFlowFilter();
    /// \brief Constructor
    blMinMaxCurvatureFlowFilter();
    /// \fn virtual ~blMinMaxCurvatureFlowFilter();
    /// \brief Destructor
    virtual ~blMinMaxCurvatureFlowFilter();

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

    template<typename InputImageType, typename OutputImageType>
    blImage* curvature(typename InputImageType::Pointer image){
        typedef itk::MinMaxCurvatureFlowImageFilter< InputImageType, OutputImageType >CurvatureFlowImageFilterType;

        typename CurvatureFlowImageFilterType::Pointer smoothing = CurvatureFlowImageFilterType::New();

        smoothing->SetInput( image );
        smoothing->SetNumberOfIterations( m_iterations );
        smoothing->SetTimeStep( m_timeStep );
        smoothing->Update();
        return new blImage(smoothing->GetOutput());
    }
};
