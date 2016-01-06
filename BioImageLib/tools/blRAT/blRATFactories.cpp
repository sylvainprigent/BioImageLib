#include "blRATFactories.h"

blRATFactories::blRATFactories(blProcessParameters* parameters){
    m_parameters = parameters;
    m_observer = new blProcessObserverCommandeLine();
}

blRATFactories::blRATFactories(std::string parameterFile){
    m_parameters = new blProcessParameters(parameterFile);
    m_parameters->load();
    m_observer = new blProcessObserverCommandeLine();
}

blRATFactories::~blRATFactories(){
    delete m_observer;
}

// getters
blRATCostInterface* blRATFactories::cost(std::string name){
    if (name == "blRATCostEuclidean"){
        blRATCostEuclidean* cost = new blRATCostEuclidean;
        return cost;
    }

    throw blException(("Cannot find the cost " + name).c_str());
}
