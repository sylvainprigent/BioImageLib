/// \file blFuzzyCMeans.h
/// \brief blFuzzyCMeans class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2013

#pragma once

#include <itkBinaryThresholdImageFilter.h>
#include "blClassifier.h"
#include <blCore/blImageList.h>
#include <blCore/blVectorNumber2D.h>

/// \class blFuzzyCMeans
/// \brief classify an image using fuzzy c means algorithm
class BLCLASSIFICATION_EXPORT blFuzzyCMeans : public blClassifier{

public:
    /// \fn blFuzzyCMeans();
    /// \brief Constructor
    blFuzzyCMeans();
    /// \fn virtual ~blFuzzyCMeans();
    /// \brief Destructor
    virtual ~blFuzzyCMeans();

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
    /// \fn void setUseKernel(bool useKernel);
    /// \brief Function that allows to use/unuse the kernel
    /// By default no kernel used
    void setUseKernel(bool useKernel);
    /// \fn void setKernelParam(float kernelParam);
    /// \brief Function that set the kernel parameter
    /// By default equals 150
    void setKernelParam(float kernelParam);
    /// \fn void setNbClass(int nbClass);
    /// \brief Function that set the desired number of classes
    /// By default equals 3
    void setNbClass(int nbClass);
    /// \fn void setExpo(double expo);
    /// \brief Function that set fuzzy parameter
    /// By default equals 2
    void setExpo(double expo);
    /// \fn void setMaxIter(int max_iter);
    /// \brief Function that set maximum number of iterations
    /// By default equals 100
    void setMaxIter(int max_iter);
    /// \fn void setStopContPrecision(double min_impro);
    /// \brief Function that set the precision for stop criterion
    /// By default equals 0.001
    void setStopContPrecision(double min_impro);
    /// \fn void setIterVerbose(bool verbose);
    /// \brief Function that set the iteration verbose
    /// By default equals false
    void setIterVerbose(bool verbose);

public:
    // outputs
    /// \fn blImageList* classesProbabilities();
    /// \return the probabilities map for each pixel to belong to
    /// each class
    blImageList* classesProbabilities();

    /// \fn std::vector<std::vector<float> > &classesMean();
    /// \brief Function that return the mean value of a class
    /// \return Mean std::vector of each class
    std::vector<std::vector<float> > &classesMean();
    /// \fn std::vector<std::vector<float> >& classesStd();
    /// \brief Function that return the covariance matrix of a class
    /// \return Mean std::vector of each class
    std::vector<std::vector<float> >& classesStd();
    /// \fn std::vector<float>& ClassesMean2D();
    /// \return the mean value of each class
    std::vector<float>& classesMean2D();
    /// \fn std::vector<float>& classesStd2D();
    /// \return the mean Standard deviation of each class
    std::vector<float>& classesStd2D();

    /// \fn blTableNumber* classesMeans();
    /// \return a table containing the centroids of each class
    blTableNumber* classesMeans();
    /// \fn blTableNumber* classesVars();
    /// \return a table containing the variance of each class
    blTableNumber* classesVars();
private:
    // parameters
    bool m_useKernel; ///< see setUseKernel
    float m_kernelParam; ///< see setKernelParam
    int m_nbClass; ///< see setNbClass
    double m_expo; ///< see setExpo
    int m_max_iter; ///< see setMaxIter
    double m_min_impro; ///< see setStopContPrecision

private:
    // outptuts
    blImageList* m_classesProbabilities; ///< Probabilities images (one per class)
    std::vector<float> m_scalarClassesMean; ///< Mean of each class (scalar image)
    std::vector<float> m_scalarClassesStd; ///< Standard deviation in each class (scalar image)
    std::vector<std::vector<float> > m_vectorClassesMean; ///< Mean of each class (vectorial image)
    std::vector<std::vector<float> > m_vectorClassesStd; ///< Standard deviation in each class (vectorial image)
};

