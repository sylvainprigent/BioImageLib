#pragma once

#include <blAssociationTracking>
#include <blTxtIO>

class blAtFactories{

public:
    blAtFactories(std::string parameterFile);
    blAtFactories(blProcessParameters* parameters);
    ~blAtFactories();

public:
    blAtCostInterface* cost(std::string name);
    blAtTrackerInterface* tracker(std::string name);
    blAtTrackSorterInterface* sorter(std::string name);

private:
    blProcessParameters* m_parameters;
    blProcessObserverCommandeLine *m_observer;
};

