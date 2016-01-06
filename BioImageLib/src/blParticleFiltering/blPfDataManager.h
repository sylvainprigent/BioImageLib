#pragma once

#include <blCore>
#include "blParticleFilteringExport.h"

/// \class blPfDataManager
/// \brief Class allowing to load and store input data for particle filtering
class BLPARTICLEFILTERING_EXPORT blPfDataManager{

public:
    /// \fn blPfDataManager(std::vector<float> &objectList, std::vector<std::string> &framesPathList);
    /// \brief Constructor for single object
    /// \param[in] objectList std::vector containing the single object state
    /// \param[in] framesPathList List of the frames path
    blPfDataManager(std::vector<float> &objectList, std::vector<std::string> &framesPathList);
    /// \fn blPfDataManager(std::vector<float> &objectList, std::vector<std::string> &framesname, std::string framesRootPath);
    /// \brief Constructor for single object
    /// \param[in] objectList std::vector containing the single object state
    /// \param[in] framesname List of the frames names
    /// \param[in] framesRootPath Path of the folder containg all the frames
    blPfDataManager(std::vector<float> &objectList, std::vector<std::string> &framesname, std::string framesRootPath);
    /// \fn blPfDataManager(std::vector<std::vector<float> > &objectList, std::vector<std::string> &framesname, std::string framesRootPath);
    /// \brief Constructor for multiple object with only one starting frame
    /// \param[in] objectList std::vector containing the states for each object
    /// \param[in] framesname List of the frames names
    /// \param[in] framesRootPath Path of the folder containg all the frames
    blPfDataManager(std::vector<std::vector<float> > &objectList, std::vector<std::string> &framesname, std::string framesRootPath);
    /// \fn blPfDataManager(std::vector<std::vector<float> > &objectList, std::vector<std::string> &framesPathList);
    /// \param[in] objectList std::vector containing the states for each object
    /// \param[in] framesPathList List of the frames path
    blPfDataManager(std::vector<std::vector<float> > &objectList, std::vector<std::string> &framesPathList);
    /// \fn blPfDataManager(std::vector<std::vector<std::vector<float> > > &objectList, std::vector<std::string> &framesPathList);
    /// \brief Constructor for multiple objects at multiple frames
    /// \param[in] objectList std::vector constining for each frame, the list of objects states
    /// \param[in] framesPathList List of the frames path
    blPfDataManager(std::vector<std::vector<std::vector<float> > > &objectList, std::vector<std::string> &framesPathList);
    /// \fn blPfDataManager(std::vector<std::vector<std::vector<float> > > &objectList, std::vector<std::string> &framesname, std::string framesRootPath);
    /// \brief Constructor for multiple objects at multiple frames
    /// \param[in] objectList std::vector constining for each frame, the list of objects states
    /// \param[in] framesname List of the frames names
    /// \param[in] framesRootPath Path of the folder containg all the frames
    blPfDataManager(std::vector<std::vector<std::vector<float> > > &objectList, std::vector<std::string> &framesname, std::string framesRootPath);
    /// \fn void setMasks(std::vector<std::string> maskfile);
    /// \brief set a mask to exclude some part of the image.
    /// pixels with intensity 255 in the mask will be excluded from the search area
    /// \param[in] maskFile path of the mask image
    void setMasks(std::vector<std::string> maskfile);
    /// \fn  std::vector<float> getObjectsAt(int frameIndex, int objectIdx);
    /// \return the state of the object "objectIdx" at frame "frameIndex"
    std::vector<float> getObjectsAt(int frameIndex, int objectIdx);
    /// \fn std::vector<std::vector<float> > getObjectsAt(int frameIndex);
    /// \return std::vector containing the objects states at frame "frameIndex"
    std::vector<std::vector<float> > getObjectsAt(int frameIndex);
    /// \fn int GetObjectNumberAtFrame(int frameIdx);
    /// \return The number of objects at the frame "frameIdx"
    int getObjectNumberAtFrame(int frameIdx);
    /// \fn void RemoveMultipleTimeObject(int frameIdx, int objectIdx);
    /// \brief Method that remove the object "objectIdx" at frame
    /// "frameIdx" from the object list
    /// Warning: This method is not yet implemented
    void removeMultipleTimeObject(int frameIdx, int objectIdx);
    /// \fn int GetTotalFrameNumber();
    /// \return the number of frames in the frame list
    int getTotalFrameNumber();
    /// \fn bool Is3DImage();
    /// \return true if the image is 3D, false if the image is 2D
    bool is3DImage();
    /// \fn blImage* getFrameAt(int index);
    /// \param[in] index index of the frame to get
    /// \return a pointer to the image at "index" in the frame list.
    /// If the frame is already loaded, return the pointer; Otherwise
    /// load the image and return the pointer
    blImage* getFrameAt(int index);
    /// \fn blImage* getCurentFrame();
    /// \return a pointer to the curent (ie last loaded) frame
    blImage* getCurentFrame();
    /// \fn Float3DImage::Pointer get3DCurentFrame();
    /// \return a pointer to the curent (ie last loaded) frame
    /// \fn int GetFrameNl();
    /// \return the number of line in a frame
    int getFrameNl();
    /// \fn int GetFrameNc();
    /// \return the number of columns in a frame
    int getFrameNc();
    /// \fn int getFrameNz();
    /// \return the number of z in a frame
    int getFrameNz();
    /// \fn void loadAllFrames();
    /// \brief Load and store all the frames
    void loadAllFrames();
    /// \fn void loadFrame(int frameIdx);
    /// \brief Load and store the frame at "frameIdx"
    /// \param[in] frameIdx index of the frame to load
    void loadFrame(int frameIdx);
    /// \fn std::string getFrameNameAt(int frameIdx);
    /// \param[in] frameIdx index of the frame to get
    /// \return path of the frame at index "frameIdx"
    std::string getFrameNameAt(int frameIdx);
    /// \fn int getInputObjectSize();
    /// \return the number of object dimension
    ///         1: One single object
    ///         2: Objects on only one frame
    ///         3: Objects on each frames
    int getInputObjectSize();
    /// \fn blImage* getCurentMask();
    /// \return a pointer to the 3D mask
    blImage* getCurentMask();
    /// \fn blImage* loadMask(int frameIdx);
    /// \return a pointer to the last loaded 2D mask
    /// \fn blImage* loadMask(int frameIdx);
    /// \return a pointer to the 3D mask
    blImage* loadMask(int frameIdx);

    /// \fn bool useMask();
    /// \return 0 if no mask has been loaded, 1 otherwise
    bool useMask();

    /// \fn void loadInfoFromFirstImage();
    /// \brief Load the image information using the first image of the time series
    void loadInfoFromFirstImage();

private:
    std::vector<std::vector<std::vector<float> > > m_initialObjects; ///< List og input objects for each frames
    std::vector<blImage*> m_frames; ///< List of frames pointers for the 2D case
    blImage* m_curentFrame; ///< Pointer to the curent Frame for the 2D case
    int m_frameTotalNumber; ///< Number of frames
    bool m_is3D; ///< Allows to know if the image is 3D or 2D
    bool m_allFramesLoaded; ///< Allows to know if all the frames have been loaded
    int m_nl; ///< Number of lines in a frame
    int m_nc; ///< Number of colums in a frame
    int m_nz; ///< Number of z in a frame
    std::vector<std::string> m_framesPathList; ///< List of the frames full path
    std::vector<std::string> m_framesName; ///< List of the frames names
    int m_inputObjectSize; ///< which object in input (see desciption of GetInputObjectSize())
    blImage* m_mask; ///< pointer to mask to ignore some pixels
    bool m_useMask; ///< Allows to know if a mask is loaded
    std::vector<std::string> m_maskfiles; ///< list of the masks path for all the frames
};
