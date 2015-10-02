/// \file blVectorNumber2D.h
/// \brief blVectorNumber2D class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include "blData.h"
#include <vector>
#include "blCoreExport.h"

/// \class blVectorNumber2D
/// \brief Class defining a 2D vector container.
/// This contains float vector 2D using 2 std::vector
class BLCORE_EXPORT blVectorNumber2D : public blData{

public:
    /// \fn blVectorNumber2D();
    /// \brief Constructor
    blVectorNumber2D();
    /// \fn virtual ~blVectorNumber2D();
    /// \brief Destructor
    virtual ~blVectorNumber2D();

public:
    /// \fn virtual std::string name();
    /// \return the name of the data
    virtual std::string name();

public:
    /// \fn void setData(const std::vector< std::vector<float> > &data);
    /// \param[in] data Vector to store
    void setData(const std::vector< std::vector<float> > &data);

    /// \fn const std::vector< std::vector<float> >& data();
    /// \return stored vector
    const std::vector< std::vector<float> >& data();

protected:
    std::vector< std::vector<float> > m_data; ///< content storage
};
