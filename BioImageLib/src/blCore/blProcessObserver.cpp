/// \file blProcessObserver.cpp
/// \brief blProcessObserver class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#include "blProcessObserver.h"
#include <iostream>
using namespace std;

// /////////////////////////////////////////////////// //
//                 Observable
// /////////////////////////////////////////////////// //
void blProcessObservable::notify(string data, int level) const{
    // Notify all observers
    for (set<blProcessObserver*>::const_iterator it = list_observers.begin(); it != list_observers.end(); ++it)
        (*it)->updateInfo(data, level);
}

void blProcessObservable::notifyProgress(int progress) const{
    // Notify all observers
    for (set<blProcessObserver*>::const_iterator it = list_observers.begin(); it != list_observers.end(); ++it)
        (*it)->updateProgress(progress);
}

void blProcessObservable::notifyWarning(string message) const{
    for (set<blProcessObserver*>::const_iterator it = list_observers.begin(); it != list_observers.end(); ++it)
        (*it)->updateWarning(message);
}

void blProcessObservable::addObserver(blProcessObserver* observer){
    list_observers.insert(observer);
}

void blProcessObservable::removeObserver(blProcessObserver* observer){
    list_observers.erase(observer);
}

void blProcessObservable::addObservers(set<blProcessObserver*> observers_list){
    for (set<blProcessObserver*>::const_iterator it = observers_list.begin(); it != observers_list.end(); ++it)
        list_observers.insert(*it);
}

// /////////////////////////////////////////////////// //
//                 blProcessObserverCommandeLine
// /////////////////////////////////////////////////// //
void blProcessObserverCommandeLine::updateInfo(string data, int level){
    cout << "level " << level << ": " << data << endl;
}

void blProcessObserverCommandeLine::updateProgress(int progress){
    cout << "Progress = " <<  progress << "%" << endl;
}

void blProcessObserverCommandeLine::updateWarning(string message){
    cout << "--------- WARNING ---------" << endl;
    cout << message << endl;
    cout << "---------------------------" << endl;
}

// /////////////////////////////////////////////////// //
//                 NullObserver
// /////////////////////////////////////////////////// //
void blProcessObserverNull::updateInfo(string, int){
    //cout << data << endl;
}

void blProcessObserverNull::updateProgress(int){
    //cout << "Progress = " <<  progress << "%" << endl;
}

void blProcessObserverNull::updateWarning(string message){
    cout << "--------- WARNING ---------" << endl;
    cout << message << endl;
    cout << "---------------------------" << endl;
}
