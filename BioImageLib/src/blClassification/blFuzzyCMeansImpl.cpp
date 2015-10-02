/// \file blFuzzyCMeansImpl.cpp
/// \brief blFuzzyCMeansImpl class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2013

#include "blFuzzyCMeansImpl.h"
#include <stdlib.h>
#include <blCore>
#include <blMath>
#include "itkImageRegionIterator.h"
#include "blRand/blRandomGeneratorPM.h"

using namespace std;

// //////////////////////////////////////////////////////
//          blFuzzyCMeansImpl
// //////////////////////////////////////////////////////
blFuzzyCMeansImpl::blFuzzyCMeansImpl(){
    m_expo = 2.0;
    m_min_impro =  0.001;
    m_nbClass = 3;
    m_max_iter = 100 ;
    m_showIter = false;
    m_kernelParam = 150;
    m_useKernel = false;
}

blFuzzyCMeansImpl::~blFuzzyCMeansImpl(){

}

void blFuzzyCMeansImpl::setUseKernel(bool useKernel){
    m_useKernel = useKernel;
}

void blFuzzyCMeansImpl::setKernelParam(float kernelParam){
    m_kernelParam = kernelParam;
}

void blFuzzyCMeansImpl::setIterVerbose(bool verbose){
    m_showIter = verbose;
}

void blFuzzyCMeansImpl::setNbClass(int nbClass){
    m_nbClass = nbClass;
}

void blFuzzyCMeansImpl::setExpo(double expo){
    m_expo = expo;
}

void blFuzzyCMeansImpl::setMaxIter(int max_iter){
    m_max_iter = max_iter;
}

void blFuzzyCMeansImpl::setStopContPrecision(double min_impro){
    m_min_impro = min_impro;
}

void blFuzzyCMeansImpl::setInput(Float2DImage::Pointer image){
    m_image = image;
}
void blFuzzyCMeansImpl::setInput(FloatColor2DImage::Pointer image){
    m_RGBVimage = image;
}

// outputs
std::vector<std::vector<float> > blFuzzyCMeansImpl::getClassesMeanRGB(){
    return m_meanRGB;
}

std::vector<std::vector<float> > blFuzzyCMeansImpl::getClassesStdRGB(){
    return m_varRGB;
}

std::vector<float>  blFuzzyCMeansImpl::getClassesMean2D(){
    return m_mean;
}

std::vector<float> blFuzzyCMeansImpl::getClassesStd2D(){
    return m_var;
}

void blFuzzyCMeansImpl::computeCentroidRGBClassif(){

    //cout << "FuzzyCMeans::computeMeanStdClassif()" << endl;
    typedef itk::ImageRegionIterator<Float2DImage> IteratorType;
    vector<IteratorType> iterators;
    for (int i = 0 ; i <  m_nbClass; ++i){
        IteratorType iterator(m_U.at(i), m_U.at(i)->GetLargestPossibleRegion());
        iterators.push_back(iterator);
    }

    typedef itk::ImageRegionIterator<FloatColor2DImage> IteratorRGBType;
    IteratorRGBType iteratorInitImage(m_RGBVimage, m_RGBVimage->GetLargestPossibleRegion());

    float max, value;
    PixelColorFloatType valueRGB;
    int pos;
    m_meanRGB.clear(); m_varRGB.clear();
    m_meanRGB.resize(m_nbClass); m_varRGB.resize(m_nbClass);
    vector<int> pixCount; pixCount.resize(m_nbClass);
    while(!iterators.at(0).IsAtEnd()){

        max = 0.0; pos = 0;
        for (int i = 0 ; i <  m_nbClass; ++i){
            value = iterators.at(i).Get();
            if ( value > max ){
                max = value;
                pos = i;
            }
            ++iterators.at(i);
        }

        valueRGB = iteratorInitImage.Get();
        m_meanRGB[pos][0] += valueRGB[0];
        m_meanRGB[pos][1] += valueRGB[1];
        m_meanRGB[pos][2] += valueRGB[2];

        m_varRGB[pos][0] += pow(valueRGB[0],2);
        m_varRGB[pos][1] += pow(valueRGB[1],2);
        m_varRGB[pos][2] += pow(valueRGB[2],2);
        pixCount.at(pos)++;
        ++iteratorInitImage;
    }

    for (int i = 0 ; i < m_nbClass ; ++i){
        for (int c = 0 ; c < 3 ; ++c ){
            m_varRGB[i][c]=sqrt((m_varRGB[i][c]-m_meanRGB[i][c]*m_meanRGB[i][c]/pixCount.at(i))/(pixCount.at(i)-1));
            m_meanRGB[i][c] /= pixCount.at(i);
        }
    }

    //cout << "m_mean size = " << m_mean.size() << endl;
    //cout <<"FuzzyCMeans::computeMeanStdClassif() - > end"  << endl;
}

void blFuzzyCMeansImpl::computeMeanStdClassif(){

    //cout << "FuzzyCMeans::computeMeanStdClassif()" << endl;
    typedef itk::ImageRegionIterator<Float2DImage> IteratorType;
    vector<IteratorType> iterators;
    for (int i = 0 ; i <  m_nbClass; ++i){
        IteratorType iterator(m_U.at(i), m_U.at(i)->GetLargestPossibleRegion());
        iterators.push_back(iterator);
    }

    IteratorType iteratorInitImage(m_image, m_image->GetLargestPossibleRegion());

    float max, value;
    int pos;
    m_mean.clear(); m_var.clear();
    m_mean.resize(m_nbClass); m_var.resize(m_nbClass);
    vector<int> pixCount; pixCount.resize(m_nbClass);
    while(!iterators.at(0).IsAtEnd()){

        max = 0.0; pos = 0;
        for (int i = 0 ; i <  m_nbClass; ++i){
            value = iterators.at(i).Get();
            if ( value > max ){
                max = value;
                pos = i;
            }
            ++iterators.at(i);
        }

        value = iteratorInitImage.Get();
        m_mean.at(pos) += value;
        m_var.at(pos) += pow(value,2);
        pixCount.at(pos)++;
        ++iteratorInitImage;
    }

    for (int i = 0 ; i < m_nbClass ; ++i){
        m_var.at(i)=sqrt((m_var.at(i)-m_mean.at(i)*m_mean.at(i)/pixCount.at(i))/(pixCount.at(i)-1));
        m_mean.at(i) /= pixCount.at(i);
    }

    //cout << "m_mean size = " << m_mean.size() << endl;
    //cout <<"FuzzyCMeans::computeMeanStdClassif() - > end"  << endl;
}


void blFuzzyCMeansImpl::update(){

    // Float 2D image
    if (m_image && !m_useKernel){
        m_nl = m_image->GetLargestPossibleRegion().GetSize()[0];
        m_nc = m_image->GetLargestPossibleRegion().GetSize()[1];
        initfcm();
        for ( int i = 0 ; i < m_max_iter ; ++i){
            stepfcm();
            if (m_showIter){cout << "- iter " << i  << ", objective = " << m_obj_fcn_new << endl;}
            if (i > 0){
                if (fabs(m_obj_fcn_new - m_obj_fcn_old) < m_min_impro || (fabs(m_obj_fcn_new - m_obj_fcn_old) < 500 && m_obj_fcn_new > m_obj_fcn_old)){
                    break;
                }
                else{
                    m_obj_fcn_old = m_obj_fcn_new;
                }
            }
        }
        computeMeanStdClassif();
    }
    // RGBV 2D image
    else if (m_RGBVimage && !m_useKernel){
        m_nl = m_RGBVimage->GetLargestPossibleRegion().GetSize()[0];
        m_nc = m_RGBVimage->GetLargestPossibleRegion().GetSize()[1];
        initfcm();
        for ( int i = 0 ; i < m_max_iter ; ++i){
            stepfcmRGBV();
            if (m_showIter){cout << "- iter " << i  << ", objective = " << m_obj_fcn_new << endl;}
            if (i > 0){
                if (fabs(m_obj_fcn_new - m_obj_fcn_old) < m_min_impro || (fabs(m_obj_fcn_new - m_obj_fcn_old) < 500 && m_obj_fcn_new > m_obj_fcn_old)){
                    break;
                }
                else{
                    m_obj_fcn_old = m_obj_fcn_new;
                }
            }
        }
        computeCentroidRGBClassif();
    }

    // if kernel
    else if (m_image && m_useKernel){
        m_nl = m_image->GetLargestPossibleRegion().GetSize()[0];
        m_nc = m_image->GetLargestPossibleRegion().GetSize()[1];
        initfcm();
        vector<float> centersOld; centersOld.resize(m_nbClass);
        for ( int i = 0 ; i < m_max_iter ; ++i){
            centersOld = stepkfcm(centersOld);
            if (m_showIter){cout << "- iter " << i << ", objective = " << m_obj_fcn_new << endl;}
            if (i > 0){
                if (fabs(m_obj_fcn_new - m_obj_fcn_old) < m_min_impro || (fabs(m_obj_fcn_new - m_obj_fcn_old) < 1 && m_obj_fcn_new > m_obj_fcn_old)){
                    break;
                }
                else{
                    m_obj_fcn_old = m_obj_fcn_new;
                }
            }
        }
        computeMeanStdClassif();
    }
    else if (m_RGBVimage && m_useKernel){
        m_nl = m_RGBVimage->GetLargestPossibleRegion().GetSize()[0];
        m_nc = m_RGBVimage->GetLargestPossibleRegion().GetSize()[1];
        initfcm();
        vector<float> centersOld; centersOld.resize(m_nbClass*3);
        for ( int i = 0 ; i < m_max_iter ; ++i){
            centersOld = stepkfcmRGBV(centersOld);
            if (m_showIter){cout << "- iter " << i  << ", objective = " << m_obj_fcn_new << endl;}
            if (i > 0){
                if (fabs(m_obj_fcn_new - m_obj_fcn_old) < m_min_impro || (fabs(m_obj_fcn_new - m_obj_fcn_old) < 1 && m_obj_fcn_new > m_obj_fcn_old)){
                    break;
                }
                else{
                    m_obj_fcn_old = m_obj_fcn_new;
                }
            }
        }
        computeCentroidRGBClassif();
    }

}

void blFuzzyCMeansImpl::saveOutput(){

}

Int2DImage::Pointer blFuzzyCMeansImpl::getOutput(){


    std::cout << "blFuzzyCMeansImpl::getOutput()" << std::endl;
    // init the output image
    Int2DImage::Pointer outputImage = Int2DImage::New();

    Int2DImage::RegionType region;
    Int2DImage::IndexType start;
    start[0] = 0;
    start[1] = 0;
    region.SetIndex(start);

    Int2DImage::SizeType size;
    size[0] = m_U.at(0)->GetLargestPossibleRegion().GetSize()[0];
    size[1] = m_U.at(0)->GetLargestPossibleRegion().GetSize()[1];
    region.SetSize(size);

    outputImage->SetRegions(region);
    outputImage->Allocate();

    // calculate the max of the probabilities
    typedef itk::ImageRegionIterator<Float2DImage> IteratorType;
    vector<IteratorType> iterators;
    for (int i = 0 ; i <  m_nbClass; ++i){
        IteratorType iterator(m_U.at(i), m_U.at(i)->GetLargestPossibleRegion());
        iterators.push_back(iterator);
    }

    typedef itk::ImageRegionIterator<Int2DImage> IteratorIntType;
    IteratorIntType iteratorOutput(outputImage, outputImage->GetLargestPossibleRegion());

    float max;
    int pos;
    float val;
    while(!iterators.at(0).IsAtEnd()){
        max = -999999;
        pos = 0;
        for (int j=0 ; j< m_nbClass ; ++j ){
            val = iterators.at(j).Get();
            if (val > max){
                max = val;
                pos = j;
            }
            ++iterators.at(j);
        }
        iteratorOutput.Set(pos);
        ++iteratorOutput;
    }

    std::cout << "blFuzzyCMeansImpl::getOutput() end" << std::endl;
    return outputImage;
}

Float2DImage::Pointer blFuzzyCMeansImpl::getOutput(int i){
    return m_U.at(i);
}

void blFuzzyCMeansImpl::initfcm(){

    // 1-Create classes images
    for (int i = 0 ; i < m_nbClass; ++i){
        m_U.push_back(blImageCreate::createNewFloat2DImage(m_nl, m_nc));
    }

    // 2- Fill images
    // 2.1- Init iterators
    typedef itk::ImageRegionIterator<Float2DImage> IteratorType;
    vector<IteratorType> iterators;
    for (int i = 0 ; i <  m_nbClass; ++i){
        IteratorType iterator(m_U.at(i), m_U.at(i)->GetLargestPossibleRegion());
        iterators.push_back(iterator);
    }

    // 2.2- Init each pixels
    float sum, value;
    while(!iterators.at(0).IsAtEnd()){

        // 2.2.1- set each pixel to rand
        sum = 0.0;
        for (int i = 0 ; i <  m_nbClass; ++i){
            value = blRandomGeneratorPM::rand() *100.0;///100.0;
            iterators.at(i).Set( value );
            sum += value;
        }
        // 2.2.2- Normalize each pixel value by the sum
        for (int i = 0 ; i <  m_nbClass; ++i){
            if (sum == 0){
                value = 1.0/m_nbClass;
            }else{
                value = iterators.at(i).Get()/sum;
            }
            iterators.at(i).Set( value );
            ++iterators.at(i);
        }
    }
}

void blFuzzyCMeansImpl::stepfcm(){

    // 1- Compute centers
    // 1.1- Init iterators
    typedef itk::ImageRegionIterator<Float2DImage> IteratorType;
    vector<IteratorType> iterators;
    for (int i = 0 ; i <  m_nbClass; ++i){
        IteratorType iterator(m_U.at(i), m_U.at(i)->GetLargestPossibleRegion());
        iterators.push_back(iterator);
    }

    IteratorType iteratorInitImage(m_image, m_image->GetLargestPossibleRegion());

    // 1.2- compute centers
    vector<float> num, den, centers;
    for (int j = 0 ; j < m_nbClass ; ++j){
        num.push_back(0.0);
        den.push_back(0.0);
    }
    //num.resize(m_nbClass); den.resize(m_nbClass);
    float uexpo;
    while(!iterators.at(0).IsAtEnd()){
        for (int j=0 ; j< m_nbClass ; ++j ){
            uexpo = pow(iterators.at(j).Get(), m_expo);
            if (!blMath::isNan(uexpo)) {
                num[j] += uexpo*iteratorInitImage.Get();
                den[j] += uexpo;
            }
            ++iterators.at(j);
        }
        ++iteratorInitImage;
    }
    for (int j = 0 ; j < m_nbClass ; ++j){
        centers.push_back(num[j]/den[j]);
    }

    // 2- compute U(k+1) and objective funciton
    float sum;
    vector<float> dist;
    for (int i=0;i<m_nbClass;++i){
        dist.push_back(0.0);
        iterators.at(i).GoToBegin();
    }
    iteratorInitImage.GoToBegin();
    m_obj_fcn_new = 0.0;
    while(!iterators.at(0).IsAtEnd()){

        sum = 0.0;
        for (int j=0;j<m_nbClass ; ++j){
            dist[j] = sqrt(pow(centers[j] -  iteratorInitImage.Get(),2));
            m_obj_fcn_new += dist[j]*dist[j]*pow(iterators.at(j).Get(),m_expo);
            dist[j] = pow(dist[j], -2/(m_expo-1));
            sum += dist[j];
        }
        for (int j=0;j<m_nbClass ; ++j){
            iterators.at(j).Set(dist[j]/sum);
            ++iterators.at(j);
        }
        ++iteratorInitImage;
    }
}

void blFuzzyCMeansImpl::stepfcmRGBV(){

    // 1- Compute centers
    // 1.1- Init iterators
    typedef itk::ImageRegionIterator<Float2DImage> IteratorType;
    vector<IteratorType> iterators;
    for (int i = 0 ; i <  m_nbClass; ++i){
        IteratorType iterator(m_U.at(i), m_U.at(i)->GetLargestPossibleRegion());
        iterators.push_back(iterator);
    }

    typedef itk::ImageRegionIterator<FloatColor2DImage> IteratorTypeRGBV;
    IteratorTypeRGBV iteratorInitImage(m_RGBVimage, m_RGBVimage->GetLargestPossibleRegion());

    // 1.2- compute centers
    vector<float> num, den, centers;
    num.resize(m_nbClass*3);
    den.resize(m_nbClass*3);
    centers.resize(m_nbClass*3);
    float uexpo;
    while(!iterators.at(0).IsAtEnd()){
        for (int j=0 ; j< m_nbClass ; ++j ){
            uexpo = pow(iterators.at(j).Get(), m_expo);
            if (!blMath::isNan(uexpo)) {
                for (int k = 0 ; k < 3 ; ++k) {
                    num[3*j+k] += uexpo*iteratorInitImage.Get()[k];
                    den[3*j+k] += uexpo;
                }
            }
            ++iterators.at(j);
        }
        ++iteratorInitImage;
    }
    for (int j = 0 ; j < m_nbClass*3 ; ++j){
        centers[j] = num[j]/den[j];
    }

    // 2- compute U(k+1) and objective funciton
    float sum;
    vector<float> dist;
    for (int i=0;i<m_nbClass;++i){
        dist.push_back(0.0);
        iterators.at(i).GoToBegin();
    }
    iteratorInitImage.GoToBegin();
    m_obj_fcn_new = 0.0;
    while(!iterators.at(0).IsAtEnd()){

        sum = 0.0;
        for (int j=0 ; j<m_nbClass ; ++j){
            dist[j] = 0.0;
            for (int k=0 ; k < 3 ; ++k){
                dist[j] = sqrt(pow(centers[3*j+k] -  iteratorInitImage.Get()[k],2));
            }
            m_obj_fcn_new += dist[j]*dist[j]*pow(iterators.at(j).Get(),m_expo);
            dist[j] = pow(dist[j], -2/(m_expo-1));
            sum += dist[j];
        }
        for (int j=0;j<m_nbClass ; ++j){
            iterators.at(j).Set(dist[j]/sum);
            ++iterators.at(j);
        }
        ++iteratorInitImage;
    }
}

float blFuzzyCMeansImpl::kernel(vector<float> center, int i, float imagePix){
    float euclidianDist = pow(center[i] - imagePix,2);
    return exp(-euclidianDist/pow(m_kernelParam,2));
}

vector<float> blFuzzyCMeansImpl::stepkfcm(vector<float> centersOld){

    // 1- Compute centers
    // 1.1- Init iterators
    typedef itk::ImageRegionIterator<Float2DImage> IteratorType;
    vector<IteratorType> iterators;
    for (int i = 0 ; i <  m_nbClass; ++i){
        IteratorType iterator(m_U.at(i), m_U.at(i)->GetLargestPossibleRegion());
        iterators.push_back(iterator);
    }

    IteratorType iteratorInitImage(m_image, m_image->GetLargestPossibleRegion());

    // 1.2- compute centers
    vector<float> num, den, centers;
    num.resize(m_nbClass*3);
    den.resize(m_nbClass*3);
    centers.resize(m_nbClass*3);
    float uexpo, kernelVal;
    while(!iterators.at(0).IsAtEnd()){
        for (int j=0 ; j< m_nbClass ; ++j ){
            uexpo = pow(iterators.at(j).Get(), m_expo);
            if (!blMath::isNan(uexpo)) {
                kernelVal = uexpo*kernel(centersOld, j, iteratorInitImage.Get());
                num[j] += kernelVal*iteratorInitImage.Get();
                den[j] += kernelVal;
            }
            ++iterators.at(j);
        }
        ++iteratorInitImage;
    }
    for (int j = 0 ; j < m_nbClass ; ++j){
        centers[j] = num[j]/den[j];
    }

    // 2- compute U(k+1) and objective funciton
    float sum;
    vector<float> dist;
    for (int i=0;i<m_nbClass;++i){
        dist.push_back(0.0);
        iterators.at(i).GoToBegin();
    }
    iteratorInitImage.GoToBegin();
    m_obj_fcn_new = 0.0;
    while(!iterators.at(0).IsAtEnd()){

        sum = 0.0;
        for (int j=0 ; j<m_nbClass ; ++j){
            dist[j] = 0.0;
            dist[j] = 2-2*kernel(centers, j, iteratorInitImage.Get());
            m_obj_fcn_new += dist[j]*dist[j]*pow(iterators.at(j).Get(),m_expo);
            dist[j] = pow(dist[j], -1/(m_expo-1));
            sum += dist[j];
        }
        for (int j=0;j<m_nbClass ; ++j){
            iterators.at(j).Set(dist[j]/sum);
            ++iterators.at(j);
        }
        ++iteratorInitImage;
    }
    return centers;
}

// ///////////////////////
float blFuzzyCMeansImpl::RGBVkernel(vector<float> center, int i, PixelColorFloatType imagePix){
    float euclidianDist = 0.0;
    for (int j = 0 ; j < 3 ; ++j){
        euclidianDist += pow(center[3*i+j] - imagePix[j],2);
    }
    return exp(-euclidianDist/pow(m_kernelParam,2));
}

vector<float> blFuzzyCMeansImpl::stepkfcmRGBV(vector<float> centersOld){

    // 1- Compute centers
    // 1.1- Init iterators
    typedef itk::ImageRegionIterator<Float2DImage> IteratorType;
    vector<IteratorType> iterators;
    for (int i = 0 ; i <  m_nbClass; ++i){
        IteratorType iterator(m_U.at(i), m_U.at(i)->GetLargestPossibleRegion());
        iterators.push_back(iterator);
    }

    typedef itk::ImageRegionIterator<FloatColor2DImage> IteratorTypeRGBV;
    IteratorTypeRGBV iteratorInitImage(m_RGBVimage, m_RGBVimage->GetLargestPossibleRegion());

    // 1.2- compute centers
    vector<float> num, den, centers;
    num.resize(m_nbClass*3);
    den.resize(m_nbClass*3);
    centers.resize(m_nbClass*3);
    float uexpo, kernelVal;
    while(!iterators.at(0).IsAtEnd()){
        for (int j=0 ; j< m_nbClass ; ++j ){
            uexpo = pow(iterators.at(j).Get(), m_expo);
            if (!blMath::isNan(uexpo)) {
                kernelVal = uexpo*RGBVkernel(centersOld, j, iteratorInitImage.Get());
                for (int k = 0 ; k < 3 ; ++k) {
                    num[3*j+k] += kernelVal*iteratorInitImage.Get()[k];
                    den[3*j+k] += kernelVal;
                }
            }
            ++iterators.at(j);
        }
        ++iteratorInitImage;
    }
    for (int j = 0 ; j < m_nbClass*3 ; ++j){
        centers[j] = num[j]/den[j];
    }

    // 2- compute U(k+1) and objective funciton
    float sum;
    vector<float> dist;
    for (int i=0;i<m_nbClass;++i){
        dist.push_back(0.0);
        iterators.at(i).GoToBegin();
    }
    iteratorInitImage.GoToBegin();
    m_obj_fcn_new = 0.0;
    while(!iterators.at(0).IsAtEnd()){

        sum = 0.0;
        for (int j=0 ; j<m_nbClass ; ++j){
            dist[j] = 0.0;
            dist[j] = 2-2*RGBVkernel(centers, j, iteratorInitImage.Get());
            m_obj_fcn_new += dist[j]*dist[j]*pow(iterators.at(j).Get(),m_expo);
            dist[j] = pow(dist[j], -1/(m_expo-1));
            sum += dist[j];
        }
        for (int j=0;j<m_nbClass ; ++j){
            iterators.at(j).Set(dist[j]/sum);
            ++iterators.at(j);
        }
        ++iteratorInitImage;
    }
    return centers;
}

