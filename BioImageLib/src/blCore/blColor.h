/// \file blColor.h
/// \brief blColor class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include <vector>

/// \class blColor
/// \brief class allowing to get a random color
class blColor{

public:
    /// \fn static std::vector<int> GetRandRGB();
    /// \return a random RGB color
    static std::vector<int> GetRandRGB();
    /// \fn static std::vector<int> hsv_to_rgb(float h, float s, float v);
    /// \brief Convert an HSV color into an RGB color
    /// \param[in] h H value
    /// \param[in] s S value
    /// \param[in] v V value
    static std::vector<int> hsv_to_rgb(float h, float s, float v);
};

