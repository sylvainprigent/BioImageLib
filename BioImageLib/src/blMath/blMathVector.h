/// \file blMathVector.h
/// \brief blMathVector class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include <vector>
#include <string>
#include "blMathExport.h"

/// \class  DistancesTypes
/// \brief class  defining static std::strings to nominate distances as input parameter
class BLMATH_EXPORT DistancesTypes{
public:
    static const std::string BHATTACHARYYA; ///< static string to refer to Bhattacharyya distance
    static const std::string MEAN_DIFF; ///< static string to refer to mean diff "distance"
};

/// \class blMathVector
/// \brief Implement usefull common operations on vectors
class BLMATH_EXPORT blMathVector {

 public:
  /// \fn template <typename T> static T norm(const std::vector<T>& vect, const bool& useSqrt = true);
  /// \brief Calculate the norm of a std::vector
  /// \param[in] vect std::vector to process
  /// \param[in] useSqrt if true calculate the norm, if false calculate the square norm
  /// \return calculated norm
  template <typename T>
  static T norm(const std::vector<T>& vect, const bool& useSqrt = true);

  /// \fn static std::vector<int> sortShell( const std::vector<float>& vectIn, std::vector<float> &vect);
  /// \brief sort a std::vector with the shell algorithm
  /// \param[in] vectIn std::vector to sort
  /// \param[out] vect Sorted std::vector
  /// \return Positions correspondances between  vectIn and vect
  static std::vector<int> sortShell( const std::vector<float>& vectIn, std::vector<float> &vect);

 public:
  /// \fn   template<typename T> static T mean(const std::vector<T>& vect);
  /// \return the mean value of vect
  template<typename T>
  static T mean(const std::vector<T>& vect);

  /// \fn static float var(std::vector<float> vect);
  /// \return the variance of vect
  static float var(const std::vector<float>& vect);

  /// \fn template<typename T> static int minPos(const std::vector<T>& vect);
  /// \return the index of the min value of vect
  template<typename T>
  static int minPos(const std::vector<T>& vect);

  /// \fn   template<typename T> static void minMax(const std::vector<T>& vect, T &mini, T &maxi);
  /// \brief Function that calculate the min and the max of a std::vector
  /// \param[in] vect std::vector to analyse
  /// \param[out] mini Min value of vect
  /// \param[out] maxi Max value of vect
  template<typename T>
  static void minMax(const std::vector<T>& vect, T &mini, T &maxi);

  /// \fn template<typename T> static T max(const std::vector<T>& vect);
  /// \return the maximum value in vect
  template<typename T>
  static T max(const std::vector<T>& vect);

  /// \fn template<typename T> static T min(const std::vector<T>& vect);
  /// \return the minimum value in vect
  template<typename T>
  static T min(const std::vector<T>& vect);


  /// \fn static int maxPos(std::vector<float> vect);
  /// \return the index of the min value of vect
  template<typename T>
  static int maxPos(const std::vector<T>& vect);


  /// \fn static void minMatrix(std::vector<std::vector<float> > matrix, std::vector<int> &minPos, float minVal);
  /// \brief Calculate the min of matrix
  /// \param[in] matrix Matrix to analyse
  /// \param[out] minPos Position of the minimum minPos[x][y]
  /// \param[out] minVal Value of the minimum
  static void minMatrix(const std::vector<std::vector<float> >& matrix, std::vector<int> &minPos, float minVal);

  /// \fn float medianAbsoluteDeviation(std::vector<float> vect, float median);
  /// \brief Calculate the median absolute deviation of a std::vector. This version allows not to recalculate the median if you have it
  /// \param[in] vect Input std::vector
  /// \param[in] median Median of vect
  /// \return Median absolute deviation of vect
  static float medianAbsoluteDeviation(std::vector<float> vect, float median);  

  /// \fn static float medianAbsoluteDeviation(std::vector<float> vect);
  /// \brief Calculate the median absolute deviation of a std::vector
  /// \param[in] vect Input std::vector
  /// \return Median absolute deviation of vect
  static float medianAbsoluteDeviation(std::vector<float> vect);

  /// \fn static float median(std::vector<float> &vect);
  /// \brief Calculate the median of a std::vector
  /// \param[in] vect Input std::vector
  /// \return Median vect
  static float median(std::vector<float> &vect);

 public:
  /// \fn static float convolute(std::vector<float> vect1, std::vector<float> vect2, int x);
  /// \brief Function that compute the convolution between vect1 and vect2 with the shift x
  /// \param[in] vect1 first std::vector
  /// \param[in] vect2 second std::vector
  /// \param[in] x Shift value
  /// \return convolution result
  static float convolute(std::vector<float> vect1, std::vector<float> vect2, int x);

  /// \fn static std::vector<float> convolute(std::vector<float> vect1, std::vector<float> vect2);
  /// \brief calculate the convolution between two std::vectors for all shifts
  /// \param[in] vect1 first std::vector
  /// \param[in] vect2 second std::vector
  /// \return convolution result
  static std::vector<float> convolute(std::vector<float> vect1, std::vector<float> vect2);

  /// \fn static template<typename T> std::vector<T> cross(std::vector<T> u, std::vector<T> v)
  /// \brief Function that calculates the cross product between two 3d std::vectors
  template<typename T>
    static std::vector<T> cross(std::vector<T> u, std::vector<T> v);

  /// \fn template<typename T> static std::vector<T> cross(T u1, T u2, T u3, T v1, T v2, T v3)
  /// \brief Function that calculates the cross product between two 3d vectors
  template<typename T>
    static std::vector<T> cross(T u1, T u2, T u3, T v1, T v2, T v3);

};

#include "blMathVector.tpp"
