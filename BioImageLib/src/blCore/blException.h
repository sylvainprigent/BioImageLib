/// \file blException.h
/// \brief blException class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2012

#pragma once

#include <iostream>
#include <sstream>
#include <exception>
#include "blCoreExport.h"

/// \class blException
/// \brief class defining the exeptions used in the code
/// heritate from the std::exception
class BLCORE_EXPORT blException : public std::exception
{
public:
    /// \fn blException( const char * Msg )
    /// \brief Constructor
    /// \param[in] Msg Message to show
    blException( const char * Msg )
    {
        std::ostringstream oss;
        oss << "Error:" << Msg;
        this->msg = oss.str();
    }

    /// \fn virtual ~blException() throw()
    ///  \brief Destructor
    virtual ~blException() throw()
    {

    }

    /// \fn virtual const char * what() const throw()
    /// \brief reimplement the what method
    virtual const char * what() const throw()
    {
        return this->msg.c_str();
    }

private:
    std::string msg; ///< message to show
};
