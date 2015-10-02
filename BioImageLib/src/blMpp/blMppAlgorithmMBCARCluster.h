/// \file blMppAlgorithmMBCARCluster.h
/// \brief blMppAlgorithmMBCARCluster class
/// \author Yellamraju Tarun (yellamraju.tarun@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once

#include "blMppAlgorithm.h"
#include "blMppContainerConfigShape2D.h"
#include "blMppShapeSuperquadric2D.h"
#include "blMppCluster.h"
#include "blMppExport.h"

/// \class blMppAlgorithmMBCARCluster
/// \brief Implement the multiple birth and cut algorithm with
/// attraction and repulsion as possible interaction. This implementation
/// merge in clusters objects that intersect after a cut
class BLMPP_EXPORT blMppAlgorithmMBCARCluster : public blMppAlgorithm{

public:
    /// \fn blMppAlgorithmMBCARCluster();
    /// \brief Redefinition from blMppAlgorithm
    blMppAlgorithmMBCARCluster();
    /// \fn blMppAlgorithmMBCARCluster(blMppGeneratorShape* shapeGenerator, blMppDataTerm* dataTerm, blMppInteraction* interaction);
    /// \brief Redefinition from blMppAlgorithm
    blMppAlgorithmMBCARCluster(blMppGeneratorShape* shapeGenerator, blMppDataTerm* dataTerm, blMppInteraction* interaction);
    /// \fn virtual ~blMppAlgorithmMBCARCluster();
    /// \brief Redefinition from blMppAlgorithm
    virtual ~blMppAlgorithmMBCARCluster();

public:
    // blProcess methods
    /// \fn virtual std::string name();
    /// \brief Inirits from blMppAlgorithm
    virtual std::string name();

public:
    // parameters setters
    /// \fn void setMaxIterations(int value);
    /// \param[in] value Maximun number of iterations the algorithm
    /// does. (default = 10000)
    void setMaxIterations(int value);
    /// \fn void setPerturbationProba(float value);
    /// \param[in] value Probability [0,1] of generating
    /// either a random configuration or a configaration
    /// from perturbations of the curent configuration
    /// (default = 0, ie no perturbations)
    void setPerturbationProba(float value);
    /// \fn void setNewShapesPerIterations(int value);
    /// \param[in] value Number of shapes generated
    /// for each new random configuration (default = 200)
    void setNewShapesPerIterations(int value);
    /// \fn void setIntegerCoef(int value);
    /// \param[in] value Multiplicative coefficient
    /// used to cast the data term and the interaction
    /// values from float to int
    void setIntegerCoef(int value);
    /// \fn void setIterationsVerboseRatio(int value);
    /// \param[in] value Number of iteration when the
    /// algorithm notify it progress
    void setIterationsVerboseRatio(int value);

    /// \fn void freeConfig(bool value);
    /// \param[in] value set to false if the config must not been deleted
    void freeConfig(bool value);

public:
    // compute
    /// \fn void compute();
    /// \brief inherited from blMppAlgorithm
    void compute();
    /// \fn virtual std::vector<blMppShape*> GetShapes();
    /// \brief inherited from blMppAlgorithm
    virtual std::vector<blMppShape*> GetShapes();

    /// \fn blMppContainerConfigShape2D* configuration();
    /// \return the curent configuration
    blMppContainerConfigShapeGraph2D* configuration();

private:
    /// \fn void initAttributs();
    /// \brief Initialize the attributs of the class
    void initAttributs();

protected:
    // parameters
    int m_maxIter; ///<
    float m_PerturbationProba; ///<
    int m_nPerIter; ///<
    int m_integerCoeff; ///<
    int m_iterationsVerboseRatio; ///<
    float m_ProbaPerutbation1; ///<
    float m_ProbaPerutbation2; ///<

protected:
    // internal data
    blMppContainerConfigShapeGraph2D* m_config; ///< curent configuration
    blMppContainerConfigShapeGraph2D* m_configPrim; ///< new proposed configuration
    int m_configSize; ///< Size of the curent configuration
    int m_countorConf; ///<
    bool m_freeConfig;
    std::vector<blMppCluster *> Config_P;
    std::vector<blMppCluster *> Config_Q;

public:
    void setConfig(blMppContainerConfigShapeGraph2D *config);
    void setConfigPrim(blMppContainerConfigShapeGraph2D *config);

public:
    // internal functions
    /// \fn void initFunctionsPointers();
    /// \brief Initialize the generation function pointer due
    /// to the parameters
    void initFunctionsPointers();
    /// \fn void cut(int n);
    /// \brief Select the best object from the two configurations
    /// m_config and m_configPrim
    int cut(int n);

    double ID;

public:
    void generateConfig();

public:
    void initConfig();
    void testConfig(std::vector<blMppShape *> &shapes);

    void saveConfig(std::string file);
    void plotPQ(std::string title);

    blMppCluster* find_cluster (double id);
    void delete_cluster (blMppCluster *cluster);
    bool merge(blMppCluster * cluster1, blMppCluster * cluster2);
    void printlist (std::vector<blMppCluster*> list);
    void printStatus(std::vector<int> list);
    void move ();
};
