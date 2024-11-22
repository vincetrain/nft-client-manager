#include <iostream>
#include <fstream>

#include <sys/types.h>

#include <csignal>
#include <unistd.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

volatile sig_atomic_t sigterm = 0;

void handleSIGTERM(int signum) {
    cout << "Recieved SIGTERM, shutting down..." << endl;
    sigterm = 1;
}

int main(void) {
    cout << "Service has started..." << endl;

    signal(SIGTERM, handleSIGTERM);
    struct sockaddr_in servaddr;
    int servsock = 0;

    servsock = socket(AF_INET, SOCK_STREAM, 0);
    servaddr.sin_family = AF_INET;  // we're working on ipv4
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);   // bind to any ip address on machine (converted to networklong)
    servaddr.sin_port = htons(1776);   // bind to port 63512 (converted to networkshort)

    bind(servsock, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(servsock, 50);

    // Main loop happens here, stops when SIGTERM recieved
    while (!sigterm) {
        cout << "Awaiting socket connection..." << endl;
        int clisock = 0;
        
        struct sockaddr_in cliaddr;
        socklen_t clilen;

        clilen = sizeof(cliaddr);
        clisock = accept(servsock, (struct sockaddr *) &cliaddr, &clilen);

        cout << "Loop happened, sleeping 3 seconds..." << endl;
        sleep(3);
    }

    // Gracefully shutdown service
    return 0;
} 