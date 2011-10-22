////////////////////////////////////////////////////////////////////////////////
/// @file           CLineServer.hpp
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

#ifndef C_LINE_SERVER_HPP
#define C_LINE_SERVER_HPP

#include <string>
#include <iostream>
#include <stdexcept>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/utility.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include "logger.hpp"

CREATE_EXTERN_STD_LOGS()

namespace simserv {

/// Defines a line server that handles string data
class CLineServer : private boost::noncopyable
{
public:
    /// Type of the message handler function
    typedef boost::function< std::string ( const std::string & ) > THandler;
    
    /// Shared pointer to a line server instance
    typedef boost::shared_ptr<CLineServer> TPointer;
    
    /// Creates a shared line server instance
    static TPointer Create( boost::asio::io_service & p_service,
        THandler p_handler, unsigned short p_port );
    
    /// Closes a line server
    ~CLineServer();
private:
    /// Creates a line server that uses the given message handler 
    CLineServer( boost::asio::io_service & p_service, THandler p_handler,
        unsigned short p_port );
    
    /// Directs client connections to the handle accept function
    void StartAccept();
    
    /// Directs client data to the message handler callback function
    void HandleAccept( const boost::system::error_code & p_error );
    
    /// entry queue for client connections
    boost::asio::ip::tcp::acceptor m_acceptor;
    
    /// socket for the accepted client connection
    boost::asio::ip::tcp::socket m_socket;
    
    /// callback function that handles client data
    THandler m_handler;
    
    /// line server port number
    unsigned short m_port;
};

} // namespace simserv

#endif // C_LINE_SERVER_HPP
