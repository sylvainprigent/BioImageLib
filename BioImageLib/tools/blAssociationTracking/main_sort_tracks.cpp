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
    if (argc < 4){
        cout << "Usage: need 6 input parameters" << endl;
        cout << "\t 1- input images folder (2D or 3D images)" << endl;
        cout << "\t 2- image file extension (ex .tif)" << endl;
        cout << "\t 3- tracks file (.txt)" << endl;
        cout << "\t 4- parameter File (.txt)" << endl;
        cout << "\t 5- output image name" << endl;
        cout << "\t 6- begin image index (optional)" << endl;
        cout << "\t 7- end image index (optional)" << endl;
        return 1;
    }
    string inputFolder = argv[1];
    string imageExtension = argv[2];
    string tracksFile = argv[3];
    string paramFile = argv[4];
    string outputImageName = argv[5];

    // -------------------- Get the input frame list ---------------------------
    vector<string> inputImagesPaths;

    // get borders
    int beginData, endData;
    if (argc > 4){
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

    // create the files names
    vector<string> inputImagesFiles; inputImagesFiles.resize(inputImagesPaths.size());
    for (unsigned int i = 0 ; i < inputImagesPaths.size() ; ++i){
        inputImagesFiles[i] = inputFolder + inputImagesPaths[i];
    }

    // ------------------------------ Instantiate the tracker ------------------
    cout << endl;
    clock_t start = clock();
    try{
        vector<vector<vector<float> > > Shapes;
        blProcessParameters *paramManager = new blProcessParameters();
        blAtCostEuclidean *cost = new blAtCostEuclidean();
        blAtTrackerNull *tracker = new blAtTrackerNull(cost, inputImagesFiles, Shapes );

        string sorterName = paramManager->getValueOfKey<string>("blAtSorterName","none");
        cout << "sorter name = " << sorterName << endl;
        vector<blAtTrack* > v; v.resize(1);
        blAtFactories factory(paramManager);

        if (sorterName != "none"){
            tracker->setTrackSorter(factory.sorter(sorterName));
        }
        else{
            tracker->setUseSorter(false);
        }

        cout << "LoadTracksFromTxtFile " << endl;
        tracker->loadTracksFromTxtFile(tracksFile);
        cout << "CalculateTracks " << endl;
        tracker->calculateTracks();

        // Save the result
        cout << "Save the result" << endl;
        blImage* im = tracker->getTracksRepresentationOnFirstImage();
        im->save(outputImageName + "_FirstImage.tif");
        tracker->saveTimeTracksRepresentationFull(outputImageName + "_projection.tif");
        tracker->saveTracksToTxtFile(outputImageName + ".txt");
    }
    catch (blException ex){
        cout << ex.what() << endl;
    }
    printf("Time elapsed: %f\n", ((double)clock() - start) / CLOCKS_PER_SEC);
}



