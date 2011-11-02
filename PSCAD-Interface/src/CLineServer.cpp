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

namespace freedm {
namespace simserv {

CLineServer::TPointer CLineServer::Create( boost::asio::io_service & p_ios,
        unsigned short p_port )
{
    return TPointer( new CLineServer(p_ios,p_port) );
}

void CLineServer::RegisterCallback( const std::string & p_type,
        TCallback p_callback )
{
    // writer critical section
    boost::lock_guard<boost::mutex> lock(m_mutex);
    m_callback.insert( TCallbackMap::value_type(p_type,p_callback) );
}

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

CLineServer::CLineServer( boost::asio::io_service & p_ios, unsigned short p_port )
    : m_acceptor(p_ios)
    , m_socket(p_ios)
    , m_port(p_port)
{
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(),m_port);
    
    // open the acceptor at the endpoint
    m_acceptor.open(endpoint.protocol());
    m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    m_acceptor.bind(endpoint);
    m_acceptor.listen();
    
    // wait for clients
    StartAccept();
}

void CLineServer::StartAccept()
{
    // close open client connections
    if( m_socket.is_open() )
    {
        m_socket.close();
    }
    
    // open next client connection as m_socket and goto HandleAccept
    m_acceptor.async_accept( m_socket, boost::bind(&CLineServer::HandleAccept,
            this, boost::asio::placeholders::error) );
}

void CLineServer::HandleAccept( const boost::system::error_code & p_error )
{
    if( !p_error )
    {
        // read client data as m_buffer and goto HandleRead
        boost::asio::async_read_until( m_socket, m_buffer, "\r\n",
                boost::bind(&CLineServer::HandleRead, this,
                boost::asio::placeholders::error) );
    }
    else
    {
        // reset connection
        StartAccept();
    }
}

void CLineServer::HandleRead( const boost::system::error_code & p_error )
{
    if( !p_error )
    {
        try
        {
            std::istream request(&m_buffer);
            std::string type, content;
            
            boost::asio::streambuf response;
            std::ostream response_stream( &response );
            
            SResult result;
            
            // get callback type
            request >> type;
            
            // get callback parameter
            std::getline( request, content );
            
            // reader critical section
            {
                boost::lock_guard<boost::mutex> lock(m_mutex);
                
                if( m_callback.count(type) > 0 )
                {
                    // pass the callback of type its content
                    result = m_callback[type](content);
                }
                else
                {
                    throw std::runtime_error("Invalid Callback");
                }
            }

            // handle client response
            if( result.s_success )
            {
                response_stream << "OK ";
            }
            else
            {
                response_stream << "NO ";
            }
            response_stream << result.s_message << "\r\n";
            
            // clear the buffer of data
            m_buffer.consume( m_buffer.size() );
            
            boost::asio::async_write( m_socket, response,
                boost::bind(&CLineServer::HandleWrite, this,
                boost::asio::placeholders::error) );
        }
        catch( std::exception & error )
        {
            // reset connection
            StartAccept();
        }
    }
    else
    {
        // reset connection
        StartAccept();
    }
}

void CLineServer::HandleWrite( const boost::system::error_code & p_error )
{
    // reset connection
    StartAccept();
}

} // namespace simserv
} // namespace freedm
