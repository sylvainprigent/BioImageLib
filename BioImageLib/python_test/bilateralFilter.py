import blCore
import blFiltering

# load image
image = blCore.blImage('/home/sylvain/codes/bioImageLib/python_test/orchide.tif');


# loop on the pixels for threshold
if ( image.pixelType() == "int" and image.is2D() ):
    for i in range(image.xSize()):
        for j in range(image.ySize()):
            if image.getPixelInt(i,j) <= 100:
                image.setPixelInt(i,j, 0);
            else:
                image.setPixelInt(i,j,255);
              
  
image.save('/home/sylvain/codes/bioImageLib/python_test/py_th.tif')

# call a biolib process
mfilter = blFiltering.blMedianFilter();
mfilter.setRadius(12);
mfilter.setInput(image);
mfilter.run();
medImage = mfilter.output();

medImage.save('/home/sylvain/codes/bioImageLib/python_test/median.tif');

