////////////////////////////////////////////////////////////////////////////////
/// @file           IWorker.hpp
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

#ifndef I_WORKER_HPP
#define I_WORKER_HPP

#include <boost/thread/thread.hpp>

namespace simserv {

/// Defines a threaded worker interface
class IWorker
{
public:
    /// Start working on a separate thread
    void Start();
    
    /// Block until all work completes
    void Join();
    
    /// Function for the work to perform
    virtual void Work() = 0;
    
    /// Destructor for derived classes
    virtual ~IWorker() {}
private:
    /// Thread to perform the work
    boost::thread m_thread;
};

} // namespace simserv

#endif // I_WORKER_HPP
