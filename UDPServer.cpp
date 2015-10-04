#include "UDPServer.h"


UDPServer::UDPServer(std::string port){
    reqNum = 1;
    dupACK_flag = 1;
    dupACK_count = 0;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    // getaddrinfo
    int status;
    if( (status = getaddrinfo("0.0.0.0", port.c_str(), &hints, &serverinfo)) != 0 ){
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
    int lostPkt;

    while(1){
        memset(&recv_buffer, 0, 1500);
        memset( &temp1 , 0, PKT_SIZE);
        memset( &temp2 , 0, PKT_SIZE);
        addr_len = sizeof(client_addr);

        if( (recvfrom(serverFD, recv_buffer, 1500, 0, (struct sockaddr*)&client_addr, &addr_len)) == -1 ){
            perror("receiver recvfrom(): ");
            exit(1);
        }

        memcpy( &temp1, recv_buffer, PKT_SIZE );
        std::cout << "**recv pkt** " << "seq_num: " << temp1.seq_num << ", ack_num: " << temp1.ack_num << ", ack_flag: " << temp1.ack_flag;
        // random drop
        if( randDrop() && dupACK_flag ){
            std::cout << "  ~~~ drop this pkt!! ~~~\n";
            dupACK_flag = 0;
            lostPkt = temp1.seq_num;
            continue;
        }
        //
        if( temp1.seq_num == lostPkt ){
            dupACK_flag = 1;
            dupACK_count = 0;
        }
        // send ack
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

            if( dupACK_count == 3 ){
                // already send out 3 dup ack, discard this packet and donot send ack
                std::cout << " - discard! send out ack: " << reqNum << std::endl;
                continue;
            }

            std::cout << " - discard! send out ack: " << reqNum << std::endl;
            sendACK( &temp2 );
            dupACK_count++;
        }
    }
    close(serverFD);
}


void UDPServer::sendACK( struct pkt * packet ){
    BYTE temp[PKT_SIZE];
    memcpy( temp, packet, PKT_SIZE );
    if( (sendto(serverFD, temp, PKT_SIZE, 0, (struct sockaddr *)&client_addr,addr_len)) == -1){
        perror("receiver sendto(): ");
        exit(1);
    }
}


bool UDPServer::randDrop(){
    int r = rand()%50 + 1; // 1 ~ 50
    return (r==25);
}


UDPServer::~UDPServer(){
}
