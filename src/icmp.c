#include "icmp.h"

unsigned short checksum(void *b, int len)
{
    unsigned short *buf = b;
    unsigned long sum = 0;
    while (len > 1)
    {
        sum += *buf++;
        len -= 2;
    }
    if (len == 1)
    {
        sum += *(unsigned char *)buf;
    }
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

void send_packet(int sockfd, struct packet *icmp, struct sockaddr_in *dest_addr)
{
    ssize_t bytes_sent =
            sendto(sockfd, icmp, sizeof(struct packet) + icmp->payload.dsize, 0,
                (struct sockaddr *)dest_addr, sizeof(struct sockaddr_in));
    if (bytes_sent <= 0)
    {
        perror("\033[0;31m[!] Error while trying to send ICMP packet\033[0m\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Sent %ld bytes\n", bytes_sent);
}

struct packet create_icmp_header(int type, int code, in_addr_t saddr,
                                 in_addr_t daddr)
{
    struct packet pkt;

    pkt.ip.version = 4;
    pkt.ip.ihl = 5;
    pkt.ip.tos = 0;
    pkt.ip.tot_len = htons(sizeof(struct packet));
    pkt.ip.id = rand();
    pkt.ip.frag_off = 0;
    pkt.ip.ttl = 64;
    pkt.ip.protocol = IPPROTO_ICMP;
    pkt.ip.saddr = saddr;
    pkt.ip.daddr = daddr;

    memset(&pkt.icmp, 0, sizeof(pkt.icmp));
    pkt.icmp.type = type;
    pkt.icmp.code = code;

    return pkt;
}

int create_icmp_socket(void)
{
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0)
    {
        perror("\033[0;31m[!] Error while creating ICMP socket\033[0m\n");
        exit(EXIT_FAILURE);
    }

    const int enable = 1;
    if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &enable, sizeof(enable)) < 0)
    {
        perror("\033[0;31m[!] Error while setting up the socket\033[0m\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    return sockfd;
}

void receive_packet(int sockfd)
{
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);
    char buffer[1500];
    ssize_t packet_len = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                                (struct sockaddr *)&sender, &sender_len);
    if (packet_len < 0)
    {
        perror("\033[0;31m[!] Error while trying to receive packets\033[0m\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}

char *xor_encrypt(char *data, char *key)
{
    size_t data_len = strlen(data);
    size_t key_len = strlen(key);

    size_t i;
    for (i = 0; i < data_len; i++)
    {
        data[i] ^= key[i % key_len];
    }
    return data;
}

char *xor_decrypt(char *data, char *key)
{
    size_t key_len = strlen(key);
    if (key_len == 0)
    {
        exit(EXIT_FAILURE);
    }

    return xor_encrypt(data, key);
}
