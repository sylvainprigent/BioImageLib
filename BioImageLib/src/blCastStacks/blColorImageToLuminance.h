/// \file blColorImageToLuminance.h
/// \brief blColorImageToLuminance class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include <blCore>
#include "itkRGBToLuminanceImageFilter.h"
#include "blCastStacksExport.h"

/// \class blColorImageToLuminance
/// \brief Extract the Luminance component from a color image
class BLCASTSTACKS_EXPORT blColorImageToLuminance : public blProcess{
public:
    /// \fn blColorImageToLuminance();
    /// \brief Constructor
    blColorImageToLuminance();
    /// \fn virtual ~blColorImageToLuminance();
    /// \brief Destructor
    virtual ~blColorImageToLuminance();

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

protected:
    blImage* m_inputImage; ///< input image
    blImage* m_outputImage; ///< output image

private:
    /// \fn template<typename InputImageType, typename OutputImageType> blImage* extractLuminance(typename InputImageType::Pointer image)
    /// \brief call itk filter implementation
    /// \param[in] image Pointer on the itk image to process
    /// \return pointer to the output blImage
    template<typename InputImageType, typename OutputImageType>
    blImage* extractLuminance(typename InputImageType::Pointer image){

        typedef itk::RGBToLuminanceImageFilter<
                InputImageType,
                OutputImageType >  FilterType;
        typename FilterType::Pointer filter = FilterType::New();
        filter->SetInput( image );
        filter->Update();
        return new blImage(filter->GetOutput());
    }
};
