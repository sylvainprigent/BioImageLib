/// \file blMppDataTermGradientCompt.h
/// \brief blMppDataTermGradientCompt class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once

#include "blMppDataTerm.h"
#include "itkGradientRecursiveGaussianImageFilter.h"
#include "itkImage.h"
#include "blMppExport.h"

/// \class blMppDataTermGradientCompt
/// \brief Class that define an Marked Point Process data term
/// as the average normalized gradient of the pixels in the border of a shape
class BLMPP_EXPORT blMppDataTermGradientCompt : public blMppDataTerm{

public:
    /// \fn blMppDataTermGradientCompt();
    /// \brief reimplement from blMppDataTerm
    blMppDataTermGradientCompt();
    /// \fn blMppDataTermGradientCompt(blImage* image);
    /// \brief reimplement from blMppDataTerm
    blMppDataTermGradientCompt(blImage* image);
    /// \fn virtual ~blMppDataTermGradientCompt();
    /// \brief reimplement from blMppDataTerm
    virtual ~blMppDataTermGradientCompt();

public:
    /// \fn virtual void initialize();
    /// \brief reimplement from blMppDataTerm
    virtual void initialize();
    /// \fn virtual float compute(blMppShape* shape);
    /// \brief reimplement from blMppDataTerm
    virtual float compute(blMppShape* shape);
    /// \fn virtual void createCompatibleShapesList();
    /// \brief reimplement from blMppDataTerm
    virtual void createCompatibleShapesList();

    /// \fn virtual bool useInsidePixels();
    /// \brief reimplement from blMppDataTerm
    virtual bool useInsidePixels();
    /// \fn virtual bool useBorderPixels();
    /// \brief reimplement from blMppDataTerm
    virtual bool useBorderPixels();
    /// \fn virtual bool useNormals();
    /// \brief reimplement from blMppDataTerm
    virtual bool useNormals();

public:
    // blProcess methods
    /// \fn virtual std::string name();
    /// \brief reimplement from blProcess
    virtual std::string name();

public:
    // setters parameters
    /// \fn void setCompt(float compt);
    /// \param[in] compt Threshold on the gradient norm
    /// to keep only well oriented gradient in the data term calculation
    void setCompt(float compt);
    /// \fn void setSigma(float sigma);
    /// \param[in] sigma Sigma used to calculate
    /// the gradient using the iterative Gaussian gradient
    void setSigma(float sigma);
    /// \fn void setGradientDirection(int gradientDirection);
    /// \param[in] gradientDirection Color of the object
    ///          - (1) Light shapes in dark background
    ///          - (-1)Dark shapes in light background
    ///          - (0) Both light and dark shapes
    void setGradientDirection(int gradientDirection);

protected:
    float m_compt; ///< see setCompt()
    int m_gradientDirection; ///< see setGradientDirection()
    float m_sigma; ///< recursive gradient sigme

protected:
    bool m_is2DImage;

protected:
    // internal Data
    itk::Image< itk::CovariantVector< float, 2 >, 2 >::Pointer m_gradient2D; ///< gradient image
    itk::Image< itk::CovariantVector< float, 3 >, 3 >::Pointer m_gradient3D; ///< gradient image

protected:
    // functions pointers for threshold
    typedef float (blMppDataTermGradientCompt::*ThresholdFunction)(float); ThresholdFunction thresholdPtr;
    float thresholdNegatif(float energy);
    float thresholdPositif(float energy);
    float thresholdsymetric(float energy);
    void initGradientFunctionsPointers();

    /// \brief Compute method for the 2D case
    /// \param[in] shape Shape for which the data term is calculated
    /// \return the value of the data term
    float compute2D(blMppShape* shape);
    /// \brief Compute method for the 3D case
    /// \param[in] shape Shape for which the data term is calculated
    /// \return the value of the data term
    float compute3D(blMppShape *shape);
    /// \brief Commpute the gradient of the image
    /// depending on the 2D or 3D case
    void calculateGradientImage();
};
