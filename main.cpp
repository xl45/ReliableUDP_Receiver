#include <iostream>
#include "UDPServer.h"

int main(){
    UDPServer myUPDServer("3480");
    myUPDServer.work();

    return 0;
}
