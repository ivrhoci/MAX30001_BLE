#include "fft.h"
#include "max30001.h"
#include "main.h"
#include "crs_app.h"
#include "app_conf.h"
#include "output_mode.h"
#include "blackman.h"

#define Fs 64 // sampling frequency
#define bioz_max30001_buff_size 8 //bioz data buffer size on max30001
#define max_count BLOCK_SIZE/Fs*bioz_max30001_buff_size
#define shift_percentage 0.25

float gaussian_interpolation(int index, int mode);
void fft_data_sumation(void);
int find_max_index(float *array, int size);

float bioz_data[BLOCK_SIZE] = { 0 };
float fft_data_sum[52] = { 0 }; //52 uzorka fft-a sumiram jer to pokriva spektar do 0.8 Hz
int counter = 0;



float breathing_frequency_output(int mode){
	uint32_t maxIndex = 0;
	float maxFrequency = 0, bpm = 0;
	char ble_buffer[10] = { 0 };
	int data=0;
	int predznak=1;
	int int2b=3;
	int btag=55;

	if (max30001_int_handler() == -1)  //pozovi f-ju koja ce u BIOZFIFO_buffer preko SPI ucitati BIOZ_FIFO
	{
		printf("int_handler failed\r\n");
	}
	else
	{
		for (int indeks=0; indeks<8; indeks++)
	  	{
	  		data=max30001_BIOZ_FIFO_buffer[indeks];
	  		btag=(data>>3)& 0x3; //izdvojim btag
	  		//printf("btag = %d\r\n",btag);
	  		data=(data>>4) & 0xfffff; //micem btag i 1 prazan bit
	 		//printf("data = %d\r\n",data);

	  		if(data & 0x80000)  //gledam 20. bit
	  		{
	  			predznak=-1;
	  			data=(data^0xfffff)+1;
	  		}
	  		else
	  			predznak=1;

	  		bioz_data[counter*8 + indeks]=(double)(predznak)*(data*1)/(pow(2,19)*96*pow(10,-6)*20); //Vref=1V, bioZ gain=20V/V, 96uA

	  		if (mode == BPM60_BIOZ_OUTPUT_MODE || BPM30_BIOZ_OUTPUT_MODE)
	  		{
			  	sprintf(ble_buffer, "%.5f\r\n",bioz_data[counter*8 + indeks]);
			  	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
	  		}
	  		//poslati taj data na uart
	  		//printf("bioz data = %.5f \n\r",bioz);
//	  		printf("%.5f\r\n",bioz_data[counter*8 + indeks]);
	  	}

		counter++;

		if (counter==24)  //skupilo se 512 uzoraka (64*8); Fs=64 Hz i ovo se dogodi svakih 8 sekundi
		{
			IIR_PROCESSING_F32Process(BPM_MODE);
			maxIndex = FFT_PROCESSING_F32Process(BPM_MODE);

			maxFrequency = (float) maxIndex * Fs / FFT_Length;
			bpm = maxFrequency * 60;
			printf("BPM = %f\r\n", bpm);

			counter = 0;            //resetiraj counter
			for(int i=0;i<512;i++)  //resetiraj buffer bioz podataka za IIR
				bioz_data[i] = 0;

			return bpm; //vrati bpm da se moze poslat preko BLE
		}
	}
	return 0;
}

int window_num = 0;
int first_entry_done = 0;
int first_minute_bpm_calc_done = 0;
float bpm[12] = { 0 };

float breathing_frequency_output_V2(int mode){
	uint32_t maxIndex = 0;
	float maxFrequency = 0, bpm_mean = 0;
	char ble_buffer[10] = { 0 };
	int data=0;
	int predznak=1;
	int int2b=3;
	int btag=55;
	int max_window_num = Fs*60/(BLOCK_SIZE*shift_percentage); //64*60=3840 uzoraka u minuti, 3840/320=12 prozora u minuti
	float gauss_index_shift = 0;
	float bpm_final = 0;
	int shift = 0;

	if (mode == BPM60_BIOZ_OUTPUT_MODE)
	{
		if(!first_minute_bpm_calc_done)
		{
			max_window_num = (Fs*60 - BLOCK_SIZE*(1 - shift_percentage))/(BLOCK_SIZE*shift_percentage); // 9 prozora u prvoj minuti
		}
	}
	else if(mode == BPM30_BIOZ_OUTPUT_MODE)
	{
		if(!first_minute_bpm_calc_done)
		{
			max_window_num = (Fs*30 - BLOCK_SIZE*(1 - shift_percentage))/(BLOCK_SIZE*shift_percentage); // 3 prozora u prvih 30s
		}
		else
			max_window_num = Fs*30/(BLOCK_SIZE*shift_percentage); //6 prozora u 30s
	}



	if (max30001_int_handler() == -1)  //pozovi f-ju koja ce u BIOZFIFO_buffer preko SPI ucitati BIOZ_FIFO
	{
		printf("int_handler failed\r\n");
	}
	else
	{
		for (int indeks=0; indeks<bioz_max30001_buff_size; indeks++)
	  	{
	  		data=max30001_BIOZ_FIFO_buffer[indeks];
	  		btag=(data>>3)& 0x3; //izdvojim btag
	  		//printf("btag = %d\r\n",btag);
	  		data=(data>>4) & 0xfffff; //micem btag i 1 prazan bit
	 		//printf("data = %d\r\n",data);

	  		if(data & 0x80000)  //gledam 20. bit
	  		{
	  			predznak=-1;
	  			data=(data^0xfffff)+1;
	  		}
	  		else
	  			predznak=1;
	  		//zapisuj u gornju cetvrtinu ako nije prvi zapis
	  		shift = first_entry_done*BLOCK_SIZE*(1-shift_percentage) + counter*bioz_max30001_buff_size;
	  		bioz_data[shift + indeks]=(double)(predznak)*(data*1)/(pow(2,19)*96*pow(10,-6)*20); //Vref=1V, bioZ gain=20V/V, 96uA

	  		//posalji bioz preko UART i BLE
	  		if (mode == BPM60_BIOZ_OUTPUT_MODE || mode == BPM30_BIOZ_OUTPUT_MODE)
	  		{
	  			printf("%.5f\r\n",bioz_data[shift + indeks]);
			  	sprintf(ble_buffer, "%.5f\r\n",bioz_data[shift + indeks]);
			  	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
	  		}
	  	}

		counter++; //povecaj brojac koji broji koliko puta je procitan FIFO s MAX30001

		if (!first_entry_done) //ako je ova funkcija pozvana 1.put puni se cijeli buffer; pri sljedecim pozivima punimo samo gornja cetvrtina buffera
		{
			if (counter==max_count)  //skupilo se BLOCK_SIZE uzoraka (max_count*bioz_max30001_buff_size); Fs=64 Hz i ovo se dogodi svakih 20 sekundi
			{
				IIR_PROCESSING_F32Process(BPM_MODE); //filtriranje
				arm_mult_f32(&aIIR_F32_Output_Scaled[0],&blackman_window[0],&aIIR_F32_Output_Scaled[0],BLOCK_SIZE); //blackman window
				maxIndex = FFT_PROCESSING_F32Process(BPM_MODE); //fft

				fft_data_sumation(); //dodaj spektar ovog okvira spektrima proslih okvira
				maxIndex = gaussian_interpolation(maxIndex, 0); //mode = 0, use Cmplx_Mag_Output_f32 data ->NOTE: ovo zapravo ne radi kako treba jer je maxIndex unit32_t, a ne float

				maxFrequency = (float) maxIndex * Fs / FFT_Length;
				bpm[window_num] = maxFrequency * 60;
				printf("BPM[%d] = %f\r\n", window_num, bpm[window_num]);

				counter = 0;            //resetiraj counter

				for(int i=0;i<BLOCK_SIZE;i++)  //pomakni podatke u bufferu za cetvrtinu duljine prozora i ostatak zamijeni s 0
				{
					if(i<BLOCK_SIZE*(1-shift_percentage))
						bioz_data[i] = bioz_data[i + (int)(BLOCK_SIZE*shift_percentage)];  //gornjih 75% podataka polovica -> donjh 75% podataka
					else
						bioz_data[i] = 0;                 //gornjih 25% podataka = 0
				}
				first_entry_done = 1;
				window_num++;
			}
			first_entry_done = 1;
//			window_num++;
		}
		else if (counter==max_count*shift_percentage)  //skupilo se shift_percentage*BLOCK_SIZE uzoraka (40*8=320); Fs=64 Hz i ovo se dogodi svakih 5 sekundi
		{
			IIR_PROCESSING_F32Process(BPM_MODE); //filtriranje
			arm_mult_f32(&aIIR_F32_Output_Scaled[0],&blackman_window[0],&aIIR_F32_Output_Scaled[0],BLOCK_SIZE); //blackman window
			maxIndex = FFT_PROCESSING_F32Process(BPM_MODE); //fft

			maxIndex = gaussian_interpolation(maxIndex, 0);  //mode = 0, use Cmplx_Mag_Output_f32 data ->NOTE: ovo zapravo ne radi kako treba jer je maxIndex unit32_t, a ne float
			fft_data_sumation();

			maxFrequency = (float) maxIndex * Fs / FFT_Length;
			bpm[window_num] = maxFrequency * 60;
			printf("BPM[%d] = %f\r\n", window_num, bpm[window_num]);

			counter = 0;            //resetiraj counter

			for(int i=0;i<BLOCK_SIZE;i++)  //pomakni podatke u bufferu za cetvrtinu duljine prozora i ostatak zamijeni s 0
			{
				if(i<BLOCK_SIZE*(1-shift_percentage))
					bioz_data[i] = bioz_data[i + (int)(BLOCK_SIZE*shift_percentage)];  //gornjih 75% podataka polovica -> donjh 75% podataka
				else
					bioz_data[i] = 0;                //gornjih 25% podataka = 0
			}

			if (window_num < max_window_num - 1)
			{
				window_num++;
			}
			else //kraj ciklusa od 30 ili 60s
			{
				//ispisi na UART izracunati BPM za svaki od pojedinih okvira
				for(int i=0;i<max_window_num;i++){
					printf("BPM[%d] = %f\r\n", i, bpm[i]);
					bpm_mean = bpm_mean + bpm[i];
				}
				//ispisi na UART prosjecni BPM
				printf("BPM mean = %f\r\n", bpm_mean/max_window_num);

				//izracunaj BPM na temelju zbrojenog spektra
				//odredit max_index
				maxIndex = find_max_index(&fft_data_sum[0], 52);
				//gaussova interpolacija
				maxIndex = gaussian_interpolation(maxIndex, 1);  //mode = 1, use fft_data_sum data ->NOTE: ovo zapravo ne radi kako treba jer je maxIndex unit32_t, a ne float
				//izracunaj konacni BPM
				maxFrequency = (float) maxIndex * Fs / FFT_Length;
				bpm_final = maxFrequency * 60;
				//pripremi se za sljedeci ciklus
				first_minute_bpm_calc_done = 1; //prosla je prva minuta
				window_num = 0;
				for (int i=0;i<52;i++)
				{
					fft_data_sum[i] = 0;        //suma FFT-ova se postavlja na 0
				}

				return bpm_final; //vrati bpm da se moze poslat preko BLE
			}
		}
	}
	return 0;
}

float gaussian_interpolation(int index, int mode){
	float gauss_index_shift = 0;

	if(mode == 0)
		gauss_index_shift = log(Cmplx_Mag_Output_f32[index + 1]/Cmplx_Mag_Output_f32[index - 1])/(2*log((Cmplx_Mag_Output_f32[index]*Cmplx_Mag_Output_f32[index])/(Cmplx_Mag_Output_f32[index + 1]*Cmplx_Mag_Output_f32[index - 1])));
	if(mode == 1)
		gauss_index_shift = log(fft_data_sum[index + 1]/fft_data_sum[index - 1])/(2*log((fft_data_sum[index]*fft_data_sum[index])/(fft_data_sum[index + 1]*fft_data_sum[index - 1])));

	return (index + gauss_index_shift);
}

void fft_data_sumation(void){
	for (int i=0;i<52;i++)
	{
		fft_data_sum[i] = fft_data_sum[i] + Cmplx_Mag_Output_f32[i];
	}
}

int find_max_index(float *array, int size){
	int max_index = 0;
	float max = array[0];

	for(int i=1;i<size;i++)
	{
		if(array[i]>max)
		{
			max = array[i];
			max_index = i;
		}
	}
	return max_index;
}
void bpm_calc_init(void){
	window_num = 0;
	first_entry_done = 0;
	first_minute_bpm_calc_done = 0;
	counter = 0;
}

