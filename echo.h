#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <list>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// common
void argParse(int argc, const char **argv, bool isClient);
void usage(bool isClient);

//server
void recvThread(int sd);

//client