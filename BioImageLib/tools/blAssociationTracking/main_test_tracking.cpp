#include "fpt_tracker_null.h"
#include "fpt_cost_base.h"

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

int main(void){

    Kernel::ParamManager *paramManager = new Kernel::ParamManager();
    FeaturePointTracking::FPT_TrackerNull *tracker = new FeaturePointTracking::FPT_TrackerNull(paramManager);

    /*
    string framesPath;
    vector<string> framesNames;
    vector<vector<vector<float> > > objects;
    FeaturePointTracking::FPT_CostEuclidian *costFunction = new FeaturePointTracking::FPT_CostEuclidian(paramManager);
    ParamManager *paramManager = new paramManager();
    FeaturePointTracking::FPT_TrackerNull tracker = new FeaturePointTracking::FPT_TrackerNull(framesPath, framesNames, objects, costFunction, paramManager);
    */

    tracker->LoadTracksFromTxtFile("C:\\Users\\prigent\\Documents\\Documents\\experiments\\ipmc_graph_detection\\200-jump\\res.tif.txt");
    tracker->SaveTracksToTxtFile("C:\\Users\\prigent\\Documents\\Documents\\experiments\\ipmc_graph_detection\\200-jump\\res_copy.txt");

}
