/// \file blThreshold.h
/// \brief blThreshold class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include <itkBinaryThresholdImageFilter.h>
#include "blClassifier.h"

/// \class blThreshold
/// \brief Threshold an image to binarize it
/// This class use two thresholds. The output
/// it insideValue (default 255) if the intensity is between the two thresholds
/// and OutsideValue (default 0) otherwhise
class BLCLASSIFICATION_EXPORT blThreshold : public blClassifier{

public:
    /// \fn blThreshold();
    /// \brief Constructor
    blThreshold();
    /// \fn virtual ~blThreshold();
    /// \brief Destructor
    virtual ~blThreshold();

public:
    /// \fn virtual std::string name();
    /// \brief Reimplement from blProcess
    virtual std::string name();

public:
    /// \fn void run();
    /// \brief Reimplement from blClassifier
    void run();

public:
    // parameters
    /// \fn void setLowBoundary(const float& min);
    /// \param[in] min Low threshold value
    void setLowBoundary(const float& min);
    /// \fn void setHighBoundary(const float& max);
    /// \param[in] max Upper threshold value
    void setHighBoundary(const float& max);
    /// \fn void setBoundaries(const float& min,const float& max);
    /// \param[in] min Low threshold value
    /// \param[in] max Upper threshold value
    void setBoundaries(const float& min,const float& max);

    /// \fn void setInsideValue(const float& inside);
    /// \param[in] inside Value set to pixels inside the two thresholds interval
    void setInsideValue(const float& inside);
    /// \fn void setOutsideValue(const float& outside);
    /// \param[in] outside Value set to pixels outside the two thresholds interval
    void setOutsideValue(const float& outside);
    /// \fn void setValues(const float& inside, const float& outside);
    /// \param[in] inside Value set to pixels inside the two thresholds interval
    /// \param[in] outside Value set to pixels outside the two thresholds interval
    void setValues(const float& inside, const float& outside);

private:
    float m_lowerThreshold; ///< see setLowBoundary
    float m_upperThreshold; ///< see setHighBoundary
    float m_insideValue;    ///< see setInsideValue
    float m_outsideValue;   ///< see setOutsideValue

private:
    /// \brief call the itk threshold implementation
    /// \param[in] image Pointer to the itk image to process
    /// \return a pointer to the resulting blImage
    template<typename InputImageType, typename OutputImageType>
    blImage* threshold(typename InputImageType::Pointer image){

        typedef itk::BinaryThresholdImageFilter <InputImageType, OutputImageType>
          BinaryThresholdImageFilterType;

        typename BinaryThresholdImageFilterType::Pointer thresholdFilter
          = BinaryThresholdImageFilterType::New();
        thresholdFilter->SetInput(image);
        thresholdFilter->SetLowerThreshold(m_lowerThreshold);
        thresholdFilter->SetUpperThreshold(m_upperThreshold);
        thresholdFilter->SetInsideValue(m_insideValue);
        thresholdFilter->SetOutsideValue(m_outsideValue);
        thresholdFilter->Update();
        return new blImage(thresholdFilter->GetOutput());
    }
};

