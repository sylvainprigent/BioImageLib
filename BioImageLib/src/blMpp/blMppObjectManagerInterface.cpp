#include "blMppObjectManagerInterface.h"
#include <blCore>

// itk
#include "itkImageRegionIterator.h"
#include "itkRescaleIntensityImageFilter.h"

#include <blImageProcessing>

using namespace std;

// ////////////////////////////////////////////////////////////////// //
//                        blMppObjectManagerInterface
// ////////////////////////////////////////////////////////////////// //
blMppObjectManagerInterface::blMppObjectManagerInterface(): blParameterInterface(){
    m_useMask = false;
    m_use2DMaskIn3D = false;
    m_use3D = false;
    m_mapFromTh = false;
    initParametersWithDefaultsValues();
}

blMppObjectManagerInterface::blMppObjectManagerInterface(blParameterManager *paramManager) : blParameterInterface(paramManager){
}

blMppObjectManagerInterface::blMppObjectManagerInterface(blParameterManager *paramManager, Float2DImage::Pointer inputImage) : blParameterInterface(paramManager){
    SetImagePointer(inputImage);
}

blMppObjectManagerInterface::blMppObjectManagerInterface(blParameterManager *paramManager, Float3DImage::Pointer inputImage) : blParameterInterface(paramManager){
    SetImagePointer(inputImage);
}

blMppObjectManagerInterface::~blMppObjectManagerInterface(){

}

// //////////////////////////////// parameters /////////////////////////////////////////////
void blMppObjectManagerInterface::initParametersWithDefaultsValues(){
    m_birthMapThresholdInf = -1;
    m_birthMapThresholdSup = -1;
    m_borderWidth = 1;
    m_overlapCoef = 0.0;
    m_ObjectRepresentationType = "border";
}

bool blMppObjectManagerInterface::checkParameters(){
    if (m_paramManager){
        m_birthMapThresholdInf = m_paramManager->getValueOfKey<float>("blMppObjectManager_birthThresholdInf", -1);
        m_birthMapThresholdSup = m_paramManager->getValueOfKey<float>("blMppObjectManager_birthThresholdSup", -1);
        m_borderWidth = m_paramManager->getValueOfKey<int>("blMppObjectManager_borderWidth", 1);
        m_overlapCoef = m_paramManager->getValueOfKey<float>("blMppObjectManager_overlapCoef", 0.0);
        m_ObjectRepresentationType = m_paramManager->getValueOfKey<string>("blMppObjectManager_objectRepresentationType", "border");
    }
    else{
        m_paramMessage = "blMppObjectManager -> Cannot load the parameters";
        return false;
    }

    if (m_borderWidth < 1){
        m_paramMessage = "MppObjectManager -> borderWidth must be greater than 1";
        return false;
    }
    if (m_overlapCoef < 0 || m_overlapCoef > 1.0){
        m_paramMessage = "MppObjectManager -> overlapCoef must be in [0,1]";
        return false;
    }
    return true;
}

// /////////////////////////////// getters/setters ////////////////////////////////
Float2DImage::Pointer blMppObjectManagerInterface::GetImagePointer(){
    return m_image;
}

Float3DImage::Pointer blMppObjectManagerInterface::Get3DImagePointer(){
    return m_image3D;
}

int blMppObjectManagerInterface::GetImageRowsNb(){
    return m_nl;
}

int blMppObjectManagerInterface::GetImageColumnsNb(){
    return m_nc;
}

int blMppObjectManagerInterface::GetImageLayerNb(){
    return m_nz;
}

bool blMppObjectManagerInterface::IsImage3D(){
    return m_use3D;
}

void blMppObjectManagerInterface::SetImagePointer(Float2DImage::Pointer image, bool useParamBurthMap){
    m_image = image;
    m_nl = image->GetLargestPossibleRegion().GetSize()[0];
    m_nc = image->GetLargestPossibleRegion().GetSize()[1];
    //cout << "image size = " << m_nl << " * " << m_nc << endl;

    // Create a birth mask
    if (useParamBurthMap){
        m_useMask = false;
        if (m_birthMapThresholdInf != -1 || m_birthMapThresholdSup != -1){

            m_useMask = true;
            m_mapFromTh = true;
            blThreshold<PixelFloatType,Image2D> thresholder;
            thresholder.initParametersWithDefaultsValues();
            if (m_birthMapThresholdInf == -1){
                thresholder.setLowBoundary(0);
            }
            else{
                thresholder.setLowBoundary(m_birthMapThresholdInf);
            }
            if (m_birthMapThresholdSup == -1){
                thresholder.setHighBoundary(999999);
            }
            else{
                thresholder.setHighBoundary(m_birthMapThresholdInf);
            }
            blImageTemplate<PixelFloatType,Image2D>* imageIn = new blImageTemplate<PixelFloatType,Image2D>(image);
            blImageTemplate<PixelFloatType,Image2D> imageOut = thresholder.compute(imageIn);
            this->SetMaskPointer(imageOut.itkImagePointer());
        }
    }
    m_use3D = false;
}

void blMppObjectManagerInterface::SetImagePointer(Float3DImage::Pointer image, bool useParamBurthMap){
    m_use3D = true;
    m_image3D = image;
    m_nl = image->GetLargestPossibleRegion().GetSize()[0];
    m_nc = image->GetLargestPossibleRegion().GetSize()[1];
    m_nz = image->GetLargestPossibleRegion().GetSize()[2];
    //cout << "image size = " << m_nl << ", "<< m_nc << ", "<< m_nz << endl;

    // Create a birth mask
    if (useParamBurthMap){
        if (m_birthMapThresholdInf != -1 || m_birthMapThresholdSup != -1){
            m_mapFromTh = true;
            m_useMask = true;

            blThreshold<PixelFloatType,Image3D> thresholder;
            thresholder.initParametersWithDefaultsValues();
            if (m_birthMapThresholdInf == -1){
                thresholder.setLowBoundary(0);
            }
            else{
                thresholder.setLowBoundary(m_birthMapThresholdInf);
            }
            if (m_birthMapThresholdSup == -1){
                thresholder.setHighBoundary(999999);
            }
            else{
                thresholder.setHighBoundary(m_birthMapThresholdInf);
            }
            blImageTemplate<PixelFloatType,Image3D>* imageIn = new blImageTemplate<PixelFloatType,Image3D>(image);
            blImageTemplate<PixelFloatType,Image3D> imageOut = thresholder.compute(imageIn);
            this->SetMaskPointer(imageOut.itkImagePointer());

        }
    }

    // Create MIP image for output
    m_image = blIntegrateImage::Integrate3DToMip(m_image3D);
}

bool blMppObjectManagerInterface::IsOverlaping(vector<float> object1, vector< vector<float> > object2){
    for (unsigned int i = 0 ; i < object2.size() ; ++i){
        if (IsOverlaping(object1, object2.at(i))){
            return true;
        }
    }
    return false;
}

vector<int> blMppObjectManagerInterface::FindOverlaping(vector<float> object1, vector< vector<float> > object2){
    vector<int> posOverlap;
    //#if defined(_OPENMP)
    //#pragma omp parallel for
    //#endif
    for (unsigned int i = 0 ; i < object2.size() ; ++i){
        if (IsOverlaping(object1, object2.at(i))){
            //#if defined(_OPENMP)
            //#pragma omp critical
            //#endif
            posOverlap.push_back(i);
        }
    }
    return posOverlap;
}

string blMppObjectManagerInterface::GetObjectName(){
    return m_objectName;
}

void blMppObjectManagerInterface::SaveObjectsOnImage(vector<vector<float> > objects, string filePath){
    if (m_use3D){
        FloatRGB3DImage::Pointer image = Draw3DObjectsOnImage(objects);
        blImageIO::SaveFloatRGB3DImage(filePath, image);
    }
    else{
        FloatRGBV2DImage::Pointer image = DrawObjectsOnImage(objects);
        blImageIO::SaveFloatRGBV2DImage(filePath, image);
    }
}

void blMppObjectManagerInterface::SaveResultAsBirthMap(vector<vector<float> > objects, string filePath){
    if (m_use3D){
        Float3DImage::Pointer image = Draw3DObjectsAsBithMap(objects);
        blImageIO::SaveFloat3DImage(filePath, image);
    }
    else{
        Float2DImage::Pointer image = DrawObjectsAsBithMap(objects);
        blImageIO::SaveFloat2DImage(filePath, image);
    }
}

Float3DImage::Pointer blMppObjectManagerInterface::Draw3DObjectsAsBithMap(vector< vector<float> > objects, Float3DImage::Pointer imageRes){

    if (objects.size() == 0){
        return imageRes;
    }
    else{
        // draw the objects
        Float3DImage::PixelType pixel;
        Float3DImage::IndexType pixelIndex;
        vector<vector<int> > pix;
        pixel = 255;
        for (unsigned int i = 0 ; i < objects.size() ; ++i ){
            pix.clear();
            this->GetInsidePixelsCoordinates(objects[i], pix);
            if (pix[0].size()<3){
                for (unsigned int k = 0 ; k < pix.size() ; k++){
                    pixelIndex[0] = pix[k][0];
                    pixelIndex[1] = pix[k][1];
                    pixelIndex[2] = objects[i][2];
                    imageRes->SetPixel(pixelIndex, pixel);
                }
            }
            else{
                for (unsigned int k = 0 ; k < pix.size() ; k++){
                    pixelIndex[0] = pix[k][0];
                    pixelIndex[1] = pix[k][1];
                    pixelIndex[2] = pix[k][2];
                    imageRes->SetPixel(pixelIndex, pixel);
                }
            }
        }
    }
    return imageRes;
}

Float3DImage::Pointer blMppObjectManagerInterface::Draw3DObjectsAsBithMap(vector< vector<float> > objects){

    Float3DImage::Pointer imageRes = blCreateImage::createNewFloat3DImage(m_nl, m_nc, m_nz);
    return Draw3DObjectsAsBithMap(objects, imageRes);
}


FloatRGB3DImage::Pointer blMppObjectManagerInterface::Calculate3DOutputImage(){
    Float3DImage::Pointer imageRes = m_image3D;

    // normalize intensity
    typedef itk::RescaleIntensityImageFilter< Float3DImage, Float3DImage > RescaleFilterType;
    RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
    rescaleFilter->SetInput(imageRes);
    rescaleFilter->SetOutputMinimum(0);
    rescaleFilter->SetOutputMaximum(255);
    rescaleFilter->Update();
    imageRes = rescaleFilter->GetOutput();

    FloatRGB3DImage::Pointer resImage = blCreateImage::createNewFloatRGB3DImage(m_nl, m_nc, m_nz);

    // copy the input image to the rgb output
    typedef itk::ImageRegionIterator<FloatRGB3DImage> IteratorType;
    typedef itk::ImageRegionIterator<Float3DImage> IteratorTypeFloat;
    IteratorType iteratorResImage(resImage, resImage->GetLargestPossibleRegion());
    IteratorTypeFloat iteratorInitImage(imageRes, imageRes->GetLargestPossibleRegion());

    itk::RGBPixel<float> pixel;
    while(!iteratorInitImage.IsAtEnd()){
        pixel[0] = iteratorInitImage.Get();
        pixel[1] = pixel[0]; pixel[2] = pixel[0];
        iteratorResImage.Set(pixel);
        ++iteratorResImage;
        ++iteratorInitImage;
    }
	return resImage;
}

FloatRGB3DImage::Pointer blMppObjectManagerInterface::Draw3DObjectsOnImage(vector< vector<float> > objects, FloatRGB3DImage::Pointer resImage, const vector<int>& color){
    if (objects.size() == 0){
        return resImage;
    }
    else{

        // draw the objects
        FloatRGB3DImage::IndexType pixelIndex;
        vector<vector<int> > pix;
        itk::RGBPixel<float> pixel;
        if (color.empty()){
            pixel[0] = 255; pixel[1] = 0; pixel[2] = 0;
        }
        else{
            pixel[0] = color[0]; pixel[1] = color[1]; pixel[2] = color[2];
        }
        for (unsigned int i = 0 ; i < objects.size() ; ++i ){
            pix.clear();
            GetObjectRepresentation(objects[i], pix);
            if (pix[0].size()<3){
                for (unsigned int k = 0 ; k < pix.size() ; k++){
                    pixelIndex[0] = pix[k][0];
                    pixelIndex[1] = pix[k][1];
                    pixelIndex[2] = objects[i][2];
                    resImage->SetPixel(pixelIndex, pixel);
                }
            }
            else{
                for (unsigned int k = 0 ; k < pix.size() ; k++){
                    //if (pix[k][0] > -1 && pix[k][0] < m_nl && pix[k][1] > -1 && pix[k][1] < m_nc ){
                    pixelIndex[0] = pix[k][0];
                    pixelIndex[1] = pix[k][1];
                    pixelIndex[2] = pix[k][2];
                    resImage->SetPixel(pixelIndex, pixel);
                    //}
                }
            }
        }
        return resImage;
    }
}

FloatRGB3DImage::Pointer blMppObjectManagerInterface::Draw3DObjectsOnImage(vector< vector<float> > objects, const vector<int>& color){

    FloatRGB3DImage::Pointer resImage = Calculate3DOutputImage();
    Draw3DObjectsOnImage(objects, resImage, color);
	return resImage;
}


vector< vector<float> > blMppObjectManagerInterface::CalculateObjectsStatistics(vector< vector<float> > objects){
    vector<float> inside_values;
    float mini, maxi, mean, vari;
    for (unsigned int i = 0 ; i < objects.size() ; ++i){
        inside_values.clear();
        this->GetInsidePixelsValues(objects[i], inside_values);
        blMathVector::minMax(inside_values, mini, maxi);
        mean = blMathVector::mean(inside_values);
        vari = blMathVector::var(inside_values);

        objects[i].push_back(mini);
        objects[i].push_back(maxi);
        objects[i].push_back(mean);
        objects[i].push_back(vari);
    }
    return objects;
}

vector<blMppObjectStatistics> blMppObjectManagerInterface::GetObjectsStatistics(vector< vector<float> > objects){

    vector<blMppObjectStatistics> objectsStats; objectsStats.resize(objects.size());
    vector<float> inside_values;
    float mini, maxi, mean, vari;
    for (unsigned int i = 0 ; i < objects.size() ; ++i){
        inside_values.clear();
        this->GetInsidePixelsValues(objects[i], inside_values);
        blMathVector::minMax(inside_values, mini, maxi);
        mean = blMathVector::mean(inside_values);
        vari = blMathVector::var(inside_values);

        objectsStats[i].setMinIntensity(mini);
        objectsStats[i].setMaxIntensity(maxi);
        objectsStats[i].setMeanIntensity(mean);
        objectsStats[i].setVarIntensity(vari);
    }
    return objectsStats;
}


Float2DImage::Pointer blMppObjectManagerInterface::DrawObjectsAsBithMap(vector< vector<float> > objects, Float2DImage::Pointer imageRes){
    if (objects.size() == 0){
        return imageRes;
    }
    else{
        // draw the objects
        Float2DImage::PixelType pixel;
        Float2DImage::IndexType pixelIndex;
        vector<vector<int> > pix;
        pixel = 255;
        for (unsigned int i = 0 ; i < objects.size() ; ++i ){
            pix.clear();
            this->GetInsidePixelsCoordinates(objects[i], pix);
            //GetObjectRepresentation(objects[i], pix);
            for (unsigned int k = 0 ; k < pix.size() ; k++){
                if (pix[k][0] > -1 && pix[k][0] < m_nl && pix[k][1] > -1 && pix[k][1] < m_nc ){
                    pixelIndex[0] = pix[k][0];
                    pixelIndex[1] = pix[k][1];
                    imageRes->SetPixel(pixelIndex, pixel);
                }
            }
        }
        return imageRes;
    }
}

Float2DImage::Pointer blMppObjectManagerInterface::DrawObjectsAsBithMap(vector< vector<float> > objects){

    Float2DImage::Pointer imageRes = blCreateImage::createNewFloat2DImage(m_nl,m_nc);
    return DrawObjectsAsBithMap(objects, imageRes);
}

FloatRGBV2DImage::Pointer blMppObjectManagerInterface::CalculateOutputImage(){

    Float2DImage::Pointer imageRes = m_image;

    // normalize intensity
    typedef itk::RescaleIntensityImageFilter< Float2DImage, Float2DImage > RescaleFilterType;
    RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
    rescaleFilter->SetInput(imageRes);
    rescaleFilter->SetOutputMinimum(0);
    rescaleFilter->SetOutputMaximum(255);
    rescaleFilter->Update();
    imageRes = rescaleFilter->GetOutput();

    FloatRGBV2DImage::Pointer resImage = blCreateImage::createNewFloatRGBV2DImage(m_nl, m_nc);

    // copy the input image to the rgb output
    typedef itk::ImageRegionIterator<FloatRGBV2DImage> IteratorType;
    typedef itk::ImageRegionIterator<Float2DImage> IteratorTypeFloat;
    IteratorType iteratorResImage(resImage, resImage->GetLargestPossibleRegion());
    IteratorTypeFloat iteratorInitImage(imageRes, imageRes->GetLargestPossibleRegion());

    itk::Vector<float> pixel(3);
    while(!iteratorInitImage.IsAtEnd()){
        pixel[0] = iteratorInitImage.Get();
        pixel[1] = pixel[0]; pixel[2] = pixel[0];
        iteratorResImage.Set(pixel);
        ++iteratorResImage;
        ++iteratorInitImage;
    }

    return resImage;
}

FloatRGBV2DImage::Pointer blMppObjectManagerInterface::DrawObjectsOnImage(vector< vector<float> > objects, const vector<int>& color){

    FloatRGBV2DImage::Pointer resImage = CalculateOutputImage();
    return DrawObjectsOnImage(objects, resImage, color);

}

FloatRGBV2DImage::Pointer blMppObjectManagerInterface::DrawObjectsOnImage(vector< vector<float> > objects, FloatRGBV2DImage::Pointer image, const vector<int>& color){
    if (objects.size() == 0){
        return image;
    }
    else{
        // draw the objects
        FloatRGBV2DImage::IndexType pixelIndex;
        itk::Vector<float> pixel(3);
        vector<vector<int> > pix;
        if (color.empty()){
            pixel[0] = 255; pixel[1] = 0; pixel[2] = 0;
        }
        else{
            pixel[0] = color[0]; pixel[1] = color[1]; pixel[2] = color[2];
        }
        for (unsigned int i = 0 ; i < objects.size() ; ++i ){
            pix.clear();
            GetObjectRepresentation(objects[i], pix);
            for (unsigned int k = 0 ; k < pix.size() ; k++){
                if (pix[k][0] > -1 && pix[k][0] < m_nl && pix[k][1] > -1 && pix[k][1] < m_nc ){
                    pixelIndex[0] = pix[k][0];
                    pixelIndex[1] = pix[k][1];
                    image->SetPixel(pixelIndex, pixel);
                }
            }
        }
        return image;
    }
}


void blMppObjectManagerInterface::SetMaskPointer(Float2DImage::Pointer mask){
    m_useMask = true;
    m_use2DMaskIn3D = false;
    m_mask = mask;
    cout << "MppObjectManager::SetMaskPointer(Float2DImage::Pointer mask)" << endl;
    if (m_birthMapThresholdInf != -1 || m_birthMapThresholdSup != -1){
        Float2DImage::Pointer maskThreshold;
        blThreshold<PixelFloatType,Image2D> thresholder;
        thresholder.initParametersWithDefaultsValues();
        if (m_birthMapThresholdInf == -1){
            thresholder.setLowBoundary(0);
        }
        else{
            thresholder.setLowBoundary(m_birthMapThresholdInf);
        }
        if (m_birthMapThresholdSup == -1){
            thresholder.setHighBoundary(999999);
        }
        else{
            thresholder.setHighBoundary(m_birthMapThresholdInf);
        }
        blImageTemplate<PixelFloatType,Image2D>* imageIn = new blImageTemplate<PixelFloatType,Image2D>(m_image);
        blImageTemplate<PixelFloatType,Image2D> imageOut = thresholder.compute(imageIn);

        maskThreshold = imageOut.itkImagePointer();

        Float2DImage::IndexType pixelIndex;
        int nl = mask->GetLargestPossibleRegion().GetSize()[0];
        int nc = mask->GetLargestPossibleRegion().GetSize()[1];
        m_birthMap.clear();
        vector<int> inter; inter.resize(2);
        for (int m = 0 ; m < nl; ++m){
            for (int n = 0 ; n < nc ; ++n){
                pixelIndex[0] = m;
                pixelIndex[1] = n;
                if (mask->GetPixel(pixelIndex) > 0 && maskThreshold->GetPixel(pixelIndex) > 0){
                    inter[0] = m;
                    inter[1] = n;
                    m_birthMap.push_back(inter);
                }
            }
        }
    }
    else {
        Float2DImage::IndexType pixelIndex;
        int nl = mask->GetLargestPossibleRegion().GetSize()[0];
        int nc = mask->GetLargestPossibleRegion().GetSize()[1];
        m_birthMap.clear();
        vector<int> inter; inter.resize(2);
        for (int m = 0 ; m < nl; ++m){
            for (int n = 0 ; n < nc ; ++n){
                pixelIndex[0] = m;
                pixelIndex[1] = n;
                if (mask->GetPixel(pixelIndex) > 0){
                    inter[0] = m;
                    inter[1] = n;
                    m_birthMap.push_back(inter);
                }
            }
        }
    }

    // Ramdomize the birthMap
    for (int i = 0 ; i < 9 ; ++i)
        random_shuffle( m_birthMap.begin(), m_birthMap.end() );
    //cout << "size birth map = "<<  m_birthMap.size() << endl;
}

void blMppObjectManagerInterface::SetMaskPointer(Float3DImage::Pointer mask){
    m_useMask = true;
    m_use2DMaskIn3D = false;
    m_3DMask = mask;
    //cout << "MppObjectManager::SetMaskPointer(Float3DImage::Pointer mask)" << endl;
    // Create a birth mask

    if (m_birthMapThresholdInf != -1 || m_birthMapThresholdSup != -1){
        Float3DImage::Pointer maskThreshold;
        blThreshold<PixelFloatType,Image3D> thresholder;
        thresholder.initParametersWithDefaultsValues();
        if (m_birthMapThresholdInf == -1){
            thresholder.setLowBoundary(0);
        }
        else{
            thresholder.setLowBoundary(m_birthMapThresholdInf);
        }
        if (m_birthMapThresholdSup == -1){
            thresholder.setHighBoundary(999999);
        }
        else{
            thresholder.setHighBoundary(m_birthMapThresholdInf);
        }
        blImageTemplate<PixelFloatType,Image3D>* imageIn = new blImageTemplate<PixelFloatType,Image3D>(m_image3D);
        blImageTemplate<PixelFloatType,Image3D> imageOut = thresholder.compute(imageIn);

        maskThreshold = imageOut.itkImagePointer();

        Float3DImage::IndexType pixelIndex;
        int nl = mask->GetLargestPossibleRegion().GetSize()[0];
        int nc = mask->GetLargestPossibleRegion().GetSize()[1];
        int nz = mask->GetLargestPossibleRegion().GetSize()[2];
        m_birthMap.clear();
        //cout << "mask size: nl=" << nl << ", nc=" << nc << ", nz=" << nz << endl;
        vector<int> inter; inter.resize(3);
        for (int m = 0 ; m < nl; ++m){
            for (int n = 0 ; n < nc ; ++n){
                for (int z = 0 ; z < nz ; ++z){
                    pixelIndex[0] = m;
                    pixelIndex[1] = n;
                    pixelIndex[2] = z;
                    if (mask->GetPixel(pixelIndex) > 0 && maskThreshold->GetPixel(pixelIndex) > 0){
                        inter[0] = m;
                        inter[1] = n;
                        inter[2] = z;
                        m_birthMap.push_back(inter);
                    }
                }
            }
        }
    }
    else{
        Float3DImage::IndexType pixelIndex;
        int nl = mask->GetLargestPossibleRegion().GetSize()[0];
        int nc = mask->GetLargestPossibleRegion().GetSize()[1];
        int nz = mask->GetLargestPossibleRegion().GetSize()[2];
        m_birthMap.clear();
        //cout << "mask size: nl=" << nl << ", nc=" << nc << ", nz=" << nz << endl;
        vector<int> inter; inter.resize(3);
        for (int m = 0 ; m < nl; ++m){
            for (int n = 0 ; n < nc ; ++n){
                for (int z = 0 ; z < nz ; ++z){
                    pixelIndex[0] = m;
                    pixelIndex[1] = n;
                    pixelIndex[2] = z;
                    if (mask->GetPixel(pixelIndex) > 0){
                        inter[0] = m;
                        inter[1] = n;
                        inter[2] = z;
                        m_birthMap.push_back(inter);
                    }
                }
            }
        }
    }
    // Ramdomize the birthMap
    for (int i = 0 ; i < 96 ; ++i)
        random_shuffle( m_birthMap.begin(), m_birthMap.end() );
    cout << "size birth map = "<<  m_birthMap.size() << endl;
}

void blMppObjectManagerInterface::Set2DFor3DBirthMapPointer(Float2DImage::Pointer mask){
    m_useMask = true;
    m_use2DMaskIn3D = true;
    m_mask = mask;

    cout << "MppObjectManager::Set2DFor3DBirthMapPointer" << endl;
    Float2DImage::IndexType pixelIndex;
    int nl = m_image3D->GetLargestPossibleRegion().GetSize()[0];
    int nc = m_image3D->GetLargestPossibleRegion().GetSize()[1];
    int nz = m_image3D->GetLargestPossibleRegion().GetSize()[2];
    int nl_mask = mask->GetLargestPossibleRegion().GetSize()[0];
    int nc_mask = mask->GetLargestPossibleRegion().GetSize()[1];
    m_birthMap.clear();
    if (nl != nl_mask || nc != nc_mask){
        throw blException("the mask and the image must be the same size");
    }
    else{
        if (m_birthMapThresholdInf != -1 || m_birthMapThresholdSup != -1){

            float th_inf, th_sup;
            if (m_birthMapThresholdInf == -1){
                th_inf = 0;
            }
            else{
                th_inf = m_birthMapThresholdInf;
            }
            if (m_birthMapThresholdSup == -1){
                th_sup = 999999;
            }
            else{
                th_sup = m_birthMapThresholdSup;
            }

            //cout << "mask size: nl=" << nl << ", nc=" << nc << ", nz=" << nz << endl;
            Float3DImage::IndexType pixelIndexTh;
            vector<int> inter; inter.resize(3);
            for (int m = 0 ; m < nl; ++m){
                for (int n = 0 ; n < nc ; ++n){
                    pixelIndex[0] = m;
                    pixelIndex[1] = n;
                    if (mask->GetPixel(pixelIndex) > 0){
                        for (int z = 0 ; z < nz ; ++z){
                            pixelIndexTh[0] = m;
                            pixelIndexTh[1] = n;
                            pixelIndexTh[2] = z;
                            if (m_image3D->GetPixel(pixelIndexTh) >= th_inf && m_image3D->GetPixel(pixelIndexTh) <= th_sup){
                                inter[0] = m;
                                inter[1] = n;
                                inter[2] = z;
                                m_birthMap.push_back(inter);
                            }
                        }
                    }
                }
            }
        }
        else{
            //cout << "mask size: nl=" << nl << ", nc=" << nc << ", nz=" << nz << endl;
            vector<int> inter; inter.resize(3);
            for (int m = 0 ; m < nl; ++m){
                for (int n = 0 ; n < nc ; ++n){
                    pixelIndex[0] = m;
                    pixelIndex[1] = n;
                    if (mask->GetPixel(pixelIndex) > 0){
                        for (int z = 0 ; z < nz ; ++z){
                            inter[0] = m;
                            inter[1] = n;
                            inter[2] = z;
                            m_birthMap.push_back(inter);
                        }
                    }
                }
            }
        }
        // Ramdomize the birthMap (21)
        for (int i = 0 ; i < 96 ; ++i)
            random_shuffle( m_birthMap.begin(), m_birthMap.end() );
        cout << "size birth map = "<<  m_birthMap.size() << endl;
    }

    // save birth map
    Float3DImage::Pointer maskIm = blCreateImage::createNewFloat3DImage(nl_mask, nc_mask, nz);
    Float3DImage::IndexType pixelInd;
    for (unsigned int i = 0 ; i < m_birthMap.size() ; ++i){
        pixelInd[0] = m_birthMap[i][0];
        pixelInd[1] = m_birthMap[i][1];
        pixelInd[2] = m_birthMap[i][2];
        maskIm->SetPixel(pixelInd, 255);
    }
    //ImageIO::SaveFloat3DImage("F:\\experiments\\axons_1_2_3\\detection_ellipse_part\\res\\birthmask.tif", maskIm);
}

void blMppObjectManagerInterface::SetOverlapCoef(float overlapCoef){
    if (overlapCoef >= 0.0 && overlapCoef <=1){
        m_overlapCoef = overlapCoef;
    }
    else {
        throw blException("The overlaping coefficient must be between 0 and 1 !");
    }
}

void blMppObjectManagerInterface::SetObjectRepresentationType(string objectRepresentationType){
    m_ObjectRepresentationType = objectRepresentationType;
}

void blMppObjectManagerInterface::GetInsidePixelsValues(const vector<float> object, vector<float> &insideValues){
    // Inside values
    vector<vector<int> > coordIn;
    GetInsidePixelsCoordinates(object, coordIn);
    insideValues.resize(coordIn.size());
    Float2DImage::IndexType pixelIndex;
    for (unsigned int i = 0 ; i < coordIn.size() ; ++i){
        pixelIndex[0] = coordIn[i][0];
        pixelIndex[1] = coordIn[i][1];
        insideValues[i] = m_image->GetPixel(pixelIndex);
    }
}

void blMppObjectManagerInterface::GetBorderPixelsValues(const vector<float> object, vector<float> &outsideValues){
    // Border values
    vector<vector<int> > coordBorder;
    GetBorderPixelsCoordinates(object, coordBorder);
    outsideValues.resize(coordBorder.size());
    Float2DImage::IndexType pixelIndex;
    for (unsigned int i = 0 ; i < coordBorder.size() ; ++i){
        pixelIndex[0] = coordBorder[i][0];
        pixelIndex[1] = coordBorder[i][1];
        outsideValues[i] = m_image->GetPixel(pixelIndex);
    }
}

void blMppObjectManagerInterface::GetInsideAndBorderPixelsValues(const vector<float> object, vector<float> &insideValues, vector<float> &outsideValues){
    GetInsidePixelsValues(object, insideValues);
    GetBorderPixelsValues(object, outsideValues);
}

void blMppObjectManagerInterface::GetObjectRepresentation(vector<float> object, vector<vector<int> > &pixels){
    if (m_ObjectRepresentationType == "border")
        GetBorderPixelsCoordinates(object, pixels);
    else if (m_ObjectRepresentationType == "centerpoint"){
        vector<int> pixel; pixel.resize(2);
        pixel[0] = object[0]; pixel[1] = object[1]; pixels.push_back(pixel);
    }
    else if (m_ObjectRepresentationType == "centercross"){
        vector<int> pixel; pixel.resize(2);
        pixel[0] = object[0]; pixel[1] = object[1]; pixels.push_back(pixel);
        pixel[0] = object[0]+1; pixel[1] = object[1]; pixels.push_back(pixel);
        pixel[0] = object[0]-1; pixel[1] = object[1]; pixels.push_back(pixel);
        pixel[0] = object[0]; pixel[1] = object[1]+1; pixels.push_back(pixel);
        pixel[0] = object[0]; pixel[1] = object[1]-1; pixels.push_back(pixel);
    }
    else if (m_ObjectRepresentationType == "full"){
        GetInsidePixelsCoordinates(object, pixels);
    }
    else{
        this->notifyWarning("MppObjectManager -> The objectRepresentationType you gave is wrong, use the default one: border !");
        //cout << "MppObjectManager -> WARNING: The objectRepresentationType you gave is wrong, use the default one: border !" << endl;
        GetBorderPixelsCoordinates(object, pixels);
    }
}

bool blMppObjectManagerInterface::useBirthMap(){
    return m_useMask;
}

bool blMppObjectManagerInterface::use2DFor3DBirthMap(){
    return m_use2DMaskIn3D;
}

Float2DImage::Pointer blMppObjectManagerInterface::Get2DMaskPointer(){
    return m_mask;
}

Float3DImage::Pointer blMppObjectManagerInterface::Get3DMaskPointer(){
    return m_3DMask;
}

int blMppObjectManagerInterface::ObjectMaxLengthZ(){
    return 0;
}
