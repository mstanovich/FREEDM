////////////////////////////////////////////////////////////////////////////////
/// @file           CSimulationInterface.hpp
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

#ifndef C_SIMULATION_INTERFACE_HPP
#define C_SIMULATION_INTERFACE_HPP

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include <boost/lexical_cast.hpp>

#include "logger.hpp"
#include "common.hpp"
#include "CDeviceKey.hpp"
#include "CDeviceTable.hpp"
#include "IWorkerInterface.hpp"

CREATE_EXTERN_STD_LOGS()

namespace simserv {

/// Defines a threaded socket interface to a power simulation
class CSimulationInterface : public IWorkerInterface
{
public:
    /// Creates a simulation interface
    CSimulationInterface( unsigned short p_port,
        CDeviceTable & p_command, CDeviceTable & p_state );
    
    /// Message handler for client socket connections
    virtual std::string HandleMessage( const std::string & p_message );
    
    /// Deconstructor for derived classes
    virtual ~CSimulationInterface() {}
private:
    /// Build a string representation of vector values
    std::string BuildString( const std::vector<TSimulationValue> & p_v ) const;
    
    /// Build a vector representation of string values
    std::vector<TSimulationValue> BuildVector( const std::string & p_s ) const;
    
    /// External cyber commands to the simulation
    CDeviceTable & m_CyberCommand;
    
    /// Current state of the power simulation
    CDeviceTable & m_SimulationState;
};

} // namespace simserv

#endif // C_SIMULATION_INTERFACE_HPP
