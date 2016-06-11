#include "Message.h"
#ifndef MSG_SOCKET_INC
#define MSG_SOCKET_INC 1

#define  BLK_READ_STATUS_OK             0
#define  BLK_READ_STATUS_EOF            4 //-   got an EOF and no data
#define  BLK_READ_STATUS_PARSE_ERROR    5 //-   the data was not parsed successfully
#define  BLK_READ_STATUS_IOERROR        6 //-   got an io error on the socket

#define  BLK_WRITE_STATUS_OK            0
#define  BLK_WRITE_STATUS_EOF           4 //-   got an EOF and no data
#define  BLK_WRITE_STATUS_IOERROR       6 //-   got an io error on the socket


class MsgSocket
{
public:
    
    socket_handle_t socket;
    MsgSocket(socket_handle_t sock)
    {
        socket = sock;
    }
    bool readMessage(Message& msg, int* status);
    bool writeMessage(Message& msg, int& status);
    void flush();
    void close();
};


#endif