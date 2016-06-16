
#ifndef HandlerBlk_hpp
#define HandlerBlk_hpp

#include <stdio.h>
#include "Protocol.h"
#include "BlkMessage.h"
#include "BlkSocket.h"

class HandlerBlk
{
public:
    Protocol        protocol;
    bool            pipelining_supported;
    socket_handle_t socket_fd;
    int             id;
    BlkSocket       messageSocket;
    
    HandlerBlk(Protocol protocol, socket_handle_t socket_handle, int _id);
    void handleBlkNormal();
    void handleBlkLoopBack();
private:
    void normalBlkProxy(BlkMessage& msg);
    void tunnelBlkProxy(BlkMessage& msg);
};

#endif
