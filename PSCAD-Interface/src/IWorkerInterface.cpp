////////////////////////////////////////////////////////////////////////////////
/// @file           IWorkerInterface.cpp
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

#include "IWorkerInterface.hpp"

namespace simserv {

/// Creates a worker interface on a given port number
IWorkerInterface::IWorkerInterface( unsigned short p_port )
{
    m_server = CLineServer::Create( m_service, p_port,
        boost::bind(&IWorkerInterface::HandleMessage, boost::ref(*this), _1) );
}

/// Starts the line server
void IWorkerInterface::Work()
{
    m_service.run();
}

} // namespace simserv
