class Worker;

class Server
{
public:
    int                     port;
    unsigned                nbr_workers;
    Worker**                worker_array;
    std::vector<Worker*> *  worker_vector;
    
    Server(unsigned _nbr_workers);
    void listen(int _port);
};

//Server* createServer(unsigned nbr_workers);