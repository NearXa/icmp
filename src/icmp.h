#ifndef ICMP_H
#define ICMP_H

#include <arpa/inet.h>
#include <dirent.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <sys/stat.h>

#include "client/client.h"
#include "server/server.h"

#define DATA_MAX_LENGTH 512
#define BAR_LENGTH 50

struct data {
    // char cid[20]; //
    // int tid; //
    // int num; //
    // int total; //
    int dsize;
    char data[DATA_MAX_LENGTH];
};

struct packet {
    struct iphdr ip;
    struct icmphdr icmp;
    struct data payload;
};

int calc_pkt(FILE *fp);
int create_icmp_socket(void);
int my_round(float n);
in_addr_t get_local_ip_address(void);
unsigned short checksum(void *buf, int len);
void check_ipv4(char *ip);
void send_packet(int sockfd, struct packet *icmp, struct sockaddr_in *dest_addr);
char *xor_encrypt(char *data, char *key);
// char *xor_decrypt(char *data, char *key); //
void loading(int current_packet, int total_packets);
void receive_packet(int sockfd);
void read_send(const char *path, int sockfd, struct packet pkt, struct sockaddr_in dest_addr);
struct packet create_icmp_header(int type, int code, in_addr_t saddr, in_addr_t daddr);

// int create_dir(const char *DirName); //
// int file_in_directory(const char *DirName, const char *FileName); //

#endif /* ICMP_H */
