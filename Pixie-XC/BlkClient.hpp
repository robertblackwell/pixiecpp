//
//  Client.hpp
//  Pixie-XC
//
//  Created by ROBERT BLACKWELL on 6/8/16.
//  Copyright © 2016 Blackwellapps. All rights reserved.
//

#ifndef Client_hpp
#define Client_hpp
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
    
    BlkClient();
    BlkClient(std::string url, int port);
    bool    connect(int& status);
    void    setBody(std::string& body);

    void    formatRequest();
    
    bool    executeRequest(int& status);
    
    void close();
    ~BlkClient();
};


#endif /* Client_hpp */
