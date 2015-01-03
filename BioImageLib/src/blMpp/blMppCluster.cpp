/// \file blMppCluster.cpp
/// \brief blMppCluster class
/// \author Yellamraju Tarun (yellamraju.tarun@inria.fr)
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppShape.h"
#include "blMppCluster.h"
#include <iostream>

using namespace std;

blMppCluster::blMppCluster(){

    dataTerm_cluster = 0;
    Label = 12;
    id = -1;
    graphid = -1;
    shape_count = 0;
    in_merge = false;
    m_goneToCut = false;
}

blMppCluster::~blMppCluster(){

    if(!in_merge)
    {
        int N = Connection.size();
        //id = -1;
        for (int i = 0; i < N ; i++ )
        {
            int k = Connection[i]->DeleteConnection(id);
            if (k < 10000){
                Connection[i]->DeleteEnergy(k);
            }
        }
    }

}

void blMppCluster::setDataTerm(){
    std::vector<blMppShapeGraph *> shapes = get_content();
    int n = get_size();
    int i = 0;
    dataTerm_cluster = 0;
    while (n!=0 && i<n)
    {
        dataTerm_cluster += shapes[i]->shape()->dataTerm();
        ++i;
    }
    float internal_interaction = set_interaction();
    dataTerm_cluster += internal_interaction;
}


void blMppCluster::setDataTerm(float dataTerm){
    dataTerm_cluster = dataTerm;
}

float blMppCluster::dataTerm(){
    return dataTerm_cluster;
}

float blMppCluster::set_interaction()
{
    std::vector<blMppShapeGraph *> shapes = get_content();
    int n = get_size();
    float energy = 0;

    for(int i=0; i<n; ++i)
    {
        double id = shapes[i]->getId();
        for(int j = i+1; j<n; ++j)
        {
            std::vector<blMppShapeGraph *> C = shapes[j]->getconnection();
            std::vector<float> E = shapes[j]->getenergy();
            int N = C.size();
            for (int k = 0; k<N; ++k)
            {
                if(C[k]->getId() == id)
                {
                    energy += E[k];
                    break;
                }
            }
        }
    }

    return energy;
}

float blMppCluster::find_interaction(blMppShapeGraph *newshape)
{
    std::vector<blMppShapeGraph *> shapes = content;
    int n = shape_count;
    float energy = 0;

    double id = newshape->getId();
    for (int i=0; i<n; ++i)
    {
        //std::cout << " ------------------ enter -----------------------" << std::endl;
        std::vector<blMppShapeGraph *> C = shapes[i]->getconnection();
        std::vector<float> E = shapes[i]->getenergy();
        int N = C.size();
        for (int j = 0; j<N; ++j)
        {
            if(C[j]->getId() == id)
            {
                energy += E[j];
                break;
            }
        }
    }

    return energy;

}

void blMppCluster::addshape(blMppShapeGraph *shape)
{
    content.push_back(shape);
    shape_count++;
}

int blMppCluster::get_size()
{
    return shape_count;
}

std::vector<blMppShapeGraph *> blMppCluster::get_content()
{
    return content;
}



//EDITS : Function Definitions
void blMppCluster::setLabel (int l)
{
    Label = l;
}

int blMppCluster::getLabel()
{
    return Label;
}

void blMppCluster::setId(double ID)
{
    id = ID;
}

double blMppCluster::getId()
{
    return id;
}

void blMppCluster::addConnection(blMppCluster *new_cluster, float value){
    int N = Connection.size();
    bool found = false;
    for (int i = 0 ; i < N ; ++i){
        if (Connection[i]->getId() == new_cluster->getId()){
            InteractionValue[i] = InteractionValue[i] + value;
            found = true;
            break;
        }
    }

    if (!found){
        Connection.push_back(new_cluster);
        InteractionValue.push_back(value);
        ConnectionStatus.push_back(0);
    }
}

void blMppCluster::AddConnection(blMppCluster *new_shape)
{
    int N = Connection.size();
    if (N>10000)
    {
        std::cout<<"ERROR: Junk object detected"<<std::endl;
    }
    Connection.push_back(new_shape);
}

double blMppCluster::DeleteConnection(double ID)
{
    int N = Connection.size();

    for (int i = N-1; i >=0 ; i-- )
    {
        if (Connection[i]->id == ID)
        {
            Connection.erase(Connection.begin()+i);
            ConnectionStatus.erase(ConnectionStatus.begin()+i);
            //InteractionValue.erase(InteractionValue.begin()+ i);
            return i;
            //break;
        }
    }
    std::cout<<"Object not found !!"<<std::endl;
    return 10000;
}

void blMppCluster::AddEnergy(float energy)
{
    InteractionValue.push_back(energy);
}

void blMppCluster::setConnectionStatus(int status, double shape_index)
{
    ConnectionStatus[shape_index] = status;
}

void blMppCluster::setConnectionStatus2(int status, double ID)
{
    int N = Connection.size();
    for (int i = 0; i < N ; i++ )
    {
        //std::cout << "i = " << i << std::endl;
        if (Connection[i]->getId() == ID)
        {
            //ConnectionStatus[i]=1;
            ConnectionStatus[i]=status;
            break;
        }
    }
    //std::cout << "setConnectionStatus2 end " << std::endl;
}

int blMppCluster::getConnectionStatus(double ID)
{
    int N = Connection.size();
    for (vector<blMppCluster*>::size_type i = 0; i < N ; i++ )
    {
        if (Connection[i]->id == ID)
        {
            return ConnectionStatus[i];
            break;
        }
    }
    return 2;
}

void blMppCluster::AddDefaultConnectionStatus()
{
    ConnectionStatus.push_back(0);
}

void blMppCluster::destroygraph()
{
    int M = ConnectionStatus.size();
    ConnectionStatus.clear();
    for(int i = 0; i<M; ++i)
    {
        ConnectionStatus.push_back(0);
    }
}

void blMppCluster::DeleteEnergy(double shape_index)
{
     if(!InteractionValue.empty())
    {
        if(shape_index != 0)
        {
            InteractionValue.erase(InteractionValue.begin()+shape_index);
        }
        else
            InteractionValue.erase(InteractionValue.begin());
    }

}

std::vector<blMppCluster *> blMppCluster::getconnection()
{
    return Connection;
}

std::vector<float> blMppCluster::getenergy()
{
    return InteractionValue;
}

void blMppCluster::setgraphid(double id)
{
    graphid = id;
}

double blMppCluster::getgraphid()
{
    return graphid;
}

double blMppCluster::find_location(double ID)
{
    int N = Connection.size();
    for (vector<blMppCluster*>::size_type i = 0; i < N ; i++ )
    {
        if (Connection[i]->id == ID)
        {
            return i;
        }
    }
    return 10000;
}

void blMppCluster::modify_connection(int index, blMppCluster *cluster)
{
    Connection[index] = cluster;
}

void blMppCluster::modify_energy(int index, float energy)
{
    InteractionValue[index] += energy;
}

void blMppCluster::set_merge(bool flag)
{
    in_merge = flag;
}

bool blMppCluster::get_merge()
{
    return in_merge;
}

std::vector<int> blMppCluster::getConnectionStatus(){
    return ConnectionStatus;
}

/// \fn void setGoToCut(bool value);
/// \brief true if the shape has been kept by at least one cut step
void setGoToCut(bool value);
/// \fn bool goneToCut();
/// return  true if the shape has been kept by at least one cut step
bool goneToCut();


bool blMppCluster::merge(blMppCluster * cluster)
{

    std::cout << "blMppCluster::merge begin" << std::endl;

    // data term
    dataTerm_cluster += cluster->dataTerm();

    // remove merged connections
    for (int i = Connection.size()-1 ; i >= 0  ; i--){
        std::cout << "toto " << std::endl;
        if ( Connection[i]->getId() == cluster->getId() ){

            std::cout << "dataTerm_cluster before = " << dataTerm_cluster << std::endl;
            std::cout << "InteractionValue = " << InteractionValue[i] << std::endl;
            dataTerm_cluster -= InteractionValue[i];
            std::cout << "dataTerm_cluster after = " << dataTerm_cluster << std::endl;
            Connection.erase(Connection.begin() + i);
            ConnectionStatus.erase(ConnectionStatus.begin() + i);
            InteractionValue.erase(InteractionValue.begin() + i);
        }
    }

    std::vector <blMppCluster*> mergeConnections = cluster->Connection;
    std::vector <float> mergeValues = cluster->InteractionValue;
    int mergeId, pos;

    //std::vector <blMppCluster*> ConnectionTemp = Connection;

    for (int i = 0 ; i < mergeConnections.size() ; ++i){
        std::cout << "i = " << i << std::endl;
        mergeId = mergeConnections[i]->getId();
        mergeConnections[i]->removeConnection(cluster->getId());

        std::cout << "mergeId = " << mergeId << std::endl;
        if (mergeId != id){
            std::cout << "search if a connection already exists" << std::endl;
            // search if a connection already exists
            pos = -1;
            for (int j = 0 ; j < Connection.size() ; ++j){
                if (Connection[j]->getId() == mergeId){
                    pos = j;
                    break;
                }
            }
            std::cout << "pos = " << pos << std::endl;
            if (pos < 0){
                Connection.push_back(mergeConnections[i]);
                InteractionValue.push_back(mergeValues[i]);
                ConnectionStatus.push_back(0);

                // update the connection of the cluster linked to the
                mergeConnections[i]->AddConnection(this);
                mergeConnections[i]->AddEnergy (mergeValues[i]);
                mergeConnections[i]->AddDefaultConnectionStatus();

            }
            else{
                InteractionValue[pos] = InteractionValue[pos] + mergeValues[i];

                mergeConnections[i]->updateEnergy(id, InteractionValue[pos]);
                //mergeConnections[i]->AddConnection(this);
                //mergeConnections[i]->AddEnergy (InteractionValue[pos]);
                //mergeConnections[i]->AddDefaultConnectionStatus();
            }
        }
    }

    std::cout << "copy all the shapes to the cluster list " << std::endl;
    // copy all the shapes to the cluster list
    for (int s = 0 ; s < cluster->content.size() ; ++s){
        blMppShapeGraph* shapeToAdd = cluster->content[s];
        shapeToAdd->update_cluster(id);
        shapeToAdd->setLabel(this->getLabel());
        addshape(shapeToAdd);
    }

    // check if a connection is several time in the list
    for (int i = 0 ; i < Connection.size() ; i++){
        int curentId = Connection[i]->getId();
        for (int j = i+1 ; j < Connection.size() ; ++j){
            if ( curentId == Connection[j]->getId()){
                std::cout << "----------------------- Found double connection -----------------------" << std::endl;
            }
        }
    }


    std::cout << "blMppCluster::merge end" << std::endl;

}

void blMppCluster::setGoToCut(bool value){
    m_goneToCut = value;
}

bool blMppCluster::goneToCut(){
    return m_goneToCut;
}

void blMppCluster::removeConnection(int clusterId){
    int N = Connection.size();
    for (int i = 0; i < N ; i++ )
    {
        int k = Connection[i]->DeleteConnection(clusterId);
        if (k < 10000){
            Connection[i]->DeleteEnergy(k);
        }
    }
}

void blMppCluster::updateEnergy(int clusterId, float value){
    int N = Connection.size();
    for (int i = 0; i < N ; i++ )
    {
        if ( Connection[i]->getId() == clusterId){
            InteractionValue[i] = value;
            break;
        }
    }
}
