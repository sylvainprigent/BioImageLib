/// \file blComposeColorImage.h
/// \brief blComposeColorImage class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include <blCore>
#include "itkComposeImageFilter.h"
#include "blCastStacksExport.h"

/// \class blComposeColorImage
/// \brief Create a color image from 3 gray scaled images
class BLCASTSTACKS_EXPORT blComposeColorImage : blProcess{

public:
    /// \fn blComposeColorImage();
    /// \brief Constructor
    blComposeColorImage();
    /// \fn virtual ~blComposeColorImage();
    /// \brief Destructor
    virtual ~blComposeColorImage();

public:
    /// \fn virtual std::string name();
    /// \brief Reimplement from blProcess
    virtual std::string name();

public:
    /// \fn void setComposant1(blImage* comp1);
    /// \param[in] comp1 First conponent (red)
    void setComposant1(blImage* comp1);
    /// \fn void setComposant2(blImage* comp2);
    /// \param[in] comp2 Second conponent (green)
    void setComposant2(blImage* comp2);
    /// \fn void setComposant3(blImage* comp3);
    /// \param[in] comp3 Third conponent (blue)
    void setComposant3(blImage* comp3);

    /// \fn void run();
    /// \brief run the computation
    void run();

    /// \fn blImage* output();
    /// \return the resulting color image
    blImage* output();

protected:
    blImage* m_comp1; ///< component 1
    blImage* m_comp2; ///< component 2
    blImage* m_comp3; ///< component 3
    blImage* m_outputImage; ///< output image

private:
    /// \brief call the itk compose filter
    /// \param[in] comp1 Red component
    /// \param[in] comp2 Green component
    /// \param[in] comp3 Blue component
    template<typename InputImageType, typename OutputImageType>
    blImage* composeImages(typename InputImageType::Pointer comp1,
                                                    typename InputImageType::Pointer comp2,
                                                    typename InputImageType::Pointer comp3){

        typedef itk::ComposeImageFilter<InputImageType,OutputImageType> ComposeFilterType;
        typename ComposeFilterType::Pointer composeFilter = ComposeFilterType::New();
        composeFilter->SetInput1(comp1);
        composeFilter->SetInput2(comp2);
        composeFilter->SetInput3(comp3);
        composeFilter->Update();

        return new blImage(composeFilter->GetOutput());
    }
};
