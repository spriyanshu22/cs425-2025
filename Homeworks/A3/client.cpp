#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

// IP and Port definitions
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define CLIENT_PORT 55555

// Utility function to print TCP flag values and sequence number (taken from sever.cpp)
void print_tcp_flags(struct tcphdr *tcp) {
    cout << "[+] TCP Flags: "
         << " SYN: " << tcp->syn
         << " ACK: " << tcp->ack
         << " FIN: " << tcp->fin
         << " RST: " << tcp->rst
         << " PSH: " << tcp->psh
         << " SEQ: " << ntohl(tcp->seq) << std::endl;
}

// Function to construct and send a custom TCP packet
void send_tcp_packet(int sock, struct sockaddr_in *dest_addr, int seq_num, int ack_num, bool syn, bool ack) {
    // Allocate buffer for IP + TCP header
    char packet[sizeof(struct iphdr) + sizeof(struct tcphdr)];
    memset(packet, 0, sizeof(packet));

    // Pointer to the IP and TCP headers within the buffer
    struct iphdr *ip = (struct iphdr *)packet;
    struct tcphdr *tcp = (struct tcphdr *)(packet + sizeof(struct iphdr));

    // Construct the IP header
    ip->ihl = 5;                            // Header length
    ip->version = 4;                        // IPv4
    ip->tos = 0;                            // Type of service
    ip->tot_len = htons(sizeof(packet));   // Total length of packet
    ip->id = htons(54321);                 // Packet ID
    ip->frag_off = 0;                       // Fragment offset
    ip->ttl = 64;                           // Time to live
    ip->protocol = IPPROTO_TCP;            // Protocol type
    ip->saddr = inet_addr("127.0.0.1");     // Source IP address
    ip->daddr = dest_addr->sin_addr.s_addr; // Destination IP address

    // Construct the TCP header
    tcp->source = htons(CLIENT_PORT);      // Source port
    tcp->dest = htons(SERVER_PORT);        // Destination port
    tcp->seq = htonl(seq_num);             // Sequence number
    tcp->ack_seq = htonl(ack_num);         // Acknowledgement number
    tcp->doff = 5;                          // Header size
    tcp->syn = syn;                        // Set SYN flag
    tcp->ack = ack;                        // Set ACK flag
    tcp->window = htons(8192);             // Window size
    tcp->check = 0;                        

    // Send the raw packet
    if (sendto(sock, packet, sizeof(packet), 0, (struct sockaddr *)dest_addr, sizeof(*dest_addr)) < 0) {
        perror("sendto() failed");
    } else {
        if (syn) cout << "[+] Sent SYN" << endl;
        else cout << "[+] Sent ACK" << endl;
        print_tcp_flags(tcp);  // Print TCP flags
    }
}

// Function to receive and process incoming TCP packets
void receive_tcp_packet(int sock, int expected_ack_number, int& server_sequence_number) {
    char buffer[65536];                     // Large buffer to hold the packet
    struct sockaddr_in source_addr;
    socklen_t addr_len = sizeof(source_addr);

    while (true) {
        // Receive raw packet
        int data_size = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&source_addr, &addr_len);
        if (data_size < 0) {
            perror("Packet reception failed");
            continue;
        }

        // Extract IP and TCP headers from the buffer
        struct iphdr *ip = (struct iphdr *)buffer;
        struct tcphdr *tcp_response = (struct tcphdr *)(buffer + (ip->ihl * 4));

        // Make sure this packet is intended for our client port
        if (ntohs(tcp_response->dest) != CLIENT_PORT) continue;

        cout << "[+] Received SYN-ACK from " << inet_ntoa(source_addr.sin_addr) << endl;
        print_tcp_flags(tcp_response);

        // Check for SYN-ACK and validate ACK number
        if (tcp_response->syn == 1 && tcp_response->ack == 1 &&
            (int)ntohl(tcp_response->ack_seq) == expected_ack_number) {
            server_sequence_number = ntohl(tcp_response->seq);
            break;
        }
    }
}

int main() {
    // Create raw socket using TCP
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int one = 1;
    if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0) {
        perror("setsockopt() failed");
        exit(EXIT_FAILURE);
    }

    // Define the destination server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Define client-side sequence numbers according to server.cpp
    int client_sequence_number = 200;
    int client_sequence_number2 = 600;

    // Step 1: Send SYN packet to initiate handshake
    send_tcp_packet(sock, &server_addr, client_sequence_number, 0, true, false);

    // Step 2: Receive SYN-ACK and extract server’s sequence number
    int server_sequence_number;
    receive_tcp_packet(sock, client_sequence_number + 1, server_sequence_number);

    // Step 3: Send final ACK to complete the handshake
    send_tcp_packet(sock, &server_addr, client_sequence_number2, server_sequence_number + 1, false, true);

    cout << "Handshake completed!\n";

    close(sock);  // Close the socket
    return 0;
}
