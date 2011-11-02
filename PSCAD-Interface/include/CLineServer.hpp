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

#include <map>
#include <string>
#include <iostream>
#include <stdexcept>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>

namespace freedm {
namespace simserv {

class CLineServer : private boost::noncopyable
{
public:
    struct SResult  // this should be an adapter (at some point)
    {
        std::string s_message;
        bool s_success;
    };
    
    typedef boost::function< SResult ( const std::string & ) > TCallback;
    typedef boost::shared_ptr<CLineServer> TPointer;
    
    static TPointer Create( boost::asio::io_service & p_ios, unsigned short p_port );
    void RegisterCallback( const std::string & p_type, TCallback p_callback );
    
    ~CLineServer();
private:
    typedef std::map<std::string,TCallback> TCallbackMap;
    
    CLineServer( boost::asio::io_service & p_ios, unsigned short p_port );
    
    void StartAccept();
    void HandleAccept( const boost::system::error_code & p_error );
    void HandleRead( const boost::system::error_code & p_error );
    void HandleWrite( const boost::system::error_code & p_error );

    boost::asio::ip::tcp::acceptor m_acceptor;
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::streambuf m_buffer;
    
    TCallbackMap m_callback;
    boost::mutex m_mutex;
    
    unsigned short m_port;
};

} // namespace simserv
} // namespace freedm

#endif // C_LINE_SERVER_HPP
