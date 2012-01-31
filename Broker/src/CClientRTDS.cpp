////////////////////////////////////////////////////////////////////////////////
/// @file           CClientRTDS.cpp
///
/// @author         Thomas Roth <tprfh7@mst.edu>
///
/// @compiler       C++
///
/// @project        Missouri S&T Power Research Group
///
/// @see            CClientRTDS.hpp
///
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
///
////////////////////////////////////////////////////////////////////////////////

#include "CClientRTDS.hpp"

//check endianess at compile time.  Middle-Endian not allowed
#if __BYTE_ORDER == __LITTLE_ENDIAN
#elif __BYTE_ORDER == __BIG_ENDIAN
#else
	#error "unsupported endianness or __BYTE_ORDER not defined"
#endif

#define TIMESTEP 50  //in microseconds

namespace freedm{
  namespace broker{

      CClientRTDS::RTDSPointer CClientRTDS::Create( boost::asio::io_service & p_service, std::string p_configFile )
{
    return CClientRTDS::RTDSPointer( new CClientRTDS(p_service, p_configFile) );
}

      CClientRTDS::CClientRTDS( boost::asio::io_service & p_service, std::string p_configFile )
          : m_socket(p_service), m_fileName(p_configFile), m_GlobalTimer(p_service)
{
     //get buffer sizes by reading the FPGA configuration file
    rx_size = 0 ;  //random right now
    tx_size = 0 ;

    // using the configuaration file to construct the two tables
}

      //FPGA's hostname and port number resolved and passed in by PosixMain
bool CClientRTDS::Connect( const std::string p_hostname, const std::string p_port )
{
    boost::asio::ip::tcp::resolver resolver( m_socket.get_io_service() );
    boost::asio::ip::tcp::resolver::query query( p_hostname, p_port );
    boost::asio::ip::tcp::resolver::iterator it = resolver.resolve(query);
    boost::asio::ip::tcp::resolver::iterator end;
    
    // attempt to connect to one of the resolved endpoints
    boost::system::error_code error = boost::asio::error::host_not_found;
    while( error && it != end )
    {
        m_socket.close();
        m_socket.connect( *it, error );
        ++it;
    }
    if( error )
    {
        throw boost::system::system_error(error);
    }
    
    return( it != end );
}

void CClientRTDS::Run() 
{   
            //Always send data to FPGA first
            boost::shared_lock<boost::shared_mutex> lock(m_cmdData.m_mutex);
            Logger::Debug << "Client_RTDS - obtained mutex as reader" << std::endl;   
            
            memcpy(txBuffer, m_cmdData, tx_size);
            boost::shared_lock<boost::shared_mutex> unlock(m_cmdData.m_mutex);
            Logger::Debug << "Client_RTDS - released reader mutex" << std::endl;

            // FPGA will send values in big-endian byte order
            // If host machine is in little-endian byte order, convert to little-endian
            #if __BYTE_ORDER == __LITTLE_ENDIAN
            endian_swap((char *)&txBuffer, tx_size);
            #endif
         
            // send to FPGA
            boost::asio::write( m_socket, boost::asio::buffer(txBuffer, tx_size) );
           

            //Receive data from FPGA next
            boost::asio::read( m_socket, boost::asio::buffer(rxBuffer, rx_size) );

            // FPGA will send values in big-endian byte order
            // If host machine is in little-endian byte order, convert to little-endian
            #if __BYTE_ORDER == __LITTLE_ENDIAN
            endian_swap((char *)&rxBuffer, rx_size);
            #endif
           
            boost::unique_lock<boost::shared_mutex> lock(m_stateData.m_mutex);
            Logger::Debug << "Client_RTDS - obtained mutex as writer" << std::endl;

            memcpy(m_stateData, rxBuffer, rx_size);
            boost::shared_lock<boost::shared_mutex> unlock(m_stateData.m_mutex);
            Logger::Debug << "Client_RTDS - released writer mutex" << std::endl;
          
            //Start the timer; on timeout, this function is called again
            m_GlobalTimer.expires_from_now( boost::posix_time::microseconds(TIMESTEP) );
            m_GlobalTimer.async_wait( boost::bind(&CClientRTDS::Run, this,
                                                  boost::asio::placeholders::error)); 

}

void CClientRTDS::Set( const std::string p_device, const std::string p_key,
    const std::string p_value )
{
    //access and write to table
}

std::string CClientRTDS::Get( const std::string p_device, const std::string p_key )
{
    //access and write to table
    //return value;
}

void CClientRTDS::Quit()
{
    
    // close connection
    m_socket.close();
}

CClientRTDS::~CClientRTDS()
{
    //  perform teardown
    if( m_socket.is_open() )
    {
        Quit();
    }
}

void CCiientRTDS::endian_swap(char *data, const int num_bytes)
{
   char tmp[num_bytes];

   for(int i=0; i<num_bytes; ++i)
      tmp[i] = data[num_bytes - 1 - i];

   for(int i=0; i<num_bytes; ++i)
      data[i] = tmp[i];
}


  }//namespace broker
}//namespace freedm
