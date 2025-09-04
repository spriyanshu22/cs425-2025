## GROUP MEMBERS

1. Harsh Agrawal - 220425
2. Naman Kumar Jaiswal- 220687
3. Priyanshu Singh 220830

---

## Overview

Implementation of both **Distance Vector Routing (DVR)** and **Link State Routing (LSR)** algorithms in C++ using an adjacency‑matrix input. The program:

1. Parses an `n×n` cost matrix from a text file.  
2. Simulates DVR by iteratively exchanging routing tables until convergence.  
3. Simulates LSR by running Dijkstra’s algorithm at each node.  
4. Prints the routing table for each router (destination, cost, next hop).

---

## What is Distance Vector Routing?

**Distance Vector Routing** is a distributed algorithm in which each router maintains a table of the best‑known cost to every destination and the first hop on that path. Periodically, each router sends its table to its immediate neighbors. Upon receiving a neighbor’s table, a router updates its own entries if a shorter path is discovered.

---

## What is Link State Routing?

**Link State Routing** requires every router to have a complete view of the network topology. Each node floods its neighbor‑cost information to all routers. Once the full graph is known, each router independently computes shortest paths (e.g., via Dijkstra’s algorithm) to build its forwarding table.

---

## Hardware & Software Requirements

- **Operating System:** Linux (POSIX headers `<arpa/inet.h>`, `<unistd.h>`)  
- **Compiler:** `g++` (C++11 or later)  
- **Build Tools:** `make` (optional)  

---

## Features Implemented

- **DVR Simulation**  
  - Initializes direct-link costs and next hops.  
  - Iteratively relaxes routing entries until no update occurs.  
- **LSR Simulation**  
  - Builds global topology from input matrix.  
  - Runs Dijkstra’s shortest‑path algorithm for each source node.  
- **Next‑Hop Calculation**  
  - Records the first hop on every computed path.  
- **Configurable Metric**  
  - Supports any positive‑integer cost (bandwidth, latency, etc.).  
- **Pretty‑Printed Tables**  
  - Tabular output showing `Dest`, `Cost`, and `Next Hop`.  

---

## Design Decisions

- **Adjacency‑Matrix Representation**  
  Simplifies input parsing and direct-cost lookups.  
- **Iterative Relaxation for DVR**  
  Modeled after the Floyd–Warshall relaxation approach to guarantee convergence without explicit neighbor messaging.  
- **Priority Queue for LSR**  
  Ensures Dijkstra’s algorithm runs in _O(E log V)_ time.  
- **Next‑Hop Tracking**  
  Stored in a separate 2D array for clarity and easy printing.  
- **Constants for Infinity**  
  Used `const int INF = 9999` to represent unreachable links.  

---

## Implementation Details

### Macro‑Level Overview

- **`main()`**  
  1. Reads filename from command line.  
  2. Calls `readGraphFromFile()` to build the cost matrix.  
  3. Invokes `simulateDVR(graph)` and prints DVR tables.  
  4. Invokes `simulateLSR(graph)` and prints LSR tables.  

- **`simulateDVR(graph)`**  
  - Initializes `dist = graph` and `nextHop` from direct links.  
  - Runs three nested loops (k, i, j) to relax `dist[i][j]` through intermediate `k`.  
  - Updates `nextHop[i][j]` whenever a shorter path is found.  
  - Calls `printDVRTable()` for each node.  

- **`simulateDVR(graph)`**
- Simulates the Distance Vector Routing (DVR) algorithm using the given adjacency matrix `graph`.
- Initializes:
  - `dist[i][j]` as the direct cost from node `i` to node `j` (or `INF` if not directly connected).
  - `nextHop[i][j]` to `j` if there is a direct edge; `-1` otherwise.
- Iteratively relaxes all pairs of distances using an intermediate node `k`, similar to the Bellman-Ford approach:
  - For each node pair `(u, v)`, checks if routing through an intermediate `k` offers a shorter path.
  - If a shorter path is found (`graph[u][k] + dist[k][v] < dist[u][v]`), updates `dist[u][v]` and sets `nextHop[u][v] = k`.
  - Continues this process until no further updates occur in an iteration.
- After convergence, it prints the DVR table for each node using `printDVRTable()`.


- **Table Printing**  
  - `printDVRTable(node, dist, nextHop)` and `printLSRTable(src, dist, prev)` format each router’s table.

---

## Code Flow

1. **Read Input**  
   Read network size and adjacency matrix from `inputfile.txt`.

2. **Simulate DVR**  
   Iteratively relax all vertex pairs until no updates occur; then print each node’s final routing table.

3. **Simulate LSR**  
   For each node, run Dijkstra’s algorithm to compute shortest paths; then print its routing table.

4. **Exit**  
   Return `0` on successful completion.

---


## Challenges
We did not face a major challenge for this assignment, only a few minor inconveniences along the way, which we overcame with collective effort and determination.

---

## Future Improvements

- **Retransmission & Timeout Handling**  
  For DVR, simulate packet loss and timeouts, including retransmission logic and backoff strategies to reflect real‑world network behavior.

- **Visualization & Logging**  
  Generate dynamic graphs or animations of convergence (for DVR) and shortest‑path trees (for LSR), plus detailed log files for step‑by‑step analysis.

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
