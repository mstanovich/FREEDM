////////////////////////////////////////////////////////////////////////////////
/// @file           IWorkerInterface.hpp
///
/// @author         Thomas Roth <tprfh7@mst.edu>
///
/// @compiler       C++
///
/// @project        Missouri S&T Power Research Group
///
/// @description    
///
/// @functions  
///
/// @license
/// These source code files were created at the Missouri University of Science
/// and Technology, and are intended for use in teaching or research. They may
/// be freely copied, modified and redistributed as long as modified versions
/// are clearly marked as such and this notice is not removed.
///
/// Neither the authors nor Missouri S&T make any warranty, express or implied,
/// nor assume any legal responsibility for the accuracy, completeness or
/// usefulness of these files or any information distributed with these files.
///
/// Suggested modifications or questions about these files can be directed to
/// Dr. Bruce McMillin, Department of Computer Science, Missouri University of
/// Science and Technology, Rolla, MO 65401 <ff@mst.edu>.
////////////////////////////////////////////////////////////////////////////////

#ifndef I_WORKER_INTERFACE_HPP
#define I_WORKER_ITNERFACE_HPP

#include <string>
#include <iostream>

#include <boost/ref.hpp>
#include <boost/asio.hpp>

#include "logger.hpp"
#include "IWorker.hpp"
#include "CLineServer.hpp"

CREATE_EXTERN_STD_LOGS()

namespace simserv {

/// Defines a threaded line server interface
class IWorkerInterface : public IWorker
{
public:
    /// Creates a worker interface on a given port number
    IWorkerInterface( unsigned short p_port );
    
    /// Starts the line server
    virtual void Work();
    
    /// Message handler function used by the line server
    virtual std::string HandleMessage( const std::string & p_message ) = 0;
    
    /// Deconstructor for derived classes
    virtual ~IWorkerInterface() {}
private:
    /// ioservice used by the line server
    boost::asio::io_service m_service;
    
    /// shared pointer to the line server
    CLineServer::TPointer m_server;
};

} // namespace simserv

#endif // I_WORKER_INTERFACE_HPP
