#include "blMppDataTermPartBhatt.h"
#include <blMath>

#include "blMppShapeEllipse.h"

using namespace std;

blMppDataTermPartBhatt::blMppDataTermPartBhatt() : blMppDataTerm(){
    m_D = 100;
    m_is2DImage = true;
    m_implementationId = 0;
}

blMppDataTermPartBhatt::blMppDataTermPartBhatt(blImage* image)
    : blMppDataTerm(image){
    m_D = 100;
    m_is2DImage = true;
    m_implementationId = 0;
}

blMppDataTermPartBhatt::~blMppDataTermPartBhatt(){

}

// blProcess methods
std::string blMppDataTermPartBhatt::name(){
    return "blMppDataTermPartBhatt";
}

// setters parameters
void blMppDataTermPartBhatt::setD(float D){
    m_D = D;
}

// dataTerm methods
void blMppDataTermPartBhatt::computeStats2D(blMppShape *shape, std::vector<float> &valuesIn){

    valuesIn.clear();
    // get statistics
    Float2DImage::Pointer image = m_image->itkFloat2DImagePointer();
    Float2DImage::IndexType pixelIndex;

    // inside mean and var
    std::vector<blPixel> *insidePixels = shape->insidePixels();
    std::vector<blPixel>::iterator itPixel;
    valuesIn.resize(insidePixels->size());
    int it = -1;
    for (itPixel = insidePixels->begin(); (itPixel != insidePixels->end()) ; ++itPixel) {
        pixelIndex[0] = (*itPixel).x();
        pixelIndex[1] = (*itPixel).y();
        it++;
        valuesIn[it] = image->GetPixel(pixelIndex);
    }
}

void blMppDataTermPartBhatt::computeStats3D(blMppShape *shape, std::vector<float>& valuesIn){

    valuesIn.clear();
    // get statistics
    Float3DImage::Pointer image = m_image->itkFloat3DImagePointer();
    Float3DImage::IndexType pixelIndex;

    // inside mean and var
    std::vector<blPixel> *insidePixels = shape->insidePixels();
    std::vector<blPixel>::iterator itPixel;
    int it = -1;
    valuesIn.resize(insidePixels->size());
    for (itPixel = insidePixels->begin(); (itPixel != insidePixels->end()) ; ++itPixel) {
        pixelIndex[0] = (*itPixel).x();
        pixelIndex[1] = (*itPixel).y();
        pixelIndex[2] = (*itPixel).z();
        it++;
        valuesIn[it] = image->GetPixel(pixelIndex);
    }
}

void blMppDataTermPartBhatt::initialize(){
    if (m_image->imageType() == blImage::TypeFloat2D){
        m_is2DImage = true;
    }
    else if (m_image->imageType() == blImage::TypeFloat3D){
        m_is2DImage = false;
    }
    else{
        throw blException("blMppDataTermPartBhatt can process only images of type Float2DImage");
    }

    if (m_generatorName == "blMppGeneratorShapeEllipse"){
        m_implementationId = 0;
    }
    else if (m_generatorName == "blMppGeneratorShapeEllipseIn3D"){
        m_implementationId = 1;
    }
    else{
        throw blException("blMppDataTermPartBhatt is compatible only with blMppGeneratorShapeEllipse and blMppGeneratorShapeEllipseIn3D");
    }
}

float blMppDataTermPartBhatt::compute(blMppShape* shape){

    if (m_implementationId == 0){
        return computeEllipse(shape);
    }
    else if (m_implementationId == 1){
        return computeEllipseIn3D(shape);
    }
    return 0;
}

float blMppDataTermPartBhatt::computeEllipse(blMppShape* shape){

    blMppShapeEllipse* shapee = dynamic_cast<blMppShapeEllipse*>(shape);

    // 2- Partition the border into 4 regions
    vector<float> vhead1,vhead2, vside1, vside2;
    partitionEllipse(shapee, vhead1,vhead2, vside1, vside2 );

    // 3- Calculate the Bhattacharyya distance for each region
    vector<float> valuesIn;
    computeStats2D(shape, valuesIn);

    float dB_head1 = blDistance::bhattacharyya(valuesIn, vhead1);
    float dB_head2 = blDistance::bhattacharyya(valuesIn, vhead2);
    float dB_side1 = blDistance::bhattacharyya(valuesIn, vside1);
    float dB_side2 = blDistance::bhattacharyya(valuesIn, vside2);

    float meanIn = blMathVector::mean(valuesIn);
    float mD_head1 = meanIn - blMathVector::mean(vhead1);
    float mD_head2 = meanIn - blMathVector::mean(vhead2);
    float mD_side1 = meanIn - blMathVector::mean(vside1);
    float mD_side2 = meanIn - blMathVector::mean(vside2);
    if (mD_head1 < 0){dB_head1=0;}
    if (mD_head2 < 0){dB_head2=0;}
    if (mD_side1 < 0){dB_side1=0;}
    if (mD_side2 < 0){dB_side2=0;}

    // 4- Makes the decision
    float dB = dB_side1;
    if (dB_side2<dB)
        dB = dB_side2;
    if (dB_head1<dB)
        dB = dB_head1;
    if (dB_head2<dB)
        dB = dB_head2;

    /*
    float dB_head = max(dB_head1, dB_head2);
    // take the min
    float dB = dB_side1;
    if (dB_side2<dB)
        dB = dB_side2;
    if (dB_head<dB)
        dB=dB_head;
     */


    // 5- Threshold the energy
    if (blMath::isNan(dB)){dB = 0;}
    if (dB < m_threshold){
        return (1- dB/m_threshold);
    }
    else{
        return exp(-(dB - m_threshold)/m_D)-1;
    }
}

void blMppDataTermPartBhatt::partitionEllipse( blMppShapeEllipse* shape, vector<float> &vhead1, vector<float> &vhead2, vector<float> &vside1, vector<float> &vside2){

    Float2DImage::Pointer image = m_image->itkFloat2DImagePointer();
    Float2DImage::IndexType pixelIndex;

    float cosi = cos(shape->theta());
    float sinu = sin(shape->theta());
    float a = shape->a();
    float b = shape->b();
    float xc = shape->getCenter().x();
    float yc = shape->getCenter().y();

    vector<float> p_large1; p_large1.resize(2);
    p_large1[0] = -cosi*a + xc;
    p_large1[1] = sinu*a + yc;
    vector<float> p_large2; p_large2.resize(2);
    p_large2[0] = cosi*a + xc;
    p_large2[1] = -sinu*a + yc;
    vector<float> p_small1; p_small1.resize(2);
    p_small1[0] =  sinu*b + xc;
    p_small1[1] =  cosi*b + yc;
    vector<float> p_small2; p_small2.resize(2);
    p_small2[0] =  -sinu*b + xc;
    p_small2[1] =  -cosi*b + yc;

    vector<float> d; d.resize(4);
    int pos;
    vhead1.clear(); vhead2.clear(); vside1.clear(); vside2.clear();
    vector<blPixel> *borderPixels = shape->borderPixels();
    for (unsigned int i=0 ; i < borderPixels->size() ; ++i){

        int x = (*borderPixels)[i].x();
        int y = (*borderPixels)[i].y();
        d[0] = blDistance::euclidian_square(p_large1[0], p_large1[1], x, y);
        d[1] = blDistance::euclidian_square(p_large2[0], p_large2[1], x, y);
        d[2] = blDistance::euclidian_square(p_small1[0], p_small1[1], x, y);
        d[3] = blDistance::euclidian_square(p_small2[0], p_small2[1], x, y);

        pos =  blMathVector::minPos(d);

        pixelIndex[0] = x;
        pixelIndex[1] = y;
        float val = image->GetPixel(pixelIndex);

        if (pos == 0){
            vhead1.push_back( val );
        }
        else if (pos==1){
            vhead2.push_back( val );
        }
        else if (pos==2){
            vside1.push_back( val );
        }
        else{
            vside2.push_back( val );
        }
    }
}

float blMppDataTermPartBhatt::computeEllipseIn3D(blMppShape* shape){
    // 1- Get the border pixels coordinates and value
     blMppShapeEllipse* shapee = dynamic_cast<blMppShapeEllipse*>(shape);

     // 2- Partition the border into 4 regions
     vector<float> vhead1,vhead2, vside1, vside2;
     partitionEllipseIn3D(shapee, vhead1,vhead2, vside1, vside2 );

     // 3- Calculate the Bhattacharyya distance for each region
     vector<float> valuesIn;
    computeStats3D(shape, valuesIn);

     float dB_head1 = blDistance::bhattacharyya(valuesIn, vhead1);
     float dB_head2 = blDistance::bhattacharyya(valuesIn, vhead2);
     float dB_side1 = blDistance::bhattacharyya(valuesIn, vside1);
     float dB_side2 = blDistance::bhattacharyya(valuesIn, vside2);

     float meanIn = blMathVector::mean(valuesIn);
     float mD_head1 = meanIn - blMathVector::mean(vhead1);
     float mD_head2 = meanIn - blMathVector::mean(vhead2);
     float mD_side1 = meanIn - blMathVector::mean(vside1);
     float mD_side2 = meanIn - blMathVector::mean(vside2);
     if (mD_head1 < 0){dB_head1=0;}
     if (mD_head2 < 0){dB_head2=0;}
     if (mD_side1 < 0){dB_side1=0;}
     if (mD_side2 < 0){dB_side2=0;}

     //float insideVar = mppMathVector::var(valuesIn);

     // 4- Makes the decision
     float dB_head = max(dB_head1, dB_head2);
     // take the min
     float dB = dB_side1;
     if (dB_side2<dB)
         dB = dB_side2;
     if (dB_head<dB)
         dB=dB_head;


     // 5- Threshold the energy
     if (blMath::isNan(dB)){dB = 0;}
     if (dB < m_threshold){
         return (1- dB/m_threshold);
     }
     else{
         return exp(-(dB - m_threshold)/m_D)-1;
     }
}

void blMppDataTermPartBhatt::partitionEllipseIn3D( blMppShapeEllipse* shape, vector<float> &vhead1, vector<float> &vhead2, vector<float> &vside1, vector<float> &vside2){

    Float3DImage::Pointer image = m_image->itkFloat3DImagePointer();
    Float3DImage::IndexType pixelIndex;

    float cosi = cos(shape->theta());
    float sinu = sin(shape->theta());
    float a = shape->a();
    float b = shape->b();
    float xc = shape->getCenter().x();
    float yc = shape->getCenter().y();
    float z = shape->getCenter().z();

    vector<float> p_large1; p_large1.resize(2);
    p_large1[0] = -cosi*a + xc;
    p_large1[1] = sinu*a + yc;
    vector<float> p_large2; p_large2.resize(2);
    p_large2[0] = cosi*a + xc;
    p_large2[1] = -sinu*a + yc;
    vector<float> p_small1; p_small1.resize(2);
    p_small1[0] =  sinu*b + xc;
    p_small1[1] =  cosi*b + yc;
    vector<float> p_small2; p_small2.resize(2);
    p_small2[0] =  -sinu*b + xc;
    p_small2[1] =  -cosi*b + yc;

    vector<float> d; d.resize(4);
    int pos;
    vhead1.clear(); vhead2.clear(); vside1.clear(); vside2.clear();
    int x, y;
    vector<blPixel> *borderPixels = shape->borderPixels();
    for (unsigned int i=0 ; i < borderPixels->size() ; ++i){
        x = (*borderPixels)[i].x();
        y = (*borderPixels)[i].y();
        d[0] = blDistance::euclidian_square(p_large1[0], p_large1[1], x, y);
        d[1] = blDistance::euclidian_square(p_large2[0], p_large2[1], x, y);
        d[2] = blDistance::euclidian_square(p_small1[0], p_small1[1], x, y);
        d[3] = blDistance::euclidian_square(p_small2[0], p_small2[1], x, y);

        pos =  blMathVector::minPos(d);

        pixelIndex[0] = x;
        pixelIndex[1] = y;
        pixelIndex[2] = z;
        float val = image->GetPixel(pixelIndex);

        if (pos == 0){
            vhead1.push_back( val );
        }
        else if (pos==1){
            vhead2.push_back( val );
        }
        else if (pos==2){
            vside1.push_back( val );
        }
        else{
            vside2.push_back( val );
        }
    }
}

void blMppDataTermPartBhatt::createCompatibleShapesList(){
    m_compatibleShapesList.push_back("blMppGeneratorShapeEllipse");
    m_compatibleShapesList.push_back("blMppGeneratorShapeEllipseIn3D");
}

bool blMppDataTermPartBhatt::useInsidePixels(){
    return true;
}

bool blMppDataTermPartBhatt::useBorderPixels(){
    return true;
}

bool blMppDataTermPartBhatt::useNormals(){
    return false;
}
