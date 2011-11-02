////////////////////////////////////////////////////////////////////////////////
/// @file           CTableStructure.cpp
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

#include "CTableStructure.hpp"

namespace freedm {
namespace simserv {

CTableStructure::CTableStructure()
{
    // skip
}

CTableStructure::CTableStructure( const std::string & p_xml,
        const std::string & p_tag )
{
    ReadFile( p_xml, p_tag );
}

void CTableStructure::ReadFile( const std::string & p_xml,
        const std::string & p_tag )
{
    using boost::property_tree::ptree;

    ptree xml_tree;
    
    std::string device, key;
    size_t index;

    CDeviceKey dkey;
    SEntry entry;

    // clear old data structures
    m_IndexToKey.clear();
    m_KeyToEntry.clear();
    
    // create property tree
    read_xml( p_xml, xml_tree );
    
    // get the number of controllers
    m_ControlCount = xml_tree.get<size_t>("ControlCount");
    
    // select the subtree rooted at the speficiation tag
    xml_tree = xml_tree.get_child(p_tag);
    
    // get the number of entries
    m_TableSize = xml_tree.size();
    
    // iterate over each entry of the tagged subtree
    BOOST_FOREACH( ptree::value_type & child, xml_tree )
    {
        // read each entry attribute
        index   = child.second.get<size_t>("<xmlattr>.index");
        device  = child.second.get<std::string>("device");
        key     = child.second.get<std::string>("key");
        
        // set the device key
        dkey.SetDevice(device);
        dkey.SetKey(key);
        
        // check for incorrect values
        if( index >= m_TableSize )
        {
            throw std::logic_error("XML has Invalid Indexes");
        }
        if( m_IndexToKey.count(index) > 0 )
        {
            throw std::logic_error("XML has Duplicate Indexes");
        }
        if( m_KeyToEntry.count(dkey) > 0 )
        {
            throw std::logic_error("XML has Duplicate Device Keys");
        }
        
        // set the entry information
        entry.s_TableIndex = index;
        
        // store the table entry
        m_IndexToKey.insert( TIndexToKey::value_type(index,dkey) );
        m_KeyToEntry.insert( TKeyToEntry::value_type(dkey,entry) );
    }
}

size_t CTableStructure::GetTableSize() const
{
    return m_TableSize;
}

size_t CTableStructure::GetControlCount() const
{
    return m_ControlCount;
}

const CDeviceKey & CTableStructure::GetDevice( size_t p_index ) const
{
    TIndexToKey::const_iterator it = m_IndexToKey.find(p_index);

    if( it == m_IndexToKey.end() )
    {
        throw std::range_error("Invalid Table Index");
    }
    return it->second;
}

size_t CTableStructure::GetIndex( const CDeviceKey & p_dkey ) const
{
    return GetEntry(p_dkey).s_TableIndex;
}

std::ostream & CTableStructure::PrintTable( std::ostream & p_os ) const
{
    TKeyToEntry::const_iterator it   = m_KeyToEntry.begin();
    TKeyToEntry::const_iterator end  = m_KeyToEntry.end();
    
    while( it != end )
    {
        // print device key
        p_os << it->first;
        
        // print entry information
        p_os << "[index=" << it->second.s_TableIndex << "]";
        
        // next entry
        ++it;
        
        // print entry delimiter
        if( it != end )
        {
            p_os << "\n";
        }
    }
    
    return p_os;
}

const CTableStructure::SEntry & CTableStructure::GetEntry(
        const CDeviceKey & p_dkey ) const
{
    TKeyToEntry::const_iterator it = m_KeyToEntry.find(p_dkey);
    
    if( it == m_KeyToEntry.end() )
    {
        throw std::range_error("Invalid Device Key");
    }
    return it->second;
}

std::ostream & operator<<( std::ostream & p_os, const CTableStructure & p_ts )
{
    return p_ts.PrintTable(p_os);
}

} // namespace simserv
} // namespace freedm
