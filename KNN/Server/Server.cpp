#include <iostream>
#include <thread>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include "Strategy/Context.h"
#include "ClientModel.h"
#include "../CLI.h"

using namespace :: std;


const int MAX_CONNECTIONS = 5;

void handle_client(int client_socket) {
    ClientModel* client = new ClientModel(client_socket);
    ServerCLI* cli = new ServerCLI(new Context(), client);
    cli -> start();
    close(client_socket);
    delete cli;
}


int main(int argc, const char * argv[]) {
    if (argc != 2) {
        std::cerr << "Error: incorrect number of arguments.\n";
        return 1;
    }

    const int server_port = stoi(argv[1]);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock<0) {
        perror("error creating socket");
        return 1;
    }

    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    //change to little endian if needed
    sin.sin_port = htons(server_port);

    if (::bind(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0 ) {
        perror("error binding socket");
    }

    if (listen(sock,MAX_CONNECTIONS) < 0) {
        perror("error listening to a socket");
    }


    while (true) {
        struct sockaddr_in client_sin;
        unsigned int addr_len = sizeof(client_sin);
        int client_sock = accept(sock,(struct sockaddr*)&client_sin, &addr_len);
        if (client_sock < 0) {
            std::cerr << "Error accepting client connection" << std::endl;
            continue;
        }

        std::thread client_thread(handle_client, client_sock);
        client_thread.detach();
    }
}





