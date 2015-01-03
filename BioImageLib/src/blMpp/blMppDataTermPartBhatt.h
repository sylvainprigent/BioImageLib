/// \file blMppDataTermPartBhatt.h
/// \brief blMppDataTermPartBhatt class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once

#include "blMppDataTerm.h"
#include "blMppShapeEllipse.h"

/// \class blMppDataTermPartBhatt
/// \brief Class that define an Marked Point Process data term
/// as the Bhattacharyya distance calculated between the distributions
/// of the pixels inside and the pixels in the border of a shape
/// The shape bordure is partitioned into 4 connected regions, and
/// at least 3 of these regions must have a Bhattacharyya distance
/// greater than the threshold for the data term to be negative
/// This data term has been made for the axon data set of Caroline
class blMppDataTermPartBhatt : public blMppDataTerm{

public:
    /// \fn blMppDataTermPartBhatt();
    /// \brief Reimplement from blMppDataTerm
    blMppDataTermPartBhatt();
    /// \fn blMppDataTermPartBhatt(blImage* image);
    /// \brief Reimplement from blMppDataTerm
    blMppDataTermPartBhatt(blImage* image);
    /// \fn virtual ~blMppDataTermPartBhatt();
    /// \brief Reimplement from blMppDataTerm
    virtual ~blMppDataTermPartBhatt();

public:
    /// \fn virtual void initialize();
    /// \brief Reimplement from blMppDataTerm
    virtual void initialize();
    /// \fn virtual float compute(blMppShape* shape);
    /// \brief Reimplement from blMppDataTerm
    virtual float compute(blMppShape* shape);
    /// \fn virtual void createCompatibleShapesList();
    /// \brief Reimplement from blMppDataTerm
    virtual void createCompatibleShapesList();

    /// \fn virtual bool useInsidePixels();
    /// \brief Reimplement from blMppDataTerm
    virtual bool useInsidePixels();
    /// \fn virtual bool useBorderPixels();
    /// \brief Reimplement from blMppDataTerm
    virtual bool useBorderPixels();
    /// \fn virtual bool useNormals();
    /// \brief Reimplement from blMppDataTerm
    virtual bool useNormals();

public:
    // blProcess methods
    /// \fn virtual std::string name();
    /// \brief Reimplement from blProcess
    virtual std::string name();

public:
    // setters parameters
    /// \fn void setD(float D);
    /// \param[in] D Slope of the exponential function that
    /// fit the Bhattacharyya distance into [-1,1]
    void setD(float D);

protected:
    float m_D; ///< slop of the threshold function

protected:
    bool m_is2DImage; ///< internal attribut to know the image type
    unsigned int m_implementationId;

protected:
    // internal methods
    /// \brief Compute function in the case of ellipse
    /// \param[in] shape Shape for which the data term is calculated
    /// \return the value of the data term
    float computeEllipse(blMppShape* shape);
    /// \brief Compute function in the case of ellipse
    /// \param[in] shape Shape for which the data term is calculated
    /// \return the value of the data term
    float computeEllipseIn3D(blMppShape* shape);
    /// \brief Compute the intensities of pixels in four equal regions in the border of the shape
    /// \param[in] shape Shape to test
    /// \param[out] vhead1 Intensities in the first region
    /// \param[out] vhead2 Intensities in the second region
    /// \param[out] vside1 Intensities in the third region
    /// \param[out] vside2 Intensities in the fourth region
    void partitionEllipse( blMppShapeEllipse* shape, std::vector<float> &vhead1, std::vector<float> &vhead2, std::vector<float> &vside1, std::vector<float> &vside2);
    /// \brief Compute the intensities of pixels in four equal regions in the border of the shape
    /// \param[in] shape Shape to test
    /// \param[out] vhead1 Intensities in the first region
    /// \param[out] vhead2 Intensities in the second region
    /// \param[out] vside1 Intensities in the third region
    /// \param[out] vside2 Intensities in the fourth region
    void partitionEllipseIn3D( blMppShapeEllipse* shape, std::vector<float> &vhead1, std::vector<float> &vhead2, std::vector<float> &vside1, std::vector<float> &vside2);
    /// \brief Compute the intensity of pixels inside the shape
    /// \param[in] shape Shape to test
    /// \param[out] valuesIn Intensity values inside the shape
    void computeStats2D(blMppShape *shape, std::vector<float> &valuesIn);
    /// \brief Compute the intensity of pixels inside the shape
    /// \param[in] shape Shape to test
    /// \param[out] valuesIn Intensity values inside the shape
    void computeStats3D(blMppShape *shape, std::vector<float> &valuesIn);
};
