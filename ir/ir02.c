#include<wiringPi.h>
#include<stdio.h>
#define IO digitalRead(Pin)

unsigned char i,idx,cnt;
unsigned char count;
unsigned char data[4];

void GetKey(void);

const int Pin = 0;
int proc = 0;

int main(void)
{
    wiringPiSetup();
    pinMode(Pin,OUTPUT);

    wiringPiISR(Pin,INT_EDGE_FALLING,GetKey);

    while(1){
    	delay(1000);
    }
    return 0;
}

void GetKey(void)
{
    if(proc==0){
      proc = 1;
		if (IO == 0) {
			count = 0;
			while (IO == 0 && count++ < 200)   //9ms
				delayMicroseconds(60);

			count = 0;
			while (IO == 1 && count++ < 80)    //4.5ms
				delayMicroseconds(60);
			idx = 0;
			cnt = 0;
			data[0]=0;
			data[1]=0;
			data[2]=0;
			data[3]=0;
			for (i =0;i<32;i++) {
				count = 0;
				while (IO == 0 && count++ < 15)  //0.56ms
					delayMicroseconds(60);

				count = 0;
				while (IO == 1 && count++ < 40)  //0: 0.56ms; 1: 1.69ms
					delayMicroseconds(60);

				if (count > 25)
					data[idx] |= (1<<cnt);

				if (cnt== 7) {
					cnt=0;
					idx++;
				}else{
					cnt++;
				}
			}
			if ((data[0]+data[1] <=0xff)&&(data[2]+data[3]==0xFF)) {
				//printf("0x%02x,0x%02x,0x%02x,0x%02x;Get the key: 0x%02x\n",data[0],data[1],data[2],data[3],data[2]);
				printf("%d,%d,%d,%d;Get the key: 0x%02x\n",data[0],data[1],data[2],data[3],data[2]);
			}
		}
      proc = 0;
    }
}

gcc -Wall ir02.c -o ir02 -lwiringPi
./ir02
