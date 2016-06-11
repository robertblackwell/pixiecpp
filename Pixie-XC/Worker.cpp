#include <iostream>
#include <deque>
#include <cstdlib>
#include <pthread.h>
#include <string>
#include <cassert>
#include <unistd.h>

#include "Logger.h"
#include "socket_functions.hpp"
#include "Queue.h"
#include "Worker.h"
#include "Statistics.h"
#include "Message.h"
#include "MsgSocket.h"
#include "SocketHandler.hpp"

Worker::Worker(Queue& _queue, int _id): queue(_queue)
{
    active_socket = 0;
    active  = false;
    id = _id;
}
Worker::~Worker()
{
    std::cout << "Worker::~Worker called" << std::endl;
}

void Worker::main()
{
    while(true)
    {
        active = false;
        int mySocketHandle = queue.remove();
        active_socket = (int) mySocketHandle;
        try
        {
            SocketHandler handler{mySocketHandle, id};
            handler.handle();
        }
        catch (std::exception e)
        {
            std::cout << "Worker - exception from socket handler" << std::endl;
        }
#ifdef HGHGH
        MsgSocket myMessageSocket(mySocketHandle);
        
        count = 0;
        LOG(DEBUG) << "worker id : " << id << " item: " << mySocketHandle << " socket : " << active_socket << std::endl;
        
        while(true)
        {
            count++;
            active = true;
            LOG(DEBUG)  << "worker id : "
                        << id << " socket: "
                        << mySocketHandle
                        << " count : "
                        << count
                        << std::endl;
            Message msg;
            int status;
            bool gotMessage = myMessageSocket.readMessage(msg, &status);
            if( gotMessage  )
            {
                // process the message and send response
                int status;
                bool res = myMessageSocket.writeMessage(msg, status);
                
            }
            else if( status == BLK_READ_STATUS_EOF)
            {
                // orderly close the connection
                myMessageSocket.flush();
                break;
            }
            if(status == BLK_READ_STATUS_PARSE_ERROR)
            {
                // close the connection, possibly after sending an error response
                myMessageSocket.flush();
                break;
            }
            else if( status == BLK_READ_STATUS_IOERROR)
            {
                // close the connection
                break;
            }
        }
        myMessageSocket.close();
#endif
        close(mySocketHandle);
    }
    
}

