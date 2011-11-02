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
#include <string>
#include <iostream>
#include <stdexcept>

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "CDeviceKey.hpp"

namespace freedm {
namespace simserv {

class CTableStructure
{
public:
    CTableStructure();
    CTableStructure( const std::string & p_xml, const std::string & p_tag );
    void ReadFile( const std::string & p_xml, const std::string & p_tag );
    
    size_t GetTableSize() const;
    size_t GetControlCount() const;

    size_t GetIndex( const CDeviceKey & p_dkey ) const;
    const CDeviceKey & GetDevice( size_t p_index ) const;

    std::ostream & PrintTable( std::ostream & p_os ) const;
private:
    struct SEntry
    {
        size_t s_TableIndex;
    };

    typedef std::map<size_t,CDeviceKey> TIndexToKey;
    typedef std::map<CDeviceKey,SEntry> TKeyToEntry;

    const SEntry & GetEntry( const CDeviceKey & p_dkey ) const;

    size_t m_TableSize;
    size_t m_ControlCount;
 
    TIndexToKey m_IndexToKey;
    TKeyToEntry m_KeyToEntry;
};

std::ostream & operator<<( std::ostream & p_os, const CTableStructure & p_ts );

} // namespace simserv
} // namespace freedm

#endif // C_TABLE_STRUCTURE_HPP
