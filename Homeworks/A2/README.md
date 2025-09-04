# DNS Resolution System

## 1. Introduction
This project implements a DNS resolution system that supports both **iterative** and **recursive** DNS lookups. The system queries DNS servers to resolve domain names into IP addresses using either the iterative method (starting from root servers) or the recursive method (using a system resolver). The implementation is done using Python and the `dnspython` library.

## 2. Flow of Code
1. **Command-Line Argument Parsing**:
   - The user provides the mode (`iterative` or `recursive`) and the domain name as command-line arguments.
   
2. **Execution Based on Mode**:
   - If `iterative` mode is selected, the program starts querying root DNS servers and works its way down the DNS hierarchy.
   - If `recursive` mode is selected, the program relies on the system's default resolver to perform a recursive DNS lookup.
   
3. **Iterative DNS Lookup Process**:
   - The program starts with a predefined list of root DNS servers.
   - It queries a root server for the domain name.
   - If no answer is found, the response provides the next tier of nameservers (TLD or authoritative servers).
   - The process repeats until an IP address is obtained or resolution fails.

4. **Recursive DNS Lookup Process**:
   - The system resolver handles the entire DNS lookup process.
   - The program queries the system resolver, which recursively contacts DNS servers until an IP address is found.
   
5. **Performance Measurement**:
   - The program records the time taken to resolve the domain and prints the result.

## 3. Changes
Below are the key modifications we introduced in the code and their reasons:

### **1. `extract_next_nameservers()` function**
- We added the loop over the previously extracted "NS" host name records to find the corresponding "A" record.
- From these "A" records, we successfully extracted and returned the list of IP Addresses of next tier name servers.

### **2. `send_dns_query()` function**
- We have added the function to wait for reply from server for 2 seconds and when we get the response, we return it else we error handle it.

### **3. `iterative_dns_lookup()` function**
- We have added the code to update the stage of Resolution. ROOT -> TLD -> AUTH. 

### **4. `recursive_dns_lookup()`**
#### **Change:**
- We have added the function for recusive Resolution from the dns.resolution package.

#### **Reason:**
- To assess performance and compare iterative vs. recursive resolution times.

## 4. Features
1. **Iterative and Recursive Lookup:** Both modes function as expected, correctly resolving domain names.
2. **Timeout Handling:** Unresponsive queries do not hang the program.
3. **Error Handling:** Failures are handled gracefully, with meaningful error messages.
4. **Performance Measurement:** Execution time is displayed to compare efficiency.
5. **Modular Code Structure:** Functions are logically separated, improving readability and maintainability.

The system now provides a robust and efficient way to resolve domain names using both iterative and recursive approaches.

## 5. Contribution
Submission by Harsh Agrawal (220425), Naman Kumar Jaiswal (220687) and Priyanshu Singh (220830). Equal Three-folds Contribution across all aspects by all the team members.