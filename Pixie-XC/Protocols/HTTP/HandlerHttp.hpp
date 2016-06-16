
#ifndef HandlerHttp_hpp
#define HandlerHttp_hpp

#include <stdio.h>
#include "Protocol.h"
#include "HttpMessage.h"
#include "HttpSocket.h"

class HandlerHttp
{
public:
    Protocol        protocol;
    bool            pipelining_supported;
    socket_handle_t socket_fd;
    BlkSocket       messageSocket;
    int             id;
    HandlerHttp(Protocol protocol, socket_handle_t socket_handle, int _id);
    void handleHttpNormal();
    void handleLoopBack();
private:
    void normalHttpProxy(BlkMessage& msg);
    void tunnelHttpProxy(BlkMessage& msg);
};

#endif 
