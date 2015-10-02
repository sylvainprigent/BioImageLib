/// \file blTableNumber.h
/// \brief blTableNumber class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include "blData.h"
#include <vector>
#include "blCoreExport.h"

/// \class blTableNumber
/// \brief Class defining a table container.
/// This contains float number from 2D std::vector
class BLCORE_EXPORT blTableNumber : public blData{

public:
    /// \fn blTableNumber();
    /// \brief Constructor
    blTableNumber();
    /// \fn virtual ~blTableNumber();
    /// \brief Destructor
    virtual ~blTableNumber();

public:
    /// \fn virtual std::string name();
    /// \return the name of the data
    virtual std::string name();

public:
    /// \fn void setData(const std::vector<float> &data);
    /// \param[in] data Vector to store
    void setData(const std::vector<std::vector< float > >&data);

    /// \fn const std::vector<std::vector< float > >& data();
    /// \return stored vector
    const std::vector<std::vector< float > >& data();

    /// \fn void setHeaders(const std::vector<std::string >& headers);
    /// \param[in] headers Headers describing each colunm data
    void setHeaders(const std::vector<std::string >& headers);

    /// \fn const std::vector<std::string >& headers();
    /// \return headers describing each colunm data
    const std::vector<std::string >& headers();

    /// \fn void setTitle(std::string title);
    /// \param[in] title
    void setTitle(std::string title);

    /// \fn std::string title();
    /// \return the title of the table
    std::string title();

public:
    /// \fn void printConsole();
    /// \brief Print the table in console
    void printConsole();

public:
    // IO
    /// \fn void load(std::string filePath);
    /// \brief load the table from a file
    /// \param[in] filePath File to store the data
    void load(std::string filePath);
    /// \fn void save(std::string filePath);
    /// \brief Save the table to a file
    /// \param[in] filePath File to store the data
    void save(std::string filePath);

protected:
    std::string m_title; ///< title
    std::vector<std::string> m_headers; ///< header
    std::vector<std::vector< float > > m_data; ///< content
};
