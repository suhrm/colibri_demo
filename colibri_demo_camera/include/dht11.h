#ifndef DHT11
#define DHT11 1

struct dht11_struct{
	int tempUpper;
	int tempLower;
	int humUpper;
	int humLower;
};

int getValues(struct dht11_struct* _s);
int read_dht11_dat();
void dht11_thread();

#endif
