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

namespace simserv {

/// Associates a simulation variable with the physical device it describes
class CDeviceKey
{
public:
    /// Constructs an empty device key
    CDeviceKey();
    
    /// Constructs a (device,key) pair from the given arguments
    CDeviceKey( const std::string & p_device, const std::string & p_key );
    
    /// Sets the physical device identifier
    void SetDevice( const std::string & p_device );
    
    /// Sets the simulation variable key
    void SetKey( const std::string & p_key );
    
    /// Gets the physical device identifier
    const std::string & GetDevice() const;
    
    /// Gets the simulation variable key
    const std::string & GetKey() const;
private:
    /// Unique physical device identifier
    std::string m_device;
    
    /// Device variable of interest
    std::string m_key;
};

/// Determines if the left device key is less than the right
bool operator<( const CDeviceKey & p_lhs, const CDeviceKey & p_rhs );

/// Determines if two device keys are equivalent
bool operator==( const CDeviceKey & p_lhs, const CDeviceKey & p_rhs );

/// Outputs a device key to the given otuput stream
std::ostream & operator<<( std::ostream & p_os, const CDeviceKey & p_dkey );

} // namespace simserv

#endif // C_DEVICE_KEY_HPP
