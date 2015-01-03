/// \file blExtractComponentColorImage.h
/// \brief blExtractComponentColorImage class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include <blCore>
#include "itkVectorIndexSelectionCastImageFilter.h"

/// \class blExtractComponentColorImage
/// \brief Extract a gray scale component (red, green or blue)
/// from a color image
class blExtractComponentColorImage : public blProcess{
public:
    /// \fn blExtractComponentColorImage();
    /// \brief Constructor
    blExtractComponentColorImage();
    /// \fn virtual ~blExtractComponentColorImage();
    /// \brief Destructor
    virtual ~blExtractComponentColorImage();

public:
    /// \fn virtual std::string name();
    /// \brief Reimplement from blProcess
    virtual std::string name();

public:
    /// \fn void setInput(blImage* image);
    /// \param[in] image Input image
    void setInput(blImage* image);

    /// \fn void setComponentIndex(unsigned int index);
    /// \brief index Index of the component to extract
    void setComponentIndex(unsigned int index);

    /// \fn void run();
    /// \brief Run the computation
    void run();

    /// \fn blImage* output();
    /// \return the extracted gray scale image
    blImage* output();

protected:
    blImage* m_inputImage; ///< input image
    blImage* m_outputImage; ///< output image
    unsigned int m_index; ///< index of the component to extract

private:
    /// \brief call the itk cast filter
    /// \param[in] image Pointer to the itk image to process
    /// \param[in] index Index of the component to extract
    template<typename InputImageType, typename OutputImageType>
    blImage* extractComponent(typename InputImageType::Pointer image, unsigned int index){
            typedef itk::VectorIndexSelectionCastImageFilter<InputImageType, OutputImageType> ImageAdaptorType;
            typename ImageAdaptorType::Pointer adaptor = ImageAdaptorType::New();
            adaptor->SetIndex(index);
            adaptor->SetInput(image);
            adaptor->Update();
            return new blImage(adaptor->GetOutput());
    }
};
