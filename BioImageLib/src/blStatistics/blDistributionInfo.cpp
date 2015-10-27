/// \file blDistributionInfo.h
/// \brief blDistributionInfo class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#include "blDistributionInfo.h"
#include <algorithm>
#include <math.h>

blDistributionInfo::blDistributionInfo(){

    m_median = 0.0;
    m_Q1 = 0.0;
    m_Q3 = 0.0;
    m_IQR = 0.0;
    m_min = 0.0;
    m_max = 0.0;
    m_mean = 0.0;
    m_var = 0.0;
}

// setters
void blDistributionInfo::setSamples(std::vector<float> samples){
    m_samples = samples;
}

// getters
float blDistributionInfo::median(){
    return m_median;
}

float blDistributionInfo::Q1(){
    return m_Q1;
}

float blDistributionInfo::Q3(){
    return m_Q3;
}

float blDistributionInfo::IQR(){
    return m_IQR;
}

float blDistributionInfo::min(){
    return m_min;
}

float blDistributionInfo::max(){
    return m_max;
}

float blDistributionInfo::mean(){
    return m_mean;
}

float blDistributionInfo::var(){
    return m_var;
}

void blDistributionInfo::run(){

    // sort the samples
    std::sort(m_samples.begin(), m_samples.end());
    int count = int(m_samples.size());

    // comput stats
    m_min = m_samples.at(0);
    m_max = m_samples.at(count-1);
    m_median = this->findQuartile(0, count);
    m_Q1 = this->findQuartile(0, count / 2);
    m_Q3 = this->findQuartile(count / 2 + (count % 2), count);
    m_IQR = m_Q3-m_Q1;

    // mean and variance
    this->computeMeanVar();

}

void blDistributionInfo::computeMeanVar(){
    m_mean = 0.0, m_var = 0.0;
    for(std::vector<float>::const_iterator it = m_samples.begin(); it!=m_samples.end(); ++it)
    {
        m_var += pow(*it, 2);
        m_mean += *it;
    }
    int n = int(m_samples.size());
    m_var=(m_var-m_mean*m_mean/n)/(n-1);
    m_mean /= n;
}

float blDistributionInfo::findQuartile(int begin, int end){

    int count = end - begin;
    if (count % 2) {
        return m_samples.at(count / 2 + begin);
    }

    float right = m_samples.at(count / 2 + begin);
    float left = m_samples.at(count / 2 - 1 + begin);
    return (right + left) / float(2.0);
}
