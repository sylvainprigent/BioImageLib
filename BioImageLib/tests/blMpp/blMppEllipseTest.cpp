//#include <blCore/blColor.h>
#include <iostream>

int blMppEllipseTest(int argc, char* argv[])
{

  std::cout<<"Testing started.."<<std::endl;
  int i = 0;

  std::vector<int> vector = Color::hsv_to_rgb(0,0,0);
  if (vector.size()!=3)
    i=1;

  if (vector[0]!=0 && vector[1]!=0 && vector[2]!=0)
    i=1;


  return 0;
}

