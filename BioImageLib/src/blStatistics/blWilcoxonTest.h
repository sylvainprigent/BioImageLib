/// \file blWilcoxonTest.h
/// \brief Wilcoxon test functions declaration
/// \author PRIGENT S.
/// \version 0.1
/// \date 2019

#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
using namespace std;

#include "blStatisticsExport.h"

/// \class wilcoxonTest
/// \brief Class that allows to compute a wilcoxon test between two distributions
class BLSTATISTICS_EXPORT blWilcoxonTest
{

public:
    /// \fn  blWilcoxonTest();
    /// \brief Constructor
    blWilcoxonTest();

    /// \fn void setDistributions(const vector<double>& distribution1, const vector<double>& distribution2);
    /// \brief Function that load the two distributions to compare
    /// \param[in] distribution1 reference to the first distribution
    /// \param[in] distribution1 reference to the second distribution
    void setDistributions(const vector<double>& distribution1, const vector<double>& distribution2);

    /// \fn void CalculateTest();
    /// \brief Function that launch the test calculation
    void calculateTest();

    /// \fn void run();
    /// \brief Function that taunch the test (same as calculateTest())
    void run();

    /// \fn double getPvalue();
    /// \brief Function that return the p-value of the test
    double getPvalue();

    /// \fn double getRank();
    /// \brief Function that return the rank of the test
    double getRank();

private:
    vector<double> m_distribution1; ///< first distribution
    vector<double> m_distribution2; ///< second distribution
    double m_pvalue;				///< p-value of the test
    double m_rank;					///< rank of the test

    /// \fn void calculateExactPvalue(double rank, int N);
    /// \brief Function that computes the p-value from the rank with the
    /// exact calculation ( used for distribition size below 20 )
    /// \param[in] rank Test rank
    /// \param[in] N size of the distributions
    void calculateExactPvalue(double rank, int N);

    /// \fn void calculateAsymptoticPvalue(double rank, int N);
    /// \brief Function that assymptoticaly calculats the test p-value
    /// (used for distribition size over 20)
    /// \param[in] rank Test rank
    /// \param[in] N size of the distributions
    void calculateAsymptoticPvalue(double rank, int N);

    /// \fn vector<double> calculateRank(vector<double> zpos);
    /// \brief Fonction that calculate the test rank from the obsolute difference
    /// of the two distributions
    /// \param[in] zpos obsolute difference of the two distributions
    vector<double> calculateRank(vector<double> zpos);

    double erf(double x);
    double gammp(double a, double x);
    void gcf(double *gammcf, double a, double x, double *gln);
    void gser(double *gamser, double a, double x, double *gln);
    double gammln(double xx);
    double cumulativeNormalDistribution(double z);

};
