#include "max30001.h"
#include "ICM20948.h"
#include "output_mode.h"
#include "string.h"
#include "stdio.h"
#include "main.h"
/**
  * Output mode change function
  *
  */
float accel_data[3]={ 0 };
float gyro_data[3]={ 0 };
float magn_data[3]={ 0 };
float temp_data=0;

int change_output_mode(char *USB_command){
	int return_value = DSP_OUTPUT_MODE;  //default output mode select

	if (strncmp(USB_command, "MODE_ECG", 8) == 0) return_value = ECG_OUTPUT_MODE;
	if (strncmp(USB_command, "MODE_BIOZ", 9) == 0) return_value = BIOZ_OUTPUT_MODE;
	if (strncmp(USB_command, "MODE_RTOR", 9) == 0) return_value = RTOR_OUTPUT_MODE;
	if (strncmp(USB_command, "MODE_COMBINED", 13) == 0) return_value = COMBINED_OUTPUT_MODE;
	if (strncmp(USB_command, "MODE_ACCEL", 10) == 0) return_value = ACCEL_OUTPUT_MODE;
	if (strncmp(USB_command, "MODE_GYRO", 9) == 0) return_value = GYRO_OUTPUT_MODE;
	if (strncmp(USB_command, "MODE_MAGN", 9) == 0) return_value = MAGN_OUTPUT_MODE;
	if (strncmp(USB_command, "MODE_TEMP", 9) == 0) return_value = TEMP_OUTPUT_MODE;
	if (strncmp(USB_command, "MODE_DSP", 8) == 0) return_value = DSP_OUTPUT_MODE;
	if (strncmp(USB_command, "MODE_BPM", 8) == 0) return_value = BPM_OUTPUT_MODE;

	return return_value;
}

void ecg_output_mode(void){
	  int data=0;
	  int predznak=1;
	  double ecg_voltage=0;
	  int intb=3;
	  int etag=55;
	  int return_value = 0;

	  intb=HAL_GPIO_ReadPin(INTB_GPIO_Port, INTB_Pin);

		  		if(intb == 0 ) //ako je interrupt pin low
		  		{
		  			//printf("intb = %d\r\n",intb);
		  			return_value = max30001_int_handler();

		  			if (return_value == -1)  //pozovi f-ju koja ce u ECG_FIFO_buffer preko SPI ucitati ECG_FIFO (32 odsjecka velicine 24 bita - 18 bit voltage data, 3 bit etag, 3 bit ptag)
		  			{
		  			//	printf("int_handler failed\r\n");
		  			}
		  			else if (return_value == 0)  //int je izazvan zbog eint ili eovf, a ne rrint
		  			{
		  				for (int indeks=0; indeks<16; indeks++)
		  				{
		  					data=max30001_ECG_FIFO_buffer[indeks];
		  					etag=(data>>3)& 0x3; //izdvojim etag
		  					//if(etag != 0) printf("etag = %d\r\n",etag);
		  					data=(data>>6) & 0x3ffff; //micem etag i ptag
		  				//	printf("data = %d\r\n",data);

		  					if(data & 0x20000)  //gledam 18. bit
		  					{
		  						predznak=-1;
		  						data=(data^0x3ffff)+1;
		  					}
		  					else
		  						predznak=1;

		  					ecg_voltage=(double)(predznak)*(data*1000)/(pow(2,17)*20); //Vref=1000mV?, ecg gain=20V/V

		  					//poslati taj data na uart
		  					//printf("voltage data = %.3f \n\r",ecg_voltage);
		  					printf("%.3f\r\n",ecg_voltage);
		  				}

		  			}
		  			else /*do nothing*/;
		  		}
}

void bioz_output_mode(void){
	  int data=0;
	  int predznak=1;
	  double bioz=0;
	  int int2b=3;
	  int btag=55;
		  int2b=HAL_GPIO_ReadPin(INT2B_GPIO_Port, INT2B_Pin);

		  		if(int2b != 1 ) //ako je neki od interrupt pinova low
		  		{
		  			//printf("int2b = %d\r\n",int2b);
		  			if (max30001_int_handler() == -1)  //pozovi f-ju koja ce u ECG_FIFO_buffer preko SPI ucitati ECG_FIFO (32 odsjecka velicine 24 bita - 18 bit voltage data, 3 bit etag, 3 bit ptag)
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

		  					bioz=(double)(predznak)*(data*1)/(pow(2,19)*96*pow(10,-6)*20); //Vref=1V, bioZ gain=20V/V, 96uA

		  					//poslati taj data na uart
		  					//printf("bioz data = %.5f \n\r",bioz);
		  					printf("%.5f\r\n",bioz);
		  				}

		  			}
		  		}
}

void rtor_output_mode(void){
	double hr=0;
	double r2r_time=0;
	int intb=3;
	int return_value = 0;
	intb=HAL_GPIO_ReadPin(INTB_GPIO_Port, INTB_Pin);

	 if (intb==0)
		{
		 return_value = max30001_int_handler();

		 if (return_value == -1)
		 {
		 printf("int_handler failed\r\n");
		 }
		 else if(return_value == 1)  //int je izazvan zbog rrint, a ne eint ili eovf
		 {

		//	printf("%d\r\n",hspValMax30001.R2R);
			r2r_time=(double)hspValMax30001.R2R * 0.0078125;
		//	printf("%f\r\n",r2r_time);
			hr=60/r2r_time;
			printf("%f\r\n",hr);
		 }
		 else /*do nothing*/ ;
		}
}

void combined_output_mode(void){
	// Obtain accelerometer and gyro data
	ICM_ReadAccelGyro();
	// Obtain magnetometer data
	ICM_ReadMag();
	// Obtain temperature sensore data
	ICM_ReadTemp();

	printf("Trenutno stanje : (Ax: %f | Ay: %f | Az: %f)   (Gx: %f | Gy: %f | Gz: %f)   (Mx: %f | My: %f | Mz: %f)  Temp: %f \r\n",
			  					accel_data[0], accel_data[1], accel_data[2],
			  					gyro_data[0], gyro_data[1], gyro_data[2],
			  					magn_data[0], magn_data[1], magn_data[2],temp_data);
}

void accel_output_mode(void){
	// Obtain accelerometer and gyro data
	ICM_ReadAccelGyro();
	printf("(Ax: %f | Ay: %f | Az: %f)\r\n", accel_data[0], accel_data[1], accel_data[2]);
}
void gyro_output_mode(void){
	// Obtain accelerometer and gyro data
	ICM_ReadAccelGyro();
	printf("(Gx: %f | Gy: %f | Gz: %f)\r\n", gyro_data[0], gyro_data[1], gyro_data[2]);
}
void magn_output_mode(void){
	// Obtain accelerometer and gyro data
	ICM_ReadMag();
	printf("(Mx: %f | My: %f | Mz: %f)\r\n", magn_data[0], magn_data[1], magn_data[2]);
}
void temp_output_mode(void){
	// Obtain accelerometer and gyro data
	ICM_ReadTemp();
	printf("Temp: %f \r\n", temp_data);
}
