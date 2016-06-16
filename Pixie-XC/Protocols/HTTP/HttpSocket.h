#include "HttpMessage.h"
#ifndef HTTP_MSG_SOCKET_INC
#define HTTP_MSG_SOCKET_INC 1

#ifndef HTTP_READ_STATUS_OK
#define  HTTP_READ_STATUS_OK             0
#define  HTTP_READ_STATUS_EOF            -5004 //-   got an EOF and no data
#define  HTTP_READ_STATUS_PARSE_ERROR    -5005 //-   the data was not parsed successfully
#define  HTTP_READ_STATUS_IOERROR        -5006 //-   got an io error on the socket

#define  HTTP_WRITE_STATUS_OK            0
#define  HTTP_WRITE_STATUS_EOF           -5004 //-   got an EOF and no data
#define  HTTP_WRITE_STATUS_IOERROR       -5016 //-   got an io error on the socket
#endif

class HttpSocket
{
public:
    
    socket_handle_t socket;
    HttpSocket();
    HttpSocket(socket_handle_t sock)
    {
        socket = sock;
    }
    bool readMessage(HttpMessage& msg, int& status);
    bool writeMessage(HttpMessage& msg, int& status);
	
	void	onReadMessage(std::function<void(HTTPMessage* message)> cb);
	void	onReadParseError(std::function<void()> cb);
	void	onReadIOError(std::function<void(int status)> cb);
	void	onReadEOF(std::function<void(int status)> cb);
	
	std::function<void(HTTPMessage* message)>	readMessageCB;
	std::function<void()>						parseErrorCB;    
	std::function<void(int status)>				ioErrorCB;    
	std::function<void(int status)>				eofCB;    
	
	void flush();
    void close();
};


#endif