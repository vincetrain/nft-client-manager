#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>

#include <csignal>
#include <unistd.h>
#include <stdlib.h>

#include <sys/socket.h>

using namespace std;

volatile sig_atomic_t sigterm = 0;

void handleSIGTERM(int signum) {
    cout << "Recieved SIGTERM, shutting down..." << endl;
    sigterm = 1;
}

int main(void) {
    signal(SIGTERM, handleSIGTERM);

    while (!sigterm) {
        // Main loop happens here
    }

    // Gracefully shutdown service
} 