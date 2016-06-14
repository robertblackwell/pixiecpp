#pixiecpp 

This is an experiment in networking.

Ultimately I want this to be a Man-In-The-Middle style proxy for http/https but for the moment I am doing the implementation
using a mickey-mouse protocol that I call "BLK".

-	To be in any way useful a mitm proxy must be able to handle the typical request/response style activity of say http, plus
-	must provide a "tunnel" style of operation (where data is passed simultaneously in each direction between client and server
	 without parsing into "message") so as to provide minimal support for the http CONNECT request when client and server 
	 are doing __https__.
	 
This particular implementation is using cpp and threads. Thus there is a listener thread and a thread for each active connection.

For the "tunnel" feature I initially used blocking I/O and added an additional thread to each connection and used a separate
thread for each direction. That is "tunnel" connections had two threads one handing ```client ---> server``` and the otehr handling
```server-->client``` traffic.

However this did not work very well as I kept getting EAGAIN errors on the sockets. This is most likely a bug in my code
that I simply could not find. But it is also possibly a result of two independent threads doing IO on the same two sockets
(file descriptors) at the same time thereby creating some type of kernel contention.

On the basis of this last speculation I re-implemented the "tunnel" feature __WITHOUT__ the additional thread by
using the __select__ system call and __non-blocking__ IO. This approach ensures that for a single connection
all activity on the connections sockets are __"single threaded"__. This implementation seems to work. BUT I need
to do a lot more testing before calling it done.

Incidentally the select implementation is available in my githyb repo __twc__.