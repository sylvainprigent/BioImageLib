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

void extractStats(std::string tracksfile, std::vector<std::vector<float> > gpoints, std::vector<int> gpointsIdxs, std::string outputDir, int numFrames){

    //int numFrames = 501;

    // load tracking file
    blSTracks* strack = new blSTracks();
    strack->loadTracksFromTxtFile(tracksfile);

    // partition tracks into groups
    /*
    std::vector<std::vector<float> > gpoints;
    std::vector<float> p1; p1.resize(2); p1[0] = 626; p1[1] = 268; gpoints.push_back(p1);
    std::vector<float> p2; p2.resize(2); p2[0] = 1280; p2[1] = 268; gpoints.push_back(p2);
    std::vector<float> p3; p3.resize(2); p3[0] = 630; p3[1] = 754; gpoints.push_back(p3);
    std::vector<float> p4; p4.resize(2); p4[0] = 1280; p4[1] = 754; gpoints.push_back(p4);
    */

    std::vector<blSTracks*> tracksGroups = strack->makeGroups(gpoints);
    strack->saveTracksGroupsToFile(tracksGroups, outputDir);


    for (int g = 0 ; g < gpoints.size() ; ++g){
        blSTracks* track = tracksGroups[g];

        int startFrameIdx = 0;
        int endFrameIdx = numFrames -1;
        int startFrame = 0;
        int endFrame = numFrames -1;
        float frameRate = 10;
        float refX = gpoints[g][0];
        float refY = gpoints[g][1];

        std::vector<float> averageDistance = track->averageDistance(startFrameIdx, endFrameIdx);
        std::vector<float> averageSpeed = track->averageSpeed(startFrameIdx, endFrameIdx, frameRate);
        std::vector<float> averageDistanceToRef = track->averageDistanceToRef(refX,  refY, startFrameIdx, endFrameIdx);

        std::vector<float> groupeDistanceDist = track->groupeDistanceDist(startFrame, endFrame);
        std::vector<float> groupeStartToEndDistanceDist = track->groupeStartToEndDistanceDist(startFrame, endFrame);
        std::vector<float> groupeSpeedDist = track->groupeSpeedDist(startFrame, endFrame, frameRate);
        std::vector<float> groupeDistanceToRefDist = track->groupeDistanceToRefDist(refX,  refY, startFrame, endFrame);


        std::string idxName = blCast::to_string(gpointsIdxs[g]);
        saveVector(averageDistance, outputDir + idxName + "_averageDistance.txt");
        saveVector(averageSpeed, outputDir + idxName + "_averageSpeed.txt");
        saveVector(averageDistanceToRef, outputDir + idxName + "_averageDistanceToRef.txt");

        saveVector(groupeDistanceDist, outputDir + idxName + "_groupeDistanceDist.txt");
        saveVector(groupeStartToEndDistanceDist, outputDir + idxName + "_groupeStartToEndDistanceDist.txt");
        saveVector(groupeSpeedDist, outputDir + idxName + "_groupeSpeedDist.txt");
        saveVector(groupeDistanceToRefDist, outputDir + idxName + "_groupeDistanceToRefDist.txt");

        track->averageDistanceToEachOthers(startFrame, endFrame);
        track->saveTracksAllInfoToTxtFile(track->tracks(), outputDir + idxName + "_tracks_info.txt");
    }
}


void movie17b(){

    std::string tracksfile = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session1\\17_06_2014\\B\\tracking_res_line\\tracks.txt";
    std::vector<std::vector<float> > gpoints;
    std::vector<float> p1; p1.resize(2); p1[0] = 650; p1[1] = 245; gpoints.push_back(p1);
    std::vector<float> p2; p2.resize(2); p2[0] = 1345; p2[1] = 250; gpoints.push_back(p2);
    std::vector<float> p3; p3.resize(2); p3[0] = 638; p3[1] = 770; gpoints.push_back(p3);
    std::vector<float> p4; p4.resize(2); p4[0] = 1335; p4[1] = 770; gpoints.push_back(p4);
    std::vector<int> gpointsIdx;
    gpointsIdx.push_back(7);
    gpointsIdx.push_back(8);
    gpointsIdx.push_back(5);
    gpointsIdx.push_back(6);
    std::string outputDir = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session1\\17_06_2014\\B\\stats_line\\";
    extractStats(tracksfile, gpoints, gpointsIdx, outputDir, 5227);
}

void movie17c(){

    std::string tracksfile = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session1\\17_06_2014\\C\\tracking_res_line\\tracks.txt";
    std::vector<std::vector<float> > gpoints;
    std::vector<float> p1; p1.resize(2); p1[0] = 624; p1[1] = 292; gpoints.push_back(p1);
    std::vector<float> p2; p2.resize(2); p2[0] = 1310; p2[1] = 292; gpoints.push_back(p2);
    std::vector<float> p3; p3.resize(2); p3[0] = 620; p3[1] = 812; gpoints.push_back(p3);
    std::vector<float> p4; p4.resize(2); p4[0] = 1310; p4[1] = 812; gpoints.push_back(p4);
    std::vector<int> gpointsIdx;
    gpointsIdx.push_back(3);
    gpointsIdx.push_back(4);
    gpointsIdx.push_back(1);
    gpointsIdx.push_back(2);
    std::string outputDir = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session1\\17_06_2014\\C\\stats_line\\";
    extractStats(tracksfile, gpoints, gpointsIdx, outputDir, 6003);

}

void movie18b(){

    std::string tracksfile = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session1\\18_06_2014\\B\\tracking_res_line\\tracks.txt";
    std::vector<std::vector<float> > gpoints;
    std::vector<float> p1; p1.resize(2); p1[0] = 650; p1[1] = 245; gpoints.push_back(p1);
    std::vector<float> p2; p2.resize(2); p2[0] = 1345; p2[1] = 250; gpoints.push_back(p2);
    std::vector<float> p3; p3.resize(2); p3[0] = 638; p3[1] = 770; gpoints.push_back(p3);
    std::vector<float> p4; p4.resize(2); p4[0] = 1335; p4[1] = 770; gpoints.push_back(p4);
    std::vector<int> gpointsIdx;
    gpointsIdx.push_back(7);
    gpointsIdx.push_back(8);
    gpointsIdx.push_back(5);
    gpointsIdx.push_back(6);
    std::string outputDir = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session1\\18_06_2014\\B\\stats_line\\";
    extractStats(tracksfile, gpoints, gpointsIdx, outputDir, 6000);
}

void movie18c(){

    std::string tracksfile = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session1\\18_06_2014\\C\\tracking_res_line\\tracks.txt";
    std::vector<std::vector<float> > gpoints;
    std::vector<float> p1; p1.resize(2); p1[0] = 624; p1[1] = 292; gpoints.push_back(p1);
    std::vector<float> p2; p2.resize(2); p2[0] = 1310; p2[1] = 292; gpoints.push_back(p2);
    std::vector<float> p3; p3.resize(2); p3[0] = 620; p3[1] = 812; gpoints.push_back(p3);
    std::vector<float> p4; p4.resize(2); p4[0] = 1310; p4[1] = 812; gpoints.push_back(p4);
    std::vector<int> gpointsIdx;
    gpointsIdx.push_back(3);
    gpointsIdx.push_back(4);
    gpointsIdx.push_back(1);
    gpointsIdx.push_back(2);
    std::string outputDir = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session1\\18_06_2014\\C\\stats_line\\";
    extractStats(tracksfile, gpoints, gpointsIdx, outputDir, 6005);

}

void movie19b(){

    std::string tracksfile = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session1\\19_06_2014\\B\\tracking_res_line\\tracks.txt";
    std::vector<std::vector<float> > gpoints;
    std::vector<float> p1; p1.resize(2); p1[0] = 646; p1[1] = 245; gpoints.push_back(p1);
    std::vector<float> p2; p2.resize(2); p2[0] = 1345; p2[1] = 250; gpoints.push_back(p2);
    std::vector<float> p3; p3.resize(2); p3[0] = 635; p3[1] = 770; gpoints.push_back(p3);
    std::vector<float> p4; p4.resize(2); p4[0] = 1345; p4[1] = 770; gpoints.push_back(p4);
    std::vector<int> gpointsIdx;
    gpointsIdx.push_back(7);
    gpointsIdx.push_back(8);
    gpointsIdx.push_back(5);
    gpointsIdx.push_back(6);
    std::string outputDir = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session1\\19_06_2014\\B\\stats_line\\";
    extractStats(tracksfile, gpoints, gpointsIdx, outputDir, 6243);

}

void movie19c(){

    std::string tracksfile = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session1\\19_06_2014\\C\\tracking_res_line\\tracks.txt";
    std::vector<std::vector<float> > gpoints;
    std::vector<float> p1; p1.resize(2); p1[0] = 624; p1[1] = 292; gpoints.push_back(p1);
    std::vector<float> p2; p2.resize(2); p2[0] = 1308; p2[1] = 292; gpoints.push_back(p2);
    std::vector<float> p3; p3.resize(2); p3[0] = 612; p3[1] = 810; gpoints.push_back(p3);
    std::vector<float> p4; p4.resize(2); p4[0] = 1316; p4[1] = 810; gpoints.push_back(p4);
    std::vector<int> gpointsIdx;
    gpointsIdx.push_back(3);
    gpointsIdx.push_back(4);
    gpointsIdx.push_back(1);
    gpointsIdx.push_back(2);
    std::string outputDir = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session1\\19_06_2014\\C\\stats_line\\";
    extractStats(tracksfile, gpoints, gpointsIdx, outputDir, 6243);

}

void movie20b(){

    std::string tracksfile = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session1\\20_06_2014\\B\\tracking_res_line\\tracks.txt";
    std::vector<std::vector<float> > gpoints;
    std::vector<float> p1; p1.resize(2); p1[0] = 646; p1[1] = 244; gpoints.push_back(p1);
    std::vector<float> p2; p2.resize(2); p2[0] = 1346; p2[1] = 250; gpoints.push_back(p2);
    std::vector<float> p3; p3.resize(2); p3[0] = 630; p3[1] = 766; gpoints.push_back(p3);
    std::vector<float> p4; p4.resize(2); p4[0] = 1332; p4[1] = 766; gpoints.push_back(p4);
    std::vector<int> gpointsIdx;
    gpointsIdx.push_back(7);
    gpointsIdx.push_back(8);
    gpointsIdx.push_back(5);
    gpointsIdx.push_back(6);
    std::string outputDir = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session1\\20_06_2014\\B\\stats_line\\";
    extractStats(tracksfile, gpoints, gpointsIdx, outputDir, 6000);

}

void movie20c(){

    std::string tracksfile = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session1\\20_06_2014\\C\\tracking_res_line\\tracks.txt";
    std::vector<std::vector<float> > gpoints;
    std::vector<float> p1; p1.resize(2); p1[0] = 626; p1[1] = 296; gpoints.push_back(p1);
    std::vector<float> p2; p2.resize(2); p2[0] = 1314; p2[1] = 296; gpoints.push_back(p2);
    std::vector<float> p3; p3.resize(2); p3[0] = 620; p3[1] = 812; gpoints.push_back(p3);
    std::vector<float> p4; p4.resize(2); p4[0] = 1320; p4[1] = 816; gpoints.push_back(p4);
    std::vector<int> gpointsIdx;
    gpointsIdx.push_back(3);
    gpointsIdx.push_back(4);
    gpointsIdx.push_back(1);
    gpointsIdx.push_back(2);
    std::string outputDir = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session1\\20_06_2014\\C\\stats_line\\";
    extractStats(tracksfile, gpoints, gpointsIdx, outputDir, 6000);

}

void movie21b(){

    std::string tracksfile = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session1\\21_06_2014\\B\\tracking_res_line\\tracks.txt";
    std::vector<std::vector<float> > gpoints;
    std::vector<float> p1; p1.resize(2); p1[0] = 652; p1[1] = 240; gpoints.push_back(p1);
    std::vector<float> p2; p2.resize(2); p2[0] = 1348; p2[1] = 250; gpoints.push_back(p2);
    std::vector<float> p3; p3.resize(2); p3[0] = 638; p3[1] = 764; gpoints.push_back(p3);
    std::vector<float> p4; p4.resize(2); p4[0] = 1336; p4[1] = 780; gpoints.push_back(p4);
    std::vector<int> gpointsIdx;
    gpointsIdx.push_back(7);
    gpointsIdx.push_back(8);
    gpointsIdx.push_back(5);
    gpointsIdx.push_back(6);
    std::string outputDir = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session1\\21_06_2014\\B\\stats_line\\";
    extractStats(tracksfile, gpoints, gpointsIdx, outputDir, 6074);

}

void movie21c(){

    std::string tracksfile = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session1\\21_06_2014\\C\\tracking_res_line\\tracks.txt";
    std::vector<std::vector<float> > gpoints;
    std::vector<float> p1; p1.resize(2); p1[0] = 622; p1[1] = 290; gpoints.push_back(p1);
    std::vector<float> p2; p2.resize(2); p2[0] = 1312; p2[1] = 290; gpoints.push_back(p2);
    std::vector<float> p3; p3.resize(2); p3[0] = 622; p3[1] = 810; gpoints.push_back(p3);
    std::vector<float> p4; p4.resize(2); p4[0] = 1316; p4[1] = 810; gpoints.push_back(p4);
    std::vector<int> gpointsIdx;
    gpointsIdx.push_back(3);
    gpointsIdx.push_back(4);
    gpointsIdx.push_back(1);
    gpointsIdx.push_back(2);
    std::string outputDir = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session1\\21_06_2014\\C\\stats_line\\";
    extractStats(tracksfile, gpoints, gpointsIdx, outputDir, 6074);

}

// ////////////////////////////// session 2 ////////////////////////////////////
void movie14b2(){

    std::string tracksfile = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\14-08-14\\B\\tracks.txt";
    std::vector<std::vector<float> > gpoints;
    std::vector<float> p1; p1.resize(2); p1[0] = 622; p1[1] = 290; gpoints.push_back(p1);
    std::vector<float> p2; p2.resize(2); p2[0] = 1312; p2[1] = 290; gpoints.push_back(p2);
    std::vector<float> p3; p3.resize(2); p3[0] = 622; p3[1] = 810; gpoints.push_back(p3);
    std::vector<float> p4; p4.resize(2); p4[0] = 1316; p4[1] = 810; gpoints.push_back(p4);
    std::vector<int> gpointsIdx;
    gpointsIdx.push_back(7);
    gpointsIdx.push_back(8);
    gpointsIdx.push_back(5);
    gpointsIdx.push_back(6);
    std::string outputDir = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\14-08-14\\B\\stats_line\\";
    extractStats(tracksfile, gpoints, gpointsIdx, outputDir, 6002);
}

void movie14bc2(){

    std::string tracksfile = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\14-08-14\\BC\\tracks.txt";
    std::vector<std::vector<float> > gpoints;
    std::vector<float> p1; p1.resize(2); p1[0] = 622; p1[1] = 290; gpoints.push_back(p1);
    std::vector<float> p2; p2.resize(2); p2[0] = 1312; p2[1] = 290; gpoints.push_back(p2);
    std::vector<float> p3; p3.resize(2); p3[0] = 622; p3[1] = 810; gpoints.push_back(p3);
    std::vector<float> p4; p4.resize(2); p4[0] = 1316; p4[1] = 810; gpoints.push_back(p4);
    std::vector<int> gpointsIdx;
    gpointsIdx.push_back(11);
    gpointsIdx.push_back(12);
    gpointsIdx.push_back(9);
    gpointsIdx.push_back(10);
    std::string outputDir = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\14-08-14\\BC\\stats_line\\";
    extractStats(tracksfile, gpoints, gpointsIdx, outputDir, 6002);
}

void movie14c2(){

    std::string tracksfile = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\14-08-14\\C\\tracks.txt";
    std::vector<std::vector<float> > gpoints;
    std::vector<float> p1; p1.resize(2); p1[0] = 622; p1[1] = 290; gpoints.push_back(p1);
    std::vector<float> p2; p2.resize(2); p2[0] = 1312; p2[1] = 290; gpoints.push_back(p2);
    std::vector<float> p3; p3.resize(2); p3[0] = 622; p3[1] = 810; gpoints.push_back(p3);
    std::vector<float> p4; p4.resize(2); p4[0] = 1316; p4[1] = 810; gpoints.push_back(p4);
    std::vector<int> gpointsIdx;
    gpointsIdx.push_back(3);
    gpointsIdx.push_back(4);
    gpointsIdx.push_back(1);
    gpointsIdx.push_back(2);
    std::string outputDir = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\14-08-14\\C\\stats_line\\";
    extractStats(tracksfile, gpoints, gpointsIdx, outputDir, 6003);
}

void movie15b2(){

    std::string tracksfile = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\15-08-14\\B\\tracks.txt";
    std::vector<std::vector<float> > gpoints;
    std::vector<float> p1; p1.resize(2); p1[0] = 622; p1[1] = 290; gpoints.push_back(p1);
    std::vector<float> p2; p2.resize(2); p2[0] = 1312; p2[1] = 290; gpoints.push_back(p2);
    std::vector<float> p3; p3.resize(2); p3[0] = 622; p3[1] = 810; gpoints.push_back(p3);
    std::vector<float> p4; p4.resize(2); p4[0] = 1316; p4[1] = 810; gpoints.push_back(p4);
    std::vector<int> gpointsIdx;
    gpointsIdx.push_back(7);
    gpointsIdx.push_back(8);
    gpointsIdx.push_back(5);
    gpointsIdx.push_back(6);
    std::string outputDir = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\15-08-14\\B\\stats_line\\";
    extractStats(tracksfile, gpoints, gpointsIdx, outputDir, 5876);
}

void movie15bc2(){

    std::string tracksfile = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\15-08-14\\BC\\tracks.txt";
    std::vector<std::vector<float> > gpoints;
    std::vector<float> p1; p1.resize(2); p1[0] = 622; p1[1] = 290; gpoints.push_back(p1);
    std::vector<float> p2; p2.resize(2); p2[0] = 1312; p2[1] = 290; gpoints.push_back(p2);
    std::vector<float> p3; p3.resize(2); p3[0] = 622; p3[1] = 810; gpoints.push_back(p3);
    std::vector<float> p4; p4.resize(2); p4[0] = 1316; p4[1] = 810; gpoints.push_back(p4);
    std::vector<int> gpointsIdx;
    gpointsIdx.push_back(11);
    gpointsIdx.push_back(12);
    gpointsIdx.push_back(9);
    gpointsIdx.push_back(10);
    std::string outputDir = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\15-08-14\\BC\\stats_line\\";
    extractStats(tracksfile, gpoints, gpointsIdx, outputDir, 3306);
}

void movie15c2(){

    std::string tracksfile = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\15-08-14\\C\\tracks.txt";
    std::vector<std::vector<float> > gpoints;
    std::vector<float> p1; p1.resize(2); p1[0] = 622; p1[1] = 290; gpoints.push_back(p1);
    std::vector<float> p2; p2.resize(2); p2[0] = 1312; p2[1] = 290; gpoints.push_back(p2);
    std::vector<float> p3; p3.resize(2); p3[0] = 622; p3[1] = 810; gpoints.push_back(p3);
    std::vector<float> p4; p4.resize(2); p4[0] = 1316; p4[1] = 810; gpoints.push_back(p4);
    std::vector<int> gpointsIdx;
    gpointsIdx.push_back(3);
    gpointsIdx.push_back(4);
    gpointsIdx.push_back(1);
    gpointsIdx.push_back(2);
    std::string outputDir = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\15-08-14\\C\\stats_line\\";
    extractStats(tracksfile, gpoints, gpointsIdx, outputDir, 6021);
}

void movie16b2(){

    std::string tracksfile = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\16-08-14\\B\\tracks.txt";
    std::vector<std::vector<float> > gpoints;
    std::vector<float> p1; p1.resize(2); p1[0] = 622; p1[1] = 290; gpoints.push_back(p1);
    std::vector<float> p2; p2.resize(2); p2[0] = 1312; p2[1] = 290; gpoints.push_back(p2);
    std::vector<float> p3; p3.resize(2); p3[0] = 622; p3[1] = 810; gpoints.push_back(p3);
    std::vector<float> p4; p4.resize(2); p4[0] = 1316; p4[1] = 810; gpoints.push_back(p4);
    std::vector<int> gpointsIdx;
    gpointsIdx.push_back(7);
    gpointsIdx.push_back(8);
    gpointsIdx.push_back(5);
    gpointsIdx.push_back(6);
    std::string outputDir = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\16-08-14\\B\\stats_line\\";
    extractStats(tracksfile, gpoints, gpointsIdx, outputDir, 6228);
}

void movie16bc2(){

    std::string tracksfile = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\16-08-14\\BC\\tracks.txt";
    std::vector<std::vector<float> > gpoints;
    std::vector<float> p1; p1.resize(2); p1[0] = 622; p1[1] = 290; gpoints.push_back(p1);
    std::vector<float> p2; p2.resize(2); p2[0] = 1312; p2[1] = 290; gpoints.push_back(p2);
    std::vector<float> p3; p3.resize(2); p3[0] = 622; p3[1] = 810; gpoints.push_back(p3);
    std::vector<float> p4; p4.resize(2); p4[0] = 1316; p4[1] = 810; gpoints.push_back(p4);
    std::vector<int> gpointsIdx;
    gpointsIdx.push_back(11);
    gpointsIdx.push_back(12);
    gpointsIdx.push_back(9);
    gpointsIdx.push_back(10);
    std::string outputDir = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\16-08-14\\BC\\stats_line\\";
    extractStats(tracksfile, gpoints, gpointsIdx, outputDir, 6038);
}

void movie16c2(){

    std::string tracksfile = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\16-08-14\\C\\tracks.txt";
    std::vector<std::vector<float> > gpoints;
    std::vector<float> p1; p1.resize(2); p1[0] = 622; p1[1] = 290; gpoints.push_back(p1);
    std::vector<float> p2; p2.resize(2); p2[0] = 1312; p2[1] = 290; gpoints.push_back(p2);
    std::vector<float> p3; p3.resize(2); p3[0] = 622; p3[1] = 810; gpoints.push_back(p3);
    std::vector<float> p4; p4.resize(2); p4[0] = 1316; p4[1] = 810; gpoints.push_back(p4);
    std::vector<int> gpointsIdx;
    gpointsIdx.push_back(3);
    gpointsIdx.push_back(4);
    gpointsIdx.push_back(1);
    gpointsIdx.push_back(2);
    std::string outputDir = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\16-08-14\\C\\stats_line\\";
    extractStats(tracksfile, gpoints, gpointsIdx, outputDir, 7357);
}

void movie17b2(){

    std::string tracksfile = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\17-08-14\\B\\tracks.txt";
    std::vector<std::vector<float> > gpoints;
    std::vector<float> p1; p1.resize(2); p1[0] = 622; p1[1] = 290; gpoints.push_back(p1);
    std::vector<float> p2; p2.resize(2); p2[0] = 1312; p2[1] = 290; gpoints.push_back(p2);
    std::vector<float> p3; p3.resize(2); p3[0] = 622; p3[1] = 810; gpoints.push_back(p3);
    std::vector<float> p4; p4.resize(2); p4[0] = 1316; p4[1] = 810; gpoints.push_back(p4);
    std::vector<int> gpointsIdx;
    gpointsIdx.push_back(7);
    gpointsIdx.push_back(8);
    gpointsIdx.push_back(5);
    gpointsIdx.push_back(6);
    std::string outputDir = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\17-08-14\\B\\stats_line\\";
    extractStats(tracksfile, gpoints, gpointsIdx, outputDir, 6107);
}

void movie17bc2(){

    std::string tracksfile = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\17-08-14\\BC\\tracks.txt";
    std::vector<std::vector<float> > gpoints;
    std::vector<float> p1; p1.resize(2); p1[0] = 622; p1[1] = 290; gpoints.push_back(p1);
    std::vector<float> p2; p2.resize(2); p2[0] = 1312; p2[1] = 290; gpoints.push_back(p2);
    std::vector<float> p3; p3.resize(2); p3[0] = 622; p3[1] = 810; gpoints.push_back(p3);
    std::vector<float> p4; p4.resize(2); p4[0] = 1316; p4[1] = 810; gpoints.push_back(p4);
    std::vector<int> gpointsIdx;
    gpointsIdx.push_back(11);
    gpointsIdx.push_back(12);
    gpointsIdx.push_back(9);
    gpointsIdx.push_back(10);
    std::string outputDir = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\17-08-14\\BC\\stats_line\\";
    extractStats(tracksfile, gpoints, gpointsIdx, outputDir, 6102);
}

void movie17c2(){

    std::string tracksfile = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\17-08-14\\C\\tracks.txt";
    std::vector<std::vector<float> > gpoints;
    std::vector<float> p1; p1.resize(2); p1[0] = 622; p1[1] = 290; gpoints.push_back(p1);
    std::vector<float> p2; p2.resize(2); p2[0] = 1312; p2[1] = 290; gpoints.push_back(p2);
    std::vector<float> p3; p3.resize(2); p3[0] = 622; p3[1] = 810; gpoints.push_back(p3);
    std::vector<float> p4; p4.resize(2); p4[0] = 1316; p4[1] = 810; gpoints.push_back(p4);
    std::vector<int> gpointsIdx;
    gpointsIdx.push_back(3);
    gpointsIdx.push_back(4);
    gpointsIdx.push_back(1);
    gpointsIdx.push_back(2);
    std::string outputDir = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\17-08-14\\C\\stats_line\\";
    extractStats(tracksfile, gpoints, gpointsIdx, outputDir, 6107);
}

void movie18b2(){

    std::string tracksfile = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\18-08-14\\B\\tracks.txt";
    std::vector<std::vector<float> > gpoints;
    std::vector<float> p1; p1.resize(2); p1[0] = 622; p1[1] = 290; gpoints.push_back(p1);
    std::vector<float> p2; p2.resize(2); p2[0] = 1312; p2[1] = 290; gpoints.push_back(p2);
    std::vector<float> p3; p3.resize(2); p3[0] = 622; p3[1] = 810; gpoints.push_back(p3);
    std::vector<float> p4; p4.resize(2); p4[0] = 1316; p4[1] = 810; gpoints.push_back(p4);
    std::vector<int> gpointsIdx;
    gpointsIdx.push_back(7);
    gpointsIdx.push_back(8);
    gpointsIdx.push_back(5);
    gpointsIdx.push_back(6);
    std::string outputDir = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\18-08-14\\B\\stats_line\\";
    extractStats(tracksfile, gpoints, gpointsIdx, outputDir, 5934);
}

void movie18bc2(){

    std::string tracksfile = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\18-08-14\\BC\\tracks.txt";
    std::vector<std::vector<float> > gpoints;
    std::vector<float> p1; p1.resize(2); p1[0] = 622; p1[1] = 290; gpoints.push_back(p1);
    std::vector<float> p2; p2.resize(2); p2[0] = 1312; p2[1] = 290; gpoints.push_back(p2);
    std::vector<float> p3; p3.resize(2); p3[0] = 622; p3[1] = 810; gpoints.push_back(p3);
    std::vector<float> p4; p4.resize(2); p4[0] = 1316; p4[1] = 810; gpoints.push_back(p4);
    std::vector<int> gpointsIdx;
    gpointsIdx.push_back(11);
    gpointsIdx.push_back(12);
    gpointsIdx.push_back(9);
    gpointsIdx.push_back(10);
    std::string outputDir = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\18-08-14\\BC\\stats_line\\";
    extractStats(tracksfile, gpoints, gpointsIdx, outputDir, 5938);
}

void movie18c2(){

    std::string tracksfile = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\18-08-14\\C\\tracks.txt";
    std::vector<std::vector<float> > gpoints;
    std::vector<float> p1; p1.resize(2); p1[0] = 622; p1[1] = 290; gpoints.push_back(p1);
    std::vector<float> p2; p2.resize(2); p2[0] = 1312; p2[1] = 290; gpoints.push_back(p2);
    std::vector<float> p3; p3.resize(2); p3[0] = 622; p3[1] = 810; gpoints.push_back(p3);
    std::vector<float> p4; p4.resize(2); p4[0] = 1316; p4[1] = 810; gpoints.push_back(p4);
    std::vector<int> gpointsIdx;
    gpointsIdx.push_back(3);
    gpointsIdx.push_back(4);
    gpointsIdx.push_back(1);
    gpointsIdx.push_back(2);
    std::string outputDir = "C:\\Users\\sprigent\\Documents\\Guichet\\tracking_poisson\\session2\\full\\output_tracks\\18-08-14\\C\\stats_line\\";
    extractStats(tracksfile, gpoints, gpointsIdx, outputDir, 5995);
}


int main(int argc, char* argv[])
{
    try{

        /*
        movie14b2();
        movie14bc2();
        movie14c2();
        movie15b2();
        movie15bc2();
        movie15c2();
        movie16b2();
        movie16bc2();
        movie16c2();
        movie17b2();
        movie17bc2();
        movie17c2();
        movie18b2();
        movie18bc2();
        movie18c2();
*/

        movie17b();
        movie17c();
        movie18b();
        movie18c();
        movie19b();
        movie19c();
        movie20b();
        movie20c();
        movie21b();
        movie21c();


    }
    catch (blException ex){
        std::cout << ex.what() << std::endl;
        return 1;
    }
}
