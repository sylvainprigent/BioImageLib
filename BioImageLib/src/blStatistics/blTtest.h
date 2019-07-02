/// \file blTtest.h
/// \brief Ttest functions declaration
/// \author PRIGENT S.
/// \version 0.1
/// \date 2019

#pragma once

#include <vector>
#include <string>
using namespace std;

#include "blStatisticsExport.h"

/// \class blTtest
/// \brief Class that contains hypothesis t-test functions
class BLSTATISTICS_EXPORT blTtest {

public:
    /// \fn int Ttest(vector<double> x, double mean, double &p_value, string tail);
    /// \brief Function that do a Ttest on the distribution 'x' due to the mean value 'mean'
    /// \param[in]  x			Vector containing the first distribution to test
    /// \param[in]  y			Vector containing the second distribution to test
    /// \param[out]	p_value		Optained p_value by the t_test
    /// \param[in]  tail		Tail to compute the test ('both', 'left', 'right')
    /// \return exited error (0: ok, 1:x dimension = 1, 2: standard deviation of x = 0, 3: dimension x != dimension y)
    static int Ttest(vector<double> x, vector<double> y, double &p_value, string tail);

	/// \fn int Ttest(vector<double> x, double mean, double &p_value, string tail);
	/// \brief Function that do a Ttest on the distribution 'x' due to the mean value 'mean'
	/// \param[in]  x			Vector containing the distribution to test
	/// \param[in]  mean		Mean value to compare x
	/// \param[out]	p_value		Optained p_value by the t_test
	/// \param[in]  tail		Tail to compute the test ('both', 'left', 'right')
	/// \return exited error (0: ok, 1:x dimension = 1, 2: standard deviation of x = 0)
    static int Ttest(vector<double> x, double mean, double &p_value, string tail);

	/// \fn int Ttest(vector<double> x, double mean, double &xmean, double &xsigma, double &studentStat, double &bothtailsP, double &lefttailP, double &righttailP);
	/// \brief Function that do a Ttest on the distribution 'x' due to the mean value 'mean' and gives all the intermediate results
	/// \param[in]  x			Vector containing the distribution to test
	/// \param[in]  mean		Mean value to compare x
	/// \param[out] xmean		Mean value of x
	/// \param[out] xsigma		Standard deviation of x
	/// \param[out] studentStat	Value of the student statistic
	/// \param[out] bothtailsP  Both tail p_value
	/// \param[out] lefttailP   Left tail p_value
	/// \param[out] righttailP  Right tail p_value
	/// \return exited error (0: ok, 1:x dimension = 1, 2: standard deviation of x = 0)
    static int Ttest(vector<double> x, double mean, double &xmean, double &xsigma, double &studentStat, double &bothtailsP, double &lefttailP, double &righttailP);

	/// \fn double studentDistribution(int k, double t);
	/// \brief Function that compute the student distribition.
	/// \param[in] k	Freedom degree
	/// \param[in] t	Point where to compute the distribution
	/// \return Value of the Student distribution at t
	/// This function use the method based on the incomplete beta function to estimate the Student distribution
    static double studentDistribution(int k, double t);

	/// \fn double gammln(double xx);
	/// \brief Function that compute the gamma logarithm at xx
	/// \param[in] xx	Position where to compute Gamma
	/// \return Gamma value on xx
	/// This implementation use the serie developpement of Gamma that converge after only 6 iterations
    static double gammln(double xx);

	/// \fn double betai(double a, double b, double x);
	/// \brief Function that compute the incomplete beta function Ix(a; b)
	/// This implementation needs the betacf function
    static double betai(double a, double b, double x);


	/// \fn double betacf(double a, double b, double x);
	/// \brief Function that evaluates continued fraction for incomplete beta function by modified Lentz's method
    static double betacf(double a, double b, double x);
};
