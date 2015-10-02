#include "blAtFactories.h"

blAtFactories::blAtFactories(blProcessParameters* parameters){
    m_parameters = parameters;
    m_observer = new blProcessObserverCommandeLine();
}

blAtFactories::blAtFactories(std::string parameterFile){
    m_parameters = new blProcessParameters(parameterFile);
    m_parameters->load();
    m_observer = new blProcessObserverCommandeLine();
}

blAtFactories::~blAtFactories(){
    delete m_observer;
}

// getters
blAtCostInterface* blAtFactories::cost(std::string name){
    if (name == "blAtCostEuclidean"){
        blAtCostEuclidean* cost = new blAtCostEuclidean;
        return cost;
    }
    if (name == "blAtCostLine"){
        blAtCostLine* cost = new blAtCostLine;
        cost->setMaxMove(m_parameters->getValueOfKey<float>("blAtCostLine_maxMovX", 10.0),
                         m_parameters->getValueOfKey<float>("blAtCostLine_maxMovY", 10.0));

        cost->setMinMove(m_parameters->getValueOfKey<float>("blAtCostLine_minMove", 0));
        cost->setLineImageDir(m_parameters->getValueOfKey<std::string>("blAtCostLine_lineImagesDir", ""));
        return cost;
    }
    if (name == "blAtCostLineEuclidean"){
        blAtCostLineEuclidean* cost = new blAtCostLineEuclidean;
        cost->setMaxMove(m_parameters->getValueOfKey<float>("blAtCostLineEuclidean_maxMovX", 10.0),
                         m_parameters->getValueOfKey<float>("blAtCostLineEuclidean_maxMovY", 10.0));

        cost->setMinMove(m_parameters->getValueOfKey<float>("blAtCostLineEuclidean_minMove", 0));
        cost->setCoefficients(m_parameters->getValueOfKey<float>("blAtCostLineEuclidean_alpha", 0.5),
                              m_parameters->getValueOfKey<float>("blAtCostLineEuclidean_beta", 0.5));
        cost->setLineImageDir(m_parameters->getValueOfKey<std::string>("blAtCostLineEuclidean_lineImagesDir", ""));
        return cost;
    }


    throw blException(("Cannot find the cost " + name).c_str());
}

blAtTrackerInterface* blAtFactories::tracker(std::string name){

    if (name == "blAtTrackerGraphDetections"){
        blAtTrackerGraphDetections* tracker = new blAtTrackerGraphDetections;
        tracker->setMaxMove(m_parameters->getValueOfKey<float>("blAtTrackerGraphDetections_L", 10.0));
        tracker->setTimeSpread(m_parameters->getValueOfKey<float>("blAtTracker_timeSpread", 1));
        tracker->setJumpPenalty(m_parameters->getValueOfKey<float>("blAtTrackerGraphDetections_jumpEpsilon", 0.01));
        tracker->setCoefIntegerConversion(m_parameters->getValueOfKey<float>("blAtTrackerGraphDetections_coefIntegerConversion", 100));
        tracker->setOptimizerName(m_parameters->getValueOfKey<std::string>("blAtTrackerGraphDetections_optimization", "shortestPath"));
        tracker->setRequiredFlow(m_parameters->getValueOfKey<int>("blAtTrackerGraphDetections_requiredFlow", 100));
        tracker->setUseOptimizedGraph(m_parameters->getValueOfKey<int>("blAtTrackerGraphDetections_useOptimizedBuild", 1));

        return tracker;
    }

    throw blException(("Cannot find the tracker " + name).c_str());
}

blAtTrackSorterInterface* blAtFactories::sorter(std::string name){

    /*
    if (name == "blAtTrackSorterAxons2"){
        blAtTrackSorterAxons2 *sorter = new blAtTrackSorterAxons2;
        sorter->setIntensityAlongTrackTh(m_parameters->getValueOfKey<float>("blAtTrackSorterAxons_IntensityAlongTrackTh", 100));
        sorter->setResolution(m_parameters->getValueOfKey<float>("blAtTrackSorterAxons_resolutionX", 0.132),
                              m_parameters->getValueOfKey<float>("blAtTrackSorterAxons_resolutionY", 0.132));
        sorter->compositeTh(m_parameters->getValueOfKey<float>("blAtTrackSorterAxons_compositeTh", 0.2));
        sorter->timeEndTh(m_parameters->getValueOfKey<float>("blAtTrackSorterAxons_timeEndTh", 139));
        sorter->lifeTimeTh(m_parameters->getValueOfKey<float>("blAtTrackSorterAxons_lifeTimeTh", 5));
        return sorter;
    }
    */

    throw blException(("Cannot find the sorter " + name).c_str());
}
