#ifndef MESSAGE_INC
#define MESSAGE_INC 1
#include <vector>

//
// Block protocol
//
// This is a description of what I call the "block protocol", This is a simple protocol
// that I invented so that I could develop a MITM proxy without having to worry about
// all the complexities of HTTP/HTTPS message formats.
//
// The intention is to have a simple protocol that provides the essential processing complexities
// of HTTP(S) but without the parsing complexities.
//
// The format of a block message is :
//
//          <START>\n<destination_port>\n<request verb>\n<body_length>\n<body>
//
//      <START>             -   is simply the text "START"
//      <destination port>  -   a string of ascii digits that is the port number of the destination server
//      <request verb>      -   one of
//                                  "NORMAL" -  pass the message to the server and give me the response
//                                  "TUNNEL" -  set up a tunnel tot he server and acknowledge that this can be
//                                              done BEFORE starting tunneling data. This is to simulate how TLS/SSL
//                                              works in a proxy environment
//      <body_length>       -   a string of ascii digits indicating the length of the message body
//      <body>              - string of UTF8 characters
//
//



class BlkMessage
{
public:
    std::string     firstLine;
    int             destination_port;
    std::string     request_verb;
    int             messageLength;
    std::string     body;
};


class BlkParser
{
public:
    int                         lineCount;
    BlkMessage&                 msg;
    int                         msgLength;
    int                         destinationPort;
    bool                        messageComplete;
    std::string                 actionVerb;
    std::string                 currentLine;
    std::string                 currentBuffer;
    std::string                 body;
    std::vector<std::string>    lines;
    bool                        parseError;
    std::string                 errorMessage;
    std::string                 rawMsg;
    
    BlkParser(BlkMessage& message);
    void    error(std::string m);
    int     append(void* buf, int n);
    void    nextLine();
    void    finishedParsing();
    bool    parseInt(std::string s, int& value);
    void    parseCharacter( char ch);
};



#endif