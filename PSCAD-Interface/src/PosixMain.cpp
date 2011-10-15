////////////////////////////////////////////////////////////////////////////////
/// @file           CDeviceKey.cpp
///
/// @author         Thomas Roth <tprfh7@mst.edu>
///
/// @compiler       C++
///
/// @project        Missouri S&T Power Research Group
///
/// @description    
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

#include <iostream>

#include "logger.hpp"
#include "CDeviceKey.hpp"

CREATE_STD_LOGS()

int main( int argc, char * argv[] )
{
    // set the logger level
    Logger::Log::setLevel(5);
    
    // create the test device keys
    simserv::CDeviceKey blank, full("device","key");
    Logger::Notice << "blank = " << blank << std::endl;
    Logger::Notice << "full  = " << full << std::endl;
    
    // non-equivalence test
    if( blank == full )
    {
        Logger::Critical << "blank == full" << std::endl;
    }
    else
    {
        Logger::Notice << "blank != full" << std::endl;
    }

    // set and get test
    blank.SetDevice(full.GetDevice());
    blank.SetKey(full.GetKey());
    
    // equivalent test
    if( blank == full )
    {
        Logger::Notice << "blank == full" << std::endl;
    }
    else
    {
        Logger::Critical << "blank != full" << std::endl;
    }
    
    return 0;
}
