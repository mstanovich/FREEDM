////////////////////////////////////////////////////////////////////////////////
/// @file           CLineClient.cpp
///
/// @author         Thomas Roth <tprfh7@mst.edu>
///
/// @compiler       C++
///
/// @project        Missouri S&T Power Research Group
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

#include "CLineClient.hpp"

namespace freedm {
namespace simserv {

/// Creates a shared simulation line client instance
CLineClient::TPointer CLineClient::Create( boost::asio::io_service & p_ios )
{
    return TPointer( new CLineClient(p_ios) );
}

/// Sets the remote simulation server hostname
void CLineClient::SetHostname( const std::string & p_hostname )
{
    m_hostname = p_hostname;
}

/// Sets the remote simulation server port number
void CLineClient::SetPort( const std::string & p_port )
{
    m_port = p_port;
}

/// Send a request to get the current value of a device setting
std::string CLineClient::Get( const std::string & p_device,
        const std::string & p_key )
{
    std::stringstream request;
    
    // format and send the request, return the response
    request << "GET " << p_device << " " << p_key << "\r\n";
    return SendRequest( request.str() );
}

/// Send a request to set the value of a device setting
void CLineClient::Set( const std::string & p_device, const std::string & p_key,
        const std::string & p_value )
{
    std::stringstream request;

    // format and send the request, ignore the response
    request << "SET " << p_device << " " << p_key << " " << p_value << "\r\n";
    SendRequest( request.str() );
}

/// Creates a simulation line client instance
CLineClient::CLineClient( boost::asio::io_service & p_ios )
    : m_socket(p_ios)
{
    // skip
}

/// Creates a socket connection to the remote simulation server
void CLineClient::Connect()
{
    boost::asio::ip::tcp::resolver resolver( m_socket.get_io_service() );
    boost::asio::ip::tcp::resolver::query query( m_hostname, m_port );
    boost::asio::ip::tcp::resolver::iterator it, end;
    boost::system::error_code error;
    
    // generate a list of endpoints
    it = resolver.resolve(query);
    
    // iterate through the endpoints
    while( it != end )
    {
        // close socket if open
        m_socket.close();
        
        // try to connect to the next endpoint
        m_socket.connect( *it, error );
        
        // iterate to the next endpoint
        it = ( error ? ++it : end );
    }
    if( error )
    {
        // propogate the error on connection failure
        throw boost::system::system_error(error);
    }
}

/// Sends a request to the remote simulation server
std::string CLineClient::SendRequest( const std::string & p_message )
{
    boost::asio::streambuf response;
    std::istream response_stream( &response );
    std::string response_code, response_content;
    
    Connect();
    {
        // format and send the request stream
        boost::asio::write( m_socket, boost::asio::buffer(p_message) );
        
        // receive and split the response stream
        boost::asio::read_until( m_socket, response, "\r\n" );
        response_stream >> response_code;
        std::getline( response_stream, response_content );
        
        // handle bad responses
        if( response_code != "OK" )
        {
            throw std::runtime_error(response_content);
        }
    }
    m_socket.close();
    
    return response_content;
}

} // namespace simserv
} // namespace freedm
