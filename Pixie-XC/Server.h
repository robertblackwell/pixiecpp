#include "Protocol.h"
class Worker;

class Server
{
public:
    int                     port;
    unsigned                nbr_workers;
    Worker**                worker_array;
    std::vector<Worker*> *  worker_vector;
    Protocol                protocol;
    
    Server(Protocol protocol, unsigned _nbr_workers);
    void listen(int _port);
};

//Server* createServer(unsigned nbr_workers);