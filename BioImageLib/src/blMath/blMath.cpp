/// \file blMath.cpp
/// \brief blMath class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blMath.h"

#include <algorithm>
#include <cmath>
#include "blRand/blRandomGeneratorPM.h"

bool blMath::isNan(const float& number)
{
#if WIN32 || WIN64
  return _isnan(number);
#else
  if ( number != number )
    {
      return true;
    }
  else
    {
      return false;
    }
#endif
}

float blMath::randSign()
{
  float value;
  value = blRandomGeneratorPM::rand();
  if (value < 0.5)
    {
      return 1;
    }
  else
    {
      return -1;
    }
}

float  blMath::rand10_1(){
  float value;
  value = blRandomGeneratorPM::rand();
  if (value < 0.33)
    return -1;
  else if (value >= 0.33 && value < 0.66){
    return 0;
  }
  else
    return 1;
}
