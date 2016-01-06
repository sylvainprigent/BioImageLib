#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#include <time.h>
#include <cstdlib>
#include <ctime>


#include "blPfModelsFactory.h"
#include "blParticleFiltering/blPfOptimizerSingleObject.h"

int main(int argc, char* argv[] ){

    // --------------------- Get inputs --------------------------//
    if (argc < 5){
        cout << "Usage: need 5 input parameters" << endl;
        cout << "\t 1- input images folder (2D or 3D images)" << endl;
        cout << "\t 2- image file extension (ex .tif)" << endl;
        cout << "\t 3- detections files folder (.txt files)" << endl;
        cout << "\t 4- parameter File (.txt)" << endl;
        cout << "\t 5- output image name" << endl;
        cout << "\t 6- mask images folder (2D or 3D images)" << endl;
        cout << "\t 7- begin image index (optional)" << endl;
        cout << "\t 8- end image index (optional)" << endl;
        return 1;
    }
    string inputFolder = argv[1];
    string imageExtension = argv[2];
    string detectionsFolder = argv[3];
    string paramFile = argv[4];
    string outputImageName = argv[5];

    // -------------------- Get the input frame list ---------------------------
    vector<string> inputImagesPaths;

    // get borders
    int beginData, endData;
    if (argc > 7){
        istringstream ss(argv[7]);
        if (!(ss >> beginData))
            cerr << "Invalid number " << argv[7] << '\n';
        istringstream ss2(argv[8]);
        if (!(ss2 >> endData))
            cerr << "Invalid number " << argv[8] << '\n';
    }
    else{
        beginData = -1;
        endData = -1;
    }

    // ---------------------------- read paths
    DIR *dp;
    struct dirent *dirp;
    if((dp = opendir(inputFolder.c_str())) == NULL) {
        cout << "Error(" << errno << ") cannot open the directory: " << inputFolder << endl;
        return errno;
    }

    while ( (dirp = readdir(dp)) ) {
        //cout << string(dirp->d_name) << endl;
        if( !(string(dirp->d_name) == ".") && !(string(dirp->d_name) == "..")){
            if(std::string::npos != string(dirp->d_name).find(imageExtension)){
                inputImagesPaths.push_back(dirp->d_name);
            }
        }
    }
    closedir(dp);
    std::sort( inputImagesPaths.begin(), inputImagesPaths.end() );

    // ------------------------------ Read the mask paths
    vector<string> inputMasksPaths;
    if (argc > 6){
        string maskFolder = argv[6];
        DIR *dpm;
        struct dirent *dirpm;
        if((dpm = opendir(maskFolder.c_str())) == NULL) {
            cout << "Error(" << errno << ") cannot open the directory: " << maskFolder << endl;
            return errno;
        }

        while ( (dirpm = readdir(dpm)) ) {
            //cout << string(dirp->d_name) << endl;
            if( !(string(dirpm->d_name) == ".") && !(string(dirpm->d_name) == "..")){
                if(std::string::npos != string(dirpm->d_name).find(imageExtension)){
                    inputMasksPaths.push_back(maskFolder + dirpm->d_name);
                }
            }
        }
        closedir(dpm);
        std::sort( inputMasksPaths.begin(), inputMasksPaths.end() );
    }
    // ------------------------------ Read the object locations ----------------
    vector<string> inputDetectionsFilesPaths;
    // read paths
    DIR *dpDetection;
    struct dirent *dirDetections;
    if((dpDetection = opendir(detectionsFolder.c_str())) == NULL) {
        cout << "Error(" << errno << ") cannot open the directory: " << detectionsFolder << endl;
        return errno;
    }
    while ( (dirDetections = readdir(dpDetection)) ) {
        if( !(string(dirDetections->d_name) == ".") && !(string(dirDetections->d_name) == "..")){
            if(std::string::npos != string(dirDetections->d_name).find(".txt")){
                inputDetectionsFilesPaths.push_back(dirDetections->d_name);
            }
        }
    }
    closedir(dpDetection);
    std::sort( inputDetectionsFilesPaths.begin(), inputDetectionsFilesPaths.end() );

    if (inputDetectionsFilesPaths.size() != inputImagesPaths.size()){
        cout << "Error: They must be the same number of detections files and frames files" << detectionsFolder << endl;
    }

    // Get the selected images
    vector<string> framesInter;
    vector<string> detectionsInter;

    if (beginData != -1 && endData != -1){
        if (int(inputImagesPaths.size()) > endData){
            for (int compt = beginData ; compt <= endData ; ++compt){
                framesInter.push_back(inputImagesPaths[compt]);
                detectionsInter.push_back(inputDetectionsFilesPaths[compt]);
            }
            inputImagesPaths.clear(); inputImagesPaths = framesInter;
            inputDetectionsFilesPaths.clear(); inputDetectionsFilesPaths = detectionsInter;
        }
        else{
            cerr << "Error : the frame range is above the number of images founded in the input folder: " << endl;
        }
    }

    /*
    cout << "input images = " << endl;
    for (int i = 0 ; i < inputImagesPaths.size() ; ++i){
        cout << inputImagesPaths.at(i) << endl;
    }
    cout << "input objects = " << endl;
    for (int i = 0 ; i < inputDetectionsFilesPaths.size() ; ++i){
        cout << inputDetectionsFilesPaths.at(i) << endl;
    }
    */

    // load all the objects files
    vector<vector<vector<float> > > objects; objects.resize(inputDetectionsFilesPaths.size());
    for (unsigned int i = 0 ; i < inputDetectionsFilesPaths.size() ; ++i){
        objects[i] = blTxtIO::LoadShapeFromTxtFile(detectionsFolder + inputDetectionsFilesPaths.at(i));
        //cout << "frame " << i << endl;
        //for (int m = 0 ; m < objects[i].size() ; ++m){
        //    cout << "\t" <<objects[i][m][0] << ", " << objects[i][m][1] << ", " << objects[i][m][2] << endl;
        //}
    }

    // ------------------------------ Instantiate the tracker ------------------
    cout << endl;
    clock_t start = clock();
    try{
        // parameters
        blProcessParameters *paramManager = new blProcessParameters(paramFile);
        paramManager->load();

        // data manager
        blPfDataManager* dataManager = new blPfDataManager(objects, inputImagesPaths, inputFolder);
        if (inputMasksPaths.size() >0){
            dataManager->setMasks(inputMasksPaths);
        }

        // model
        blPfModelsFactory modelFactory(paramManager);
        std::string modelName = paramManager->getValueOfKey<std::string>("blPfModelName", "none");
        blPfModelInterface* pfmodel = modelFactory.model(dataManager, modelName);
        pfmodel->init();

        // optimizer
        blPfOptimizerMultipleObjectMultipleStart* pfFilter = new blPfOptimizerMultipleObjectMultipleStart(pfmodel, 0);
        blProcessObserverCommandeLine* observer = new blProcessObserverCommandeLine;
        pfFilter->addObserver(observer);
        pfFilter->setParticleNumber(paramManager->getValueOfKey<int>("blPfparticleNumber", 200));
        pfFilter->setResamplingThreshold(paramManager->getValueOfKey<float>("blPfdegeneracyTh", 10000.0));
        pfFilter->setResamplingMethod(paramManager->getValueOfKey<string>("blPfResampling", "ResampleResidual"));
        float alreadyUsedNeighborThreshold = paramManager->getValueOfKey<float>("blPfalreadyUsedNeighborThreshold", 2);
        pfFilter->setalreadyUsedNeighborThreshold(alreadyUsedNeighborThreshold*alreadyUsedNeighborThreshold);
        pfFilter->run();

        // Save the result
        cout << "Save the result = " << endl;
        pfFilter->saveTrackRepresentation(outputImageName + ".tif");
        FloatColor2DImage::Pointer im = pfFilter->getTracksRepresentationOnLastImage();
        blImageIO::saveFloatColor2DImage(outputImageName + "_projection.tif", im);
        pfFilter->saveTracksToTxtFile(outputImageName + ".txt");

        delete paramManager;
        delete pfmodel;
        delete pfFilter;
        delete observer;
    }
    catch (blException ex){
        cout << ex.what() << endl;
    }
    printf("Time elapsed: %f\n", ((double)clock() - start) / CLOCKS_PER_SEC);
}



