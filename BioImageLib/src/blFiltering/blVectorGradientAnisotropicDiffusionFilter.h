/// \file blVectorGradientAnisotropicDiffusionFilter.h
/// \brief blVectorGradientAnisotropicDiffusionFilter class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include "blFilter.h"
#include "itkVectorGradientAnisotropicDiffusionImageFilter.h"

/// \class blVectorGradientAnisotropicDiffusionFilter
/// \brief This filter performs anisotropic diffusion on a vector Image
class blVectorGradientAnisotropicDiffusionFilter : public blFilter{

public:
    /// \fn blVectorGradientAnisotropicDiffusionFilter();
    /// \brief Constructor
    blVectorGradientAnisotropicDiffusionFilter();
    /// \fn virtual ~blVectorGradientAnisotropicDiffusionFilter();
    /// \brief Destructor
    virtual ~blVectorGradientAnisotropicDiffusionFilter();

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
    /// \brief set parameter
    /// \param[in] iterations Number of iterations
    void setNumberOfIterations(int iterations);
    /// \fn void setTimeStep(float timeStep);
    /// \brief set parameter
    /// \param[in] timeStep Time step
    void setTimeStep(float timeStep);
    /// \fn void setConductanceParameter(float conductanceParameter);
    /// \brief set parameter
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
        typedef itk::VectorGradientAnisotropicDiffusionImageFilter< InputImageType,
          OutputImageType > VectorGradientAnisotropicDiffusionImageFilterType;
        typename VectorGradientAnisotropicDiffusionImageFilterType::Pointer filter =
          VectorGradientAnisotropicDiffusionImageFilterType::New();
        filter->SetInput( image );
        filter->SetTimeStep(m_timeStep);
        filter->SetNumberOfIterations(m_iterations);
        filter->SetConductanceParameter(m_conductanceParameter);
        filter->Update();
        return new blImage(filter->GetOutput());
    }
};
