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

#include "blRATFactories.h"

#include "blCore/blCast.h"

using namespace std;

int main(int argc, char* argv[] ){

    // --------------------- Get inputs --------------------------//
    if (argc < 5){
        cout << "Usage: need 5 input parameters" << endl;
        cout << "\t 1- input images folder (2D images)" << endl;
        cout << "\t 2- image file extension (ex .tif)" << endl;
        cout << "\t 3- detections files folder (.txt files)" << endl;
        cout << "\t 4- parameter File (.txt)" << endl;
        cout << "\t 5- output image name" << endl;

        return 1;
    }
    string inputFolder = argv[1];
    string imageExtension = argv[2];
    string detectionsFolder = argv[3];
    string paramFile = argv[4];
    string outputImageName = argv[5];

    // -------------------- Get the input frame list ---------------------------
    vector<string> inputImagesPaths;

    // read paths
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

    // ------------------------------ Read the Shape locations ----------------
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
                inputDetectionsFilesPaths.push_back(detectionsFolder + dirDetections->d_name);
            }
        }
    }
    closedir(dpDetection);
    std::sort( inputDetectionsFilesPaths.begin(), inputDetectionsFilesPaths.end() );

    //for(int i = 0 ; i < inputDetectionsFilesPaths.size() ; i++){
    //    cout << "inputDetectionsFile:" << inputDetectionsFilesPaths[i] << endl;
    //}

    if (inputDetectionsFilesPaths.size() != inputImagesPaths.size()){
        cout << "Error: They must be the same number of detections files and frames files" << detectionsFolder << endl;
        cout << "inputDetectionsFilesPaths:" << inputDetectionsFilesPaths.size() << " files" << endl;
        cout << "inputImagesPaths:" << inputImagesPaths.size() << " files" << endl;
    }

    // ------------------------------ Instantiate the tracker ------------------
    cout << endl;
    clock_t start = clock();
    try{
        // load the parameters
        blProcessParameters *paramManager = new blProcessParameters(paramFile);
        paramManager->load();

        // initialize the objects manager
        cout << "initialize the objects manager" << endl;
        blRATObjectsManager* objectsManager = new blRATObjectsManager;
        objectsManager->loadFramesStates(inputDetectionsFilesPaths);

        // initialize the cost manager
        cout << "initialize the cost manager" << endl;
        blRATCostManager *costManager = new blRATCostManager(objectsManager);
        blRATFactories factory(paramManager);
        std::vector<std::string> defaultCostList;
        std::vector<std::string> defaultCostWeights;
        std::vector<std::string> costs = paramManager->getVectorStringValuesOfKey("blRATCostsNames", defaultCostList);
        std::vector<std::string> weights = paramManager->getVectorStringValuesOfKey("blRATCostsWeights", defaultCostWeights);
        for(int i = 0 ; i < costs.size() ; ++i){
            blRATCostInterface* cost = factory.cost(costs[i]);

            float val = float(atof(weights[i].c_str()));
            costManager->addCost(cost, val);
        }
        costManager->initialize();

        // initialize the tracker
        cout << "initialize the tracker" << endl;
        blRATTracker* tracker = new blRATTracker(costManager, inputImagesPaths.size(), objectsManager);
        blProcessObserverCommandeLine* observer = new blProcessObserverCommandeLine;
        tracker->addObserver(observer);

        tracker->setMaxMove(paramManager->getValueOfKey<float>("blRATTrackerMaxMove", 10));
        tracker->setIntegerCoeff(1000);
        tracker->setIterationsVerboseRatio(100);
        tracker->initialize();
        cout << "tracker->run()" << endl;
        tracker->run();
        cout << "tracker->saveTracksToTxtFile();" << endl;
        tracker->saveTracksToTxtFile(outputImageName + "tracks.txt");
        std::vector<blRATTrack*> tracks = tracker->getTracks();

        // frames loader
        cout << "frames loader" << endl;
        blRATFramesLoader* frameLoader = new blRATFramesLoader;
        frameLoader->setFramesUrls(inputImagesPaths);
        frameLoader->setTracks(tracks);
        frameLoader->saveTimeTracksRepresentation(outputImageName);

        cout << "number of tracks = " << tracks.size() << endl;

        delete frameLoader;
        delete costManager;
        delete tracker;
        delete paramManager;
        for (int i = tracks.size()-1 ; i >= 0 ; i--){
            delete tracks[i];
        }
        delete observer;
    }
    catch (blException ex){
        cout << ex.what() << endl;
    }
    printf("Time elapsed: %f\n", ((double)clock() - start) / CLOCKS_PER_SEC);
}



