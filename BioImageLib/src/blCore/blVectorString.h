/// \file blVectorString.h
/// \brief blVectorString class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include "blData.h"
#include <vector>
#include "blCoreExport.h"

/// \class blVectorString
/// \brief Class defining a vector container.
/// This contains string vector using the std::vector
class BLCORE_EXPORT blVectorString : public blData{

public:
    /// \fn blVectorString();
    /// \brief Constructor
    blVectorString();
    /// \fn virtual ~blVectorString();
    /// \brief Destructor
    virtual ~blVectorString();

public:
    /// \fn virtual std::string name();
    /// \return the name of the data
    virtual std::string name();

public:
    /// \fn void setData(const std::vector<std::string> &data);
    /// \param[in] data Vector to store
    void setData(const std::vector<std::string> &data);

    /// \fn std::vector<std::string>& data();
    /// \return stored vector
    std::vector<std::string>& data();

protected:
    std::vector<std::string> m_data; ///< content
};
