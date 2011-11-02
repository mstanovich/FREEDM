////////////////////////////////////////////////////////////////////////////////
/// @file           CDeviceTable.cpp
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

#include "CDeviceTable.hpp"

namespace freedm {
namespace simserv {

CDeviceTable::CDeviceTable()
{
    // skip
}

CDeviceTable::CDeviceTable( const std::string &p_xml,
        const std::string &p_tag )
{
    ReadFile( p_xml, p_tag );
}

void CDeviceTable::ReadFile( const std::string & p_xml,
        const std::string & p_tag )
{
    // redefine internal structure
    m_structure.ReadFile( p_xml, p_tag );
    
    // reset data
    m_data.clear();
    m_data.resize(m_structure.GetTableSize());
}

void CDeviceTable::SetData( std::vector<TValue> p_vector )
{
    // check for data integrity
    if( p_vector.size() != m_data.size() )
    {
        throw std::domain_error("Invalid Vector Size");
    }
    
    // writer critical section
    TWriteLock lock(m_mutex);
    m_data.swap(p_vector);
}

void CDeviceTable::SetValue( const CDeviceKey & p_dkey, TValue p_value )
{
    size_t index = m_structure.GetIndex(p_dkey);
    
    // writer critical section
    TWriteLock lock(m_mutex);
    m_data[index] = p_value;
}

std::vector<CDeviceTable::TValue> CDeviceTable::GetData()
{
    // reader critical section
    TReadLock lock(m_mutex);
    return m_data;
}

CDeviceTable::TValue CDeviceTable::GetValue( const CDeviceKey & p_dkey )
{
    size_t index = m_structure.GetIndex(p_dkey);
    
    // reader critical section
    TReadLock lock(m_mutex);
    return m_data[index];
}

std::ostream & CDeviceTable::PrintTable( std::ostream & p_os ) const
{
    size_t n = m_data.size();
    size_t i = 0;
    
    while( i < n )
    {
        // print device key and value pair
        p_os << m_structure.GetDevice(i) << "\t" << m_data[i];
        
        // next entry
        ++i;
        
        // print delimiter
        if( i < n )
        {
            p_os << "\n";
        }
    }
    
    return p_os;
}

std::ostream & operator<<( std::ostream & p_os, const CDeviceTable & p_table )
{
    return p_table.PrintTable(p_os);
}

} // namespace simserv
} // namespace freedm
