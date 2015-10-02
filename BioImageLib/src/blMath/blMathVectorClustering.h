/// \file blMathVectorClustering.h
/// \brief blVectorKMeans class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include <vector>
#include "blMathExport.h"

/// \class blVectorKMeans
/// \brief class the split a std::vector or matrix into several classes using the kmeans algorithm
class BLMATH_EXPORT blVectorKMeans{

public:
    /// \fn blVectorKMeans(std::vector<float> inputVector, int classNumber);
    /// \brief constructor for a std::vector
    /// \param[in] inputVector Vector to classify
    /// \param[in] classNumber Number of classes
    blVectorKMeans(std::vector<float> inputVector, int classNumber);
    /// \fn blVectorKMeans(std::vector<float> inputVector, int numLayers, int classNumber);
    /// \brief constructor for matrix
    /// \param[in] inputVector Vector to classify
    /// \param[in] numLayers Number of columns in the matrix (access the data with [nc*i+j])
    /// \param[in] classNumber Number of classes
    blVectorKMeans(std::vector<float> inputVector, int numLayers, int classNumber);

    /// \fn void Update();
    /// \brief Run the Classification
    void Update();
    /// \fn std::vector<int> GetClassification();
    /// \return The Classification result
    std::vector<int> GetClassification();
    /// \fn std::vector<float> GetCenters();
    /// \return Values of the class BLCORE_EXPORT centroids
    std::vector<float> GetCenters();
    /// \fn int GetNumberOfIter();
    /// \return The number of iterations after converge
    int GetNumberOfIter();
    /// \fn std::vector<int> GetNumberValuesPerCluster();
    /// \return The number of entity per class
    std::vector<int> GetNumberValuesPerCluster();
    /// \fn void SetMaxNumberOfIter(int iterMax);
    /// \brief Function that set the max number of iterations (default 200)
    /// \param[in] iterMax Maximum iteration number
    void SetMaxNumberOfIter(int iterMax);

private:
    std::vector<float> means; ///< means
    std::vector<int> clustering; ///< clusters
    int iter; ///< iter
    int iterMax;  ///< iterMax
    std::vector<float> inputVector; ///< inputVector
    int classNumber; ///< classNumber
    std::vector<int> clusterCount; ///< clusterCount
    int numLayers; ///< numLayers

    /// \fn void Update1();
    /// \brief update for 1
    void Update1();
    /// \fn void UpdateN();
    /// \brief update for N
    void UpdateN();
};
