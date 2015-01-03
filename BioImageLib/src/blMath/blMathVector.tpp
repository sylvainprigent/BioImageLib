/// \file blMathVector.tpp
/// \brief blMathVector class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include <algorithm>
#include <numeric>


template<typename T>
std::vector<T> blMathVector::cross(std::vector<T> u, std::vector<T> v)
{
  std::vector<T> output; output.resize(3);
  output[0] = u[1]*v[2]-u[2]*v[1];
  output[1] = u[2]*v[0]-u[0]*v[2];
  output[2] = u[0]*v[1]-u[1]*v[0];
  return output;
}

template<typename T>
std::vector<T> blMathVector::cross(T u1, T u2, T u3, T v1, T v2, T v3)
{
  std::vector<T> output; output.resize(3);
  output[0] = u2*v3-u3*v2;
  output[1] = u3*v1-u1*v3;
  output[2] = u1*v2-u2*v1;
  return output;
}

template<typename T>
T blMathVector::norm(const std::vector<T>& vect, const bool& useSqrt){
  T norm = 0.0;
  for (unsigned int i = 0 ; i < vect.size() ; ++i){
    norm += vect[i]*vect[i];
  }
  if (useSqrt){
    return sqrt(norm);
  }
  else{
    return norm;
  }
}

template<typename T>
T blMathVector::mean(const std::vector<T>& vect)
{
  return std::accumulate(vect.begin(), vect.end(), 0.0) / vect.size();
}

template<typename T>
void blMathVector::minMax(const std::vector<T>& vect, T &mini, T &maxi)
{
  mini = *std::min_element(vect.begin(), vect.end());
  maxi = *std::max_element(vect.begin(), vect.end());
}

template<typename T>
T blMathVector::max(const std::vector<T>& vect)
{
  T maxi = *std::max_element(vect.begin(), vect.end());
  return maxi;
}

template<typename T>
T blMathVector::min(const std::vector<T>& vect)
{
  T mini = *std::min_element(vect.begin(), vect.end());
  return mini;
}

template<typename T>
int blMathVector::maxPos(const std::vector<T>& vect)
{
  typename std::vector<T>::const_iterator maxi = std::max_element(vect.begin(), vect.end());
  return std::distance(vect.begin(), maxi);
}

template<typename T>
int blMathVector::minPos(const std::vector<T>& vect)
{
  typename std::vector<T>::const_iterator mini = std::min_element(vect.begin(), vect.end());
  return std::distance(vect.begin(), mini);
}
