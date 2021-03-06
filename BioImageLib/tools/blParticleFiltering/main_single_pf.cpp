#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>


#include "blPfModelsFactory.h"
#include "blParticleFiltering/blPfOptimizerSingleObject.h"
#include <blCore>

int main(int argc, char* argv[] ){

    // --------------------- Get inputs --------------------------//
    if (argc < 5){
        cout << "Usage: need X input parameters" << endl;
        cout << "\t 1- input images folder (2D images)" << endl;
        cout << "\t 2- image file extension (ex .tif)" << endl;
        cout << "\t 3- starting point (in a .txt)" << endl;
        cout << "\t 4- parameter File (.txt)" << endl;
        cout << "\t 5- output image name" << endl;
        cout << "\t 6- begin image index (optional)" << endl;
        cout << "\t 7- end image index (optional)" << endl;
        return 1;
    }
    string inputFolder = argv[1];
    string imageExtension = argv[2];
    string startingPointFile = argv[3];
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

    // Get the selected images
    vector<string> framesInter;

    if (beginData != -1 && endData != -1){
        if (int(inputImagesPaths.size()) > endData){
            for (int compt = beginData ; compt <= endData ; ++compt){
                framesInter.push_back(inputImagesPaths[compt]);
            }
            inputImagesPaths.clear(); inputImagesPaths = framesInter;
        }
        else{
            cerr << "Error : the frame range is above the number of images founded in the input folder: " << endl;
        }
    }

    cout << "input images = " << endl;
    for (unsigned int i = 0 ; i < inputImagesPaths.size() ; ++i){
        cout << inputImagesPaths.at(i) << endl;
    }

    // ------------------------------ Read the object location ----------------
    vector<vector<float> > objects;
    objects = blTxtIO::LoadShapeFromTxtFile(startingPointFile);
    if (objects.size() < 1){
        cerr << "Error : There are no objects in the specified file: " << endl;
    }

    vector<float> singleObject; singleObject.resize(3);
    for (unsigned int i = 0 ; i < objects[0].size() ; ++i){
        singleObject[i] = objects[0][i];
    }

    cout << "starting point = " << singleObject[0] << ", " << singleObject[1] << endl;

    // ------------------------------ Instantiate the tracker ------------------
    cout << endl;
    try{
        blProcessParameters *paramManager = new blProcessParameters(paramFile);
        paramManager->load();

        // data manager
        blPfDataManager* dataManager = new blPfDataManager(singleObject, inputImagesPaths, inputFolder);

        // model
        blPfModelsFactory modelFactory(paramManager);
        std::string modelName = paramManager->getValueOfKey<std::string>("blPfModelName", "none");
        blPfModelInterface* pfmodel = modelFactory.model(dataManager, modelName);
        pfmodel->init();

        // optimizer
        blPfOptimizerSingleObject* pfFilter = new blPfOptimizerSingleObject(pfmodel);
        blProcessObserverCommandeLine* observer = new blProcessObserverCommandeLine;
        pfFilter->addObserver(observer);
        pfFilter->setParticleNumber(paramManager->getValueOfKey<int>("blPfparticleNumber", 200));
        pfFilter->setResamplingThreshold(paramManager->getValueOfKey<float>("blPfdegeneracyTh", 10000.0));
        pfFilter->setResamplingMethod(paramManager->getValueOfKey<string>("blPfResampling", "ResampleResidual"));
        pfFilter->initialization();
        pfFilter->run();

        blPfTrack* track = pfFilter->getTrack();

        // plot the result
        cout << "track = " << endl;
        for(int i =0 ; i < track->getTrackSize() ; ++i){
            cout << track->getStateAt(i)->getStateAt(0)
                 << ", " << track->getStateAt(i)->getStateAt(1)
                 << ", " << track->getStateAt(i)->getStateAt(2)
                 << ", " << track->getStateAt(i)->getStateAt(3)
                 << endl;
        }

        // Save the result
        cout << "Save the result = " << endl;
        pfFilter->saveTrackRepresentation(outputImageName);

        delete paramManager;
        delete pfmodel;
        delete pfFilter;
        delete observer;
    }
    catch (blException ex){
        cout << ex.what() << endl;
    }
    catch ( std::exception ex ){
         cout << ex.what() << endl;
    }
}



