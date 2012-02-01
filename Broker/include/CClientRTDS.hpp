////////////////////////////////////////////////////////////////////////////////
/// @file           CClientRTDS.hpp
///
/// @author         Yaxi Liu <ylztf@mst.edu>
///                 Thomas Roth <tprfh7@mst.edu>
///
/// @compiler       C++
///
/// @project        Missouri S&T Power Research Group
///
/// @description
///     Client side implementation of the simulation line protocol.
///
/// @functions
///     CLineClient::Create( io_service & )
///     CLineClient::Connect( const string &, const string & )
///     CLineClient::Set( const string &, const string &, const string & )
///     CLineClient::Get( const string &, const string & )
///     CLineClient::Quit()
///
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
///
////////////////////////////////////////////////////////////////////////////////

#ifndef C_CLIENT_RTDS_HPP
#define C_CLIENT_RTDS_HPP

#include <string>
#include <iostream>
#include <stdexcept>

#include <boost/asio.hpp>
#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

#include "logger.hpp" 
#include "CTableRTDS.hpp"

namespace freedm{
  namespace broker{
/// Provides an interface for communicating with a RTDS model
class CClientRTDS : private boost::noncopyable
{
    ////////////////////////////////////////////////////////////////////////////////
    /// CClientRTDS
    ///
    /// @description
    ///     The connection point with FPGA.  Acts as a client to FPGA's server.
    ///     The GET and SET commands operate on (Device,Key) pairs, where Device is
    ///     the unique identifier of some physical hardware and Key is the variable
    ///     name of that hardware to be manipulated.
    ///
    /// @limitations
    ///     none
    ///
    ////////////////////////////////////////////////////////////////////////////////
public:
    typedef boost::shared_ptr<CClientRTDS> RTDSPointer;
    static RTDSPointer Create( boost::asio::io_service & p_service, const std::string p_xml );
    
    ////////////////////////////////////////////////////////////////////////////
    /// Connect( const string &, const string & )
    ///
    /// @description
    ///     Creates a socket connection to the given hostname and service.
    ///     Is called by DeviceFactory when DeviceFactory is created.
    ///
    /// @Shared_Memory
    ///     none
    ///
    /// @Error_Handling
    ///     Throws an exception for unexpected connection errors.
    ///
    /// @pre
    ///     p_hostname and p_service specify a valid endpoint
    ///
    /// @post
    ///     m_socket attempts to connect to the passed service
    ///
    /// @param
    ///     p_hostname is the hostname of the desired endpoint
    ///     p_port is the port number of the desired endpoint
    ///
    /// @return
    ///     true if m_socket connected to the endpoint
    ///     false otherwise
    ///
    /// @limitations
    ///     none
    ///
    ////////////////////////////////////////////////////////////////////////////
    bool Connect( const std::string p_hostname, const std::string p_port );
  
    ////////////////////////////////////////////////////////////////////////////
    /// Set( const string &, const string &, const string & )
    ///
    /// @description
    ///     Search the cmdTable and then update the cmdBuf.
    ///
    /// @Shared_Memory
    ///     none
    ///
    /// @Error_Handling
    ///     Throws an exception if the server does not acknowledge the request.
    ///
    /// @pre
    ///     The socket connection has been established with a call to Connect
    ///
    /// @post
    ///     Writes a set message to m_socket
    ///     Reads an acknowledgement from m_socket
    ///
    /// @param
    ///     p_device is the unique identifier of the target device
    ///     p_key is the variable of the target device to modify
    ///     p_value is the value to set for p_device's p_key
    ///
    /// @limitations
    ///     The precondition is not enforced.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void Set( const std::string p_device, const std::string p_key, const double p_value );
    
    ////////////////////////////////////////////////////////////////////////////
    /// Get( const string &, const string & )
    ///
    /// @description
    ///     Search the stateTable and read from it.
    ///
    /// @Shared_Memory
    ///     none
    ///
    /// @Error_Handling
    ///     Throws an exception if the server does not respond to the request.
    ///
    /// @pre
    ///     The socket connection has been established with a call to Connect
    ///
    /// @post
    ///     Writes a get message to m_socket
    ///     Reads a response from m_socket
    ///
    /// @param
    ///     p_device is the unique identifier of the target device
    ///     p_key is the variable of the target device to access
    ///
    /// @return
    ///     p_device's p_key as determined by the line server response
    ///
    /// @limitations
    ///     The precondition is not enforced.
    ///
    ////////////////////////////////////////////////////////////////////////////
    double Get( const std::string p_device, const std::string p_key );
    
    ////////////////////////////////////////////////////////////////////////////
    /// Quit
    ///
    /// @description
    ///     Sends a quit request to the line server and closes the socket.
    ///
    /// @Shared_Memory
    ///     none
    ///
    /// @Error_Handling
    ///     Throws an exception if the server does not acknowledge the request.
    ///
    /// @pre
    ///     The socket connection has been established with a call to Connect
    ///
    /// @post
    ///     Writes a quit message to m_socket
    ///     Reads an acknowledgement from m_socket
    ///     Closes m_socket if no exception occurs
    ///
    /// @limitations
    ///     The precondition is not enforced.
    ///
    ////////////////////////////////////////////////////////////////////////////
    void Quit();
    
    ////////////////////////////////////////////////////////////////////////////
    /// ~CLineRTDS
    ///
    /// @description
    ///     Closes the socket before destroying an object instance.
    ///
    /// @Shared_Memory
    ///     none
    ///
    /// @Error_Handling
    ///     none
    ///
    /// @pre
    ///     none
    ///
    /// @post
    ///     m_socket is closed
    ///
    /// @limitations
    ///     none
    ///
    ////////////////////////////////////////////////////////////////////////////    
    ~CClientRTDS();

private:
    ////////////////////////////////////////////////////////////////////////////
    /// CClientRTDS( io_service & )
    ///
    /// @description
    ///     Creates a RTDS client on the given service.
    ///
    /// @Shared_Memory
    ///     Uses the passed io_service until destroyed.
    ///
    /// @Error_Handling
    ///     none
    ///
    /// @pre
    ///     none
    ///
    /// @post
    ///     io_service is shared with m_socket
    ///
    /// @param
    ///     p_service is the io_service the socket runs on
    ///
    /// @limitations
    ///     none
    ///
    ////////////////////////////////////////////////////////////////////////////
    CClientRTDS( boost::asio::io_service & p_service, const std::string p_xml );
    
    /////////////////////////////////////////////////////////////////////////
    /// Run
    /// @description
    ///      This is the main communication engine.
    ///      At every time step, initiate a send and receive of message to FPGA.
    //////////////////////////////////////////////////////////////////////////
    void Run();

    void endian_swap(char * data, const int num_bytes);

    /// socket to connect to FPGA server
    boost::asio::ip::tcp::socket m_socket;

    //store the readings from RTDS as well as commands to send to RTDS
    CTableRTDS m_cmdTable;

    CTableRTDS m_stateTable;

    boost::asio::deadline_timer m_GlobalTimer;
};

  }//namespace broker
}//namespace freedm

#endif // C_CLIENT_RTDS_HPP
