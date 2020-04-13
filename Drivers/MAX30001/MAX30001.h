/*******************************************************************************
* Copyright (C) 2015 Maxim Integrated Products, Inc., All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
*******************************************************************************/
/*
 * max30001.h
 *
 *  Created on: Oct 9, 2015
 *      Author: faisal.tariq
 */

#ifndef MAX30001_H_
#define MAX30001_H_

#define NULL 0  // dodano jer je javljalo use of undeclared identifier 'NULL'
#include<stdint.h>

#define mbed_COMPLIANT // Uncomment to Use timer for MAX30001 FCLK (for mbed)
                       // Comment to use the RTC clock

#define ASYNC_SPI_BUFFER_SIZE (32 * 3) // Maximimum buffer size for async byte transfers

// Defines for data callbacks
#define MAX30001_DATA_ECG        0x30
#define MAX30001_DATA_PACE       0x31
#define MAX30001_DATA_RTOR       0x32
#define MAX30001_DATA_BIOZ       0x33
#define MAX30001_DATA_LEADOFF_DC 0x34
#define MAX30001_DATA_LEADOFF_AC 0x35
#define MAX30001_DATA_BCGMON     0x36
#define MAX30001_DATA_ACLEADON   0x37

#define MAX30001_SPI_MASTER_PORT 0
#define MAX30001_SPI_SS_INDEX    0

#define MAX30001_INT_PORT_B      3
#define MAX30001_INT_PIN_B       6

#define MAX30001_INT_PORT_2B     4
#define MAX30001_INT_PIN_2B      5

#define MAX30001_INT_PORT_FCLK   1
#define MAX30001_INT_PIN_FCLK    7

#define MAX30001_FUNC_SEL_TMR    2 // 0=FW Control, 1= Pulse Train, 2=Timer

#define MAX30001_INDEX    3
#define MAX30001_POLARITY 0
#define MAX30001_PERIOD   30518
#define MAX30001_CYCLE    50

#define MAX30001_IOMUX_IO_ENABLE 1

#define MAX30001_SPI_PORT            0
#define MAX30001_CS_PIN              0
#define MAX30001_CS_POLARITY         0
#define MAX30001_CS_ACTIVITY_DELAY   0
#define MAX30001_CS_INACTIVITY_DELAY 0
#define MAX30001_CLK_HI              1
#define MAX30001_CLK_LOW             1
#define MAX30001_ALT_CLK             0
#define MAX30001_CLK_POLARITY        0
#define MAX30001_CLK_PHASE           0
#define MAX30001_WRITE               1
#define MAX30001_READ                0

#define MAX30001_INT_PORT_B  3
#define MAX30001INT_PIN_B    6

void MAX30001_AllowInterrupts(int state);

/**
* Maxim Integrated MAX30001 ECG/BIOZ chip
*/
//class MAX30001 {

//public:
  typedef enum { // MAX30001 Register addresses
    STATUS     = 0x01,
    EN_INT     = 0x02,
    EN_INT2    = 0x03,
    MNGR_INT   = 0x04,
    MNGR_DYN   = 0x05,
    SW_RST     = 0x08,
    SYNCH      = 0x09,
    FIFO_RST   = 0x0A,
    INFO       = 0x0F,
    CNFG_GEN   = 0x10,
    CNFG_CAL   = 0x12,
    CNFG_EMUX  = 0x14,
    CNFG_ECG   = 0x15,
    CNFG_BMUX  = 0x17,
    CNFG_BIOZ  = 0x18,
    CNFG_PACE  = 0x1A,
    CNFG_RTOR1 = 0x1D,
    CNFG_RTOR2 = 0x1E,

    // Data locations
    ECG_FIFO_BURST = 0x20,
    ECG_FIFO       = 0x21,
    FIFO_BURST     = 0x22,
    BIOZ_FIFO      = 0x23,
    RTOR           = 0x25,

    PACE0_FIFO_BURST = 0x30,
    PACE0_A          = 0x31,
    PACE0_B          = 0x32,
    PACE0_C          = 0x33,

    PACE1_FIFO_BURST = 0x34,
    PACE1_A          = 0x35,
    PACE1_B          = 0x36,
    PACE1_C          = 0x37,

    PACE2_FIFO_BURST = 0x38,
    PACE2_A          = 0x39,
    PACE2_B          = 0x3A,
    PACE2_C          = 0x3B,

    PACE3_FIFO_BURST = 0x3C,
    PACE3_A          = 0x3D,
    PACE3_B          = 0x3E,
    PACE3_C          = 0x3F,

    PACE4_FIFO_BURST = 0x40,
    PACE4_A          = 0x41,
    PACE4_B          = 0x42,
    PACE4_C          = 0x43,

    PACE5_FIFO_BURST = 0x44,
    PACE5_A          = 0x45,
    PACE5_B          = 0x46,
    PACE5_C          = 0x47,

  } MAX30001_REG_map_t;


  /*********************************************************************************/

  typedef enum {
    MAX30001_NO_INT = 0, // No interrupt
    MAX30001_INT_B  = 1,  // INTB selected for interrupt
    MAX30001_INT_2B = 2  // INT2B selected for interrupt
  } max30001_intrpt_Location_t;


typedef enum {
    MAX30001_INT_DISABLED = 0,
    MAX30001_INT_CMOS     = 1,
    MAX30001_INT_ODN      = 2,
    MAX30001_INT_ODNR     = 3
  } max30001_intrpt_type_t;

  typedef enum {          // Input Polarity selection
    MAX30001_NON_INV = 0, // Non-Inverted
    MAX30001_INV     = 1      // Inverted
  } max30001_emux_pol;

  typedef enum {              // OPENP and OPENN setting
    MAX30001_ECG_CON_AFE = 0, // ECGx is connected to AFE channel
    MAX30001_ECG_ISO_AFE = 1  // ECGx is isolated from AFE channel
  } max30001_emux_openx;

  typedef enum {                // EMUX_CALP_SEL & EMUX_CALN_SEL
    MAX30001_NO_CAL_SIG = 0, // No calibration signal is applied
    MAX30001_INPT_VMID  = 1,  // Input is connected to VMID
    MAX30001_INPT_VCALP = 2, // Input is connected to VCALP
    MAX30001_INPT_VCALN = 3  // Input is connected to VCALN
  } max30001_emux_calx_sel;

  typedef enum {                     // EN_ECG, EN_BIOZ, EN_PACE
    MAX30001_CHANNEL_DISABLED = 0, //
    MAX30001_CHANNEL_ENABLED = 1
  } max30001_en_feature;

  /*********************************************************************************/
 // Data
  extern uint32_t max30001_ECG_FIFO_buffer[32]; // (303 for internal test)

  typedef struct { // Creating a structure for BLE data
    int16_t R2R;
    int16_t fmstr;
  } max30001_t;

  extern max30001_t hspValMax30001; // R2R, FMSTR
  



  /**
   * @brief This function sets up the Resistive Bias mode and also selects the master clock frequency.
   * @brief Uses Register: CNFG_GEN-0x10
   * @param En_rbias: Enable and Select Resitive Lead Bias Mode
   * @param Rbiasv: Resistive Bias Mode Value Selection
   * @param Rbiasp: Enables Resistive Bias on Positive Input
   * @param Rbiasn: Enables Resistive Bias on Negative Input
   * @param Fmstr: Selects Master Clock Frequency
   * @returns 0-if no error.  A non-zero value indicates an error.
   *
  */
  int max30001_Rbias_FMSTR_Init(uint8_t En_rbias, uint8_t Rbiasv,
                                uint8_t Rbiasp, uint8_t Rbiasn, uint8_t Fmstr);

  /**
   * @brief This function uses sets up the calibration signal internally.  If it is desired to use the internal signal, then
   * @brief this function must be called and the registers set, prior to setting the CALP_SEL and CALN_SEL in the ECG_InitStart
   * @brief and BIOZ_InitStart functions.
   * @brief Uses Register: CNFG_CAL-0x12
   * @param En_Vcal: Calibration Source (VCALP and VCALN) Enable
   * @param Vmode:   Calibration Source Mode Selection
   * @param Vmag:    Calibration Source Magnitude Selection (VMAG)
   * @param Fcal:    Calibration Source Frequency Selection (FCAL)
   * @param Thigh:   Calibration Source Time High Selection
   * @param Fifty:   Calibration Source Duty Cycle Mode Selection
   * @returns 0-if no error.  A non-zero value indicates an error.
   *
   */
  int max30001_CAL_InitStart(uint8_t En_Vcal, uint8_t Vmode, uint8_t Vmag,
                             uint8_t Fcal, uint16_t Thigh, uint8_t Fifty);

  /**
   * @brief This function disables the VCAL signal
   * @returns 0-if no error.  A non-zero value indicates an error.
   */
  int max30001_CAL_Stop(void);

  /**
   * @brief This function handles the assignment of the two interrupt pins (INTB & INT2B) with various
   * @brief functions/behaviors  of the MAX30001.  Also, each pin can be configured for different drive capability.
   * @brief Uses Registers: EN_INT-0x02 and EN_INT2-0x03.
   * @param max30001_intrpt_Locatio_t  <argument>:  All the arguments with the aforementioned enumeration essentially
   *        can be configured to generate an interrupt on either INTB or INT2B or NONE.
   * @param max30001_intrpt_type_t  intb_Type:  INTB Port Type (EN_INT Selections).
   * @param max30001_intrpt_type _t int2b_Type:   INT2B Port Type (EN_INT2 Selections)
   * @returns 0-if no error.  A non-zero value indicates an error.
   *
   */
     int max30001_INT_assignment(max30001_intrpt_Location_t en_enint_loc,     max30001_intrpt_Location_t en_eovf_loc,  max30001_intrpt_Location_t en_fstint_loc,
		                 max30001_intrpt_Location_t en_dcloffint_loc, max30001_intrpt_Location_t en_bint_loc,  max30001_intrpt_Location_t en_bovf_loc,
		                 max30001_intrpt_Location_t en_bover_loc,     max30001_intrpt_Location_t en_bundr_loc, max30001_intrpt_Location_t en_bcgmon_loc,
		                 max30001_intrpt_Location_t en_pint_loc,      max30001_intrpt_Location_t en_povf_loc,  max30001_intrpt_Location_t en_pedge_loc,
		                 max30001_intrpt_Location_t en_lonint_loc,    max30001_intrpt_Location_t en_rrint_loc, max30001_intrpt_Location_t en_samp_loc,
		                 max30001_intrpt_type_t  intb_Type,           max30001_intrpt_type_t int2b_Type);



  /**
   * @brief For MAX30001/3 ONLY
   * @brief This function sets up the MAX30001 for the ECG measurements.
   * @brief Registers used:  CNFG_EMUX, CNFG_GEN, MNGR_INT, CNFG_ECG.
   * @param En_ecg: ECG Channel Enable <CNFG_GEN register bits>
   * @param Openp: Open the ECGN Input Switch (most often used for testing and calibration studies) <CNFG_EMUX register bits>
   * @param Openn: Open the ECGN Input Switch (most often used for testing and calibration studies) <CNFG_EMUX register bits>
   * @param Calp_sel: ECGP Calibration Selection <CNFG_EMUX register bits>
   * @param Caln_sel: ECGN Calibration Selection <CNFG_EMUX register bits>
   * @param E_fit: ECG FIFO Interrupt Threshold (issues EINT based on number of unread FIFO records) <CNFG_GEN register bits>
   * @param Clr_rrint: RTOR R Detect Interrupt (RRINT) Clear Behavior <CNFG_GEN register bits>
   * @param Rate: ECG Data Rate
   * @param Gain: ECG Channel Gain Setting
   * @param Dhpf: ECG Channel Digital High Pass Filter Cutoff Frequency
   * @param Dlpf:  ECG Channel Digital Low Pass Filter Cutoff Frequency
   * @returns 0-if no error.  A non-zero value indicates an error.
   *
   */
  int max30001_ECG_InitStart(uint8_t En_ecg, uint8_t Openp, uint8_t Openn,
                             uint8_t Pol, uint8_t Calp_sel, uint8_t Caln_sel,
                             uint8_t E_fit, uint8_t Rate, uint8_t Gain,
                             uint8_t Dhpf, uint8_t Dlpf);

  /**
   * @brief For MAX30001/3 ONLY
   * @brief This function enables the Fast mode feature of the ECG.
   * @brief Registers used: MNGR_INT-0x04, MNGR_DYN-0x05
   * @param Clr_Fast: FAST MODE Interrupt Clear Behavior <MNGR_INT Register>
   * @param Fast: ECG Channel Fast Recovery Mode Selection (ECG High Pass Filter Bypass) <MNGR_DYN Register>
   * @param Fast_Th: Automatic Fast Recovery Threshold
   * @returns 0-if no error.  A non-zero value indicates an error.
   *
   */
  int max30001_ECGFast_Init(uint8_t Clr_Fast, uint8_t Fast, uint8_t Fast_Th);

  /**
  * @brief For MAX30001/3 ONLY
  * @brief This function disables the ECG.
  * @brief Uses Register CNFG_GEN-0x10.
  * @returns 0-if no error.  A non-zero value indicates an error.
  *
  */
  int max30001_Stop_ECG(void);

  /**
   *  @brief For MAX30001 ONLY
   *  @brief This function sets up the MAX30001 for pace signal detection.
   *  @brief If both PACE and BIOZ are turned ON, then make sure Fcgen is set for 80K or 40K in the
   *  @brief max30001_BIOZ_InitStart() function.  However, if Only PACE is on but BIOZ off, then Fcgen can be set
   *  @brief for 80K only, in the max30001_BIOZ_InitStart() function
   *  @brief Registers used: MNGR_INT-0x04, CNFG_GEN-0x37, CNFG_PACE-0x1A.
   *  @param En_pace : PACE Channel Enable <CNFG_GEN Register>
   *  @param Clr_pedge : PACE Edge Detect Interrupt (PEDGE) Clear Behavior <MNGR_INT Register>
   *  @param Pol: PACE Input Polarity Selection <CNFG_PACE Register>
   *  @param Gn_diff_off: PACE Differentiator Mode <CNFG_PACE Register>
   *  @param Gain: PACE Channel Gain Selection <CNFG_PACE Register>
   *  @param Aout_lbw:  PACE Analog Output Buffer Bandwidth Mode <CNFG_PACE Register>
   *  @param Aout: PACE Single Ended Analog Output Buffer Signal Monitoring Selection <CNFG_PACE Register>
   *  @param Dacp (4bits): PACE Detector Positive Comparator Threshold <CNFG_PACE Register>
   *  @param Dacn(4bits): PACE Detector Negative Comparator Threshold <CNFG_PACE Register>
   *  @returns 0-if no error.  A non-zero value indicates an error <CNFG_PACE Register>
   *
   */
  int max30001_PACE_InitStart(uint8_t En_pace, uint8_t Clr_pedge, uint8_t Pol,
                              uint8_t Gn_diff_off, uint8_t Gain,
                              uint8_t Aout_lbw, uint8_t Aout, uint8_t Dacp,
                              uint8_t Dacn);

  /**
   *@brief For MAX30001 ONLY
   *@param This function disables the PACE.  Uses Register CNFG_GEN-0x10.
   *@returns 0-if no error.  A non-zero value indicates an error.
   *
   */
  int max30001_Stop_PACE(void);

  /**
   * @brief For MAX30001/2 ONLY
   * @brief This function sets up the MAX30001 for BIOZ measurement.
   * @brief Registers used: MNGR_INT-0x04, CNFG_GEN-0X10, CNFG_BMUX-0x17,CNFG_BIOZ-0x18.
   * @param En_bioz: BIOZ Channel Enable <CNFG_GEN Register>
   * @param Openp: Open the BIP Input Switch <CNFG_BMUX Register>
   * @param Openn: Open the BIN Input Switch <CNFG_BMUX Register>
   * @param Calp_sel: BIP Calibration Selection <CNFG_BMUX Register>
   * @param Caln_sel: BIN Calibration Selection <CNFG_BMUX Register>
   * @param CG_mode:  BIOZ Current Generator Mode Selection <CNFG_BMUX Register>
   * @param B_fit:  BIOZ FIFO Interrupt Threshold (issues BINT based on number of unread FIFO records) <MNGR_INT Register>
   * @param Rate: BIOZ Data Rate <CNFG_BIOZ Register>
   * @param Ahpf: BIOZ/PACE Channel Analog High Pass Filter Cutoff Frequency and Bypass <CNFG_BIOZ Register>
   * @param Ext_rbias:  External Resistor Bias Enable <CNFG_BIOZ Register>
   * @param Gain: BIOZ Channel Gain Setting <CNFG_BIOZ Register>
   * @param Dhpf: BIOZ Channel Digital High Pass Filter Cutoff Frequency <CNFG_BIOZ Register>
   * @param Dlpf:  BIOZ Channel Digital Low Pass Filter Cutoff Frequency <CNFG_BIOZ Register>
   * @param Fcgen:  BIOZ Current Generator Modulation Frequency <CNFG_BIOZ Register>
   * @param Cgmon:  BIOZ Current Generator Monitor <CNFG_BIOZ Register>
   * @param Cgmag:  BIOZ Current Generator Magnitude <CNFG_BIOZ Register>
   * @param Phoff: BIOZ Current Generator Modulation Phase Offset <CNFG_BIOZ Register>
   * @param Inapow_mode: BIOZ Channel Instrumentation Amplifier (INA) Power Mode <CNFG_BIOZ Register>
   * @returns 0-if no error.  A non-zero value indicates an error.
   *
   */
  int max30001_BIOZ_InitStart(uint8_t En_bioz, uint8_t Openp, uint8_t Openn,
                              uint8_t Calp_sel, uint8_t Caln_sel,
                              uint8_t CG_mode,
                              /* uint8_t En_bioz,*/ uint8_t B_fit, uint8_t Rate,
                              uint8_t Ahpf, uint8_t Ext_rbias, uint8_t Gain,
                              uint8_t Dhpf, uint8_t Dlpf, uint8_t Fcgen,
                              uint8_t Cgmon, uint8_t Cgmag, uint8_t Phoff, uint8_t Inapow_mode);

  /**
   * @brief For MAX30001/2 ONLY
   * @brief This function disables the BIOZ.  Uses Register CNFG_GEN-0x10.
   * @returns 0-if no error.  A non-zero value indicates an error.
       * @returns 0-if no error.  A non-zero value indicates an error.
   *
   */
  int max30001_Stop_BIOZ(void);

  /**
   * @brief For MAX30001/2 ONLY
   * @brief BIOZ modulated Resistance Built-in-Self-Test, Registers used: CNFG_BMUX-0x17
   * @param En_bist: Enable Modulated Resistance Built-in-Self-test <CNFG_BMUX Register>
   * @param Rnom: BIOZ RMOD BIST Nominal Resistance Selection <CNFG_BMUX Register>
   * @param Rmod: BIOZ RMOD BIST Modulated Resistance Selection <CNFG_BMUX Register>
   * @param Fbist: BIOZ RMOD BIST Frequency Selection <CNFG_BMUX Register>
   * @returns 0-if no error.  A non-zero value indicates an error.
   *
   */
  int max30001_BIOZ_InitBist(uint8_t En_bist, uint8_t Rnom, uint8_t Rmod,
                             uint8_t Fbist);

  /**
   * @brief For MAX30001/3/4 ONLY
   * @brief Sets up the device for RtoR measurement
   * @param EN_rtor: ECG RTOR Detection Enable <RTOR1 Register>
   * @param Wndw: R to R Window Averaging (Window Width = RTOR_WNDW[3:0]*8mS) <RTOR1 Register>
   * @param Gain: R to R Gain (where Gain = 2^RTOR_GAIN[3:0], plus an auto-scale option) <RTOR1 Register>
   * @param Pavg: R to R Peak Averaging Weight Factor <RTOR1 Register>
   * @param Ptsf: R to R Peak Threshold Scaling Factor <RTOR1 Register>
   * @param Hoff: R to R minimum Hold Off <RTOR2 Register>
   * @param Ravg: R to R Interval Averaging Weight Factor <RTOR2 Register>
   * @param Rhsf: R to R Interval Hold Off Scaling Factor <RTOR2 Register>
   * @param Clr_rrint: RTOR Detect Interrupt Clear behaviour <MNGR_INT Register>
   * @returns 0-if no error.  A non-zero value indicates an error.
   *
   */
  int max30001_RtoR_InitStart(uint8_t En_rtor, uint8_t Wndw, uint8_t Gain,
                              uint8_t Pavg, uint8_t Ptsf, uint8_t Hoff,
                              uint8_t Ravg, uint8_t Rhsf, uint8_t Clr_rrint);

  /**
   * @brief For MAX30001/3/4 ONLY
   * @brief This function disables the RtoR.  Uses Register CNFG_RTOR1-0x1D
   * @returns 0-if no error.  A non-zero value indicates an error.
   *
   */
  int max30001_Stop_RtoR(void);

  /**
   * @brief This is a function that waits for the PLL to lock; once a lock is achieved it exits out. (For convenience only)
   * @returns 0-if no error.  A non-zero value indicates an error.
   *
   */
  int max30001_PLL_lock(void);

  /**
   * @brief This function causes the MAX30001 to reset.  Uses Register SW_RST-0x08
   * @return 0-if no error.  A non-zero value indicates an error.
   *
   */
  int max30001_sw_rst(void);

  /**
   * @brief This function provides a SYNCH operation.  Uses Register SYCNH-0x09. Please refer to the data sheet for
   * @brief the details on how to use this.
   * @returns 0-if no error.  A non-zero value indicates an error.
   *
   */
  int max30001_synch(void);

  /**
   * @brief This function performs a FIFO Reset.  Uses Register FIFO_RST-0x0A. Please refer to the data sheet
   * @brief for the details on how to use this.
   * @returns 0-if no error.  A non-zero value indicates an error.
   */
  int max300001_fifo_rst(void);

  /**
   *
   * @brief This is a callback function which collects all the data from the ECG, BIOZ, PACE and RtoR. It also handles
   * @brief Lead On/Off.  This  function is passed through the argument of max30001_COMMinit().
   * @returns 0-if no error.  A non-zero value indicates an error.
   *
   */
  int max30001_int_handler(void);

  /**
   * @brief This is function called from the max30001_int_handler() function and processes all the ECG, BIOZ, PACE
   * @brief and the RtoR data and sticks them in appropriate arrays and variables each unsigned 32 bits.
   * @param ECG data will be in the array (input): max30001_ECG_FIFO_buffer[]
   * @param Pace data will be in the array (input): max30001_PACE[]
   * @param RtoRdata will be in the variable (input): max30001_RtoR_data
   * @param BIOZ data will be in the array (input): max30001_BIOZ_FIFO_buffer[]
   * @param global  max30001_ECG_FIFO_buffer[]
   * @param global  max30001_PACE[]
   * @param global  max30001_BIOZ_FIFO_buffer[]
   * @param global  max30001_RtoR_data
   * @param global  max30001_DCLeadOff
   * @param global  max30001_ACLeadOff
   * @param global  max30001_LeadON
   * @returns 0-if no error.  A non-zero value indicates an error.
   *
   */
  int max30001_FIFO_LeadONOff_Read(void);

  /**
   * @brief This function allows writing to a register.
   * @param addr:  Address of the register to write to
   * @param data:  24-bit data read from the register.
   * @returns 0-if no error.  A non-zero value indicates an error.
   *
   */
  int max30001_reg_write(MAX30001_REG_map_t addr, uint32_t data);

  /**
   * @brief This function allows reading from a register
   * @param addr:   Address of the register to read from.
   * @param *return_data: pointer to the value read from the register.
   * @returns 0-if no error.  A non-zero value indicates an error.
   *
   */
  int max30001_reg_read(MAX30001_REG_map_t addr, uint32_t *return_data);

  /**
   * @brief This function enables the DC Lead Off detection. Either ECG or BIOZ can be detected, one at a time.
   * @brief Registers Used:  CNFG_GEN-0x10
   * @param En_dcloff: BIOZ Digital Lead Off Detection Enable
   * @param Ipol: DC Lead Off Current Polarity (if current sources are enabled/connected)
   * @param Imag: DC Lead off current Magnitude Selection
   * @param Vth: DC Lead Off Voltage Threshold Selection
   * @returns 0-if no error.  A non-zero value indicates an error.
   *
   */
  int max30001_Enable_DcLeadOFF_Init(int8_t En_dcloff, int8_t Ipol, int8_t Imag,
                                     int8_t Vth);

  /**
   * @brief This function disables the DC Lead OFF feature, whichever is active.
   * @returns 0-if no error.  A non-zero value indicates an error.
   *
   */
  int max30001_Disable_DcLeadOFF(void);

  /**
   * @brief This function sets up the BIOZ for AC Lead Off test.
   * @brief Registers Used:  CNFG_GEN-0x10, MNGR_DYN-0x05
   * @param En_bloff: BIOZ Digital Lead Off Detection Enable <CNFG_GEN register>
   * @param Bloff_hi_it:      DC Lead Off Current Polarity (if current sources are enabled/connected) <MNGR_DYN register>
   * @param Bloff_lo_it:      DC Lead off current Magnitude Selection <MNGR_DYN register>
   * @returns 0-if no error.  A non-zero value indicates an error.
   *
   */
  int max30001_BIOZ_Enable_ACLeadOFF_Init(uint8_t En_bloff, uint8_t Bloff_hi_it,
                                          uint8_t Bloff_lo_it);

  /**
   * @brief This function Turns of the BIOZ AC Lead OFF feature
   * @brief Registers Used:  CNFG_GEN-0x10
   * @returns 0-if no error.  A non-zero value indicates an error.
   *
   */
  int max30001_BIOZ_Disable_ACleadOFF(void);

  /**
   * @brief This function enables the Current Gnerator Monitor
   * @brief Registers Used:  CNFG_BIOZ-0x18
   * @returns 0-if no error.  A non-zero value indicates an error.
   *
   */
  int max30001_BIOZ_Enable_BCGMON(void);

  /**
   *
   * @brief This function enables the Lead ON detection. Either ECG or BIOZ can be detected, one at a time.
   * @brief Also, the en_bioz, en_ecg, en_pace setting is saved so that when this feature is disabled through the
   * @brief max30001_Disable_LeadON() function (or otherwise) the enable/disable state of those features can be retrieved.
   * @param Channel: ECG or BIOZ detection
   * @returns 0-if everything is good.  A non-zero value indicates an error.
   *
   */
  int max30001_Enable_LeadON(int8_t Channel);

  /**
   * @brief This function turns off the Lead ON feature, whichever one is active.  Also, retrieves the en_bioz,
   * @brief en_ecg, en_pace and sets it back to as it was.
   * @param 0-if everything is good.  A non-zero value indicates an error.
   *
   */
  int max30001_Disable_LeadON(void);

  /**
   *
   * @brief This function is toggled every 2 seconds to switch between ECG Lead ON and BIOZ Lead ON detect
   * @brief Adjust LEADOFF_SERVICE_TIME to determine the duration between the toggles.
   * @param CurrentTime - This gets fed the time by RTC_GetValue function
   *
   */
  void max30001_ServiceLeadON(uint32_t currentTime);

  /**
   *
   * @brief This function is toggled every 2 seconds to switch between ECG DC Lead Off and BIOZ DC Lead Off
   * @brief Adjust LEADOFF_SERVICE_TIME to determine the duration between the toggles.
   * @param CurrentTime - This gets fed the time by RTC_GetValue function
   *
   */
  void max30001_ServiceLeadoff(uint32_t currentTime);

  /**
   *
   * @brief This function sets current RtoR values and fmstr values in a pointer structure
   * @param hspValMax30001 - Pointer to a structure where to store the values
   *
   */
  void max30001_ReadHeartrateData(max30001_t *_hspValMax30001);

  /**
   * @brief type definition for data interrupt
   */
  typedef void (*PtrFunction)(uint32_t id, uint32_t *buffer, uint32_t length);

  /**
   * @brief Used to connect a callback for when interrupt data is available
   */
  void onDataAvailable(PtrFunction _onDataAvailable);

 // static MAX30001 *instance;

//private:
  void dataAvailable(uint32_t id, uint32_t *buffer, uint32_t length);
  /// interrupt handler for async spi events
  //static void spiHandler(int events);
  /// wrapper method to transmit and recieve SPI data
  int SPI_Transmit(const uint8_t *tx_buf, uint32_t tx_size, uint8_t *rx_buf,
                   uint32_t rx_size);
   uint32_t readPace(int group, uint8_t* result);


/**
 * @brief Preventive measure used to dismiss interrupts that fire too early during
 * @brief initialization on INTB line
 *
 */
void MAX30001Mid_IntB_Handler(void);

/**
 * @brief Preventive measure used to dismiss interrupts that fire too early during
 * @brief initialization on INT2B line
 *
 */
void MAX30001Mid_Int2B_Handler(void);

/**
 * @brief Allows Interrupts to be accepted as valid.
 * @param state: 1-Allow interrupts, Any-Don't allow interrupts.
 *
 */
void MAX30001_AllowInterrupts(int state);

#endif /* MAX30001_H_ */

