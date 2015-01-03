#include "blMppFactories.h"

blMppFactory::blMppFactory(blProcessParameters* parameters){
    m_parameters = parameters;
    m_observer = new blProcessObserverCommandeLine();
}

blMppFactory::blMppFactory(std::string parameterFile){
    m_parameters = new blProcessParameters(parameterFile);
    m_parameters->load();
    m_observer = new blProcessObserverCommandeLine();
}

blMppFactory::~blMppFactory(){
    delete m_observer;
}

// getters
blMppGeneratorShape* blMppFactory::generator(std::string name){

    if (name == "blMppGeneratorShapeSuperquadric2D"){
        blMppGeneratorShapeSuperquadric2D* generator = new blMppGeneratorShapeSuperquadric2D();
        generator->setMajorAxisRange(m_parameters->getValueOfKey<int>("blMppGeneratorShapeSuperquadric2D_minMajorAxis",20),
                                     m_parameters->getValueOfKey<int>("blMppGeneratorShapeSuperquadric2D_maxMajorAxis",30));
        generator->setMinorAxisRange(m_parameters->getValueOfKey<int>("blMppGeneratorShapeSuperquadric2D_minMinorAxis",3),
                                     m_parameters->getValueOfKey<int>("blMppGeneratorShapeSuperquadric2D_maxMinorAxis",4));

        generator->setMajorExposantRange(m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_minExposantX",2),
                                         m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_maxExposantX",2),
                                         m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_exposantXStep",1));

        generator->setMinorExposantRange(m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_minExposantY",2),
                                         m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_maxExposantY",2),
                                         m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_exposantYStep",1));

        generator->setThetaRange(m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_minTheta",0),
                                 m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_maxTheta",3.14),
                                 m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_thetaStep",0.2));

        generator->setPerturbationsProba(m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_translationProba",0.33),
                                         m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_translationResize",0.33),
                                         m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_translationRotate",0.34));
        generator->setDeltaResize(m_parameters->getValueOfKey<int>("blMppGeneratorShapeSuperquadric2D_deltaResize",1));
        generator->setDeltaTranslation(m_parameters->getValueOfKey<int>("blMppGeneratorShapeSuperquadric2D_deltaTranslate",1));
        generator->setDeltaRotation(m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_deltaRotate",0.2));
        return generator;
    }
    if (name == "blMppGeneratorShapeSuperquadric3D"){
        blMppGeneratorShapeSuperquadric3D* generator = new blMppGeneratorShapeSuperquadric3D();
        generator->setAxisRange_a(m_parameters->getValueOfKey<int>("blMppGeneratorShapeSuperquadric3D_mina",20),
                                     m_parameters->getValueOfKey<int>("blMppGeneratorShapeSuperquadric3D_maxa",30));
        generator->setAxisRange_b(m_parameters->getValueOfKey<int>("blMppGeneratorShapeSuperquadric3D_minb",3),
                                     m_parameters->getValueOfKey<int>("blMppGeneratorShapeSuperquadric3D_maxb",4));
        generator->setAxisRange_c(m_parameters->getValueOfKey<int>("blMppGeneratorShapeSuperquadric3D_minc",3),
                                     m_parameters->getValueOfKey<int>("blMppGeneratorShapeSuperquadric3D_maxc",4));

        generator->setExposantRange_m(m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric3D_minExposantm",2),
                                         m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric3D_maxExposantm",2),
                                         m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric3D_exposantmStep",1));
        generator->setExposantRange_n(m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric3D_minExposantn",2),
                                         m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric3D_maxExposantn",2),
                                         m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric3D_exposantnStep",1));
        generator->setExposantRange_p(m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric3D_minExposantp",2),
                                         m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric3D_maxExposantp",2),
                                         m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric3D_exposantpStep",1));


        generator->setPhiRange(m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric3D_minPhi",0),
                                 m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric3D_maxPhi",3.14),
                                 m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric3D_phiStep",0.2));
        generator->setThetaRange(m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric3D_minTheta",0),
                                 m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric3D_maxTheta",3.14),
                                 m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric3D_thetaStep",0.2));
        generator->setGammaRange(m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric3D_minGamma",0),
                                 m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric3D_maxGamma",3.14),
                                 m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric3D_gammaStep",0.2));

        generator->setPerturbationsProba(m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric3D_translationProba",0.33),
                                         m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric3D_translationResize",0.33),
                                         m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric3D_translationRotate",0.34));
        generator->setDeltaResize(m_parameters->getValueOfKey<int>("blMppGeneratorShapeSuperquadric3D_deltaResize",1));
        generator->setDeltaTranslation(m_parameters->getValueOfKey<int>("blMppGeneratorShapeSuperquadric3D_deltaTranslate",1));
        generator->setDeltaRotation(m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric3D_deltaRotate",0.2));
        return generator;
    }
    if (name == "blMppGeneratorShapeEllipse"){
        blMppGeneratorShapeEllipse* generator = new blMppGeneratorShapeEllipse();
        generator->setMajorAxisRange(m_parameters->getValueOfKey<int>("blMppGeneratorShapeEllipse_minMajorAxis",20),
                                     m_parameters->getValueOfKey<int>("blMppGeneratorShapeEllipse_maxMajorAxis",30));
        generator->setMinorAxisRange(m_parameters->getValueOfKey<int>("blMppGeneratorShapeEllipse_minMinorAxis",3),
                                     m_parameters->getValueOfKey<int>("blMppGeneratorShapeEllipse_maxMinorAxis",4));

        generator->setThetaRange(m_parameters->getValueOfKey<float>("blMppGeneratorShapeEllipse_minTheta",0),
                                 m_parameters->getValueOfKey<float>("blMppGeneratorShapeEllipse_maxTheta",3.14),
                                 m_parameters->getValueOfKey<float>("blMppGeneratorShapeEllipse_thetaStep",0.2));

        generator->setPerturbationsProba(m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_translationProba",0.33),
                                         m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_translationResize",0.33),
                                         m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_translationRotate",0.34));
        generator->setDeltaResize(m_parameters->getValueOfKey<int>("blMppGeneratorShapeSuperquadric2D_deltaResize",1));
        generator->setDeltaTranslation(m_parameters->getValueOfKey<int>("blMppGeneratorShapeSuperquadric2D_deltaTranslate",1));
        generator->setDeltaRotation(m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_deltaRotate",0.2));
        return generator;
    }
    if (name == "blMppGeneratorShapeRectangle"){
        blMppGeneratorShapeRectangle* generator = new blMppGeneratorShapeRectangle();
        generator->setMajorAxisRange(m_parameters->getValueOfKey<int>("blMppGeneratorShapeRectangle_minMajorAxis",20),
                                     m_parameters->getValueOfKey<int>("blMppGeneratorShapeRectangle_maxMajorAxis",30));
        generator->setMinorAxisRange(m_parameters->getValueOfKey<int>("blMppGeneratorShapeRectangle_minMinorAxis",3),
                                     m_parameters->getValueOfKey<int>("blMppGeneratorShapeRectangle_maxMinorAxis",4));

        generator->setThetaRange(m_parameters->getValueOfKey<float>("blMppGeneratorShapeRectangle_minTheta",0),
                                 m_parameters->getValueOfKey<float>("blMppGeneratorShapeRectangle_maxTheta",3.14),
                                 m_parameters->getValueOfKey<float>("blMppGeneratorShapeRectangle_thetaStep",0.2));

        generator->setPerturbationsProba(m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_translationProba",0.33),
                                         m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_translationResize",0.33),
                                         m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_translationRotate",0.34));
        generator->setDeltaResize(m_parameters->getValueOfKey<int>("blMppGeneratorShapeSuperquadric2D_deltaResize",1));
        generator->setDeltaTranslation(m_parameters->getValueOfKey<int>("blMppGeneratorShapeSuperquadric2D_deltaTranslate",1));
        generator->setDeltaRotation(m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_deltaRotate",0.2));
        return generator;
    }
    if (name == "blMppGeneratorShapeCircle"){
        blMppGeneratorShapeCircle* generator = new blMppGeneratorShapeCircle();
        generator->setRadiusRange(m_parameters->getValueOfKey<int>("blMppGeneratorShapeCircle_minRadius",3),
                                  m_parameters->getValueOfKey<int>("blMppGeneratorShapeCircle_maxRadius",5));

        generator->setPerturbationsProba(m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_translationProba",0.33),
                                         m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_translationResize",0.33),
                                         m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_translationRotate",0.34));
        generator->setDeltaResize(m_parameters->getValueOfKey<int>("blMppGeneratorShapeSuperquadric2D_deltaResize",1));
        generator->setDeltaTranslation(m_parameters->getValueOfKey<int>("blMppGeneratorShapeSuperquadric2D_deltaTranslate",1));
        generator->setDeltaRotation(m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_deltaRotate",0.2));
        return generator;
    }
    if (name == "blMppGeneratorShapeCircleIn3D"){
        blMppGeneratorShapeCircleIn3D* generator = new blMppGeneratorShapeCircleIn3D();
        generator->setRadiusRange(m_parameters->getValueOfKey<int>("blMppGeneratorShapeCircle_minRadius",3),
                                  m_parameters->getValueOfKey<int>("blMppGeneratorShapeCircle_maxRadius",5));

        generator->setPerturbationsProba(m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_translationProba",0.33),
                                         m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_translationResize",0.33),
                                         m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_translationRotate",0.34));
        generator->setDeltaResize(m_parameters->getValueOfKey<int>("blMppGeneratorShapeSuperquadric2D_deltaResize",1));
        generator->setDeltaTranslation(m_parameters->getValueOfKey<int>("blMppGeneratorShapeSuperquadric2D_deltaTranslate",1));
        generator->setDeltaRotation(m_parameters->getValueOfKey<float>("blMppGeneratorShapeSuperquadric2D_deltaRotate",0.2));
        return generator;
    }
    if (name == "blMppGeneratorShapeSphere"){
        blMppGeneratorShapeSphere* generator = new blMppGeneratorShapeSphere();
        generator->setRadiusRange(m_parameters->getValueOfKey<int>("blMppGeneratorShapeSphere_minRadius",3),
                                  m_parameters->getValueOfKey<int>("blMppGeneratorShapeSphere_maxRadius",5));

        generator->setPerturbationsProba(m_parameters->getValueOfKey<float>("blMppGeneratorShapeSphere_translationProba",0.33),
                                         m_parameters->getValueOfKey<float>("blMppGeneratorShapeSphere_translationResize",0.33),
                                         m_parameters->getValueOfKey<float>("blMppGeneratorShapeSphere_translationRotate",0.34));
        generator->setDeltaResize(m_parameters->getValueOfKey<int>("blMppGeneratorShapeSphere_deltaResize",1));
        generator->setDeltaTranslation(m_parameters->getValueOfKey<int>("blMppGeneratorShapeSphere_deltaTranslate",1));
        generator->setDeltaRotation(m_parameters->getValueOfKey<float>("blMppGeneratorShapeSphere_deltaRotate",0.2));
        return generator;
    }
    if ( name == "blMppGeneratorShapeSilhouetteTemplate2D"){
        blMppGeneratorShapeSilhouetteTemplate2D* generator = new blMppGeneratorShapeSilhouetteTemplate2D();
        generator->setSilhouetteLibraryFile(m_parameters->getValueOfKey<std::string>("blMppGeneratorShapeSilhouetteTemplate2D_libraryFile",""));
        return generator;
    }
    if (name == "blMppGeneratorShapeSilhouette2D"){
        blMppGeneratorShapeSilhouette2D* generator = new blMppGeneratorShapeSilhouette2D();
        generator->setLibraryDirectory(m_parameters->getValueOfKey<std::string>("blMppGeneratorShapeSilhouette2D_libraryDir",""));
        generator->setNumberOfFile(m_parameters->getValueOfKey<int>("blMppGeneratorShapeSilhouette2D_numberOfFiles",0));
        return generator;
    }
    if ( name == "blMppGeneratorShapeTestClusters"){
        blMppGeneratorShapeTestClusters* generator = new blMppGeneratorShapeTestClusters();
        generator->setShapeFile(m_parameters->getValueOfKey<std::string>("blMppGeneratorShapeTestClusters_libraryFile",""));
        return generator;
    }
    if ( name == "NULL"){
        return NULL;
    }
    throw blException(("Cannot find the generator " + name).c_str());
}

blMppDataTerm* blMppFactory::dataTerm(std::string name){

    if (name == "blMppDataTermBhattacharyya"){
        blMppDataTermBhattacharyya *dataTerm = new blMppDataTermBhattacharyya();
        dataTerm->setD(m_parameters->getValueOfKey<float>("blMppDataTermBhattacharyya_D",100));
        dataTerm->setGradientDirection(m_parameters->getValueOfKey<int>("blMppDataTermBhattacharyya_gradientDirection",1));
        dataTerm->setThreshold(m_parameters->getValueOfKey<float>("blMppDataTerm_threshold",3));
        dataTerm->setBorderWidth(m_parameters->getValueOfKey<int>("blMppDataTerm_borderWidth",1));
        return dataTerm;
    }
    if (name == "blMppDataTermGradientNormalized"){
        blMppDataTermGradientNormalized *dataTerm = new blMppDataTermGradientNormalized();
        dataTerm->setEpsilon(m_parameters->getValueOfKey<float>("blMppDataTermGradientNormalized_epsilon",0.000001));
        dataTerm->setGradientDirection(m_parameters->getValueOfKey<int>("blMppDataTermGradientNormalized_gradientDirection",1));
        dataTerm->setSigma(m_parameters->getValueOfKey<float>("blMppDataTermGradientNormalized_sigma",1.5));
        dataTerm->setThreshold(m_parameters->getValueOfKey<float>("blMppDataTerm_threshold",3));
        dataTerm->setBorderWidth(m_parameters->getValueOfKey<int>("blMppDataTerm_borderWidth",1));
        return dataTerm;
    }
    if (name == "blMppDataTermGradient"){
        blMppDataTermGradient *dataTerm = new blMppDataTermGradient();
        dataTerm->setD(m_parameters->getValueOfKey<float>("blMppDataTermGradient_D",100));
        dataTerm->setGradientDirection(m_parameters->getValueOfKey<int>("blMppDataTermGradient_gradientDirection",1));
        dataTerm->setSigma(m_parameters->getValueOfKey<float>("blMppDataTermGradient_sigma",1.5));
        dataTerm->setThreshold(m_parameters->getValueOfKey<float>("blMppDataTerm_threshold",3));
        dataTerm->setBorderWidth(m_parameters->getValueOfKey<int>("blMppDataTerm_borderWidth",1));
        return dataTerm;
    }
    if (name == "blMppDataTermMeanIntensity"){
        blMppDataTermMeanIntensity *dataTerm = new blMppDataTermMeanIntensity;
        dataTerm->setD(m_parameters->getValueOfKey<int>("blMppDataTermMeanIntensity_D",100));
        dataTerm->setThreshold(m_parameters->getValueOfKey<float>("blMppDataTerm_threshold",200));
        return dataTerm;
    }
    if (name == "blMppDataTermBhattacharyyaCircleConstraint"){
        blMppDataTermBhattacharyyaCircleConstraint *dataTerm = new blMppDataTermBhattacharyyaCircleConstraint;
        dataTerm->setD(m_parameters->getValueOfKey<float>("blMppDataTermBhattacharyyaCircleConstraint_D",100));
        dataTerm->setGradientDirection(m_parameters->getValueOfKey<int>("blMppDataTermBhattacharyyaCircleConstraint_gradientDirection",1));
        dataTerm->setMedianRadius(m_parameters->getValueOfKey<int>("blMppDataTermBhattacharyyaCircleConstraint_medianRadius",1));
        dataTerm->setLambda(m_parameters->getValueOfKey<float>("blMppDataTermBhattacharyyaCircleConstraint_lambda",0.5));
        dataTerm->setThreshold(m_parameters->getValueOfKey<float>("blMppDataTerm_threshold",3));
        dataTerm->setBorderWidth(m_parameters->getValueOfKey<int>("blMppDataTerm_borderWidth",1));

        return dataTerm;

    }
    if (name == "blMppDataTermGradientCompt"){
        blMppDataTermGradientCompt *dataTerm = new blMppDataTermGradientCompt();
        dataTerm->setCompt(m_parameters->getValueOfKey<float>("blMppDataTermGradientCompt_compt",0.0));
        dataTerm->setGradientDirection(m_parameters->getValueOfKey<int>("blMppDataTermGradientCompt_gradientDirection",1));
        dataTerm->setSigma(m_parameters->getValueOfKey<float>("blMppDataTermGradientCompt_sigma",1.5));
        dataTerm->setThreshold(m_parameters->getValueOfKey<float>("blMppDataTerm_threshold",3));
        dataTerm->setBorderWidth(m_parameters->getValueOfKey<int>("blMppDataTerm_borderWidth",1));
        return dataTerm;
    }
    if (name == "blMppDataTermPartBhatt"){
        blMppDataTermPartBhatt *dataTerm = new blMppDataTermPartBhatt();
        dataTerm->setD(m_parameters->getValueOfKey<float>("blMppDataTermPartBhatt_D",100));
        dataTerm->setThreshold(m_parameters->getValueOfKey<float>("blMppDataTerm_threshold",3));
        dataTerm->setBorderWidth(m_parameters->getValueOfKey<int>("blMppDataTerm_borderWidth",1));
        return dataTerm;
    }
    throw blException(("Cannot find the data term " + name).c_str());
}

blMppInteraction* blMppFactory::interaction(std::string name){

    if (name == "blMppInteractionNoOverlap"){
        blMppInteractionNoOverlap* interaction = new blMppInteractionNoOverlap();
        interaction->setOverlapCoef(m_parameters->getValueOfKey<float>("blMppInteractionNoOverlap_overlapCoef",0.0));
        interaction->setMinZDistance(m_parameters->getValueOfKey<int>("blMppInteractionNoOverlap_minZDistance",1));

        return interaction;
    }
    if (name == "blMppInteractionAlignedSQ2D"){
        blMppInteractionAlignedSQ2D* interaction = new blMppInteractionAlignedSQ2D();
        interaction->setOverlapCoef(m_parameters->getValueOfKey<float>("blMppInteractionNoOverlap_overlapCoef",0.0));
        return interaction;
    }
    if (name == "blMppInteractionAttraction"){
        blMppInteractionAttraction* interaction = new blMppInteractionAttraction();
        interaction->setOverlapCoef(m_parameters->getValueOfKey<float>("blMppInteractionNoOverlap_overlapCoef",0.0));
        return interaction;
    }
    if (name == "blMppInteractionCircle"){
        blMppInteractionCircle* interaction = new blMppInteractionCircle();
        interaction->setOverlapCoef(m_parameters->getValueOfKey<float>("blMppInteractionNoOverlap_overlapCoef",0.0));
        return interaction;
    }
    throw blException(("Cannot find the interaction " + name).c_str());
}

blMppAlgorithm* blMppFactory::algorithm(blMppGeneratorShape* generator, blMppDataTerm* dataTerm, blMppInteraction*interaction, std::string name){

    if (name == "blMppAlgorithmMBCR"){
        int threadNumber = m_parameters->getValueOfKey<int>("blMppAlgorithmMBCR_threadNumber",0);
        if (threadNumber < 2){
            blMppAlgorithmMBCR *algorithm = new blMppAlgorithmMBCR(generator, dataTerm, interaction);
            algorithm->setMaxIterations(m_parameters->getValueOfKey<int>("blMppAlgorithmMBCR_maxIterations",10000));
            algorithm->setPerturbationProba(m_parameters->getValueOfKey<float>("blMppAlgorithmMBCR_perturbationProba",0.0));
            algorithm->setNewShapesPerIterations(m_parameters->getValueOfKey<int>("blMppAlgorithmMBCR_newShapesPerIterations",50));
            algorithm->setIntegerCoef(m_parameters->getValueOfKey<int>("blMppAlgorithmMBCR_integerCoef",1000));
            algorithm->setIterationsVerboseRatio(m_parameters->getValueOfKey<int>("blMppAlgorithmMBCR_iterationsVerboseRatio",100));
            return algorithm;
        }
        else{
            std::cout << "use blMppAlgorithmMBCRParallel" << std::endl;
            blMppAlgorithmMBCRParallel *algorithm = new blMppAlgorithmMBCRParallel(generator, dataTerm, interaction);
            algorithm->setMaxIterations(m_parameters->getValueOfKey<int>("blMppAlgorithmMBCR_maxIterations",10000));
            algorithm->setPerturbationProba(m_parameters->getValueOfKey<float>("blMppAlgorithmMBCR_perturbationProba",0.0));
            algorithm->setNewShapesPerIterations(m_parameters->getValueOfKey<int>("blMppAlgorithmMBCR_newShapesPerIterations",50));
            algorithm->setIntegerCoef(m_parameters->getValueOfKey<int>("blMppAlgorithmMBCR_integerCoef",1000));
            algorithm->setIterationsVerboseRatio(m_parameters->getValueOfKey<int>("blMppAlgorithmMBCR_iterationsVerboseRatio",100));
            algorithm->setNumberOfThread(threadNumber);
            return algorithm;
        }
    }
    if (name == "blMppAlgorithmMBCAR"){
        blMppAlgorithmMBCAR *algorithm = new blMppAlgorithmMBCAR(generator, dataTerm, interaction);
        algorithm->setMaxIterations(m_parameters->getValueOfKey<int>("blMppAlgorithmMBCR_maxIterations",10000));
        algorithm->setPerturbationProba(m_parameters->getValueOfKey<float>("blMppAlgorithmMBCR_perturbationProba",0.0));
        algorithm->setNewShapesPerIterations(m_parameters->getValueOfKey<int>("blMppAlgorithmMBCR_newShapesPerIterations",50));
        algorithm->setIntegerCoef(m_parameters->getValueOfKey<int>("blMppAlgorithmMBCR_integerCoef",1000));
        algorithm->setIterationsVerboseRatio(m_parameters->getValueOfKey<int>("blMppAlgorithmMBCR_iterationsVerboseRatio",100));
        return algorithm;
    }
    if (name == "blMppAlgorithmMBCARCluster"){
        blMppAlgorithmMBCARCluster *algorithm = new blMppAlgorithmMBCARCluster(generator, dataTerm, interaction);
        algorithm->setMaxIterations(m_parameters->getValueOfKey<int>("blMppAlgorithmMBCR_maxIterations",10000));
        algorithm->setPerturbationProba(m_parameters->getValueOfKey<float>("blMppAlgorithmMBCR_perturbationProba",0.0));
        algorithm->setNewShapesPerIterations(m_parameters->getValueOfKey<int>("blMppAlgorithmMBCR_newShapesPerIterations",50));
        algorithm->setIntegerCoef(m_parameters->getValueOfKey<int>("blMppAlgorithmMBCR_integerCoef",1000));
        algorithm->setIterationsVerboseRatio(m_parameters->getValueOfKey<int>("blMppAlgorithmMBCR_iterationsVerboseRatio",100));
        return algorithm;
    }
    throw blException(("Cannot find the algorithm " + name).c_str());
}
