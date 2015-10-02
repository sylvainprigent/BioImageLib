/// \file blDistributionInfo.h
/// \brief blDistributionInfo class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2015

#pragma once

#include <vector>

#include "blStatisticsExport.h"


/// \class blDistributionInfo
/// \brief Class computing the informations of a distribution: min, max, mean , var quartiles
class BLSTATISTICS_EXPORT blDistributionInfo{

public:

    /// \brief blDistributionInfo
    /// \param samples The distribution samples
    blDistributionInfo();
    /// \brief run the stats calculation
    void run();

public:
    // setters
    void setSamples(std::vector<float> samples);

    // getters
    float median();
    float Q1();
    float Q3();
    float IQR();
    float min();
    float max();
    float mean();
    float var();

private:
    /// \brief Calculates quartiles
    /// \param begin Index where the quartile starts
    /// \param end Index where the quartile ends
    /// \return the quartile value
    float findQuartile(int begin, int end);
    /// \brief calculate mean and variance of the distribution
    void computeMeanVar();

private:
    std::vector<float> m_samples; ///< samples
    float m_median; ///< median
    float m_Q1; ///< first quartile
    float m_Q3; ///< third quartile
    float m_IQR; ///< IQR = Q3-Q1
    float m_min; ///< min
    float m_max; ///< max
    float m_mean; ///< mean
    float m_var; ///< var
};
