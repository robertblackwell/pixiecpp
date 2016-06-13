#include <iostream>
#include <deque>
#include <cstdlib>
#include <pthread.h>
#include <string>
#include <cassert>

#include "Logger.h"
#include "BlkMessage.h"


BlkParser::BlkParser(BlkMessage& message): msg(message)
{
    messageComplete = false;
    lineCount = 0;
    destinationPort = -1;
    actionVerb = "";
    currentBuffer = "";
    currentLine = "";
    body = "";
    errorMessage = "";
    parseError = false;
    
}
void BlkParser::error(std::string m)
{
    errorMessage = m;
    parseError = true;
}
void BlkParser::append(void* buf, int n)
{
    int bytes_consumed = 1;
    for(int i = 0; i < n; i++){
        parseCharacter(((char*)buf)[i]);
        
        if( parseError )
            return;// bytes_consumed;
        
        if( messageComplete )
            return;// bytes_consumed;
        bytes_consumed++;
    }
    return;// -1;
}
void BlkParser::nextLine()
{
    lines.push_back(currentLine);
    currentLine = "";
    lineCount++;
}
void BlkParser::finishedParsing()
{
    msg.firstLine = lines[0];
    msg.destination_port = destinationPort;
    msg.request_verb = actionVerb;
    msg.messageLength = msgLength;
    msg.body = body;
    //std::cout << "Parser::finishedParsing body : " << body << std::endl;
    
    messageComplete = true;
}
bool BlkParser::parseInt(std::string s, int& value)
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
void BlkParser::parseCharacter( char ch)
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
                error("destination port must be numeric ");
            } else if(lineCount == 1){
                destinationPort = val;
            }
            nextLine();
        }else{
            currentLine += ch;
        }
    }else if( lineCount == 2 ){
        if( ch == '\n'  ){
            if ( currentLine != "NORMAL" && currentLine != "TUNNEL" ){
                error("verb line must be text 'NORMAL' or 'TUNNEL'");
            }
            actionVerb = currentLine;
            nextLine();
        } else {
            currentLine += (char)toupper( ch);
            int len = (int)currentLine.length();
            std::string data = currentLine;
            std::transform(data.begin(), data.end(), data.begin(), ::tolower);
            
            if(
               currentLine != std::string("NORMAL").substr(0, len )
               &&
               currentLine != std::string("TUNNEL").substr(0, len )
               )
                error("invalid verb to message ["+ currentLine +"]");
        }
    } else if( lineCount == 3 ){
        if( ch == '\n' ){
            int val;
            bool validInt = parseInt(currentLine, val);
            if( ! validInt ){
                error("second line must be numeric - message body length");
            } else if(lineCount == 3){
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

