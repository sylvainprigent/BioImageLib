#include <iostream>
#include <string>
#include <vector>

#include <blTrackingStats>

#include <time.h>
#include <cstdlib>
#include <ctime>

std::string usage(std::string name)
{
    return     "" + name + " <option(s)> "
            + "Options:\n"
            + "\t-h\t\tShow this help message\n"
            + "\t-inputImage INPUT\tInput image\n"
            + "\t-radius RADIUS\tRadius of the filter in pixels\n"
            + "\t-outputImage OUTPUT\tOutput filtered image\n";
}

void saveVector(std::vector<float> data, std::string fileName){

    ofstream file(fileName.c_str(), ios::out | ios::trunc);
    if(file)
    {
        // 2- Save each track
        for (unsigned int i = 0 ; i < data.size() ; ++i){
            file << data[i] << endl;
        }
        file.close();
    }
    else{
        cerr << "Impossible to open the file !" << fileName << endl;
    }

}

int main(int argc, char* argv[])
{
    try{


        std::string outputDir = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\test_tracking\\stats2\\";
        int numFrames = 24;

        // load tracking file
        blSTracks* strack = new blSTracks();
        strack->loadTracksFromTxtFile("C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\test_tracking\\tracking_res.tif.txt");
        strack->saveTracksToTxtFile("C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\test_tracking\\tracking_res_copie.txt");

        // partition tracks into groups
        std::vector<std::vector<float> > gpoints;
        std::vector<float> p1; p1.resize(2); p1[0] = 626; p1[1] = 268; gpoints.push_back(p1);
        std::vector<float> p2; p2.resize(2); p2[0] = 1280; p2[1] = 268; gpoints.push_back(p2);
        std::vector<float> p3; p3.resize(2); p3[0] = 630; p3[1] = 754; gpoints.push_back(p3);
        std::vector<float> p4; p4.resize(2); p4[0] = 1280; p4[1] = 754; gpoints.push_back(p4);

        std::vector<blSTracks*> tracksGroups = strack->makeGroups(gpoints);
        strack->saveTracksGroupsToFile(tracksGroups, "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\test_tracking\\tracking_group");


        for (int g = 0 ; g < gpoints.size() ; ++g){
            blSTracks* track = tracksGroups[g];

            int startFrameIdx = 0;
            int endFrameIdx = numFrames -1;
            int startFrame = 0;
            int endFrame = numFrames -1;
            float frameRate = 5;
            float refX = gpoints[g][0];
            float refY = gpoints[g][1];

            std::vector<float> averageDistance = track->averageDistance(startFrameIdx, endFrameIdx);
            std::vector<float> averageSpeed = track->averageSpeed(startFrameIdx, endFrameIdx, frameRate);
            std::vector<float> averageDistanceToRef = track->averageDistanceToRef(refX,  refY, startFrameIdx, endFrameIdx);

            std::vector<float> groupeDistanceDist = track->groupeDistanceDist(startFrame, endFrame);
            std::vector<float> groupeStartToEndDistanceDist = track->groupeStartToEndDistanceDist(startFrame, endFrame);
            std::vector<float> groupeSpeedDist = track->groupeSpeedDist(startFrame, endFrame, frameRate);
            std::vector<float> groupeDistanceToRefDist = track->groupeDistanceToRefDist(refX,  refY, startFrame, endFrame);

            saveVector(averageDistance, outputDir + blCast::to_string(g) + "_averageDistance.txt");
            saveVector(averageSpeed, outputDir + blCast::to_string(g) + "_averageSpeed.txt");
            saveVector(averageDistanceToRef, outputDir + blCast::to_string(g) + "_averageDistanceToRef.txt");

            saveVector(groupeDistanceDist, outputDir + blCast::to_string(g) + "_groupeDistanceDist.txt");
            saveVector(groupeStartToEndDistanceDist, outputDir + blCast::to_string(g) + "_groupeStartToEndDistanceDist.txt");
            saveVector(groupeSpeedDist, outputDir + blCast::to_string(g) + "_groupeSpeedDist.txt");
            saveVector(groupeDistanceToRefDist, outputDir + blCast::to_string(g) + "_groupeDistanceToRefDist.txt");

            track->averageDistanceToEachOthers(startFrame, endFrame);
            track->saveTracksAllInfoToTxtFile(track->tracks(), outputDir + blCast::to_string(g) + "_tracks_info.txt");
        }

    }
    catch (blException ex){
        std::cout << ex.what() << std::endl;
        return 1;
    }
}
