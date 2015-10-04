#ifndef UDP_SERVER
#define UDP_SERVER

#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <stdio.h>
#include <sys/time.h>
#include <arpa/inet.h>


class UDPServer{
public:
    UDPServer(std::string port);
    virtual ~UDPServer();
    void work();

private:
    struct addrinfo hints;
    struct addrinfo *serverinfo;
    struct sockaddr_storage client_addr;
    socklen_t addr_len;
    int serverFD;
    char recv_buffer[1500];
};


#endif // UDP_SERVER
