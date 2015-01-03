/// \file blProjectionImage.h
/// \brief blZProjection class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include <blCore>

/// \class blZProjection
/// \brief Project a 3D image along z direction
/// to obtain a 2D image
class blZProjection : public blProcess{

public:
    /// \fn blZProjection();
    /// \brief Constructor
    blZProjection();
    /// \fn virtual ~blZProjection();
    /// \brief Desctructor
    virtual ~blZProjection();

public:
    /// \fn virtual std::string name();
    /// \brief Redefinition from blProcess
    virtual std::string name();

public:
    /// \fn void setInput(blImage* image);
    /// \param[in] image Input image
    void setInput(blImage* image);

    /// \fn void setProjectionType(std::string projectionType);
    /// \brief Set the type of projection to use. Use the static
    /// strings as input
    /// \param[in] projectionType Type of projection
    void setProjectionType(std::string projectionType);

    /// \fn void run();
    /// \brief Run the computation
    void run();

    /// \fn blImage* output();
    /// \return the extracted gray scale image
    blImage* output();

private:
    /// \brief implement maximum projection along Z on an itk image
    /// \param[in] image Pointer to the itk image to project
    /// \return Pointer to the projected itk image
    Float2DImage::Pointer MaximumZ(Float3DImage::Pointer image);
    /// \brief implement minimum projection along Z on an itk image
    /// \param[in] image Pointer to the itk image to project
    /// \return Pointer to the projected itk image
    Float2DImage::Pointer MinimumZ(Float3DImage::Pointer image);
    /// \brief implement mean projection along Z on an itk image
    /// \param[in] image Pointer to the itk image to project
    /// \return Pointer to the projected itk image
    Float2DImage::Pointer MeanZ(Float3DImage::Pointer image);
    /// \brief implement standard deviation projection along Z on an itk image
    /// \param[in] image Pointer to the itk image to project
    /// \return Pointer to the projected itk image
    Float2DImage::Pointer StdZ(Float3DImage::Pointer image);
    /// \brief implement median projection along Z on an itk image
    /// \param[in] image Pointer to the itk image to project
    /// \return Pointer to the projected itk image
    Float2DImage::Pointer MedianZ(Float3DImage::Pointer image);

    /// \fn blImage* ProjectOneComponent(blImage* input, unsigned int index);
    /// \brief run the projection filter in one component of rgb image
    /// \return the casted component
    /// \param[in] input Image to cast
    /// \param[in] index Index of the layer to cast
    blImage* ProjectOneComponent(blImage* input, unsigned int index);

private:
    blImage* m_inputImage; ///< input image
    blImage* m_outputImage; ///< output image
    std::string m_projectionType; ///< type of projection to use

public:
    // projection types
    static const std::string Maximum; ///< Maximum projection type
    static const std::string Minimum; ///< Minimum projection type
    static const std::string Mean;    ///< Mean projection type
    static const std::string Sum;     ///< Sum projection type
    static const std::string StandardDeviation; ///< StandardDeviation projection type
    static const std::string Median;  ///< Median projection type
};
