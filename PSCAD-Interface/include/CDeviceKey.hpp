////////////////////////////////////////////////////////////////////////////////
/// @file           CDeviceKey.hpp
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

#ifndef C_DEVICE_KEY_HPP
#define C_DEVICE_KEY_HPP

#include <string>
#include <iostream>

namespace freedm {
namespace simserv {

class CDeviceKey
{
public:
    CDeviceKey();
    CDeviceKey( const std::string & p_device, const std::string & p_key );

    void SetDevice( const std::string & p_device );
    void SetKey( const std::string & p_key );

    const std::string & GetDevice() const;
    const std::string & GetKey() const;
private:
    std::string m_device;
    std::string m_key;
};

bool operator<( const CDeviceKey & p_lhs, const CDeviceKey & p_rhs );
bool operator==( const CDeviceKey & p_lhs, const CDeviceKey & p_rhs );

std::ostream & operator<<( std::ostream & p_os, const CDeviceKey & p_dkey );

} // namespace simserv
} // namespace freedm

#endif // C_DEVICE_KEY_HPP
