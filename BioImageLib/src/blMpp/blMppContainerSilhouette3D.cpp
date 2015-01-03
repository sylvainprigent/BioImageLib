/// \file blMppContainerSilhouette3D.cpp
/// \brief blMppContainerSilhouette3D class
/// \author Emmanuelle Poulain (Emmanuelle.Poulain@inria.fr)
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppContainerSilhouette3D.h"
#include <iostream>

using namespace std;

// //////////////////////////////////////////////// //
//             blMppContainerSilhouette3D
// /////////////////////////////////////////////// //
blMppContainerSilhouette3D::~blMppContainerSilhouette3D(){
    delete m_data;

}

blMppContainerSilhouette3D::blMppContainerSilhouette3D(){
    m_data = new std::vector<vector<blMppContainerSilhouette3DLine> >;
    m_nbLayout = 0;
    m_layoutIdx = -1;
}

void blMppContainerSilhouette3D::setData(std::vector<vector<blMppContainerSilhouette3DLine> > *data){
    delete m_data;
    m_data = data;
    m_nbLayout = data->size();
}

void blMppContainerSilhouette3D::addLine(int layOutIdx, int lineIdx, const blPixel & leftPixel, const blPixel & rightPixel){
    (*m_data)[layOutIdx].push_back(blMppContainerSilhouette3DLine(layOutIdx,lineIdx, leftPixel, rightPixel));//cutindex
}

void blMppContainerSilhouette3D::addLayout(){
    //vector<blMppContainerSilhouette3DLine>*  layout ;//= new vector<blMppContainerSilhouette3DLine> ;
    (*m_data).resize(m_nbLayout +1);
    m_nbLayout = m_nbLayout + 1;
}


void blMppContainerSilhouette3D::addLine(int layOutIdx, const blMppContainerSilhouette3DLine &line){//cutindex

    (*m_data)[layOutIdx].push_back(line);

}



int blMppContainerSilhouette3D::area(){
    return m_area;
}

void blMppContainerSilhouette3D::setArea(int area){
    m_area = area;
}

int blMppContainerSilhouette3D::nbLayout(){

    return m_nbLayout;
}

vector<blMppContainerSilhouette3DLine> blMppContainerSilhouette3D::operator[](int i){
    return (*m_data)[i];
}

bool blMppContainerSilhouette3D::isOverlaping(blMppContainerSilhouette3D *other, float allowedPurcentage){

    //std::cout << "this silhouette = " << std::endl;
    //this->printConsole();
    //std::cout << "other silhouette = " << std::endl;
    //other->printConsole();

    // allowedPurcentage = 0.0;
    //cout << "current\n" << endl;
    // cout<<m_data->size()<<endl;
    //printConsole();
    // cout << "\nother\n" << endl;
    //other->printConsole();
    if (allowedPurcentage == 0){
        // find the first common line x
        int m = -1; int n = -1;int l1 = -1; int l2 = -1;

        findFirstCommonLayout(other,l1,l2);

        if(l1==-1){
            return false;
        }
        while(1){
            // cout << "m"<<m<<endl;
            m= -1; n= -1;
            findFirstCommonLine(other,l1,l2, m, n);

            if (m != -1){

                //(*(*m_data)[l1])[m].pairNumber()
                //other->(*(*m_data)[l2])[n].pairNumber()
                //other->m_data->operator()[l2][n].pairNumber()
                // compare objects line by line
                //while(1){
                for ( m =0; m<(*m_data)[l1].size();++m){
                    // cout << "m"<<m<<endl;
                    for ( n=0;n<(*other)[l2].size();++n){
                        if((*m_data)[l1][m].lineIdx() == (*other)[l2][n].lineIdx()){
                            for (int i = 0 ; i <(*m_data)[l1][m].pairNumber()  ; ++i){

                                for (int j = 0 ;j < (*other)[l2][n].pairNumber()   ; ++j){
                                    float y1 =(*m_data)[l1][m].getPixelLeftAt(i).y();
                                    float y2 =(*m_data)[l1][m].getPixelRightAt(i).y();
                                    float yp1 = (*other)[l2][n].getPixelLeftAt(j).y();
                                    float yp2 = (*other)[l2][n].getPixelRightAt(j).y();


                                    if (y2 < yp1){
                                        continue;
                                    }
                                    if (yp2 < y1){
                                        continue;
                                    }
                                    if ( y2 >= yp1 && y2 <= yp2){
                                        return true;
                                    }
                                    if (yp2 >= y1 && yp2 <= y2){
                                        return true;
                                    }
                                }
                            }
                        }
                    }
                    // }

                }
            }
            //cout <<"l1"<<l1<<endl;
            l1++; if (l1 >= nbLayout()){return false;}
            l2++; if (l2 >= other->nbLayout()){return false;}
            // if ((*m_data)[l1][0].layoutIdx() != (*other)[l2][0].layoutIdx()){
            //	return false;
            // }
        }
    }
    else
    {
        if (purcentageOverlaping(other) > allowedPurcentage){
            return true;
        }
        return false;
    }

}

float blMppContainerSilhouette3D::purcentageOverlaping(blMppContainerSilhouette3D *other){
    // find the first common line x
    int m = -1; int n = -1;
    int l1 = -1; int l2 = -1;


    findFirstCommonLayout(other,l1,l2);
    //std::cout << "first common layout = " << l1 << ", " << l2 << std::endl;
    if(l1==-1){
        return 0;
    }
    int overlapArea = 0;
    float area = other->area();
    if (m_area <= area){
        area = m_area;
    }
    while(1){
        m=-1;
        n=-1;

        findFirstCommonLine(other, l1,l2,m, n);
        if (m != -1){

            for ( m =0; m<(*m_data)[l1].size();++m){
                for ( n=0;n<(*other)[l2].size();++n){
                    if((*m_data)[l1][m].lineIdx() == (*other)[l2][n].lineIdx()){
                        for (int i = 0 ; i <(*m_data)[l1][m].pairNumber()  ; ++i){
                            for (int j = 0 ;j < (*other)[l2][n].pairNumber()   ; ++j){
                                float y1 =(*m_data)[l1][m].getPixelLeftAt(i).y();
                                float y2 =(*m_data)[l1][m].getPixelRightAt(i).y();
                                float yp1 = (*other)[l2][n].getPixelLeftAt(j).y();
                                float yp2 = (*other)[l2][n].getPixelRightAt(j).y();

                                if (y2 < yp1){
                                    continue;
                                }
                                if (yp2 < y1){
                                    continue;
                                }
                                if ( y2 >= yp1 && y2 <= yp2){
                                    if (y1 <= yp1)
                                        overlapArea += y2 - yp1 + 1;
                                    else
                                        overlapArea += yp2 - yp1 + 1;
                                }
                                if (yp2 >= y1 && yp2 <= y2){
                                    if (yp1 <= y1)
                                        overlapArea += yp2 - y1 + 1;
                                    else
                                        overlapArea += y2 - y1 + 1;
                                }
                            }
                        }
                    }
                }
            }



        }

        l1++; if (l1 >= nbLayout()){return float(overlapArea)/area;}
        l2++; if (l2 >= other->nbLayout()){return float(overlapArea)/area;}

    }
}




bool blMppContainerSilhouette3D::isInside(blPixel){

    std::cout << "WARNING : blMppContainerSilhouette3D::isInside is not implemented" << std::endl;
    // // find common x
    // int x = pixel.x();
    // int x_min = m_data->operator [](0).lineIdx();
    // int x_max = m_data->operator [](m_data->size()-1).lineIdx();

    // if ( x >= x_min && x <= x_max){
    //     int pos = x - x_min;

    //     // test all the couples
    //     int y = pixel.y();
    //     for (int j = 0 ;j < m_data->operator [](pos).pairNumber() ; ++j){
    //         float y1 = m_data->operator [](pos).getPixelLeftAt(j).y();
    //         float y2 = m_data->operator [](pos).getPixelRightAt(j).y();

    //         if ( y1 <= y && y <= y2 ){
    //             return true;
    //         }
    //     }
    //     return false;
    // }
    // else{
    return false;
    // }

}

void blMppContainerSilhouette3D::findFirstCommonLine(blMppContainerSilhouette3D* other,int layoutIdx1, int layoutIdx2, int &m, int &n){

    int x_min = (*m_data)[layoutIdx1][0].lineIdx();
    int x_max = (*m_data)[layoutIdx1][(*m_data)[layoutIdx1].size()-1].lineIdx();

    int xp_min = (*other)[layoutIdx2][0].lineIdx();
    int xp_max = (*other)[layoutIdx2][(*other)[layoutIdx2].size()-1].lineIdx();


    if (x_min >= xp_min && x_min <=xp_max){
        m = 0;
        n = x_min - xp_min;
    }
    else if (xp_min >= x_min && xp_min <=x_max){
        m = xp_min - x_min;
        n = 0;
    }
}

void blMppContainerSilhouette3D::findFirstCommonLayout(blMppContainerSilhouette3D* other,int &layoutIdx1, int &layoutIdx2){
    //printConsole();
    int x_min = (*m_data)[0][0].layoutIdx();
    int x_max = (*m_data)[nbLayout()-1][0].layoutIdx();

    int xp_min = (*other)[0][0].layoutIdx();
    int xp_max = (*other)[other->nbLayout()-1][0].layoutIdx();

    if (x_min >= xp_min && x_min <=xp_max){
        layoutIdx1 = 0;
        layoutIdx2 = x_min - xp_min;
    }
    else if (xp_min >= x_min && xp_min <=x_max){
        layoutIdx1 = xp_min - x_min;
        layoutIdx2 = 0;
    }
}

void blMppContainerSilhouette3D::calculateArea(){
    m_area = 0;
    //std::cout<<"m_datasize"<<m_data->size() <<std::endl;
    for(int l = 0; l < nbLayout(); ++l){
        for (int i = 0 ; i <(*m_data)[l].size() ; ++i){
            for (int j = 0 ; j < (*m_data)[l][i].pairNumber() ; ++j){
                int y1 = (*m_data)[l][i].getPixelLeftAt(j).y();
                int y2 = (*m_data)[l][i].getPixelRightAt(j).y();
                m_area += abs(y2-y1)+1;
            }
        }
    }
}

std::vector<blPixel> *blMppContainerSilhouette3D::insidePixels(){

    std::vector<blPixel> *insidePixels = new std::vector<blPixel>;

    insidePixels->resize(m_area);
    //printConsole();
    int iter = -1;
    for(int l = 0; l < m_data->size() ; ++l){
        for (int i = 0 ; i < (*m_data)[l].size() ; ++i){
            for (int j = 0 ; j < (*m_data)[l][i].pairNumber() ; ++j){
                int x =  (*m_data)[l][i].lineIdx();
                int y1 = (*m_data)[l][i].getPixelLeftAt(j).y();
                int y2 = (*m_data)[l][i].getPixelRightAt(j).y();
                int z =  (*m_data)[l][i].getPixelRightAt(j).z();

                if (y1 == y2){
                    iter++;
                    insidePixels->operator [](iter) = blPixel(x,y1,z);
                }
                else{
                    for (int y = int(y1) ; y <= int(y2) ; ++y){
                        iter++;
                        insidePixels->operator [](iter) = blPixel(x,y,z);
                    }
                }
            }
        }
    }

    /*
    std::cout << "inside pixels = " << std::endl;
    for (int i = 0 ; i < insidePixels->size() ; ++i){
        std::cout << "( "<< insidePixels->operator [](i).x() << ", ";
        std::cout << insidePixels->operator [](i).y() << ", ";
        std::cout << insidePixels->operator [](i).z() << " )" << std::endl;
    }
    */

    return insidePixels;

}

void blMppContainerSilhouette3D::printConsole(){
    for(int l = 0; l <(*m_data).size(); ++l){
        cout << "layout " << l << endl;
        for (int i = 0 ; i <(*m_data)[l].size() ; ++i){
            for (int j = 0 ; j < (*m_data)[l][i].pairNumber() ; ++j){
                int x = (*m_data)[l][i].lineIdx();
                int y1 = (*m_data)[l][i].getPixelLeftAt(j).y();
                int y2 = (*m_data)[l][i].getPixelRightAt(j).y();

                cout << "line: " << x << " ( " << y1 << ", " << y2 << "), layout id = " << (*m_data)[l][i].layoutIdx() << endl;
            }
        }
    }
}

vector<vector<blMppContainerSilhouette3DLine> >* blMppContainerSilhouette3D::data(){
    return m_data;
}


blMppContainerSilhouette3D* blMppContainerSilhouette3D::translate(blPixel pixel){

    //std::cout << "blMppContainerSilhouette3D::translate begin" << std::endl;
    std::vector<std::vector<blMppContainerSilhouette3DLine> > *dataNew = new std::vector<std::vector<blMppContainerSilhouette3DLine> >;
    dataNew->resize(m_data->size());
    for (int i = 0 ; i < m_data->size() ; ++i){
        for (int j = 0 ; j < (*m_data)[i].size() ; ++j){
            (*dataNew)[i].push_back( (*m_data)[i][j].translate(pixel, (*m_data)[i][j].layoutIdx()) );
        }
    }
    blMppContainerSilhouette3D *silhouette = new blMppContainerSilhouette3D;
    silhouette->setArea(m_area);
    //std::cout << "blMppContainerSilhouette3D:: set Data" << std::endl;
    silhouette->setData(dataNew);

    //std::cout << "blMppContainerSilhouette3D::translate end" << std::endl;
    return silhouette;

}

blMppContainerSilhouette3D* blMppContainerSilhouette3D::copy(){

    blMppContainerSilhouette3D* silhouette = new blMppContainerSilhouette3D();

    silhouette->setArea(m_area);
    std::vector<std::vector<blMppContainerSilhouette3DLine> > *data = new std::vector<std::vector<blMppContainerSilhouette3DLine> >();
    data->resize(m_data->size());
    for (int i = 0 ; i < m_data->size() ; ++i){
        for (int j = 0 ; j < (*m_data)[i].size() ; ++j){
            (*data)[i].push_back( (*m_data)[i][j] );
        }
    }
    silhouette->setData(data);
    return silhouette;
}

int blMppContainerSilhouette3D::silhouetteMaxLength(){

    /// \todo implement this function if needed
    return 0;
}

// //////////////////////////////////////////////// //
//             blMppContainerSilhouette3DLine
// /////////////////////////////////////////////// //
blMppContainerSilhouette3DLine::blMppContainerSilhouette3DLine(){
    m_lineIdx = -1;
    m_layoutIdx = -1;
}

blMppContainerSilhouette3DLine::blMppContainerSilhouette3DLine(int lineIdx){
    m_lineIdx = lineIdx;
}

blMppContainerSilhouette3DLine::blMppContainerSilhouette3DLine(int layoutIdx, int lineIdx, blPixel leftPixel, blPixel rightPixel){

    m_layoutIdx = layoutIdx;
    m_lineIdx = lineIdx;
    m_pixelsLeft.resize(1);
    m_pixelsRight.resize(1);
    m_pixelsLeft[0] = leftPixel;
    m_pixelsRight[0] = rightPixel;
}

void blMppContainerSilhouette3DLine::addPointPair(const blPixel &leftPixel, const blPixel &rightPixel){
    m_pixelsLeft.push_back(leftPixel);
    m_pixelsRight.push_back(rightPixel);
    m_lineIdx = m_pixelsLeft[m_pixelsLeft.size()-1].x();
    m_layoutIdx = m_pixelsLeft[m_pixelsLeft.size()-1].z();
}

// getters / setters
int blMppContainerSilhouette3DLine::lineIdx(){
    return m_lineIdx;
}

int blMppContainerSilhouette3DLine::layoutIdx(){
    return m_layoutIdx;
}

int blMppContainerSilhouette3DLine::pairNumber(){
    return m_pixelsLeft.size();
}

blPixel blMppContainerSilhouette3DLine::getPixelLeftAt(int i){
    return m_pixelsLeft[i];
}

blPixel blMppContainerSilhouette3DLine::getPixelRightAt(int i){
    return m_pixelsRight[i];
}

void blMppContainerSilhouette3DLine::setData(int layoutIdx,int lineIdx, std::vector<blPixel> &left, std::vector<blPixel> &right){
    m_layoutIdx = layoutIdx;
    m_lineIdx = lineIdx;
    m_pixelsLeft = left;
    m_pixelsRight = right;
}

blMppContainerSilhouette3DLine blMppContainerSilhouette3DLine::translate(blPixel t, int layoutId){

    std::vector<blPixel> pixelsLeft; pixelsLeft.resize(m_pixelsLeft.size());
    std::vector<blPixel> pixelsRight; pixelsRight.resize(m_pixelsRight.size());

    int newX = m_lineIdx+t.x();
    for (int i = 0 ; i < pixelsLeft.size() ; ++i){
        pixelsLeft[i] = blPixel(newX, m_pixelsLeft[i].y()+t.y(), m_pixelsLeft[i].z() + t.z());
        pixelsRight[i] = blPixel(newX, m_pixelsRight[i].y()+t.y(), m_pixelsRight[i].z() + t.z());
    }

    blMppContainerSilhouette3DLine line;
    line.setData(layoutId + t.z(), newX, pixelsLeft, pixelsRight);
    return line;
}

void blMppContainerSilhouette3DLine::clear(){
    // m_layoutIdx = 0;
    m_lineIdx = 0;
    m_pixelsLeft.clear();
    m_pixelsRight.clear();
}
