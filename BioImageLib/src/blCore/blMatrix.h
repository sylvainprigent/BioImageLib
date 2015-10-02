/// \file blMatrix.h
/// \brief blMatrix class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include "blData.h"
#include "blCoreExport.h"

/// \class blMatrix
/// \brief Cass defining a container for a matrix (in 2D)
class BLCORE_EXPORT blMatrix : public blData{

public:
    /// \fn blMatrix();
    /// \brief Constructor
    blMatrix();
    /// \fn blMatrix(unsigned int rowNum, unsigned int columnNum);
    /// \brief Constructor that allocate the matrix
    /// \param[in] rowNum Number of row
    /// \param[in] columnNum Number of columns
    blMatrix(unsigned int rowNum, unsigned int columnNum);
    /// \fn blMatrix(float * data, unsigned int rowNum, unsigned int columnNum);
    /// \brief Constructor that create a matrix by copying a buffer of data
    /// \param[in] data Buffer of data
    /// \param[in] rowNum Number of row
    /// \param[in] columnNum Number of columns
    blMatrix(float * data, unsigned int rowNum, unsigned int columnNum);
    /// \fn blMatrix(const blMatrix &matrix);
    /// \brief Constructor that create a matrix by copy of an existing matrix
    /// \param[in] matrix Matrix to copy
    blMatrix(const blMatrix &matrix);
    /// \fn virtual ~blMatrix();
    /// \brief Destructor
    virtual ~blMatrix();

public:
    // getters
    /// \fn unsigned int numberOfRows() const;
    /// \brief getter
    /// \return the number of rows
    unsigned int numberOfRows() const;
    /// \fn unsigned int numberOfColumns() const;
    /// \brief getter
    /// \return the number of comumns
    unsigned int numberOfColumns() const;

public:
    //accessors to values
    /// \fn float& operator()(unsigned int i, unsigned int j);
    /// \brief operator
    /// \param[in] i Row index
    /// \param[in] j Column index
    float& operator()(unsigned int i, unsigned int j);
    /// \fn float operator()(unsigned int i, unsigned int j) const;
    /// \brief operator
    /// \param[in] i Row index
    /// \param[in] j Column index
    float operator()(unsigned int i, unsigned int j) const;
    /// \fn float& operator [](unsigned int i);
    /// \brief operator
    /// \param[in] i Coordinate of the value to edit
    float& operator [](unsigned int i);
    /// \fn float operator [](unsigned int i) const;
    /// \brief operator
    /// \param[in] i Coordinate of the value to get
    float operator [](unsigned int i) const;

public:
    // operations
    /// \fn blMatrix operator +(const blMatrix &matrix) const;
    /// \brief operator
    blMatrix operator +(const blMatrix &matrix) const;
    /// \fn blMatrix operator +(const float &value) const;
    /// \brief operator
    blMatrix operator +(const float &value) const;
    /// \fn blMatrix operator -(const blMatrix &matrix) const;
    /// \brief operator
    blMatrix operator -(const blMatrix &matrix) const;
    /// \fn blMatrix operator -(const float &value) const;
    /// \brief operator
    blMatrix operator -(const float &value) const;
    /// \fn blMatrix operator *(const blMatrix &matrix) const;
    /// \brief operator
    blMatrix operator *(const blMatrix &matrix) const;
    /// \fn blMatrix operator *(const float &value) const;
    /// \brief operator
    blMatrix operator *(const float &value) const;
    /// \fn blMatrix operator /(const float &value) const;
    /// \brief operator
    blMatrix operator /(const float &value) const;

public:
    // self operations
    /// \fn blMatrix& operator =(const blMatrix &matrix);
    /// \brief operator
    blMatrix& operator =(const blMatrix &matrix);
    /// \fn blMatrix& operator +=(const blMatrix &matrix);
    /// \brief operator
    blMatrix& operator +=(const blMatrix &matrix);
    /// \fn blMatrix& operator +=(const float &value);
    /// \brief operator
    blMatrix& operator +=(const float &value);
    /// \fn blMatrix& operator -=(const blMatrix &matrix);
    /// \brief operator
    blMatrix& operator -=(const blMatrix &matrix);
    /// \fn blMatrix& operator -=(const float &value);
    /// \brief operator
    blMatrix& operator -=(const float &value);
    /// \fn blMatrix& operator *=(const float &value);
    /// \brief operator
    blMatrix& operator *=(const float &value);
    /// \fn blMatrix& operator /=(const float &value);
    /// \brief operator
    blMatrix& operator /=(const float &value);

public:
    // comparisons
    /// \fn int operator ==(const blMatrix &) const;
    /// \brief operator
    int operator ==(const blMatrix &) const;
    /// \fn int operator !=(const blMatrix &mat) const;
    /// \brief operator
    int operator !=(const blMatrix &mat) const;

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
    /// \param[in] rowNum Number of row
    /// \param[in] columnNum Number of columns
    void allocate(unsigned int rowNum, unsigned int columnNum);
    /// \fn void free();
    /// \brief Free the memory
    void free();

private:
    // data
    unsigned int m_rowNum; ///< Number of row
    unsigned int m_columnNum; ///< Number of columns
    float* m_data; ///< pointer to the matrix buffer
    bool m_isAlreadyAllocated; ///< true if the matrix memory is allocated
};
