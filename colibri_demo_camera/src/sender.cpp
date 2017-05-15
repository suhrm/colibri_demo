#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <string.h>

#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>

//C includes
#include <cstdint>
#include <cstring>
#include <cstdlib>
//sys includes
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>

//internet layer related
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <inttypes.h>
#include <netinet/tcp.h>
//misc
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <assert.h>




int connect_tcp(std::string ip, int port){
	int sock;
	struct sockaddr_in server;
	sock = socket(AF_INET , SOCK_STREAM , 0);
	int enable = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
	{
		std::cout << "Error reusing address" << std::endl;
		exit(1);
	}
	if (sock == -1)
	{
		std::cout << "Error Could not create socket" << std::endl;
		exit(1);
	}
	server.sin_addr.s_addr = inet_addr(ip.c_str());
	server.sin_family = AF_INET;
	server.sin_port = htons( port );
	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		std::cout << "error connection failed" << std::endl;
		exit(0);
	}
	return sock;
}



// function used to send all data through tcp connection
int sendall(int s, char *buf, int *len){
	int total = 0;		// how many bytes we've sent
	int bytesleft = *len; // how many we have left to send
	int n;
	while (total < *len) {

    	n = send(s, buf + total, bytesleft, 0);
    	if (n == -1) {
			std::cout << "an error occurred while sending" << std::endl;
			return -1;
		}
		total += n;
		bytesleft -= n;
	}
	*len = total; // return number actually sent here
	return 0; //Succeeded in sending all bytes
}
