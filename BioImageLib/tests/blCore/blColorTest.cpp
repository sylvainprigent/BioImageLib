#include <blCore/blColor.h>
#include <iostream>

int main(int argc, char* argv[])
{ 
  std::vector<int> vector;

  // Size and konwn conversion
  vector = blColor::hsv_to_rgb(0,0,0);
  if (vector.size()!=3)
    return 1; 
  if (vector[0]!=0 && vector[1]!=0 && vector[2]!=0)
    return 2;
  
  //std::vector<int> vector2 = blColor::hsv_to_rgb(359,100,100);
  vector = blColor::hsv_to_rgb(0.2,0.95,0.95);
  if (vector[0]!=196 && vector[1]!=243 && vector[2]!=12)
    return 3;

  vector = blColor::hsv_to_rgb(0.6,0.53,0.54);
  if (vector[0]!=64 && vector[1]!=94 && vector[2]!=138)
    return 4;

  vector = blColor::hsv_to_rgb(0.32,0.95,0.12);
  if (vector[0]!=3 && vector[1]!=30 && vector[2]!=1)
    return 5;

  // ERROR Cases
  // HSV superior too larges value : return 0 0 0
  vector = blColor::hsv_to_rgb(360,100,100);
  if (vector[0]!=0 && vector[1]!=0 && vector[2]!=0)
    return 6;

  vector = blColor::hsv_to_rgb(359,101,100);
  if (vector[0]!=0 && vector[1]!=0 && vector[2]!=0)
    return 7;

  vector = blColor::hsv_to_rgb(359,100,101);
  if (vector[0]!=0 && vector[1]!=0 && vector[2]!=0)
    return 8;

  // HSV negative value : return 0 0 0, sauf al dernière ou c'est -256 -256 -256
  vector = blColor::hsv_to_rgb(-1,0,0);
  if (vector[0]!=0 && vector[1]!=0 && vector[2]!=0)
    return 9;

  vector = blColor::hsv_to_rgb(0,-1,0);
  if (vector[0]!=0 && vector[1]!=0 && vector[2]!=0)
    return 10;

  vector = blColor::hsv_to_rgb(0,0,-1);
  if (vector[0]!=-256 && vector[1]!=-256 && vector[2]!=-256)
    return 11;

  return 0;
}

