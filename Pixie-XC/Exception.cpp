//
//  Exception.cpp
//  Pixie-XC
//
//  Created by ROBERT BLACKWELL on 1/2/16.
//  Copyright © 2016 Blackwellapps. All rights reserved.
//
#include <cstdlib>
#include "Exception.hpp"
#include <string>
#include <stdexcept>

Exception::Exception()
{
    message = "this is default";
}
Exception::Exception(std::string msg)
{
    message = msg;
}
Exception::~Exception() _NOEXCEPT
{

}
const char* Exception::what()
{
    std::string ss = "So this is an xception";
    const char* cstr = ss.c_str();
    int len = (int) strlen(cstr);
    char* buffer = (char*) malloc(len+1);
    strcpy(buffer, cstr);
    return buffer;
//    char* m;
//    int ln = asprintf(&m, "Pixie exception %s \n ", message.c_str());
//    return m;
}