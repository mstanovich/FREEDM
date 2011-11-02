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
#include "CDeviceTable.hpp"

CREATE_STD_LOGS()

int main( int argc, char * argv[] )
{
    // set the logger level
    Logger::Log::setLevel(5);
    
    // create and test the structure
    try
    {
        freedm::simserv::CDeviceTable sample( "sample.xml", "my_table" );
        Logger::Notice << "\n" << sample << std::endl;
    }
    catch( std::exception & e )
    {
        Logger::Critical << e.what() << std::endl;
    }
    
    return 0;
}
