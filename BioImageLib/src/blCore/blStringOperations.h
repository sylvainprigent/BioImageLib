/// \file blStringOperations.h
/// \brief blStringOperations class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include <string>
#include "blCoreExport.h"

/// \class blStringOperations
/// \brief Define operations on strings
class BLCORE_EXPORT blStringOperations{

public:
    /// \fn static std::string getFileNameFromPath(std::string file);
    /// \brief Extract a file name from a file path
    /// \param[in] file Path of the file
    /// return the file name
    static std::string getFileNameFromPath(std::string file);
};
