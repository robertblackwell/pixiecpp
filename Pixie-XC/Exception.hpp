//
//  Exception.hpp
//  Pixie-XC
//
//  Created by ROBERT BLACKWELL on 1/2/16.
//  Copyright © 2016 Blackwellapps. All rights reserved.
//

#ifndef Exception_hpp
#define Exception_hpp

#include <stdio.h>
#include <string>
class Exception : public std::exception
{
    public:
    std::string     message;
    
    Exception();
    ~Exception() _NOEXCEPT;

    Exception(std::string msg);
    
    const char*     what();
    std::string*    getMessage();

};

#endif /* Exception_hpp */
