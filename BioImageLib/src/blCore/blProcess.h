/// \file blProcess.h
/// \brief blProcess class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2014

#pragma once

#include "blProcessObserver.h"

/// \class blProcess
/// \brief Class defining the interface of a process class
/// It heritates from blProcessObservable to enable the
/// use of progress messages
class blProcess : public blProcessObservable{

public:
    /// \fn blProcess();
    /// \brief Contructor
    blProcess();
    /// \fn virtual ~blProcess();
    /// \brief Destructor
    virtual ~blProcess();

public:
    /// \fn virtual std::string name();
    /// \return a name identifying the process
    virtual std::string name();
};
