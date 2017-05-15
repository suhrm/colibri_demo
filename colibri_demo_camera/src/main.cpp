#include <thread>
#include "dht11.h"
#include "sender.h"
#include <unistd.h>

void sendData();

std::thread t1;
std::thread t2;
int main(int argc, char const *argv[]) {
	t1 = std::thread(dht11_thread);
	t2 = std::thread(sendData);
	while(1){
		usleep(1000000);
	//	dht11_thread();
	}
	return 0;
}


void sendData(){
	struct dht11_struct value;
	int len = sizeof(dht11_struct);
	while(1){
		if(getValues(&value) == 0){
			int socket = connect_tcp("192.168.1.102", 12345);
			sendall(socket, (char*)&value, &len );
			close(socket);
		}

		usleep(2000000);
	}
}
