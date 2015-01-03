/// \file blData.h
/// \brief blData class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include <string>

/// \class blData
/// \brief All the class implementing a bioImagelib data must heritate from this class
class blData{

public:
    /// \fn blData();
    /// \brief Constructor
    blData();
    /// \fn virtual ~blData();
    /// \brief Destructor
    virtual ~blData();

public:
    /// \fn virtual std::string name();
    /// \return the name of the data
    virtual std::string name();
};
