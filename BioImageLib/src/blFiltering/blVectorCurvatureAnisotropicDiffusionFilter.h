/// \file blVectorCurvatureAnisotropicDiffusionFilter.h
/// \brief blVectorCurvatureAnisotropicDiffusionFilter class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include "blFilter.h"
#include "itkVectorCurvatureAnisotropicDiffusionImageFilter.h"

/// \class blVectorCurvatureAnisotropicDiffusionFilter
/// \brief This filter performs anisotropic diffusion on a vector
/// Image using the modified curvature diffusion equation (MCDE)
class blVectorCurvatureAnisotropicDiffusionFilter : public blFilter{

public:
    /// \fn blVectorCurvatureAnisotropicDiffusionFilter();
    /// \brief Constructor
    blVectorCurvatureAnisotropicDiffusionFilter();
    /// \fn virtual ~blVectorCurvatureAnisotropicDiffusionFilter();
    /// \brief Destructor
    virtual ~blVectorCurvatureAnisotropicDiffusionFilter();

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
    /// \param[in] timeStep Time step
    void setTimeStep(float timeStep);
    /// \fn void setConductanceParameter(float conductanceParameter);
    /// \param[in] conductanceParameter Conductance parameter
    void setConductanceParameter(float conductanceParameter);

private:
    // parameters
    int m_iterations;
    float m_timeStep;
    float m_conductanceParameter;

private:
    // internal methods
    template<typename InputImageType, typename OutputImageType>
    blImage* anisotropicDiffusion(typename InputImageType::Pointer image){
        typedef itk::VectorCurvatureAnisotropicDiffusionImageFilter< InputImageType,
          OutputImageType > VectorCurvatureAnisotropicDiffusionImageFilterType;
        typename VectorCurvatureAnisotropicDiffusionImageFilterType::Pointer filter =
          VectorCurvatureAnisotropicDiffusionImageFilterType::New();
        filter->SetInput( image );
        filter->SetTimeStep(m_timeStep);
        filter->SetNumberOfIterations(m_iterations);
        filter->SetConductanceParameter(m_conductanceParameter);
        filter->Update();
        return new blImage(filter->GetOutput());
    }
};
