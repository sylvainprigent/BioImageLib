#include "blPfDataManager.h"
#include "blCastStacks/blCastImageToFloat.h"

using namespace std;

blPfDataManager::blPfDataManager(vector<float> &objectList, vector<string> &framesPathList){

    m_inputObjectSize = 1;
    m_initialObjects.resize(1);
    m_initialObjects[0].resize(1);
    m_initialObjects[0][0] = objectList;
    m_framesPathList = framesPathList;
    m_frameTotalNumber = m_framesPathList.size();
    m_allFramesLoaded = false;
    m_useMask = false;
}

blPfDataManager::blPfDataManager(vector<float> &objectList, vector<string> &framesname, string framesRootPath){

    m_inputObjectSize = 1;
    m_initialObjects.resize(1);
    m_initialObjects[0].resize(1);
    m_initialObjects[0][0] = objectList;
    for (unsigned int i = 0 ; i < framesname.size() ; ++i){
        m_framesPathList.push_back(framesRootPath + framesname[i]);
    }
    m_framesName = framesname;
    m_frameTotalNumber = m_framesPathList.size();
    m_allFramesLoaded = false;
    m_useMask = false;
}

blPfDataManager::blPfDataManager(vector<vector<float> > &objectList, vector<string> &framesPathList){

    m_inputObjectSize = 2;
    m_initialObjects.resize(1);
    m_initialObjects[0] = objectList;
    m_framesPathList = framesPathList;
    m_frameTotalNumber = m_framesPathList.size();
    m_allFramesLoaded = false;
    m_useMask = false;
}

blPfDataManager::blPfDataManager(vector<vector<float> > &objectList, vector<string> &framesname, string framesRootPath){
    m_inputObjectSize = 2;
    m_initialObjects.resize(1);
    m_initialObjects[0] = objectList;
    for (unsigned int i = 0 ; i < framesname.size() ; ++i){
        m_framesPathList.push_back(framesRootPath + framesname[i]);
    }
    m_framesName = framesname;
    m_frameTotalNumber = m_framesPathList.size();
    m_allFramesLoaded = false;
    m_useMask = false;

    loadInfoFromFirstImage();

}

void blPfDataManager::loadInfoFromFirstImage(){
    // load the first frame to get the image info
    blImageIOInfo info;
    info.updateInfo(m_framesPathList[0]);
    if (info.dimension() == 2){
        m_is3D = false;
        //m_curentFrame = blImageIO::loadToFloat2DImage(m_framesPathList[0]);
        //m_nl = m_curentFrame->GetLargestPossibleRegion().GetSize()[0];
        //m_nc = m_curentFrame->GetLargestPossibleRegion().GetSize()[1];
    }
    else{
        m_is3D = true;
        //m_curentFrame3D = blImageIO::loadToFloat3DImage(m_framesPathList[0]);
        //m_nl = m_curentFrame3D->GetLargestPossibleRegion().GetSize()[0];
        //m_nc = m_curentFrame3D->GetLargestPossibleRegion().GetSize()[1];
        //m_nz = m_curentFrame3D->GetLargestPossibleRegion().GetSize()[2];
    }

    m_curentFrame = new blImage(m_framesPathList[0]);
    blImageSize size = m_curentFrame->imageSize();
    m_nl = size.xSize();
    m_nc = size.ySize();
    m_nz = size.zSize();
}

blPfDataManager::blPfDataManager(vector<vector<vector<float> > > &objectList, vector<string> &framesPathList){

    m_inputObjectSize = 3;
    m_initialObjects = objectList;
    m_framesPathList = framesPathList;
    m_frameTotalNumber = m_framesPathList.size();
    m_allFramesLoaded = false;
    m_useMask = false;


}

blPfDataManager::blPfDataManager(vector<vector<vector<float> > > &objectList, vector<string> &framesname, string framesRootPath){

    m_inputObjectSize = 3;
    m_initialObjects = objectList;
    for (unsigned int i = 0 ; i < framesname.size() ; ++i){
        m_framesPathList.push_back(framesRootPath + framesname[i]);
    }
    m_framesName = framesname;
    m_frameTotalNumber = m_framesPathList.size();
    m_allFramesLoaded = false;
    m_useMask = false;

    loadInfoFromFirstImage();
}

void blPfDataManager::setMasks(vector<string> maskfile){
    m_useMask = true;
    m_maskfiles = maskfile;
}


//Float3DImage::Pointer blPfDataManager::getCurent3DMask(){
//    return m_mask3D;
//}

//Float2DImage::Pointer blPfDataManager::getCurent2DMask(){
//    return m_mask2D;
//}

blImage* blPfDataManager::getCurentMask(){
    return m_mask;
}

//Float3DImage::Pointer blPfDataManager::load3DMask(int frameIdx){
//    m_mask3D = blImageIO::loadToFloat3DImage(m_maskfiles[frameIdx]);
//    return m_mask3D;
//}

//Float2DImage::Pointer blPfDataManager::load2DMask(int frameIdx){
//    m_mask2D = blImageIO::loadToFloat2DImage(m_maskfiles[frameIdx]);
//    return m_mask2D;
//}

blImage* blPfDataManager::loadMask(int frameIdx){
    m_mask = new blImage(m_maskfiles[frameIdx]);
    return m_mask;
}

bool blPfDataManager::useMask(){
    return m_useMask;
}

vector<float> blPfDataManager::getObjectsAt(int frameIndex, int objectIdx){
    return m_initialObjects[frameIndex][objectIdx];
}

vector<vector<float> > blPfDataManager::getObjectsAt(int frameIndex){
    return m_initialObjects[frameIndex];
}

void blPfDataManager::removeMultipleTimeObject(int frameIdx, int objectIdx){
    m_initialObjects[frameIdx].erase(m_initialObjects[frameIdx].begin() + objectIdx);
}

int blPfDataManager::getObjectNumberAtFrame(int frameIdx){
    return m_initialObjects[frameIdx].size();
}

void blPfDataManager::loadAllFrames(){
    m_allFramesLoaded = true;

    for (unsigned int i = 0 ; i < m_framesPathList.size() ; ++i){
        blImage* temp = new blImage(m_framesPathList[i]);
        blCastImageToFloat caster;
        caster.setInput(temp);
        caster.run();
        m_frames.push_back( caster.output() );
    }
    blImageSize sizeIm = m_frames[0]->imageSize();
    m_nl = sizeIm.xSize();
    m_nc = sizeIm.ySize();
    m_nz = sizeIm.zSize();


    if (m_nz == 1){
        m_is3D = false;
    }
    else{
        m_is3D = true;
    }



    //    blImageIOInfo info;
    //    info.updateInfo(m_framesPathList[0]);
    //    if (info.dimension() == 2){
    //        m_is3D = false;
    //        for (unsigned int i = 0 ; i < m_framesPathList.size() ; ++i){
    //            m_frames.push_back(blImageIO::loadToFloat2DImage(m_framesPathList[i]));
    //        }
    //        m_nl = m_frames[0]->GetLargestPossibleRegion().GetSize()[0];
    //        m_nc = m_frames[0]->GetLargestPossibleRegion().GetSize()[1];
    //    }
    //    else{
    //        m_is3D = true;
    //        for (unsigned int i = 0 ; i < m_framesPathList.size() ; ++i){
    //            m_frames3D.push_back(blImageIO::loadToFloat3DImage(m_framesPathList[i]));
    //        }
    //        m_nl = m_frames[0]->GetLargestPossibleRegion().GetSize()[0];
    //        m_nc = m_frames[0]->GetLargestPossibleRegion().GetSize()[1];
    //        m_nz = m_frames[0]->GetLargestPossibleRegion().GetSize()[2];
    //    }
}

void blPfDataManager::loadFrame(int frameIdx){


    m_curentFrame =  new blImage(m_framesPathList[frameIdx]);
    blCastImageToFloat caster;
    caster.setInput(m_curentFrame);
    caster.run();
    m_curentFrame = caster.output();


    blImageSize sizeIm = m_curentFrame->imageSize();
    m_nl = sizeIm.xSize();
    m_nc = sizeIm.ySize();
    m_nz = sizeIm.zSize();

    if (m_nz == 1){
        m_is3D = false;
    }
    else{
        m_is3D = true;
    }


    //    blImageIOInfo info;
    //    info.updateInfo(m_framesPathList[frameIdx]);
    //    if (info.dimension() == 2){
    //        m_is3D = false;
    //        m_curentFrame = blImageIO::loadToFloat2DImage(m_framesPathList[frameIdx]);
    //        m_nl = m_curentFrame->GetLargestPossibleRegion().GetSize()[0];
    //        m_nc = m_curentFrame->GetLargestPossibleRegion().GetSize()[1];
    //    }
    //    else{
    //        m_is3D = true;
    //        m_curentFrame3D = blImageIO::loadToFloat3DImage(m_framesPathList[frameIdx]);
    //        m_nl = m_curentFrame3D->GetLargestPossibleRegion().GetSize()[0];
    //        m_nc = m_curentFrame3D->GetLargestPossibleRegion().GetSize()[1];
    //        m_nz = m_curentFrame3D->GetLargestPossibleRegion().GetSize()[2];
    //    }
}

int blPfDataManager::getTotalFrameNumber(){
    return m_frameTotalNumber;
}

bool blPfDataManager::is3DImage(){
    blImageIOInfo info;
    info.updateInfo(m_framesPathList[0]);
    if (info.dimension() == 2){
        m_is3D = false;
    }
    else{
        m_is3D = true;
    }

    return m_is3D;
}

//Float2DImage::Pointer blPfDataManager::get2DFrameAt(int index){
//    if (m_allFramesLoaded){
//        return m_frames[index];
//    }
//    else{
//        loadFrame(index);
//        return m_curentFrame;
//    }
//}

//Float3DImage::Pointer blPfDataManager::get3DFrameAt(int index){
//    if (m_allFramesLoaded){
//        return m_frames3D[index];
//    }
//    else{
//        loadFrame(index);
//        return m_curentFrame3D;
//    }
//}

blImage* blPfDataManager::getFrameAt(int index){
    if (m_allFramesLoaded){
        return m_frames[index];
    }
    else{
        loadFrame(index);
        return m_curentFrame;
    }
}



string blPfDataManager::getFrameNameAt(int frameIdx){
    return m_framesName[frameIdx];
}

//Float2DImage::Pointer blPfDataManager::get2DCurentFrame(){
//    return m_curentFrame;
//}

//Float3DImage::Pointer blPfDataManager::get3DCurentFrame(){
//    return m_curentFrame3D;
//}

blImage* blPfDataManager::getCurentFrame(){
    return m_curentFrame;
}

int blPfDataManager::getFrameNl(){
    return m_nl;
}

int blPfDataManager::getFrameNc(){
    return m_nc;
}

int blPfDataManager::getFrameNz(){
    return m_nz;
}

int blPfDataManager::getInputObjectSize(){
    return m_inputObjectSize;
}
