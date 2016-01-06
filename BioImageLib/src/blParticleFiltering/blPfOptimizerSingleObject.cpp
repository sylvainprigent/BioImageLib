#include "blPfOptimizerSingleObject.h"

#include "itkRescaleIntensityImageFilter.h"
#include <blCastStacks/blProjectionImage.h>
#include <blGeometry>

using namespace std;

// ////////////////////////////////////////////////// //
//                blPfOptimizerSingleObject                      //
// ////////////////////////////////////////////////// //
blPfOptimizerSingleObject::blPfOptimizerSingleObject(blPfModelInterface *particleModel, int startingFrameIdx, int objectIdx):
    blPfOptimizerInterface(particleModel, startingFrameIdx, objectIdx){
}

blPfOptimizerSingleObject::blPfOptimizerSingleObject(blPfModelInterface *particleModel, int particleNumber, int resamplingThreshold, string resamplingMethod, int startingFrameIdx, int objectIdx):
    blPfOptimizerInterface(particleModel, particleNumber, resamplingThreshold, resamplingMethod, startingFrameIdx, objectIdx){
}

void blPfOptimizerSingleObject::run(){
    // main loop
    int frameNumber = m_particleModel->getFrameNumber();
    blPfState* selectedParticleState;
    for (int t=1 ; t<frameNumber ; ++t){
        this->notify("process frame " + blCast::to_string(t));
        cout << "set curent frame" << endl;
        m_particleModel->setCurentFrameIdx(t); // to load the data
        cout << "one step" << endl;
        selectedParticleState = oneStepParticleFilter(t);
        m_particleModel->setSelectedPreviousState(selectedParticleState);
        //cout << "save states" << endl;
        //saveParticulesPositions(selectedParticleState);
    }
}

blPfTrack *blPfOptimizerSingleObject::getTrack(){
    return m_track;
}

void blPfOptimizerSingleObject::saveParticulesPositions(blPfState* selectedParticle){
    int curentframe = m_particleModel->getCurentFrameIdx();

    Float2DImage::Pointer frame;
    if (!m_particleModel->getDataManager()->is3DImage()){
        // 1- get curent frame
        cout << "blPfOptimizerSingleObject::SaveParticulesPositions, get curent frame" << endl;
        frame = m_particleModel->getDataManager()->getFrameAt(curentframe)->itkFloat2DImagePointer();
        cout << "blPfOptimizerSingleObject::SaveParticulesPositions, get curent frame end" << endl;
    }
    else{
        cout << "blPfOptimizerSingleObject::SaveParticulesPositions, 3d case" << endl;
        blZProjection projection;
        projection.setInput(m_particleModel->getDataManager()->getFrameAt(curentframe) );
        projection.setProjectionType(blZProjection::Maximum);
        projection.run();
        frame = projection.output()->itkFloat2DImagePointer();
        //frame = blIntegrateImage::Integrate3DToMip(m_particleModel->getDataManager()->get3DFrameAt(curentframe));
    }

    // 2- create rgb image
    // 2.1- normalize intensity
    typedef itk::RescaleIntensityImageFilter< Float2DImage, Float2DImage > RescaleFilterType;
    RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
    rescaleFilter->SetInput(frame);
    rescaleFilter->SetOutputMinimum(0);
    rescaleFilter->SetOutputMaximum(255);
    rescaleFilter->Update();
    Float2DImage::Pointer imageFrame = rescaleFilter->GetOutput();
    int nl = frame->GetLargestPossibleRegion().GetSize()[0];
    int nc = frame->GetLargestPossibleRegion().GetSize()[1];


    FloatColor2DImage::Pointer resImage = blImageCreate::createNewFloatColor2DImage(nl, nc);

    // 2.2- copy the input image to the rgb output
    typedef itk::ImageRegionIterator<FloatColor2DImage> IteratorType;
    typedef itk::ImageRegionIterator<Float2DImage> IteratorTypeFloat;
    IteratorType iteratorResImage(resImage, resImage->GetLargestPossibleRegion());
    IteratorTypeFloat iteratorInitImage(imageFrame, imageFrame->GetLargestPossibleRegion());

    itk::RGBPixel<float> pixel;
    while(!iteratorInitImage.IsAtEnd()){
        pixel[0] = iteratorInitImage.Get();
        pixel[1] = pixel[0]; pixel[2] = pixel[0];
        iteratorResImage.Set(pixel);
        ++iteratorResImage;
        ++iteratorInitImage;
    }

    // plot particules
    pixel[0] = 255; pixel[1] = 0; pixel[2] = 0;
    FloatColor2DImage::IndexType index2d;
    for (int i = 0 ; i < m_Ns ; ++i){
        index2d[0] = m_particles[i]->getStateAt(m_curentFrameIdx)->getStateAt(1);
        index2d[1] = m_particles[i]->getStateAt(m_curentFrameIdx)->getStateAt(2);
        resImage->SetPixel(index2d, pixel);
    }

    // plot the selected particle
    pixel[0] = 0; pixel[1] = 0; pixel[2] = 255;
    index2d[0] = selectedParticle->getStateAt(1);
    index2d[1] = selectedParticle->getStateAt(2);
    resImage->SetPixel(index2d, pixel);



    // save result
    string path = "C:\\Users\\prigent\\Documents\\Documents\\experiments\\test_particle_filtering\\inter\\particles_" + blCast::to_string(curentframe) + ".tif";
    blImageIO::saveFloatColor2DImage(path, resImage, true);
}

void blPfOptimizerSingleObject::saveTrackRepresentation(string imagePath){

    // get the image 2D or 3D
    bool is3Dimage = m_particleModel->getDataManager()->is3DImage();

    if (is3Dimage){
        cout << "blPfOptimizerSingleObject::SaveTrackRepresentation -> 3D case" << endl;
        // Create a 3d image to save the representation
        int nl = m_particleModel->getDataManager()->getFrameNl();
        int nc = m_particleModel->getDataManager()->getFrameNc();
        FloatColor3DImage::Pointer outputImage = blImageCreate::createNewFloatColor3DImage(nl, nc, m_track->getTrackSize());

        // get the ploting color
        vector<int> rgb = blColor::GetRandRGB();

        // plot
        for (int i = 0 ; i < m_track->getTrackSize() ; ++i){
            // copy the frame
            FloatColor3DImage::IndexType index3d;
            Float2DImage::IndexType index2d;
            FloatColor3DImage::PixelType pixel3d;
            index3d[2] = i;

            blZProjection projection;
            projection.setInput( m_particleModel->getDataManager()->getFrameAt(i) );
            projection.setProjectionType(blZProjection::Maximum);
            projection.run();
            Float2DImage::Pointer curentFramePointer = projection.output()->itkFloat2DImagePointer();

            //Float2DImage::Pointer curentFramePointer = blIntegrateImage::Integrate3DToMip( m_particleModel->GetDataManager()->Get3DFrameAt(i) );

            for (int m = 0 ; m < nl ; ++m){
                for (int n = 0 ; n < nc ; ++n){
                    index3d[0] = m; index3d[1] = n;
                    index2d[0] = m; index2d[1] = n;
                    float value = curentFramePointer->GetPixel(index2d);
                    pixel3d[0] = value; pixel3d[1] = value; pixel3d[2] = value;
                    outputImage->SetPixel(index3d, pixel3d);
                }
            }
            // plot the track till i
            pixel3d[0] = rgb[0]; pixel3d[1] = rgb[1]; pixel3d[2] = rgb[2];
            for (int t = 1 ; t <= i ; ++t){
                // Cacluate the line points
                vector<int> px, py;
                int x1 = m_track->getStateAt(t-1)->getStateAt(1);
                int y1 = m_track->getStateAt(t-1)->getStateAt(2);
                int x2 = m_track->getStateAt(t)->getStateAt(1);
                int y2 = m_track->getStateAt(t)->getStateAt(2);
                blMathGeometry::Calculate2DLineCoordinates(x1,y1, x2, y2, px, py);

                // plot the points
                for (unsigned int j = 0 ; j < px.size() ; ++j){
                    index3d[0] = px[j];
                    index3d[1] = py[j];
                    outputImage->SetPixel(index3d, pixel3d);
                }
            }
        }
        // save the 3d image
        blImageIO::saveFloatColor3DImage(imagePath, outputImage, true);
    }
    else{
        cout << "blPfOptimizerSingleObject::SaveTrackRepresentation -> 2D case" << endl;
        // Create a 3d image to save the representation
        int nl = m_particleModel->getDataManager()->getFrameNl();
        int nc = m_particleModel->getDataManager()->getFrameNc();
        FloatColor3DImage::Pointer outputImage = blImageCreate::createNewFloatColor3DImage(nl, nc, m_track->getTrackSize());

        vector<int> rgb = blColor::GetRandRGB();
        cout << "rgb = " << rgb[0] << ", " << rgb[1] << ", " << rgb[2] << endl;
        for (int i = 0 ; i < m_track->getTrackSize() ; ++i){
            // copy the frame
            FloatColor3DImage::IndexType index3d;
            Float2DImage::IndexType index2d;
            FloatColor3DImage::PixelType pixel3d;
            index3d[2] = i;
            Float2DImage::Pointer curentFramePointer = m_particleModel->getDataManager()->getFrameAt(i)->itkFloat2DImagePointer();
            for (int m = 0 ; m < nl ; ++m){
                for (int n = 0 ; n < nc ; ++n){
                    index3d[0] = m; index3d[1] = n;
                    index2d[0] = m; index2d[1] = n;
                    float value = curentFramePointer->GetPixel(index2d);
                    pixel3d[0] = value; pixel3d[1] = value; pixel3d[2] = value;
                    outputImage->SetPixel(index3d, pixel3d);
                }
            }

            // plot the track till i
            pixel3d[0] = rgb[0]; pixel3d[1] = rgb[1]; pixel3d[2] = rgb[2];
            for (int t = 1 ; t <= i ; ++t){
                // Cacluate the line points
                vector<int> px, py;
                int x1 = m_track->getStateAt(t-1)->getStateAt(1);
                int y1 = m_track->getStateAt(t-1)->getStateAt(2);
                int x2 = m_track->getStateAt(t)->getStateAt(1);
                int y2 = m_track->getStateAt(t)->getStateAt(2);
                blMathGeometry::Calculate2DLineCoordinates(x1,y1, x2, y2, px, py);

                // plot the points
                for (unsigned int j = 0 ; j < px.size() ; ++j){
                    index3d[0] = px[j];
                    index3d[1] = py[j];
                    outputImage->SetPixel(index3d, pixel3d);
                }
            }
        }
        // save the 3d image
        blImageIO::saveFloatColor3DImage(imagePath, outputImage, true);
    }

}
