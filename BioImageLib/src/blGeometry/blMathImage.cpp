/// \file blMathImage.cpp
/// \brief blMathImage class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMathImage.h"
#include "blMathGeometry.h"

std::vector<float> blMathImage::CalculatePixelsValuesInsideSphere(const std::vector<float>& sphere, const Float3DImage::Pointer& image)
{
    Float3DImage::IndexType pixelIndex;
    std::vector<float> values;
    double euclid;

    int m_nl = image->GetLargestPossibleRegion().GetSize()[0];
    int m_nc = image->GetLargestPossibleRegion().GetSize()[1];
    int m_nz = image->GetLargestPossibleRegion().GetSize()[2];

    if (sphere[0] - sphere[3] < 0 || sphere[0] + sphere[3] > m_nl){
        return values;
    }
    if (sphere[1] - sphere[3] < 0 || sphere[1] + sphere[3] > m_nc){
        return values;
    }
    if (sphere[2] - sphere[3] < 0 || sphere[2] + sphere[3] > m_nz){
        return values;
    }

    for (int x = sphere[0] - sphere[3]; x <= sphere[0] + sphere[3] ; ++x)
    {
        for (int y = sphere[1] - sphere[3]; y <= sphere[1] + sphere[3] ; ++y)
        {
            for (int z = sphere[2] - sphere[3]; z <= sphere[2] + sphere[3] ; ++z)
            {
                euclid = pow(x - sphere[0],2) + pow(y - sphere[1],2) + pow(z - sphere[2],2);
                if ( euclid <= sphere[3]*sphere[3] )
                {
                    pixelIndex[0] = x;
                    pixelIndex[1] = y;
                    pixelIndex[2] = z;
                    values.push_back( image->GetPixel(pixelIndex) );
                }
            }
        }
    }
    return values;
}

std::vector<float> blMathImage::CalculatePixelsValuesInsideDiskIn3D(const std::vector<float>& object, const Float3DImage::Pointer& image)
{
    std::vector<float> values;
    std::vector<int> xp, yp;
    for (int r=0; r< int(object[3]) ; ++r)
    {
        blMathGeometry::CalculateCircleCoordinates( object[0], object[1], r, xp, yp );
    }

    Float3DImage::IndexType pixelIndex;
    for (unsigned int i = 0 ; i < xp.size() ; ++i)
    {
        pixelIndex[0] = xp[i];
        pixelIndex[1] = yp[i];
        pixelIndex[2] = object[2];
        values.push_back(image->GetPixel(pixelIndex));
    }
    return values;
}

