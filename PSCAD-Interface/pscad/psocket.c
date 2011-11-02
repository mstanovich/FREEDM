////////////////////////////////////////////////////////////////////////////////
/// @file           psocket.c
///
/// @author         Thomas Roth <tprfh7@mst.edu>
///
/// @compiler       C
///
/// @project        Missouri S&T Power Research Group 
///
/// @description    Implementation of PSCAD-Socket functions
///                 Requires the following POSIX headers:
///                 "netdb.h" ; "sys/socket.h"
///
/// @functions
/// itodd( char *, int, int, int, int )
/// create_log( const char *, const char *, int )
/// append_log( const char *, const char *, const double *, int )
/// connect_to_server( const char *, int, const char * )
/// make_packet( const char *, const double *, int )
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

#include "time.h"
#include "stdio.h"
#include "string.h"

#include "netdb.h"
#include "sys/socket.h"

#define MESSAGE_ACCEPT "OK"
#define MESSAGE_REJECT "NO"

////////////////////////////////////////////////////////////////////////////////
/// @brief  Converts an integer IPv4 address into dot-decimal notation
/// @pre    addr must be allocated to store at least 16-bytes
/// @post   addr is modified to hold the conversion result
/// @param  addr Character array to store the dot-decimal notation
/// @param  ip1 First octet of the IPv4 address to convert
/// @param  ip2 Second octet of the IPv4 address to convert
/// @param  ip3 Third octet of the IPv4 address to convert
/// @param  ip4 Fourth octet of the IPv4 address to convert
/// @return strlen(addr), excluding the null-character
////////////////////////////////////////////////////////////////////////////////
int itodd( char * addr, int ip1, int ip2, int ip3, int ip4 )
{
    return sprintf( addr, "%d.%d.%d.%d", ip1, ip2, ip3, ip4 );
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Creates a new log and initializes it with a basic header
/// @pre    log must be writable
/// @post   log is created or erased and filled with a basic header
/// @param  log Filename of the log to create
/// @param  addr Address of the remote server
/// @param  port Port of the remote server  
////////////////////////////////////////////////////////////////////////////////
void create_log( const char * log, const char * addr, int port )
{
    FILE * fd;
    time_t rawtime;
    struct tm * timeinfo;
    
    // attempt to create the given filename
    if( (fd = fopen( log, "w" )) != 0 )
    {
        // get current time
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        
        // print log header
        fprintf( fd, "Current Time:   %s", asctime(timeinfo) );
        fprintf( fd, "Server Address: %s:%d\n", addr, port );
        fclose(fd);
    }
    else
    {
        // unable to create the given filename
        fprintf( stderr, "Unable to create %s", log );
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Appends a message and associated floating-point data to a log file
/// @pre    log must be appendable
/// @pre    data must contain len entries
/// @post   msg and data are appended to the end of the log
/// @param  log Filename of the log to receive the message
/// @param  msg Message to write to the log
/// @param  data Floating-point data to write to the log
/// @param  len Amount of floating-point data to write
////////////////////////////////////////////////////////////////////////////////
void append_log( const char *log, const char *msg, const double *data, int len )
{
    FILE * fd;
    int index;
    
    // attempt to open the given filename
    if( (fd = fopen( log, "a" )) != 0 )
    {
        // print the message
        fprintf( fd, "%s", msg );
        
        // print the data
        for( index = 0; index < len; index++ )
        {
            fprintf( fd, "\t%e", data[index] );
        }

        fprintf( fd, "\n" );
        fclose(fd);
    }
    else
    {
        // unable to open the given filename
        fprintf( stderr, "Unable to open %s", log );
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Creates a new socket and connects it to the remote server
/// @pre    none
/// @post   client socket created to remote server
/// @post   log modified if an error occurs
/// @param  addr Address of the remote server
/// @param  port Port of the remote server
/// @param  log Filename of the log to receive errors
/// @return socket descriptor for the created socket or -1 on error
////////////////////////////////////////////////////////////////////////////////
int connect_to_server( const char * addr, int port, const char * log )
{
    struct hostent * hostname;
    struct sockaddr_in server;
    int client;
    
    // resolve the server hostname
    if( (hostname = gethostbyname(addr)) == 0 )
    {
        append_log( log, hstrerror(h_errno), 0, 0 );
        return -1;
    }
    
    // create the client socket
    if( (client = socket( AF_INET, SOCK_STREAM, 0 )) == -1 )
    {
        append_log( log, strerror(errno), 0, 0 );
        return -1;
    }
    
    // specify server details
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    memcpy( &server.sin_addr, hostname->h_addr_list[0], hostname->h_length );
    
    // connect to the server
    if( connect( client, (struct sockaddr *)&server, sizeof(server) ) == -1 )
    {
        append_log( log, strerror(errno), 0, 0 );
        close(client);
        return -1;
    }
    
    return client;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Allocates a new packet based on the given header and data
/// @pre    header should contain at least one byte of information
/// @pre    len should describe the number of elements in data
/// @post   memory is allocated for the new packet
/// @param  header Header to associate with the new packet
/// @param  data Array of data to associate with the new packet
/// @param  len Length of the array of packet data
/// @return pointer to newly allocated packet or null on error
////////////////////////////////////////////////////////////////////////////////
void * make_packet( const char * header, const double * data, int len )
{
    void * packet;
    int header_size = strlen(header);
    int data_size   = len * sizeof(double);
    int packet_size = header_size + 1 + data_size + 2;    
    
    // allocate new packet
    if( (packet = malloc(packet_size)) != 0 )
    {
        // copy data into the packet
        memcpy( packet, header, header_size );
        memcpy( packet+header_size, " ", 1 );
        memcpy( packet+header_size+1, data, data_size );
        memcpy( packet+header_size+1+data_size, "\r\n", 2 );
    }
    
    return packet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  
/// @pre    
/// @post   
/// @param  
////////////////////////////////////////////////////////////////////////////////
void read_packet( const void *packet, double *data, int len, const char *log )
{
    const void * separator;
    char * header;
    int header_size;
    
    separator = strchr( packet, ' ' );

    if( separator != 0 )
    {
        header_size = separator - packet;
        header = malloc(header_size+1);
        memcpy( header, packet, header_size );
        header[header_size] = '\0';
        
        if( strcmp(header,MESSAGE_ACCEPT) == 0 )
        {
            if( len-header_size == data_length )
            {
                memcpy( data, separator+1, len-header_size, 0 );
            }
            else
            {
                append_log( log, "Received a corrupt piece of data", 0, 0 );
            }
        }
        else if( strcmp(header,MESSAGE_REJECT) == 0 )
        {
            // rejected message
            append_log( log, separator+1, len-header_size, 0 );
        }
        else
        {
            // unrecognized packet header, interpret as corrupt
            append_log( log, "Received a corrupt packet header", 0, 0 );
        }
        
        free(header);
    }
    else
    {
        // packet contains no space, interpret as corrupt
        append_log( log, "Received a corrupt packet", 0, 0 );
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  
/// @pre    
/// @post   
/// @param  
////////////////////////////////////////////////////////////////////////////////
int send_packet( int sd, const void * data, int bytes, const char * file )
{
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  
/// @pre    
/// @post   
/// @param  
////////////////////////////////////////////////////////////////////////////////
int recv_packet( int sd, void * data, int bytes, const char * file )
{
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  
/// @pre    
/// @post   
/// @param  
////////////////////////////////////////////////////////////////////////////////
void pscad_send_zero__()
{
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  
/// @pre    
/// @post   
/// @param  
////////////////////////////////////////////////////////////////////////////////
void pscad_send_step__()
{
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  
/// @pre    
/// @post   
/// @param  
////////////////////////////////////////////////////////////////////////////////
void pscad_send_last__()
{
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  
/// @pre    
/// @post   
/// @param  
////////////////////////////////////////////////////////////////////////////////
void pscad_recv_zero__()
{
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  
/// @pre    
/// @post   
/// @param  
////////////////////////////////////////////////////////////////////////////////
void pscad_recv_step__()
{
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  
/// @pre    
/// @post   
/// @param  
////////////////////////////////////////////////////////////////////////////////
void pscad_recv_last__()
{
}

// recv_zero    create receive log
// recv_step    attempt to receive data
// recv_last    terminate receive log

// send_zero    create send log ; send reset message
// send_step    attempt to send data
// send_last    terminate send log ; (send quit message?)
//  may as well send quit even if it isn't used at the moment

// recognized message types:
//  SET, GET, EXIT, RESET