/// \file blProcessObserver.h
/// \brief blProcessObserver class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include <string>
#include <set>
#include "blCoreExport.h"

/// \class blProcessObserver
/// \brief class blProcessObserver implementing the observer for the patern Observer/Observable
/// for a blprocess
class BLCORE_EXPORT blProcessObserver{
public:
    /// \fn virtual void updateInfo(std::string data, int level = 1) = 0;
    /// \brief To send an information of level 1
    /// \param[in] data Information to be send
    /// \param[in] level Level of the information
    virtual void updateInfo(std::string data, int level = 1) = 0;
    /// \fn virtual void updateWarning(std::string message) = 0;
    /// \brief To send a warning
    /// \param[in] message Warning massage
    virtual void updateWarning(std::string message) = 0;
    /// \fn virtual void updateProgress(int progress) = 0;
    /// \brief To send the progress statue
    /// \param[in] progress Percentage of progress
    virtual void updateProgress(int progress) = 0;
};

/// \class blProcessObservable
/// \brief class implementing the Observable for the patern Observer/Observable
///  for a blprocess
class BLCORE_EXPORT blProcessObservable{
protected:
    std::set<blProcessObserver*> list_observers; ///< list of the pointers to observers

public:
    /// \fn void notify(std::string data) const;
    /// \brief To notify a level 1 info to observers
    /// \param[in] data Information to be send
    /// \param[in] level Level of the information
    void notify(std::string data, int level = 1) const;
    /// \fn void notifyProgress(int progress) const;
    /// \brief To notify the progress to observers
    /// \param[in] progress Percentage of progress
    void notifyProgress(int progress) const;
    /// \fn void notifyWarning(std::string message) const;
    /// \brief To notify a warning to observers
    /// \param[in] message Warning message
    void notifyWarning(std::string message) const;
    /// \fn void addObserver(Observer* observer);
    /// \brief To add an observer to this observable object
    /// \param[in] observer pointer to the observer to add
    void addObserver(blProcessObserver* observer);
    /// \fn void addObservers(set<Observer*> observers_list);
    /// \brief To add a list of observer to this observable object
    /// \param[in] observers_list list of pointers to the observer to add
    void addObservers(std::set<blProcessObserver*> observers_list);
    /// \brief void removeObserver(Observer* observer);
    /// \brief To remove an observer to this observable object
    /// \param[in] observer Pointer to the observer to remove
    void removeObserver(blProcessObserver* observer);
};

/// \class blProcessObserverCommandeLine
/// \brief Observer that write the observed signals in the console
class BLCORE_EXPORT blProcessObserverCommandeLine : public blProcessObserver{
public:
    /// \fn virtual void updateInfo(std::string data, int level = 1);
    /// \brief To send an information of level 1
    /// \param[in] data Information to be send
    /// \param[in] level Level of the information
    void updateInfo(std::string data, int level = 1);
    /// \fn virtual void updateProgress(int progress);
    /// \brief To send the progress statue
    /// \param[in] progress Percentage of progress
    void updateProgress(int progress);
    /// \fn virtual void updateWarning(std::string message);
    /// \brief To send a warning
    /// \param[in] message Warning massage
    void updateWarning(std::string message);
};

/// \class blProcessObserverNull
/// \brief Observer that only write warnings in the consol
class BLCORE_EXPORT blProcessObserverNull : public blProcessObserver{
public:
    /// \fn virtual void updateInfo(std::string data, int level = 1);
    /// \brief To send an information of level 1
    /// \param[in] data Information to be send
    /// \param[in] level Level of the information
    void updateInfo(std::string data, int level=1);
    /// \fn virtual void updateProgress(int progress) ;
    /// \brief To send the progress statue
    /// \param[in] progress Percentage of progress
    void updateProgress(int progress);
    /// \fn virtual void updateWarning(std::string message);
    /// \brief To send a warning
    /// \param[in] message Warning massage
    void updateWarning(std::string message);
};
