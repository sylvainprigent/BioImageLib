/// \file blTxtIO.h
/// \brief blTxtIO class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2013

#pragma once

#include <vector>
#include <string>

#include "blTxtIOExport.h"

/// \class blTxtIO
/// \brief class BLTXTIO_EXPORT containing static function to load/write CSV type of data into txt files
class BLTXTIO_EXPORT blCsvIO{

public:
    /// \fn static std::vector<double> loadArray(std::string filePath, std::string separator = ',');
    /// \brief Load a csv file into a double vector
    /// \param[in] filePath File to read
    /// \param[in] separater Data separator for the CSV format
    /// \returns the data vector
    static std::vector<double> loadArray(std::string filePath, std::string separator = ",");

    /// \fn static void saveNumber(std::string filePath, double number);
    /// \brief Save a number into a file
    /// \param[in] filePath File to write
    /// \param[in] number Number to write
    static void saveNumber(std::string filePath, double number);
};
