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

#include "blAtFactories.h"

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
        cout << "\t 6- begin image index (optional)" << endl;
        cout << "\t 7- end image index (optional)" << endl;

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
    if (argc > 6){
        istringstream ss(argv[6]);
        if (!(ss >> beginData))
            cerr << "Invalid number " << argv[6] << '\n';
        istringstream ss2(argv[7]);
        if (!(ss2 >> endData))
            cerr << "Invalid number " << argv[7] << '\n';
    }
    else{
        beginData = -1;
        endData = -1;
    }

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
                inputDetectionsFilesPaths.push_back(dirDetections->d_name);
            }
        }
    }
    closedir(dpDetection);
    std::sort( inputDetectionsFilesPaths.begin(), inputDetectionsFilesPaths.end() );

    if (inputDetectionsFilesPaths.size() != inputImagesPaths.size()){
        cout << "Error: They must be the same number of detections files and frames files" << detectionsFolder << endl;
        cout << "inputDetectionsFilesPaths:" << inputDetectionsFilesPaths.size() << " files" << endl;
        cout << "inputImagesPaths:" << inputImagesPaths.size() << " files" << endl;
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

    // load all the Shapes files
    vector<vector<vector<float> > > Shapes; Shapes.resize(inputDetectionsFilesPaths.size());
    for (unsigned int i = 0 ; i < inputDetectionsFilesPaths.size() ; ++i){
        Shapes[i] = blTxtIO::LoadShapeFromTxtFile(detectionsFolder + inputDetectionsFilesPaths.at(i));
    }

    // create the files names
    vector<string> inputImagesFiles; inputImagesFiles.resize(inputImagesPaths.size());
    for (unsigned int i = 0 ; i < inputImagesPaths.size() ; ++i){
        inputImagesFiles[i] = inputFolder + inputImagesPaths[i];
    }

    // ------------------------------ Instantiate the tracker ------------------
    cout << endl;
    clock_t start = clock();
    try{
        blProcessParameters *paramManager = new blProcessParameters(paramFile);
        paramManager->load();

        blAtFactories factory(paramManager);
        blAtCostInterface* cost = factory.cost(paramManager->getValueOfKey<std::string>("blAtCostName", "none"));
        blAtTrackerInterface* tracker = factory.tracker(paramManager->getValueOfKey<std::string>("blAtTrackerName", "none"));


        // init
        tracker->setInputs(cost, inputImagesFiles, Shapes);

        blProcessObserverCommandeLine* observer = new blProcessObserverCommandeLine;
        tracker->addObserver(observer);

        std::string sorterName = paramManager->getValueOfKey<string>("blAtSorterName", "none");
        blAtTrackSorterInterface* sorter = NULL;
        if (sorterName != "none"){
            sorter = factory.sorter(sorterName);
            tracker->setTrackSorter(sorter);
        }
        else{
            tracker->setUseSorter(false);
        }

        // run
        tracker->run();
        tracker->calculateTracks();

        // Save the result
        tracker->saveTracksToTxtFile(outputImageName + "tracks.txt");
        tracker->saveTimeTracksRepresentationFullIndividual(outputImageName);

        vector<blAtTrack*> tracks = tracker->output();
        cout << "number of tracks = " << tracks.size() << endl;

        delete cost;
        delete tracker;
        delete paramManager;
        if (sorter != NULL){delete sorter;}
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



