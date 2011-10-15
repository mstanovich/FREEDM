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

namespace simserv {

/// Creates an empty device table
CDeviceTable::CDeviceTable()
{
    Logger::Debug << __PRETTY_FUNCTION__ << std::endl;
}

/// Creates a device table from the given specification file
CDeviceTable::CDeviceTable( const std::string & p_xml,
    const std::string & p_tag )
{
    Logger::Debug << __PRETTY_FUNCTION__ << std::endl;
    
    ReadFile( p_xml, p_tag );
}

/// Parses and stores the given specification file as a device table
void CDeviceTable::ReadFile( const std::string & p_xml,
    const std::string & p_tag )
{
    Logger::Debug << __PRETTY_FUNCTION__ << std::endl;
    
    m_structure.ReadFile( p_xml, p_tag );
    
    m_data.clear();
    m_data.resize(m_structure.GetTableSize());
    ResetVector();
}

/// Sets the value of a device key for a given controller
void CDeviceTable::SetValue( const CDeviceKey & p_dkey,
    TSimulationValue p_value, size_t p_control )
{
    size_t index = m_structure.GetIndex(p_dkey);
    
    // check for write permission
    if( m_structure.CheckAccess(p_dkey,p_control) == false )
    {
        throw std::domain_error("Private Table SetValue");
    }
    
    // writer critical section
    TWriteLock lock(m_mutex);
    m_data[index] = p_value;
}

/// Returns the value of a device key for a given controller
TSimulationValue CDeviceTable::GetValue( const CDeviceKey & p_dkey,
    size_t p_control )
{
    size_t index = m_structure.GetIndex(p_dkey);
    
    // check for read permission
    if( m_structure.CheckAccess(p_dkey,p_control) == false )
    {
        throw std::domain_error("Private Table GetValue");
    }
    
    // reader critical section
    TReadLock lock(m_mutex);
    return m_data[index];
}

/// Resets the value of a device key to its initial value
void CDeviceTable::ResetValue( const CDeviceKey & p_dkey, size_t p_control )
{
    SetValue( p_dkey, m_structure.GetInitialValue(p_dkey), p_control );
}

/// Sets the content of the device table to the given vector
void CDeviceTable::SetVector( const std::vector<TSimulationValue> & p_vector )
{
    // check for data integrity
    if( p_vector.size() != m_data.size() )
    {
        throw std::domain_error("Invalid Vector Size");
    }
    
    // writer critical section
    TWriteLock lock(m_mutex);
    m_data = p_vector;
}

/// Returns the content of the device table as a vector
std::vector<TSimulationValue> CDeviceTable::GetVector()
{
    // reader critical section
    TReadLock lock(m_mutex);
    return m_data;
}

/// Resets all device keys to their initial values
void CDeviceTable::ResetVector()
{
    std::vector<TSimulationValue> initial_vector;
    TSimulationValue value;
    CDeviceKey dkey;
    
    // create a vector of initial values
    for( size_t i = 0, n = m_data.size(); i < n; i++ )
    {
        dkey    = m_structure.GetDevice(i);
        value   = m_structure.GetInitialValue(dkey);

        initial_vector.push_back(value);
    }
    
    SetVector(initial_vector);
}

/// Prints the device table to the given output stream
std::ostream & CDeviceTable::PrintTable( std::ostream & p_os ) const
{
    size_t n = m_data.size();
    size_t i = 0;
    
    while( i < n )
    {
        // print device key and value pair
        p_os << m_structure.GetDevice(i) << "\t" << m_data[i];
        
        ++i;
        
        // print delimiter
        if( i < n )
        {
            p_os << "\n";
        }
    }
    
    return p_os;
}

/// Outputs a device table to the given output stream
std::ostream & operator<<( std::ostream & p_os, const CDeviceTable & p_table )
{
    return p_table.PrintTable(p_os);
}

} // namespace simserv
