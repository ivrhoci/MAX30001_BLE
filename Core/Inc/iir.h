#ifndef INC_IIR_H_
#define INC_IIR_H_

/* Includes ------------------------------------------------------------------*/
#include "arm_math.h"
/* Private defines -----------------------------------------------------------*/
#define TEST_LENGTH_SAMPLES             512
#define BLOCK_SIZE                      1280
#define NUM_STAGES                       4
#define TEST_MODE                        1
#define BPM_MODE                         0
extern float32_t 	aIIR_F32_Output_Scaled[BLOCK_SIZE];
extern float        bioz_data[BLOCK_SIZE];

/* Exported functions ------------------------------------------------------- */
void IIR_PROCESSING_F32Process(uint32_t mode);

/* Private variables ---------------------------------------------------------*/


#endif /* INC_IIR_H_ */
