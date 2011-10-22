////////////////////////////////////////////////////////////////////////////////
/// @file           CLineServer.cpp
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

#include "CLineServer.hpp"

namespace simserv {

/// Creates a shared line server instance
CLineServer::TPointer CLineServer::Create( boost::asio::io_service & p_service,
    unsigned short p_port, THandler p_handler )
{
    return TPointer( new CLineServer(p_service, p_port, p_handler) );
}

/// Closes a line server
CLineServer::~CLineServer()
{
    if( m_acceptor.is_open() )
    {
        m_acceptor.close();
    }
    if( m_socket.is_open() )
    {
        m_socket.close();
    }
}

/// Creates a line server that uses the given message handler 
CLineServer::CLineServer( boost::asio::io_service & p_service,
    unsigned short p_port, THandler p_handler )
    : m_acceptor(p_service)
    , m_socket(p_service)
    , m_port(p_port)
    , m_handler(p_handler)
{
    boost::asio::ip::tcp::endpoint endpt( boost::asio::ip::tcp::v4(), m_port );
    
    // open the acceptor at endpoint
    m_acceptor.open( endpt.protocol() );
    m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    m_acceptor.bind( endpt );
    m_acceptor.listen();
    
    // wait for clients
    StartAccept();
}

/// Directs client connections to the handle accept function
void CLineServer::StartAccept()
{
    m_acceptor.async_accept( m_socket, boost::bind(&CLineServer::HandleAccept,
        this, boost::asio::placeholders::error) );
}

/// Directs client data to the message handler callback function
void CLineServer::HandleAccept( const boost::system::error_code & p_error )
{
    boost::asio::streambuf request;
    std::istream request_stream( &request );
    std::string request_data;
    
    boost::asio::streambuf response;
    std::ostream response_stream( &response );
    std::string response_data;
    
    size_t bytes;

    if( !p_error )
    {
        try
        {
            // store the client request as a string
            bytes = boost::asio::read_until( m_socket, request, "\r\n" );
            std::getline( request_stream, request_data );
            
            // send the request to the message handler
            response_data = m_handler(request_data);
            response_stream << response_data;
            
            // write the handler response to the client
            boost::asio::write( m_socket, response );
            request.consume( bytes );
        }
        catch( std::exception & e )
        {
            // placeholder
            throw e;
        }
        
        if( m_socket.is_open() )
        {
            m_socket.close();
        }
    }
    StartAccept();
}

} // namespace simserv
