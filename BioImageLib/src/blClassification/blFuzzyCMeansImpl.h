/// \file blFuzzyCMeansImpl.h
/// \brief blFuzzyCMeansImpl class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2013

#pragma once

#include <blCore>
#include "blClassifier.h"

// //////////////////////////////////////////////////////
//          blFuzzyCMeansImpl
// //////////////////////////////////////////////////////
/// \class blFuzzyCMeansImpl
/// \brief Implementation clustering on an itkImage using fuzzy-c-means algorithm
/// Set useKernel to true to use a gaussian kernel for distance calculation.
class blFuzzyCMeansImpl{

private:
    Float2DImage::Pointer m_image;
    FloatColor2DImage::Pointer m_RGBVimage;
    Float2DImage::Pointer m_outputImage;

    std::vector<Float2DImage::Pointer> m_U;
    // param
    //std::string m_saveFileName;
    float m_expo, m_min_impro, m_obj_fcn_old, m_obj_fcn_new;
    int m_nbClass, m_max_iter, m_nl, m_nc ;
    bool m_showIter;
    float m_kernelParam, m_useKernel;
    std::vector<float> m_mean, m_var; // mean and std of the classification classes for a float2DImage
    std::vector<std::vector<float> > m_meanRGB, m_varRGB; // mean and std of the classification classes for a floatColor2DImage

public:
    /// \fn blFuzzyCMeansImpl();
    /// \brief Constructor
    blFuzzyCMeansImpl();

    /// \fn ~FuzzyCMeans();
    /// \brief Destructor
    ~blFuzzyCMeansImpl();

    // Set parameters manually
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
    /// \fn void setInput(Float2DImage::Pointer image);
    /// \param[in] image Image to be processed
    void setInput(Float2DImage::Pointer image);
    /// \fn void setInput(FloatColor2DImage::Pointer image);
    /// \param[in] image Image to be processed
    void setInput(FloatColor2DImage::Pointer image);
    /// \fn void update();
    /// \brief Starts the calculation
    void update();
    /// \fn void saveOutput();
    /// \brief Save the clasification result
    void saveOutput();
    /// \fn Float2DImage::Pointer getOutput(int i);
    /// \brief Function that return the probability map for each class
    /// \param[in] i class identification
    /// \return pointer to the image containing the map: values in [0,1]
    Float2DImage::Pointer getOutput(int i);

    /// \fn Int2DImage::Pointer getOutput();
    ///  \return the Pointer to the itk image containing the classification
    Int2DImage::Pointer getOutput();

    /// \fn std::vector<std::vector<float> >& getClassesMeanRGB();
    /// \brief Function that return the mean value of a class
    /// \return Mean std::vector of each class
    std::vector<std::vector<float> > getClassesMeanRGB();

    /// \fn std::vector<std::vector<float> >& getClassesStdRGB();
    /// \brief Function that return the covariance matrix of a class
    /// \return Mean std::vector of each class
    std::vector<std::vector<float> > getClassesStdRGB();

    /// \fn std::vector<float>&  getClassesMean2D();
    /// \return the mean value of each class
    std::vector<float>  getClassesMean2D();

    /// \fn float getClassesStd2D(int classId);
    /// \return the mean Standard deviation of each class
    std::vector<float> getClassesStd2D();

private:
    /// \fn void initfcm();
    /// \brief function that randomly initialise the class probability maps
    void initfcm();
    /// \fn void stepfcm();
    /// \brief function that update the Uij values for a Float2DImage
    void stepfcm();
    /// \fn void stepfcmRGBV();
    /// \brief function that update the Uij values for a FloatColor2DImage
    void stepfcmRGBV();
    /// \fn std::vector<float> stepkfcm(std::vector<float> centersOld);
    /// \brief function that update the kerneled version of Uij values for a Float2DImage
    /// \param[in] centersOld std::vector containing the centers
    std::vector<float> stepkfcm(std::vector<float> centersOld);
    /// \fn std::vector<float> stepkfcmRGBV(std::vector<float> centersOld);
    /// \brief function that update the kerneled version of Uij values for a FloatColor2DImage
    /// \param[in] centersOld std::vector containing the centers
    std::vector<float> stepkfcmRGBV(std::vector<float> centersOld);

    /// \fn float kernel(std::vector<float> center, int i, itk::Vector<float> imagePix);
    /// \brief compute the gaussian kernel between a center and FloatColor2DImage
    /// \param[in] center std::vector containing the class centers
    /// \param[in] i class to process
    /// \param[in] imagePix pixel to consider
    /// \return kernel value
    float kernel(std::vector<float> center, int i, float imagePix);
    /// \fn float kernel(std::vector<float> center, int i, itk::Vector<float> imagePix);
    /// \brief compute the gaussian kernel between a center and Float2DImage
    /// \param[in] center std::vector containing the class centers
    /// \param[in] i class to process
    /// \param[in] imagePix pixel to consider
    /// \return kernel value
    float RGBVkernel(std::vector<float> center, int i, PixelColorFloatType imagePix);

    /// \fn void computeMeanStdClassif();
    ///  \brief Compute the mean and std of the classes
    void computeMeanStdClassif();
    /// \fn void computeCentroidRGBClassif();
    /// \brief Compute the centroids of the RGB classes
    void computeCentroidRGBClassif();

};
