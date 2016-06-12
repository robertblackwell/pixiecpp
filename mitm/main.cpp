//
//  main.cpp
//  Pixie-XC
//
//  Created by ROBERT BLACKWELL on 12/27/15.
//  Copyright © 2015 Blackwellapps. All rights reserved.
//
#include <iostream>
#include <deque>
#include <cstdlib>
#include <pthread.h>
#include <string>
#include "Protocol.h"
#include "Exception.hpp"

#include "Logger.h"
INITIALIZE_EASYLOGGINGPP

#include "Server.h"

#define NUM_THREADS 15

int main(int argc, const char * argv[])
{
    
    el::Configurations defaultConf;
    defaultConf.setToDefault();
    //    defaultConf.set(el::Level::Info, el::ConfigurationType::Enabled, "false");
    defaultConf.set(el::Level::Debug, el::ConfigurationType::Enabled, "true");
    defaultConf.set(el::Level::Global, el::ConfigurationType::Format, "%level|%func[%fbase:%line] : %msg");
    el::Loggers::reconfigureAllLoggers(defaultConf);
    
    try
    {
        Server server{Protocol::BLK_PROXY, 20};
        server.listen(8001);
    }
    catch(Exception e)
    {
        std::cerr << "Abnornal Termination" <<  e.what() << std::endl;
    }
    catch(...)
    {
        std::cerr << "Abnornal Termination" << std::endl;
    }
    return 0;
}
