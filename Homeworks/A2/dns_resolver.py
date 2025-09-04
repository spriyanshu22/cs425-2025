import dns.message
import dns.query
import dns.rdatatype
import dns.resolver
import time

# Root DNS servers used to start the iterative resolution process
ROOT_SERVERS = {
    "198.41.0.4": "Root (a.root-servers.net)",
    "199.9.14.201": "Root (b.root-servers.net)",
    "192.33.4.12": "Root (c.root-servers.net)",
    "199.7.91.13": "Root (d.root-servers.net)",
    "192.203.230.10": "Root (e.root-servers.net)"
}

TIMEOUT = 3  # Timeout in seconds for each DNS query attempt, if response is not reached within 3 seconds, process terminates

def send_dns_query(server, domain):
    """ 
    An helper function that sends a DNS query to the given server for an "A" record of the specified domain.
    Returns the response if successful, otherwise returns None.
    
    Parameters:
        server - The server where the query is to be sent, takes values 
        domain - The domain for which the query is to be sent
        
    Output:
        returns the dns response for the dns query
    """
    
    try:
        query = dns.message.make_query(domain, dns.rdatatype.A)  # Construct the DNS query
        response = dns.query.udp(query, server, timeout=2)  # Wait for the reply from the server for 2 seconds
        return response
    except Exception:
        return None  # If an error occurs (timeout, unreachable server, etc.), return None

def extract_next_nameservers(response):
    """ 
    Extracts nameserver (NS) records from the authority section of the response.
    Then, resolves those NS names to IP addresses.
    
    Parameters:
        response - response we got from a DNS Server
    
    Output:
        Returns a list of IPs of the next tier nameservers.
    """
    ns_ips = []  # List to store resolved nameserver IPs
    ns_names = []  # List to store nameserver domain names
    
    # Loop through the authority section to extract NS records
    for rrset in response.authority:
        if rrset.rdtype == dns.rdatatype.NS:            # Checks the record type of the response
            for rr in rrset:                            # For every record, we add the nameserver hostname
                ns_name = rr.to_text()
                ns_names.append(ns_name)  # Extract nameserver hostname
                print(f"Extracted NS hostname: {ns_name}")

    # Resolving the extracted NS hostnames to IP addresses
    for ns_name in ns_names:
        try:
            ns_response = dns.resolver.resolve(ns_name, "A")  # Query the A record for the nameserver
            for rdata in ns_response:                         # For every response r entry, add the resolved ips
                ns_ips.append(rdata.to_text())  # Add the resolved IP address
                print(f"Resolved NS {ns_name} to IP: {rdata.to_text()}")
        except Exception as e:
            print(f"[ERROR] Error resolving {ns_name}: {e}")  # Handle resolution failure
    
    return ns_ips  # Return list of resolved nameserver IPs
    

def iterative_dns_lookup(domain):
    """ 
    Performs an iterative DNS resolution starting from root servers.
    It queries root servers, then TLD servers, then authoritative servers,
    following the hierarchy until an answer is found or resolution fails.
    
    Parameters:
        domain - The relevant domain that needs to be resolved
        
    Output:
        1. prints the resolved dns IP Address
    """
    print(f"[Iterative DNS Lookup] Resolving {domain}")

    next_ns_list = list(ROOT_SERVERS.keys())  # Start with the root server IPs
    stage = "ROOT"  # Track resolution stage (ROOT, TLD, AUTH)

    # Keep Searching for the case of Iterative Servers
    while next_ns_list:
        ns_ip = next_ns_list[0]  # Pick the first available nameserver to query
        response = send_dns_query(ns_ip, domain)    # Send Query
        
        if response: # Checks if response is not NONE
            print(f"[DEBUG] Querying {stage} server ({ns_ip}) - SUCCESS")
            
            # If an answer is found, print and return
            if response.answer:
                print(f"[SUCCESS] {domain} -> {response.answer[0][0]}")
                return
            
            # If no answer, extract the next set of nameservers
            next_ns_list = extract_next_nameservers(response)
            
            # Updating the stage of resolution process
            if stage == "ROOT":
                stage = "TLD" 
            elif stage == "TLD":
                stage = "AUTH" 

        else:
            print(f"[ERROR] Query failed for {stage} {ns_ip}")
            return  # Stop resolution if a query fails
    
    print("[ERROR] Resolution failed.")  # Final failure message if no nameservers respond

# Recursive DNS Resolution
def recursive_dns_lookup(domain):
    """ 
    Performs recursive DNS resolution using the system's default resolver.
    This approach relies on a resolver (like Google DNS or a local ISP resolver)
    to fetch the result recursively.
    
    Parameters:
        domain - The relevant domain that needs to be resolved
        
    Output:
        1. prints the resolved dns IP Address
    """
    print(f"[Recursive DNS Lookup] Resolving {domain}")
    try:
        answer = dns.resolver.resolve(domain, "A")  # Performs recursive resolution, using dns.resolver package
        for rdata in answer:
            print(f"[SUCCESS] {domain} -> {rdata}") # Pronts all resolved IP Addresses
    except Exception as e:
        print(f"[ERROR] Recursive lookup failed: {e}")  # Handle resolution failure

if __name__ == "__main__":
    
    import sys          # Relevant Library for command line argument handling
    
    # Argument constraint handling, must receive 2 inputs (mode and domanin) and mode can only take 2 values.
    if len(sys.argv) != 3 or sys.argv[1] not in {"iterative", "recursive"}:
        
        # In case of improper use, inform the correct command line format
        print("Usage: python3 dns_server.py <iterative|recursive> <domain>")
        sys.exit(1)

    # mode informs the method of resolution, takes values "iterative" or "recursive"
    mode = sys.argv[1]
    
    # domain takes the value of the domain name that is to be resolved
    domain = sys.argv[2]  
    
    start_time = time.time()  # Record start time
    
    # As per the selected DNS resolution mode, execute the relevant resolution function
    if mode == "iterative":
        iterative_dns_lookup(domain)
    else:
        recursive_dns_lookup(domain)
    
    # Print total dns resolution time
    print(f"Time taken: {time.time() - start_time:.3f} seconds")  
