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

namespace simserv {

/// Creates an empty table structure
CTableStructure::CTableStructure()
{
    Logger::Debug << __PRETTY_FUNCTION__ << std::endl;
}

/// Creates a table structure defined by a tag in an xml file
CTableStructure::CTableStructure( const std::string & p_xml,
    const std::string & p_tag )
{
    Logger::Debug << __PRETTY_FUNCTION__ << std::endl;
    
    ReadFile( p_xml, p_tag );
}

/// Parses and stores the content of an xml file as the table structure
void CTableStructure::ReadFile( const std::string & p_xml,
    const std::string & p_tag )
{
    using boost::property_tree::ptree;
    Logger::Debug << __PRETTY_FUNCTION__ << std::endl;
    
    ptree xml_tree;
    
    ptree::assoc_iterator access_it;
    std::set<size_t> access_set;
    size_t access_index;
    
    boost::optional<TSimulationValue> initial;
    std::string device, key;
    size_t index;

    CDeviceKey dkey;
    SEntry entry;

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
        // convenience reference
        ptree & subtree = child.second;
        
        // reset access set
        access_set.clear();
        
        // read each entry attribute
        index       = subtree.get<size_t>("<xmlattr>.index");
        device      = subtree.get<std::string>("device");
        key         = subtree.get<std::string>("key");
        initial  = subtree.get_optional<TSimulationValue>("initial");

        // this should use equal_range - but I couldn't get it to work :(
        access_it = subtree.find("access");
        if( access_it == subtree.not_found() )
        {
            // set the default access level to global
            for( size_t i = 1; i <= m_ControlCount; i++ )
            {
                access_set.insert(i);
            }
        }
        while( access_it != subtree.not_found() )
        {
            // get the index of the next controller with access
            access_index = access_it->second.get_value<size_t>();
            
            // insert the index into the access set
            if( access_set.count(access_index) > 0 )
            {
                throw std::logic_error("Duplicate Access Indexes");
            }
            access_set.insert(access_index);
            
            // erase the index so the next one can be selected
            subtree.erase(subtree.to_iterator(access_it));
            
            // select the next access index
            access_it = subtree.find("access");
        }
        
        // set the device key
        dkey.SetDevice(device);
        dkey.SetKey(key);
        
        // check for incorrect values
        if( index == 0 || index > m_TableSize )
        {
            throw std::logic_error("XML File has Invalid Indexes");
        }
        if( m_IndexToDevice.count(index) > 0 )
        {
            throw std::logic_error("XML File has Duplicate Indexes");
        }
        if( m_DeviceToEntry.count(dkey) > 0 )
        {
            throw std::logic_error("XML File has Duplicate Device Keys");
        }
        if( access_set.size() > 0 && (*access_set.begin()) == 0 )
        {
            throw std::logic_error("XML File has Invalid Access Indexes");
        }
        if( access_set.size() > 0 && (*access_set.end()) > m_ControlCount )
        {
            throw std::logic_error("XML File has Invalid Access Indexes");
        }
        
        // set the entry information
        entry.s_TableIndex      = index;
        entry.s_InitialValue    = ( initial ? initial.get() : 0 );
        entry.s_ControlAccess   = access_set;
        
        // store the table entry
        m_IndexToDevice.insert( TIndexToDevice::value_type(index,dkey) );
        m_DeviceToEntry.insert( TDeviceToEntry::value_type(dkey,entry) );
    }
}

/// Returns the number of stored device keys
size_t CTableStructure::GetTableSize() const
{
    return m_TableSize;
}

/// Returns the number of known controllers
size_t CTableStructure::GetControlCount() const
{
    return m_ControlCount;
}

/// Converts a table index into its stored device key
const CDeviceKey & CTableStructure::GetDevice( size_t p_index ) const
{
    TIndexToDevice::const_iterator it = m_IndexToDevice.find(p_index);

    if( it == m_IndexToDevice.end() )
    {
        throw std::range_error("Invalid Table Index");
    }
    return it->second;
}

/// Converts a device key into its table index
size_t CTableStructure::GetIndex( const CDeviceKey & p_dkey ) const
{
    return GetEntry(p_dkey).s_TableIndex;
}

/// Returns the initial value assigned to a device key
TSimulationValue CTableStructure::GetInitialValue(
    const CDeviceKey & p_dkey ) const
{
    return GetEntry(p_dkey).s_InitialValue;
}

/// Checks whether a controller has access to a device key
bool CTableStructure::CheckAccess( const CDeviceKey & p_dkey,
    size_t p_control ) const
{
    return( GetEntry(p_dkey).s_ControlAccess.count(p_control) > 0 );
}

/// Prints the table structure to the given output stream
std::ostream & CTableStructure::PrintTable( std::ostream & p_os ) const
{
    TDeviceToEntry::const_iterator it   = m_DeviceToEntry.begin();
    TDeviceToEntry::const_iterator end  = m_DeviceToEntry.end();
    std::set<size_t>::const_iterator access_it, access_end;
    
    while( it != end )
    {
        // get access list iterators
        access_it   = it->second.s_ControlAccess.begin();
        access_end  = it->second.s_ControlAccess.end();

        // print device key
        p_os << it->first;
        
        // print entry information
        p_os << "[index=" << it->second.s_TableIndex << "]";
        p_os << "[init_value=" << it->second.s_InitialValue << "]";
        p_os << "[access=(";
        while( access_it != access_end )
        {
            p_os << (*access_it);
            
            ++access_it;
            
            // print access delimiter
            if( access_it != access_end )
            {
                p_os << ",";
            }
        }
        p_os << ")]";
        
        ++it;
        
        // print entry delimiter
        if( it != end )
        {
            p_os << "\n";
        }
    }
    
    return p_os;
}

/// Returns the stored entry information for a device key
const CTableStructure::SEntry & CTableStructure::GetEntry(
    const CDeviceKey & p_dkey ) const
{
    TDeviceToEntry::const_iterator it = m_DeviceToEntry.find(p_dkey);

    if( it == m_DeviceToEntry.end() )
    {
        throw std::range_error("Invalid Device Key");
    }
    return it->second;
}

/// Outputs a table structure to the given output stream
std::ostream & operator<<( std::ostream & p_os, const CTableStructure & p_ts )
{
    return p_ts.PrintTable(p_os);
}

} // namespace simserv
