/// \file blComparisonImageFilter.h
/// \brief blComparisonImageFilter class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include <blCore>
#include "itkTestingComparisonImageFilter.h"
#include "blComparisonImageExport.h"

/// \class blComparisonImageFilter
/// \brief Compare if two images are equals
class BLCOMPARISONIMAGE_EXPORT blComparisonImageFilter : public blProcess{

public:
    /// \fn blComparisonImageFilter();
    /// \brief Constructor
    blComparisonImageFilter();
    /// \fn virtual ~blComparisonImageFilter();
    /// \brief Destructor
    virtual ~blComparisonImageFilter();

public:
    /// \fn virtual std::string name();
    /// \brief Reimplement from blProcess
    virtual std::string name();

public:
    // inputs
    /// \fn void setReferenceImage(blImage* image);
    /// \param[in] image Reference image
    void setReferenceImage(blImage* image);
    /// \fn void setTestImage(blImage* image);
    /// \param[in] image Image to compare with the reference image
    void setTestImage(blImage* image);

    // run
    /// \fn void run();
    /// \brief Run the computation
    void run();

    // outputs
    /// \fn unsigned long numberOfPixelsWithDifferences();
    /// \return the number of pixels that are differents in the
    /// two input images
    unsigned long numberOfPixelsWithDifferences();

    // parameters
    /// \fn void setDifferenceThreshold( float intensityTolerance );
    /// \param[in] intensityTolerance Tolerence threshold to determine
    /// if two pixels are differents
    void setDifferenceThreshold( float intensityTolerance );
    /// \fn void setToleranceRadius( int radiusTolerance );
    /// \param[in] radiusTolerance Tolerence radius
    void setToleranceRadius( int radiusTolerance );

private:
    // inputs
    blImage* m_referenceImage; ///< Pionter to the reference image
    blImage* m_testImage; ///< Pointer to the test image

    // outputs
    unsigned long m_numberOfPixelsWithDifferences; ///< see numberOfPixelsWithDifferences();

    //parameters
    float m_intensityTolerance;  ///< see setDifferenceThreshold();
    int m_radiusTolerance;       ///< see setToleranceRadius();

private:
    /// \fn unsigned short compareOneComponent(blImage* refImage, blImage* testImage, unsigned int index);
    /// \brief Compare one monochrome component of the 2 images
    /// \param[in] refImage reference image
    /// \param[in] testImage test image
    /// \param[in] index index of the layer to compare
    unsigned short compareOneComponent(blImage* refImage, blImage* testImage, unsigned int index);

    /// \brief call the itk implementation
    /// \param[in] refImage reference image
    /// \param[in] testImage test image
    template<typename InputImageType>
    void compareImages(typename InputImageType::Pointer refImage, typename InputImageType::Pointer testImage){
        typedef itk::Testing::ComparisonImageFilter<InputImageType,InputImageType> DiffType;
        typename DiffType::Pointer diff = DiffType::New();
        diff->SetValidInput(refImage);
        diff->SetTestInput(testImage);
        diff->SetDifferenceThreshold( m_intensityTolerance );
        diff->SetToleranceRadius( m_radiusTolerance );
        diff->UpdateLargestPossibleRegion();

        m_numberOfPixelsWithDifferences = diff->GetNumberOfPixelsWithDifferences();
    }
};
