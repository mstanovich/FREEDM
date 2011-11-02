////////////////////////////////////////////////////////////////////////////////
/// @file           CDeviceTable.hpp
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

#ifndef C_DEVICE_TABLE_CPP
#define C_DEVICE_TABLE_CPP

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

#include <boost/thread/shared_mutex.hpp>

#include "CDeviceKey.hpp"
#include "CTableStructure.hpp"

namespace freedm {
namespace simserv {

class CDeviceTable
{
public:
    typedef double TValue;
    
    CDeviceTable();
    CDeviceTable( const std::string & p_xml, const std::string & p_tag );
    void ReadFile( const std::string & p_xml, const std::string & p_tag );

    void SetData( std::vector<TValue> p_data );
    void SetValue( const CDeviceKey & p_dkey, TValue p_value );
    
    std::vector<TValue> GetData();
    TValue GetValue( const CDeviceKey & p_dkey );

    std::ostream & PrintTable( std::ostream & p_os ) const;
private:
    typedef boost::unique_lock<boost::shared_mutex> TWriteLock;
    typedef boost::shared_lock<boost::shared_mutex> TReadLock;

    std::vector<TValue> m_data;
    boost::shared_mutex m_mutex;
    
    CTableStructure m_structure;
};

std::ostream & operator<<( std::ostream & p_os, const CDeviceTable & p_table );

} // namespace simserv
} // namespace freedm

#endif // C_DEVICE_TABLE_CPP
