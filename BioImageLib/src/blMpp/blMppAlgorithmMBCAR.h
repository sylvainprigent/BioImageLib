/// \file blMppAlgorithmMBCAR.h
/// \brief blMppAlgorithmMBCAR class
/// \author Yellamraju Tarun (yellamraju.tarun@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once

#include "blMppAlgorithm.h"
#include "blMppContainerConfigShape2D.h"
#include "blMppShapeSuperquadric2D.h"
#include "blMppCluster.h"

/// \class blMppAlgorithmMBCAR
/// \brief Implement the multiple birth and cut algorithm with
/// attraction and repulsion as possible interaction. This implementation
/// does not cluster the attractive objects, thus confilcted objects are ignored
class blMppAlgorithmMBCAR : public blMppAlgorithm{

public:
    /// \fn blMppAlgorithmMBCAR();
    /// \brief Redefinition from blMppAlgorithm
    blMppAlgorithmMBCAR();
    /// \fn blMppAlgorithmMBCAR(blMppGeneratorShape* shapeGenerator, blMppDataTerm* dataTerm, blMppInteraction* interaction);
    /// \brief Redefinition from blMppAlgorithm
    blMppAlgorithmMBCAR(blMppGeneratorShape* shapeGenerator, blMppDataTerm* dataTerm, blMppInteraction* interaction);
    /// \fn virtual ~blMppAlgorithmMBCAR();
    /// \brief Redefinition from blMppAlgorithm
    virtual ~blMppAlgorithmMBCAR();

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

    /// \fn void initAttributs();
    /// \brief Initialize the attributs of the class
    void initAttributs();

public:
    // compute
    /// \fn void compute();
    /// \brief inherited from blMppAlgorithm
    void compute();
    /// \fn virtual std::vector<blMppShape*> GetShapes();
    /// \brief inherited from blMppAlgorithm
    virtual std::vector<blMppShape*> GetShapes();

    /// \fn blMppContainerConfigShapeGraph2D* configuration();
    /// \return the curent configuration
    blMppContainerConfigShapeGraph2D* configuration();

protected:
    // parameters
    int m_maxIter; ///< maximum number of iterations
    float m_PerturbationProba; ///< probability to generate a perturbation
    int m_nPerIter; ///< number of shape generated at each iteraction
    int m_integerCoeff; ///< precision coefficient used to transform float to int
    int m_iterationsVerboseRatio; ///< number of iterations between two process notification
    float m_ProbaPerutbation1; ///< inter probability of perturbation translate, rotation, resize
    float m_ProbaPerutbation2; ///< inter probability of perturbation translate, rotation, resize

protected:
    // internal data
    blMppContainerConfigShapeGraph2D* m_config; ///< curent configuration
    blMppContainerConfigShapeGraph2D* m_configPrim; ///< new proposed configuration
    int m_configSize; ///< Size of the curent configuration
    int m_countorConf; ///< count the number of object in a configuration
    bool m_freeConfig; ///< true is free config in destructor (not deleted when using parallel)

public:
    /// \fn void setConfig(blMppContainerConfigShapeGraph2D *config);
    /// \brief set config for parallel computing
    /// \param[in] config Configuration to set
    void setConfig(blMppContainerConfigShapeGraph2D *config);
    /// \fn void setConfigPrim(blMppContainerConfigShapeGraph2D *config);
    /// \brief set config for parallel computing
    /// \param[in] config Configuration prim to set
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

protected:
    double ID; ///< curent object Id for shape generation

private:
    /// \fn void generateConfig();
    /// \brief generate a new configuration of shapes
    void generateConfig();

private:
    /// \fn void initConfig();
    /// \brief initialize the configuration pointers
    void initConfig();
    /// \fn void testConfig(std::vector<blMppShape *> &shapes);
    /// \brief to test the generate config algorithm
    /// \param[in] shapes Shapes to be added in the configuration
    void testConfig(std::vector<blMppShape *> &shapes);

    /// \fn void saveConfig(std::string file);
    /// \param[in] file URL of the file where the configuration is saved
    void saveConfig(std::string file);
    /// \fn void plotPQ(std::string title);
    /// \brief cout in console configurations P and Q in the image.
    /// \param[in] title before the cout
    void plotPQ(std::string title);

};
