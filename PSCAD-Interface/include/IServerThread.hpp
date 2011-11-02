////////////////////////////////////////////////////////////////////////////////
/// @file           IServerThread.hpp
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

#ifndef I_SERVER_THREAD_HPP
#define I_SERVER_THREAD_HPP

#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace freedm {
namespace simserv {

class IServerThread
{
public:
    void Start();
    void Join();
    void Stop();
private:
    boost::thread m_thread;
protected:
    boost::asio::io_service m_ios;
};

} // namespace simserv
} // namespace freedm

#endif // I_SERVER_THREAD_HPP
