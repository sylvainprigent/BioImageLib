/// \file blDistance.h
/// \brief blDistance class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include <vector>

/// \class blDistance
/// \brief Definine static functions that implement
/// mathematical distances between points of distributions
class blDistance{

public:
    /// \fn static float euclidian_square(float ax, float ay, float bx, float by);
    /// \brief Calculate the suqare of the euclidian distance betwen 2D points a and b
    /// \param[in] ax x coordinate of a
    /// \param[in] ay y coordinate of a
    /// \param[in] bx x coordinate of b
    /// \param[in] by y coordinate of b
    /// \return the square euclidian distance
    static float euclidian_square(float ax, float ay, float bx, float by);

    /// \fn static float bhattacharyya(std::vector<float> vect1, std::vector<float> vect2);
    /// \brief Calculate the bhattacharyya modified (not homogeneous) distance between two std::vectors
    /// \param[in] vect1 first std::vector
    /// \param[in] vect2 second std::vector
    /// \return calculated distance
    static float bhattacharyya(const std::vector<float>& vect1, const std::vector<float>& vect2);

    /// \fn static float bhattacharyyaMedian(std::vector<float> vect1, std::vector<float> vect2);
    /// \brief Calculate the bhattacharyya modified (not homogeneous, and use median in stead of mean) distance between two std::vectors
    /// \param[in] vect1 first std::vector
    /// \param[in] vect2 second std::vector
    /// \return calculated distance
    static float bhattacharyyaMedian( std::vector<float> vect1, std::vector<float> vect2);
};


/// \class blFrechetDistance
/// \brief Calculate the Frechet distance between two curves
class blFrechetDistance{

public:
   /// \fn blFrechetDistance(std::vector<std::vector<float> > curve_1, std::vector<std::vector<float> > curve_2, int dimension);
   /// \brief Constructor
   /// \param[in] curve_1 First curve to compare
   /// \param[in] curve_2 second curve to compare
   /// \param[in] dimension dimension of the curves
   blFrechetDistance(std::vector<std::vector<float> > curve_1, std::vector<std::vector<float> > curve_2, int dimension);
   /// \fn float Compute();
   /// \brief Run the distance calculation
   float Compute();

private:
    // attributs
    std::vector<std::vector<float> > m_CA;
    int m_dimension;
    std::vector<std::vector<float> > m_curve_1;
    std::vector<std::vector<float> > m_curve_2;

    // methods
    float c(int i, int j);
    float dfcn(int point_curve_1, int point_curve_2);

};
