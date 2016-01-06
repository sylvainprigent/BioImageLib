/// \file blCast.h
/// \brief blCast class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include <sstream>
#include "blCoreExport.h"
using namespace std;

/// \class blCast
/// \brief cast strings and numbers
class BLCORE_EXPORT blCast{

public:
    /// \fn template<typename T> std::string to_string( const T & Value )
    /// \brief Funtion that convert any type into string
    template<typename T>
    static std::string to_string( const T & Value )
    {
        std::ostringstream oss;
        oss << Value;
        return oss.str();
    }

    /// \fn template<typename T> static bool from_string( const std::string & Str, T & Dest )
    /// \brief Convert a string to a number
    template<typename T>
    static bool from_string( const std::string & Str, T & Dest )
    {
        //// créer un flux à partir de la chaîne donnée
        istringstream iss( Str );
        //// tenter la conversion vers Dest
        iss >> Dest;
        return true;
    }
};

