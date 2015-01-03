/// \file blMppShapeGraph.h
/// \brief blMppShapeGraph class
/// \author Yellamraju Tarun (yellamraju.tarun@inria.fr)
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once

#include "blMppShape.h"

/// \class blMppShapeGraph
/// \brief define methods to link shapes to memorize interactions
class blMppShapeGraph{

public:
    /// \fn blMppShapeGraph();
    /// \brief Constructor
    blMppShapeGraph(blMppShape* shape);

    /// \fn ~blMppShapeGraph();
    /// \brief Destructor
    ~blMppShapeGraph();

public:
    blMppShape* shape();

public:
    //EDITS adding methods for using attraction energies
    void setLabel(int Label);
    int getLabel();
    void addConnection (blMppShapeGraph* new_shape) ;
    double deleteConnection (double ID);
    void addEnergy (float energy);
    void deleteEnergy (double shape_index);
    void setId(double ID);
    double getId();
    std::vector<blMppShapeGraph *> getconnection();
    std::vector<float> getenergy ();
    void setConnectionStatus (bool status, double shape_index);
    void setConnectionStatus2 (bool status, double ID);
    bool getConnectionStatus (double ID);
    void addDefaultConnectionStatus ();
    void setgraphid (double id);
    double getgraphid ();
    void destroygraph ();
    void update_cluster (double id);
    double get_CID ();

public:
    std::vector<int>& clusterHistory();
    void addClusterHistory(int id, int iter);
    void removeLastHistory();
    std::vector<int>& clusterHistoryIter();

private:
    blMppShape* m_shape;

protected:
    //EDITS adding parameters for using attraction energies
    int m_label;
    double m_id;
    double m_graphid;
    std::vector <blMppShapeGraph*> m_connection;
    std::vector <float> m_interactionValue;
    std::vector <bool> m_connectionStatus;
    double m_cluster_ID;
    std::vector<int> m_clusterHistory;
    std::vector<int> m_clusterHistoryIter;
};
