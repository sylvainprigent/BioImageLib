
#include "blImage.h"

int main(void){

    // load image
    blImage* image = new blImage("/home/sylvain/Desktop/particles_1.tif");


    // save image
    image->save("/home/sylvain/Desktop/particles_1_copy.tif", false);

}
