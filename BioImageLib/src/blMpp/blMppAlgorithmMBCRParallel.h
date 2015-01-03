/// \file blMppAlgorithmMBCRParallel.h
/// \brief blMppAlgorithmMBCRParallel class
/// \author Sylvain Prigent (Sylvain.Prigent@inria.fr)
/// \version 0.1
/// \date 2014

#pragma once

#include "blMppAlgorithm.h"
#include "blMppContainerConfigShape2D.h"

/// \class blMppAlgorithmMBCRParallel
/// \brief Implement the multiple birth and cut algorithm with
/// binary (ie infinity) repulsion as possible interaction
/// This implementation call blMppAlgorithmMBCR to make it parallel
class blMppAlgorithmMBCRParallel : public blMppAlgorithm{

public:
    /// \fn blMppAlgorithmMBCRParallel();
    /// \brief Redefinition from blMppAlgorithm
    blMppAlgorithmMBCRParallel();
    /// \fn blMppAlgorithmMBCRParallel(blMppGeneratorShape* shapeGenerator, blMppDataTerm* dataTerm, blMppInteraction* interaction);
    /// \brief Redefinition from blMppAlgorithm
    blMppAlgorithmMBCRParallel(blMppGeneratorShape* shapeGenerator, blMppDataTerm* dataTerm, blMppInteraction* interaction);
    /// \fn virtual ~blMppAlgorithmMBCRParallel();
    /// \brief Redefinition from blMppAlgorithm
    virtual ~blMppAlgorithmMBCRParallel();

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
    /// \fn void setNumberOfThread(int threadNumber);
    /// \param[in] threadNumber Number of threads
    void setNumberOfThread(int threadNumber);

public:
    // compute
    /// \fn void compute();
    /// \brief inherited from blMppAlgorithm
    void compute();
    /// \fn virtual std::vector<blMppShape*> GetShapes();
    /// \brief inherited from blMppAlgorithm
    virtual std::vector<blMppShape*> GetShapes();

private:
    /// \fn void initAttributs();
    ///  \brief Initialize the class attributs
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
    int m_threadNumber; ///< number of threads for parallel computing

protected:
    // internal data
    blMppContainerConfigShape2D* m_config; ///< curent configuration
    std::vector<blMppContainerConfigShape2D*> m_configurations; ///< configurations in competition

protected:
    /// \fn void do_round();
    /// \brief make a competition between two configuration
    void do_round();
    /// \fn void final_round();
    /// \brief make the final competition between the two last configurations
    void final_round();

protected:
    // internal functions
    /// \fn void initFunctionsPointers();
    /// \brief Initialize the generation function pointer due
    /// to the parameters
    void initFunctionsPointers();

};
