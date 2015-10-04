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


// for phase
#define SLOW_START 0
#define CONGESTION_AVOID 1
#define FAST_RECOVERY 2
// 
#define INIT_SEQ_NUM 1
// for ack_flag
#define FLAG_ACK 0
#define FALG_DATA 1

typedef char BYTE;

// pkt max length = 1500-20(ip)-20(udp) = 1460
// now the pkt size is 988+10 bytes, 1KB
struct pkt{
    int seq_num;
    int ack_num;
    int ack_flag;
    BYTE payload[988];
};


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
