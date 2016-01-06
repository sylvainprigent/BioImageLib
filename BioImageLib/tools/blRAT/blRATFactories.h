#pragma once

#include <blRAT>
#include <blTxtIO>

class blRATFactories{

public:
    blRATFactories(std::string parameterFile);
    blRATFactories(blProcessParameters* parameters);
    ~blRATFactories();

public:
    blRATCostInterface* cost(std::string name);

private:
    blProcessParameters* m_parameters;
    blProcessObserverCommandeLine *m_observer;
};

