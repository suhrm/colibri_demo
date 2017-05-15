#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <mutex>
#include <cstring>
#include <unistd.h>

#include "dht11.h"

std::mutex mtx;

#define MAXTIMINGS	85
#define DHTPIN		7
int dht11_dat[5] = { 0, 0, 0, 0, 0 };

struct dht11_struct local_val;

int status = -1;


int read_dht11_dat()
{


	uint8_t laststate	= HIGH;
	uint8_t counter		= 0;
	uint8_t j		= 0, i;
	float	f;

	dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

	pinMode( DHTPIN, OUTPUT );
	digitalWrite( DHTPIN, LOW );
	delay( 18 );
	digitalWrite( DHTPIN, HIGH );
	delayMicroseconds( 40 );
	pinMode( DHTPIN, INPUT );

	for ( i = 0; i < MAXTIMINGS; i++ )
	{
		counter = 0;
		while ( digitalRead( DHTPIN ) == laststate )
		{
			counter++;
			delayMicroseconds( 1 );
			if ( counter == 255 )
			{
				break;
			}
		}
		laststate = digitalRead( DHTPIN );

		if ( counter == 255 )
			break;

		if ( (i >= 4) && (i % 2 == 0) )
		{
			dht11_dat[j / 8] <<= 1;
			if ( counter > 16 )
				dht11_dat[j / 8] |= 1;
			j++;
		}
	}

	if ( (j >= 40) &&
	     (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) )
	{
		local_val.tempUpper = dht11_dat[2];
		local_val.tempLower = dht11_dat[3];
		local_val.humUpper  = dht11_dat[0];
		local_val.humLower  = dht11_dat[1];
		return 0;
	}else  {
		return -1;
	}
}

void dht11_thread()
{

	if ( wiringPiSetup() == -1 ){
		std::cout << "failed when initializing wiringPi" << '\n';
		exit( 1 );
	}
	while ( 1 )
	{
		mtx.lock();
		status = read_dht11_dat();
		mtx.unlock();
		std::cout << "status: " << status << '\n';
		usleep(1000000);
	}


}


int getValues(struct dht11_struct* _s){
	mtx.lock();
	if(status != -1){
		std::memcpy(_s, &local_val, sizeof(dht11_struct));
		mtx.unlock();
		return 0;
	}
	mtx.unlock();
	return -1;

}
