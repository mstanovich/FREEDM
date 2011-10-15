////////////////////////////////////////////////////////////////////////////////
/// @file           CTableStructure.hpp
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

#ifndef C_TABLE_STRUCTURE_HPP
#define C_TABLE_STRUCTURE_HPP

#include <map>
#include <set>
#include <string>
#include <utility>
#include <iostream>
#include <stdexcept>

#include <boost/foreach.hpp>
#include <boost/optional/optional.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "common.hpp"
#include "logger.hpp"
#include "CDeviceKey.hpp"

CREATE_EXTERN_STD_LOGS()

namespace simserv {

class CTableStructure
{
public:
    /// Creates an empty table structure
    CTableStructure();
    
    /// Creates a table structure defined by a tag in an xml file
    CTableStructure( const std::string & p_xml, const std::string & p_tag );
    
    /// Parses and stores the content of an xml file as the table structure
    void ReadFile( const std::string & p_xml, const std::string & p_tag );
    
    /// Returns the number of stored device keys
    size_t GetTableSize() const;
    
    /// Returns the number of known controllers
    size_t GetControlCount() const;
    
    /// Converts a table index into its stored device key
    const CDeviceKey & GetDevice( size_t p_index ) const;
    
    /// Converts a device key into its table index
    size_t GetIndex( const CDeviceKey & p_dkey ) const;
    
    /// Returns the initial value assigned to a device key
    TSimulationValue GetInitialValue( const CDeviceKey & p_dkey ) const;
    
    /// Checks whether a controller has access to a device key
    bool CheckAccess( const CDeviceKey & p_dkey, size_t p_control ) const;
    
    /// Prints the table structure to the given output stream
    std::ostream & PrintTable( std::ostream & p_os ) const;
private:
    /// Device key attributes stored by the table
    struct SEntry
    {
        /// Location of the entry in the xml file
        size_t s_TableIndex;
        
        /// Initial value assigned to the entry
        TSimulationValue s_InitialValue;
        
        /// Controllers with access to the entry
        std::set<size_t> s_ControlAccess;
    };
    
    /// Type of the index map
    typedef std::map<size_t, CDeviceKey> TIndexToDevice;
    
    /// Type of the device key map
    typedef std::map<CDeviceKey, SEntry> TDeviceToEntry;

    /// Returns the stored entry information for a device key
    const SEntry & GetEntry( const CDeviceKey & p_dkey ) const;
    
    /// Number of device key entries
    size_t m_TableSize;
    
    /// Number of recognized controllers
    size_t m_ControlCount;
    
    /// Map from table index to device key
    TIndexToDevice m_IndexToDevice;
    
    /// Map from device key to entry information
    TDeviceToEntry m_DeviceToEntry;
};

/// Outputs a table structure to the given output stream
std::ostream & operator<<( std::ostream & p_os, const CTableStructure & p_ts );

} // namespace simserv

#endif // C_TABLE_STRUCTURE_HPP
