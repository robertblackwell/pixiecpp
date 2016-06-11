#include <iostream>
#include <deque>
#include <cstdlib>
#include <pthread.h>
#include <string>
#include <cassert>

#include "Logger.h"
#include "Message.h"


Parser::Parser(Message& message): msg(message)
{
    messageComplete = false;
    lineCount = 0;
    currentBuffer = "";
    currentLine = "";
    body = "";
    errorMessage = "";
    parseError = false;
    
}
void Parser::error(std::string m)
{
    errorMessage = m;
    parseError = true;
}
void Parser::append(void* buf, int n)
{
    for(int i = 0; i < n; i++){
        parseCharacter(((char*)buf)[i]);
        
        if( parseError )
            return;
        
        if( messageComplete )
            return;
    }
}
void Parser::nextLine()
{
    lines.push_back(currentLine);
    currentLine = "";
    lineCount++;
}
void Parser::finishedParsing()
{
    msg.firstLine = lines[0];
    msg.messageLength = msgLength;
    msg.body = body;
    //std::cout << "Parser::finishedParsing body : " << body << std::endl;
    
    messageComplete = true;
}
bool Parser::parseInt(std::string s, int& value)
{
    try
    {
        value = std::stoi(s);
        return true;
    }
    catch(...)
    {
        return false;
    }
}
void Parser::parseCharacter( char ch)
{
    currentBuffer += ch;
    if( lineCount == 0 ){
        if( ch == '\n'  ){
            if (currentLine != "START" ){
                error("first line must be text 'START'");
            }
            nextLine();
        } else {
            currentLine += (char)toupper( ch);
            int len = (int)currentLine.length();
            std::string data = currentLine;
            std::transform(data.begin(), data.end(), data.begin(), ::tolower);

            if( currentLine != std::string("START").substr(0, len ))
                error("invalid start to message ["+ currentLine +"]");
                }
    } else if( lineCount == 1 ){
        if( ch == '\n' ){
            int val;
            bool validInt = parseInt(currentLine, val);
            if( ! validInt ){
                error("second line must be numeric - message body length");
            } else if(lineCount == 1){
                msgLength = val;
            }
            nextLine();
        }else{
            currentLine += ch;
        }
    }else{
        if( body.length() >= msgLength ){
            error("message body too long");
        }
        body += ch;
        if( body.length() == msgLength ){
            finishedParsing();
        }
    }
}

