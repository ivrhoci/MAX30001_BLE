/*
 * ICM20948.h
 *
 *  Created on: Oct 26, 2018
 *      Author: cory
 */

#ifndef ICM20948_H_
#define ICM20948_H_

extern float accel_data[3];
extern float gyro_data[3];
extern float magn_data[3];
extern float temp_data;

#define SPI_BUS			(&hspi1) // ***
#define UART_BUS		(&huart3) // ***

#define USER_BANK_SEL	(0x7F)
#define USER_BANK_0		(0x00)
#define USER_BANK_1		(0x10)
#define USER_BANK_2		(0x20)
#define USER_BANK_3		(0x30)

//USER BANK 0 registers
#define WHO_AM_I        (0x00)
#define USER_CTRL       (0x03)  // R/W
#define LP_CONFIG       (0x05)  // R/W
#define PWR_MGMT_1 		(0x06)  // R/W
#define PWR_MGMT_2		(0x07)  // R/W
#define INT_PIN_CFG     (0x0F)  // R/W
#define INT_ENABLE      (0x10)  // R/W
#define INT_ENABLE_1    (0x11)  // R/W
#define INT_ENABLE_2    (0x12)  // R/W
#define INT_ENABLE_3    (0x13)  // R/W
#define I2C_MST_STATUS  (0x17)  // R/C
#define INT_STATUS      (0x19)  // R/C
#define INT_STATUS_1    (0x1A)  // R/C
#define INT_STATUS_2    (0x1B)  // R/C
#define INT_STATUS_3    (0x1C)  // R/C
#define FIFO_EN_1       (0x66)  // R/W
#define FIFO_EN_2       (0x67)  // R/W
#define FIFO_RST        (0x68)  // R/W
#define FIFO_MODE       (0x69)  // R/W
#define FIFO_R_W        (0x72)  // R/W
#define FIFO_CFG        (0x76)  // R/W
#define REG_BANK_SEL    (0x7F)  // R/W

//USER BANK 1 registers
#define SELF_TEST_X_GYRO       		(0x02)  // R/W
#define SELF_TEST_Y_GYRO       		(0x03)  // R/W
#define SELF_TEST_Z_GYRO       		(0x04)  // R/W
#define SELF_TEST_X_ACCEL 	   		(0x0E)  // R/W
#define SELF_TEST_Y_ACCEL	   		(0x0F)  // R/W
#define SELF_TEST_Z_ACCEL      		(0x10)  // R/W
#define XA_OFFS_H              		(0x14)  // R/W
#define XA_OFFS_L              		(0x15)  // R/W
#define YA_OFFS_H              		(0x17)  // R/W
#define YA_OFFS_L              		(0x18)  // R/W
#define ZA_OFFS_H              		(0x1A)  // R/W
#define ZA_OFFS_L              		(0x1B)  // R/W
#define TIMEBASE_CORRECTION_PLL     (0x28)  // R/W

//USER BANK 2 registers
#define GYRO_SMPLRT_DIV       		(0x00)  // R/W
#define GYRO_CONFIG_1       		(0x01)  // R/W
#define GYRO_CONFIG_2       		(0x02)  // R/W
#define XG_OFFS_USRH 	   			(0x03)  // R/W
#define XG_OFFS_USRL	   			(0x04)  // R/W
#define YG_OFFS_USRH      			(0x05)  // R/W
#define YG_OFFS_USRL              	(0x06)  // R/W
#define ZG_OFFS_USRH              	(0x07)  // R/W
#define ZG_OFFS_USRL              	(0x08)  // R/W
#define ODR_ALIGN_EN              	(0x09)  // R/W
#define ACCEL_SMPLRT_DIV_1          (0x10)  // R/W
#define ACCEL_SMPLRT_DIV_2          (0x11)  // R/W
#define ACCEL_INTEL_CTRL     		(0x12)  // R/W
#define ACCEL_WOM_THR      			(0x13)  // R/W
#define ACCEL_CONFIG_1              (0x14)  // R/W
#define ACCEL_CONFIG_2              (0x15)  // R/W
#define FSYNC_CONFIG              	(0x52)  // R/W
#define TEMP_CONFIG              	(0x53)  // R/W
#define MOD_CTRL_USR              	(0x54)  // R/W

//USER BANK 3 registers
#define I2C_MST_ODR_CONFIG       	(0x00)  // R/W
#define I2C_MST_CTRL       			(0x01)  // R/W
#define I2C_MST_DELAY_CTRL       	(0x02)  // R/W
#define I2C_SLV0_ADDR 	   			(0x03)  // R/W
#define I2C_SLV0_REG	   			(0x04)  // R/W
#define I2C_SLV0_CTRL      			(0x05)  // R/W
#define I2C_SLV0_DO              	(0x06)  // R/W
#define I2C_SLV1_ADDR              	(0x07)  // R/W
#define I2C_SLV1_REG              	(0x08)  // R/W
#define I2C_SLV1_CTRL              	(0x09)  // R/W
#define I2C_SLV1_DO         		(0x0A)  // R/W
#define I2C_SLV2_ADDR          		(0x0B)  // R/W
#define I2C_SLV2_REG     			(0x0C)  // R/W
#define I2C_SLV2_CTRL      			(0x0D)  // R/W
#define I2C_SLV2_DO              	(0x0E)  // R/W
#define I2C_SLV3_ADDR               (0x0F)  // R/W
#define I2C_SLV3_REG              	(0x10)  // R/W
#define I2C_SLV3_CTRL              	(0x11)  // R/W
#define I2C_SLV3_DO              	(0x12)  // R/W
#define I2C_SLV4_ADDR     			(0x13)  // R/W
#define I2C_SLV4_REG      			(0x14)  // R/W
#define I2C_SLV4_CTRL              	(0x15)  // R/W
#define I2C_SLV4_DO              	(0x16)  // R/W


#define CLK_BEST_AVAIL	(0x01)
#define GYRO_SCALE_250	(0x00)
#define GYRO_LPF_17HZ 	(0x29)

void ICM_PowerOn();
uint8_t ICM_WHOAMI(void);
void ICM_SelectBank(uint8_t bank);
void ICM_ReadAccelGyro(void);
void ICM_ReadMag(void);
uint16_t ICM_Initialize(void);
void ICM_SelectBank(uint8_t bank);
void ICM_Disable_I2C(void);
void ICM_CSHigh(void);
void ICM_CSLow(void);
void ICM_SetClock(uint8_t clk);
void ICM_AccelGyroOff(void);
void ICM_AccelGyroOn(void);
void ICM_SetGyro_Scale_and_LPF(uint8_t scale, uint8_t lpf);
void ICM_SetGyroLPF(uint8_t lpf);
void ICM_ReadFIFO(void);
void ICM_ReadTemp(void);
void icm_usb_init(char *USB_command, uint32_t regvalue);
#endif /* ICM20948_H_ */
