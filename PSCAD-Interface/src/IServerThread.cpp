////////////////////////////////////////////////////////////////////////////////
/// @file           IServerThread.cpp
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

#include "IServerThread.hpp"

namespace freedm {
namespace simserv {

void IServerThread::Start()
{
    m_thread = boost::thread( &boost::asio::io_service::run, &m_ios );
}

void IServerThread::Join()
{
    m_thread.join();
}

void IServerThread::Stop()
{
    m_ios.stop();
}

} // namespace simserv
} // namespace freedm
