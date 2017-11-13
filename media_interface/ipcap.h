#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

typedef struct str_capdev{
  int socket;
  IplImage* image;
  char* buffer;
} IpBasedCapture;

typedef struct str_serverInfo{
  int portNumber;
  char* ipAddress;
} ServerInfo;


int createServerSocket( const char* ipAddress, 
			unsigned short serverPort );

IplImage* queryFrameOverIP( IpBasedCapture* capture );

IpBasedCapture* 
createIpBasedCapture(char* ipAddress, unsigned short serverPort );

void releaseIpBasedCapture( IpBasedCapture* capture );

void loadServerInfo( ServerInfo* info, char* meOrPartner_txt );


/**
 * @arg ipAddress ip address of capture server
 * @arg serverPort port number of the capture server
 */
int createServerSocket( const char* ipAddress,
			unsigned short serverPort ){

  int serverSock;
  struct sockaddr_in serverAddr;
  int addrLen;

  serverSock = socket( AF_INET, SOCK_STREAM, 0 );

  if( serverSock < 0 ) {
    perror( "server: socket" );
    exit( 1 );
  }

  memset( &serverAddr, 0, sizeof( serverAddr ) ) ;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = inet_addr( ipAddress );
  serverAddr.sin_port = htons( serverPort );

  addrLen = sizeof( serverAddr );

  if(connect( serverSock, (struct sockaddr*)&serverAddr, addrLen ) < 0 ){
    perror("server: connect");
    exit( 1 );
  }

  return serverSock;
}

IplImage* queryFrameOverIP( IpBasedCapture* capture ){

  IplImage* image = capture->image;
  int serverSock = capture->socket;
  char* buffer = capture->buffer;
  char dummyBuffer[] = "receive";

  int remaining;

  uchar* data;
  int step;
  CvSize size;

  int eof_flag = 0;
  int error_flag = 0;
  int received = 0; 


  cvGetRawData( image, &data, &step, &size );

  remaining= step * size.height;
  memset( buffer, '\0', remaining );


  if( send( serverSock, dummyBuffer, strlen( dummyBuffer ), 0 )  < 0 ){
    close( serverSock );
    exit( 1 );
  }

  while( remaining > 0 ) {
    int receive_status = 
      recv( serverSock, buffer + received, remaining, 0);

    if( receive_status == 0 ) {
      perror("client: recv 0");
      eof_flag = 1;
      break;
    } else if( receive_status < 0 ) {
      perror("client: recv");
      error_flag = 1;
      break;
    }
    
    received += receive_status;
    remaining -= receive_status;
  }
  
  if( ( eof_flag == 1 ) || ( error_flag == 1 ) ) {
    close( serverSock );
    exit( 1 );
  }
  
  memcpy( data, buffer, received );

  return image;
}

IpBasedCapture* 
createIpBasedCapture(char* ipAddress, unsigned short serverPort ){

  IpBasedCapture* capture 
    = (IpBasedCapture*)malloc( sizeof( IpBasedCapture ) );

  int dataLength = 640 * 480 * 3;
  
  capture->buffer = 
    (char*)malloc( sizeof( char ) * ( dataLength + 1 ) );
  capture->socket = createServerSocket( ipAddress, serverPort );
  capture->image = 
    cvCreateImage( cvSize( 640, 480 ), IPL_DEPTH_8U, 3 );
}

void releaseIpBasedCapture( IpBasedCapture* capture ){
  cvReleaseImage( &(capture->image) );
  free( capture->buffer );
  //shutdown( capture->socket, SHUT_WR );
  //usleep( 50 * 1000 );
  usleep( 50 * 1000 );
}


void loadServerInfo( ServerInfo* info, char* meOrPartner_txt ){
  FILE* file;
  char* buffer = (char*)malloc( sizeof( char ) * 256 );
  char* ipAddress;

  if( ( file = fopen( meOrPartner_txt, "r" ) ) == NULL ){
    fprintf( stderr, "file open error\n" );
    return;
  }
  
  fgets( buffer, 256, file );

  ipAddress = (char*)malloc( sizeof( char ) *  strlen( buffer ) );
  strcpy( ipAddress, buffer );
  
  info->ipAddress = ipAddress;
  //info->ipAddress[strlen(info->ipAddress)-1] = '\0';

  fgets( buffer, 256, file );

  info->portNumber = atoi( buffer );

  //fclose( file );

  free( buffer );

  return;
}
