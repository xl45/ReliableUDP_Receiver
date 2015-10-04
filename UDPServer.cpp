#include "UDPServer.h"


UDPServer::UDPServer(std::string port){
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    // getaddrinfo
    int status;
    if( (status = getaddrinfo(NULL, port.c_str(), &hints, &serverinfo)) != 0 ){
        std::cout << "getaddrinfo() function error:" << gai_strerror(status) << std::endl;
        exit(1);
    }

    // get fd
    if( (serverFD = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol)) == -1){
        perror("receiver socket(): ");
        exit(1);
    }

    // bind to port
    if( bind(serverFD, serverinfo->ai_addr, serverinfo->ai_addrlen) == -1 ){
        perror("receiver bind(): ");
        exit(1);
    }
}


void UDPServer::work(){
    while(1){
        memset(&recv_buffer, 0, 1500);
        addr_len = sizeof(client_addr);

        if( (recvfrom(serverFD, recv_buffer, 1500, 0, (struct sockaddr*)&client_addr, &addr_len)) == -1 ){
            perror("receiver recvfrom(): ");
            exit(1);
        }

        std::cout << "receiver receive: \n" << recv_buffer << std::endl << std::endl; 
    }

    close(serverFD);
}


UDPServer::~UDPServer(){
}
