/// \file blResizeImage.h
/// \brief blResizeImage class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include <blCore>
#include "itkRGBToLuminanceImageFilter.h"
#include "itkIdentityTransform.h"
#include "itkResampleImageFilter.h"

/// \class blResizeImage
/// \brief Extract a gray scale component (red, green or blue)
/// from a color image
class blResizeImage : public blProcess{
public:
    /// \fn blResizeImage();
    /// \brief Constructor
    blResizeImage();
    /// \fn virtual ~blResizeImage();
    /// \brief Destructor
    virtual ~blResizeImage();


public:
    /// \fn virtual std::string name();
    /// \brief Reimplement from blProcess
    virtual std::string name();

public:
    /// \fn void setInput(blImage* image);
    /// \param[in] image Input image
    void setInput(blImage* image);

    /// \fn void run();
    /// \brief Run the computation
    void run();

    /// \fn blImage* output();
    /// \return the extracted gray scale image
    blImage* output();

public:
    // parameters
    /// \fn void setSizeY(unsigned int sizeY);
    /// \param[in] sizeY Size of the output image in the Y direction
    void setSizeY(unsigned int sizeY);

protected:
    // I/O
    blImage* m_inputImage; ///< input image
    blImage* m_outputImage; ///< output image

    // param
    unsigned int m_sizeY; ///< output Y size of the image


private:
    /// \brief call the itk image resize implementation
    ///  \param[in] image Pointer to the itk image to process
    /// \return the pointer to the resized blImage
    template<typename InputImageType>
    blImage* resizeImage2D(typename InputImageType::Pointer image){

        // resize
        typename InputImageType::SizeType inputSize;
        inputSize[0] = image->GetLargestPossibleRegion().GetSize()[0];
        inputSize[1] = image->GetLargestPossibleRegion().GetSize()[1];

        typename InputImageType::SizeType outputSize;
        outputSize[1] = m_sizeY;
        float factor = m_sizeY/float(inputSize[1]);
        outputSize[0] = int(factor*float(inputSize[0]));

        std::cout << "resizeImage2D output image size = " << outputSize[0] << ", " << outputSize[1] << std::endl;

        typename InputImageType::SpacingType outputSpacing;
        outputSpacing[0] = image->GetSpacing()[0] * (static_cast<double>(inputSize[0]) / static_cast<double>(outputSize[0]));
        outputSpacing[1] = image->GetSpacing()[1] * (static_cast<double>(inputSize[1]) / static_cast<double>(outputSize[1]));

        typedef itk::IdentityTransform<double, 2> TransformType;
        typedef itk::ResampleImageFilter<InputImageType, InputImageType> ResampleImageFilterType;
        typename ResampleImageFilterType::Pointer resample = ResampleImageFilterType::New();
        resample->SetInput(image);
        resample->SetSize(outputSize);
        resample->SetOutputSpacing(outputSpacing);
        resample->SetTransform(TransformType::New());
        resample->UpdateLargestPossibleRegion();
        resample->Update();
        return new blImage(resample->GetOutput());
    }
};
