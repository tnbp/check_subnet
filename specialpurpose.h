bool is_special_purpose(unsigned long ul_ip) {
        if ((ul_ip & 0xFF000000) == 0x00000000) return true; // "This host on this network"
	if ((ul_ip & 0xFF000000) == 0x0A000000)	return true; // Private-Use (10.0.0.0/8)
	if ((ul_ip & 0xFFC00000) == 0x64400000) return true; // Shared Address Space (100.64.0.0/10)
	if ((ul_ip & 0xFF000000) == 0x7F000000) return true; // Loopback (127.0.0.0/8)
	if ((ul_ip & 0xFFFF0000) == 0xA9FE0000) return true; // Link Local (169.254.0.0/16)
	if ((ul_ip & 0xFFF00000) == 0xAC100000) return true; // Private-Use (172.16.0.0/12)
	if ((ul_ip & 0xFFFFFFFF) == 0xC0000008) return true; // IPv4 dummy address
	if ((ul_ip & 0xFFFFFFFF) == 0xC0000009) return true; // Port Control Protocol Anycast
	if ((ul_ip & 0xFFFFFFFF) == 0xC000000A) return true; // Traversal Using Relays around NAT Anycast
	if ((ul_ip & 0xFFFFFFFF) == 0xC00000AA) return true; // NAT64/DNS64 Discovery
	if ((ul_ip & 0xFFFFFFFF) == 0xC00000AB) return true; // NAT64/DNS64 Discovery
	if ((ul_ip & 0xFFFFFF00) == 0xC0000000) return true; // IETF Protocol Assignments
	if ((ul_ip & 0xFFFFFFF8) == 0xC0000000) return true; // IPv4 Service Continuity Prefix
	if ((ul_ip & 0xFFFFFF00) == 0xC0000200) return true; // Documentation (TEST-NET-1)
	if ((ul_ip & 0xFFFFFF00) == 0xC01FC400) return true; // AS112-v4
	if ((ul_ip & 0xFFFFFF00) == 0xC034C100) return true; // AMT
	if ((ul_ip & 0xFFFFFF00) == 0xC0586300) return true; // Deprecated (6to4 Relay Anycast)
	if ((ul_ip & 0xFFFF0000) == 0xC0A80000) return true; // Private-Use
	if ((ul_ip & 0xFFFFFF00) == 0xC0AF3000) return true; // Direct Delegation AS112 Service
	if ((ul_ip & 0xFFFE0000) == 0xC6120000) return true; // Benchmarking
	if ((ul_ip & 0xFFFFFF00) == 0xC6336400) return true; // Documentation (TEST-NET-2)
	if ((ul_ip & 0xFFFFFF00) == 0xCB007100) return true; // Documentation (TEST-NET-3)
	if ((ul_ip & 0xF0000000) == 0xF0000000) return true; // Reserved
	if ((ul_ip & 0xFFFFFFFF) == 0xFFFFFFFF) return true; // Limited Broadcast
	return false;
}

void print_special_purpose(unsigned long ul_ip) {
        if ((ul_ip & 0xFF000000) == 0x00000000) { printf("\"This host on this network\"\n"); return; }
	if ((ul_ip & 0xFF000000) == 0x0A000000) { printf("Private-Use\n"); return; }
	if ((ul_ip & 0xFFC00000) == 0x64400000) { printf("Shared Address Space\n"); return; }
	if ((ul_ip & 0xFF000000) == 0x7F000000) { printf("Loopback\n"); return; }
	if ((ul_ip & 0xFFFF0000) == 0xA9FE0000) { printf("Link Local\n"); return; }
	if ((ul_ip & 0xFFF00000) == 0xAC100000) { printf("Private-Use\n"); return; }
	if ((ul_ip & 0xFFFFFFFF) == 0xC0000008) { printf("IPv4 dummy address\n"); return; }
	if ((ul_ip & 0xFFFFFFFF) == 0xC0000009) { printf("Port Control Protocol Anycast\n"); return; }
	if ((ul_ip & 0xFFFFFFFF) == 0xC000000A) { printf("Traversal Using Relays around NAT Anycast\n"); return; }
	if ((ul_ip & 0xFFFFFFFF) == 0xC00000AA) { printf("NAT64/DNS64 Discovery\n"); return; }
	if ((ul_ip & 0xFFFFFFFF) == 0xC00000AB) { printf("NAT64/DNS64 Discovery\n"); return; }
	if ((ul_ip & 0xFFFFFF00) == 0xC0000000) { printf("IETF Protocol Assignments\n"); return; }
	if ((ul_ip & 0xFFFFFFF8) == 0xC0000000) { printf("IPv4 Service Continuity Prefix\n"); return; }
	if ((ul_ip & 0xFFFFFF00) == 0xC0000200) { printf("Documentation (TEST-NET-1)\n"); return; }
	if ((ul_ip & 0xFFFFFF00) == 0xC01FC400) { printf("AS112-v4\n"); return; }
	if ((ul_ip & 0xFFFFFF00) == 0xC034C100) { printf("AMT\n"); return; }
	if ((ul_ip & 0xFFFFFF00) == 0xC0586300) { printf("Deprecated (6to4 Relay Anycast)\n"); return; }
	if ((ul_ip & 0xFFFF0000) == 0xC0A80000) { printf("Private-Use\n"); return; }
	if ((ul_ip & 0xFFFFFF00) == 0xC0AF3000) { printf("Direct Delegation AS112 Service\n"); return; }
	if ((ul_ip & 0xFFFE0000) == 0xC6120000) { printf("Benchmarking\n"); return; }
	if ((ul_ip & 0xFFFFFF00) == 0xC6336400) { printf("Documentation (TEST-NET-2)\n"); return; }
	if ((ul_ip & 0xFFFFFF00) == 0xCB007100) { printf("Documentation (TEST-NET-3)\n"); return; }
	if ((ul_ip & 0xF0000000) == 0xF0000000) { printf("Reserved\n"); return; }
	if ((ul_ip & 0xFFFFFFFF) == 0xFFFFFFFF) { printf("Limited Broadcast\n"); return; }
}
