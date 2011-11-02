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

namespace freedm {
namespace simserv {

CDeviceKey::CDeviceKey()
{
    // skip
}

CDeviceKey::CDeviceKey( const std::string &p_device, const std::string &p_key )
    : m_device(p_device), m_key(p_key)
{
    // skip
}

void CDeviceKey::SetDevice( const std::string & p_device )
{
    m_device = p_device;
}

void CDeviceKey::SetKey( const std::string & p_key )
{
    m_key = p_key;
}

const std::string & CDeviceKey::GetDevice() const
{
    return m_device;
}

const std::string & CDeviceKey::GetKey() const
{
    return m_key;
}

bool operator<( const CDeviceKey & p_lhs, const CDeviceKey & p_rhs )
{
    const std::string & l_device    = p_lhs.GetDevice();
    const std::string & l_key       = p_lhs.GetKey();
    const std::string & r_device    = p_rhs.GetDevice();
    const std::string & r_key       = p_rhs.GetKey();
    
    return( l_device < r_device || (l_device == r_device && l_key < r_key) );
}

bool operator==( const CDeviceKey & p_lhs, const CDeviceKey & p_rhs )
{
    const std::string & l_device    = p_lhs.GetDevice();
    const std::string & l_key       = p_lhs.GetKey();
    const std::string & r_device    = p_rhs.GetDevice();
    const std::string & r_key       = p_rhs.GetKey();
    
    return( l_device == r_device && l_key == r_key );
}

std::ostream & operator<<( std::ostream & p_os, const CDeviceKey & p_dkey )
{
    const std::string & device  = p_dkey.GetDevice();
    const std::string & key     = p_dkey.GetKey();
    
    return( p_os << "(" << device << "," << key << ")" );
}

} // namespace simserv
} // namespace freedm
