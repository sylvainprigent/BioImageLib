/// \file blMppAlgorithm.cpp
/// \brief blMppAlgorithm class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppAlgorithm.h"

using namespace std;

// ///////////////////////////////////////////////////////////////////// //
//                            blMppAlgorithm
// ///////////////////////////////////////////////////////////////////// //
blMppAlgorithm::blMppAlgorithm() : blProcess(){
    m_shapeGenerator = NULL;
    m_dataTerm = NULL;
    m_interaction = NULL;
    m_shapeMaxLength = -1;
}

blMppAlgorithm::blMppAlgorithm(blMppGeneratorShape* shapeGenerator, blMppDataTerm* dataTerm, blMppInteraction* interaction) : blProcess(){
    m_shapeGenerator = shapeGenerator;
    m_dataTerm = dataTerm;
    m_interaction = interaction;
    m_shapeMaxLength = -1;
    connectDataTermGenerator();
}

void blMppAlgorithm::connectDataTermGenerator(){
    m_shapeGenerator->setBorderWidth(m_dataTerm->borderWidth());
    m_shapeMaxLength = m_shapeGenerator->shapesMaxLength();

    m_shapeGenerator->setCalculateBorderPixels(m_dataTerm->useBorderPixels());
    m_shapeGenerator->setCalculateInsidePixels(m_dataTerm->useInsidePixels());
    m_shapeGenerator->setCalculateNormals(m_dataTerm->useNormals());

    m_dataTerm->setGeneratorName(m_shapeGenerator->name());
    m_interaction->setGeneratorName(m_shapeGenerator->name());
    m_interaction->setGeneratorType(m_shapeGenerator->is3DShape());
}

blMppAlgorithm::~blMppAlgorithm(){

}

std::string blMppAlgorithm::name(){
    return "blMppAlgorithm";
}

// setters
void blMppAlgorithm::setShapeGenerator(blMppGeneratorShape *shapeGenerator){
    m_shapeGenerator = shapeGenerator;
}

void blMppAlgorithm::setdataTerm(blMppDataTerm *dataTerm){
    m_dataTerm = dataTerm;
}

void blMppAlgorithm::setInteraction(blMppInteraction* interaction){
    m_interaction = interaction;
}

void blMppAlgorithm::setShapeMaxLength(int shapeMaxLength){
    m_shapeMaxLength = shapeMaxLength;
}

blTableNumber* blMppAlgorithm::getShapesTable(){

    blTableNumber* table = new blTableNumber;

    std::vector<blMppShape*> shapes = GetShapes();

    if (shapes.size() > 0){

        // title
        table->setTitle(shapes[0]->name());

        // header
        table->setHeaders( shapes[0]->parametersDescription());

        // data
        std::vector<std::vector<float> > shapesData;
        shapesData.resize(shapes.size());
        for (int i = 0 ; i < shapes.size() ; ++i){
            shapesData[i] = shapes[i]->getParameters();
        }
        table->setData(shapesData);
    }

    return table;
}
