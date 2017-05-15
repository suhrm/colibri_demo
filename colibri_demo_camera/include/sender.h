#ifndef SENDER
#define SENDER 1
#include <string>

int sendall(int s, char *buf, int *len);

int connect_tcp(std::string ip, int port);

#endif
