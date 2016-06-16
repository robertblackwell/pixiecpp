
#ifndef BlkClient_hpp
#define BlkClient_hpp
#
#include <stdio.h>
#include <string>
#include "BlkMessage.h"
#include "BlkSocket.h"

class BlkClient
{
public:
    std::string             host;
    std::string             url;
    int                     port;
    int                     socket;
    
    BlkMessage              requestMessage{};
    BlkMessage              responseMessage{};
    BlkSocket               messageSocket{};
    
    BlkClient(std::string _host, std::string _url, int _port);
    BlkClient(std::string _host, std::string _url, int _port, BlkMessage& _requestMessage);
    
    bool    connect(int& status);
    
    bool    executeRequest(int& status);
    
    void close();
    ~BlkClient();
};


#endif /* Client_hpp */
