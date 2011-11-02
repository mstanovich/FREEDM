////////////////////////////////////////////////////////////////////////////////
/// @file           CLineClient.hpp
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

#ifndef C_LINE_CLIENT_HPP
#define C_LINE_CLIENT_HPP

#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace freedm {
namespace simserv {

/// Simulation line client able to GET and SET values for devices
class CLineClient : private boost::noncopyable
{
public:
    /// Shared pointer to a line client instance
    typedef boost::shared_ptr<CLineClient> TPointer;
    
    /// Creates a shared simulation line client instance
    static TPointer Create( boost::asio::io_service & p_ios );

    /// Sets the remote simulation server hostname
    void SetHostname( const std::string & p_hostname );
    
    /// Sets the remote simulation server port number
    void SetPort( const std::string & p_port );
    
    /// Send a request to get the current value of a device setting
    std::string Get( const std::string & p_device, const std::string & p_key );
    
    /// Send a request to set the value of a device setting
    void Set( const std::string & p_device, const std::string & p_key,
            const std::string & p_value );
private:
    /// Creates a simulation line client instance
    CLineClient( boost::asio::io_service & p_ios );

    /// Creates a socket connection to the remote simulation server
    void Connect();
    
    /// Sends a request to the remote simulation server
    std::string SendRequest( const std::string & p_message );

    /// Socket connectio nto the simulation line server
    boost::asio::ip::tcp::socket m_socket;
    
    /// Simulation line server hostname
    std::string m_hostname;
    
    /// Simulation line server port number
    std::string m_port;
};

} // namespace simserv
} // namespace freedm

#endif // C_LINE_CLIENT_HPP
