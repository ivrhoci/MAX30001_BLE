# MAX30001_BLE
## Project Overview
This GitHub repository contains the embedded software part of my Master's Thesis project "Wireless System for Heart and Respiration Rate Monitoring Based on MAX30001 Integrated Circuit". The hardware on which this software is supposed to run can be found here (insert link to HW folder). The basic idea behind the project is to use the MAX30001 to gather ECG/Heart rate and BioZ (bioimpedance) data which is then processed using DSP algorithms and the output data (Heart and Respiration Rate) is transmitted over bluetooth. 

For development and further research capabilities configuration parameters (for MAX30001 and ICM-20948) and output data can be modified using a developed Matlab GUI which communicates with the board using UART interface (board USB is seen as VCOM port).

## Getting Started

The software was written in [STM32CubeIDE 1.1.0](https://www.st.com/en/development-tools/stm32cubeide.html) and it's written for [STM32WB55RG](https://www.st.com/en/microcontrollers-microprocessors/stm32wb55rg.html) MCU which controls the [MAX30001](https://www.maximintegrated.com/en/products/analog/data-converters/analog-front-end-ics/MAX30001.html) and [ICM-20948](https://invensense.tdk.com/products/motion-tracking/9-axis/icm-20948/) ICs over SPI interface.

The configuration parameters for both MAX30001 and ICM-20948 can be changed "on the go" with this [Matlab GUI](https://github.com/ivrhoci/MAX30001_BLE/tree/master/Matlab%20GUI). Also, the GUI enables to select which data is sent over bluetooth/USB/UART (TBD). For more information about the GUI see the readme file in the GUI's folder. 


