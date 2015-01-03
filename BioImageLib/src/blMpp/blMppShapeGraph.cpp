/// \file blMppShapeGraph.cpp
/// \brief blMppShapeGraph class
/// \author Yellamraju Tarun (yellamraju.tarun@inria.fr)
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#include "blMppShapeGraph.h"

using namespace std;

blMppShapeGraph::blMppShapeGraph(blMppShape* shape){
    m_shape = shape;

    //EDIT
    m_label = 12;
    m_cluster_ID = 0;
    m_id = -1;
    m_graphid = -1;

}

blMppShapeGraph::~blMppShapeGraph(){
    int N = m_connection.size();
    for (int i = 0; i < N ; i++ )
    {
        int k = m_connection[i]->deleteConnection(m_id);
        if(k!=10000)
            m_connection[i]->deleteEnergy(k);
    }

    delete m_shape;
}

blMppShape* blMppShapeGraph::shape(){
    return m_shape;
}

//EDITS : Function Definitions
void blMppShapeGraph::setLabel (int l)
{
    m_label = l;
}

int blMppShapeGraph::getLabel()
{
    return m_label;
}

void blMppShapeGraph::setId(double ID)
{
    m_id = ID;
}

double blMppShapeGraph::getId()
{
    return m_id;
}

void blMppShapeGraph::addConnection(blMppShapeGraph *new_shape)
{
    int N = m_connection.size();
    if (N>10000)
    {
        std::cout<<"ERROR: Junk object detected"<<std::endl;
    }
    m_connection.push_back(new_shape);
}

double blMppShapeGraph::deleteConnection(double ID)
{
    int N =m_connection.size();
    //if (N>10000)
    //{
      //  std::cout<<"ERROR: Junk object detected"<<std::endl;
    //}
    //std::cout << "size before = " << Connection.size() <<std::endl;
    //for (vector<blMppShapeGraph*>::size_type i = 0; i < N ; i++ )
    for (int i = 0; i < N ; i++ )
    {
        /*if (Connection[i]->Connection.size() > 10000)
        {
            std::cout<<"ERROR: junk object detected !!"<<std::endl;
            std::cout<<"Connection size : "<<Connection[i]->Connection.size()<<std::endl;
            std::cout<<"ID of junk object : "<<Connection[i]->getId()<<std::endl;
            std::cout<<"i = "<<i<<std::endl;
            //delete Connection[i];
        }*/

        if (m_connection[i]->m_id == ID)
        {
            m_connection.erase(m_connection.begin()+i);
            m_connectionStatus.erase(m_connectionStatus.begin()+i);
            //InteractionValue.erase(InteractionValue.begin()+ i);
            return i;
            //break;
        }
    }
    std::cout<<"Object not found !!"<<std::endl;
    return 10000;
}

void blMppShapeGraph::addEnergy(float energy)
{
    m_interactionValue.push_back(energy);
}

void blMppShapeGraph::setConnectionStatus(bool status, double shape_index)
{
    m_connectionStatus[shape_index] = status;
}

void blMppShapeGraph::setConnectionStatus2(bool status, double ID)
{
    int N = m_connection.size();
    //std::cout << "N = " << N << std::endl;
    //std::cout << "ConnectionStatus = " << ConnectionStatus.size() << std::endl;
    //for (vector<blMppShapeGraph*>::size_type i = 0; i < N ; i++ )
    for (int i = 0; i < N ; i++ )
    {
        //std::cout << "i = " << i << std::endl;
        if (m_connection[i]->m_id == ID)
        {
            //ConnectionStatus[i]=1;
            m_connectionStatus[i]=status;
            break;
        }
    }
    //std::cout << "setConnectionStatus2 end " << std::endl;
}

bool blMppShapeGraph::getConnectionStatus(double ID)
{
    int N = m_connection.size();
    for (vector<blMppShape*>::size_type i = 0; i < N ; i++ )
    {
        if (m_connection[i]->m_id == ID)
        {
            return m_connectionStatus[i];
            break;
        }
    }
    return 0;
}

void blMppShapeGraph::addDefaultConnectionStatus()
{
    m_connectionStatus.push_back(0);
}

void blMppShapeGraph::destroygraph()
{
    int M = m_connectionStatus.size();
    m_connectionStatus.clear();
    for(int i = 0; i<M; ++i)
    {
        m_connectionStatus.push_back(0);
    }
}

void blMppShapeGraph::deleteEnergy(double shape_index)
{
    /*if(InteractionValue.size()>1 && shape_index != 0)
    {
        InteractionValue.erase(InteractionValue.begin()+shape_index);
    }
    else if(InteractionValue.size()>1 && shape_index == 0)
    {
        InteractionValue.erase(InteractionValue.begin());
    }
    else
        InteractionValue.clear();*/
    //std::cout << "shape idx = " << shape_index << std::endl;
    if(!m_interactionValue.empty())
    {
        if(shape_index != 0)
        {
            m_interactionValue.erase(m_interactionValue.begin()+shape_index);
        }
        else
            m_interactionValue.erase(m_interactionValue.begin());
    }

}

std::vector<blMppShapeGraph*> blMppShapeGraph::getconnection()
{
    return m_connection;
}

std::vector<float> blMppShapeGraph::getenergy()
{
    return m_interactionValue;
}

void blMppShapeGraph::setgraphid(double id)
{
    m_graphid = id;
}

double blMppShapeGraph::getgraphid()
{
    return m_graphid;
}

void blMppShapeGraph::update_cluster(double id)
{
    m_cluster_ID = id;
}

double blMppShapeGraph::get_CID()
{
    return m_cluster_ID;
}

std::vector<int>& blMppShapeGraph::clusterHistory(){
    return m_clusterHistory;
}

void blMppShapeGraph::addClusterHistory(int id, int iter){
    m_clusterHistory.push_back(id);
    m_clusterHistoryIter.push_back(iter);
}

void blMppShapeGraph::removeLastHistory(){
    m_clusterHistory.erase(m_clusterHistory.end()-1);
    m_clusterHistoryIter.erase(m_clusterHistoryIter.end()-1);
}

std::vector<int>& blMppShapeGraph::clusterHistoryIter(){
    return m_clusterHistoryIter;
}
