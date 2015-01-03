/// \file blImageSize.h
/// \brief blImageSize class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

/// \class blImageSize
/// \brief Contains the size of a blImage
class blImageSize{

public:
    /// \fn blImageSize();
    /// \brief Constructor
    blImageSize();
    /// \fn blImageSize(int xSize, int ySize, int colorSize);
    /// \brief Constructor
    /// \param[in] xSize Size along X
    /// \param[in] ySize Size along Y
    /// \param[in] colorSize Number of color components
    blImageSize(int xSize, int ySize, int colorSize);
    /// \fn blImageSize(int xSize, int ySize, int zSize, int colorSize);
    /// \brief Constructor
    /// \param[in] xSize Size along X
    /// \param[in] ySize Size along Y
    /// \param[in] zSize Size along Z
    /// \param[in] colorSize Number of color components
    blImageSize(int xSize, int ySize, int zSize, int colorSize);

public:
    // getters
    /// \fn int xSize();
    /// \return the x size of the image
    int xSize();
    /// \fn int ySize();
    /// \return the y size of the image
    int ySize();
    /// \fn int zSize();
    /// \return the z size of the image
    int zSize();
    /// \fn int colorSize();
    ///  \return the color size (1 gray, 3 rgb...)
    int colorSize();

    // setters
    /// \fn void setXSize(int value);
    /// \param[in] value x size of the image
    void setXSize(int value);
    /// \fn void setYSize(int value);
    /// \param[in] value y size of the image
    void setYSize(int value);
    /// \fn void setZSize(int value);
    /// \param[in] value z size of the image
    void setZSize(int value);
    /// \fn void setColorSize(int value);
    /// \param[in] value the color size (1 gray, 3 rgb...)
    void setColorSize(int value);
    /// \fn void setSize(int xSize, int ySize, int zSize, int colorSize);
    /// \param[in] xSize x size of the image
    /// \param[in] ySize y size of the image
    /// \param[in] zSize z size of the image
    /// \param[in] colorSize the color size (1 gray, 3 rgb...)
    void setSize(int xSize, int ySize, int zSize, int colorSize);
    /// \fn void setSize(int xSize, int ySize, int colorSize);
    /// \param[in] xSize x size of the image
    /// \param[in] ySize y size of the image
    /// \param[in] colorSize the color size (1 gray, 3 rgb...)
    void setSize(int xSize, int ySize, int colorSize);

private:
    int m_xSize;     ///< see xSize()
    int m_ySize;     ///< see ySize()
    int m_zSize;     ///< see zSize()
    int m_colorSize; ///< see colorSize()
};
