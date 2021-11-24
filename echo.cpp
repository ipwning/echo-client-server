#include "echo.h"

uint16_t PORT;
bool IS_ECHO;
bool IS_BROADCAST;
struct in_addr IP;

void usage(bool isClient) {
    if(isClient) {
        puts("syntax : echo-client <ip> <port>");
        puts("sample : echo-client 192.168.10.2 1234");
    } else {
        puts("syntax : echo-server <port> [-e[-b]]");
        puts("sample : echo-server 1234 -e -b");
    }
    exit(-1);
}

void argParse(int argc, const char **argv, bool isClient) {
    PORT = 0;
    if(isClient) {
        if(argc != 3) usage(isClient);
        PORT = atoi(argv[2]);
        int res = inet_pton(AF_INET, argv[1], &IP);
        switch (res) {
            case 1: break;
            case 0: std::cerr << "not a valid network address\n"; usage(isClient);
            case -1: perror("inet_pton"); usage(isClient);
        }
    } else {
        if(argc < 2) usage(isClient);
		for (int i = 1; i < argc; i++) {
			if (strncmp(argv[i], "-e", 2) == 0) {
				IS_ECHO = true;
                continue;
			} else if (strncmp(argv[i], "-b", 2) == 0 && IS_ECHO) {
                IS_BROADCAST = true;
                if(PORT) break;
                continue;
            } else {
                PORT = atoi(argv[i]);
            }
		}
    }
}

