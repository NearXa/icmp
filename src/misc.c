#include "icmp.h"

in_addr_t get_local_ip_address(void)
{
    struct ifaddrs *ifap, *ifa;
    struct sockaddr_in *sa;
    in_addr_t addr;

    getifaddrs(&ifap);
    for (ifa = ifap; ifa; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr->sa_family==AF_INET)
        {
            sa = (struct sockaddr_in *) ifa->ifa_addr;
            addr = sa->sin_addr.s_addr;
            if (addr != inet_addr("127.0.0.1"))
            {
                break;
            }
        }
    }
    freeifaddrs(ifap);
    return addr;
}

void check_ipv4(char *ip)
{
    struct sockaddr_in addr;
    int result = inet_pton(AF_INET, ip, &(addr.sin_addr));
    if (result == 0)
    {
        fprintf(stderr, "\033[0;31m[!] Invalid IP Address\033[0m\n");
        exit(EXIT_FAILURE);
    }
}

int my_round(float n)
{
    int num = 0;
    if (n > 0)
        n += 0.5;
    else
        n -= 0.5;
    num = n;
    return num;
}

int calc_pkt(FILE *fp)
{
    fseek(fp, 0L, SEEK_END);
    long int res = ftell(fp);
    float size = res;
    size = size / 512;
    size = my_round(size);
    fseek(fp, 0L, SEEK_SET);
    return size;
}

void loading(int current_packet, int total_packets)
{
    int percentage = (current_packet * 100) / total_packets;
    int number_of_hashes = (percentage * BAR_LENGTH) / 100;

    printf("\r[");
    for (int i = 0; i < BAR_LENGTH; ++i)
    {
        if (i < number_of_hashes) {
            if (percentage == 100)
                printf("\033[0;32m█\033[0m");
            else
                printf("\033[0;31m█\033[0m");
        } else {
            printf("▒");
        }
    }
    printf("] %d %% - %d / %d packets ", percentage, current_packet, total_packets);

    fflush(stdout);
}

/*

// int create_dir(const char *DirName)
{
    if (mkdir(DirName, 0777) == -1)
    {
        perror("\033[0;31m[!]Erreur lors de la création du dossier ! \033[0m\n");
        return 1;
    }
    return 0;
}

int file_in_directory(const char *DirName, const char *FileName)
{
    char path[256];
    snprintf(path, sizeof(path), "%s/%s", DirName, FileName);
    FILE *file = fopen(path, "w");
    if (file == NULL)
    {
        perror("\033[0;31m[!]Erreur lors de la création du fichier ! \033[0m\n");
        return 1;
    }
    fclose(file);
    return 0;
}

*/
