/// \file blMathGeometry.cpp
/// \brief blMathGeometry class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMathGeometry.h"

#include <cmath>
#include <algorithm>
#include <string>
#include <iostream>

using namespace std;

// /////////////////////////////////////////////////////////////////////////
//                             blMathGeometry
// /////////////////////////////////////////////////////////////////////////
void blMathGeometry::CalculateCircleCoordinates(int x_centre, int y_centre, int r, std::vector<std::vector<int> > &coords){

    //coords.clear();
    int x = 0;
    int y = r;
    int d = r - 1;
    while( y >= x )
    {
        if (x == 0){
            AddPixel( x_centre + x , y_centre + y, coords );
            AddPixel( x_centre + y , y_centre + x, coords );
            AddPixel( x_centre - y , y_centre + x, coords );
            AddPixel( x_centre + x , y_centre - y, coords );
        }
        else if (y == 0){
            AddPixel( x_centre + x , y_centre, coords );
            AddPixel( x_centre , y_centre + x, coords );
            AddPixel( x_centre - x , y_centre, coords );
            AddPixel( x_centre, y_centre - x, coords );
        }
        else if (x==y){
            AddPixel( x_centre + x , y_centre + x, coords );
            AddPixel( x_centre - x , y_centre + x, coords );
            AddPixel( x_centre + x , y_centre - x, coords );
            AddPixel( x_centre - x , y_centre - x, coords );
        }
        else{
            AddPixel( x_centre + x , y_centre + y, coords );
            AddPixel( x_centre + y , y_centre + x, coords );
            AddPixel( x_centre - x , y_centre + y, coords );
            AddPixel( x_centre - y , y_centre + x, coords );
            AddPixel( x_centre + x , y_centre - y, coords );
            AddPixel( x_centre + y , y_centre - x, coords );
            AddPixel( x_centre - x , y_centre - y, coords );
            AddPixel( x_centre - y , y_centre - x, coords );
        }

        if( d >= 2*x )
        {
            d = d-2*x-1 ;
            x = x+1 ;
        }
        else if( d <= 2*(r-y) )
        {
            d = d+2*y-1;
            y = y-1    ;
        }
        else
        {
            d = d+2*(y-x-1);
            y = y-1;
            x = x+1;
        }
    }
}
void blMathGeometry::AddPixel(int x, int y, std::vector<std::vector<int> > &coords)
{
    std::vector<int> coord; coord.resize(2);
    coord[0] = x; coord[1] = y;
    coords.push_back(coord);
}

void blMathGeometry::CalculateCircleCoordinates( int x_centre, int y_centre, int r, std::vector<int>& xp, std::vector<int>& yp )
{
    int x = 0;
    int y = r;
    int d = r - 1;
    while( y >= x )
    {
        if (x == 0){
            AddPixel( x_centre + x , y_centre + y, xp, yp );
            AddPixel( x_centre + y , y_centre + x, xp, yp );
            AddPixel( x_centre - y , y_centre + x, xp, yp );
            AddPixel( x_centre + x , y_centre - y, xp, yp );
        }
        else if (y == 0){
            AddPixel( x_centre + x , y_centre, xp, yp );
            AddPixel( x_centre , y_centre + x, xp, yp );
            AddPixel( x_centre - x , y_centre, xp, yp );
            AddPixel( x_centre, y_centre - x, xp, yp );
        }
        else if (x==y){
            AddPixel( x_centre + x , y_centre + x, xp, yp );
            AddPixel( x_centre - x , y_centre + x, xp, yp );
            AddPixel( x_centre + x , y_centre - x, xp, yp );
            AddPixel( x_centre - x , y_centre - x, xp, yp );
        }
        else{
            AddPixel( x_centre + x , y_centre + y, xp, yp );
            AddPixel( x_centre + y , y_centre + x, xp, yp );
            AddPixel( x_centre - x , y_centre + y, xp, yp );
            AddPixel( x_centre - y , y_centre + x, xp, yp );
            AddPixel( x_centre + x , y_centre - y, xp, yp );
            AddPixel( x_centre + y , y_centre - x, xp, yp );
            AddPixel( x_centre - x , y_centre - y, xp, yp );
            AddPixel( x_centre - y , y_centre - x, xp, yp );
        }

        if( d >= 2*x )
        {
            d = d-2*x-1 ;
            x = x+1 ;
        }
        else if( d <= 2*(r-y) )
        {
            d = d+2*y-1;
            y = y-1    ;
        }
        else
        {
            d = d+2*(y-x-1);
            y = y-1;
            x = x+1;
        }
    }
}
void blMathGeometry::AddPixel(int x, int y, std::vector<int> &xp, std::vector<int> &yp)
{
    xp.push_back(x);
    yp.push_back(y);
}

void blMathGeometry::CalculateRectangleCorners(std::vector<float> &xp, std::vector<float> &yp, std::vector<float> A, std::vector<float> B, int width){

    float ABx = B[0]-A[0]; float ABy = B[1]-A[1];
    float AB_norm = pow(float(B[0]-A[0]), 2) + pow(float(B[1]-A[1]), 2);
    float width_norm = sqrt(float(width)/2.0);

    xp.resize(4); yp.resize(4);
    // B1
    xp[0] = - width_norm*(ABy)/AB_norm + B[0];
    yp[0] =   width_norm*(ABx)/AB_norm + B[1];
    // B2
    xp[1] =   width_norm*(ABy)/AB_norm + B[0];
    yp[1] = - width_norm*(ABx)/AB_norm + B[1];
    // A2
    xp[2] =   width_norm*(-ABy)/AB_norm + A[0];
    yp[2] = - width_norm*(-ABx)/AB_norm + A[1];
    // A1
    xp[3] = - width_norm*(-ABy)/AB_norm + A[0];
    yp[3] =   width_norm*(-ABx)/AB_norm + A[1];
}

void blMathGeometry::Calculate2DLineCoordinates( int x1, int y1, int x2, int y2, std::vector<int> &px, std::vector<int> &py){
    float maxi = 0; float val;
    maxi = fabs(float(x2 - x1));
    val =  fabs(float(y2 - y1));
    if (val > maxi){
        maxi = val;
    }
    int m = int(maxi + 1);
    px = LinSpaceRound(x1, x2, m);
    py = LinSpaceRound(y1, y2, m);
}

void blMathGeometry::Calculate3dLineCoordinates(std::vector<int> &px, std::vector<int> &py, std::vector<int> &pz, std::vector<int> start_pts, std::vector<int> end_pts){

    float maxi = 0; float val;
    for (unsigned int i = 0 ; i  < start_pts.size() ; ++i){
        val = fabs(float(end_pts[i] - start_pts[i]));
        if (val > maxi){
            maxi = val;
        }
    }
    int m = int(maxi + 1);
    px = LinSpaceRound(start_pts[0], end_pts[0], m);
    py = LinSpaceRound(start_pts[1], end_pts[1], m);
    pz = LinSpaceRound(start_pts[2], end_pts[2], m);
}

std::vector<float> blMathGeometry::LinSpace(float d1, float d2, int n){

    unsigned int m = n;
    float n1 = floor(m)-1;

    std::vector<float> vect; vect.resize(m-1);
    std::vector<float> z; z.resize(m);
    for(unsigned int i = 0 ; i < m-1 ; ++i){
        vect[i] = i;
    }
    for (unsigned int i = 0 ; i < m-1 ; ++i){
        z[i] =  d1 + vect[i]*(d2-d1)/n1;
    }
    z[m-1] = d2;
    return z;
}

std::vector<int> blMathGeometry::LinSpaceRound(float d1, float d2, int n){
    std::vector<float> lin = LinSpace(d1, d2, n);
    std::vector<int> linFloor; linFloor.resize(lin.size());
    for (unsigned int i = 0 ; i < lin.size() ; ++i){
        linFloor[i] = int(floor(lin[i]+0.5));
    }
    return linFloor;
}

void blMathGeometry::CalculatePixelsCoordinateBorderSphere(std::vector<float> sphere, int borderWidth, std::vector<int> &px, std::vector<int> &py, std::vector<int> &pz ){
    //cout << "CalculatePixValuesInBorderCircle -> begin"  << endl;
    double euclid;

    px.clear(); py.clear(); pz.clear();
    int r = sphere[3] + borderWidth + 1;
    float r1 = sphere[3]*sphere[3]; float r2 = pow(sphere[3] + borderWidth, 2);
    for (int x = sphere[0] - r; x <= sphere[0] + r ; ++x){
        for (int y = sphere[1] - r; y <= sphere[1] + r ; ++y){
            for (int z = sphere[2] - r; z <= sphere[2] + r ; ++z){
                euclid = pow(x - sphere[0],2) + pow(y - sphere[1],2) + pow(z - sphere[2],2);
                if ( euclid > r1 && euclid <= r2 ){
                    px.push_back(x);
                    py.push_back(y);
                    pz.push_back(z);
                }
            }
        }
    }
}

bool blMathGeometry::DoesEllipsesIntersect(std::vector<float> ellipse1, std::vector<float> ellipse2, double levelSet){
  //cout << ellipse1<<endl;  
  //cout << ellipse2<<endl;
    double al2 = double(ellipse2[3]);
    //cout << al2<< " - " << ellipse2[3]<<endl;

    double bl2 = double(ellipse2[2]);

    double a2square = double(ellipse2[3])*double(ellipse2[3]);
    double b2square = double(ellipse2[2])*double(ellipse2[2]);
    double a1square = double(ellipse1[3])*double(ellipse1[3]);
    double b1square = double(ellipse1[2])*double(ellipse1[2]);

    // Lipschitz constants
    double L1=2.0*a2square/b1square;
    double mu=2.0*b2square/a1square;

    // Calculate rotation coefficients for ellipse 1
    double cos_teta1 = cos(double(ellipse1[4]));
    //cout << double(ellipse1[4])<< " - " << ellipse1[4]<<endl;
    double sin_teta1 = sin(double(ellipse1[4]));
    //cout << double(ellipse2[4])<< " - " << ellipse2[4]<<endl;
    double cos_teta1_square = cos_teta1*cos_teta1;
    double sin_teta1_square = sin_teta1*sin_teta1;
    double sin_cos_teta1 = sin_teta1*cos_teta1;

    // Calculate rotation coefficients for ellipse 2
    double cos_teta2 = cos(double(ellipse2[4]));
    double sin_teta2 = sin(double(ellipse2[4]));
    double cos_teta2_square = cos_teta2*cos_teta2;
    double sin_teta2_square = sin_teta2*sin_teta2;
    double sin_cos_teta2 = sin_teta2*cos_teta2;
    // A2_demi
    //A2_demi=R2'*diag([1/a2,1/b2])*R2
    double alpha = 1.0/al2; double beta = 1.0/bl2;
    double a2_demi = alpha*cos_teta2_square + beta*sin_teta2_square;
    double b2_demi = (beta-alpha)*sin_cos_teta2;
    double d2_demi = alpha*sin_teta2_square + beta*cos_teta2_square;
    //cout << "A2_demi = " << endl;
    //cout << a2_demi << ", " << b2_demi << endl;
    //cout << b2_demi << ", " << d2_demi << endl;

    // A2_inv_demi
    // A2_inv_demi=R2'*diag([a2,b2])*R2
    double a2_inv_demi = al2*cos_teta2_square + bl2*sin_teta2_square;
    double b2_inv_demi = (bl2-al2)*sin_cos_teta2;
    double d2_inv_demi = al2*sin_teta2_square + bl2*cos_teta2_square;

    // A1
    // A1=R1'*D1*R1
    alpha = 1.0/a1square; beta = 1.0/b1square;
    double a1_mat = alpha*cos_teta1_square + beta*sin_teta1_square;
    double b1_mat = (beta-alpha)*sin_cos_teta1;
    double d1_mat = alpha*sin_teta1_square + beta*cos_teta1_square;
    //cout << "A1 = " << endl;
    //cout << a1_mat << ", " << b1_mat << endl;
    //cout << b1_mat << ", " << d1_mat << endl;

    // A2_inv_demi*A1
    double A2invdemi_A1_a = a1_mat*a2_inv_demi + b1_mat*b2_inv_demi;
    double A2invdemi_A1_b = b1_mat*a2_inv_demi + d1_mat*b2_inv_demi;
    double A2invdemi_A1_c = a1_mat*b2_inv_demi + b1_mat*d2_inv_demi;
    double A2invdemi_A1_d = b1_mat*b2_inv_demi + d1_mat*d2_inv_demi;

    // initialize y
    // y=A2_demi*cen2;
    double y[2];
    y[0] = a2_demi*double(ellipse2[0]) + b2_demi*double(ellipse2[1]);
    y[1] = b2_demi*double(ellipse2[0]) + d2_demi*double(ellipse2[1]);
    double A2demi_cen2[2]; 
    A2demi_cen2[0]=y[0]; 
    A2demi_cen2[1]=y[1];
    //cout << "y = " << y[0] << ", " << y[1] << endl;

    // initialize Gradient
    double A2invdemi_y_center1[2];
    A2invdemi_y_center1[0] = a2_inv_demi*y[0] + b2_inv_demi*y[1] - double(ellipse1[0]);
    A2invdemi_y_center1[1] = b2_inv_demi*y[0] + d2_inv_demi*y[1] - double(ellipse1[1]);

    // grad=2*A2_inv_demi*A1*(A2_inv_demi*y-cen1);
    double grad[2];
    grad[0] = 2.0*(A2invdemi_A1_a*A2invdemi_y_center1[0] + A2invdemi_A1_b*A2invdemi_y_center1[1]);
    grad[1] = 2.0*(A2invdemi_A1_c*A2invdemi_y_center1[0] + A2invdemi_A1_d*A2invdemi_y_center1[1]);
    //cout << "grad = " << grad[0] << ", " << grad[1] << endl;

    // Main loop
    bool condition=true;
    int i=1;
    double y_old[2];
    double norm, norm_stop;
    while (condition){
        //cout <<"************************************ "<<endl; 
        y_old[0]=y[0]; y_old[1]=y[1];
        ////cout << "i="<<i<<endl;
        //-- Gradient descent
        y[0]=y[0]-grad[0]*2/(L1+mu);
        y[1]=y[1]-grad[1]*2/(L1+mu);
        //cout << "y = " << y[0] << ", " << y[1] << endl;
	//cout<<" a2demi cen2 0 : "<<A2demi_cen2[0]<<endl;
	//cout<<" a2demi cen2 1 : "<<A2demi_cen2[1]<<endl;
        //-- Projection of x on the ellipsoid 2
	double v0 = (y[0]-A2demi_cen2[0])*(y[0]-A2demi_cen2[0]);
	double v1 = (y[1]-A2demi_cen2[1])*(y[1]-A2demi_cen2[1]);
	//cout <<"v0 = "<<v0<<endl; 
	//cout <<"v1 = "<<v1<<endl; 
        norm = sqrt(v0 + v1);
        //cout<<"morm : "<<norm<<endl;
	if ( norm>1.0 ){
            y[0] = A2demi_cen2[0] + (y[0]-A2demi_cen2[0])/norm;
            y[1] = A2demi_cen2[1] + (y[1]-A2demi_cen2[1])/norm;
            //cout << "y_norm = " << y[0] << ", " << y[1] << endl;
        }

        //-- Gradient
        // A2invdemi_y_center1
        A2invdemi_y_center1[0] = a2_inv_demi*y[0] + b2_inv_demi*y[1] - double(ellipse1[0]);
        A2invdemi_y_center1[1] = b2_inv_demi*y[0] + d2_inv_demi*y[1] - double(ellipse1[1]);
        //cout<<"morm : "<<norm<<endl;
	//cout<<"	A2invdemi_y_center1[0] "<<A2invdemi_y_center1[0]<<endl;
	//cout<<"	A2invdemi_y_center1[1] "<<A2invdemi_y_center1[1]<<endl;
        
	// grad
        grad[0] = 2*(A2invdemi_A1_a*A2invdemi_y_center1[0] + A2invdemi_A1_b*A2invdemi_y_center1[1]);
        grad[1] = 2*(A2invdemi_A1_c*A2invdemi_y_center1[0] + A2invdemi_A1_d*A2invdemi_y_center1[1]);
        //cout << " -- it : "<<i<<" : grad = " << grad[0] << ", " << grad[1] << endl;

        //-- Update the condition
        norm_stop = sqrt( pow(y[0]-y_old[0], 2) + pow(y[1]-y_old[1], 2) );
        ////cout << "norm val = " << norm_stop << endl;
        if (norm_stop < 0.00000001){
            condition=false;
        }
        i++;
    }
    //cout << "i = " << i << endl;
    // Value of the level set which satisfy the constraint
    // x=A2_inv_demi*y - cen1;
    double x_c1[2];
    x_c1[0] = a2_inv_demi*y[0] + b2_inv_demi*y[1] -double(ellipse1[0]);
    x_c1[1] = b2_inv_demi*y[0] + d2_inv_demi*y[1] -double(ellipse1[1]);

    double xinter[2];
    xinter[0] = a1_mat*x_c1[0] + b1_mat*x_c1[1];
    xinter[1] = b1_mat*x_c1[0] + d1_mat*x_c1[1];

    double lambda_min = x_c1[0]*xinter[0] + x_c1[1]*xinter[1] -levelSet;


    if ( lambda_min<0 ){
        return true;
    }
    else
        return false;

}

bool blMathGeometry::DoesEllipsoidIntersect(std::vector<float> ellipsoid1, std::vector<float> ellipsoid2, double levelSet){
    // function which test the intersection of 2 ellipsoids
    // Input : cx1 cy1 cz1, cx2, cy2, cz2 -> center of the ellipsoids (colum vector)
    //         a1, a2 -> major axis
    //        b1, b2 -> median axis
    //         c1, c2 -> minor axis
    //         theta1, theta2 -> rotation about y
    //         phi1, phi2 -> rotation about z
    //         gam1, gam2 -> rotation about x
    //
    // Output : etat -> 1 if intersect 0 otherwise

    // Quadratics forms to reprsent the ellipsoids (their zero level set)
    //--- Q(x)=x'Ax+Bx+c -----

    //cout << "DoesEllipsoidIntersect->begin " << endl;
    double al2 = double(ellipsoid2[3]);
    double bl2 = double(ellipsoid2[4]);
    double cl2 = double(ellipsoid2[5]);
    double a1square =  double(ellipsoid1[3])*double(ellipsoid1[3]);
    double b1square =  double(ellipsoid1[4])*double(ellipsoid1[4]);
    double c1square =  double(ellipsoid1[5])*double(ellipsoid1[5]);
    double a2square =  double(ellipsoid2[3])*double(ellipsoid2[3]);
    //float b2square =  ellipsoid2[4]*ellipsoid2[4];
    double c2square =  double(ellipsoid2[5])*double(ellipsoid2[5]);

    // Lipschitz constants
    double L1=2.0*a2square/c1square;
    double mu=2.0*c2square/a1square;


    // Calculate rotation coefficients for ellipse 2
    double cos_phi = cos(ellipsoid2[6]); double sin_phi = sin(ellipsoid2[6]);
    double cos_teta = cos(ellipsoid2[7]); double sin_teta = sin(ellipsoid2[7]);
    double cos_gam = cos(ellipsoid2[8]); double sin_gam = sin(ellipsoid2[8]);
    double a2 = cos_teta * cos_phi;
    double b2 = cos_phi*sin_gam*sin_teta - cos_gam*sin_phi;
    double c2 = cos_phi*cos_gam*sin_teta+sin_gam*sin_phi;
    double d2 = cos_teta*sin_phi;
    double e2 = sin_phi*sin_gam*sin_teta+cos_phi*cos_gam;
    double f2 = sin_phi*cos_gam*sin_teta-sin_gam*cos_phi;
    double g2 = -sin_teta;
    double h2 = sin_gam*cos_teta;
    double i2 = cos_gam*cos_teta;
    //cout << "Rx2Ry2Rz2 :" << endl;
    //cout << a2 << ", " << b2 << ", " << c2 << endl;
    //cout << d2 << ", " << e2 << ", " << f2 << endl;
    //cout << g2 << ", " << h2 << ", " << i2 << endl<<endl;

    // Calculate rotation coefficients for ellipse 1
    cos_phi = cos(ellipsoid1[6]);  sin_phi = sin(ellipsoid1[6]);
    cos_teta = cos(ellipsoid1[7]);  sin_teta = sin(ellipsoid1[7]);
    cos_gam = cos(ellipsoid1[8]);  sin_gam = sin(ellipsoid1[8]);

    double a1 = cos_teta * cos_phi;
    double b1 = cos_phi*sin_gam*sin_teta - cos_gam*sin_phi;
    double c1 = cos_phi*cos_gam*sin_teta+sin_gam*sin_phi;
    double d1 = cos_teta*sin_phi;
    double e1 = sin_phi*sin_gam*sin_teta+cos_phi*cos_gam;
    double f1 = sin_phi*cos_gam*sin_teta-sin_gam*cos_phi;
    double g1 = -sin_teta;
    double h1 = sin_gam*cos_teta;
    double i1 = cos_gam*cos_teta;

    // A2_demi
    double alpha = 1.0/al2; double beta = 1.0/bl2; double gamma = 1.0/cl2;
    double a2_demi = alpha*a2*a2 + beta*b2*b2 + gamma*c2*c2;
    double b2_demi = alpha*a2*d2 + beta*b2*e2 + gamma*c2*f2;
    double c2_demi = alpha*a2*g2 + beta*b2*h2 + gamma*c2*i2;
    double e2_demi = alpha*d2*d2 + beta*e2*e2 + gamma*f2*f2;
    double f2_demi = alpha*d2*g2 + beta*e2*h2 + gamma*f2*i2;
    double i2_demi = alpha*g2*g2 + beta*h2*h2 + gamma*i2*i2;
    //cout << "A2_demi :" << endl;
    //cout << a2_demi << ", " << b2_demi << ", " << c2_demi << endl;
    //cout << b2_demi << ", " << e2_demi << ", " << f2_demi << endl;
    //cout << c2_demi << ", " << f2_demi << ", " << i2_demi << endl<<endl;

    // A2_inv_demi
    double a2_inv_demi = al2*a2*a2 + bl2*b2*b2 + cl2*c2*c2;
    double b2_inv_demi = al2*a2*d2 + bl2*b2*e2 + cl2*c2*f2;
    double c2_inv_demi = al2*a2*g2 + bl2*b2*h2 + cl2*c2*i2;
    double e2_inv_demi = al2*d2*d2 + bl2*e2*e2 + cl2*f2*f2;
    double f2_inv_demi = al2*d2*g2 + bl2*e2*h2 + cl2*f2*i2;
    double i2_inv_demi = al2*g2*g2 + bl2*h2*h2 + cl2*i2*i2;
    //cout << "A2_inv_demi :" << endl;
    //cout << a2_inv_demi << ", " << b2_inv_demi << ", " << c2_inv_demi << endl;
    //cout << b2_inv_demi << ", " << e2_inv_demi << ", " << f2_inv_demi << endl;
    //cout << c2_inv_demi << ", " << f2_inv_demi << ", " << i2_inv_demi << endl<<endl;

    // A1
    alpha = 1.0/a1square; beta = 1.0/b1square; gamma = 1.0/c1square;
    double a1_mat = alpha*a1*a1 + beta*b1*b1 + gamma*c1*c1;
    double b1_mat = alpha*a1*d1 + beta*b1*e1 + gamma*c1*f1;
    double c1_mat = alpha*a1*g1 + beta*b1*h1 + gamma*c1*i1;
    double e1_mat = alpha*d1*d1 + beta*e1*e1 + gamma*f1*f1;
    double f1_mat = alpha*d1*g1 + beta*e1*h1 + gamma*f1*i1;
    double i1_mat = alpha*g1*g1 + beta*h1*h1 + gamma*i1*i1;
    //cout << "A1 :" << endl;
    //cout << a1_mat << ", " << b1_mat << ", " << c1_mat << endl;
    //cout << b1_mat << ", " << e1_mat << ", " << f1_mat << endl;
    //cout << c1_mat << ", " << f1_mat << ", " << i1_mat << endl<<endl;

    // A2_inv_demi*A1
    double A2invdemi_A1_a = a1_mat*a2_inv_demi + b1_mat*b2_inv_demi + c1_mat*c2_inv_demi;
    double A2invdemi_A1_b = b1_mat*a2_inv_demi + e1_mat*b2_inv_demi + f1_mat*c2_inv_demi;
    double A2invdemi_A1_c = c1_mat*a2_inv_demi + f1_mat*b2_inv_demi + i1_mat*c2_inv_demi;
    double A2invdemi_A1_d = a1_mat*b2_inv_demi + b1_mat*e2_inv_demi + c1_mat*f2_inv_demi;
    double A2invdemi_A1_e = b1_mat*b2_inv_demi + e1_mat*e2_inv_demi + f1_mat*f2_inv_demi;
    double A2invdemi_A1_f = c1_mat*b2_inv_demi + f1_mat*e2_inv_demi + i1_mat*f2_inv_demi;
    double A2invdemi_A1_g = a1_mat*c2_inv_demi + b1_mat*f2_inv_demi + c1_mat*i2_inv_demi;
    double A2invdemi_A1_h = b1_mat*c2_inv_demi + e1_mat*f2_inv_demi + f1_mat*i2_inv_demi;
    double A2invdemi_A1_i = c1_mat*c2_inv_demi + f1_mat*f2_inv_demi + i1_mat*i2_inv_demi;
    //cout << "A2_inv_demi*A1 :" << endl;
    //cout << A2invdemi_A1_a << ", " << A2invdemi_A1_b << ", " << A2invdemi_A1_c << endl;
    //cout << A2invdemi_A1_d << ", " << A2invdemi_A1_e << ", " << A2invdemi_A1_f << endl;
    //cout << A2invdemi_A1_g << ", " << A2invdemi_A1_h << ", " << A2invdemi_A1_i << endl<<endl;


    // initialize y
    double y[3];
    y[0] = a2_demi*ellipsoid2[0] + b2_demi*ellipsoid2[1] + c2_demi*ellipsoid2[2] ;
    y[1] = b2_demi*ellipsoid2[0] + e2_demi*ellipsoid2[1] + f2_demi*ellipsoid2[2] ;
    y[2] = c2_demi*ellipsoid2[0] + f2_demi*ellipsoid2[1] + i2_demi*ellipsoid2[2] ;
    //cout << "y = " << y[0] << ", " << y[1] << ", " << y[2] << endl;

    double A2demi_cen2[3]; A2demi_cen2[0]=y[0]; A2demi_cen2[1]=y[1]; A2demi_cen2[2]=y[2];

    // initialize Gradient
    double A2invdemi_y_center1[3];
    A2invdemi_y_center1[0] = a2_inv_demi*y[0] + b2_inv_demi*y[1] + c2_inv_demi*y[2] -ellipsoid1[0];
    A2invdemi_y_center1[1] = b2_inv_demi*y[0] + e2_inv_demi*y[1] + f2_inv_demi*y[2] -ellipsoid1[1];
    A2invdemi_y_center1[2] = c2_inv_demi*y[0] + f2_inv_demi*y[1] + i2_inv_demi*y[2] -ellipsoid1[2];

    double grad[3];
    grad[0] = 2.0*(A2invdemi_A1_a*A2invdemi_y_center1[0]+A2invdemi_A1_b*A2invdemi_y_center1[1]+A2invdemi_A1_c*A2invdemi_y_center1[2]);
    grad[1] = 2.0*(A2invdemi_A1_d*A2invdemi_y_center1[0]+A2invdemi_A1_e*A2invdemi_y_center1[1]+A2invdemi_A1_f*A2invdemi_y_center1[2]);
    grad[2] = 2.0*(A2invdemi_A1_g*A2invdemi_y_center1[0]+A2invdemi_A1_h*A2invdemi_y_center1[1]+A2invdemi_A1_i*A2invdemi_y_center1[2]);
    //cout << "grad = " << grad[0] << ", " << grad[1] << ", " << grad[2] << endl;
    bool condition=true;
    int i=1;
    //cout << "DoesEllipsoidIntersect->start while " << endl;
    double y_old[3]; double norm, norm_stop;
    while (condition){
        y_old[0]=y[0]; y_old[1]=y[1]; y_old[2]=y[2];
        //cout << "i="<<i<<endl;
        //-- Gradient descent
        y[0]=y[0]-grad[0]*2.0/(L1+mu);
        y[1]=y[1]-grad[1]*2.0/(L1+mu);
        y[2]=y[2]-grad[2]*2.0/(L1+mu);
        //cout << "y = " << y[0] << ", " << y[1] << ", " << y[2] << endl;

        //-- Projection of x on the ellipsoid 2
        norm = sqrt( pow(y[0]-A2demi_cen2[0], 2.0)+pow(y[1]-A2demi_cen2[1], 2.0)+pow(y[2]-A2demi_cen2[2], 2.0) );
        if ( norm>1.0 ){
            y[0] = A2demi_cen2[0] + (y[0]-A2demi_cen2[0])/norm;
            y[1] = A2demi_cen2[1] + (y[1]-A2demi_cen2[1])/norm;
            y[2] = A2demi_cen2[2] + (y[2]-A2demi_cen2[2])/norm;
        }
        //-- Gradient
        // A2invdemi_y_center1
        A2invdemi_y_center1[0] = a2_inv_demi*y[0] + b2_inv_demi*y[1] + c2_inv_demi*y[2] -ellipsoid1[0];
        A2invdemi_y_center1[1] = b2_inv_demi*y[0] + e2_inv_demi*y[1] + f2_inv_demi*y[2] -ellipsoid1[1];
        A2invdemi_y_center1[2] = c2_inv_demi*y[0] + f2_inv_demi*y[1] + i2_inv_demi*y[2] -ellipsoid1[2];
        // grad
        grad[0] = 2.0*(A2invdemi_A1_a*A2invdemi_y_center1[0]+A2invdemi_A1_b*A2invdemi_y_center1[1]+A2invdemi_A1_c*A2invdemi_y_center1[2]);
        grad[1] = 2.0*(A2invdemi_A1_d*A2invdemi_y_center1[0]+A2invdemi_A1_e*A2invdemi_y_center1[1]+A2invdemi_A1_f*A2invdemi_y_center1[2]);
        grad[2] = 2.0*(A2invdemi_A1_g*A2invdemi_y_center1[0]+A2invdemi_A1_h*A2invdemi_y_center1[1]+A2invdemi_A1_i*A2invdemi_y_center1[2]);
        //cout << "grad = " << grad[0] << ", " << grad[1] << ", " << grad[2] << endl;

        //-- Update the condition
        norm_stop = ( pow(y[0]-y_old[0], 2) + pow(y[1]-y_old[1], 2) + pow(y[2]-y_old[2], 2) );
        //cout << "norm val = " << norm_stop << endl;
        if (sqrt(norm_stop) < 0.000001 ){
            condition=false;
        }
        i++;
    }
    //cout << "i = " << i << endl;
    // Value of the level set which satisfy the constraint
    double x_c1[3];
    x_c1[0] = a2_inv_demi*y[0] + b2_inv_demi*y[1] + c2_inv_demi*y[2] -ellipsoid1[0];
    x_c1[1] = b2_inv_demi*y[0] + e2_inv_demi*y[1] + f2_inv_demi*y[2] -ellipsoid1[1];
    x_c1[2] = c2_inv_demi*y[0] + f2_inv_demi*y[1] + i2_inv_demi*y[2] -ellipsoid1[2];

    double xinter[3];
    xinter[0] = a1_mat*x_c1[0] + b1_mat*x_c1[1] + c1_mat*x_c1[2];
    xinter[1] = b1_mat*x_c1[0] + e1_mat*x_c1[1] + f1_mat*x_c1[2];
    xinter[2] = c1_mat*x_c1[0] + f1_mat*x_c1[1] + i1_mat*x_c1[2];


    double lambda_min = x_c1[0]*xinter[0] + x_c1[1]*xinter[1] + x_c1[2]*xinter[2] -levelSet;

    if ( lambda_min<0 )
        return true;
    else
        return false;
}

// //////////////////////////////////////////////// //
//                  blPoint2D
// //////////////////////////////////////////////// //
blPoint2D::blPoint2D(float x, float y){
    m_x = x;
    m_y=y;
}
blPoint2D::blPoint2D(){

}

void blPoint2D::setCoords(float x, float y){
    m_x = x;
    m_y=y;
}
void blPoint2D::setX(float x){
    m_x = x;
}
void blPoint2D::setY(float y){
    m_y = y;
}
float blPoint2D::x(){
    return m_x;
}
float blPoint2D::y(){
    return m_y;
}

// //////////////////////////////////////////////// //
//                  blPolygon
// //////////////////////////////////////////////// //
blPolygon::blPolygon(){
    m_center = blPoint2D(0,0);
    m_umSides = 0;
}
void blPolygon::addVertex(blPoint2D point){
    m_points.push_back(point);
    m_umSides++;
    // update center
    m_center.setX((m_center.x()*(m_umSides-1) + point.x())/m_umSides);
    m_center.setY((m_center.y()*(m_umSides-1) + point.y())/m_umSides);
}

int blPolygon::numSides(){
    return m_umSides;
}
blPoint2D blPolygon::center(){
    return m_center;
}
blPoint2D blPolygon::pointAt(int pos){
    return m_points.at(pos);
}


/* Tests for a collision between any two convex polygons.
 * Returns false if they do not intersect
 * Returns true if they intersect
 */
bool blOverlapTests::polypolyIntersect(blPolygon *a, blPolygon *b)
{
    blPoint2D axis;
    float tmp, minA, maxA, minB, maxB;
    int side, i;

    /* test polygon A's sides */
    for (side = 0; side < a->numSides(); side++)
    {
        /* get the axis that we will project onto */
        if (side == 0)
        {
            axis.setX( a->pointAt(a->numSides() - 1).y() - a->pointAt(0).y() );
            axis.setY( a->pointAt(0).x() - a->pointAt(a->numSides() - 1).x() );
        }
        else
        {
            axis.setX( a->pointAt(side - 1).y() - a->pointAt(side).y() );
            axis.setY( a->pointAt(side).x() - a->pointAt(side - 1).x() );
        }

        /* normalize the axis */
        tmp = sqrt(axis.x() * axis.x() + axis.y() * axis.y());
        axis.setX( axis.x() / tmp );
        axis.setY( axis.y() / tmp );

        /* project polygon A onto axis to determine the min/max */
        minA = maxA = a->pointAt(0).x() * axis.x() + a->pointAt(0).y() * axis.y();
        for (i = 1; i < a->numSides(); i++)
        {
            tmp = a->pointAt(i).x() * axis.x() + a->pointAt(i).y() * axis.y();
            if (tmp > maxA)
                maxA = tmp;
            else if (tmp < minA)
                minA = tmp;
        }
        /* correct for offset */
        tmp = a->center().x() * axis.x() + a->center().y() * axis.y();
        minA += tmp;
        maxA += tmp;

        /* project polygon B onto axis to determine the min/max */
        minB = maxB = b->pointAt(0).x() * axis.x() + b->pointAt(0).y() * axis.y();
        for (i = 1; i < b->numSides(); i++)
        {
            tmp = b->pointAt(i).x() * axis.x() + b->pointAt(i).y() * axis.y();
            if (tmp > maxB)
                maxB = tmp;
            else if (tmp < minB)
                minB = tmp;
        }
        /* correct for offset */
        tmp = b->center().x() * axis.x() + b->center().y() * axis.y();
        minB += tmp;
        maxB += tmp;

        /* test if lines intersect, if not, return false */
        if (maxA < minB || minA > maxB)
            return false;
    }

    /* test polygon B's sides */
    for (side = 0; side < b->numSides(); side++)
    {
        /* get the axis that we will project onto */
        if (side == 0)
        {
            axis.setX( b->pointAt(b->numSides() - 1).y() - b->pointAt(0).y() );
            axis.setY( b->pointAt(0).x() - b->pointAt(b->numSides() - 1).x() );
        }
        else
        {
            axis.setX( b->pointAt(side - 1).y() - b->pointAt(side).y() );
            axis.setY( b->pointAt(side).x() - b->pointAt(side - 1).x() );
        }

        /* normalize the axis */
        tmp = sqrt(axis.x() * axis.x() + axis.y() * axis.y());
        axis.setX( axis.x() / tmp);
        axis.setY( axis.y() / tmp );

        /* project polygon A onto axis to determine the min/max */
        minA = maxA = a->pointAt(0).x() * axis.x() + a->pointAt(0).y() * axis.y();
        for (i = 1; i < a->numSides(); i++)
        {
            tmp = a->pointAt(i).x() * axis.x() + a->pointAt(i).y() * axis.y();
            if (tmp > maxA)
                maxA = tmp;
            else if (tmp < minA)
                minA = tmp;
        }
        /* correct for offset */
        tmp = a->center().x() * axis.x() + a->center().y() * axis.y();
        minA += tmp;
        maxA += tmp;

        /* project polygon B onto axis to determine the min/max */
        minB = maxB = b->pointAt(0).x() * axis.x() + b->pointAt(0).y() * axis.y();
        for (i = 1; i < b->numSides(); i++)
        {
            tmp = b->pointAt(i).x() * axis.x() + b->pointAt(i).y() * axis.y();
            if (tmp > maxB)
                maxB = tmp;
            else if (tmp < minB)
                minB = tmp;
        }
        /* correct for offset */
        tmp = b->center().x() * axis.x() + b->center().y() * axis.y();
        minB += tmp;
        maxB += tmp;

        /* test if lines intersect, if not, return false */
        if (maxA < minB || minA > maxB)
            return false;
    }

    return true;
}
