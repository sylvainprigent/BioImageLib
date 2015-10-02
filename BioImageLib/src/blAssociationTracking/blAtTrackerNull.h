/// \file blAtTrackerNull.h
/// \brief blAtTrackerNull class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2013

#pragma once

#include "blAtTrackerInterface.h"
#include "blAtCostInterface.h"
#include "blAtDetection.h"
#include "blAssociationTrackingExport.h"

// /////////////////////////////////////////////////////// //
//                   blAtTrackerNull
// ////////////////////////////////////////////////////// //
/// \class blAtTrackerNull
/// \brief Class that define an empty tracker. It is usefull for loading and post processing
/// a tracking result stored in a file
class BLASSOCIATIONTRACKING_EXPORT blAtTrackerNull : public blAtTrackerInterface{

public:
    /// \fn blAtTrackerNull(blAtCostInterface *costFunction, std::vector<std::string> framesFiles, std::vector<std::vector<std::vector<float> > > objects);
    /// \brief Constructor
    /// \param[in] framesFiles List of the images names (full path)
    /// \param[in] objects List of detected objects in each frame object [frame][rank object][object info]
    /// \param[in] costFunction Pointer to the object that calculate the cost function
    blAtTrackerNull(blAtCostInterface *costFunction, std::vector<std::string> framesFiles, std::vector<std::vector<std::vector<float> > > objects);

    /// \fn blAtTrackerNull();
    /// \brief Default constructor
    blAtTrackerNull();
    /// \fn ~blAtTrackerGraph();
    /// \brief Destructor
    ~blAtTrackerNull();

public:
    // parameters
    /// \fn void initParametersWithDefaultsValues();
    /// \brief initialize the parameters with default values
    void initParametersWithDefaultsValues();

public:
    // virtual
    /// \fn void run();
    /// \brief Start the calculation
    void run();
};
