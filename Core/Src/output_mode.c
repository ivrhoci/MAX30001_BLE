#include "max30001.h"
#include "ICM20948.h"
#include "output_mode.h"
#include "string.h"
#include "stdio.h"
#include "main.h"
#include "crs_app.h"
#include "app_conf.h"
#include "stm32_seq.h"
#include "BPM_calc.h"

float accel_data[3]={ 0 };
float gyro_data[3]={ 0 };
float magn_data[3]={ 0 };
float temp_data=0;

/**
  * Output mode change function
  *
  */
int change_output_mode(char *USB_command){
	int return_value = 0;  //default output mode select
	char buffer[100]="";

	if (strncmp(USB_command, "MODE_HR_ECG", 11) == 0)
	{
		HAL_NVIC_DisableIRQ(EXTI4_IRQn);
		HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
		return_value = HR_ECG_OUTPUT_MODE;
		UTIL_SEQ_RegTask( 1<<CFG_TASK_HR_ECG_OUTPUT_MODE , UTIL_SEQ_RFU, hr_ecg_output_mode);
		UTIL_SEQ_SetTask( 1<<CFG_TASK_HR_ECG_OUTPUT_MODE, CFG_SCH_PRIO_0);
		sprintf(buffer, "HR and ECG Output Mode selected\r\n");
		CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
	}
	else if (strncmp(USB_command, "MODE_BPM60_BIOZ", 13) == 0)
	{
		HAL_NVIC_DisableIRQ(EXTI4_IRQn);
		HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
		return_value = BPM60_BIOZ_OUTPUT_MODE;
		bpm_calc_init(); //resetira broj okvira i flagove za pocetno punjenje buffera
		UTIL_SEQ_RegTask( 1<<CFG_TASK_BPM60_BIOZ_OUTPUT_MODE , UTIL_SEQ_RFU, bpm60_bioz_output_mode);
		UTIL_SEQ_SetTask( 1<<CFG_TASK_BPM60_BIOZ_OUTPUT_MODE, CFG_SCH_PRIO_0);
		sprintf(buffer, "BPM 60s and BIOZ Output Mode selected\r\n");
		CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
	}
	else if (strncmp(USB_command, "MODE_BPM30_BIOZ", 13) == 0)
	{
		HAL_NVIC_DisableIRQ(EXTI4_IRQn);
		HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
		bpm_calc_init(); //resetira broj okvira i flagove za pocetno punjenje buffera
		return_value = BPM30_BIOZ_OUTPUT_MODE;
		UTIL_SEQ_RegTask( 1<<CFG_TASK_BPM30_BIOZ_OUTPUT_MODE , UTIL_SEQ_RFU, bpm30_bioz_output_mode);
		UTIL_SEQ_SetTask( 1<<CFG_TASK_BPM30_BIOZ_OUTPUT_MODE, CFG_SCH_PRIO_0);
		sprintf(buffer, "BPM 30s and BIOZ Output Mode selected\r\n");
		CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
	}
	else if (strncmp(USB_command, "MODE_ECG", 8) == 0)
	{
		HAL_NVIC_DisableIRQ(EXTI4_IRQn);
		HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
		return_value = ECG_OUTPUT_MODE;
		UTIL_SEQ_RegTask( 1<<CFG_TASK_ECG_OUTPUT_MODE , UTIL_SEQ_RFU, ecg_output_mode);
		UTIL_SEQ_SetTask( 1<<CFG_TASK_ECG_OUTPUT_MODE, CFG_SCH_PRIO_0);
		sprintf(buffer, "ECG Output Mode selected\r\n");
		CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
	}
	else if (strncmp(USB_command, "MODE_BIOZ", 9) == 0)
	{
		HAL_NVIC_DisableIRQ(EXTI4_IRQn);
		HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
		return_value = BIOZ_OUTPUT_MODE;
		UTIL_SEQ_RegTask( 1<<CFG_TASK_BIOZ_OUTPUT_MODE , UTIL_SEQ_RFU, bioz_output_mode);
		UTIL_SEQ_SetTask( 1<<CFG_TASK_BIOZ_OUTPUT_MODE, CFG_SCH_PRIO_0);
		sprintf(buffer, "BIOZ Output Mode selected\r\n");
		CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
	}
	else if (strncmp(USB_command, "MODE_RTOR", 9) == 0)
	{
		HAL_NVIC_DisableIRQ(EXTI4_IRQn);
		HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
		return_value = RTOR_OUTPUT_MODE;
		UTIL_SEQ_RegTask( 1<<CFG_TASK_RTOR_OUTPUT_MODE , UTIL_SEQ_RFU, rtor_output_mode);
		UTIL_SEQ_SetTask( 1<<CFG_TASK_RTOR_OUTPUT_MODE, CFG_SCH_PRIO_0);
		sprintf(buffer, "RTOR Output Mode selected\r\n");
		CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
	}
	else if (strncmp(USB_command, "MODE_COMBINED", 13) == 0)
	{
		HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
		HAL_NVIC_EnableIRQ(EXTI4_IRQn);
		return_value = COMBINED_OUTPUT_MODE;
		UTIL_SEQ_RegTask( 1<<CFG_TASK_COMBINED_OUTPUT_MODE , UTIL_SEQ_RFU, combined_output_mode);
		UTIL_SEQ_SetTask( 1<<CFG_TASK_COMBINED_OUTPUT_MODE, CFG_SCH_PRIO_0);
		sprintf(buffer, "Combined Output Mode selected\r\n");
		CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
	}
	else if (strncmp(USB_command, "MODE_ACCEL", 10) == 0)
	{
		HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
		HAL_NVIC_EnableIRQ(EXTI4_IRQn);
		return_value = ACCEL_OUTPUT_MODE;
		UTIL_SEQ_RegTask( 1<<CFG_TASK_ACCEL_OUTPUT_MODE , UTIL_SEQ_RFU, accel_output_mode);
		UTIL_SEQ_SetTask( 1<<CFG_TASK_ACCEL_OUTPUT_MODE, CFG_SCH_PRIO_0);
		sprintf(buffer, "Accel Output Mode selected\r\n");
		CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
	}
	else if (strncmp(USB_command, "MODE_GYRO", 9) == 0)
	{
		HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
		HAL_NVIC_EnableIRQ(EXTI4_IRQn);
		return_value = GYRO_OUTPUT_MODE;
		UTIL_SEQ_RegTask( 1<<CFG_TASK_GYRO_OUTPUT_MODE , UTIL_SEQ_RFU, gyro_output_mode);
		UTIL_SEQ_SetTask( 1<<CFG_TASK_GYRO_OUTPUT_MODE, CFG_PRIO_NBR);
		sprintf(buffer, "Gyro Output Mode selected\r\n");
		CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
	}
	else if (strncmp(USB_command, "MODE_MAGN", 9) == 0)
	{
		HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
		HAL_NVIC_EnableIRQ(EXTI4_IRQn);
		return_value = MAGN_OUTPUT_MODE;
		UTIL_SEQ_RegTask( 1<<CFG_TASK_MAGN_OUTPUT_MODE , UTIL_SEQ_RFU, magn_output_mode);
		UTIL_SEQ_SetTask( 1<<CFG_TASK_MAGN_OUTPUT_MODE, CFG_SCH_PRIO_0);
		sprintf(buffer, "Magn Output Mode selected\r\n");
		CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
	}
	else if (strncmp(USB_command, "MODE_TEMP", 9) == 0)
	{
		HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
		HAL_NVIC_EnableIRQ(EXTI4_IRQn);
		return_value = TEMP_OUTPUT_MODE;
		UTIL_SEQ_RegTask( 1<<CFG_TASK_TEMP_OUTPUT_MODE , UTIL_SEQ_RFU, temp_output_mode);
		UTIL_SEQ_SetTask( 1<<CFG_TASK_TEMP_OUTPUT_MODE, CFG_SCH_PRIO_0);
		sprintf(buffer, "Temp Output Mode selected\r\n");
		CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
	}
	else if (strncmp(USB_command, "MODE_DSP", 8) == 0)
	{
		HAL_NVIC_DisableIRQ(EXTI4_IRQn);
		HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
		return_value = DSP_OUTPUT_MODE;
		UTIL_SEQ_RegTask( 1<<CFG_TASK_DSP_OUTPUT_MODE , UTIL_SEQ_RFU, dsp_output_mode);
		UTIL_SEQ_SetTask( 1<<CFG_TASK_DSP_OUTPUT_MODE, CFG_SCH_PRIO_0);
		sprintf(buffer, "DSP Output Mode selected\r\n");
		CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
	}
	else if (strncmp(USB_command, "MODE_BPM", 8) == 0)
	{
		HAL_NVIC_DisableIRQ(EXTI4_IRQn);
		HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
		return_value = BPM_OUTPUT_MODE;
		UTIL_SEQ_RegTask( 1<<CFG_TASK_BPM_OUTPUT_MODE , UTIL_SEQ_RFU, bpm_output_mode);
		UTIL_SEQ_SetTask( 1<<CFG_TASK_BPM_OUTPUT_MODE, CFG_SCH_PRIO_0);
		sprintf(buffer, "BPM Output Mode selected\r\n");
		CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
	}

	return return_value;
}

void ecg_output_mode(void){
	int data=0;
	int predznak=1;
	double ecg_voltage=0;
	int intb=3;
	int etag=55;
	int return_value = 0;
	char ble_buffer[10] = { 0 };

	//zovem max30001 interrupt handler -> cita podatke sa buffera i zapisuje u ECG_FIFO_buffer
	return_value = max30001_int_handler();

	if (return_value == -1)
	{
		printf("int_handler failed\r\n");
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

			//određivanje predznaka
			if(data & 0x20000)  //gledam 18. bit
			{
				predznak=-1;
				data=(data^0x3ffff)+1;
			}
			else
				predznak=1;

			//racunanje ecg napona -> GAIN=20!
			ecg_voltage=(double)(predznak)*(data*1000)/(pow(2,17)*20); //Vref=1000mV, ecg gain=20V/V

			//slanje podatka na UART i preko BLE
			printf("%.3f\r\n",ecg_voltage);
			sprintf(ble_buffer, "%.3f\r\n",ecg_voltage);
			CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
		}
	}
}

void bioz_output_mode(void){
	int data=0;
	int predznak=1;
	double bioz=0;
	int int2b=3;
	int btag=55;
	char ble_buffer[10] = { 0 };

	//zovem max30001 interrupt handler -> cita podatke sa buffera i zapisuje u BIOZ_FIFO_buffer
	if (max30001_int_handler() == -1)
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
		//	printf("data = %d\r\n",data);

			if(data & 0x80000)  //gledam 20. bit
		  	{
		  		predznak=-1;
		  		data=(data^0xfffff)+1;
		  	}
		  	else
		  		predznak=1;

			//racunanje bioimpedancije -> GAIN=20, I_ampl=96uA
		  	bioz=(double)(predznak)*(data*1)/(pow(2,19)*96*pow(10,-6)*20); //Vref=1V, bioZ gain=20V/V, 96uA

			//slanje podatka na UART i preko BLE
		  	printf("%.5f\r\n",bioz);
		  	sprintf(ble_buffer, "%.5f\r\n",bioz);
		  	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
		}
	}
}

void rtor_output_mode(void){
	double hr=0;
	double r2r_time=0;
	int intb=3;
	int return_value = 0;
	char ble_buffer[15] = { 0 };
	//zovem max30001 interrupt handler
	return_value = max30001_int_handler();
	if (return_value == -1)
	{
		printf("int_handler failed\r\n");
	}
	else if(return_value == 1)  //int je izazvan zbog rrint, a ne eint ili eovf
	{

		r2r_time=(double)hspValMax30001.R2R * 0.0078125;
		hr=60/r2r_time;

		printf("%f\r\n",hr);
	  	sprintf(ble_buffer, "%f\r\n",hr);
	  	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
	}
	else /*do nothing*/ ;
}

void combined_output_mode(void){
	char ble_buffer[150] = { 0 };

	// Obtain accelerometer and gyro data
	ICM_ReadAccelGyro();
	// Obtain magnetometer data
	ICM_ReadMag();
	// Obtain temperature sensore data
	ICM_ReadTemp();

	//send data over UART
	printf("(Ax: %f | Ay: %f | Az: %f)   (Gx: %f | Gy: %f | Gz: %f)   (Mx: %f | My: %f | Mz: %f)  Temp: %f \r\n",
			  					accel_data[0], accel_data[1], accel_data[2],
			  					gyro_data[0], gyro_data[1], gyro_data[2],
			  					magn_data[0], magn_data[1], magn_data[2], temp_data);
	//send data over BLE
		//accel data
	sprintf(ble_buffer, "%f,", accel_data[0]);                            //šalje se i "," jer pomoću njega SerialPlot prepoznaje vise vrsta podataka
	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
	sprintf(ble_buffer, "%f,", accel_data[1]);
	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
	sprintf(ble_buffer, "%f,", accel_data[2]);
	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
		//gyro data
	sprintf(ble_buffer, "%f,", gyro_data[0]);
	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
	sprintf(ble_buffer, "%f,", gyro_data[1]);
	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
	sprintf(ble_buffer, "%f,", gyro_data[2]);
	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
		//magn data
  	sprintf(ble_buffer, "%f,", magn_data[0]);
	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
	sprintf(ble_buffer, "%f,", magn_data[1]);
	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
	sprintf(ble_buffer, "%f,", magn_data[2]);
	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
		//temp data
	sprintf(ble_buffer, "%f\r\n", temp_data);
  	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
}

void accel_output_mode(void){
	char ble_buffer[50] = { 0 };

	// Obtain accelerometer and gyro data
	ICM_ReadAccelGyro();

	//send data over UART
	printf("(Ax: %f | Ay: %f | Az: %f)\r\n", accel_data[0], accel_data[1], accel_data[2]);
	//send data over BLE
	sprintf(ble_buffer, "%f,", accel_data[0]);
	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
	sprintf(ble_buffer, "%f,", accel_data[1]);
	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
	sprintf(ble_buffer, "%f\r\n", accel_data[2]);
	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
}

void gyro_output_mode(void){
	char ble_buffer[50] = { 0 };
	// Obtain accelerometer and gyro data

	ICM_ReadAccelGyro();

	//send data over UART
	printf("(Gx: %f | Gy: %f | Gz: %f)\r\n", gyro_data[0], gyro_data[1], gyro_data[2]);
	//send data over BLE
	sprintf(ble_buffer, "%f,", gyro_data[0]);
	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
	sprintf(ble_buffer, "%f,", gyro_data[1]);
	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
	sprintf(ble_buffer, "%f\r\n", gyro_data[2]);
	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
}

void magn_output_mode(void){
	char ble_buffer[50] = { 0 };

	// Obtain magn data
	ICM_ReadMag();

	//send data over UART
	printf("(Mx: %f | My: %f | Mz: %f)\r\n", magn_data[0], magn_data[1], magn_data[2]);
	//send data over BLE
  	sprintf(ble_buffer, "%f,", magn_data[0]);
	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
	sprintf(ble_buffer, "%f,", magn_data[1]);
	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
	sprintf(ble_buffer, "%f\r\n", magn_data[2]);
	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
}

void temp_output_mode(void){
	char ble_buffer[25] = { 0 };

	// Obtain accelerometer and gyro data
	ICM_ReadTemp();

	//send data over UART
	printf("Temp: %f \r\n", temp_data);
	//send data over BLE
	sprintf(ble_buffer, "%f\r\n", temp_data);
  	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
}

void dsp_output_mode(void){
	/*ispisuje podatke samo na UART - sluzi za provjeru ispravnosti DSP algoritma (IIR filter + FFT) na setu testnih podataka*/
	IIR_PROCESSING_F32Process(TEST_MODE);
	FFT_PROCESSING_F32Process(TEST_MODE);
}
void bpm_output_mode(void){
	char ble_buffer[15] = { 0 };
	float bpm = 0;
	bpm = breathing_frequency_output(BPM_OUTPUT_MODE);
	if (bpm != 0)  //znaci da se bpm stvarno izracunao u prosloj funkciji
	{
		printf("%f\r\n", bpm);
		sprintf(ble_buffer, "%f\r\n", bpm);
	  	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
	}
}
void hr_ecg_output_mode(void){
	int data=0;
	int predznak=1;
	double ecg_voltage=0;
	int intb=3;
	int etag=55;
	int return_value = 0;
	char ble_buffer[20] = { 0 };
	double hr=0;
	double r2r_time=0;

	//zovem max30001 interrupt handler -> cita podatke sa buffera i zapisuje u ECG_FIFO_buffer
	return_value = max30001_int_handler();

	if (return_value == -1)
	{
		printf("int_handler failed\r\n");
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

			//određivanje predznaka
			if(data & 0x20000)  //gledam 18. bit
			{
				predznak=-1;
				data=(data^0x3ffff)+1;
			}
			else
				predznak=1;

			//racunanje ecg napona -> GAIN=20!
			ecg_voltage=(double)(predznak)*(data*1000)/(pow(2,17)*20); //Vref=1000mV, ecg gain=20V/V

			//slanje podatka na UART i preko BLE
			printf("%.3f\r\n",ecg_voltage);
			sprintf(ble_buffer, "%.3f\r\n",ecg_voltage);
			CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
		}
	}
	else if(return_value == 1)  //int je izazvan zbog rrint, a ne eint ili eovf
	{
		r2r_time=(double)hspValMax30001.R2R * 0.0078125;
		hr=60/r2r_time;
		//slanje podatka na UART i preko BLE
		printf("%f\r\n",hr);
	  	sprintf(ble_buffer, "HR = %f\r\n",hr);
	  	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
	}
	else /*do nothing*/ ;
}

void bpm60_bioz_output_mode(void){
	char ble_buffer[15] = { 0 };
	float bpm = 0;
	bpm = breathing_frequency_output_V2(BPM60_BIOZ_OUTPUT_MODE);
	if (bpm != 0)  //znaci da se bpm stvarno izracunao u prosloj funkciji
	{
		sprintf(ble_buffer, "600\r\n");                                  //dodano kako bi se kasnije u analizi u Matlabu lako mogao odrediti zadnji uzorak u određenom ciklusu
	  	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
		printf("BPM = %f\r\n", bpm);
		sprintf(ble_buffer, "BPM = %f\r\n", bpm);
	  	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
	}
}
void bpm30_bioz_output_mode(void){
	char ble_buffer[15] = { 0 };
	float bpm = 0;
	bpm = breathing_frequency_output_V2(BPM30_BIOZ_OUTPUT_MODE);
	if (bpm != 0)  //znaci da se bpm stvarno izracunao u prosloj funkciji
	{
		sprintf(ble_buffer, "600\r\n");                                   //dodano kako bi se kasnije u analizi u Matlabu lako mogao odrediti zadnji uzorak u određenom ciklusu
	  	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
		printf("BPM = %f\r\n", bpm);
		sprintf(ble_buffer, "BPM = %f\r\n", bpm);
	  	CRSAPP_Update_Char(CRS_RX_CHAR_UUID, (uint8_t *)&ble_buffer[0]);
	}
}
