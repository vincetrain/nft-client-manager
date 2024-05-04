#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <stdlib.h>

#include <sys/socket.h>

using namespace std;

static string nftconf = "/etc/ncm/ncm.conf";

int main(void) {
    daemonize();
    return 0;
} 

/**
 * Daemonizes the current running script
*/
void daemonize() {
    pid_t pid = fork(); // fork the current proccess and return it's process id

    // if pid > 0, we know it's the parent process so we close it
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    // negative pid means an error... so exit if an error occured
    else if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    // load old nftables config
    string command = "nft -f " + nftconf;
    system(command.c_str());

    // main daemon loop
    while (1) {
        // todo:
        //  - listen for incoming ncm join requests
        //  - update nftables conf to allow clients to join ncm and talk to this computer
        //      - authenticate with a shared secret
        writeToConfig();
        sleep(3);   // wait 3 seconds before looping again
    }

}

/**
 * Writes current ncm table configuration to the configuration file
*/
void writeToConfig() {
    string command = "nft list table inet ncm-table > " + nftconf;
    system(command.c_str());
}