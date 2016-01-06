#pragma once

using namespace std;
#include <blParticleFiltering>
#include <blTxtIO>

// //////////////////////////////////////// //
//             blPfModelsFactory
// //////////////////////////////////////// //
class blPfModelsFactory{

public:
    blPfModelsFactory(std::string parameterFile);
    blPfModelsFactory(blProcessParameters* parameters);
    ~blPfModelsFactory();

public:
    blPfModelInterface* model(blPfDataManager* dataManager, std::string name);

private:
    blProcessParameters* m_parameters;
    blProcessObserverCommandeLine *m_observer;
};
