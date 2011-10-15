////////////////////////////////////////////////////////////////////////////////
/// @file           CDeviceKey.cpp
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

#include "CDeviceKey.hpp"

namespace simserv {

/// Constructs an empty device key
CDeviceKey::CDeviceKey()
{
    // skip
}

/// Constructs a (device,key) pair from the given arguments
CDeviceKey::CDeviceKey(const std::string & p_device, const std::string & p_key)
    : m_device(p_device), m_key(p_key)
{
    // skip
}

/// Sets the physical device identifier
void CDeviceKey::SetDevice( const std::string & p_device )
{
    m_device = p_device;
}

/// Sets the simulation variable key
void CDeviceKey::SetKey( const std::string & p_key )
{
    m_key = p_key;
}

/// Gets the physical device identifier
const std::string & CDeviceKey::GetDevice() const
{
    return m_device;
}

/// Gets the simulation variable key
const std::string & CDeviceKey::GetKey() const
{
    return m_key;
}

/// Determines if the left device key is less than the right
bool operator<( const CDeviceKey & p_lhs, const CDeviceKey & p_rhs )
{
    const std::string & l_device    = p_lhs.GetDevice();
    const std::string & l_key       = p_lhs.GetKey();
    const std::string & r_device    = p_rhs.GetDevice();
    const std::string & r_key       = p_rhs.GetKey();
    
    return( l_device < r_device || (l_device == r_device && l_key < r_key) );
}

/// Determines if two device keys are equivalent
bool operator==( const CDeviceKey & p_lhs, const CDeviceKey & p_rhs )
{
    const std::string & l_device    = p_lhs.GetDevice();
    const std::string & l_key       = p_lhs.GetKey();
    const std::string & r_device    = p_rhs.GetDevice();
    const std::string & r_key       = p_rhs.GetKey();
    
    return( l_device == r_device && l_key == r_key );
}

/// Outputs a device key to the given otuput stream
std::ostream & operator<<( std::ostream & p_os, const CDeviceKey & p_dkey )
{
    const std::string & device  = p_dkey.GetDevice();
    const std::string & key     = p_dkey.GetKey();
    
    return( p_os << "(" << device << "," << key << ")" );
}

} // namespace simserv
