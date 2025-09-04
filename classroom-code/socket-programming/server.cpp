#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080

/*
 > STEPS
    1. Create a socket with the socket() system call.
    2. Bind the socket to an address using the bind() system call. For a server socket on the Internet, an address consists of a port number on the host machine.
    3. Listen for connections with the listen() system call.
    4. Accept a connection with the accept() system call. This call typically blocks until a client connects with the server.
    5. Send and receive data using the read() and write() system calls.
 */
 
int main() {
    int server_fd, new_socket;  // server_fd is the file descriptor for the socket that the server will use to listen for incoming connections
                                // new_socket is the file descriptor for the socket that the server will use to communicate with the client
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    const char* hello = "Hello from server";

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options (macOS compatible)
    // This allows the server to reuse the address and port helping to avoid the "Address already in use" error
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the address
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    // Accept an incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept");
        exit(EXIT_FAILURE);
    }

    read(new_socket, buffer, 1024);
    std::cout << "Message from client: " << buffer << std::endl;
    send(new_socket, hello, strlen(hello), 0);
    std::cout << "Hello message sent" << std::endl;

    while(1){
        char buffer[1024] = {0};
        cin>>buffer;
        
        if (buffer[0] == '.') {
            break;
        } else {
            std::cout << "Message from client: " << buffer << std::endl;
        }
    }

    close(new_socket);
    close(server_fd);

    return 0;
}
