/// \file blCast.h
/// \brief blCast class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include <sstream>

/// \class blCast
/// \brief cast strings and numbers
class blCast{

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
        // créer un flux à partir de la chaîne donnée
        std::istringstream iss( Str );
        // tenter la conversion vers Dest
        return iss >> Dest != 0;
    }
};

