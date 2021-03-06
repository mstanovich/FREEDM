////////////////////////////////////////////////////////////////////////////////
/// @file       CTableRTDS.hpp
///
/// @author     Thomas Roth <tprfh7@mst.edu>
///
/// @project    FREEDM DGI
///
/// @description
///     Defines a table of device variables defined by an XML input file.
///
/// @copyright
///     These source code files were created at Missouri University of Science
///     and Technology, and are intended for use in teaching or research. They
///     may be freely copied, modified, and redistributed as long as modified
///     versions are clearly marked as such and this notice is not removed.
///     Neither the authors nor Missouri S&T make any warranty, express or
///     implied, nor assume any legal responsibility for the accuracy,
///     completeness, or usefulness of these files or any information
///     distributed with these files. 
///     
///     Suggested modifications or questions about these files can be directed
///     to Dr. Bruce McMillin, Department of Computer Science, Missouri
///     University of Science and Technology, Rolla, MO 65409 <ff@mst.edu>.
////////////////////////////////////////////////////////////////////////////////

#ifndef C_TABLE_RTDS_HPP
#define C_TABLE_RTDS_HPP

#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include <boost/thread/shared_mutex.hpp>

#include "CDeviceKeyCoupled.hpp"
#include "CTableStructure.hpp"
#include "types/IDevice.hpp"

namespace freedm {
namespace broker {
namespace device {

class CRtdsAdapter;

////////////////////////////////////////////////////////////////////////////////
/// @brief Provides storage for data obtained from RTDS or commands to send.
/// 
/// @description The table class stores a set of data indexed by device-key
///  combo object. Its internal structure is defined by an XML file passed to 
///  the constructor.
///
/// @limitations None.
////////////////////////////////////////////////////////////////////////////////
class CTableRTDS
{
public:
    /// constructor
    CTableRTDS(const std::string & p_xml, const std::string & p_tag);

    /// set value in command table
    void SetValue(const CDeviceKeyCoupled & p_dkey, SettingValue p_value);

    /// get value from state table
    SettingValue GetValue(const CDeviceKeyCoupled & p_dkey) const;

    ~CTableRTDS();

    friend class CRtdsAdapter;
private:
    /// manages the XML specification
    CTableStructure m_structure;

    /// read-write mutex for m_data
    mutable boost::shared_mutex m_mutex;

    /// actual values. Notice this is in float type
    float * m_data;

    /// number of m_data elements
    size_t m_length;
};

}//namespace broker
} // namespace freedm
}// namespace device

#endif // C_TABLE_RTDS_HPP
