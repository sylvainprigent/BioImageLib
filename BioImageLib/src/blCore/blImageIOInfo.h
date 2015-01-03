/// \file blImageIOInfo.h
/// \brief blImageIOInfo class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include <string>

/// \class blImageIOInfo
/// \brief class containing the informations of an image file
class blImageIOInfo{

public:
    /// \fn blImageIOInfo();
    /// \brief Constructor
    blImageIOInfo();

    /// \fn void updateInfo(std::string inputFilename);
    /// \brief Update the image information
    /// \param[in] inputFilename image path
    void updateInfo(std::string inputFilename);

    /// \brief std::string GetPixelType();
    /// \return the pixel type
    std::string pixelType(){return m_pixelType;}
    /// \fn const size_t dimension()
    /// \return the image dimension
    const size_t dimension(){return m_dimension;}
    /// \fn int colorDimension()
    /// \return the color dimension
    int colorDimension(){return m_dimColor;}

private:
    std::string m_pixelType; ///< float, double...
    size_t m_dimension; ///< 2D, 3D
    int m_dimColor; ///< 1-grayscale...
};
