/// \file blMatrix3D.h
/// \brief blMatrix3D class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include "blData.h"

/// \class blMatrix3D
/// \brief Cass defining a container for a matrix (in 2D)
class blMatrix3D : public blData{

public:
    /// \fn blMatrix3D();
    /// \brief Constructor
    blMatrix3D();
    /// \fn blMatrix3D(unsigned int dimX, unsigned int dimY, unsigned int dimZ);
    /// \brief Constructor that allocate the matrix
    /// \param[in] dimX Number of row
    /// \param[in] dimY Number of columns
    /// \param[in] dimZ Number of slices
    blMatrix3D(unsigned int dimX, unsigned int dimY, unsigned int dimZ);
    /// \fn blMatrix3D(float * data, unsigned int dimX, unsigned int dimY, unsigned int dimZ);
    /// \brief Constructor that create a matrix by copying a buffer of data
    /// \param[in] data Buffer of data
    /// \param[in] dimX Number of row
    /// \param[in] dimY Number of columns
    /// \param[in] dimZ Number of slices
    blMatrix3D(float * data, unsigned int dimX, unsigned int dimY, unsigned int dimZ);
    /// \fn blMatrix3D(const blMatrix3D &matrix);
    /// \brief Constructor that create a matrix by copy of an existing matrix
    /// \param[in] matrix Matrix to copy
    blMatrix3D(const blMatrix3D &matrix);
    /// \fn virtual ~blMatrix3D();
    /// \brief Destructor
    virtual ~blMatrix3D();

public:
    // getters
    /// \fn unsigned int dimX() const;
    /// \brief getter
    /// \return the dimestion in the x axis
    unsigned int dimX() const;
    /// \fn unsigned int dimY() const;
    /// \brief getter
    /// \return the dimestion in the y axis
    unsigned int dimY() const;
    /// \fn unsigned int dimZ() const;
    /// \brief getter
    /// \return the dimestion in the z axis
    unsigned int dimZ() const;

public:
    //accessors to values
    /// \fn float& operator()(unsigned int i, unsigned int j, unsigned int k);
    /// \brief accessor
    /// \param[in] i coordinate of the value in x
    /// \param[in] j coordinate of the value in y
    /// \param[in] k coordinate of the value in k
    float& operator()(unsigned int i, unsigned int j, unsigned int k);
    /// \fn float operator()(unsigned int i, unsigned int j, unsigned int k) const;
    /// \brief accessor
    /// \param[in] i coordinate of the value in x
    /// \param[in] j coordinate of the value in y
    /// \param[in] k coordinate of the value in k
    float operator()(unsigned int i, unsigned int j, unsigned int k) const;
    /// \fn float& operator [](unsigned int i);
    /// \brief accessor
    /// \param[in] i Coordinate of the value to edit
    float& operator [](unsigned int i);
    /// \fn float operator [](unsigned int i) const;
    /// \brief accessor
    /// \param[in] i Coordinate of the value to get
    float operator [](unsigned int i) const;

public:
    // operations
    /// \fn blMatrix3D operator +(const blMatrix3D &matrix) const;
    /// \brief operator
    blMatrix3D operator +(const blMatrix3D &matrix) const;
    /// \fn blMatrix3D operator +(const float &value) const;
    /// \brief operator
    blMatrix3D operator +(const float &value) const;
    /// \fn blMatrix3D operator -(const blMatrix3D &matrix) const;
    /// \brief operator
    blMatrix3D operator -(const blMatrix3D &matrix) const;
    /// \fn blMatrix3D operator -(const float &value) const;
    /// \brief operator
    blMatrix3D operator -(const float &value) const;
    /// \fn blMatrix3D operator *(const float &value) const;
    /// \brief operator
    blMatrix3D operator *(const float &value) const;
    /// \fn blMatrix3D operator /(const float &value) const;
    /// \brief operator
    blMatrix3D operator /(const float &value) const;

public:
    // self operations
    /// \fn blMatrix3D& operator =(const blMatrix3D &matrix);
    /// \brief operator
    blMatrix3D& operator =(const blMatrix3D &matrix);
    /// \fn blMatrix3D& operator +=(const blMatrix3D &matrix);
    /// \brief operator
    blMatrix3D& operator +=(const blMatrix3D &matrix);
    /// \fn blMatrix3D& operator +=(const float &value);
    /// \brief operator
    blMatrix3D& operator +=(const float &value);
    /// \fn blMatrix3D& operator -=(const blMatrix3D &matrix);
    /// \brief operator
    blMatrix3D& operator -=(const blMatrix3D &matrix);
    /// \fn blMatrix3D& operator -=(const float &value);
    /// \brief operator
    blMatrix3D& operator -=(const float &value);
    /// \fn blMatrix3D& operator *=(const float &value);
    /// \brief operator
    blMatrix3D& operator *=(const float &value);
    /// \fn blMatrix3D& operator /=(const float &value);
    /// \brief operator
    blMatrix3D& operator /=(const float &value);


public:
    // comparisons
    /// \fn int operator ==(const blMatrix3D &) const;
    /// \brief Comparison operator
    int operator ==(const blMatrix3D &) const;
    /// \fn int operator !=(const blMatrix3D &mat) const;
    /// \brief Comparison operator
    int operator !=(const blMatrix3D &mat) const;

public:
    /// \fn virtual std::string name();
    /// \return the name of the data
    virtual std::string name();

private:
    /// \fn void reset();
    /// \brief Empty the matrix
    void reset();
    /// \fn void allocate(unsigned int dimX, unsigned int dimY, unsigned int dimZ);
    /// \brief Allocate the memory for the matrix
    /// \param[in] dimX Dimestion in the x axis
    /// \param[in] dimY Dimestion in the y axis
    /// /// \param[in] dimZ Dimestion in the z axis
    void allocate(unsigned int dimX, unsigned int dimY, unsigned int dimZ);
    /// \fn void free();
    /// \brief Free the memory
    void free();

private:
    // data
    unsigned int m_dimX; ///< dimestion in the x axis
    unsigned int m_dimY; ///< dimestion in the y axis
    unsigned int m_dimZ; ///< dimestion in the z axis
    float* m_data; ///< pointer to the matrix content
    bool m_isAlreadyAllocated; ///< true if the matrix memory is allocated
};
