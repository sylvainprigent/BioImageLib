/// \file blCastImage.h
/// \brief blCastImage class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include <blCore>

/// \class blCastImage
/// \brief Define the interface of the classes
/// used to cast an image from one type to another
class blCastImage : public blProcess{

public:
    /// \fn blCastImage();
    /// \brief Constructor
    blCastImage();
    /// \fn virtual ~blCastImage();
    /// \brief Destructor
    virtual ~blCastImage();

public:
    /// \fn virtual std::string name();
    /// \brief Reimplemented from blProcess
    virtual std::string name();

public:
    /// \fn void setInput(blImage *image);
    /// \param[in] image Image to cast
    void setInput(blImage *image);
    /// \fn virtual void run() = 0;
    /// \brief Implement this function to create a new
    /// cast filter
    virtual void run() = 0;
    /// \fn blImage* output();
    /// \return a pointer to the casted image
    blImage* output();

protected:
    blImage* m_inputImage; ///< input image
    blImage* m_outputImage; ///< output image
};
