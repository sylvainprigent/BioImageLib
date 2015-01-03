/// \file blMathVector.cpp
/// \brief blMathVector class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blMathVector.h"

#include <cmath>
#include <algorithm>
#include <string>

using namespace std;

const string DistancesTypes::BHATTACHARYYA = "bhattacharyya";
const string DistancesTypes::MEAN_DIFF = "mean_diff";

std::vector<int> blMathVector::sortShell(const std::vector<float>& vectIn, std::vector<float> &vect)
{
    //std::vector<double> vect;
    std::vector<int> pos;
    pos.resize(vectIn.size());
    for ( int i = 0 ; i < int(pos.size()) ; i++)
        pos[i] = i;
    int teblos;

    vect = vectIn;
    int n = int(vect.size());
    int inc = floor((float(n)/2.0)+0.5);
    int j;
    float temp;
    while ( inc > 0 )
    {
        for ( int i = inc ; i < n ; i++)
        {
            temp = vect[i];
            teblos = pos[i];
            j = i;
            while ( (j >= inc) && (vect[j-inc] > temp) )
            {
                vect[j] = vect[j-inc];
                pos[j] = pos[j-inc];
                j = j-inc;
            }
            vect[j] = temp;
            pos[j] = teblos;
        }
        inc = floor((inc / 2.2)+0.5);
    }
    return pos;
}

float blMathVector::var(const std::vector<float>& vect)
{
    float mean = 0.0, var = 0.0;
    for(std::vector<float>::const_iterator it = vect.begin(); it!=vect.end(); ++it)
    {
        var += pow(*it, 2);
        mean += *it;
    }
    int n = vect.size();
    var=(var-mean*mean/n)/(n-1);
    return var;
}

void blMathVector::minMatrix(const std::vector<std::vector<float> >& matrix, std::vector<int> &minPos, float minVal)
{
    minPos.resize(2); minPos[0] = 0; minPos[1] = 0;
    minVal = matrix[0][0];
    float val;
    for (unsigned int i = 0 ; i<matrix.size() ; ++i)
    {
        for (unsigned int j=0 ; j<matrix[i].size() ; ++j)
        {
            val = matrix[i][j];
            if ( val < minVal)
            {
                minVal = val;
                minPos[0] = i;
                minPos[1] = j;
            }
        }
    }
}

float blMathVector::convolute(std::vector<float> vect1, std::vector<float> vect2, int x)
{
    float conv = 0.0;
    int length = vect1.size();
    for (int t = x ; t < length ; ++t )
    {
        conv += vect1[t]*vect2[t-x];
    }
    int offset = length-1 - (x-1);
    for (int t = 0 ; t < x ; ++t)
    {
        conv += vect1[t]*vect2[offset + t];
    }
    return conv;
}

std::vector<float> blMathVector::convolute(std::vector<float> vect1, std::vector<float> vect2)
{
    std::vector<float> conv;
    conv.resize(vect1.size());
    for (unsigned int x = 0 ; x < vect1.size() ; ++x)
    {
        conv[x] = convolute(vect1, vect2, x);
    }
    return conv;
}

float blMathVector::median(vector<float> &vect){

    vector<float> vect_sorted;
    blMathVector::sortShell(vect, vect_sorted);

    return vect_sorted[ int( floor(vect_sorted.size()/2 +0.5) ) ];
}

float blMathVector::medianAbsoluteDeviation(std::vector<float> vect){
    vector<float> vect_sorted;
    blMathVector::sortShell(vect, vect_sorted);
    float median =  vect_sorted[ int( floor(vect_sorted.size()/2 +0.5) ) ];

    return medianAbsoluteDeviation(vect, median);
}

float blMathVector::medianAbsoluteDeviation(std::vector<float> vect, float median){

    for (unsigned int i = 0 ; i < vect.size() ; ++i){
        vect[i] = fabs(vect[i] - median);
    }

    vector<float> vect_sorted;
    blMathVector::sortShell(vect, vect_sorted);

    return vect_sorted[ int( floor(vect_sorted.size()/2 +0.5) ) ];
}
