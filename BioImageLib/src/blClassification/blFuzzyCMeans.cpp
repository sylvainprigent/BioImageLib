/// \file blFuzzyCMeans.cpp
/// \brief blFuzzyCMeans class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2013

#include "blFuzzyCMeans.h"
#include "blFuzzyCMeansImpl.h"
#include <stdlib.h>
#include <blCore>
#include "itkImageRegionIterator.h"
#include <blCastStacks>

using namespace std;

// //////////////////////////////////////////////////////
//          blFuzzyCMeans
// //////////////////////////////////////////////////////
blFuzzyCMeans::blFuzzyCMeans() : blClassifier(){
    m_useKernel = false;
    m_kernelParam = 150;
    m_nbClass = 3;
    m_expo = 2.0;
    m_max_iter = 100 ;
    m_min_impro =  0.001;

}

blFuzzyCMeans::~blFuzzyCMeans(){

}

std::string blFuzzyCMeans::name(){
    return "blFuzzyCMeans";
}

// parameters
void blFuzzyCMeans::setUseKernel(bool useKernel){
    m_useKernel = useKernel;
}

void blFuzzyCMeans::setKernelParam(float kernelParam){
    m_kernelParam = kernelParam;
}

void blFuzzyCMeans::setNbClass(int nbClass){
    m_nbClass = nbClass;
}

void blFuzzyCMeans::setExpo(double expo){
    m_expo = expo;
}

void blFuzzyCMeans::setMaxIter(int max_iter){
    m_max_iter = max_iter;
}

void blFuzzyCMeans::setStopContPrecision(double min_impro){
    m_min_impro = min_impro;
}

void blFuzzyCMeans::run(){

    if (m_inputImage->imageType() == blImage::TypeInt2D
            || m_inputImage->imageType() == blImage::TypeIntColor2D){

        // cast image to float
        blCastImageToFloat cast;
        cast.setInput(m_inputImage);
        cast.run();
        blImage* castInput = cast.output();

        // apply convolutin
        blFuzzyCMeans process;
        process.setInput(castInput);
        process.setUseKernel(m_useKernel);
        process.setKernelParam(m_kernelParam);
        process.setNbClass(m_nbClass);
        process.setExpo(m_expo);
        process.setMaxIter(m_max_iter);
        process.setStopContPrecision(m_min_impro);
        process.run();
        m_outputImage = process.output();
        m_classesProbabilities = process.classesProbabilities();
        m_scalarClassesMean = process.classesMean2D();
        m_scalarClassesStd = process.classesStd2D();
        delete castInput;
    }
    else if (m_inputImage->imageType() == blImage::TypeFloat2D){

        std::cout << "blFuzzyCMeans run TypeFloat2D" << std::endl;
        blFuzzyCMeansImpl process;
        process.setUseKernel(m_useKernel);
        process.setKernelParam(m_kernelParam);
        process.setNbClass(m_nbClass);
        process.setExpo(m_expo);
        process.setMaxIter(m_max_iter);
        process.setStopContPrecision(m_min_impro);
        process.setInput(m_inputImage->itkFloat2DImagePointer());
        process.setIterVerbose(true);
        process.update();
        m_outputImage = new blImage(process.getOutput());

        std::cout << "blFuzzyCMeans run TypeFloat2D calculate probabilities maps" << std::endl;
        std::vector<blImage*> probMaps; probMaps.resize(m_nbClass);
        for (int i = 0 ; i < m_nbClass ; ++i){
            probMaps[i] = new blImage(process.getOutput(i));
        }
        m_classesProbabilities = new blImageList;
        m_classesProbabilities->setData(probMaps);

        std::cout << "blFuzzyCMeans run TypeFloat2D calculate means" << std::endl;

        m_scalarClassesMean = process.getClassesMean2D();
        std::cout << "blFuzzyCMeans:: m_scalarClassesMean = ";
        for (int i = 0 ; i < m_scalarClassesMean.size() ; ++i){
            std::cout << m_scalarClassesMean[i] << "\t";
        }
        std::cout << std::endl;
        std::cout << "blFuzzyCMeans run TypeFloat2D calculate std" << std::endl;
        m_scalarClassesStd = process.getClassesStd2D();
        std::cout << "blFuzzyCMeans:: m_scalarClassesStd = ";
        for (int i = 0 ; i < m_scalarClassesStd.size() ; ++i){
            std::cout << m_scalarClassesStd[i] << "\t";
        }
        std::cout << std::endl;

        std::cout << "blFuzzyCMeans run TypeFloat2D end" << std::endl;

    }
    else if (m_inputImage->imageType() == blImage::TypeFloatColor2D){

        // process
        blFuzzyCMeansImpl process;
        process.setUseKernel(m_useKernel);
        process.setKernelParam(m_kernelParam);
        process.setNbClass(m_nbClass);
        process.setExpo(m_expo);
        process.setMaxIter(m_max_iter);
        process.setStopContPrecision(m_min_impro);
        process.setInput(m_inputImage->itkFloatColor2DImagePointer());
        process.update();
        m_outputImage = new blImage(process.getOutput());

        std::vector<blImage*> probMaps; probMaps.resize(m_nbClass);
        for (int i = 0 ; i < m_nbClass ; ++i){
            probMaps[i] = new blImage(process.getOutput(i));
        }
        m_classesProbabilities->setData(probMaps);

        m_vectorClassesMean = process.getClassesMeanRGB();
        m_vectorClassesStd = process.getClassesStdRGB();

    }
    else if (m_inputImage->imageType() == blImage::TypeInt3D
             || m_inputImage->imageType() == blImage::TypeFloat3D
             || m_inputImage->imageType() == blImage::TypeIntColor3D
             || m_inputImage->imageType() == blImage::TypeFloatColor3D){

        throw blException("blFuzzyCMeans is not yet implemented for 3D images");
    }
    else{
        std::string message = "blFuzzyCMeans does not support the image type " + m_inputImage->imageType();
        throw blException(message.c_str());
    }
}

// outputs
blImageList* blFuzzyCMeans::classesProbabilities(){
    return m_classesProbabilities;
}


std::vector<std::vector<float> >& blFuzzyCMeans::classesMean(){
    return m_vectorClassesMean;
}
std::vector<std::vector<float> >& blFuzzyCMeans::classesStd(){
    return m_vectorClassesStd;
}

std::vector<float>& blFuzzyCMeans::classesMean2D(){
    return m_scalarClassesMean;
}

std::vector<float>& blFuzzyCMeans::classesStd2D(){
    return m_scalarClassesStd;
}


blTableNumber* blFuzzyCMeans::classesMeans(){

    blTableNumber* meansData = new blTableNumber;
    if (m_inputImage->imageType() == blImage::TypeFloat2D || m_inputImage->imageType() == blImage::TypeInt2D){
        std::vector<float> mean = m_scalarClassesMean;
        std::vector<std::vector<float> > meanMat; meanMat.resize(1);
        meanMat[0] = mean;
        meansData->setData(meanMat);
    }
    else if (m_inputImage->imageType() == blImage::TypeFloatColor2D || m_inputImage->imageType() == blImage::TypeIntColor2D) {
        std::vector<std::vector<float> > meanMat = this->classesMean();
        meansData->setData(meanMat);

    }
    else{
        throw blException("blFuzzyCMeans: Data type note implemented");
    }

    meansData->setTitle("Means");
    std::vector<std::string> header; header.resize(m_nbClass);
    for (int i = 0 ; i < m_nbClass ; ++i){
        header[i] = "class " + blCast::to_string(i);
    }
    meansData->setHeaders(header);

    return meansData;

}

blTableNumber* blFuzzyCMeans::classesVars(){
    blTableNumber* varsData = new blTableNumber;
    if (m_inputImage->imageType() == blImage::TypeFloat2D || m_inputImage->imageType() == blImage::TypeInt2D){
        std::cout << " in image type 2D scalar" << std::endl;
        std::vector<float> var = m_scalarClassesStd;
        std::cout << "blFuzzyCMeans:: var = ";
        for (int i = 0 ; i < m_scalarClassesStd.size() ; ++i){
            std::cout << m_scalarClassesStd[i] << "\t";
        }
        std::cout << std::endl;
        std::vector<std::vector<float> > varMat; varMat.resize(1);
        varMat[0] = var;
        varsData->setData(varMat);
    }
    else if (m_inputImage->imageType() == blImage::TypeFloatColor2D || m_inputImage->imageType() == blImage::TypeIntColor2D) {
        std::vector<std::vector<float> > varMat = this->classesStd();
        varsData->setData(varMat);
    }
    else{
        throw blException("blFuzzyCMeans: Data type note implemented");
    }

    varsData->setTitle("Vars");
    std::vector<std::string> header; header.resize(m_nbClass);
    for (int i = 0 ; i < m_nbClass ; ++i){
        header[i] = "class " + blCast::to_string(i);
    }
    varsData->setHeaders(header);

    return varsData;
}
