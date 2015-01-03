/// \file blPixel.h
/// \brief blPixel class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

/// \class blPixel
/// \brief Container for a pixel index
class blPixel{

public:
    /// \fn blPixel();
    /// \brief Constructor
    blPixel();
    /// \fn blPixel(int x, int y);
    /// \brief Constructor
    /// \param[in] x X position of the pixel
    /// \param[in] y Y position of the pixel
    blPixel(int x, int y);
    /// \fn blPixel(int x, int y, int z);
    /// \brief Constructor
    /// \param[in] x X position of the pixel
    /// \param[in] y Y position of the pixel
    /// \param[in] z Z position of the pixel
    blPixel(int x, int y, int z);

    /// \fn int x();
    /// \return the X position of the pixel
    int x();
    /// \fn int y();
    /// \return the Y position of the pixel
    int y();
    /// \fn int z();
    /// \return the Z position of the pixel
    int z();
    /// \fn void setX(int x);
    /// \param[in] x the X position of the pixel
    void setX(int x);
    /// \fn void setY(int y);
    /// \param[in] y the Y position of the pixel
    void setY(int y);
    /// \fn void setZ(int z);
    /// \param[in] z the Z position of the pixel
    void setZ(int z);

protected:
    int m_x; ///< position x
    int m_y; ///< position y
    int m_z; ///< position z

};
