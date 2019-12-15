#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "colors.h"
#include "specialpurpose.h"

short netmask_set_bits(unsigned long);
unsigned long compose_address(unsigned short, unsigned short, unsigned short, unsigned short);
void binary_ip_string(unsigned long, char*);
void decimal_ip_string(unsigned long, char*);
bool is_special_purpose(unsigned long);
void print_special_purpose(unsigned long);

int main(int argc, char **argv) {
	int sdr_ip[4],rcv_ip[4],netmask[4];
	char str_sdr_ip[4*8+3+1], str_rcv_ip[4*8+3+1], str_netmask_ip[4*8+3+1];
	printf("Enter sender IP:\t");
	while (scanf("%d.%d.%d.%d", &sdr_ip[0], &sdr_ip[1], &sdr_ip[2], &sdr_ip[3]) != 4) {
	}
	for (int i=0; i<4; i++) {
		if (sdr_ip[i] > 255) {
			printf(RED "%d.%d.%d.%d: invalid IP address" RESET "\n", sdr_ip[0], sdr_ip[1], sdr_ip[2], sdr_ip[3]);
			return 1;
		}
	}
	printf("Enter receiver IP:\t");
	while (scanf("%d.%d.%d.%d", &rcv_ip[0], &rcv_ip[1], &rcv_ip[2], &rcv_ip[3]) != 4) {
	}
	for (int i=0; i<4; i++) {
		if (rcv_ip[i] > 255) {
			printf(RED "%d.%d.%d.%d: invalid IP address" RESET "\n", rcv_ip[0], rcv_ip[1], rcv_ip[2], rcv_ip[3]);
			return 1;
		}
	}
	printf("------------------\n");
	printf("Enter netmask:\t\t");
	while (scanf("%d.%d.%d.%d", &netmask[0], &netmask[1], &netmask[2], &netmask[3]) != 4) {
	}
	for (int i=0; i<4; i++) {
		if (netmask[i] > 255) {
			printf(RED "%d.%d.%d.%d: invalid netmask" RESET "\n", netmask[0], netmask[1], netmask[2], netmask[3]);
			return 1;
		}
	}
	if (netmask_set_bits(compose_address(netmask[0], netmask[1], netmask[2], netmask[3])) == -1) {
		printf(RED "%d.%d.%d.%d: invalid netmask" RESET "\n", netmask[0], netmask[1], netmask[2], netmask[3]);
		return 1;
	}
	// preliminary checks have succeeded at this point - all input is valid
	unsigned long ulong_sdr_ip = compose_address(sdr_ip[0], sdr_ip[1], sdr_ip[2], sdr_ip[3]);
	binary_ip_string(ulong_sdr_ip, str_sdr_ip);
	unsigned long ulong_rcv_ip = compose_address(rcv_ip[0], rcv_ip[1], rcv_ip[2], rcv_ip[3]);
	binary_ip_string(ulong_rcv_ip, str_rcv_ip);
	unsigned long ulong_netmask = compose_address(netmask[0], netmask[1], netmask[2], netmask[3]);
	binary_ip_string(ulong_netmask, str_netmask_ip);
	// zeroth check - is input 0.0.0.0 ("This host on this network") or 255.255.255.255 ("Limited Broadcast")?
	if (ulong_sdr_ip == 0xFFFFFFFF || !ulong_sdr_ip) { 
		printf("\n" CYN "INFO:" RESET " Sender IP %d.%d.%d.%d is a special-purpose address:\n", sdr_ip[0], sdr_ip[1], sdr_ip[2], sdr_ip[3]);
		print_special_purpose(ulong_sdr_ip);
		return 0;
	}
	if (ulong_rcv_ip == 0xFFFFFFFF || !ulong_rcv_ip) {
		printf("\n" CYN "INFO:" RESET " Receiver IP %d.%d.%d.%d is a special-purpose address:\n", rcv_ip[0], rcv_ip[1], rcv_ip[2], rcv_ip[3]);
		print_special_purpose(ulong_rcv_ip);
		return 0;
	}
	// first check - is input a network address?
	// this is true if apart from netmask bits, all bits are zero ((IP AND NOT NETMASK) == 0)
	if (!(ulong_sdr_ip & ~ulong_netmask)) {
		printf(RED "Error: Sender IP %d.%d.%d.%d / Netmask %d.%d.%d.%d is a network address!" RESET "\n", sdr_ip[0], sdr_ip[1], sdr_ip[2], sdr_ip[3], netmask[0], netmask[1], netmask[2], netmask[3]);
		return 1;
	}
	if (!(ulong_rcv_ip & ~ulong_netmask)) {
		printf(RED "Error: Receiver IP %d.%d.%d.%d / Netmask %d.%d.%d.%d is a network address!" RESET "\n", rcv_ip[0], rcv_ip[1], rcv_ip[2], rcv_ip[3], netmask[0], netmask[1], netmask[2], netmask[3]);
		return 1;
	}
	// second check - is input a broadcast address?
	// this is true if apart from netmask bits, all bits are one ((NOT NETMASK AND IP) == NOT NETMASK)
	if ((ulong_sdr_ip & ~ulong_netmask) == ~ulong_netmask) {
		printf(RED "Error: Sender IP %d.%d.%d.%d / Netmask %d.%d.%d.%d is a broadcast address!" RESET "\n", sdr_ip[0], sdr_ip[1], sdr_ip[2], sdr_ip[3], netmask[0], netmask[1], netmask[2], netmask[3]);
		return 1;
	}
	if ((ulong_rcv_ip & ~ulong_netmask) == ~ulong_netmask) {
		printf(RED "Error: Receiver IP %d.%d.%d.%d / Netmask %d.%d.%d.%d is a broadcast address" RESET "\n", rcv_ip[0], rcv_ip[1], rcv_ip[2], rcv_ip[3], netmask[0], netmask[1], netmask[2], netmask[3]);
		return 1;
	}
	// third check - are sender and receiver addresses identical?
	if (ulong_sdr_ip == ulong_rcv_ip) {
		printf(RED "Error: Sender and receiver addresses are identical!" RESET "\n");
		return 1;
	}
	// print binary representations
	if (is_special_purpose(ulong_sdr_ip)) {
		printf("\n" CYN "INFO:" RESET " Sender IP %d.%d.%d.%d is a special-purpose address:\n", sdr_ip[0], sdr_ip[1], sdr_ip[2], sdr_ip[3]);
		print_special_purpose(ulong_sdr_ip);
	}
	if (is_special_purpose(ulong_rcv_ip)) {
		printf("\n" CYN "INFO:" RESET " Receiver IP %d.%d.%d.%d is a special-purpose address:\n", rcv_ip[0], rcv_ip[1], rcv_ip[2], rcv_ip[3]);
		print_special_purpose(ulong_rcv_ip);
	}
	printf("\n===================================================================\n\n------ BINARY REPRESENTATIONS ------\n");
	printf("Sender IP (A):\t\t\t%s\n", str_sdr_ip);
	printf("Receiver IP (B):\t\t%s\n", str_rcv_ip);
	printf("Netmask (N):\t\t\t%s\n", str_netmask_ip);
	printf("\n");
	char sdr_net_pfx[36], sdr_host_part[36], rcv_net_pfx[36], rcv_host_part[36];
	binary_ip_string(ulong_sdr_ip & ulong_netmask, sdr_net_pfx);
	binary_ip_string(ulong_rcv_ip & ulong_netmask, rcv_net_pfx);
	binary_ip_string(ulong_sdr_ip & ~ulong_netmask, sdr_host_part);
	binary_ip_string(ulong_rcv_ip & ~ulong_netmask, rcv_host_part);
	printf("Sender network prefix (A&N):\t%s\n", sdr_net_pfx);
	printf("Receiver network prefix (B&N):\t%s\n\n", rcv_net_pfx);
	printf("Sender host part (A&~N):\t%s\n", sdr_host_part);
	printf("Receiver host part (B&~N)\t%s\n\n", rcv_host_part);
	// finally, check if subnets match (NETMASK AND IP1 == NETMASK AND IP2)
	if ((ulong_sdr_ip & ulong_netmask) == (ulong_rcv_ip & ulong_netmask)) {
		printf("Sender IP %d.%d.%d.%d and receiver IP %d.%d.%d.%d " B_GRN "are" RESET " on the same subnet!\n", sdr_ip[0], sdr_ip[1], sdr_ip[2], sdr_ip[3], rcv_ip[0], rcv_ip[1], rcv_ip[2], rcv_ip[3]);
	}
	else {
		printf("Sender IP %d.%d.%d.%d and receiver IP %d.%d.%d.%d are " B_RED "NOT" RESET " on the same subnet!\n", sdr_ip[0], sdr_ip[1], sdr_ip[2], sdr_ip[3], rcv_ip[0], rcv_ip[1], rcv_ip[2], rcv_ip[3]);
	}
	// print some network info and exit
	printf("\n===================================================================\n\n------ SUBNET INFO ------\n");
	unsigned long ulong_subnet = ulong_sdr_ip & ulong_netmask;
	char str_subnet[36];
	char str_broadcast[36];
	decimal_ip_string(ulong_subnet, str_subnet);
	decimal_ip_string(ulong_sdr_ip | (~ulong_netmask & 0xFFFFFFFF), str_broadcast);
	printf("Subnet name (CIDR):\t%s/%d\n", str_subnet, netmask_set_bits(ulong_netmask));
	printf("Broadcast address:\t%s\n", str_broadcast);
	unsigned long max_clients = 0;
	short nm_bits = netmask_set_bits(ulong_netmask);
	max_clients = (1 << (32 - nm_bits)) - 2;
	max_clients &= 0xFFFFFFFF;
	printf("Max. # of clients: \t%lu\n\n\n", max_clients);
	return main(argc, argv);
}

unsigned long compose_address(unsigned short a, unsigned short b, unsigned short c, unsigned short d) {
	return (a << 24) + (b << 16) + (c << 8) + d;
}

short netmask_set_bits(unsigned long netmask) {
	short ones = 0;
	for (int i=0; i < 32; i++) {
		if ((1 << i) & netmask) ones++;
		else if (ones) return -1;
	}
	return ones;
}

void binary_ip_string(unsigned long ul_ip, char *str_ip) {
	short i = 0;
	for (; i < 8 ; i++) {
		str_ip[i] = ((1 << (31-i)) & ul_ip)?'1':'0';
	}
	str_ip[1*8+0] = '.';
	for (; i < 16; i++) {
		str_ip[i+1] = ((1 << (31-i)) & ul_ip)?'1':'0';
	}
	str_ip[2*8+1] = '.';
	for (; i< 24; i++) {
		str_ip[i+2] = ((1 << (31-i)) & ul_ip)?'1':'0';
	}
	str_ip[3*8+2] = '.';
	for (; i < 32; i++) {
		str_ip[i+3] = ((1 << (31-i)) & ul_ip)?'1':'0';
	}
	str_ip[4*8+3] = '\0';
}

void decimal_ip_string(unsigned long ul_ip, char *str_ip) {
	unsigned short dec_ip[4];
	for (int i = 0; i < 4; i++) {
		dec_ip[i] = (ul_ip >> (8*i)) & 0xFF;
	}
	sprintf(str_ip, "%d.%d.%d.%d", dec_ip[3], dec_ip[2], dec_ip[1], dec_ip[0]);
}
