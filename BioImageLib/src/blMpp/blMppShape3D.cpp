/// \file blMppShape3D.cpp
/// \brief blMppShape3D class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppShape3D.h"
#include <iostream>

using namespace std;

blMppShape3D::blMppShape3D(){
    m_silhouette = NULL;
}

blMppShape3D::~blMppShape3D(){
    delete m_silhouette;
}

std::string blMppShape3D::name(){
    return "blMppShape3D";
}


void blMppShape3D::setSilhouette(blMppContainerSilhouette3D* silhouette){
    m_silhouette = silhouette;
}

std::vector<blPixel> *blMppShape3D::representation(std::string type){

    std::vector<blPixel> *pixels;
    if (type == "border")
        pixels = borderPixels();
    else if (type == "centerpoint"){
        pixels = new std::vector<blPixel>;
        pixels->push_back(m_center);
    }
    else if (type == "centercross"){
        pixels = new std::vector<blPixel>;
        pixels->push_back(m_center);

        blPixel pix;
        pix.setX(m_center.x()+1);
        pix.setY(m_center.y());
        pixels->push_back(pix);

        blPixel pix2;
        pix2.setX(m_center.x()-1);
        pix2.setY(m_center.y());
        pixels->push_back(pix2);

        blPixel pix3;
        pix3.setX(m_center.x());
        pix3.setY(m_center.y()+1);
        pixels->push_back(pix3);

        blPixel pix4;
        pix4.setX(m_center.x());
        pix4.setY(m_center.y()-1);
        pixels->push_back(pix4);
    }
    else if (type == "full"){
        pixels = this->insidePixels();
    }
    else if (type == "silhouette"){
        pixels = new std::vector<blPixel>;
        vector<vector<blMppContainerSilhouette3DLine> >* silhouette =  this->silhouette()->data();
        for (int i = 0 ; i < silhouette->size() ; ++i){
            for(int j = 0; j < (*silhouette)[i].size(); ++j){
                blPixel left = (*silhouette)[i][j].getPixelLeftAt(0);
                blPixel right = (*silhouette)[i][j].getPixelRightAt(0);
                pixels->push_back(blPixel(left.x(), left.y()));
                pixels->push_back(blPixel(right.x(), right.y()));
            }
        }
    }
    else{
        pixels = this->borderPixels();
    }
    return pixels;
}

blMppContainerSilhouette3D* blMppShape3D::silhouette(){
    return m_silhouette;
}

void blMppShape3D::copyData(blMppShape3D* copied){

    copied->setCenter(m_center);
    copied->setSilhouette(m_silhouette->copy());
    copied->setDataTerm(m_dataTerm);

    if (m_insidePixelsCalculated){
        std::vector<blPixel>* insidePixels = new std::vector<blPixel>();
        insidePixels->resize(m_insidePixels->size());
        for (int i = 0 ; i < m_insidePixels->size() ; ++i){
            insidePixels->operator [](i) = m_insidePixels->operator [](i);
        }
        copied->setInsidePixels(insidePixels);
    }
    else{
        copied->setInsidePixels(m_silhouette->insidePixels());
    }
    if (m_borderPixelsCalculated){
        std::vector<blPixel>* borderPixels = new std::vector<blPixel>();
        borderPixels->resize(m_borderPixels->size());
        for (int i = 0 ; i < m_borderPixels->size() ; ++i){
            //if ( (*m_borderPixels)[i].z() < 0 ){
            //    std::cout << "found negative coordinate " << std::endl;
            //}
            borderPixels->operator [](i) = m_borderPixels->operator [](i);
        }
        copied->setBorderPixels(borderPixels);
    }
}

void blMppShape3D::computeInsidePixels(){
    m_insidePixelsCalculated = true;
    m_insidePixels = m_silhouette->insidePixels();
}

void blMppShape3D::computeBorderPixels(int borderWidth){
    if (borderWidth > 1){
        throw blException("the default shape border implementation can handle only borderwidth of 1");
    }
    //std::cout << "generateInsideShapeImage" << std::endl;
    m_borderPixels = new vector<blPixel>();
    int minX, minY, minZ,sizeX, sizeY, sizeZ;
    vector<vector<int> > pixelsImage = generateInsideShapeImage(minX, minY, minZ, sizeX, sizeY, sizeZ);

    for (int k=1 ; k < sizeZ-1 ; k++){
        for (int i=1 ; i < sizeX-1 ; i++){
            for (int j=1 ; j < sizeY-1 ; j++){
                if (pixelsImage[k*sizeX+i][j] == 0){
                    if ( (pixelsImage[k*sizeX +i-1 ][j] > 0 || pixelsImage[k * sizeX + i+1][j] > 0 || pixelsImage[k*sizeX+i][j-1] > 0
                          || pixelsImage[k * sizeX + i][j+1] > 0 || pixelsImage[(k-1)*sizeX+i][j] > 0 || pixelsImage[(k+1)*sizeX+i][j] > 0)
                         ){
                        m_borderPixels->push_back(blPixel(i + (minX - 3),j + (minY - 3), k + (minZ - 3))) ;
                    }
                }
            }
        }
    }
    m_borderPixelsCalculated = true;
}

void blMppShape3D::computeNormalsBorder(){

    throw blException("blMppShape3D does not have any default implementation of normal computation");
}

void blMppShape3D::findClosestPixels(int pos, vector<blPixel> &neighboors){

    blPixel ref = m_borderPixels->operator [](pos);
    for (int x = ref.x()-1 ; ref.x() +1 ; ++x){
        for (int y = ref.y()-1 ; y < ref.y() +1 ; ++y)
        {
            if ( x != ref.x() && y != ref.y()){
                if (isPointInBorder(x, y)){
                    neighboors.push_back(blPixel(x,y));
                }
            }
        }
    }
}

bool blMppShape3D::isPointInBorder(int x, int y){
    for (int i = 0 ; i < m_borderPixels->size() ; ++i){
        blPixel curent = m_borderPixels->operator [](i);
        if (curent.x() == x && curent.y() == y){
            return true;
        }
    }
    return false;
}

vector<vector<int> > blMppShape3D::generateInsideShapeImage(int &minX, int &minY, int &minZ, int &sizeX, int &sizeY, int &sizeZ){

    if (!m_insidePixelsCalculated){
        this->computeInsidePixels();
    }

    // get the silhouette area
    int maxX = -999999;
    minX = 999999;
    int maxY = -999999;
    minY = 999999;
    int maxZ = -999999;
    minZ = 999999;
    int x, y,z;
    for (int i = 0 ; i < m_insidePixels->size() ; ++i ){
        x = (*m_insidePixels)[i].x();
        y =(*m_insidePixels)[i].y();
        z = (*m_insidePixels)[i].z();
        if (x > maxX)
            maxX = x;
        if (x < minX)
            minX = x;
        if (y > maxY)
            maxY = y;
        if (y < minY)
            minY = y;
        if(z < minZ)
            minZ = z;
        if(z > maxZ)
            maxZ = z;
    }

    if (!m_centerSetted){
        m_center = blPixel((minX+maxX)/2, (minY+maxY)/2, (minZ+maxZ)/2);
    }

    sizeX = maxX - minX + 6; // +6 for border width
    sizeY = maxY - minY + 6; // +6 for border width
    sizeZ = maxZ - minZ + 6; // +6 for border width
    m_maxDistanceToBarycenter = sqrt( (maxX - minX)*(maxX - minX) + (maxY - minY)*(maxY - minY) + (maxZ - minZ)*(maxZ - minZ))/2;

    vector<vector<int> > silhouette; silhouette.resize(sizeZ*sizeX);

    for (int i=0 ; i < sizeX*sizeZ ; ++i){
        silhouette[i].resize(sizeY);
        for (int j=0 ; j <sizeY ; ++j){
            silhouette[i][j]=0;;
        }
    }

    for (int i = 0 ; i < m_insidePixels->size() ; ++i ){
        x = (*m_insidePixels)[i].x();
        y = (*m_insidePixels)[i].y();
        z = (*m_insidePixels)[i].z();
        silhouette[(z-(minZ)+3)*sizeX +x-(minX)+3 ][y-(minY)+3] = 1;
    }
    return silhouette;
}
