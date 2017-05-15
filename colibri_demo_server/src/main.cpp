#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
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

#include "dht11.h"



int recvall(int s, char *buf, int *len);
void listen_tcp(int TCPPORT);
void send_command_but(int index);
void send_command_ax(int index);



int main(int argc, char const *argv[]) {
  /* code */
  listen_tcp(12345);
  return 0;
}

int recvall(int s, char *buf, int *len) {
	int total = 0;		// how many bytes we've sent
	int bytesleft = *len; // how many we have left to send
	int n;
	while (total < *len) {

		n = recv(s, buf + total, bytesleft, 0);
		if (n == -1) {
			std::cout << "an error occurred while recv" << std::endl;
			return -1;
		}
		total += n;
		bytesleft -= n;
	}

	*len = total; // return number actually sent here
	return 0; //Succeeded in sending all bytes
}



void listen_tcp(int TCPPORT){
	  int sockfd, newsockfd,conf,check;
    struct sockaddr_in serv_addr, cli_addr; // make a server and client
	  socklen_t clilen = sizeof(cli_addr);
    int yes =1;
    // creates the socket descriptor. AF_INET = IPv4, SOCK_STREAM = standard TCP, 0 = default
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
	{
		std::cout<< "Cannot create TCP user socket--socketfd"<<std::endl;
		exit(1);
	}

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
	{
		perror("Setsockopt failed");
		exit(1);
    }

	memset(&serv_addr, 0, sizeof(serv_addr)); // clear the structs to make sure everything is NULL
    serv_addr.sin_family = AF_INET;          // sets IPv4
    serv_addr.sin_addr.s_addr = INADDR_ANY; // use my IPv4 address,  use all available interfaces
    serv_addr.sin_port = htons(TCPPORT);     // sets port to listen

	if(bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) // Binds the listener to the IP-address and do a error check with the if
    {
        std::cout<<"Cannot bind TCP user socket--socketfd"<<std::endl;
        exit(1);
    }
	 // Sets the non blocking flag
    int flags = fcntl(sockfd, F_GETFL, 0);// Gets the file access mode
    if (flags == -1){
        exit(2);
    }
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK); // Sets the nonblock flag

	listen(sockfd,5); // Make a listen with a queue
	while(1)
	{
		newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0)
		{
			if(errno != EWOULDBLOCK || errno !=EAGAIN)
			{
				perror("ListenCam: ERROR on accept");
				close(newsockfd);
			}
			else
			{// if no connection sleep
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}
		else
		{
      int size = -1;
			struct dht11_struct value;
      recv(newsockfd, (char*)&value , sizeof(dht11_struct), 0);
			std::cout << "temp" << value.tempUpper << "." << value.tempLower << '\n';
			std::cout << "hum" << value.humUpper << "." << value.humLower << '\n';



	  }
  }
	close(sockfd); // close socket if thread exits
}
