#ifndef MESSAGE_INC
#define MESSAGE_INC 1

class Message
{
public:
    std::string     firstLine;
    int             messageLength;
    std::string     body;
};


class Parser
{
public:
    int                         lineCount;
    Message&                    msg;
    int                         msgLength;
    bool                        messageComplete;
    std::string                 currentLine;
    std::string                 currentBuffer;
    std::string                 body;
    std::vector<std::string>    lines;
    bool                        parseError;
    std::string                 errorMessage;
    
    Parser(Message& message);
    void error(std::string m);
    void append(void* buf, int n);
    void nextLine();
    void finishedParsing();
    bool parseInt(std::string s, int& value);
    void parseCharacter( char ch);
};



#endif