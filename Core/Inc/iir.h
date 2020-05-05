#ifndef INC_IIR_H_
#define INC_IIR_H_

/* Includes ------------------------------------------------------------------*/
#include "arm_math.h"
/* Private defines -----------------------------------------------------------*/
#define TEST_LENGTH_SAMPLES             512
#define BLOCK_SIZE                      512
#define NUM_STAGES                       4
#define TEST_MODE                        1
#define BPM_MODE                         0
extern float32_t 	aIIR_F32_Output_Scaled[BLOCK_SIZE];
extern float        bioz_data[TEST_LENGTH_SAMPLES];
#define TimerCount_Start()	do{\
							SysTick->LOAD  =  0xFFFFFF  ;	/* set reload register */\
							SysTick->VAL  =  0  ;			/* Clear Counter */		 \
							SysTick->CTRL  =  0x5 ;			/* Enable Counting*/	 \
							}while(0)

/* Systick Stop and retrieve CPU Clocks count */
#define TimerCount_Stop(Value) 	do {\
								SysTick->CTRL  =0;	/* Disable Counting */				 \
								Value = SysTick->VAL;/* Load the SysTick Counter Value */\
								Value = 0xFFFFFF - Value;/* Capture Counts in CPU Cycles*/\
								}while(0)

/* Exported functions ------------------------------------------------------- */
void IIR_PROCESSING_F32Process(uint32_t mode);

/* Private variables ---------------------------------------------------------*/


#endif /* INC_IIR_H_ */
