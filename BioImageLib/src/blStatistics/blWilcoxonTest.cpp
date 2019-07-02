#include "blWilcoxonTest.h"

// Constructor
blWilcoxonTest::blWilcoxonTest()
{
}

// Public Methods
void blWilcoxonTest::setDistributions(const vector<double> &distribution1, const vector<double>& distribution2)
{
    m_distribution1 = distribution1;
    m_distribution2 = distribution2;
}

void blWilcoxonTest::calculateTest()
{
    /// 1- Calculate z = x-y
    int N = int(m_distribution1.size());
    vector<double> z; z.resize(m_distribution1.size());
    vector<double> zpos; zpos.resize(m_distribution1.size());
    for (vector<double>::size_type i = 0; i < vector<double>::size_type(N) ; ++i)
    {
        z[i] = m_distribution1[i] - m_distribution2[i];
        zpos[i] = fabs(z[i]);
    }

    /// 2- Calculate ranks
    vector<double> ranks = calculateRank(zpos);

    /// 3- Calculate test rank W
    double Wp = 0;
    for (int i = 0 ; i < N ; ++i)
    {
        if (z[i] > 0)
            Wp += ranks[i];
    }

    double Wm = (((double) (N * (N + 1))) / 2.0) - Wp;

    double m_rank = Wp;
    if (Wm < Wp)
        m_rank = Wm;

    /// 4- Calculate p_value
    if (N < 20)
        calculateExactPvalue(m_rank, N);
    else
        calculateAsymptoticPvalue(m_rank, N);

}

void blWilcoxonTest::run(){
    calculateTest();
}

double blWilcoxonTest::getPvalue()
{
    return m_pvalue;
}

double blWilcoxonTest::getRank()
{
    return m_rank;
}


// Private Methods

/// \fn void wilcoxonTest::CalculateExactPvalue(double Winput, int N)
///  from http://www.fon.hum.uva.nl/Service/Statistics/Signed_Rank_Algorihms.html#C
void blWilcoxonTest::calculateExactPvalue(double Winput, int N)
{
    int W, MaximalW, NumberOfPossibilities, CountLarger;
    int i, RankSum, j;

    /* Determine Wmax, i.e., work with the largest Rank Sum */
    MaximalW = N*(N+1)/2;
    if(Winput < MaximalW/2)Winput = MaximalW - Winput;
    W = int(Winput);    /* Convert to long int */
    if(W != Winput)++W;  /* Increase to next full integer */

    // The total number of possible outcomes is 2**N
    NumberOfPossibilities = int(pow(double(2), N));

    // Initialize and loop. The loop-interior will be run 2**N times.
    CountLarger = 0;
    // Generate all distributions of sign over ranks as bit-patterns (i).
    for(i=0; i < NumberOfPossibilities; ++i)
    {
        RankSum = 0;

        //   Shift "sign" bits out of i to determine the Sum of Ranks (j).
        for(j=0; j < N; ++j)
        {
            if((i >> j) & 1)RankSum += j + 1;
        }

        // Count the number of "samples" that have a Sum of Ranks larger than
        // or equal to the one found (i.e., >= W).
        if(RankSum >= W)++CountLarger;
    }

    // multiply by 2 for both tail test
    m_pvalue = 2*((double)CountLarger) / ((double)NumberOfPossibilities);
}


void blWilcoxonTest::calculateAsymptoticPvalue(double rank, int N)
{

    /// 1- Center the variable
    double ES = (double) (N * (N + 1)) / 4.0;

    // Same as (but saves computations):
    // double VarW = ((double) (N * (N + 1) * (2*N + 1))) / 24;
    double VarS = ES * ((double) (2 * N + 1) / 6.0);
    // - 0.5 is a continuity correction
    double z = (rank - ES - 0.5) / sqrt(VarS);

    double standardNormal = cumulativeNormalDistribution(z);
    // multiply by 2 for both tail test
    m_pvalue = 2*standardNormal;

}

double blWilcoxonTest::cumulativeNormalDistribution(double z)
{
    return 0.5*( 1 + erf(z/sqrt(2.0)) );
}


// Returns the value ln[Γ(xx)] for xx > 0.
double blWilcoxonTest::gammln(double xx)
{
    double x,y,tmp,ser;
    static double cof[6]={76.18009172947146,-86.50532032941677,
                          24.01409824083091,-1.231739572450155,
                          0.1208650973866179e-2,-0.5395239384953e-5};
    int j;

    y=x=xx;
    tmp=x+5.5;
    tmp -= (x+0.5)*log(tmp);
    ser=1.000000000190015;
    for (j=0;j<=5;j++) ser += cof[j]/++y;
    return -tmp+log(2.5066282746310005*ser/x);
}

// Returns the incomplete gamma function P (a, x) evaluated by its series representation as gamser.
// Also returns ln Γ(a) as gln.
void blWilcoxonTest::gser(double *gamser, double a, double x, double *gln)
{
    int ITMAX  = 100;
    double EPS = 3.0e-7;

    int n;
    double sum,del,ap;
    *gln=gammln(a);
    if (x <= 0.0)
    {
        if (x < 0.0) cout<<"x less than 0 in routine gser"<<endl;
        *gamser=0.0;
        return;
    }
    else
    {
        ap=a;
        del=sum=1.0/a;
        for (n=1;n<=ITMAX;n++)
        {
            ++ap;
            del *= x/ap;
            sum += del;
            if (fabs(del) < fabs(sum)*EPS)
            {
                *gamser=sum*exp(-x+a*log(x)-(*gln));
                return;
            }
        }
        cout<<"a too large, ITMAX too small in routine gser"<< endl;
        return;
    }
}


// Returns the incomplete gamma function Q(a, x) evaluated by its continued fraction represen-
// tation as gammcf. Also returns ln Γ(a) as gln.
void blWilcoxonTest::gcf(double *gammcf, double a, double x, double *gln)
{
    int ITMAX  = 100 ;
    double EPS = 3.0e-7;
    double FPMIN = 1.0e-30;

    int i;
    double an,b,c,d,del,h;

    *gln=gammln(a);
    b=x+1.0-a;
    // Set up for evaluating continued fraction by modified Lentz’s method (§5.2) with b0 = 0.
    c=1.0/FPMIN;
    d=1.0/b;
    h=d;
    for (i=1;i<=ITMAX;i++)
    {
        //Iterate to convergence.
        an = -i*(i-a);
        b += 2.0;
        d=an*d+b;
        if (fabs(d) < FPMIN) d=FPMIN;
        c=b+an/c;
        if (fabs(c) < FPMIN) c=FPMIN;
        d=1.0/d;
        del=d*c;
        h *= del;
        if (fabs(del-1.0) < EPS) break;
    }
    if (i > ITMAX) cout << "a too large, ITMAX too small in gcf" << endl;
    *gammcf=exp(-x+a*log(x)-(*gln))*h;
}


double blWilcoxonTest::gammp(double a, double x)
{

    double gamser,gammcf,gln;
    if (x < 0.0 || a <= 0.0) cout << "Invalid arguments in routine gammp" << endl;
    if (x < (a+1.0))
    {
        gser(&gamser,a,x,&gln);
        return gamser;
    }
    else
    {
        gcf(&gammcf,a,x,&gln);
        return 1.0-gammcf;
    }
}


double blWilcoxonTest::erf(double x)
{
    return x < 0.0 ? -gammp(0.5,x*x) : gammp(0.5,x*x);

}

vector<int> SortShell(vector<double> vectIn, vector<double> &vect);
vector<double> blWilcoxonTest::calculateRank(vector<double> zpos)
{
    int N = int(zpos.size());
    vector<double> sortedZ;
    vector<int> corespPos = SortShell(zpos, sortedZ);

    // Compute rank on sorted data
    vector<double> ranks; ranks.resize(N);
    int pass;
    double val;
    for (int i = 0 ; i < N ; ++i)
    {
        pass = 0;
        for (int n = i+1 ; n < N ; ++n)
        {
            if (sortedZ[n] == sortedZ[i])
                pass++;
            else
                break;
        }
        if (pass > 0)
        {
            val = 0;
            for (int n = i ; n < i+pass+1 ; ++n)
            {
                val += n;
            }
            val /= pass+1;
            for (int n = i ; n < i+pass+1 ; ++n )
                ranks[n] = val+1;
            i=i+pass;
        }
        else
        {
            ranks[i] = i+1;
        }
    }

    // unsorted data
    vector<double> finalRanks; finalRanks.resize(N);
    for(int i=0; i<N; ++i)
    {
        finalRanks[corespPos[i]] = ranks[i];
    }
    return finalRanks;

}

int my_round (double x);
/// \fn vector<int> SortShell(vector<double> vectIn, vector<double> vect)
/// \brief sort a vector with the shell algorithm
/// \param[in] vectIn Vector to sort
/// \param[out] vect Sorted vector
/// \return Positions correspondances between  vectIn end vect
vector<int> SortShell(vector<double> vectIn, vector<double> &vect)
{
    //vector<double> vect;
    vector<int> pos;
    pos.resize(vectIn.size());
    for ( int i = 0 ; i < int(pos.size()) ; i++)
        pos[i] = i;
    int tempPos;

    vect = vectIn;
    int n = int(vect.size());
    int inc = my_round((n/2));
    int j;
    double temp;
    while ( inc > 0 )
    {
        for ( int i = inc ; i < n ; i++)
        {
            temp = vect[i];
            tempPos = pos[i];
            j = i;
            while ( (j >= inc) && (vect[j-inc] > temp) )
            {
                vect[j] = vect[j-inc];
                pos[j] = pos[j-inc];
                j = j-inc;
            }
            vect[j] = temp;
            pos[j] = tempPos;
        }
        inc = my_round((inc / 2.2));
    }

    return pos;
}

int my_round (double x)
{
    int i = (int) x;
    if (x >= 0.0)
    {
        return ((x-i) >= 0.5) ? (i + 1) : (i);
    }
    else
    {
        return (-x+i >= 0.5) ? (i - 1) : (i);
    }
}
