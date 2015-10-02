/// \file blMppAlgorithmMBCR.h
/// \brief blMppAlgorithmMBCR class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once

#include "blMppAlgorithm.h"
#include "blMppContainerConfigShape2D.h"
#include "blMppExport.h"

/// \class blMppAlgorithmMBCR
/// \brief Implement the multiple birth and cut algorithm with
/// binary (ie infinity) repulsion as possible interaction
/// This implementation is based on the publication
/// "A 3D segmentation algorithm for ellipsoidal shapes. Application to nuclei extraction."
/// Soubi{\`e}s, Emmanuel and Weiss, Pierre and Descombes, Xavier
/// ICPRAM - International Conference on Pattern Recognition Applications and Methods, Barcelona, 2013
/// The graph used in this implementation is a simplification of the graph
/// decribed in the publication above
class BLMPP_EXPORT blMppAlgorithmMBCR : public blMppAlgorithm{

public:
    /// \fn blMppAlgorithmMBCR();
    /// \brief Redefinition from blMppAlgorithm
    blMppAlgorithmMBCR();
    /// \fn blMppAlgorithmMBCR(blMppGeneratorShape* shapeGenerator, blMppDataTerm* dataTerm, blMppInteraction* interaction);
    /// \brief Redefinition from blMppAlgorithm
    blMppAlgorithmMBCR(blMppGeneratorShape* shapeGenerator, blMppDataTerm* dataTerm, blMppInteraction* interaction);
    /// \fn virtual ~blMppAlgorithmMBCR();
    /// \brief Redefinition from blMppAlgorithm
    virtual ~blMppAlgorithmMBCR();

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
    blMppContainerConfigShape2D* configuration();

public:
    /// \fn void setConfig(blMppContainerConfigShapeGraph2D *config);
    /// \brief set config for parallel computing
    /// \param[in] config Configuration to set
    void setConfig(blMppContainerConfigShape2D *config);
    /// \fn void setConfigPrim(blMppContainerConfigShapeGraph2D *config);
    /// \brief set config for parallel computing
    /// \param[in] config Configuration prim to set
    void setConfigPrim(blMppContainerConfigShape2D *config);

public:
    // internal functions
    /// \fn void initFunctionsPointers();
    /// \brief Initialize the generation function pointer due
    /// to the parameters
    void initFunctionsPointers();
    /// \fn void cut(int n);
    /// \brief Select the best object from the two configurations
    /// m_config and m_configPrim
    void cut(int n);

protected:
    typedef void (blMppAlgorithmMBCR::*generateNewConfiguration)(blMppContainerConfigShape2D *config);
    generateNewConfiguration generateNewConfigurationPtr;
    /// \fn void generateNewRandConfiguration(blMppContainerConfigShape2D *config);
    /// \brief Generate a new random configuration
    /// \param[out] config Pointer to the generated configuration
    void generateNewRandConfiguration(blMppContainerConfigShape2D *config);
    /// \fn void generateNewPerturbationConfiguration(blMppContainerConfigShape2D *config);
    /// \brief Generate a new configuration doing perturbations on
    /// the shapes in the curent configuration m_config
    /// \param[out] config Pointer to the generated configuration
    void generateNewPerturbationConfiguration(blMppContainerConfigShape2D *config);

private:
    /// \fn void initAttributs();
    /// \brief initialize all the class attributs (method called in the constructor)
    void initAttributs();

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
    blMppContainerConfigShape2D* m_config; ///< curent configuration
    blMppContainerConfigShape2D* m_configPrim; ///< new proposed configuration
    int m_configSize; ///< Size of the curent configuration
    int m_countorConf; ///< contor of the shapes in the configuration
    bool m_freeConfig; ///< true if the destructor free the configuration

};
