#include "../icmp.h"

void run_server(void)
{
    int sockfd = create_icmp_socket();
    struct in_addr host;
    host.s_addr = get_local_ip_address();
    struct sockaddr_in src_addr;
    struct packet pkt;
    socklen_t addrlen = sizeof(src_addr);
    ssize_t packet_len;

    fprintf(stdout, "\033[0;32m[+] Server is running on %s\033[0m\n\n", inet_ntoa(host));
    while (1)
    {
        packet_len = recvfrom(sockfd, &pkt, sizeof(pkt), 0, (struct sockaddr *)&src_addr, &addrlen);

        if (packet_len < 0)
        {
            perror("\033[0;31m[!] Error receiving packets\033[0m\n");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        printf("Received ICMP Echo Request from: %s\n", inet_ntoa(src_addr.sin_addr));
        printf("\n%s\n", pkt.payload.data);

        if (pkt.icmp.type == ICMP_ECHO)
        {
            struct packet reply = create_icmp_header(ICMP_ECHOREPLY, 0, pkt.ip.daddr, pkt.ip.saddr);
            reply.payload.dsize = pkt.payload.dsize;
            memcpy(reply.payload.data, pkt.payload.data, pkt.payload.dsize);
            reply.icmp.checksum = checksum(&reply.icmp, sizeof(reply.icmp)+ reply.payload.dsize);
            ssize_t bytes_sent = sendto(sockfd, &reply, sizeof(reply.icmp) + sizeof(reply.ip) + pkt.payload.dsize, 0, (struct sockaddr *)&src_addr, addrlen);
            if (bytes_sent <= 0)
            {
                perror("\033[0;31m[!] Error while trying to send ICMP packet\033[0m\n");
                close(sockfd);
                exit(EXIT_FAILURE);
            }
            printf("Sent %ld bytes\n", bytes_sent);
            memset(&pkt.payload.data, 0, sizeof(pkt.payload.data));
        }

    }
    close(sockfd);
}
