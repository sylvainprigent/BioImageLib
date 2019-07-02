/// \file blTtest.cpp
/// \brief Ttest class implementation
/// \author PRIGENT S.
/// \version 0.1
/// \date 2019

#include <math.h>
#include <iostream>

#include "blTtest.h"

int blTtest::Ttest(vector<double> x, vector<double> y, double &p_value, string tail)
{
    if (x.size() != y.size()){
        return 3;
    }
    vector<double> data;
    data.resize(x.size());
    for (vector<double>::size_type i = 0 ; i < x.size() ; i++){
        data[i] = x[i] - y[i];
    }
    return blTtest::Ttest(data, 0.0, p_value, tail);
}

int blTtest::Ttest(vector<double> x, double mean, double &p_value, string tail)
{
    double xmean;
    double xsigma;
    double studentStat;
    double lefttailP;
    double righttailP;
    double bothtailsP;
    int retour = Ttest(x, mean, xmean, xsigma, studentStat, bothtailsP, lefttailP, righttailP);
    if (tail == "both")
        p_value = bothtailsP;
    else if (tail == "right")
        p_value = righttailP;
    else if (tail == "left")
        p_value = lefttailP;
    else
        cout << "HypothesisTest::Ttest -> Error: tail have to be both, right or left"<< endl;
    return retour;
}

int blTtest::Ttest(vector<double> x, double mean, double &xmean, double &xsigma, double &studentStat, double &bothtailsP, double &lefttailP, double &righttailP)
{
    int n = int(x.size());
    bothtailsP = 0;
    lefttailP = 0;
    righttailP = 0;

    int dim = int(x.size());
    if( dim < 2 )
    {
        bothtailsP = 1.0;
        lefttailP = 1.0;
        righttailP = 1.0;
        return 1;
    }

    /// 1- Compute the mean value
    xmean = 0;
    for(int i = 0 ; i < n ; ++i)
    {
        xmean += x[i];
    }
    xmean /= double(n);

    /// 2- Compute variance
    double xvariance = 0;
    xsigma = 0;
    if( n!=1 )
    {
        for(int i = 0 ; i < n ; i++)
        {
            xvariance += x[i]*x[i];
        }
        xvariance=(xvariance-n*xmean*xmean)/(n-1);
        xsigma = sqrt(xvariance);
    }
    if( xsigma == 0.0 )
    {
        bothtailsP = 1.0;
        lefttailP = 1.0;
        righttailP = 1.0;
        return 2;
    }

    /// 3- Compute the student statistic
    studentStat = (xmean-mean)/(xsigma/sqrt(double(n)));

    /// 4- Compute the p-value
    double p_value = blTtest::studentDistribution(n-1, studentStat);
    bothtailsP = 1-p_value;
    if (studentStat <= 0)
    {
        lefttailP = bothtailsP/2;
        righttailP = 1- lefttailP;
    }
    else
    {
        righttailP = bothtailsP/2;
        lefttailP = 1- righttailP;
    }

    return 0;
}

double blTtest::studentDistribution(int k, double t)
{
    return 1- blTtest::betai( double(k)/2, 0.5, double(k)/(double(k)+t*t));
}


// ******************************** gammaln ***********************************
double blTtest::gammln(double xx)
{
    double x,y,tmp,ser;
    static double cof[6]={76.18009172947146,-86.50532032941677, 24.01409824083091,-1.231739572450155, 0.1208650973866179e-2,-0.5395239384953e-5};
    int j;
    y=x=xx;
    tmp=x+5.5;
    tmp -= (x+0.5)*log(tmp);
    ser = 1.000000000190015;
    for ( j = 0; j <= 5 ; j++)
        ser += cof[j]/++y;
    return -tmp+log(2.5066282746310005*ser/x);
}


// ********************* incomplete beta function Ix(a; b) ********************* 
double blTtest::betai(double a, double b, double x)
{
    double bt;
    if ( x < 0.0 || x > 1.0 )
        cout << "betai -> x have to be between 0 ant 1" << endl;
    if (x == 0.0 || x == 1.0)
    {
        bt=0.0;
    }
    else //Factors in front of the continued fraction.
    {
        bt=exp(gammln(a+b)-gammln(a)-gammln(b)+a*log(x)+b*log(1.0-x));
    }
    if (x < (a+1.0)/(a+b+2.0)) //Use continued fraction directly.
    {
        return bt*blTtest::betacf(a,b,x)/a;
    }
    else // Use continued fraction after making the symetry transformation.
    {
        return 1.0-bt*blTtest::betacf(b,a,1.0-x)/b;
    }
}

// ********************************* betacf ************************************
double blTtest::betacf(double a, double b, double x)
{
    // Precision parameters
    int MAXIT = 100;
    double EPS = 3.0e-7;
    double FPMIN = 1.0e-30;

    int m,m2;
    double aa,c,d,del,h,qab,qam,qap;
    qab=a+b; // These q's will be used in factors that occur
    qap=a+1.0; //in the coefcients (6.4.6).
    qam=a-1.0;

    c=1.0; // First step of Lentz's method.
    d=1.0-qab*x/qap;
    if (fabs(d) < FPMIN)
        d=FPMIN;
    d=1.0/d;
    h=d;
    for (m=1 ; m <= MAXIT ; m++)
    {
        m2=2*m;
        aa = m*(b-m)*x/((qam+m2)*(a+m2));
        d=1.0+aa*d; // One step (the even one) of the recurrence.
        if (fabs(d) < FPMIN)
            d=FPMIN;
        c=1.0+aa/c;
        if (fabs(c) < FPMIN)
            c=FPMIN;
        d=1.0/d;
        h *= d*c;
        aa = -(a+m)*(qab+m)*x/((a+m2)*(qap+m2));
        d=1.0+aa*d; // Next step of the recurrence (the odd one).
        if (fabs(d) < FPMIN)
            d=FPMIN;
        c=1.0+aa/c;
        if (fabs(c) < FPMIN)
            c=FPMIN;
        d=1.0/d;
        del=d*c;
        h *= del;
        if (fabs(del-1.0) < EPS) // Stop condition
            break;
    }
    if (m > MAXIT)
        cout << "a or b too big, or MAXIT too small in betacf" << endl;
    return h;
}
