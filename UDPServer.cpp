#include "UDPServer.h"


UDPServer::UDPServer(std::string port){
    reqNum = 1;

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
    struct pkt temp1;
    struct pkt temp2;

    while(1){
        memset(&recv_buffer, 0, 1500);
        memset( &temp1 , 0, 1000);
        memset( &temp2 , 0, 1000);
        addr_len = sizeof(client_addr);

        if( (recvfrom(serverFD, recv_buffer, 1500, 0, (struct sockaddr*)&client_addr, &addr_len)) == -1 ){
            perror("receiver recvfrom(): ");
            exit(1);
        }

        memcpy( &temp1, recv_buffer, 1000 );
        std::cout << "--recv pkt-- " << "seq_num: " << temp1.seq_num << ", ack_num: " << temp1.ack_num << ", ack_flag: " << temp1.ack_flag;
    
        if( temp1.seq_num == reqNum ){
            temp2.seq_num = -1;
            temp2.ack_num = temp1.seq_num + 1;
            temp2.ack_flag = FLAG_ACK;
            reqNum++;

            std::cout << " - accepted! send out ack: " << reqNum << std::endl;
            sendACK( &temp2 );
        }else{
            temp2.seq_num = -1;
            temp2.ack_num = reqNum;
            temp2.ack_flag = FLAG_ACK;

            std::cout << " - discard! send out ack: " << reqNum << std::endl;
            sendACK( &temp2 );
        }
    }
    close(serverFD);
}


void UDPServer::sendACK( struct pkt * packet ){
    
}


UDPServer::~UDPServer(){
}
