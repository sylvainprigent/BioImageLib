/// \file blDistance.cpp
/// \brief blDistance class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blDistance.h"
#include "math.h"
#include "blMathVector.h"

using namespace std;

// ////////////////////////////////////////////////////////////// //
//                      blDistance
// ////////////////////////////////////////////////////////////// //
float blDistance::euclidian_square(float ax, float ay, float bx, float by){
  return pow(ax - bx, 2) + pow(ay-by, 2);
}

float blDistance::bhattacharyyaMedian( vector<float> vect1, vector<float> vect2){
    float mean1 = blMathVector::median(vect1);
    float mean2 = blMathVector::median(vect2);
    float var1 = blMathVector::medianAbsoluteDeviation(vect1, mean1);
    float var2 = blMathVector::medianAbsoluteDeviation(vect2, mean2);
    var1 = var1*var1;
    var2 = var2*var2;

    float epsilon = 0.0000001;
    if (var1 < epsilon){
        var1 += epsilon;
    }
    if (var2 < epsilon){
        var2 += epsilon;
    }
    return  pow(mean1 - mean2,2)/(4*sqrt(var1+var2)) + 0.5* log((var1+var2) / (2*sqrt(sqrt(var1)*sqrt(var2))));
    //return  pow(mean1 - mean2,2)/(4*(var1+var2)) + 0.5*log( (var1+var2) / (2*sqrt(var1*var2)) );
}

float blDistance::bhattacharyya(const std::vector<float>& vect1, const std::vector<float>& vect2){
    float mean1 = blMathVector::mean(vect1);
    float mean2 = blMathVector::mean(vect2);
    float var1 = blMathVector::var(vect1);
    float var2 = blMathVector::var(vect2);

    float epsilon = 0.0000001;
    if (var1 < epsilon)
    {
        var1 += epsilon;
    }
    if (var2 < epsilon)
    {
        var2 += epsilon;
    }
    return  pow(mean1 - mean2,2)/(4*sqrt(var1+var2)) + 0.5* log((var1+var2) / (2*sqrt(sqrt(var1)*sqrt(var2))));
}


// ////////////////////////////////////////////////////////////// //
//                      blFrechetDistance
// ////////////////////////////////////////////////////////////// //
blFrechetDistance::blFrechetDistance(vector<vector<float> > curve_1, vector<vector<float> > curve_2, int dimension){
    m_curve_1 = curve_1;
    m_curve_2 = curve_2;
    m_dimension = dimension;
}

float blFrechetDistance::Compute(){

    // lenghts of the curves
    int l_curve_1 = m_curve_1.size();
    int l_curve_2 = m_curve_2.size();
    //cout << "l_curve_1 = " << l_curve_1 << endl;
    //cout << "l_curve_2 = " << l_curve_2 << endl;


    // initialize CA to a matrix of -1s
    m_CA.resize(l_curve_1);
    for (int m = 0 ; m < l_curve_1 ; ++m){
        m_CA[m].resize(l_curve_2);
        for (int n = 0 ; n < l_curve_2 ; ++n){
            m_CA[m][n] = -1;
        }
    }

    // measure value
    return c(l_curve_1-1, l_curve_2-1);

}

float blFrechetDistance::c(int i, int j){

    //cout << "c on " << i << ", " << j << endl;
    float CAij = 0.0;
    // coupling search function
    if (m_CA[i][j] > -1){
        // don't update CA in this case
        CAij = m_CA[i][j];
    }
    else if ( i==0 && j==0 ){
        m_CA[i][j] = dfcn(0,0);     // update the CA permanent
        CAij = m_CA[i][j];                    // set the current relevant value
    }
    else if (i>0 && j==0){
        m_CA[i][j] = max( c(i-1,0), dfcn(i,0) );
        CAij = m_CA[i][j];
    }
    else if (i==0 && j>0){
        m_CA[i][j] = max( c(0,j-1), dfcn(0,j) );
        CAij = m_CA[i][j];
    }
    else if (i>0 && j>0){

        float mini = c(i-1,j);
        float inter = c(i-1,j-1);
        if (inter < mini){
            mini = inter;
        }
        inter = c(i,j-1);
        if (inter < mini){
            mini = inter;
        }

        m_CA[i][j] = max( mini, dfcn(i,j) );
        CAij = m_CA[i][j];
    }
    else{
        m_CA[i][j] = 999999.9;
    }
    return CAij;
}

float blFrechetDistance::dfcn(int point_curve_1, int point_curve_2){
    float dist = 0;
    for (int i = 0 ; i < m_dimension ; ++i){
        dist += pow(m_curve_1[point_curve_1][i] - m_curve_2[point_curve_2][i], 2);
    }
    //cout << "dist = " << sqrt(dist) << endl;
    return sqrt(dist);
}
