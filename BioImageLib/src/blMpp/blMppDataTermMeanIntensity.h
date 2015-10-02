/// \file blMppDataTermMeanIntensity.h
/// \brief blMppDataTermMeanIntensity class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once

#include "blMppDataTerm.h"
#include "blMppExport.h"

/// \class blMppDataTermMeanIntensity
/// \brief Class that define an Marked Point Process data term
/// as the Bhattacharyya distance calculated between the distributions
/// of the pixels inside and the pixels in the border of a shape
class BLMPP_EXPORT blMppDataTermMeanIntensity : public blMppDataTerm{

public:
    /// \fn blMppDataTermMeanIntensity();
    /// \brief Reimplement from blMppDataTerm
    blMppDataTermMeanIntensity();
    /// \fn blMppDataTermMeanIntensity(blImage* image);
    /// \brief Reimplement from blMppDataTerm
    blMppDataTermMeanIntensity(blImage* image);
    /// \fn virtual ~blMppDataTermMeanIntensity();
    /// \brief Reimplement from blMppDataTerm
    virtual ~blMppDataTermMeanIntensity();

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
    float m_D; ///< see setD()

protected:
    // internal methods
    /// \fn void computeStats(blMppShape *shape, float &meanIn);
    /// \brief Compute the statistics of a shapes pixels values for the mean intensity
    void computeStats(blMppShape *shape, float &meanIn);

};
