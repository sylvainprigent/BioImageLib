/// \file blImageList.h
/// \brief blImageList class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include "blData.h"
#include "blImage.h"
#include <vector>
#include "blCoreExport.h"

/// \class blImageList
/// \brief Class defining a vector container.
/// This contains string vector using the std::vector
class BLCORE_EXPORT blImageList : public blData{

public:
    /// \fn blImageList();
    /// \brief Constructor
    blImageList();
    /// \fn virtual ~blImageList();
    /// \brief Destructor
    virtual ~blImageList();

public:
    /// \fn virtual std::string name();
    /// \return the name of the data
    virtual std::string name();

public:
    /// \fn void setData(const std::vector<blImage*> &data);
    /// \param[in] data Vector to store
    void setData(const std::vector<blImage*> &data);

    /// \fn std::vector<blImage*>& data();;
    /// \return stored vector
    std::vector<blImage*>& data();

    /// \fn unsigned int length();
    /// \return the number of image in the list
    unsigned int length();

    /// \fn blImage* imageAt(unsigned int i);
    /// \param[in] i ith image in the list
    /// \return the pointer to the ith image in the list
    blImage* imageAt(unsigned int i);

protected:
    std::vector<blImage*> m_data; ///< list of image pointer
};
