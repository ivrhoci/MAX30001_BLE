#include "main.h"
#include "ICM20948.h"
#include <string.h>

//default sensitivity scale factors
float accel_sens_scale_factor=4096;
float gyro_sens_scale_factor=131;
float magn_sens_scale_factor=0.15;
float temp_sens_scale_factor=333.87;
float room_temp_offset = 20;
/*
 *
 * SPI abstraction
 *
 */
void ICM_readBytes(uint8_t reg, uint8_t *pData, uint16_t Size) // ***
{
	reg = reg | 0x80;
	ICM_CSLow();
	HAL_SPI_Transmit_DMA(SPI_BUS, &reg, 1);
	while (HAL_SPI_GetState(SPI_BUS) != HAL_SPI_STATE_READY) //dodano
			;
	HAL_SPI_Receive_DMA(SPI_BUS, pData, Size);
	while (HAL_SPI_GetState(SPI_BUS) != HAL_SPI_STATE_READY) //dodano
			;
	ICM_CSHigh();
}

void ICM_WriteBytes(uint8_t reg, uint8_t *pData, uint16_t Size) // ***
{
	reg = reg & 0x7F;
	ICM_CSLow();
	HAL_SPI_Transmit_DMA(SPI_BUS, &reg, 1);
	HAL_SPI_Transmit_DMA(SPI_BUS, pData, Size);
	ICM_CSHigh();

}

void ICM_ReadOneByte(uint8_t reg, uint8_t* pData) // ***
{
	reg = reg | 0x80;
	ICM_CSLow();
	HAL_SPI_Transmit_DMA(SPI_BUS, &reg, 1);
	while (HAL_SPI_GetState(SPI_BUS) != HAL_SPI_STATE_READY)
		;
	HAL_SPI_Receive_DMA(SPI_BUS, pData, 1);
	while (HAL_SPI_GetState(SPI_BUS) != HAL_SPI_STATE_READY)
		;
	ICM_CSHigh();
}

void ICM_WriteOneByte(uint8_t reg, uint8_t Data) // ***
{
	reg = reg & 0x7F;
	ICM_CSLow();
	HAL_SPI_Transmit_DMA(SPI_BUS, &reg, 1);
	HAL_SPI_Transmit_DMA(SPI_BUS, &Data, 1);
	ICM_CSHigh();
}

/*
 *
 * AUX I2C abstraction for magnetometer
 *
 */
void ICM_I2C_Slave_Mag_Write(uint8_t reg,uint8_t value)
  {
  	ICM_WriteOneByte(0x7F, 0x30);

  	HAL_Delay(1);
  	ICM_WriteOneByte(0x03 ,0x0C);//mode: write

  	HAL_Delay(1);
  	ICM_WriteOneByte(0x04 ,reg);//set reg addr

  	HAL_Delay(1);
  	ICM_WriteOneByte(0x06 ,value);//send value

  	HAL_Delay(1);
  }

  static uint8_t ICM_I2C_Slave_Mag_Read(uint8_t reg)
  {
  	uint8_t  Data;
  	ICM_WriteOneByte(0x7F, 0x30);  //Select USER BANK 3
    HAL_Delay(1);
  	ICM_WriteOneByte(0x03 ,0x0C|0x80); //I2C_SLV0_ADDR register, 0x0C is the addres and 0x80 indicates it's a read transfer
    HAL_Delay(1);
  	ICM_WriteOneByte(0x04 ,reg);// set I2C slave 0 register address from where to begin data transfer
    HAL_Delay(1);
  	ICM_WriteOneByte(0x06 ,0xff);//read (Data out when slave 0 is set to write)??
  	HAL_Delay(1);
  	ICM_WriteOneByte(0x7F, 0x00); // Select USER BANK 0
  	ICM_ReadOneByte(0x3B,&Data); // Sensor data read from external I2C devices via the I2C master interface
    HAL_Delay(1);
  	return Data;
  }

/*
 *
 * Read magnetometer
 *
 */
void ICM_ReadMag() {
	  uint8_t byte[7]= { 0 };
	  int16_t raw_data[3] = { 0 };

	   byte[6] = ICM_I2C_Slave_Mag_Read(0x01); // treba??, na adresi 0x01 je samo device ID

	   byte[0] = ICM_I2C_Slave_Mag_Read(0x11); // X-axis measurement data lower 8bit
	   byte[1] = ICM_I2C_Slave_Mag_Read(0x12); // X-axis measurement data higher 8bit
	   raw_data[0] = (byte[1]<<8) | byte[0]; // X-axis merged measurement data

	   byte[2] = ICM_I2C_Slave_Mag_Read(0x13); // Y-axis measurement data higher 8bit
	   byte[3] = ICM_I2C_Slave_Mag_Read(0x14); // Y-axis measurement data higher 8bit
	   raw_data[1] = (byte[3]<<8) | byte[2]; // Y-axis merged measurement data

	   byte[4] = ICM_I2C_Slave_Mag_Read(0x15); // Z-axis measurement data higher 8bit
	   byte[5] = ICM_I2C_Slave_Mag_Read(0x16); // Z-axis measurement data higher 8bit
	   raw_data[2] = (byte[5]<<8) | byte[4]; // Z-axis merged measurement data

//	   for(int i=0;i<6;i++)
//	   	printf("byte[%i] nakon citanja %u\r\n",i,byte[i]);
//	   for(int i=0;i<3;i++)
//		printf("raw_data[%i] nakon citanja %i\r\n",i,raw_data[i]);

	   magn_data[0] = raw_data[0] * magn_sens_scale_factor; // X-axis scaled measurement data
	   magn_data[1] = raw_data[1] * magn_sens_scale_factor; // Y-axis scaled measurement data
	   magn_data[2] = raw_data[2] * magn_sens_scale_factor; // Z-axis scaled measurement data

	   ICM_I2C_Slave_Mag_Write(0x31,0x01); //single measurement mode?
}

/*
 *
 * Sequence to setup ICM290948 as early as possible after power on
 *
 */
void ICM_PowerOn(void) {
	char uart_buffer[200];
	uint8_t whoami = 0xEA;
	uint8_t test = ICM_WHOAMI();
	if (test == whoami) {
		printf("Completed WHO_AM_I, %i is 0xEA\r\n",test);
		ICM_CSHigh();
		HAL_Delay(10);
		ICM_SelectBank(USER_BANK_0);
		HAL_Delay(10);
		ICM_Disable_I2C();                     //Reset I2C Slave module and put the serial interface in SPI mode only.
		HAL_Delay(10);
		ICM_SetClock((uint8_t)CLK_BEST_AVAIL); // Auto selects the best available clock source – PLL if ready, else use the Internal oscillator
		HAL_Delay(10);
		ICM_AccelGyroOff();                   //why?
		HAL_Delay(20);
		ICM_AccelGyroOn();
		HAL_Delay(10);
		ICM_Initialize();

		//configure FIFO_EN_2 register
//		ICM_WriteOneByte(0x67, 0x1F);         //write accel, gyro and temp data to FIFO
	} else {
		printf("Failed WHO_AM_I, %i is not 0xEA\r\n",test);
		HAL_Delay(100);
	}
}

uint16_t ICM_Initialize(void) {
		ICM_SelectBank(USER_BANK_2);
		HAL_Delay(20);
		ICM_SetGyro_Scale_and_LPF(GYRO_SCALE_250, GYRO_LPF_17HZ);
		HAL_Delay(10);

		// Set gyroscope sample rate to 100hz (0x0A) in GYRO_SMPLRT_DIV register (0x00)
		ICM_WriteOneByte(0x00, 0x0A);
		HAL_Delay(10);

		// Set accelerometer low pass filter to 136hz (0x11) and the scale to +/- 8G (0x04) in register ACCEL_CONFIG (0x14)
		ICM_WriteOneByte(0x14, (0x04 | 0x11));

		// Set accelerometer sample rate register (11:8) to 0000 in ACCEL_SMPLRT_DIV_1 register (0x10)
		ICM_WriteOneByte(0x10, 0x00);
		HAL_Delay(10);

		// Set accelerometer sample rate to 102 hz (0x0A) in ACCEL_SMPLRT_DIV_2 register (0x11)
		ICM_WriteOneByte(0x11, 0x0A);
		HAL_Delay(10);

		ICM_SelectBank(USER_BANK_2);
		HAL_Delay(20);

		// Configure AUX_I2C Magnetometer (onboard ICM-20948)
		ICM_SelectBank(USER_BANK_0); // Select user bank 0
		ICM_WriteOneByte(0x0F, 0x30); // INT Pin / Bypass Enable Configuration
		ICM_WriteOneByte(0x03, 0x20); // I2C_MST_EN
		ICM_SelectBank(USER_BANK_3); // Select user bank 3
		ICM_WriteOneByte(0x01, 0x4D); // I2C Master mode and Speed 400 kHz ???
		ICM_WriteOneByte(0x02, 0x01); // I2C_SLV0_DELAY_EN
		ICM_WriteOneByte(0x05, 0x81); // I2C_SLV0_CTRL - Enable reading data from this slave and Number of bytes to be read=1

		// Initialize magnetometer
		ICM_I2C_Slave_Mag_Write(0x32, 0x01); // Reset AK8963
		HAL_Delay(1000);
		ICM_I2C_Slave_Mag_Write(0x31, 0x02); // use i2c to set AK8963 working on Continuous measurement mode1 & 16-bit output

		return 1337;
	}
/*
 *
 * Read Accelerometer and Gyroscope data
 *
 */
void ICM_ReadAccelGyro(void) {
	uint8_t byte[12]= { 0 };
	int16_t raw_data[6] = { 0 };

	ICM_SelectBank(USER_BANK_0);

	ICM_readBytes(0x2D, byte, 12);  //read 12 bytes (2 + 2 + 2 for accel and 2 + 2 + 2  for gyro)

//	for(int i=0;i<12;i++)
//	printf("byte[%i] nakon citanja %u\r\n",i,byte[i]);

//accel raw data
	raw_data[0] = (byte[0] << 8) | byte[1]; // X-axis merged measurement data
	raw_data[1] = (byte[2] << 8) | byte[3]; // Y-axis merged measurement data
	raw_data[2] = (byte[4] << 8) | byte[5]; // Z-axis merged measurement data
//gyro raw data
	raw_data[3] = (byte[6] << 8) | byte[7]; // X-axis merged measurement data
	raw_data[4] = (byte[8] << 8) | byte[9]; // Y-axis merged measurement data
	raw_data[5] = (byte[10] << 8) | byte[11]; // Z-axis merged measurement data

//	for(int i=0;i<6;i++)
//	printf("raw_data[%i] nakon citanja %i\r\n",i,raw_data[i]);

//accel float and scaled data
	accel_data[0] = raw_data[0] / accel_sens_scale_factor; // X-axis scaled measurement data (scaled in g values)
	accel_data[1] = raw_data[1] / accel_sens_scale_factor; // Y-axis scaled measurement data (scaled in g values)
	accel_data[2] = raw_data[2] / accel_sens_scale_factor; // Z-axis scaled measurement data (scaled in g values)
//gyro float and scaled data
	gyro_data[0] = raw_data[3] / gyro_sens_scale_factor; // X-axis scaled measurement data (scaled in dps (degrees per second) values)
	gyro_data[1] = raw_data[4] / gyro_sens_scale_factor; // Y-axis scaled measurement data (scaled in dps values)
	gyro_data[2] = raw_data[5] / gyro_sens_scale_factor; // Z-axis scaled measurement data (scaled in dps values)
}
/*
 *
 * Read Temperature sensor data
 *
 */
void ICM_ReadTemp(void) {
	uint8_t byte[2]= { 0 };
	int16_t raw_data =  0;

	//postavit user bank na 0?
	ICM_SelectBank(USER_BANK_0);

	ICM_readBytes(0x39, byte, 2);

//	for(int i=0;i<2;i++)
//	printf("byte[%i] nakon citanja %u\r\n",i,byte[i]);

//temp raw data
	raw_data = (byte[0] << 8) | byte[1];

//	printf("raw_data nakon citanja %i\r\n",raw_data);

//temp float and scaled data
	temp_data = ((raw_data - room_temp_offset)/ temp_sens_scale_factor) + 21; //scaled in degrees celsius

}
/*
 *
 * Auxiliary functions
 *
 */
void ICM_SelectBank(uint8_t bank) {
	ICM_WriteOneByte(USER_BANK_SEL, bank);
}
void ICM_Disable_I2C(void) {
	ICM_WriteOneByte(0x03, 0x78);
}
void ICM_CSHigh(void) {
	HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, SET);
}
void ICM_CSLow(void) {
	HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, RESET);
}
void ICM_SetClock(uint8_t clk) {
	ICM_WriteOneByte(PWR_MGMT_1, clk);
}
void ICM_AccelGyroOff(void) {
	ICM_WriteOneByte(PWR_MGMT_2, (0x38 | 0x07));
}
void ICM_AccelGyroOn(void) {
	ICM_WriteOneByte(0x07, (0x00 | 0x00));
}
uint8_t ICM_WHOAMI(void) {
	uint8_t spiData = 0x01;
	ICM_ReadOneByte(0x00, &spiData);
	return spiData;
}
void ICM_SetGyro_Scale_and_LPF(uint8_t scale, uint8_t lpf) {
	ICM_WriteOneByte(GYRO_CONFIG_1, (scale|lpf));
}
/*
 *
 * Read FIFO - work in progress
 *
 */
void ICM_ReadFIFO(void) {
	uint8_t raw_data[14]={ 0 };

	for(int i=0;i<14;i++)
	printf("raw_data[%i] prije citanja %u\r\n",i,raw_data[i]);

	ICM_readBytes(0x72, raw_data, 14);

	for(int i=0;i<14;i++)
	printf("raw_data[%i] nakon citanja %u\r\n",i,raw_data[i]);

	accel_data[0] = (raw_data[0] << 8) | raw_data[1];
	accel_data[1] = (raw_data[2] << 8) | raw_data[3];
	accel_data[2] = (raw_data[4] << 8) | raw_data[5];

	gyro_data[0] = (raw_data[6] << 8) | raw_data[7];
	gyro_data[1] = (raw_data[8] << 8) | raw_data[9];
	gyro_data[2] = (raw_data[10] << 8) | raw_data[11];

	temp_data = (raw_data[12] << 8) | raw_data[13];

}
/*
 *
 * Read a register - work in progress
 *
 */
int icm20948_reg_read(uint8_t reg, uint16_t *return_data) {

  uint8_t result[3];
  uint8_t data_array[1];
  int32_t success = 99;

  data_array[0] = reg | 0x80; // For Read, Or with 0x80


	HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, GPIO_PIN_RESET); //CS pin low
	success = HAL_SPI_TransmitReceive_DMA(&hspi1, &data_array[0], &result[0], 3);
	HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, GPIO_PIN_SET); //CS pin high

  *return_data = (uint16_t)(result[1] << 8) + result[2];
  printf("reg_read_success = %d\n", success);
	if (success != 0) {
    return -1;
  } else {
    return 0;
  }
}
/*
 *
 * USB init - work in progress
 *
 */
void icm_usb_init(char *USB_command, uint32_t regvalue){
	char buffer[100]="";
	uint8_t icm_regvalue_new = (uint8_t) regvalue;
	uint8_t icm_regvalue_old = 0;
//*******************************USER BANK 0 register***************************************
	//*******************************USER_CTRL***************************************
	if (strncmp(USB_command, "ICM_USER_CTRL", 13) == 0){

		ICM_SelectBank(USER_BANK_0);

			sprintf(buffer, "Received USER_CTRL register write command = %.6s\r\n", USB_parameter);
			printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
			CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

			//Read old register value
			ICM_ReadOneByte(USER_CTRL,&icm_regvalue_old);
			sprintf(buffer, "Old USER_CTR register value = %X\r\n", icm_regvalue_old);
			printf("%s\n", buffer);                             //debug ispis na UART COM port
			CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

			//Write new register value
			ICM_WriteOneByte(USER_CTRL,icm_regvalue_new);

			//Read new register value
			ICM_ReadOneByte(USER_CTRL,&icm_regvalue_old);
			sprintf(buffer, "New USER_CTR register value = %X\r\n", icm_regvalue_old);
			printf("%s\n", buffer);                             //debug ispis na UART COM port
			CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
		}
	//*******************************  LP_CONFIG  ***************************************
	if (strncmp(USB_command, "ICM_LP_CONFIG", 13) == 0){

			ICM_SelectBank(USER_BANK_0);

				sprintf(buffer, "Received LP_CONFIG register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(LP_CONFIG,&icm_regvalue_old);
				sprintf(buffer, "Old LP_CONFIG register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(LP_CONFIG,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(LP_CONFIG,&icm_regvalue_old);
				sprintf(buffer, "New LP_CONFIG register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  PWR_MGMT_1  ***************************************
	if (strncmp(USB_command, "ICM_PWR_MGMT_1", 14) == 0){

			ICM_SelectBank(USER_BANK_0);

				sprintf(buffer, "Received PWR_MGMT_1 register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(PWR_MGMT_1,&icm_regvalue_old);
				sprintf(buffer, "Old PWR_MGMT_1 register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(PWR_MGMT_1,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(PWR_MGMT_1,&icm_regvalue_old);
				sprintf(buffer, "New PWR_MGMT_1 register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  PWR_MGMT_2  ***************************************
	if (strncmp(USB_command, "ICM_PWR_MGMT_2", 14) == 0){

			ICM_SelectBank(USER_BANK_0);

				sprintf(buffer, "Received PWR_MGMT_2 register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(PWR_MGMT_2,&icm_regvalue_old);
				sprintf(buffer, "Old PWR_MGMT_2 register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(PWR_MGMT_2,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(PWR_MGMT_2,&icm_regvalue_old);
				sprintf(buffer, "New PWR_MGMT_2 register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  INT_PIN_CFG  ***************************************
	if (strncmp(USB_command, "ICM_INT_PIN_CFG", 15) == 0){

			ICM_SelectBank(USER_BANK_0);

				sprintf(buffer, "Received INT_PIN_CFG register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(INT_PIN_CFG,&icm_regvalue_old);
				sprintf(buffer, "Old INT_PIN_CFG register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(INT_PIN_CFG,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(INT_PIN_CFG,&icm_regvalue_old);
				sprintf(buffer, "New INT_PIN_CFG register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  INT_ENABLE  ***************************************
	if (strncmp(USB_command, "ICM_INT_ENABLE_0", 16) == 0){

			ICM_SelectBank(USER_BANK_0);

				sprintf(buffer, "Received INT_ENABLE register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(INT_ENABLE,&icm_regvalue_old);
				sprintf(buffer, "Old INT_ENABLE register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(INT_ENABLE,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(INT_ENABLE,&icm_regvalue_old);
				sprintf(buffer, "New INT_ENABLE register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  INT_ENABLE  ***************************************
	if (strncmp(USB_command, "ICM_INT_ENABLE_1", 16) == 0){

			ICM_SelectBank(USER_BANK_0);

				sprintf(buffer, "Received INT_ENABLE_1 register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(INT_ENABLE_1,&icm_regvalue_old);
				sprintf(buffer, "Old INT_ENABLE_1 register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(INT_ENABLE_1,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(INT_ENABLE_1,&icm_regvalue_old);
				sprintf(buffer, "New INT_ENABLE_1 register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  INT_ENABLE_2  ***************************************
	if (strncmp(USB_command, "ICM_INT_ENABLE_2", 16) == 0){

			ICM_SelectBank(USER_BANK_0);

				sprintf(buffer, "Received INT_ENABLE_2 register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(INT_ENABLE_2,&icm_regvalue_old);
				sprintf(buffer, "Old INT_ENABLE_2 register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(INT_ENABLE_2,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(INT_ENABLE_2,&icm_regvalue_old);
				sprintf(buffer, "New INT_ENABLE_2 register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  INT_ENABLE_3  ***************************************
	if (strncmp(USB_command, "ICM_INT_ENABLE_3", 16) == 0){

			ICM_SelectBank(USER_BANK_0);

				sprintf(buffer, "Received INT_ENABLE_3 register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(INT_ENABLE_3,&icm_regvalue_old);
				sprintf(buffer, "Old INT_ENABLE_3 register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(INT_ENABLE_3,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(INT_ENABLE_3,&icm_regvalue_old);
				sprintf(buffer, "New INT_ENABLE_3 register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  FIFO_EN_1  ***************************************
	if (strncmp(USB_command, "ICM_FIFO_EN_1", 13) == 0){

			ICM_SelectBank(USER_BANK_0);

				sprintf(buffer, "Received FIFO_EN_1 register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(FIFO_EN_1,&icm_regvalue_old);
				sprintf(buffer, "Old FIFO_EN_1 register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(FIFO_EN_1,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(FIFO_EN_1,&icm_regvalue_old);
				sprintf(buffer, "New FIFO_EN_1 register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  FIFO_EN_2  ***************************************
	if (strncmp(USB_command, "ICM_FIFO_EN_2", 13) == 0){

			ICM_SelectBank(USER_BANK_0);

				sprintf(buffer, "Received FIFO_EN_2 register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(FIFO_EN_2,&icm_regvalue_old);
				sprintf(buffer, "Old FIFO_EN_2 register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(FIFO_EN_2,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(FIFO_EN_2,&icm_regvalue_old);
				sprintf(buffer, "New FIFO_EN_2 register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  FIFO_RST  ***************************************
	if (strncmp(USB_command, "ICM_FIFO_RST", 12) == 0){

			ICM_SelectBank(USER_BANK_0);

				sprintf(buffer, "Received FIFO_RST register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(FIFO_RST,&icm_regvalue_old);
				sprintf(buffer, "Old FIFO_RST register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(FIFO_RST,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(FIFO_RST,&icm_regvalue_old);
				sprintf(buffer, "New FIFO_RST register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  FIFO_MODE  ***************************************
	if (strncmp(USB_command, "ICM_FIFO_MODE", 13) == 0){

			ICM_SelectBank(USER_BANK_0);

				sprintf(buffer, "Received FIFO_MODE register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(FIFO_MODE,&icm_regvalue_old);
				sprintf(buffer, "Old FIFO_MODE register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(FIFO_MODE,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(FIFO_MODE,&icm_regvalue_old);
				sprintf(buffer, "New FIFO_MODE register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  FIFO_R_W  ***************************************
	if (strncmp(USB_command, "ICM_FIFO_R_W", 12) == 0){

			ICM_SelectBank(USER_BANK_0);

				sprintf(buffer, "Received FIFO_R_W register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(FIFO_R_W,&icm_regvalue_old);
				sprintf(buffer, "Old FIFO_R_W register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(FIFO_R_W,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(FIFO_R_W,&icm_regvalue_old);
				sprintf(buffer, "New FIFO_R_W register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  FIFO_CFG  ***************************************
	if (strncmp(USB_command, "ICM_FIFO_CFG", 12) == 0){

			ICM_SelectBank(USER_BANK_0);

				sprintf(buffer, "Received FIFO_CFG register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(FIFO_CFG,&icm_regvalue_old);
				sprintf(buffer, "Old FIFO_CFG register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(FIFO_CFG,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(FIFO_CFG,&icm_regvalue_old);
				sprintf(buffer, "New FIFO_CFG register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
//*******************************USER BANK 1 register***************************************
	//*******************************  SELF_TEST_X_GYRO  ***************************************
	if (strncmp(USB_command, "ICM_SELF_TEST_X_GYRO", 20) == 0){

			ICM_SelectBank(USER_BANK_1);

				sprintf(buffer, "Received SELF_TEST_X_GYRO register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(SELF_TEST_X_GYRO,&icm_regvalue_old);
				sprintf(buffer, "Old SELF_TEST_X_GYRO register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(SELF_TEST_X_GYRO,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(SELF_TEST_X_GYRO,&icm_regvalue_old);
				sprintf(buffer, "New SELF_TEST_X_GYRO register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  SELF_TEST_Y_GYRO  ***************************************
	if (strncmp(USB_command, "ICM_SELF_TEST_Y_GYRO", 20) == 0){

			ICM_SelectBank(USER_BANK_1);

				sprintf(buffer, "Received SELF_TEST_Y_GYRO register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(SELF_TEST_Y_GYRO,&icm_regvalue_old);
				sprintf(buffer, "Old SELF_TEST_Y_GYRO register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(SELF_TEST_Y_GYRO,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(SELF_TEST_Y_GYRO,&icm_regvalue_old);
				sprintf(buffer, "New SELF_TEST_Y_GYRO register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  SELF_TEST_Z_GYRO  ***************************************
	if (strncmp(USB_command, "ICM_SELF_TEST_Z_GYRO", 20) == 0){

			ICM_SelectBank(USER_BANK_1);

				sprintf(buffer, "Received SELF_TEST_Z_GYRO register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(SELF_TEST_Z_GYRO,&icm_regvalue_old);
				sprintf(buffer, "Old SELF_TEST_Z_GYRO register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(SELF_TEST_Z_GYRO,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(SELF_TEST_Z_GYRO,&icm_regvalue_old);
				sprintf(buffer, "New SELF_TEST_Z_GYRO register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  SELF_TEST_X_ACCEL  ***************************************
	if (strncmp(USB_command, "ICM_SELF_TEST_X_ACCEL", 21) == 0){

			ICM_SelectBank(USER_BANK_1);

				sprintf(buffer, "Received SELF_TEST_X_ACCEL register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(SELF_TEST_X_ACCEL,&icm_regvalue_old);
				sprintf(buffer, "Old SELF_TEST_X_ACCEL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(SELF_TEST_X_ACCEL,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(SELF_TEST_X_ACCEL,&icm_regvalue_old);
				sprintf(buffer, "New SELF_TEST_X_ACCEL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  SELF_TEST_Y_ACCEL  ***************************************
	if (strncmp(USB_command, "ICM_SELF_TEST_Y_ACCEL", 21) == 0){

			ICM_SelectBank(USER_BANK_1);

				sprintf(buffer, "Received SELF_TEST_Y_ACCEL register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(SELF_TEST_Y_ACCEL,&icm_regvalue_old);
				sprintf(buffer, "Old SELF_TEST_Y_ACCEL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(SELF_TEST_Y_ACCEL,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(SELF_TEST_Y_ACCEL,&icm_regvalue_old);
				sprintf(buffer, "New SELF_TEST_Y_ACCEL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  SELF_TEST_Z_ACCEL  ***************************************
	if (strncmp(USB_command, "ICM_SELF_TEST_Z_ACCEL", 21) == 0){

			ICM_SelectBank(USER_BANK_1);

				sprintf(buffer, "Received SELF_TEST_Z_ACCEL register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(SELF_TEST_Z_ACCEL,&icm_regvalue_old);
				sprintf(buffer, "Old SELF_TEST_Z_ACCEL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(SELF_TEST_Z_ACCEL,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(SELF_TEST_Z_ACCEL,&icm_regvalue_old);
				sprintf(buffer, "New SELF_TEST_Z_ACCEL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  XA_OFFS_H  ***************************************
	if (strncmp(USB_command, "ICM_XA_OFFS_H", 13) == 0){

			ICM_SelectBank(USER_BANK_1);

				sprintf(buffer, "Received XA_OFFS_H register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(XA_OFFS_H,&icm_regvalue_old);
				sprintf(buffer, "Old XA_OFFS_H register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(XA_OFFS_H,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(XA_OFFS_H,&icm_regvalue_old);
				sprintf(buffer, "New XA_OFFS_H register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  XA_OFFS_L  ***************************************
	if (strncmp(USB_command, "ICM_XA_OFFS_L", 13) == 0){

			ICM_SelectBank(USER_BANK_1);

				sprintf(buffer, "Received XA_OFFS_L register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(XA_OFFS_L,&icm_regvalue_old);
				sprintf(buffer, "Old XA_OFFS_L register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(XA_OFFS_L,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(XA_OFFS_L,&icm_regvalue_old);
				sprintf(buffer, "New XA_OFFS_L register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  YA_OFFS_H  ***************************************
	if (strncmp(USB_command, "ICM_YA_OFFS_H", 13) == 0){

			ICM_SelectBank(USER_BANK_1);

				sprintf(buffer, "Received YA_OFFS_H register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(YA_OFFS_H,&icm_regvalue_old);
				sprintf(buffer, "Old YA_OFFS_H register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(YA_OFFS_H,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(YA_OFFS_H,&icm_regvalue_old);
				sprintf(buffer, "New YA_OFFS_H register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  YA_OFFS_L  ***************************************
	if (strncmp(USB_command, "ICM_YA_OFFS_L", 13) == 0){

			ICM_SelectBank(USER_BANK_1);

				sprintf(buffer, "Received YA_OFFS_L register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(YA_OFFS_L,&icm_regvalue_old);
				sprintf(buffer, "Old YA_OFFS_L register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(YA_OFFS_L,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(YA_OFFS_L,&icm_regvalue_old);
				sprintf(buffer, "New YA_OFFS_L register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  ZA_OFFS_H  ***************************************
	if (strncmp(USB_command, "ICM_ZA_OFFS_H", 13) == 0){

			ICM_SelectBank(USER_BANK_1);

				sprintf(buffer, "Received ZA_OFFS_H register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(ZA_OFFS_H,&icm_regvalue_old);
				sprintf(buffer, "Old ZA_OFFS_H register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(ZA_OFFS_H,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(ZA_OFFS_H,&icm_regvalue_old);
				sprintf(buffer, "New ZA_OFFS_H register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  ZA_OFFS_L  ***************************************
	if (strncmp(USB_command, "ICM_ZA_OFFS_L", 13) == 0){

			ICM_SelectBank(USER_BANK_1);

				sprintf(buffer, "Received ZA_OFFS_L register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(ZA_OFFS_L,&icm_regvalue_old);
				sprintf(buffer, "Old ZA_OFFS_L register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(ZA_OFFS_L,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(ZA_OFFS_L,&icm_regvalue_old);
				sprintf(buffer, "New ZA_OFFS_L register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  TIMEBASE_CORRECTION_PLL  ***************************************
	if (strncmp(USB_command, "ICM_TIMEBASE_CORRECTION_PLL", 27) == 0){

			ICM_SelectBank(USER_BANK_1);

				sprintf(buffer, "Received TIMEBASE_CORRECTION_PLL register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(TIMEBASE_CORRECTION_PLL,&icm_regvalue_old);
				sprintf(buffer, "Old TIMEBASE_CORRECTION_PLL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(TIMEBASE_CORRECTION_PLL,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(TIMEBASE_CORRECTION_PLL,&icm_regvalue_old);
				sprintf(buffer, "New TIMEBASE_CORRECTION_PLL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
//*******************************USER BANK 2 register***************************************
	//*******************************  GYRO_SMPLRT_DIV  ***************************************
	if (strncmp(USB_command, "ICM_GYRO_SMPLRT_DIV", 19) == 0){

			ICM_SelectBank(USER_BANK_2);

				sprintf(buffer, "Received GYRO_SMPLRT_DIV register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(GYRO_SMPLRT_DIV,&icm_regvalue_old);
				sprintf(buffer, "Old GYRO_SMPLRT_DIV register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(GYRO_SMPLRT_DIV,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(GYRO_SMPLRT_DIV,&icm_regvalue_old);
				sprintf(buffer, "New GYRO_SMPLRT_DIV register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  GYRO_CONFIG_1  ***************************************
	if (strncmp(USB_command, "ICM_GYRO_CONFIG_1", 17) == 0){

			ICM_SelectBank(USER_BANK_2);

				sprintf(buffer, "Received GYRO_CONFIG_1 register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(GYRO_CONFIG_1,&icm_regvalue_old);
				sprintf(buffer, "Old GYRO_CONFIG_1 register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(GYRO_CONFIG_1,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(GYRO_CONFIG_1,&icm_regvalue_old);
				sprintf(buffer, "New GYRO_CONFIG_1 register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  GYRO_CONFIG_2  ***************************************
	if (strncmp(USB_command, "ICM_GYRO_CONFIG_2", 17) == 0){

			ICM_SelectBank(USER_BANK_2);

				sprintf(buffer, "Received GYRO_CONFIG_2 register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(GYRO_CONFIG_2,&icm_regvalue_old);
				sprintf(buffer, "Old GYRO_CONFIG_2 register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(GYRO_CONFIG_2,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(GYRO_CONFIG_2,&icm_regvalue_old);
				sprintf(buffer, "New GYRO_CONFIG_2 register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  XG_OFFS_USRH  ***************************************
	if (strncmp(USB_command, "ICM_XG_OFFS_USRH", 16) == 0){

			ICM_SelectBank(USER_BANK_2);

				sprintf(buffer, "Received XG_OFFS_USRH register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(XG_OFFS_USRH,&icm_regvalue_old);
				sprintf(buffer, "Old XG_OFFS_USRH register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(XG_OFFS_USRH,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(XG_OFFS_USRH,&icm_regvalue_old);
				sprintf(buffer, "New XG_OFFS_USRH register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  XG_OFFS_USRL  ***************************************
	if (strncmp(USB_command, "ICM_XG_OFFS_USRL", 16) == 0){

			ICM_SelectBank(USER_BANK_2);

				sprintf(buffer, "Received XG_OFFS_USRL register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(XG_OFFS_USRL,&icm_regvalue_old);
				sprintf(buffer, "Old XG_OFFS_USRL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(XG_OFFS_USRL,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(XG_OFFS_USRL,&icm_regvalue_old);
				sprintf(buffer, "New XG_OFFS_USRL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  YG_OFFS_USRH  ***************************************
	if (strncmp(USB_command, "ICM_YG_OFFS_USRH", 16) == 0){

			ICM_SelectBank(USER_BANK_2);

				sprintf(buffer, "Received YG_OFFS_USRH register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(YG_OFFS_USRH,&icm_regvalue_old);
				sprintf(buffer, "Old YG_OFFS_USRH register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(YG_OFFS_USRH,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(YG_OFFS_USRH,&icm_regvalue_old);
				sprintf(buffer, "New YG_OFFS_USRH register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  YG_OFFS_USRL  ***************************************
	if (strncmp(USB_command, "ICM_YG_OFFS_USRL", 16) == 0){

			ICM_SelectBank(USER_BANK_2);

				sprintf(buffer, "Received YG_OFFS_USRL register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(YG_OFFS_USRL,&icm_regvalue_old);
				sprintf(buffer, "Old YG_OFFS_USRL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(YG_OFFS_USRL,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(YG_OFFS_USRL,&icm_regvalue_old);
				sprintf(buffer, "New YG_OFFS_USRL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  ZG_OFFS_USRH  ***************************************
	if (strncmp(USB_command, "ICM_ZG_OFFS_USRH", 16) == 0){

			ICM_SelectBank(USER_BANK_2);

				sprintf(buffer, "Received ZG_OFFS_USRH register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(ZG_OFFS_USRH,&icm_regvalue_old);
				sprintf(buffer, "Old ZG_OFFS_USRH register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(ZG_OFFS_USRH,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(ZG_OFFS_USRH,&icm_regvalue_old);
				sprintf(buffer, "New ZG_OFFS_USRH register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  ZG_OFFS_USRL  ***************************************
	if (strncmp(USB_command, "ICM_ZG_OFFS_USRL", 16) == 0){

			ICM_SelectBank(USER_BANK_2);

				sprintf(buffer, "Received ZG_OFFS_USRL register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(ZG_OFFS_USRL,&icm_regvalue_old);
				sprintf(buffer, "Old ZG_OFFS_USRL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(ZG_OFFS_USRL,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(ZG_OFFS_USRL,&icm_regvalue_old);
				sprintf(buffer, "New ZG_OFFS_USRL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  ODR_ALIGN_EN  ***************************************
	if (strncmp(USB_command, "ICM_ODR_ALIGN_EN", 16) == 0){

			ICM_SelectBank(USER_BANK_2);

				sprintf(buffer, "Received ODR_ALIGN_EN register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(ODR_ALIGN_EN,&icm_regvalue_old);
				sprintf(buffer, "Old ODR_ALIGN_EN register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(ODR_ALIGN_EN,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(ODR_ALIGN_EN,&icm_regvalue_old);
				sprintf(buffer, "New ODR_ALIGN_EN register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  ACCEL_SMPLRT_DIV_1  ***************************************
	if (strncmp(USB_command, "ICM_ACCEL_SMPLRT_DIV_1", 22) == 0){

			ICM_SelectBank(USER_BANK_2);

				sprintf(buffer, "Received ACCEL_SMPLRT_DIV_1 register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(ACCEL_SMPLRT_DIV_1,&icm_regvalue_old);
				sprintf(buffer, "Old ACCEL_SMPLRT_DIV_1 register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(ACCEL_SMPLRT_DIV_1,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(ACCEL_SMPLRT_DIV_1,&icm_regvalue_old);
				sprintf(buffer, "New ACCEL_SMPLRT_DIV_1 register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  ACCEL_SMPLRT_DIV_2  ***************************************
	if (strncmp(USB_command, "ICM_ACCEL_SMPLRT_DIV_2", 22) == 0){

			ICM_SelectBank(USER_BANK_2);

				sprintf(buffer, "Received ACCEL_SMPLRT_DIV_2 register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(ACCEL_SMPLRT_DIV_2,&icm_regvalue_old);
				sprintf(buffer, "Old ACCEL_SMPLRT_DIV_2 register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(ACCEL_SMPLRT_DIV_2,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(ACCEL_SMPLRT_DIV_2,&icm_regvalue_old);
				sprintf(buffer, "New ACCEL_SMPLRT_DIV_2 register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  ACCEL_INTEL_CTRL  ***************************************
	if (strncmp(USB_command, "ICM_ACCEL_INTEL_CTRL", 20) == 0){

			ICM_SelectBank(USER_BANK_2);

				sprintf(buffer, "Received ACCEL_INTEL_CTRL register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(ACCEL_INTEL_CTRL,&icm_regvalue_old);
				sprintf(buffer, "Old ACCEL_INTEL_CTRL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(ACCEL_INTEL_CTRL,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(ACCEL_INTEL_CTRL,&icm_regvalue_old);
				sprintf(buffer, "New ACCEL_INTEL_CTRL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  ACCEL_WOM_THR  ***************************************
	if (strncmp(USB_command, "ICM_ACCEL_WOM_THR", 17) == 0){

			ICM_SelectBank(USER_BANK_2);

				sprintf(buffer, "Received ACCEL_WOM_THR register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(ACCEL_WOM_THR,&icm_regvalue_old);
				sprintf(buffer, "Old ACCEL_WOM_THR register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(ACCEL_WOM_THR,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(ACCEL_WOM_THR,&icm_regvalue_old);
				sprintf(buffer, "New ACCEL_WOM_THR register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  ACCEL_CONFIG_1  ***************************************
	if (strncmp(USB_command, "ICM_ACCEL_CONFIG_1", 18) == 0){

			ICM_SelectBank(USER_BANK_2);

				sprintf(buffer, "Received ACCEL_CONFIG_1 register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(ACCEL_CONFIG_1,&icm_regvalue_old);
				sprintf(buffer, "Old ACCEL_CONFIG_1 register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(ACCEL_CONFIG_1,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(ACCEL_CONFIG_1,&icm_regvalue_old);
				sprintf(buffer, "New ACCEL_CONFIG_1 register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  ACCEL_CONFIG_2  ***************************************
	if (strncmp(USB_command, "ICM_ACCEL_CONFIG_2", 18) == 0){

			ICM_SelectBank(USER_BANK_2);

				sprintf(buffer, "Received ACCEL_CONFIG_2 register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(ACCEL_CONFIG_2,&icm_regvalue_old);
				sprintf(buffer, "Old ACCEL_CONFIG_2 register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(ACCEL_CONFIG_2,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(ACCEL_CONFIG_2,&icm_regvalue_old);
				sprintf(buffer, "New ACCEL_CONFIG_2 register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  FSYNC_CONFIG  ***************************************
	if (strncmp(USB_command, "ICM_FSYNC_CONFIG", 16) == 0){

			ICM_SelectBank(USER_BANK_2);

				sprintf(buffer, "Received FSYNC_CONFIG register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(FSYNC_CONFIG,&icm_regvalue_old);
				sprintf(buffer, "Old FSYNC_CONFIG register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(FSYNC_CONFIG,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(FSYNC_CONFIG,&icm_regvalue_old);
				sprintf(buffer, "New FSYNC_CONFIG register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  TEMP_CONFIG  ***************************************
	if (strncmp(USB_command, "ICM_TEMP_CONFIG", 15) == 0){

			ICM_SelectBank(USER_BANK_2);

				sprintf(buffer, "Received TEMP_CONFIG register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(TEMP_CONFIG,&icm_regvalue_old);
				sprintf(buffer, "Old TEMP_CONFIG register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(TEMP_CONFIG,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(TEMP_CONFIG,&icm_regvalue_old);
				sprintf(buffer, "New TEMP_CONFIG register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  MOD_CTRL_USR  ***************************************
	if (strncmp(USB_command, "ICM_MOD_CTRL_USR", 16) == 0){

			ICM_SelectBank(USER_BANK_2);

				sprintf(buffer, "Received MOD_CTRL_USR register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(MOD_CTRL_USR,&icm_regvalue_old);
				sprintf(buffer, "Old MOD_CTRL_USR register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(MOD_CTRL_USR,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(MOD_CTRL_USR,&icm_regvalue_old);
				sprintf(buffer, "New MOD_CTRL_USR register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
//*******************************USER BANK 3 register***************************************
	//*******************************  I2C_MST_ODR_CONFIG  ***************************************
	if (strncmp(USB_command, "ICM_I2C_MST_ODR_CONFIG", 22) == 0){

			ICM_SelectBank(USER_BANK_3);

				sprintf(buffer, "Received I2C_MST_ODR_CONFIG register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(I2C_MST_ODR_CONFIG,&icm_regvalue_old);
				sprintf(buffer, "Old I2C_MST_ODR_CONFIG register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(I2C_MST_ODR_CONFIG,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(I2C_MST_ODR_CONFIG,&icm_regvalue_old);
				sprintf(buffer, "New I2C_MST_ODR_CONFIG register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  I2C_MST_CTRL  ***************************************
	if (strncmp(USB_command, "ICM_I2C_MST_CTRL", 16) == 0){

			ICM_SelectBank(USER_BANK_3);

				sprintf(buffer, "Received I2C_MST_CTRL register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(I2C_MST_CTRL,&icm_regvalue_old);
				sprintf(buffer, "Old I2C_MST_CTRL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(I2C_MST_CTRL,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(I2C_MST_CTRL,&icm_regvalue_old);
				sprintf(buffer, "New I2C_MST_CTRL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  I2C_MST_DELAY_CTRL  ***************************************
	if (strncmp(USB_command, "ICM_I2C_MST_DELAY_CTRL", 22) == 0){

			ICM_SelectBank(USER_BANK_3);

				sprintf(buffer, "Received I2C_MST_DELAY_CTRL register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(I2C_MST_DELAY_CTRL,&icm_regvalue_old);
				sprintf(buffer, "Old I2C_MST_DELAY_CTRL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(I2C_MST_DELAY_CTRL,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(I2C_MST_DELAY_CTRL,&icm_regvalue_old);
				sprintf(buffer, "New I2C_MST_DELAY_CTRL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  I2C_SLV0_ADDR  ***************************************
	if (strncmp(USB_command, "ICM_I2C_SLV0_ADDR", 17) == 0){

			ICM_SelectBank(USER_BANK_3);

				sprintf(buffer, "Received I2C_SLV0_ADDR register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(I2C_SLV0_ADDR,&icm_regvalue_old);
				sprintf(buffer, "Old I2C_SLV0_ADDR register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(I2C_SLV0_ADDR,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(I2C_SLV0_ADDR,&icm_regvalue_old);
				sprintf(buffer, "New I2C_SLV0_ADDR register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  I2C_SLV0_REG  ***************************************
	if (strncmp(USB_command, "ICM_I2C_SLV0_REG", 16) == 0){

			ICM_SelectBank(USER_BANK_3);

				sprintf(buffer, "Received I2C_SLV0_REG register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(I2C_SLV0_REG,&icm_regvalue_old);
				sprintf(buffer, "Old I2C_SLV0_REG register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(I2C_SLV0_ADDR,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(I2C_SLV0_REG,&icm_regvalue_old);
				sprintf(buffer, "New I2C_SLV0_REG register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  I2C_SLV0_CTRL  ***************************************
	if (strncmp(USB_command, "ICM_I2C_SLV0_CTRL", 17) == 0){

			ICM_SelectBank(USER_BANK_3);

				sprintf(buffer, "Received I2C_SLV0_CTRL register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(I2C_SLV0_CTRL,&icm_regvalue_old);
				sprintf(buffer, "Old I2C_SLV0_CTRL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(I2C_SLV0_CTRL,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(I2C_SLV0_CTRL,&icm_regvalue_old);
				sprintf(buffer, "New I2C_SLV0_CTRL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  I2C_SLV0_DO  ***************************************
	if (strncmp(USB_command, "ICM_I2C_SLV0_DO", 15) == 0){

			ICM_SelectBank(USER_BANK_3);

				sprintf(buffer, "Received I2C_SLV0_DO register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(I2C_SLV0_DO,&icm_regvalue_old);
				sprintf(buffer, "Old I2C_SLV0_DO register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(I2C_SLV0_DO,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(I2C_SLV0_DO,&icm_regvalue_old);
				sprintf(buffer, "New I2C_SLV0_DO register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  I2C_SLV1_ADDR  ***************************************
	if (strncmp(USB_command, "ICM_I2C_SLV1_ADDR", 17) == 0){

			ICM_SelectBank(USER_BANK_3);

				sprintf(buffer, "Received I2C_SLV1_ADDR register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(I2C_SLV1_ADDR,&icm_regvalue_old);
				sprintf(buffer, "Old I2C_SLV1_ADDR register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(I2C_SLV1_ADDR,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(I2C_SLV1_ADDR,&icm_regvalue_old);
				sprintf(buffer, "New I2C_SLV1_ADDR register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  I2C_SLV1_REG  ***************************************
	if (strncmp(USB_command, "ICM_I2C_SLV1_REG", 16) == 0){

			ICM_SelectBank(USER_BANK_3);

				sprintf(buffer, "Received I2C_SLV1_REG register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(I2C_SLV1_REG,&icm_regvalue_old);
				sprintf(buffer, "Old I2C_SLV1_REG register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(I2C_SLV1_REG,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(I2C_SLV1_REG,&icm_regvalue_old);
				sprintf(buffer, "New I2C_SLV1_REG register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  I2C_SLV1_CTRL  ***************************************
	if (strncmp(USB_command, "ICM_I2C_SLV1_CTRL", 17) == 0){

			ICM_SelectBank(USER_BANK_3);

				sprintf(buffer, "Received I2C_SLV1_CTRL register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(I2C_SLV1_CTRL,&icm_regvalue_old);
				sprintf(buffer, "Old I2C_SLV1_CTRL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(I2C_SLV1_CTRL,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(I2C_SLV1_CTRL,&icm_regvalue_old);
				sprintf(buffer, "New I2C_SLV1_CTRL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  I2C_SLV1_DO  ***************************************
	if (strncmp(USB_command, "ICM_I2C_SLV1_DO", 15) == 0){

			ICM_SelectBank(USER_BANK_3);

				sprintf(buffer, "Received I2C_SLV1_DO register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(I2C_SLV1_DO,&icm_regvalue_old);
				sprintf(buffer, "Old I2C_SLV1_DO register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(I2C_SLV1_DO,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(I2C_SLV1_DO,&icm_regvalue_old);
				sprintf(buffer, "New I2C_SLV1_DO register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  I2C_SLV2_ADDR  ***************************************
	if (strncmp(USB_command, "ICM_I2C_SLV2_ADDR", 17) == 0){

			ICM_SelectBank(USER_BANK_3);

				sprintf(buffer, "Received I2C_SLV2_ADDR register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(I2C_SLV2_ADDR,&icm_regvalue_old);
				sprintf(buffer, "Old I2C_SLV2_ADDR register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(I2C_SLV2_ADDR,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(I2C_SLV2_ADDR,&icm_regvalue_old);
				sprintf(buffer, "New I2C_SLV2_ADDR register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  I2C_SLV2_REG  ***************************************
	if (strncmp(USB_command, "ICM_I2C_SLV2_REG", 16) == 0){

			ICM_SelectBank(USER_BANK_3);

				sprintf(buffer, "Received I2C_SLV2_REG register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(I2C_SLV2_REG,&icm_regvalue_old);
				sprintf(buffer, "Old I2C_SLV2_REG register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(I2C_SLV2_REG,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(I2C_SLV2_REG,&icm_regvalue_old);
				sprintf(buffer, "New I2C_SLV2_REG register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  I2C_SLV2_CTRL  ***************************************
	if (strncmp(USB_command, "ICM_I2C_SLV2_CTRL", 17) == 0){

			ICM_SelectBank(USER_BANK_3);

				sprintf(buffer, "Received I2C_SLV2_CTRL register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(I2C_SLV2_CTRL,&icm_regvalue_old);
				sprintf(buffer, "Old I2C_SLV2_CTRL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(I2C_SLV2_CTRL,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(I2C_SLV2_CTRL,&icm_regvalue_old);
				sprintf(buffer, "New I2C_SLV2_CTRL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  I2C_SLV2_DO  ***************************************
	if (strncmp(USB_command, "ICM_I2C_SLV2_DO", 15) == 0){

			ICM_SelectBank(USER_BANK_3);

				sprintf(buffer, "Received I2C_SLV2_DO register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(I2C_SLV2_DO,&icm_regvalue_old);
				sprintf(buffer, "Old I2C_SLV2_DO register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(I2C_SLV2_DO,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(I2C_SLV2_DO,&icm_regvalue_old);
				sprintf(buffer, "New I2C_SLV2_DO register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  I2C_SLV3_ADDR  ***************************************
	if (strncmp(USB_command, "ICM_I2C_SLV3_ADDR", 17) == 0){

			ICM_SelectBank(USER_BANK_3);

				sprintf(buffer, "Received I2C_SLV3_ADDR register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(I2C_SLV3_ADDR,&icm_regvalue_old);
				sprintf(buffer, "Old I2C_SLV3_ADDR register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(I2C_SLV3_ADDR,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(I2C_SLV3_ADDR,&icm_regvalue_old);
				sprintf(buffer, "New I2C_SLV3_ADDR register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  I2C_SLV3_REG  ***************************************
	if (strncmp(USB_command, "ICM_I2C_SLV3_REG", 16) == 0){

			ICM_SelectBank(USER_BANK_3);

				sprintf(buffer, "Received I2C_SLV3_REG register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(I2C_SLV3_REG,&icm_regvalue_old);
				sprintf(buffer, "Old I2C_SLV3_REG register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(I2C_SLV3_REG,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(I2C_SLV3_REG,&icm_regvalue_old);
				sprintf(buffer, "New I2C_SLV3_REG register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  I2C_SLV3_CTRL  ***************************************
	if (strncmp(USB_command, "ICM_I2C_SLV3_CTRL", 17) == 0){

			ICM_SelectBank(USER_BANK_3);

				sprintf(buffer, "Received I2C_SLV3_CTRL register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(I2C_SLV3_CTRL,&icm_regvalue_old);
				sprintf(buffer, "Old I2C_SLV3_CTRL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(I2C_SLV3_CTRL,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(I2C_SLV3_CTRL,&icm_regvalue_old);
				sprintf(buffer, "New I2C_SLV3_CTRL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  I2C_SLV3_DO  ***************************************
	if (strncmp(USB_command, "ICM_I2C_SLV3_DO", 15) == 0){

			ICM_SelectBank(USER_BANK_3);

				sprintf(buffer, "Received I2C_SLV3_DO register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(I2C_SLV3_DO,&icm_regvalue_old);
				sprintf(buffer, "Old I2C_SLV3_DO register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(I2C_SLV3_DO,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(I2C_SLV3_DO,&icm_regvalue_old);
				sprintf(buffer, "New I2C_SLV3_DO register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  I2C_SLV4_ADDR  ***************************************
	if (strncmp(USB_command, "ICM_I2C_SLV4_ADDR", 17) == 0){

			ICM_SelectBank(USER_BANK_3);

				sprintf(buffer, "Received I2C_SLV4_ADDR register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(I2C_SLV4_ADDR,&icm_regvalue_old);
				sprintf(buffer, "Old I2C_SLV4_ADDR register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(I2C_SLV4_ADDR,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(I2C_SLV4_ADDR,&icm_regvalue_old);
				sprintf(buffer, "New I2C_SLV4_ADDR register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  I2C_SLV4_REG  ***************************************
	if (strncmp(USB_command, "ICM_I2C_SLV4_REG", 16) == 0){

			ICM_SelectBank(USER_BANK_3);

				sprintf(buffer, "Received I2C_SLV4_REG register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(I2C_SLV4_REG,&icm_regvalue_old);
				sprintf(buffer, "Old I2C_SLV4_REG register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(I2C_SLV4_REG,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(I2C_SLV4_REG,&icm_regvalue_old);
				sprintf(buffer, "New I2C_SLV4_REG register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  I2C_SLV4_CTRL  ***************************************
	if (strncmp(USB_command, "ICM_I2C_SLV4_CTRL", 17) == 0){

			ICM_SelectBank(USER_BANK_3);

				sprintf(buffer, "Received I2C_SLV4_CTRL register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(I2C_SLV4_CTRL,&icm_regvalue_old);
				sprintf(buffer, "Old I2C_SLV4_CTRL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(I2C_SLV4_CTRL,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(I2C_SLV4_CTRL,&icm_regvalue_old);
				sprintf(buffer, "New I2C_SLV4_CTRL register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
	//*******************************  I2C_SLV4_DO  ***************************************
	if (strncmp(USB_command, "ICM_I2C_SLV4_DO", 15) == 0){

			ICM_SelectBank(USER_BANK_3);

				sprintf(buffer, "Received I2C_SLV4_DO register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				ICM_ReadOneByte(I2C_SLV4_DO,&icm_regvalue_old);
				sprintf(buffer, "Old I2C_SLV4_DO register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				ICM_WriteOneByte(I2C_SLV4_DO,icm_regvalue_new);

				//Read new register value
				ICM_ReadOneByte(I2C_SLV4_DO,&icm_regvalue_old);
				sprintf(buffer, "New I2C_SLV4_DO register value = %X\r\n", icm_regvalue_old);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}

}
