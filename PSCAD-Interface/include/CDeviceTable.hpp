////////////////////////////////////////////////////////////////////////////////
/// @file           CDeviceTable.hpp
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

#ifndef C_DEVICE_TABLE_CPP
#define C_DEVICE_TABLE_CPP

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

#include <boost/thread/shared_mutex.hpp>

#include "common.hpp"
#include "logger.hpp"
#include "CDeviceKey.hpp"
#include "CTableStructure.hpp"

CREATE_EXTERN_STD_LOGS()

namespace simserv {

/// Defines a table of device keys from an xml specification file
class CDeviceTable
{
public:
    /// Creates an empty device table
    CDeviceTable();
    
    /// Creates a device table from the given specification file
    CDeviceTable( const std::string & p_xml, const std::string & p_tag );
    
    /// Parses and stores the given specification file as a device table
    void ReadFile( const std::string & p_xml, const std::string & p_tag );
    
    /// Sets the value of a device key for a given controller
    void SetValue( const CDeviceKey & p_dkey, TSimulationValue p_value,
        size_t p_control );
    
    /// Returns the value of a device key for a given controller
    TSimulationValue GetValue( const CDeviceKey & p_dkey, size_t p_control );
    
    /// Resets the value of a device key to its initial value
    void ResetValue( const CDeviceKey & p_dkey, size_t p_control );
    
    /// Sets the content of the device table to the given vector
    void SetVector( const std::vector<TSimulationValue> & p_vector );
    
    /// Returns the content of the device table as a vector
    std::vector<TSimulationValue> GetVector();
    
    /// Resets all device keys to their initial values
    void ResetVector();
    
    /// Prints the device table to the given output stream
    std::ostream & PrintTable( std::ostream & p_os ) const;
private:
    /// Lock type for readers of m_mutex
    typedef boost::shared_lock<boost::shared_mutex> TReadLock;
    
    /// Lock type for writers of m_mutex
    typedef boost::unique_lock<boost::shared_mutex> TWriteLock;
    
    /// Internal structure of the device table
    CTableStructure m_structure;
    
    /// Read-Write mutex for m_data
    boost::shared_mutex m_mutex;
    
    /// Storage location for table elements
    std::vector<TSimulationValue> m_data;
};

/// Outputs a device table to the given output stream
std::ostream & operator<<( std::ostream & p_os, const CDeviceTable & p_table );

} // namespace simserv

#endif // C_DEVICE_TABLE_CPP
