/// \file blAtTrack.h
/// \brief blAtTrack class
/// \author Sylvain Prigent
/// \version 0.1
/// \date 2013

#pragma once

#include "blAtConnection.h"
#include "blAssociationTrackingExport.h"
#include "blAssociationTrackingExport.h"

/// \class blAtTrack
/// \brief Class allowing to store a track of feature point tracking connections
/// this curent version can store only linear tracks (ie no tree)
class BLASSOCIATIONTRACKING_EXPORT blAtTrack
{
public:
    /// \fn blAtTrack();
    /// \brief Constructor
    blAtTrack();
    /// \fn ~blAtTrack();
    /// \brief Destructor
    ~blAtTrack();
    /// \fn blAtTrack(blAtConnection *initialConnection);
    /// \brief Constructor
    /// \param[in] initialConnection First connection of the track
    blAtTrack(blAtConnection *initialConnection);
    /// \fn void addConnection(blAtConnection *connection);
    /// \brief add a connection to the track
    /// \param[in] connection Pointer to the connection to add
    void addConnection(blAtConnection *connection);
    /// \fn int getTrackSize();
    /// \return the number of connections in the track
    unsigned int getTrackSize();
    /// \fn blAtConnection* getConnectionAt(int index);
    /// \param[in] index Index of the connection to get
    /// \return Pointer to the connection at "index"
    blAtConnection* getConnectionAt(int index);
    /// \fn blAtConnection* getLastConnection();
    /// \return A pointer to the last connection of the track
    blAtConnection* getLastConnection();
    /// \fn void clear();
    /// \brief Clear the track by removing all the connections. Memory is not free
    void clear();

private:
    std::vector<blAtConnection*> m_connections; ///< list of the connections in the track
};
