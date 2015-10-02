/// \file blMppCluster.h
/// \brief blMppCluster class
/// \author Yellamraju Tarun (yellamraju.tarun@inria.fr)
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once

#include <vector>
#include <string>
#include <blCore>
#include "blMppContainerSilhouette.h"
#include "blMppShapeGraph.h"
#include "blMppExport.h"

/// \class blMppCluster
/// \brief Class defining an Shape for a Marked Point Process
class BLMPP_EXPORT blMppCluster{

public:
    /// \fn blMppCluster();
    /// \brief Constructor
    blMppCluster();
    /// \fn virtual ~blMppCluster();
    /// \brief Desctructor
    virtual ~blMppCluster();

public:

    //EDITS
    void setLabel(int Label);
    int getLabel();
    void addConnection(blMppCluster *new_cluster, float value);
    void AddConnection (blMppCluster* new_shape) ;
    double DeleteConnection (double ID);
    void AddEnergy (float energy);
    void DeleteEnergy (double shape_index);
    void setId(double ID);
    double getId();
    std::vector<blMppCluster *> getconnection();
    std::vector<float> getenergy ();
    void setConnectionStatus (int status, double shape_index);
    void setConnectionStatus2 (int status, double ID);
    int getConnectionStatus(double ID);
    std::vector<int> getConnectionStatus();
    void AddDefaultConnectionStatus ();
    void setgraphid (double id);
    double getgraphid ();
    void destroygraph ();
    double find_location(double ID);
    void modify_connection(int index, blMppCluster* cluster );
    void modify_energy (int index, float energy);

    //New in clusters
    void setDataTerm();
    float dataTerm();
    void addshape (blMppShapeGraph *);
    std::vector <blMppShapeGraph *> get_content();
    int get_size();
    float find_interaction (blMppShapeGraph *);
    //bool merge (blMppCluster *, blMppAlgorithmMBCR3 *algo);
    //void delete_cluster (blMppCluster *cluster);
    float set_interaction();
    void set_merge (bool flag);
    bool get_merge ();

    bool merge(blMppCluster * cluster);

    void setDataTerm(float dataTerm);

    /// \fn void setGoToCut(bool value);
    /// \brief true if the shape has been kept by at least one cut step
    void setGoToCut(bool value);
    /// \fn bool goneToCut();
    /// return  true if the shape has been kept by at least one cut step
    bool goneToCut();

    void removeConnection(int clusterId);
    void updateEnergy(int clusterId, float value);

public:
    float dataTerm_cluster;


    int Label;
    double id;
    double graphid;

    std::vector <blMppCluster*> Connection;
    std::vector <float> InteractionValue;
    std::vector <int> ConnectionStatus;
    std::vector <blMppShapeGraph *> content;
    int shape_count;
    bool in_merge;
    bool m_goneToCut;

};
