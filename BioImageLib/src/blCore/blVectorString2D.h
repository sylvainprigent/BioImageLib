/// \file blVectorString2D.h
/// \brief blVectorString2D class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include "blData.h"
#include <vector>

/// \class blVectorString2D
/// \brief Class defining a vector container.
/// This contains string 2D vector using two std::vector
class blVectorString2D : public blData{

public:
    /// \fn blVectorString2D();
    /// \brief Constructor
    blVectorString2D();
    /// \fn virtual ~blVectorString2D();
    /// \brief Destructor
    virtual ~blVectorString2D();

public:
    /// \fn virtual std::string name();
    /// \return the name of the data
    virtual std::string name();

public:
    /// \fn void setData(const std::vector< std::vector<std::string> > &data);
    /// \param[in] data Vector to store
    void setData(const std::vector< std::vector<std::string> > &data);

    /// \fn std::vector< std::vector<std::string> >& data();
    /// \return stored vector
    std::vector< std::vector<std::string> >& data();

protected:
    std::vector< std::vector<std::string> > m_data; ///< content
};
