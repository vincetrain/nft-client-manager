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

char* readAuthKey() {
    char* key[256];
    ifstream keyFile;
    keyFile.open("authkey");
    if (keyFile.is_open()) {
        keyFile >> key;
    }
}

int main(void) {
    cout << "Service has started..." << endl;

    signal(SIGTERM, handleSIGTERM);


    int servfd = 0, clifd = 0;
    struct sockaddr_in servaddr, cliaddr;
    servaddr.sin_port = 63512;

    socklen_t clilen;
    clilen = sizeof(cliaddr);

    servfd = socket(AF_INET, SOCK_STREAM, 0);

    listen(servfd, 50);

    // Main loop happens here, stops when SIGTERM recieved
    while (!sigterm) {
        cout << "Awaiting socket connection..." << endl;

        char cliaddr_str[INET_ADDRSTRLEN];

        clifd = accept(servfd, (struct sockaddr *)&cliaddr, NULL);
        inet_ntop(AF_INET, &(cliaddr.sin_addr), cliaddr_str, INET_ADDRSTRLEN);  // After getting address of socket, store it to the cliaddr_str buffer

        cout << cliaddr_str << endl;

        char* recvKey[257];

        int e = read(clifd, recvKey, sizeof(recvKey)-1);
        recvKey[e] = 0;
        close(clifd);

        cout << recvKey << endl;
        cout << "Loop happened, sleeping 3 seconds..." << endl;
        sleep(3);
    }

    // Gracefully shutdown service
} 