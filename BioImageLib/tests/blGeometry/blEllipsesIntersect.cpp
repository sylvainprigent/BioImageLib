#include <blGeometry/blMathGeometry.h>
#include <iostream>
#include <vector>

int main(int argc, char* argv[])
{ 
  // ** TEST NON INTERESECTION **
  std::vector<float> ellipse1(5);
  ellipse1[0] = 0; 
  ellipse1[1] = 0;
  ellipse1[2] = 3;
  ellipse1[3] = 4;
  ellipse1[4] = 0;
  
  std::vector<float> ellipse2(5);
  ellipse2[0] = 10;
  ellipse2[1] = 10;
  ellipse2[2] = 2;
  ellipse2[3] = 4;
  ellipse2[4] = 0;
  
  if(blMathGeometry::DoesEllipsesIntersect(ellipse1, ellipse2))
    return 1;
  
  // ** TEST INTERESECTION **
  ellipse1[0] = 75; 
  ellipse1[1] = 162;
  ellipse1[2] = 8;
  ellipse1[3] = 23;
  ellipse1[4] = 2.6923;

  ellipse2[0] = 49;
  ellipse2[1] = 150;
  ellipse2[2] = 6;
  ellipse2[3] = 24;
  ellipse2[4] = 3.02232;

  if(!blMathGeometry::DoesEllipsesIntersect(ellipse1, ellipse2))
    return 1;
  
  // ** TEST INTERESECTION **
  ellipse1[0] = 0; 
  ellipse1[1] = 0;
  ellipse1[2] = 7;
  ellipse1[3] = 6;
  ellipse1[4] = 0;

  ellipse2[0] = 5;
  ellipse2[1] = 5;
  ellipse2[2] = 6;
  ellipse2[3] = 8;
  ellipse2[4] = 0;
  if(!blMathGeometry::DoesEllipsesIntersect(ellipse1, ellipse2))
    return 1;

  // ** TEST EGAL **
  ellipse1[0] = 0; 
  ellipse1[1] = 0;
  ellipse1[2] = 7;
  ellipse1[3] = 6;
  ellipse1[4] = 0;

  ellipse2[0] = 0;
  ellipse2[1] = 0;
  ellipse2[2] = 7;
  ellipse2[3] = 6;
  ellipse2[4] = 0;
  if(!blMathGeometry::DoesEllipsesIntersect(ellipse1, ellipse2))
    return 1;

  // ** TEST include **
  ellipse1[0] = 0; 
  ellipse1[1] = 0;
  ellipse1[2] = 7;
  ellipse1[3] = 6;
  ellipse1[4] = 0;

  ellipse2[0] = 0;
  ellipse2[1] = 0;
  ellipse2[2] = 4;
  ellipse2[3] = 3;
  ellipse2[4] = 0;
  if(!blMathGeometry::DoesEllipsesIntersect(ellipse1, ellipse2))
    return 1;

  
  return 0;
}
