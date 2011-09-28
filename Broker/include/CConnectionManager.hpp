////////////////////////////////////////////////////////////////////
/// @file      CConnection.hpp
///
/// @author    Derek Ditch <derek.ditch@mst.edu>
///            Stephen Jackson <scj7t4@mst.edu>
///            Christopher M. Kohloff <chris@kohloff.com>
///
/// @compiler  C++
///
/// @project   FREEDM DGI
///
/// @description Connection manager, for the connections.
///
/// @license
/// These source code files were created at as part of the
/// FREEDM DGI Subthrust, and are
/// intended for use in teaching or research.  They may be
/// freely copied, modified and redistributed as long
/// as modified versions are clearly marked as such and
/// this notice is not removed.
///
/// Neither the authors nor the FREEDM Project nor the
/// National Science Foundation
/// make any warranty, express or implied, nor assumes
/// any legal responsibility for the accuracy,
/// completeness or usefulness of these codes or any
/// information distributed with these codes.
///
/// Suggested modifications or questions about these codes
/// can be directed to Dr. Bruce McMillin, Department of
/// Computer Science, Missour University of Science and
/// Technology, Rolla, /// MO  65409 (ff@mst.edu).
///
////////////////////////////////////////////////////////////////////
#ifndef CONNECTIONMANAGER_HPP
#define CONNECTIONMANAGER_HPP

#include "IHandler.hpp"
#include "CGlobalConfiguration.hpp"
#include "CListener.hpp"
#include "CConnection.hpp"

#include "utility/uuid.hpp"
#include "templates/Singleton.hpp"
#include "types/remotehost.hpp"

#include <set>
#include <string>
#include <map>
#include <exception>
#include <boost/foreach.hpp>
#include <boost/bimap.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace freedm {
namespace broker {

class CReliableConnection;
class CListener;
class CConnection;

/// Manages open connections so that they may be cleanly stopped
class CConnectionManager
    : public templates::Singleton<CConnectionManager>
{
public:
    /// Typedef for the map which handles uuid to hostname
    typedef std::map<std::string, remotehost> hostnamemap;
    
    /// Typedef for the map which handles uuid to connection 
    typedef boost::bimap<std::string, CConnection::ConnectionPtr> connectionmap;

    /// Initialize the connection manager with the node uuid.
    CConnectionManager();

    /// Connection manager teardown.
    ~CConnectionManager() {  };

    /// Add the specified connection to the manager and start it.
    void Start(CListener::ConnectionPtr c);
 
    /// Place a hostname and uuid into the hostname / uuid map.
    void PutHostname(std::string u_, std::string host_, std::string port);
   
    /// Place a hostname and uuid into the hostname / uuid map.
    void PutHostname(std::string u_, remotehost host_);
 
    /// Register a connection with the manager once it has been built.
    void PutConnection(std::string uuid, CConnection::ConnectionPtr c);

    /// Stop the specified connection.
    void Stop(CConnection::ConnectionPtr c);
    
    /// Stop the specified connection
    void Stop(CListener::ConnectionPtr c);

    /// Stop all connections.
    void StopAll();

    /// Get The UUID
    std::string GetUUID() { return m_uuid; };

    /// Get The Hostname
    remotehost GetHostname() { return m_hostname; };
    
    /// Get the hostname from the UUID.
    remotehost GetHostnameByUUID( std::string uuid ) const; 

    /// Fetch a connection pointer via UUID
    CConnection::ConnectionPtr GetConnectionByUUID( std::string uuid_,  boost::asio::io_service& ios );

    /// An iterator to the beginning of the hostname map
    hostnamemap::iterator GetHostnamesBegin() { return m_hostnames.begin(); };

    /// An iterator to the end of the hostname map.
    hostnamemap::iterator GetHostnamesEnd() { return m_hostnames.end(); };

    /// An iterator to the specified hostname.
    hostnamemap::iterator GetHostname(std::string uuid) { return m_hostnames.find(uuid); };

    /// Iterator to the beginning of the connection map.
    connectionmap::iterator GetConnectionsBegin() { return m_connections.begin(); };

    /// Iterator to the end of the connections map.
    connectionmap::iterator GetConnectionsEnd() { return m_connections.end(); };
    
    // Transient Network Simulation
    /// Load a network configuration & apply it.
    void LoadNetworkConfig();
    
    /// Unknown uuid exception.
    class EUnknownUUID : public std::exception
    {
        virtual const char* what() const throw()
        {
            return "No UUID by that name in referenced table.";
        }
    };
private:
    /// Mapping from uuid to hostname.
    hostnamemap m_hostnames;
    /// Hostname of this node.
    remotehost m_hostname;
    /// Forward map (UUID->Connection)
    connectionmap   m_connections;
    /// Incoming messages channel
    CListener::ConnectionPtr m_inchannel;
    /// Node UUID
    std::string m_uuid;
    /// Mutex for protecting the handler maps above
    boost::mutex m_Mutex;       
    /// Mutex that prevents two connections being constructed simultaenously
    boost::mutex m_GetConnectionMutex;
};

} // namespace broker
} // namespace freedm

#endif // CONNECTIONMANAGER_HPP
