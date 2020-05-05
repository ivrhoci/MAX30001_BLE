#ifndef INC_OUTPUT_MODE_H_
#define INC_OUTPUT_MODE_H_

#define ECG_OUTPUT_MODE         0 //outputs ECG data
#define BIOZ_OUTPUT_MODE        1 //outputs BioZ data
#define RTOR_OUTPUT_MODE        2 //outputs RtoR data
#define COMBINED_OUTPUT_MODE    3 //outputs all ICM data
#define ACCEL_OUTPUT_MODE       4 //outputs accelerometer data
#define GYRO_OUTPUT_MODE        5 //outputs gyroscope data
#define MAGN_OUTPUT_MODE        6 //outputs magnetometer data
#define TEMP_OUTPUT_MODE        7 //outputs temperature data
#define DSP_OUTPUT_MODE         8 //outputs IIR filtered and FFT (raw and magnitude) data
#define BPM_OUTPUT_MODE         9 //outputs breathing frequency data (breaths per minute)

#define TEST_MODE                        1
#define BPM_MODE                         0

int change_output_mode(char *USB_command);
void ecg_output_mode(void);
void bioz_output_mode(void);
void rtor_output_mode(void);
void combined_output_mode(void);
void accel_output_mode(void);
void gyro_output_mode(void);
void magn_output_mode(void);
void temp_output_mode(void);

#endif /* INC_OUTPUT_MODE_H_ */
