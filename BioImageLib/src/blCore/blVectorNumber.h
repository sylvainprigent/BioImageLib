/// \file blVectorNumber.h
/// \brief blVectorNumber class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include "blData.h"
#include <vector>
#include "blCoreExport.h"

/// \class blVectorNumber
/// \brief Class defining a vector container.
/// This contains float vector from the std::vector
class BLCORE_EXPORT blVectorNumber : public blData{

public:
    /// \fn blVectorNumber();
    /// \brief Constructor
    blVectorNumber();
    /// \fn virtual ~blVectorNumber();
    /// \brief Destructor
    virtual ~blVectorNumber();

public:
    /// \fn virtual std::string name();
    /// \return the name of the data
    virtual std::string name();

public:
    /// \fn void setData(const std::vector<float> &data);
    /// \param[in] data Vector to store
    void setData(const std::vector<float> &data);

    /// \fn const std::vector<float>& data();
    /// \return stored vector
    const std::vector<float>& data();

protected:
    std::vector<float> m_data; ///< Content storage
};
