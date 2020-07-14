/**
  ******************************************************************************
  * @file    STM32F429_DSPDEMO/Src/fft_processing.c
  * @author  MCD Application Team
  * @brief   FFT calculation Service Routines
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "fft.h"


float32_t aFFT_Input_f32[FFT_Length]={ 0 };
float32_t aFFT_Output_f32 [FFT_Length]={ 0 };
float32_t Cmplx_Mag_Output_f32 [FFT_Length/2]={ 0 };
uint8_t ifftFlag = 0;

/**
  * @brief  This function Calculate FFT in F32.
  * @param  FFT Length : 4096, 2048, 1024, 256, 64
  *         mode:
  *         0 - test mode - debug outputs
  *         1 - bpm mode - used in bpm calculation
  * @retval None
  */
uint32_t FFT_PROCESSING_F32Process(uint32_t mode)
{
  arm_rfft_fast_instance_f32  FFT_F32_struct;

  float32_t maxValue = 0;    /* Max FFT value is stored here */
  uint32_t maxIndex = 0;    /* Index in Output array where max value is */

  for (int i = 0; i < FFT_Length; i++)
  {
	  //Copy data from IIR filter
	  if(i<BLOCK_SIZE)
		  aFFT_Input_f32[i] = aIIR_F32_Output_Scaled[i];
	  else /* Zero padding*/
		  aFFT_Input_f32[i] = 0;
  }

  /* Initialize the RFFT/RIFFT module */
  arm_rfft_fast_init_f32(&FFT_F32_struct, FFT_Length);
 /* RFFT processing*/
  arm_rfft_fast_f32(&FFT_F32_struct, aFFT_Input_f32, aFFT_Output_f32, ifftFlag);

  if (mode == TEST_MODE)
    {
	  printf("RFFT (real FFT) with N=4096 done\r\n");
	  printf("FFT output (real[0], imag[0], real[1], imag[1], ...: \r\n");
	  for (int i = 0; i < FFT_Length; i++)
	  {
		  printf("%f\r\n", aFFT_Output_f32[i]);
	  }
    }


  /* Process the data through the Complex Magnitude Module for calculating the magnitude at each bin */
  arm_cmplx_mag_f32(aFFT_Output_f32, Cmplx_Mag_Output_f32, FFT_Length/2);

  if (mode == TEST_MODE)
  {
	  printf("Complex Magnitude output: \r\n");
	  for (int i = 0; i < FFT_Length/2; i++)
	  {
		  printf("%f\r\n", Cmplx_Mag_Output_f32[i]);
	  }
  }

  /* Calculates maxValue and maxIndex */
  arm_max_f32(Cmplx_Mag_Output_f32, FFT_Length/2, &maxValue, &maxIndex);

  if (mode == TEST_MODE)
  {
	  printf("Max value (not scaled with N) = %f\r\n", maxValue);
	  printf("Max index = %d\r\n", maxIndex);
  }

  return maxIndex;

}


/************************ (C) COPYRIGHT STMicroelectronics ************************/
