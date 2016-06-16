
#ifndef ErrorCodes_h
#define ErrorCodes_h

#define SOCKET_STATUS_GOOD              0
#define SOCKET_STATUS_EOF               -1001
#define SOCKET_STATUS_ERROR             -1002
#define SOCKET_STATUS_EAGAIN            -1003

#define  BLK_READ_STATUS_OK             0
#define  BLK_READ_STATUS_EOF            -2004 //-   got an EOF and no data
#define  BLK_READ_STATUS_PARSE_ERROR    -2005 //-   the data was not parsed successfully
#define  BLK_READ_STATUS_IOERROR        -2006 //-   got an io error on the socket

#define  BLK_WRITE_STATUS_OK            0
#define  BLK_WRITE_STATUS_EOF           -2104 //-   got an EOF and no data
#define  BLK_WRITE_STATUS_IOERROR       -2106 //-   got an io error on the socket

#define  HTTP_READ_STATUS_OK             0
#define  HTTP_READ_STATUS_EOF            -5004 //-   got an EOF and no data
#define  HTTP_READ_STATUS_PARSE_ERROR    -5005 //-   the data was not parsed successfully
#define  HTTP_READ_STATUS_IOERROR        -5006 //-   got an io error on the socket

#define  HTTP_WRITE_STATUS_OK            0
#define  HTTP_WRITE_STATUS_EOF           -5004 //-   got an EOF and no data
#define  HTTP_WRITE_STATUS_IOERROR       -5016 //-   got an io error on the socket


#endif /* ErrorCodes_h */
