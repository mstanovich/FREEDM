////////////////////////////////////////////////////////////////////////////////
/// @file CTableStructure.hpp
///
/// @author Thomas Roth <tprfh7@mst.edu>
///
/// @compiler C++
///
/// @project Missouri S&T Power Research Group
///
/// @description
/// Defines a table structure class initialized from an XML input file.
///
/// @functions
/// CTableStructure::CTableStructure( const string &, const string & )
/// CTableStructure::GetSize() const
/// CTableStructure::FindIndex( const CDeviceKey & ) const
/// CTableStructure::FindDevice( size_t ) const
/// CTableStructure::HasAccess( const CDeviceKey &, size_t ) const
///
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
///
////////////////////////////////////////////////////////////////////////////////

#ifndef C_TABLE_STRUCTURE_HPP
#define C_TABLE_STRUCTURE_HPP

#include <map>
#include <set>
#include <string>
#include <sstream>
#include <stdexcept>

#include <boost/foreach.hpp>
#include <boost/bimap.hpp>
#include <boost/optional/optional.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "logger.hpp"
#include "CDeviceKeyCoupled.hpp"

CREATE_EXTERN_STD_LOGS()

namespace freedm {
    namespace broker {
////////////////////////////////////////////////////////////////////////////////
/// CTableStructure
///
/// @description
/// The table structure class maintains an internal state that can be
/// accessed or modified by a device-key pair, which are bundled into one
/// object called CDeviceKeyCoupled. Its internal structure is defined
/// by the XML file used to create the class instance.
///
/// @limitations
/// none
///
////////////////////////////////////////////////////////////////////////////////
class CTableStructure
{
public:
    ////////////////////////////////////////////////////////////////////////////
    /// CTableStructure( const string &, const string & )
    ///
    /// @description
    /// Creates an instance of CTableStructure with an internal structure
    /// specified by passed XML tag in the passed filename.
    ///
    /// @Shared_Memory
    /// none
    ///
    /// @Error_Handling
    /// An exception is thrown if the XML Input file has an invalid format.
    /// If an exception is thrown, the object is safely deconstructed.
    ///
    /// @pre
    /// p_xml adheres to the limitations format requirements
    ///
    /// @post
    /// none
    ///
    /// @param
    /// p_xml is the filename of the XML Input file
    /// p_tag is the XML tag of the table specification, eithre <command> or <state>.
    ///
    /// @limitations
    /// Required XML Format:
    /// <root>
    /// ...
    /// <p_tag>
    /// <entry index="1">
    /// <device>Unique Device Identifier</device>
    /// <key>Device Variable(such as power)</key>
    /// </entry>
    /// ...
    /// <entry index="n">
    /// ...
    /// </entry>
    /// </p_tag>
    /// </root>
    ///
    ////////////////////////////////////////////////////////////////////////////
    CTableStructure( const std::string & p_xml, const std::string & p_tag );
    
    ////////////////////////////////////////////////////////////////////////////
    /// GetSize() const
    ///
    /// @description
    /// Returns the number of unique device-keys stored in the table.
    ///
    /// @Shared_Memory
    /// none
    ///
    /// @Error_Handling
    /// none
    ///
    /// @pre
    /// none
    ///
    /// @post
    /// none
    ///
    /// @return
    /// m_TableSize
    ///
    /// @limitations
    /// none
    ///
    ////////////////////////////////////////////////////////////////////////////
    size_t GetSize() const { return m_TableSize; }
    
    ////////////////////////////////////////////////////////////////////////////
    /// FindIndex( const CDeviceKeyCoupled & ) const
    ///
    /// @description
    /// Converts a device-key object to a numeric index.
    ///
    /// @Shared_Memory
    /// none
    ///
    /// @Error_Handling
    /// Throws an exception if p_dkey is not stored in the table.
    ///
    /// @pre
    /// p_dkey was listed in the XML Input file for this instance
    ///
    /// @post
    /// none
    ///
    /// @param
    /// p_dkey is the device-key combo object to convert into an index
    ///
    /// @return
    /// index corresponding to p_dkey
    ///
    /// @limitations
    /// none
    ///
    ////////////////////////////////////////////////////////////////////////////
    size_t FindIndex( const CDeviceKeyCoupled & p_dkey ) const;
  
private:
    struct SDevice {};
    struct SIndex {};
    
    typedef boost::bimap< boost::bimaps::tagged<CDeviceKeyCoupled,SDevice>, boost::bimaps::tagged<size_t,SIndex> > TBimap;
    
    /// number of table entries
    size_t m_TableSize;
    
    /// bidirectional map from CDeviceKeyCopled object to numeric index
    TBimap m_TableHeaders;
};
    }//namespace broker
} // namespace freedm

#endif // C_TABLE_STRUCTURE_HPP
