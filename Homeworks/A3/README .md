## GROUP MEMBERS

1. Harsh Agrawal - 220425
2. Naman Kumar Jaiswal- 220687
3. Priyanshu Singh 220830

## We have used 12345 for the Server's listening port

## Overview
Implementation of a TCP client using raw sockets to perform a simplified three-way handshake with fixed sequence numbers. The client:
1. Sends SYN (seq=200)
2. Receives SYN-ACK (seq=400, ack=201)
3. Sends final ACK (seq=600, ack=401)
---

## What is a TCP Three-Way Handshake?

The **TCP three-way handshake** is a foundational process used to establish a reliable connection between a client and server before data transmission begins. It involves three main steps:

1. **SYN (Synchronize)** – The client sends a TCP segment with the SYN flag set to indicate it wants to start a connection and proposes an initial sequence number.
2. **SYN-ACK (Synchronize-Acknowledge)** – The server responds with a segment that has both the SYN and ACK flags set. It acknowledges the client’s sequence number and proposes its own.
3. **ACK (Acknowledge)** – The client replies with an ACK segment, acknowledging the server's sequence number.

Once this three-step process is completed, both sides agree on initial sequence numbers and a full-duplex TCP connection is established.

In this assignment, the handshake is simulated using TCP flags and sequence numbers to successfully complete the handshake with the server.

---

## Hardware & Software Requirements

- **Operating System:** Linux: We use POSIX compliant libraries (like `<unistd.h>` for utility fuctions & `<arpa/inet.h>` for socket programming in given server code.
- **Tools:** `make` (optional)

## Features Implemented

- Sends a manually crafted SYN packet using raw sockets.
- Receives and validates the SYN-ACK response from the server.
- Sends the final ACK to complete the TCP three-way handshake.
- Handles custom sequence and acknowledgment numbers.
- Parses and logs TCP flags and header information.

---

## Design Decisions

- **Use of Raw Sockets:**  
  Raw sockets were chosen to allow full control over TCP packet construction, as required for manually implementing the handshake without relying on the OS's TCP stack.

- **Hardcoded Sequence Numbers:**  
  Specific sequence numbers (e.g., 200, 400) were hardcoded to match the server’s expectations, ensuring compatibility and successful handshake completion.

- **Minimal Header Fields:**  
  Only the essential fields in the IP and TCP headers were set (e.g., version, length, flags, sequence numbers) to keep the implementation focused and avoid unnecessary complexity.

- **Packet Logging for Debugging:**  
  Key values such as TCP flags, sequence numbers, and acknowledgment numbers are printed to verify each stage of the handshake and aid in debugging.

- **Synchronous Design:**  
  The client waits for each response before proceeding to the next step to closely follow the strict three-way handshake sequence.


---

## Implementation Details

### Macro-Level Overview (High-Level Structure)

- **`main()`**
  - Initializes a raw socket with `IP_HDRINCL` option.
  - Sends a SYN packet to initiate the handshake.
  - Waits and processes the SYN-ACK from the server.
  - Sends the final ACK to complete the handshake.

- **`send_tcp_packet()`**
  - Constructs and sends a TCP packet with customizable flags (SYN/ACK), sequence numbers, and acknowledgment numbers.

- **`receive_tcp_packet()`**
  - Listens for incoming TCP packets using `recvfrom()`.
  - Parses IP and TCP headers and validates the expected SYN-ACK.
  - Extracts and stores the server’s sequence number.

- **`print_tcp_flags()`**
  - Displays the TCP flags and sequence information for debugging.

---

### Micro-Level Details (Low-Level Logic)

- **Socket Creation**
  - A raw socket is created with `socket(AF_INET, SOCK_RAW, IPPROTO_TCP)`.
  - `setsockopt()` with `IP_HDRINCL` ensures we provide our own IP header.

- **IP Header Construction**
  - Fields like `ihl`, `version`, `tot_len`, `ttl`, `protocol`, and source/destination IPs are manually set.

- **TCP Header Construction**
  - Fields like `source` and `dest` ports, `seq`, `ack_seq`, `doff`, flags (`syn`, `ack`), and `window` are set manually.

- **Loop and Validation**
  - The receiver loops until a valid SYN-ACK is received before proceeding to send ACK.

---

## Code Flow

![Server Architecture Diagram](https://drive.google.com/file/d/1-H_FDemrUTU0j_dFoYBP_zj26hOQ4ng0)

## Code Flow

### 1. Socket Initialization
- **Raw Socket Creation**:  
  `int sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);`

  Creates a raw socket to manually handle TCP/IP headers.

- **Header Inclusion**:  
  `setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one));`

  Enables manual IP header construction.

- **Server Address Configuration**:  
  `server_addr.sin_port = htons(SERVER_PORT);`  
  `server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);`

  Sets server IP/port (127.0.0.1:12345).

---

### 2. SYN Packet Transmission
- **Packet Construction**:  
  - Fixed sequence number: 200  
  - SYN flag enabled  
  - Source port: 55555 (arbitrary)  
  `send_tcp_packet(..., 200, 0, true, false);`

- **Header Configuration**:  
  - IP: TTL=64, ID=54321  
  - TCP: Window=8192, checksum=0 (auto-calculated)

---

### 3. SYN-ACK Reception
- **Packet Filtering**:  
  `if (ntohs(tcp_response->dest) != CLIENT_PORT) continue;`

  Filters non-server packets.

- **Validation**:  
  `if (tcp_response->syn && tcp_response->ack && ntohl(tcp_response->ack_seq) == 201)`

  Verifies SYN-ACK (seq=400, ack=201).

---

### 4. Final ACK Transmission
- **Sequence Management**:  
  - Client seq: 600 (200+1)  
  - Server ack: 401 (400+1)  
  `send_tcp_packet(..., 600, 401, false, true);`

---

### 5. Handshake Completion
- **Socket Closure**:  
  `close(sock);`

  Releases resources after successful handshake.

---

## Testing


---

## Challenges
We did not face a major challenge for this assignment, only a few minor inconveniences along the way, which we overcame with collective effort and determination.

---

## Future Improvements

- Add checksum calculation for both IP and TCP headers.
- Extend the implementation to handle multiple concurrent handshakes.
- Explore handling retransmissions and timeout scenarios.

---

## Team Contribution

Team Members: Harsh Agrawal (220425), Naman Kumar Jaiswal (220687) and Priyanshu Singh (220830)

Contributions: The project was a collaborative effort, with each team member contributing equally three folds across all aspects.

---

## Sources Referred

- [CS425 Github Repository](https://github.com/privacy-iitk/cs425-2025)

---

## Declaration

We hereby declare that this assignment was completed independently and did not involve plagiarism of any form.

---

## Feedback

It was a great learning experience!
