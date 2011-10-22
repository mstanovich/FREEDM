////////////////////////////////////////////////////////////////////////////////
/// @file           CSimulationInterface.cpp
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

#include "CSimulationInterface.hpp"

namespace simserv {

/// Creates a simulation interface
CSimulationInterface::CSimulationInterface( unsigned short p_port,
    CDeviceTable & p_command, CDeviceTable & p_state )
    : IWorkerInterface(p_port)
    , m_CyberCommand(p_command)
    , m_SimulationState(p_state)
{
    // skip
}

/// Message handler for client socket connections
std::string CSimulationInterface::HandleMessage( const std::string & p_message )
{
    std::stringstream message(p_message);
    std::string header, data;
    
    try
    {
        message >> header;
        if( header == "GET" )
        {
            data = BuildString( m_CyberCommand.GetVector() );
        }
        else if( header == "SET" )
        {
            std::getline( message, data );
            m_SimulationState.SetVector( BuildVector(data) );
        }
        else if( header == "RST" )
        {
            m_SimulationState.ResetVector();
        }
        else
        {
            throw;
        }
        
        data = "SUCCESS " + data;
    }
    catch( std::exception & e )
    {
        // placeholder
        data = "ERROR ";
        data = data + e.what();
    }
    
    return data;
}

/// Build a string representation of vector values
std::string CSimulationInterface::BuildString(
    const std::vector<TSimulationValue> & p_v ) const
{
    size_t i = 0;
    size_t n = p_v.size();
    std::string result = "";
    
    while( i < n )
    {
        // append the value to the result
        result += boost::lexical_cast<std::string>(p_v[i]);
        
        ++i;
        
        // append delimiter
        if( i < n )
        {
            result += " ";
        }
    }
    
    return result;
}

/// Build a vector representation of string values
std::vector<TSimulationValue> CSimulationInterface::BuildVector(
    const std::string & p_s ) const
{
    std::vector<TSimulationValue> result;
    std::stringstream stream(p_s);
    std::string entry;
    
    while( stream >> entry )
    {
        // append the string to the result
        result.push_back( boost::lexical_cast<TSimulationValue>(entry) );
    }
    
    return result;
}

} // namespace simserv
