#include "fft.h"
#include "max30001.h"
#include "main.h"
#define Fs 64 // sampling frequency

float bioz_data[TEST_LENGTH_SAMPLES] = { 0 };
int counter = 0;

void breathing_frequency_output(void){
	uint32_t maxIndex = 0;
	float maxFrequency = 0, bpm = 0;

	int data=0;
	int predznak=1;

	int int2b=3;
	int btag=55;


	int2b=HAL_GPIO_ReadPin(INT2B_GPIO_Port, INT2B_Pin);

		if(int2b != 1 ) //ako je interrupt pin low
		{
			//printf("int2b = %d\r\n",int2b);
			if (max30001_int_handler() == -1)  //pozovi f-ju koja ce u BIOZFIFO_buffer preko SPI ucitati BIOZ_FIFO (8 odsjecka velicine 24 bita - 18 bit voltage data, 3 bit etag, 3 bit ptag)
			{
	  			//	printf("int_handler failed\r\n");
			}
			else
			{
				for (int indeks=0; indeks<8; indeks++)
			  	{
			  		data=max30001_BIOZ_FIFO_buffer[indeks];
			  		btag=(data>>3)& 0x3; //izdvojim btag
			  		//printf("btag = %d\r\n",btag);
			  		data=(data>>4) & 0xfffff; //micem btag i 1 prazan bit
			  		//	printf("data = %d\r\n",data);

			  		if(data & 0x80000)  //gledam 20. bit
			  		{
			  			predznak=-1;
			  			data=(data^0xfffff)+1;
			  		}
			  		else
			  			predznak=1;

			  		bioz_data[counter*8 + indeks]=(double)(predznak)*(data*1)/(pow(2,19)*96*pow(10,-6)*20); //Vref=1V, bioZ gain=20V/V, 96uA

			  		//poslati taj data na uart
			  		//printf("bioz data = %.5f \n\r",bioz);
			  		printf("%.5f\r\n",bioz_data[counter*8 + indeks]);
			  	}

				counter++;

				if (counter==64)  //skupilo se 512 uzoraka (64*8); Fs=64 Hz i ovo se dogodi svakih 8 sekundi
				{
					IIR_PROCESSING_F32Process(BPM_MODE);
					maxIndex = FFT_PROCESSING_F32Process(BPM_MODE);

					maxFrequency = (float) maxIndex * Fs / FFT_Length;
					bpm = maxFrequency * 60;
					printf("BPM = %f\r\n", bpm);

					counter = 0;            //resetiraj counter
					for(int i=0;i<512;i++)  //resetiraj buffer bioz podataka za IIR
						bioz_data[i] = 0;

				}
			 }
		}

}
