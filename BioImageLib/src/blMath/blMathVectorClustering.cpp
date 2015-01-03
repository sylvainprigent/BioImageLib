/// \file blMathVectorClustering.cpp
/// \brief blVectorKMeans class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blMathVectorClustering.h"
#include "blMathVector.h"

#include <iostream>
#include <stdlib.h>
#include <blCore>
#include "blRand/blRandomGeneratorPM.h"

using namespace std;

blVectorKMeans::blVectorKMeans(std::vector<float> inputVector, int classNumber){

    this->inputVector = inputVector;
    this->classNumber = classNumber;
    this->numLayers = 1;
    iterMax = 200;
}

blVectorKMeans::blVectorKMeans(std::vector<float> inputVector, int numLayers, int classNumber){
    this->inputVector = inputVector;
    this->classNumber = classNumber;
    iterMax = 200;
    this->numLayers = numLayers;
}

void blVectorKMeans::Update(){
    if (numLayers == 1){
        Update1();
    }
    else{
        UpdateN();
    }
}

void blVectorKMeans::UpdateN(){
    int nc = numLayers;
    int nl = inputVector.size()/numLayers;

    means.resize(nl*nc);

    // 1- initialisation
    // 1.1- randomly initialize the class centers
    //cout << "initial centers = " <<endl;
    int pos;
    for (int c = 0 ; c < classNumber ; ++c){
        pos = int(blRandomGeneratorPM::rand() * float(nl));
        for (int j = 0 ; j < nc ; ++j){
            means[nc*c+j] = inputVector[nc*pos+j];
            //cout << means[nc*c+j] << "     \t";
        }
        //cout << endl;
    }

    // 1.2 Init the clusters
    vector<int> clusteringOld; clusteringOld.resize(nl);
    vector<float> dist; dist.resize(classNumber);
    for (int i = 0 ; i < nl ; ++i){
        dist.clear(); dist.resize(classNumber);
        for (int c = 0 ; c < classNumber ; ++c){
            dist[c] = 0;
            for (int j = 0 ; j < nc ; ++j){
                dist[c] += pow(means[nc*c+j] - inputVector[nc*i+j], 2);
            }
        }
        clusteringOld[i] = blMathVector::minPos(dist);
    }

    // 2- main loop
    bool convergence = false;
    clustering.resize(nl);
    clusterCount.resize(classNumber);
    iter = 0;
    while (!convergence){
        iter++;
        // 2.1- re-evaluate the clustering
        for (int i = 0 ; i < nl ; ++i){
            for (int c = 0 ; c < classNumber ; ++c){
                dist[c] = 0;
                for (int j = 0 ; j < nc ; ++j){
                    dist[c] += pow(means[nc*c+j] - inputVector[nc*i+j], 2);
                }
            }
            clustering[i] = blMathVector::minPos(dist);
        }

        // 2.2- re-evaluate the centers
        means.clear(); clusterCount.clear();
        means.resize(nc*classNumber); clusterCount.resize(classNumber);
        for (int i = 0 ; i < nl ; ++i){
            for (int j = 0 ; j < nc; ++j){
                means[nc*clustering[i]+j] += inputVector[nc*i+j];
            }
            clusterCount[clustering[i]] ++;
        }
        for (int c = 0 ; c < classNumber ; ++c){
            for (int j = 0 ; j < nc; ++j){
                means[nc*c+j] /= clusterCount[c];
            }
        }

        bool stop = true;
        for (int i = 0 ; i < nl ; ++i){
            if (clusteringOld[i] != clustering[i]){
                stop = false;
                break;
            }
        }

        // 2.3- stop criterion
        if (iter > iterMax || (stop && iter > 10)){
            convergence = true;
        }
        clusteringOld = clustering;
    }

}

void blVectorKMeans::Update1(){
    int dataSize = inputVector.size();
    means.resize(classNumber);

    // 1- initialisation
    // 1.1- randomly initialize the class centers
    float maxi = inputVector[0];
    float val;
    for (int i = 0 ; i < dataSize ; ++i){
        val = inputVector[i];
        if (val > maxi)
            maxi = val;
    }

    for (int c = 0 ; c < classNumber ; ++c){
        int purcent = int(blRandomGeneratorPM::rand() * 100.0);
        means[c] = purcent*maxi/100;
    }

    // 1.2 Init the clusters
    vector<int> clusteringOld; clusteringOld.resize(dataSize);
    vector<float> dist; dist.resize(classNumber);
    for (int i = 0 ; i < dataSize ; ++i){
        dist.clear(); dist.resize(classNumber);
        for (int c = 0 ; c < classNumber ; ++c){
            dist[c] = pow(means[c] - inputVector[i], 2);
        }
        clusteringOld[i] = blMathVector::minPos(dist);
    }

    // 2- main loop
    bool convergence = false;
    clustering.resize(dataSize);
    clusterCount.resize(classNumber);
    iter = 0;
    while (!convergence){
        iter++;
        // 2.1- re-evaluate the clustering
        for (int i = 0 ; i < dataSize ; ++i){
            dist.clear(); dist.resize(classNumber);
            for (int c = 0 ; c < classNumber ; ++c){
                dist[c] = pow(means[c] - inputVector[i], 2);
            }
            clustering[i] = blMathVector::minPos(dist);
        }

        // 2.2- re-evaluate the centers
        means.clear(); clusterCount.clear();
        means.resize(classNumber); clusterCount.resize(classNumber);
        for (int i = 0 ; i < dataSize ; ++i){
            means[clustering[i]] += inputVector[i];
            clusterCount[clustering[i]] ++;
        }
        for (int c = 0 ; c < classNumber ; ++c){
            means[c] /= clusterCount[c];
        }

        bool stop = true;
        for (int i = 0 ; i < dataSize ; ++i){
            if (clusteringOld[i] != clustering[i]){
                stop = false;
                break;
            }
        }

        // 2.3- stop criterion
        if (iter > iterMax || (stop && iter > 10)){
            convergence = true;
        }
        clusteringOld = clustering;
    }
}

vector<int> blVectorKMeans::GetClassification(){
    return clustering;
}

vector<float> blVectorKMeans::GetCenters(){
    return means;
}

int blVectorKMeans::GetNumberOfIter(){
    return iter;
}

void blVectorKMeans::SetMaxNumberOfIter(int iterMax){
    this->iterMax = iterMax;
}

vector<int> blVectorKMeans::GetNumberValuesPerCluster(){
    return clusterCount;
}
