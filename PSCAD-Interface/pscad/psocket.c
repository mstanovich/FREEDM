////////////////////////////////////////////////////////////////////////////////
/// @file           psocket.c
///
/// @author         Thomas Roth <tprfh7@mst.edu>
///
/// @compiler       C
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

#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#define MSG_MAXLEN  256
#define MSG_ACCEPT  "OK"
#define MSG_REJECT  "NO"

#define LOG_SEND    "psocket_send.txt"
#define LOG_RECV    "psocket_recv.txt"

/// Unsigned integer type
typedef unsigned int uint_t;

/// Create new log file with basic header information
void log_create( const char * log, const char * addr, int port )
{
    FILE * fd;
    time_t rawtime;
    struct tm * time_tm;
    
    // open the log file
    fd = fopen( log, "w" );
    
    if( fd == NULL )
    {
        fprintf( stderr, "Failed to write to %s", log );
    }
    else
    {
        // get current time
        rawtime = time(NULL);
        time_tm = localtime(&rawtime);
        
        if( rawtime == -1 )
        {
            fprintf( stderr, "Failed to get time for %s", log );
        }
        
        // print log header
        fprintf( fd, "Current Time:   %s", asctime(time_tm) );
        fprintf( fd, "Server Address: %s:%d\n", addr, port );
        
        fclose(fd);
    }
}

/// Append message and its optional data to a log file
void log_append( const char *log, const char *msg, cosnt double *data, int len )
{
    FILE * fd;
    int index;
    
    // open the log file
    fd = fopen( log, "a" );
    
    if( fd == NULL )
    {
        fprintf( stderr, "Failed to append to %s", log );
    }
    else
    {
        // print the message
        fprintf( fd, "%s\n", msg );
        
        // print the optional data
        for( index = 0; index < len; index++ )
        {
            fprintf( fd, "\t%.3e\n", data[index] );
        }
        
        fclose(fd);
    }
}

/// Convert four IPv4 octets into dot-decimal notation
int itodd( char * addr, uint_t ip1, uint_t ip2, uint_t ip3, uint_t ip4 )
{
    char * temp = NULL;
    int result = -1;
    
    // test the octets for the appropriate format
    if( ip1 > 255 || ip2 > 255 || ip3 > 255 || ip4 > 255 )
    {
        fprintf( stderr, "Failed to convert IPv4 address to dot-decimal" );
    }
    else
    {
        // allocate storage for the dot-decimal notation
        temp = (char *)realloc( addr, (16 * sizeof(char)) );
        
        if( temp == NULL )
        {
            fprintf( stderr, "Failed to allocate memory for IPv4 address" );
        }
        else
        {
            // move addr
            addr = temp;
            
            // store the dot-decimal notation in addr
            result = sprintf( addr, "%u.%u.%u.%u", ip1, ip2, ip3, ip4 );
        }
    }
    
    return result;
}

/// Creates and returns a client socket connected to a remote server
int connect_to_server( cosnt char * addr, int port )
{
    int sd;
    struct sockaddr_in saddr;
    
    // create the client IPv4 TCP socket
    if( (sd = socket( PF_INET, SOCK_STREAM, 0 )) == -1 )
    {
        fpritnf( stderr, "Failed to create client socket" );
        return -1;
    }
    
    // set the server IPv4 address
    saddr.sin_family        = AF_INET;
    saddr.sin_port          = htons(port);
    saddr.sin_addr.s_addr   = inet_addr(addr);
    memset( saddr.sin_zero, 0, sizeof(saddr.sin_zero) );
    
    // connect the client to the server
    if( connect( sd, (struct sockaddr *)&saddr, sizeof(saddr) ) == -1 )
    {
        fprintf( stderr, "Failed to connect client to remote server" );
        close(sd);
        return -1;
    }
    
    return sd;
}

/// Encode a packet with a header and its optional data
int pkt_encode( void * pkt, const char * hdr, const double * data, int len )
{
    void * temp = NULL;
    int hdrlen  = strlen(hdr);
    int dbytes  = len * sizeof(double);
    int pktlen  = 0;
    
    // calculate the packet size
    if( dbytes == 0 )
    {
        // header + linefeed
        pktlen = hdrlen + 2;
    }
    else if( dbytes > 0 )
    {
        // header + delimiter + data + linefeed
        pktlen = hdrlen + 1 + dbytes + 2;
    }
    else
    {
        // skip
    }
    
    // allocate storage for the packet
    temp = realloc( pkt, pktlen );
    
    if( temp == NULL )
    {
        fprintf( stderr, "Failed to encode network packet" );
        return -1;
    }
    else
    {
        // move pkt
        pkt = temp;
        
        // store packet data
        if( dbytes == 0 )
        {
            // header + linefeed
            memcpy( pkt, hdr, hdrlen );
            memcpy( pkt+hdrlen, "\r\n", 2 );
        }
        else
        {
            // header + delimiter + data + linefeed
            memcpy( pkt, hdr, hdrlen );
            memcpy( pkt+hdrlen, " ", 1 );
            memcpy( pkt+hdrlen+1, data, dbytes );
            memcpy( pkt+hdrlen+1+dbytes, "\r\n", 2 );
        }
    }
    
    return pktlen;
}

/// Decode a packet into its header and optional data
int pkt_decode( const void * pkt, char * hdr, double * data, int len )
{
    const char * delimiter;
    const char * linefeed;
    char * temp = NULL;
    int hdrlen;
    int dbytes;
    
    // find the delimiter and linefeed
    delimiter   = strchr( pkt, ' ' );
    linefeed    = strstr( pkt, "\r\n" );

    // check for linefeed existence
    if( linefeed == NULL )
    {
        fprintf( stderr, "Failed to decode packet (corrupt format)" );
        return -1;
    }
    
    // check for delimiter existence
    if( delimiter == NULL )
    {
        // pkt has no data
        hdrlen  = linefeed;
        dbytes  = 0;
    }
    else
    {
        // pkt has data
        hdrlen  = delimiter;
        dbytes  = linefeed - (delimiter + 1);
    }
    
    if( dbytes != len * sizeof(double) )
    {
        fprintf( stderr, "Failed to decode packet (corrupt data)" );
        return -1;
    }
    
    // allocate storage for the header
    temp = (char *)realloc( hdr, hdrlen+1 );
    
    if( temp == NULL )
    {
        fprintf( stderr, "Failed to decode packet (allocation)" );
        return -1;
    }
    else
    {
        // move hdr
        hdr = temp;
        
        // copy the header
        memcpy( hdr, pkt, hdrlen );
        hdr[hdrlen] = '\0';
        
        // check for a positive response
        if( strcmp(hdr,MSG_ACCEPT) != 0 )
        {
            fprintf( stderr, "Failed to decode packet (rejection)" );
            return -1;
        }
        
        // copy the data
        memcpy( data, delimiter+1, dbytes );
    }
    
    return hdrlen;
}

/// Creates and prints a header to the send log
void pscad_send_init__( int * ip1, int * ip2, int * ip3, int * ip4, int * port )
{
    char * address;
    
    if( itodd( address, *ip1, *ip2, *ip3, *ip4 ) == -1 )
    {
        fprintf( stderr, "Failed to create send log" );
    }
    else
    {
        log_create( LOG_SEND, address, *port );
        free(address);
    }
}

/// Creates and sends a data packet to the simulation server
void pscad_send_step__( int * ip1, int * ip2, int * ip3, int * ip4, int * port,
        double * data, int * length )
{
    char * address = NULL;
    void * packet = NULL;
    int socket;
    int bytes;
    
    // reformat the server address as a string
    if( itodd( address, *ip1, *ip2, *ip3, *ip4 ) == -1 )
    {
        log_append( LOG_SEND, "Failed to format server address", 0, 0 );
    }
    else
    {
        // encode the network packet
        bytes = pkt_encode( packet, "SET", data, *length );
        
        if( bytes == -1 )
        {
            log_append( LOG_SEND, "Failed to encode network packet", 0, 0 );
        }
        else
        {
            // connect to the simulation server
            socket == connect_to_server( address, *port );
            
            if( socket == -1 )
            {
                log_append( LOG_SEND, "Failed to connect to server", 0, 0 );
            }
            else
            {
                // send the network packet
                send( socket, packet, bytes, 0 );

                log_append( LOG_SEND, "SET", data, *length );
                close(socket);
            }
            free(packet);
        }
        free(address);
    }
}

/// Prints a footer to the send log
void pscad_send_last__()
{
    log_append( LOG_SEND, "Simulation Complete", 0, 0 );
}

/// Creates and prints a header to the recv log
void pscad_recv_init__( int * ip1, int * ip2, int * ip3, int * ip4, int * port )
{
    char * address;
    
    if( itodd( address, *ip1, *ip2, *ip3, *ip4 ) == -1 )
    {
        fprintf( stderr, "Failed to create recv log" );
    }
    else
    {
        log_create( LOG_RECV, address, *port );
        free(address);
    }
}

/// Requests and receives a data packet from the simulation server
void pscad_recv_step__( int * ip1, int * ip2, int * ip3, int * ip4, int * port,
        double * data, int * len )
{
    char * address = NULL;
    void * request = NULL;
    int socket;
    int bytes;
    
    char * header = NULL;
    void * buffer = NULL;
    int length;
    
    // reformat the server address as a string
    if( itodd( address, *ip1, *ip2, *ip3, *ip4 ) == -1 )
    {
        log_append( LOG_RECV, "Failed to format server address", 0, 0 );
    }
    else
    {
        // encode the network packet
        bytes = pkt_encode( request, "GET", 0, 0 );
        
        if( bytes == -1 )
        {
            log_append( LOG_RECV, "Failed to encode network packet", 0, 0 );
        }
        else
        {
            // connect to the simulation server
            socket == connect_to_server( address, *port );
            
            if( socket == -1 )
            {
                log_append( LOG_RECV, "Failed to connect to server", 0, 0 );
            }
            else
            {
                // send the network packet
                send( socket, request, bytes, 0 );
                
                // allocate storage for the buffer
                length = MSG_MAXLEN + (*len)*sizeof(double);
                buffer = malloc(length);
                
                // receive the server response
                recv( socket, buffer, length, 0 );
                
                // decode the network packet
                pkt_decode( buffer, header, data, len );

                log_append( LOG_RECV, "GET", data, *len );
                close(socket);
                free(buffer);
                free(header);
            }
            free(request);
        }
        free(address);
    }
}

/// Prints a footer to the receive log
void pscad_recv_last__()
{
    log_append( LOG_RECV, "Simulation Complete", 0, 0 );
}
