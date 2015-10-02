/// \file blAtTrackerGraphDetections.h
/// \brief blAtTrackerGraphDetections class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2013

#pragma once

#include <lemon/list_graph.h>
#include <lemon/concepts/digraph.h>
#include <lemon/lgf_reader.h>
#include <lemon/lgf_writer.h>
#include <lemon/capacity_scaling.h>

#include "blAtTrackerInterface.h"
#include "blAtCostInterface.h"
#include "blAtDetection.h"
#include "blAssociationTrackingExport.h"

// /////////////////////////////////////////////////////// //
//                   blAtTrackerGraphDetections
// ////////////////////////////////////////////////////// //
/// \class blAtTrackerGraphDetections
/// \brief Class that allows to track particules in several frames using a
/// graph containing all the detections. Tracks are then extracted using shortest path runed iterativelly
class BLASSOCIATIONTRACKING_EXPORT blAtTrackerGraphDetections : public blAtTrackerInterface{

public:
    /// \fn blAtTrackerGraphDetections(blAtCostInterface *costFunction, std::vector<std::string> framesFiles, std::vector<std::vector<std::vector<float> > > objects);
    /// \brief Constructor
    /// \param[in] framesFiles List of the images names
    /// \param[in] objects List of detected objects in each frame object [frame][rank object][object info]
    /// \param[in] costFunction Pointer to the object that calculate the cost function
    blAtTrackerGraphDetections(blAtCostInterface *costFunction, std::vector<std::string> framesFiles, std::vector<std::vector<std::vector<float> > > objects);

    /// \fn blAtTrackerGraphDetections();
    /// \brief Constructor
    blAtTrackerGraphDetections();

    /// \fn ~blAtTrackerGraph();
    /// \brief Destructor
    ~blAtTrackerGraphDetections();

public:
    // parameters
    void initParametersWithDefaultsValues();

public:
    // setters
    /// \fn void setMaxMove(float L);
    /// \param[in] L Maximum distance (in pixels) an object can move
    /// between consecutive rames
    void setMaxMove(float L);
    /// \fn void setJumpPenalty(float penalty);
    /// \param[in] penalty Penalisation value applied to the cost
    /// of a connection that connect none consecutive frames
    void setJumpPenalty(float penalty);
    /// \fn void setCoefIntegerConversion(float coef);
    /// \param[in] coef Multiplication coefficient applied
    /// to the cost (in float) to convert it in integer
    void setCoefIntegerConversion(float coef);
    /// \fn void setOptimizerName(std::string name);
    /// \param[in] name Name of the optimizer to use.
    ///  possible values are "shortestPath" and "minCostFlow"
    void setOptimizerName(std::string name);
    /// \fn void setRequiredFlow(int flow);
    /// \param[in] flow Required flow for the max flow, min cut
    /// optimizer
    void setRequiredFlow(int flow);
    /// \fn void setUseOptimizedGraph(int useOptimizedBuild);
    /// \param[in] useOptimizedBuild 1 if use the optimized version of graph
    /// construction, o otherwise
    void setUseOptimizedGraph(int useOptimizedBuild);

public:
    // virtuals
    /// \fn void run();
    /// \brief Start the calculation
    void run();

private:
    // methods
    void calculateInitialConnections();
    void buildGraphNegative(lemon::ListDigraph &g, lemon::ListDigraph::ArcMap<int> &cap, lemon::ListDigraph::ArcMap<int> &cost, lemon::ListDigraph::Node &source, lemon::ListDigraph::Node &target, std::vector<lemon::ListDigraph::Arc> &arcs, std::vector< std::vector<int> > &arcsInOutMap);
    void buildGraphNegativeOptimized(lemon::ListDigraph &g, lemon::ListDigraph::ArcMap<int> &cap, lemon::ListDigraph::ArcMap<int> &cost, lemon::ListDigraph::Node &source, lemon::ListDigraph::Node &target, std::vector<lemon::ListDigraph::Arc> &arcs, std::vector<std::vector<int> > &nodesConnectedArcsIds);
    void buildGraphNegative(lemon::ListDigraph &g, lemon::ListDigraph::ArcMap<int> &cap, lemon::ListDigraph::ArcMap<int> &cost, lemon::ListDigraph::Node &source, lemon::ListDigraph::Node &target, std::vector<lemon::ListDigraph::Arc> &arcs);

    int findNode( std::vector<blAtDetection> nodesData, int i, int j);
    int CalculateNodeIdx( int t, int nt);
    void updateMinCostFlow();
    void updateShortestPath();

    // parameters
    float m_jumpEpsilon; ///< see setJumpPenalty
    float m_coefIntegerConversion; ///< setCoefIntegerConversion
    int m_requiredFlow; ///< see setRequiredFlow
    std::string m_optimizer; ///< name of the optimizer
    bool m_negativeNodes; ///< always true
    bool m_useOptimizedBuild; ///< see setUseOptimizedGraph

    // internal data
    std::vector<blAtDetection> m_nodesData; ///< contains the initial detections
};

// for optimization
/// \fn bool compareX(std::vector<float> object1, std::vector<float> object2);
/// \brief Compare X position of two objects (ie two detections)
/// \return true if X position of Object 1 is smaller than X position of
/// object 2, and false otherwise
/// \param[in] object1 First object to compare
/// \param[in] object2 Second object to compare
bool compareX(std::vector<float> object1, std::vector<float> object2);
/// \fn bool compareY(std::vector<float> object1, std::vector<float> object2);
/// \brief Compare Y position of two objects (ie two detections)
/// \return true if Y position of Object 1 is smaller than Y position of
/// object 2, and false otherwise
/// \param[in] object1 First object to compare
/// \param[in] object2 Second object to compare
bool compareY(std::vector<float> object1, std::vector<float> object2);
