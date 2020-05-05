#include<max30001.h>
#include<stdbool.h>
#include<main.h>
#include<string.h>
//#include "mbed.h"

/*********************************************************************************/
/**
   * @brief STATUS (0x01) 
   */
 union max30001_status_reg {
    uint32_t all;

    struct {
      uint32_t loff_nl : 1;
      uint32_t loff_nh : 1;
      uint32_t loff_pl : 1;
      uint32_t loff_ph : 1;

      uint32_t bcgmn     : 1;
      uint32_t bcgmp     : 1;
      uint32_t reserved1 : 1;
      uint32_t reserved2 : 1;

      uint32_t pllint : 1;
      uint32_t samp   : 1;
      uint32_t rrint  : 1;
      uint32_t lonint : 1;

      uint32_t pedge  : 1;
      uint32_t povf   : 1;
      uint32_t pint   : 1;
      uint32_t bcgmon : 1;

      uint32_t bundr : 1;
      uint32_t bover : 1;
      uint32_t bovf  : 1;
      uint32_t bint  : 1;

      uint32_t dcloffint : 1;
      uint32_t fstint    : 1;
      uint32_t eovf      : 1;
      uint32_t eint      : 1;

      uint32_t reserved : 8;

    } bit;

  } max30001_status;

  
  /**
   * @brief EN_INT (0x02) 
   */

 union max30001_en_int_reg {
    uint32_t all;

    struct {
      uint32_t intb_type : 2;
      uint32_t reserved1 : 1;
      uint32_t reserved2 : 1;

      uint32_t reserved3 : 1;
      uint32_t reserved4 : 1;
      uint32_t reserved5 : 1;
      uint32_t reserved6 : 1;

      uint32_t en_pllint : 1;
      uint32_t en_samp   : 1;
      uint32_t en_rrint  : 1;
      uint32_t en_lonint : 1;

      uint32_t en_pedge  : 1;
      uint32_t en_povf   : 1;
      uint32_t en_pint   : 1;
      uint32_t en_bcgmon : 1;

      uint32_t en_bundr : 1;
      uint32_t en_bover : 1;
      uint32_t en_bovf  : 1;
      uint32_t en_bint  : 1;

      uint32_t en_dcloffint : 1;
      uint32_t en_fstint    : 1;
      uint32_t en_eovf      : 1;
      uint32_t en_eint      : 1;

      uint32_t reserved : 8;

    } bit;

  } max30001_en_int;

  
  /**
   * @brief EN_INT2 (0x03) 
   */  
  union max30001_en_int2_reg {
    uint32_t all;

    struct {
      uint32_t intb_type : 2;
      uint32_t reserved1 : 1;
      uint32_t reserved2 : 1;

      uint32_t reserved3 : 1;
      uint32_t reserved4 : 1;
      uint32_t reserved5 : 1;
      uint32_t reserved6 : 1;

      uint32_t en_pllint : 1;
      uint32_t en_samp   : 1;
      uint32_t en_rrint  : 1;
      uint32_t en_lonint : 1;

      uint32_t en_pedge  : 1;
      uint32_t en_povf   : 1;
      uint32_t en_pint   : 1;
      uint32_t en_bcgmon : 1;

      uint32_t en_bundr  : 1;
      uint32_t en_bover  : 1;
      uint32_t en_bovf   : 1;
      uint32_t en_bint   : 1;

      uint32_t en_dcloffint : 1;
      uint32_t en_fstint    : 1;
      uint32_t en_eovf      : 1;
      uint32_t en_eint      : 1;

      uint32_t reserved : 8;

    } bit;

  } max30001_en_int2;

  /**
   * @brief MNGR_INT (0x04) 
   */  
  union max30001_mngr_int_reg {
    uint32_t all;

    struct {
      uint32_t samp_it   : 2;
      uint32_t clr_samp  : 1;
      uint32_t clr_pedge : 1;
      uint32_t clr_rrint : 2;
      uint32_t clr_fast  : 1;
      uint32_t reserved1 : 1;
      uint32_t reserved2 : 4;
      uint32_t reserved3 : 4;

      uint32_t b_fit     : 3;
      uint32_t e_fit     : 5;

      uint32_t reserved : 8;

    } bit;

  } max30001_mngr_int;

   /**
   * @brief MNGR_DYN (0x05) 
   */ 
  union max30001_mngr_dyn_reg {
    uint32_t all;

    struct {
      uint32_t bloff_lo_it : 8;
      uint32_t bloff_hi_it : 8;
      uint32_t fast_th     : 6;
      uint32_t fast        : 2;
      uint32_t reserved    : 8;
    } bit;

  } max30001_mngr_dyn;

  // 0x08
  // uint32_t max30001_sw_rst;

  // 0x09
  // uint32_t max30001_synch;

  // 0x0A
  // uint32_t max30001_fifo_rst;

  
   /**
   * @brief INFO (0x0F) 
   */
  union max30001_info_reg {
    uint32_t all;
    struct {
      uint32_t serial    : 12;
      uint32_t part_id   : 2;
      uint32_t sample    : 1;
      uint32_t reserved1 : 1;
      uint32_t rev_id    : 4;
      uint32_t pattern   : 4;
      uint32_t reserved  : 8;
    } bit;

  } max30001_info;

   /**
   * @brief CNFG_GEN (0x10) 
   */
  union max30001_cnfg_gen_reg {
    uint32_t all;
    struct {
      uint32_t rbiasn     : 1;
      uint32_t rbiasp     : 1;
      uint32_t rbiasv     : 2;
      uint32_t en_rbias   : 2;
      uint32_t vth        : 2;
      uint32_t imag       : 3;
      uint32_t ipol       : 1;
      uint32_t en_dcloff  : 2;
      uint32_t en_bloff   : 2;
      uint32_t reserved1  : 1;
      uint32_t en_pace    : 1;
      uint32_t en_bioz    : 1;
      uint32_t en_ecg     : 1;
      uint32_t fmstr      : 2;
      uint32_t en_ulp_lon : 2;
      uint32_t reserved : 8;
    } bit;

  } max30001_cnfg_gen;

  
   /**
   * @brief CNFG_CAL (0x12) 
   */  
  union max30001_cnfg_cal_reg {
    uint32_t all;
    struct {
      uint32_t thigh     : 11;
      uint32_t fifty     : 1;
      uint32_t fcal      : 3;
      uint32_t reserved1 : 5;
      uint32_t vmag      : 1;
      uint32_t vmode     : 1;
      uint32_t en_vcal   : 1;
      uint32_t reserved2 : 1;
      uint32_t reserved  : 8;
    } bit;

  } max30001_cnfg_cal;

   /**
   * @brief CNFG_EMUX  (0x14) 
   */
  union max30001_cnfg_emux_reg {
    uint32_t all;
    struct {
      uint32_t reserved1 : 16;
      uint32_t caln_sel  : 2;
      uint32_t calp_sel  : 2;
      uint32_t openn     : 1;
      uint32_t openp     : 1;
      uint32_t reserved2 : 1;
      uint32_t pol       : 1;
      uint32_t reserved : 8;
    } bit;

  } max30001_cnfg_emux;

  
   /**
   * @brief CNFG_ECG   (0x15) 
   */  
  union max30001_cnfg_ecg_reg {
    uint32_t all;
    struct {
      uint32_t reserved1 : 12;
      uint32_t dlpf      : 2;
      uint32_t dhpf      : 1;
      uint32_t reserved2 : 1;
      uint32_t gain      : 2;
      uint32_t reserved3 : 4;
      uint32_t rate      : 2;

      uint32_t reserved  : 8;
    } bit;

  } max30001_cnfg_ecg;

   /**
   * @brief CNFG_BMUX   (0x17) 
   */  
  union max30001_cnfg_bmux_reg {
    uint32_t all;
    struct {
      uint32_t fbist     : 2;
      uint32_t reserved1 : 2;
      uint32_t rmod      : 3;
      uint32_t reserved2 : 1;
      uint32_t rnom      : 3;
      uint32_t en_bist   : 1;
      uint32_t cg_mode   : 2;
      uint32_t reserved3 : 2;
      uint32_t caln_sel  : 2;
      uint32_t calp_sel  : 2;
      uint32_t openn     : 1;
      uint32_t openp     : 1;
      uint32_t reserved4 : 2;
      uint32_t reserved : 8;
    } bit;

  } max30001_cnfg_bmux;

   /**
   * @brief CNFG_BIOZ   (0x18) 
   */ 
  union max30001_bioz_reg {
    uint32_t all;
    struct {
      uint32_t phoff     : 4;
      uint32_t cgmag     : 3;
      uint32_t cgmon     : 1;
      uint32_t fcgen     : 4;
      uint32_t dlpf      : 2;
      uint32_t dhpf      : 2;
      uint32_t gain      : 2;
      uint32_t inapow_mode : 1;
      uint32_t ext_rbias : 1;
      uint32_t ahpf      : 3;
      uint32_t rate      : 1;
      uint32_t reserved : 8;
    } bit;

  } max30001_cnfg_bioz;

  
   /**
   * @brief CNFG_PACE   (0x1A) 
   */   
  union max30001_cnfg_pace_reg {
    uint32_t all;

    struct {
      uint32_t dacn        : 4;
      uint32_t dacp        : 4;
      uint32_t reserved1   : 4;
      uint32_t aout        : 2;
      uint32_t aout_lbw    : 1;
      uint32_t reserved2   : 1;
      uint32_t gain        : 3;
      uint32_t gn_diff_off : 1;
      uint32_t reserved3   : 3;
      uint32_t pol         : 1;
      uint32_t reserved    : 8;
    } bit;

  } max30001_cnfg_pace;

   /**
   * @brief CNFG_RTOR1   (0x1D) 
   */   
  union max30001_cnfg_rtor1_reg {
    uint32_t all;
    struct {
      uint32_t reserved1 : 8;
      uint32_t ptsf      : 4;
      uint32_t pavg      : 2;
      uint32_t reserved2 : 1;
      uint32_t en_rtor   : 1;
      uint32_t gain      : 4;
      uint32_t wndw      : 4;
      uint32_t reserved : 8;
    } bit;

  } max30001_cnfg_rtor1;

   /**
   * @brief CNFG_RTOR2 (0x1E) 
   */   
  union max30001_cnfg_rtor2_reg {
    uint32_t all;
    struct {
      uint32_t reserved1 : 8;
      uint32_t rhsf      : 3;
      uint32_t reserved2 : 1;
      uint32_t ravg      : 2;
      uint32_t reserved3 : 2;
      uint32_t hoff      : 6;
      uint32_t reserved4 : 2;
      uint32_t reserved : 8;
    } bit;

  } max30001_cnfg_rtor2;

/*********************************************************************************/
 // Data
  uint32_t max30001_ECG_FIFO_buffer[32]={0}; // (303 for internal test)
  uint32_t max30001_BIOZ_FIFO_buffer[8]; // (303 for internal test)

  uint32_t max30001_PACE[18]; // Pace Data 0-5

  uint32_t max30001_RtoR_data; // This holds the RtoR data

  uint32_t max30001_DCLeadOff; // This holds the LeadOff data, Last 4 bits give
                               // the status, BIT3=LOFF_PH, BIT2=LOFF_PL,
                               // BIT1=LOFF_NH, BIT0=LOFF_NL
                               // 8th and 9th bits tell Lead off is due to ECG or BIOZ.  
                               // 0b01 = ECG Lead Off and 0b10 = BIOZ Lead off

  uint32_t max30001_ACLeadOff; // This gives the state of the BIOZ AC Lead Off
                               // state.  BIT 1 = BOVER,   BIT 0 = BUNDR

  uint32_t max30001_bcgmon; // This holds the BCGMON data, BIT 1 = BCGMP, BIT0 =
                            // BCGMN

  uint32_t max30001_LeadOn; // This holds the LeadOn data, BIT1 = BIOZ Lead ON,
                            // BIT0 = ECG Lead ON, BIT8= Lead On Status Bit

  uint32_t max30001_timeout; // If the PLL does not respond, timeout and get out.

  
  max30001_t hspValMax30001; // R2R, FMSTR
	
  PtrFunction onDataAvailableCallback;
	//******************************************************************************
	
//SPI_HandleTypeDef hspi2;


//******************************************************************************
int max30001_Rbias_FMSTR_Init(uint8_t En_rbias, uint8_t Rbiasv,
                                        uint8_t Rbiasp, uint8_t Rbiasn,
                                        uint8_t Fmstr) {
  if (max30001_reg_read(CNFG_GEN, &max30001_cnfg_gen.all) == -1) {
    return -1;
  }
	else
	{
		//printf("max30001_cnfg_gen.bit.rbiasv = %d\n",max30001_cnfg_gen.bit.rbiasv);
	}
  max30001_cnfg_gen.bit.en_rbias = En_rbias;
  max30001_cnfg_gen.bit.rbiasv   = Rbiasv;
  max30001_cnfg_gen.bit.rbiasp   = Rbiasp;
  max30001_cnfg_gen.bit.rbiasn   = Rbiasn;
  max30001_cnfg_gen.bit.fmstr    = Fmstr;

  if (max30001_reg_write(CNFG_GEN, max30001_cnfg_gen.all) == -1) {
    return -1;
  }
	else
	{
		//printf("max30001_cnfg_gen.bit.rbiasv = %d\n",max30001_cnfg_gen.bit.rbiasv);
	}
	
	if (max30001_reg_read(CNFG_GEN, &max30001_cnfg_gen.all) == -1) {
    return -1;
  }
	else
	{
		//printf("max30001_cnfg_gen.bit.rbiasv = %d\n",max30001_cnfg_gen.bit.rbiasv);
	}
	
  return 0;
}

//******************************************************************************
int max30001_CAL_InitStart(uint8_t En_Vcal, uint8_t Vmode,
                                     uint8_t Vmag, uint8_t Fcal, uint16_t Thigh,
                                     uint8_t Fifty) {
  // CNFG_CAL
  if (max30001_reg_read(CNFG_CAL, &max30001_cnfg_cal.all) == -1) {
    return -1;
  }

  max30001_cnfg_cal.bit.vmode = Vmode;
  max30001_cnfg_cal.bit.vmag  = Vmag;
  max30001_cnfg_cal.bit.fcal  = Fcal;
  max30001_cnfg_cal.bit.thigh = Thigh;
  max30001_cnfg_cal.bit.fifty = Fifty;

  if (max30001_reg_write(CNFG_CAL, max30001_cnfg_cal.all) == -1) {
    return -1;
  }

  // RTOS uses a 32768HZ clock.  32768ticks represents 1secs.  1sec/10 =
  // 100msecs.
  //wait(1.0 / 10.0);

  if (max30001_reg_read(CNFG_CAL, &max30001_cnfg_cal.all) == -1) {
    return -1;
  }

  max30001_cnfg_cal.bit.en_vcal = En_Vcal;

  if (max30001_reg_write(CNFG_CAL, max30001_cnfg_cal.all) == -1) {
    return -1;
  }

  // RTOS uses a 32768HZ clock.  32768ticks represents 1secs.  1sec/10 =
  // 100msecs.
  //wait(1.0 / 10.0);

  return 0;
}

//******************************************************************************
int max30001_CAL_Stop(void) {

  if (max30001_reg_read(CNFG_CAL, &max30001_cnfg_cal.all) == -1) {
    return -1;
  }

  max30001_cnfg_cal.bit.en_vcal = 0; // Disable VCAL, all other settings are left unaffected

  if (max30001_reg_write(CNFG_CAL, max30001_cnfg_cal.all) == -1) {
    return -1;
  }

  return 0;
}
//******************************************************************************
//******************************************************************************
int max30001_INT_assignment(max30001_intrpt_Location_t en_enint_loc,     max30001_intrpt_Location_t en_eovf_loc,  max30001_intrpt_Location_t en_fstint_loc,
		                    max30001_intrpt_Location_t en_dcloffint_loc, max30001_intrpt_Location_t en_bint_loc,  max30001_intrpt_Location_t en_bovf_loc,
		                    max30001_intrpt_Location_t en_bover_loc,     max30001_intrpt_Location_t en_bundr_loc, max30001_intrpt_Location_t en_bcgmon_loc,
		                    max30001_intrpt_Location_t en_pint_loc,      max30001_intrpt_Location_t en_povf_loc,  max30001_intrpt_Location_t en_pedge_loc,
		                    max30001_intrpt_Location_t en_lonint_loc,    max30001_intrpt_Location_t en_rrint_loc, max30001_intrpt_Location_t en_samp_loc,
		                    max30001_intrpt_type_t  intb_Type,           max30001_intrpt_type_t int2b_Type)


{
  // INT1

  if (max30001_reg_read(EN_INT, &max30001_en_int.all) == -1) {
    return -1;
  }

  // max30001_en_int2.bit.en_pint       = 0b1;  // Keep this off...

  max30001_en_int.bit.en_eint = 1 & en_enint_loc;
  max30001_en_int.bit.en_eovf = 1 & en_eovf_loc;
  max30001_en_int.bit.en_fstint = 1 & en_fstint_loc;

  max30001_en_int.bit.en_dcloffint = 1 & en_dcloffint_loc;
  max30001_en_int.bit.en_bint = 1 & en_bint_loc;
  max30001_en_int.bit.en_bovf = 1 & en_bovf_loc;

  max30001_en_int.bit.en_bover = 1 & en_bover_loc;
  max30001_en_int.bit.en_bundr = 1 & en_bundr_loc;
  max30001_en_int.bit.en_bcgmon = 1 & en_bcgmon_loc;

  max30001_en_int.bit.en_pint = 1 & en_pint_loc;
  max30001_en_int.bit.en_povf = 1 & en_povf_loc;
  max30001_en_int.bit.en_pedge = 1 & en_pedge_loc;

  max30001_en_int.bit.en_lonint = 1 & en_lonint_loc;
  max30001_en_int.bit.en_rrint = 1 & en_rrint_loc;
  max30001_en_int.bit.en_samp = 1 & en_samp_loc;

  max30001_en_int.bit.intb_type = intb_Type;

  if (max30001_reg_write(EN_INT, max30001_en_int.all) == -1) {
    return -1;
  }

  // INT2

  if (max30001_reg_read(EN_INT2, &max30001_en_int2.all) == -1) {
    return -1;
  }

  max30001_en_int2.bit.en_eint   = 1 & (en_enint_loc >> 1);
  max30001_en_int2.bit.en_eovf   = 1 & (en_eovf_loc >> 1);
  max30001_en_int2.bit.en_fstint = 1 & (en_fstint_loc >> 1);

  max30001_en_int2.bit.en_dcloffint = 1 & (en_dcloffint_loc >> 1);
  max30001_en_int2.bit.en_bint      = 1 & (en_bint_loc >> 1);
  max30001_en_int2.bit.en_bovf      = 1 & (en_bovf_loc >> 1);

  max30001_en_int2.bit.en_bover  = 1 & (en_bover_loc >> 1);
  max30001_en_int2.bit.en_bundr  = 1 & (en_bundr_loc >> 1);
  max30001_en_int2.bit.en_bcgmon = 1 & (en_bcgmon_loc >> 1);

  max30001_en_int2.bit.en_pint  = 1 & (en_pint_loc >> 1);
  max30001_en_int2.bit.en_povf  = 1 & (en_povf_loc >> 1);
  max30001_en_int2.bit.en_pedge = 1 & (en_pedge_loc >> 1);

  max30001_en_int2.bit.en_lonint = 1 & (en_lonint_loc >> 1);
  max30001_en_int2.bit.en_rrint  = 1 & (en_rrint_loc >> 1);
  max30001_en_int2.bit.en_samp   = 1 & (en_samp_loc >> 1);

  max30001_en_int2.bit.intb_type = int2b_Type;

  if (max30001_reg_write(EN_INT2, max30001_en_int2.all) == -1) {
    return -1;
  }

  return 0;
}

//******************************************************************************
int max30001_ECG_InitStart(uint8_t En_ecg, uint8_t Openp,
                                     uint8_t Openn, uint8_t Pol,
                                     uint8_t Calp_sel, uint8_t Caln_sel,
                                     uint8_t E_fit, uint8_t Rate, uint8_t Gain,
                                     uint8_t Dhpf, uint8_t Dlpf) {

  // CNFG_EMUX

  if (max30001_reg_read(CNFG_EMUX, &max30001_cnfg_emux.all) == -1) {
    return -1;
  }
  printf("CNFG_EMUX register prije pisanja: %d\r\n",max30001_cnfg_emux.all);

  max30001_cnfg_emux.bit.openp    = Openp;
  max30001_cnfg_emux.bit.openn    = Openn;
  max30001_cnfg_emux.bit.pol      = Pol;
  max30001_cnfg_emux.bit.calp_sel = Calp_sel;
  max30001_cnfg_emux.bit.caln_sel = Caln_sel;

  if (max30001_reg_write(CNFG_EMUX, max30001_cnfg_emux.all) == -1) {
    return -1;
  }


  //*****************************************************
  if (max30001_reg_read(CNFG_EMUX, &max30001_cnfg_emux.all) == -1) {
     return -1;
   }
   printf("CNFG_EMUX register nakon pisanja: %d\r\n",max30001_cnfg_emux.all);

  /**** ENABLE CHANNELS ****/
  // CNFG_GEN

  if (max30001_reg_read(CNFG_GEN, &max30001_cnfg_gen.all) == -1) {
    return -1;
  }
  printf("CNFG_GEN register prije pisanja: %d\r\n",max30001_cnfg_gen.all);

  max30001_cnfg_gen.bit.en_ecg = En_ecg; // 1

  // fmstr is default

  if (max30001_reg_write(CNFG_GEN, max30001_cnfg_gen.all) == -1) {
    return -1;
  }

  //*****************************************************
  if (max30001_reg_read(CNFG_GEN, &max30001_cnfg_gen.all) == -1) {
     return -1;
   }
   printf("CNFG_GEN register nakon pisanja: %d\r\n",max30001_cnfg_gen.all);


  /**** Wait for PLL Lock & References to settle down ****/

  max30001_timeout = 0;

  do {
    if (max30001_reg_read(STATUS, &max30001_status.all) == -1) // Wait and spin for PLL to lock...
    {
      return -1;
    }
  } while (max30001_status.bit.pllint == 1 && max30001_timeout++ <= 1000);

  // MNGR_INT

  if (max30001_reg_read(MNGR_INT, &max30001_mngr_int.all) == -1) {
    return -1;
  }

  max30001_mngr_int.bit.e_fit = E_fit; // 31

  if (max30001_reg_write(MNGR_INT, max30001_mngr_int.all) == -1) {
    return -1;
  }

  // CNFG_ECG

  if (max30001_reg_read(CNFG_ECG, &max30001_cnfg_ecg.all) == -1) {
    return -1;
  }

  max30001_cnfg_ecg.bit.rate = Rate; 
  max30001_cnfg_ecg.bit.gain = Gain;
  max30001_cnfg_ecg.bit.dhpf = Dhpf;
  max30001_cnfg_ecg.bit.dlpf = Dlpf;

  if (max30001_reg_write(CNFG_ECG, max30001_cnfg_ecg.all) == -1) {
    return -1;
  }

  return 0;
}

//******************************************************************************
int max30001_ECGFast_Init(uint8_t Clr_Fast, uint8_t Fast, uint8_t Fast_Th) {
  if (max30001_reg_read(MNGR_INT, &max30001_mngr_int.all) == -1) {
    return -1;
  }

  max30001_mngr_int.bit.clr_fast = Clr_Fast;

  if (max30001_reg_write(MNGR_INT, max30001_mngr_int.all) == -1) {
    return -1;
  }

  if (max30001_reg_read(MNGR_DYN, &max30001_mngr_dyn.all) == -1) {
    return -1;
  }

  max30001_mngr_dyn.bit.fast = Fast;
  max30001_mngr_dyn.bit.fast_th = Fast_Th;

  if (max30001_reg_write(MNGR_INT, max30001_mngr_int.all) == -1) {
    return -1;
  }

  return 0;
}

//******************************************************************************
int max30001_Stop_ECG(void) {

  if (max30001_reg_read(CNFG_GEN, &max30001_cnfg_gen.all) == -1) {
    return -1;
  }

  max30001_cnfg_gen.bit.en_ecg = 0; // Stop ECG

  // fmstr is default

  if (max30001_reg_write(CNFG_GEN, max30001_cnfg_gen.all) == -1) {
    return -1;
  }

  return 0;
}

//******************************************************************************

//******************************************************************************
int max30001_Start_ECG(void) {

  if (max30001_reg_read(CNFG_GEN, &max30001_cnfg_gen.all) == -1) {
    return -1;
  }

  max30001_cnfg_gen.bit.en_ecg = 1; // Start ECG

  // fmstr is default

  if (max30001_reg_write(CNFG_GEN, max30001_cnfg_gen.all) == -1) {
    return -1;
  }

  return 0;
}

//******************************************************************************

//******************************************************************************
int max30001_BIOZ_InitStart(
    uint8_t En_bioz, uint8_t Openp, uint8_t Openn, uint8_t Calp_sel,
    uint8_t Caln_sel, uint8_t CG_mode, uint8_t B_fit, uint8_t Rate,
    uint8_t Ahpf, uint8_t Ext_rbias, uint8_t Gain, uint8_t Dhpf, uint8_t Dlpf,
    uint8_t Fcgen, uint8_t Cgmon, uint8_t Cgmag, uint8_t Phoff, uint8_t Inapow_mode) {

  // CNFG_BMUX

  if (max30001_reg_read(CNFG_BMUX, &max30001_cnfg_bmux.all) == -1) {
    return -1;
  }

  max30001_cnfg_bmux.bit.openp    = Openp;       // 1;
  max30001_cnfg_bmux.bit.openn    = Openn;       // 1;
  max30001_cnfg_bmux.bit.calp_sel = Calp_sel; // 0b10;
  max30001_cnfg_bmux.bit.caln_sel = Caln_sel; // 0b11;
  max30001_cnfg_bmux.bit.cg_mode  = CG_mode;   // 0b00;

  if (max30001_reg_write(CNFG_BMUX, max30001_cnfg_bmux.all) == -1) {
    return -1;
  }

  /**** SET MASTER FREQUENCY, ENABLE CHANNELS ****/

  // CNFG_GEN

  if (max30001_reg_read(CNFG_GEN, &max30001_cnfg_gen.all) == -1) {
    return -1;
  }

  max30001_cnfg_gen.bit.en_bioz = En_bioz;

  // fmstr is default

  if (max30001_reg_write(CNFG_GEN, max30001_cnfg_gen.all) == -1) {
    return -1;
  }

  /**** Wait for PLL Lock & References to settle down ****/

  max30001_timeout = 0;

  do {
    if (max30001_reg_read(STATUS, &max30001_status.all) ==
        -1) // Wait and spin for PLL to lock...
    {
      return -1;
    }

  } while (max30001_status.bit.pllint == 1 && max30001_timeout++ <= 1000);

  /**** Start of CNFG_BIOZ ****/

  // MNGR_INT

  if (max30001_reg_read(MNGR_INT, &max30001_mngr_int.all) == -1) {
    return -1;
  }

  max30001_mngr_int.bit.b_fit = B_fit; //;

  if (max30001_reg_write(MNGR_INT, max30001_mngr_int.all) == -1) {
    return -1;
  }

  // CNFG_BIOZ

  if (max30001_reg_read(CNFG_BIOZ, &max30001_cnfg_bioz.all) == -1) {
    return -1;
  }

  max30001_cnfg_bioz.bit.rate      = Rate;
  max30001_cnfg_bioz.bit.ahpf      = Ahpf;
  max30001_cnfg_bioz.bit.ext_rbias = Ext_rbias;
  max30001_cnfg_bioz.bit.gain      = Gain;
  max30001_cnfg_bioz.bit.dhpf      = Dhpf;
  max30001_cnfg_bioz.bit.dlpf      = Dlpf;
  max30001_cnfg_bioz.bit.fcgen     = Fcgen;
  max30001_cnfg_bioz.bit.cgmon     = Cgmon;
  max30001_cnfg_bioz.bit.cgmag     = Cgmag;
  max30001_cnfg_bioz.bit.phoff     = Phoff;
  max30001_cnfg_bioz.bit.inapow_mode = Inapow_mode;

  if (max30001_reg_write(CNFG_BIOZ, max30001_cnfg_bioz.all) == -1) {
    return -1;
  }

  return 0;
}

//******************************************************************************
int max30001_Stop_BIOZ(void) {

  if (max30001_reg_read(CNFG_GEN, &max30001_cnfg_gen.all) == -1) {
    return -1;
  }

  max30001_cnfg_gen.bit.en_bioz = 0; // Stop BIOZ

  if (max30001_reg_write(CNFG_GEN, max30001_cnfg_gen.all) == -1) {
    return -1;
  }

  return 0;
}

//******************************************************************************

//******************************************************************************
int max30001_Start_BIOZ(void) {

  if (max30001_reg_read(CNFG_GEN, &max30001_cnfg_gen.all) == -1) {
    return -1;
  }

  max30001_cnfg_gen.bit.en_bioz = 1; // Start BIOZ

  if (max30001_reg_write(CNFG_GEN, max30001_cnfg_gen.all) == -1) {
    return -1;
  }

  return 0;
}

//******************************************************************************
int max30001_BIOZ_InitBist(uint8_t En_bist, uint8_t Rnom,
                                     uint8_t Rmod, uint8_t Fbist) {

  // CNFG_BMUX

  if (max30001_reg_read(CNFG_BMUX, &max30001_cnfg_bmux.all) == -1) {
    return -1;
  }

  max30001_cnfg_bmux.bit.en_bist = En_bist;
  max30001_cnfg_bmux.bit.rnom = Rnom;
  max30001_cnfg_bmux.bit.rmod = Rmod;
  max30001_cnfg_bmux.bit.fbist = Fbist;

  if (max30001_reg_write(CNFG_BMUX, max30001_cnfg_bmux.all) == -1) {
    return -1;
  }

  return 0;
}
//******************************************************************************
int max30001_RtoR_InitStart(uint8_t En_rtor, uint8_t Wndw,
                                      uint8_t Gain, uint8_t Pavg, uint8_t Ptsf,
                                      uint8_t Hoff, uint8_t Ravg, uint8_t Rhsf,
                                      uint8_t Clr_rrint) {

  // MNGR_INT

  if (max30001_reg_read(MNGR_INT, &max30001_mngr_int.all) == -1) {
    return -1;
  }

  max30001_mngr_int.bit.clr_rrint =
      Clr_rrint; // 0b01 & 0b00 are for interrupt mode...
  // 0b10 is for monitoring mode... it just overwrites the data...

  if (max30001_reg_write(MNGR_INT, max30001_mngr_int.all) == -1) {
    return -1;
  }

  // RTOR1
  if (max30001_reg_read(CNFG_RTOR1, &max30001_cnfg_rtor1.all) == -1) {
    return -1;
  }

  max30001_cnfg_rtor1.bit.wndw = Wndw;
  max30001_cnfg_rtor1.bit.gain = Gain;
  max30001_cnfg_rtor1.bit.en_rtor = En_rtor;
  max30001_cnfg_rtor1.bit.pavg = Pavg;
  max30001_cnfg_rtor1.bit.ptsf = Ptsf;

  if (max30001_reg_write(CNFG_RTOR1, max30001_cnfg_rtor1.all) == -1) {
    return -1;
  }
  // RTOR2

  if (max30001_reg_read(CNFG_RTOR2, &max30001_cnfg_rtor2.all) == -1) {
    return -1;
  }
  max30001_cnfg_rtor2.bit.hoff = Hoff;
  max30001_cnfg_rtor2.bit.ravg = Ravg;
  max30001_cnfg_rtor2.bit.rhsf = Rhsf;

  if (max30001_reg_write(CNFG_RTOR2, max30001_cnfg_rtor2.all) == -1) {
    return -1;
  }

  return 0;
}

//******************************************************************************
int max30001_Stop_RtoR(void) {

  if (max30001_reg_read(CNFG_RTOR1, &max30001_cnfg_rtor1.all) == -1) {
    return -1;
  }

  max30001_cnfg_rtor1.bit.en_rtor = 0; // Stop RtoR

  if (max30001_reg_write(CNFG_RTOR1, max30001_cnfg_rtor1.all) == -1) {
    return -1;
  }

  return 0;
}

//******************************************************************************
//******************************************************************************
int max30001_Start_RtoR(void) {

  if (max30001_reg_read(CNFG_RTOR1, &max30001_cnfg_rtor1.all) == -1) {
    return -1;
  }

  max30001_cnfg_rtor1.bit.en_rtor = 1; // Start RtoR

  if (max30001_reg_write(CNFG_RTOR1, max30001_cnfg_rtor1.all) == -1) {
    return -1;
  }

  return 0;
}

//******************************************************************************
int max30001_PLL_lock(void) {
  // Spin to see PLLint become zero to indicate a lock.

  max30001_timeout = 0;

  do {
    if (max30001_reg_read(STATUS, &max30001_status.all) ==
        -1) // Wait and spin for PLL to lock...
    {
      return -1;
    }

  } while (max30001_status.bit.pllint == 1 && max30001_timeout++ <= 1000);

  return 0;
}

//******************************************************************************
int max30001_sw_rst(void) {
  // SW reset for the MAX30001 chip

  if (max30001_reg_write(SW_RST, 0x000000) == -1) {
    return -1;
  }

  return 0;
}

//******************************************************************************
int max30001_synch(void) { // For synchronization
  if (max30001_reg_write(SYNCH, 0x000000) == -1) {
    return -1;
  }
  return 0;
}

//******************************************************************************
int max300001_fifo_rst(void) { // Resets the FIFO
  if (max30001_reg_write(FIFO_RST, 0x000000) == -1) {
    return -1;
  }
  return 0;
}

//******************************************************************************
int max30001_reg_write(MAX30001_REG_map_t addr, uint32_t data) {

  uint8_t result[4];
  uint8_t data_array[4];
  int32_t success = 99;

  data_array[0] = (addr << 1) & 0xff;

  data_array[3] = data & 0xff;
  data_array[2] = (data >> 8) & 0xff;
  data_array[1] = (data >> 16) & 0xff;

  //success = SPI_Transmit(&data_array[0], 4, &result[0], 4);
	
	HAL_GPIO_WritePin(GPIOB, NSS_SPI2_Pin, GPIO_PIN_RESET); //CS pin low
  success = HAL_SPI_TransmitReceive(&hspi2, &data_array[0], &result[0], 4, 1000);
	HAL_GPIO_WritePin(GPIOB, NSS_SPI2_Pin, GPIO_PIN_SET); //CS pin high
	
	//printf("reg_write_success = %d\n", success);
  if (success != 0) {
    return -1;
  } else {
    return 0;
  }
}

//******************************************************************************
int max30001_reg_read(MAX30001_REG_map_t addr, uint32_t *return_data) {
	
  uint8_t result[4];
  uint8_t data_array[1];
  int32_t success = 99;

  data_array[0] = ((addr << 1) & 0xff) | 1; // For Read, Or with 1
	
	 //success = SPI_Transmit(&data_array[0], 1, &result[0], 4);
	
	HAL_GPIO_WritePin(GPIOB, NSS_SPI2_Pin, GPIO_PIN_RESET); //CS pin low
	success = HAL_SPI_TransmitReceive(&hspi2, &data_array[0], &result[0], 4, 1000);
	HAL_GPIO_WritePin(GPIOB, NSS_SPI2_Pin, GPIO_PIN_SET); //CS pin high
	
  *return_data = /*result[0] + */ (uint32_t)(result[1] << 16) + (result[2] << 8) + result[3]; // result[0] je zakomentiran...za�to?
 // printf("reg_read_success = %d\n", success);
	if (success != 0) {
    return -1;
  } else {
    return 0;
  }
}


//******************************************************************************
int max30001_BIOZ_Enable_BCGMON(void) {
  // CNFG_BIOZ
  if (max30001_reg_read(CNFG_BIOZ, &max30001_cnfg_bioz.all) == -1) {
    return -1;
  }

  max30001_cnfg_bioz.bit.cgmon = 1;

  if (max30001_reg_write(CNFG_BIOZ, max30001_cnfg_bioz.all) == -1) {
    return -1;
  }

  max30001_reg_read(CNFG_BIOZ, &max30001_cnfg_bioz.all);

  return 0;
}

#if 1
//******************************************************************************

//******************************************************************************

#endif
//******************************************************************************


//******************************************************************************
int max30001_FIFO_LeadONOff_Read() {

  uint8_t result[32 * 3]; // 32words - 3bytes each
  uint8_t data_array[4];
  int32_t success = 0;
  int i, j;

  uint32_t total_databytes;
  uint8_t i_index;
  uint8_t data_chunk;
  uint8_t loop_logic;

  uint8_t etag, ptag, btag;

  uint8_t adr;

  int8_t ret_val;

  if (max30001_status.bit.eint == 1) {    //fifo je pun
		//printf("FIFO je pun\n");
    adr = ECG_FIFO_BURST;
    data_array[0] = ((adr << 1) & 0xff) | 1;

    // The SPI routine only sends out data of 32 bytes in size.  Therefore the
    // data is being read in
    // smaller chunks in this routine...

    total_databytes = (max30001_mngr_int.bit.e_fit + 1) * 3;  //ECG FIFO Interrupt Threshold (issues EINT based on number of unread FIFO records)00000 to 11111 = 1 to 32,

    i_index = 0;
    loop_logic = 1;

    while (loop_logic) {
      if (total_databytes > 30) {
        data_chunk = 30;
        total_databytes = total_databytes - 30;
      } else {
        data_chunk = total_databytes;
        loop_logic = 0;
      }

      /* The extra 1 byte is for the extra byte that comes out of the SPI */
      //success = SPI_Transmit(&data_array[0], 1, &result[i_index], (data_chunk + 1)); // Make a copy of the FIFO over here...
			HAL_GPIO_WritePin(GPIOB, NSS_SPI2_Pin, GPIO_PIN_RESET); //CS pin low
      success = HAL_SPI_TransmitReceive(&hspi2, &data_array[0], &result[i_index], (data_chunk + 1), 1000);
			HAL_GPIO_WritePin(GPIOB, NSS_SPI2_Pin, GPIO_PIN_SET); //CS pin high
			
      if (success != 0) {
        return -1;
      }

      /* This is important, because every transaction above creates an empty
       * redundant data at result[0] */
      for (j = i_index; j < (data_chunk + i_index); j++) /* get rid of the 1 extra byte by moving the whole array up one */
      {
        result[j] = result[j + 1];
      }

      i_index = i_index + 30; /* point to the next array location to put the data in */
    }

    //ReadAllPaceOnce = 0;

    /* Put the content of the FIFO based on the EFIT value, We ignore the
     * result[0] and start concatenating indexes: 1,2,3 - 4,5,6 - 7,8,9 -  */
    for (i = 0, j = 0; i < max30001_mngr_int.bit.e_fit + 1; i++, j = j + 3) // index1=23-16 bit, index2=15-8 bit, index3=7-0 bit
    {
      max30001_ECG_FIFO_buffer[i] = ((uint32_t)result[j] << 16) + (result[j + 1] << 8) + result[j + 2];
			
			//printf("ECG_FIFO_buffer[%d] = %d\n\r",i,max30001_ECG_FIFO_buffer[i]);
			
      etag = (0x34 & result[j + 2]) >> 3; //0b00111000
      ptag = 0x07 & result[j + 2];        //0b00000111

      if (ptag != 7 ){//&& ReadAllPaceOnce == 0) {  //0b111

        //ReadAllPaceOnce = 1; // This will prevent extra read of PACE, once group
                             // 0-5 is read ONCE.
	  	//readPace(ptag, paceResult); // BUG: result data from ECG is being overwritten by the PACE data
      	//anyPaceDetected = true;
      }
    }
    
   // if (anyPaceDetected)
   //   dataAvailable(MAX30001_DATA_PACE, max30001_PACE, 18); // Send out the Pace data once only

  /*  if (etag != 6) { //0b110

      dataAvailable(MAX30001_DATA_ECG, max30001_ECG_FIFO_buffer, (max30001_mngr_int.bit.e_fit + 1));
    }
*/
    ret_val = 0;
  } 
	/* End of ECG init */

	
	
	
  /* RtoR */

  if (max30001_status.bit.rrint == 1) {
	  //printf("reading RTOR register\r\n");
    if (max30001_reg_read(RTOR, &max30001_RtoR_data) == -1) {
      //printf("RTOR register read failed\r\n");
      return -1;
    }

    max30001_RtoR_data = (0x00FFFFFF & max30001_RtoR_data) >> 10;
   // printf("RTOR data = %d\r\n",max30001_RtoR_data);
    hspValMax30001.R2R = (uint16_t)max30001_RtoR_data;
    hspValMax30001.fmstr = (uint16_t)max30001_cnfg_gen.bit.fmstr;

    dataAvailable(MAX30001_DATA_RTOR, &max30001_RtoR_data, 1);
    ret_val = 1;
  }

  // Handling BIOZ data...

  if (max30001_status.bit.bint == 1) {
    adr = 0x22;
    data_array[0] = ((adr << 1) & 0xff) | 1;

    /* [(BFIT+1)*3byte]+1extra byte due to the addr */
   //success = HAL_SPI_TransmitReceive(&hspi2, &data_array[0], &result[i_index], (data_chunk + 1), 0);
    //if (SPI_Transmit(&data_array[0], 1, &result[0],((max30001_mngr_int.bit.b_fit + 1) * 3) + 1) == -1) // Make a copy of the FIFO over here...
    HAL_GPIO_WritePin(GPIOB, NSS_SPI2_Pin, GPIO_PIN_RESET); //CS pin low
    success = HAL_SPI_TransmitReceive(&hspi2, &data_array[0], &result[0], (((max30001_mngr_int.bit.b_fit + 1) * 3) + 1), 1000);  // Make a copy of the FIFO over here...
	HAL_GPIO_WritePin(GPIOB, NSS_SPI2_Pin, GPIO_PIN_SET); //CS pin high
    		//tu su razlicite duljine transmit i receive dijelova???
    if (success != 0) {
            return -1;
          }


    btag = 0x07 & result[3]; //0b00000111

    /* Put the content of the FIFO based on the BFIT value, We ignore the
     * result[0] and start concatenating indexes: 1,2,3 - 4,5,6 - 7,8,9 -  */
    for (i = 0, j = 0; i < max30001_mngr_int.bit.b_fit + 1; i++, j = j + 3) // index1=23-16 bit, index2=15-8 bit, index3=7-0 bit
    {
      max30001_BIOZ_FIFO_buffer[i] = ((uint32_t)result[j + 1] << 16) + (result[j + 2] << 8) + result[j + 3];
    }

    if (btag != 6) { //0b110
      dataAvailable(MAX30001_DATA_BIOZ, max30001_BIOZ_FIFO_buffer, 8);
    }
    ret_val = 0;
  }


/*
 if (max30001_status.bit.dcloffint == 1) // ECG/BIOZ Lead Off
  {
    dcloffint_OneShot = 1;
    max30001_DCLeadOff = 0;
    max30001_DCLeadOff = max30001_DCLeadOff | (max30001_cnfg_gen.bit.en_dcloff << 8) | (max30001_status.all & 0x00000F);
    dataAvailable(MAX30001_DATA_LEADOFF_DC, &max30001_DCLeadOff, 1);

    ret_val = 0b100;

  } else if (dcloffint_OneShot == 1 && max30001_status.bit.dcloffint == 0) // Just send once when it comes out of dc lead off
  {
    max30001_DCLeadOff = 0;
    max30001_DCLeadOff = max30001_DCLeadOff | (max30001_cnfg_gen.bit.en_dcloff << 8) | (max30001_status.all & 0x00000F);
    dataAvailable(MAX30001_DATA_LEADOFF_DC, &max30001_DCLeadOff, 1);
    dcloffint_OneShot = 0;
  }

  if (max30001_status.bit.bover == 1 || max30001_status.bit.bundr == 1) // BIOZ AC Lead Off
  {
    acloffint_OneShot = 1;
    max30001_ACLeadOff = 0;
    max30001_ACLeadOff =
        max30001_ACLeadOff | ((max30001_status.all & 0x030000) >> 16);
    dataAvailable(MAX30001_DATA_LEADOFF_AC, &max30001_ACLeadOff, 1);

    ret_val = 0b1000;
  } else if (acloffint_OneShot == 1 && max30001_status.bit.bover == 0 && max30001_status.bit.bundr == 0) // Just send once when it comes out of ac lead off
  {
    max30001_ACLeadOff = 0;
    max30001_ACLeadOff = max30001_ACLeadOff | ((max30001_status.all & 0x030000) >> 16);
    dataAvailable(MAX30001_DATA_LEADOFF_AC, &max30001_ACLeadOff, 1);
    acloffint_OneShot = 0;
  }
*/

/*
  if (max30001_status.bit.bcgmon == 1) // BIOZ BCGMON check
  {
    bcgmon_OneShot = 1;
    max30001_bcgmon = 0;
    max30001_bcgmon = max30001_bcgmon | ((max30001_status.all & 0x000030) >> 4);
    dataAvailable(MAX30001_DATA_BCGMON, &max30001_bcgmon, 1);

    ret_val = 0b10000;
  } else if (bcgmon_OneShot == 1 && max30001_status.bit.bcgmon == 0) {
    max30001_bcgmon = 0;
    max30001_bcgmon = max30001_bcgmon | ((max30001_status.all & 0x000030) >> 4);
    bcgmon_OneShot = 0;
    dataAvailable(MAX30001_DATA_BCGMON, &max30001_bcgmon, 1);
  }
*/
#if 0
if(max30001_status.bit.lonint == 1)   // AC LeadON Check
{
	max30001_LeadOn = 0;
	max30001_reg_read(STATUS,&max30001_status.all);        // Reading is important
	max30001_LeadOn = max30001_LeadOn | (max30001_cnfg_gen.bit.en_ulp_lon << 8) | ((max30001_status.all & 0x000800) >> 11);  // 0b01 will mean ECG Lead On, 0b10 will mean BIOZ Lead On
    // LEAD ON has been detected... Now take actions
}
#endif
/*
  if (max30001_status.bit.lonint == 1 &&
      acleadon_OneShot == 0) // AC LeadON Check, when lead is on
  {
    max30001_LeadOn = 0;
    max30001_reg_read(STATUS, &max30001_status.all); // Reading is important
    max30001_LeadOn =
        max30001_LeadOn | (max30001_cnfg_gen.bit.en_ulp_lon << 8) |
        ((max30001_status.all & 0x000800) >>
         11); // 0b01 will mean ECG Lead On, 0b10 will mean BIOZ Lead On

    // LEAD ON has been detected... Now take actions
    acleadon_OneShot = 1;
    dataAvailable(MAX30001_DATA_ACLEADON, &max30001_LeadOn, 1); // One shot data will be sent...
  } else if (max30001_status.bit.lonint == 0 && acleadon_OneShot == 1) {
    max30001_LeadOn = 0;
    max30001_reg_read(STATUS, &max30001_status.all);
    max30001_LeadOn =
        max30001_LeadOn | (max30001_cnfg_gen.bit.en_ulp_lon << 8) | ((max30001_status.all & 0x000800) >> 11); // 0b01 will mean ECG Lead On, 0b10 will mean BIOZ Lead On
    dataAvailable(MAX30001_DATA_ACLEADON, &max30001_LeadOn, 1); // One shot data will be sent...
    acleadon_OneShot = 0;
  }
*/
  return ret_val;
}

//******************************************************************************


 
//******************************************************************************

//******************************************************************************

int max30001_int_handler() {

  static uint32_t InitReset = 0;
  int8_t return_value;
  //bool check_one_more = true;

  //status_check:
  max30001_reg_read(STATUS, &max30001_status.all);
  //printf("status register: %d\r\n",max30001_status.all);
  // Inital Reset and any FIFO over flow invokes a FIFO reset
  // if (InitReset == 0 || max30001_status.bit.eovf == 1 || max30001_status.bit.bovf == 1) 
	if (InitReset == 0 || max30001_status.bit.eovf == 1) { //bez BioZ overflow za sada
 // if (InitReset == 0 ) { //bez BioZ i ECG overflow za sada
    // Do a FIFO Reset
    max30001_reg_write(FIFO_RST, 0x000000);
	printf("FIFO rst %d\r\n", InitReset);
    InitReset++;
    return 2;
  }

  return_value = 0;

  // The four data handling goes on over here
  if (max30001_status.bit.eint == 1 || max30001_status.bit.pint == 1 || max30001_status.bit.bint == 1 || max30001_status.bit.rrint == 1
  	|| max30001_status.bit.dcloffint == 1 || max30001_status.bit.bover == 1 || max30001_status.bit.bundr == 1
  	|| max30001_status.bit.bcgmon == 1 || max30001_status.bit.lonint == 1) {
    return_value = return_value | max30001_FIFO_LeadONOff_Read();
  }
/*
  // ECG/BIOZ DC Lead Off test
  if (max30001_status.bit.dcloffint == 1) {
    return_value = return_value | max30001_FIFO_LeadONOff_Read();
  }

  // BIOZ AC Lead Off test
  if (max30001_status.bit.bover == 1 || max30001_status.bit.bundr == 1) {
    return_value = return_value | max30001_FIFO_LeadONOff_Read();
  }

  // BIOZ DRVP/N test using BCGMON.
  if (max30001_status.bit.bcgmon == 1) {
    return_value = return_value | max30001_FIFO_LeadONOff_Read();
  }

  if (max30001_status.bit.lonint == 1) // ECG Lead ON test: i.e. the leads are touching the body...
  {

    max30001_FIFO_LeadONOff_Read();
  }

  if (check_one_more) {
    check_one_more = false;
    goto status_check;
  }
	*/
	
  return return_value;
}


/// function pointer to the async callback
//static event_callback_t functionpointer;
/// flag used to indicate an async xfer has taken place
static volatile int xferFlag = 0;

/**
* @brief Callback handler for SPI async events
* @param events description of event that occurred
*/


//static void spiHandler(int events) { xferFlag = 1; }

/**
* @brief Transmit and recieve QUAD SPI data
* @param tx_buf pointer to transmit byte buffer
* @param tx_size number of bytes to transmit
* @param rx_buf pointer to the recieve buffer
* @param rx_size number of bytes to recieve
*/
//HAL_StatusTypeDef HAL_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout)
//HAL_StatusTypeDef HAL_SPI_TransmitReceive(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout)
/*
int SPI_Transmit(const uint8_t *tx_buf, uint32_t tx_size, uint8_t *rx_buf, uint32_t rx_size)
{
    m_cs->write(0);
    for(int i = 0; i < tx_size; i++) 
    {
        m_spi->write(tx_buf[i]);
    }
    for(int i = 0; i < (rx_size - tx_size); i++) 
    {
    	rx_buf[i + 1] = m_spi->write(0xFF);
    }
    m_cs->write(1);

    return 0;
}
*/
//******************************************************************************
void max30001_ReadHeartrateData(max30001_t *_hspValMax30001) {
  _hspValMax30001->R2R = hspValMax30001.R2R;
  _hspValMax30001->fmstr = hspValMax30001.fmstr;
}

//******************************************************************************
void onDataAvailable(PtrFunction _onDataAvailable) {
  onDataAvailableCallback = _onDataAvailable;
}

/**
* @brief Used to notify an external function that interrupt data is available
* @param id type of data available
* @param buffer 32-bit buffer that points to the data
* @param length length of 32-bit elements available
*/
void dataAvailable(uint32_t id, uint32_t *buffer, uint32_t length) {
  if (onDataAvailableCallback != NULL) {
    (*onDataAvailableCallback)(id, buffer, length);
  }
}

/**
* @brief Callback handler for SPI async events
* @param events description of event that occurred
*/
//void spiHandler(int events) { xferFlag = 1; }   //to sam zakomentirao jer je javljalo da je to redefinition od spiHanfler

//******************************************************************************
static int allowInterrupts = 0;

/*void MAX30001Mid_IntB_Handler(void) {
  if (allowInterrupts == 0) return;
  max30001_int_handler();
}

void MAX30001Mid_Int2B_Handler(void) {
  if (allowInterrupts == 0) return;
  max30001_int_handler();
}
*/
void MAX30001_AllowInterrupts(int state) { 
allowInterrupts = state; 
}
//*********************************************************************************
//User defined default initialisation function
void MAX30001_init(void) {

	MAX30001_AllowInterrupts(1);			//allow interrupts

	if(max30001_sw_rst()== -1)  			// Do a software reset of the MAX30001
	printf("sw reset failed\r\n");
	else
	printf("sw reset done\r\n");

	//This function handles the assignment of the two interrupt pins (INTB & INT2B)

	  		max30001_intrpt_Location_t en_enint_loc = MAX30001_INT_B;   	// ECG FIFO Interrupt -> INT_B
	  		max30001_intrpt_Location_t en_eovf_loc = MAX30001_INT_B;    	// ECG FIFO Overflow -> INT_B
	    	max30001_intrpt_Location_t en_fstint_loc = MAX30001_NO_INT;
	  		max30001_intrpt_Location_t en_dcloffint_loc = MAX30001_NO_INT;
	  		max30001_intrpt_Location_t en_bint_loc = MAX30001_INT_2B;   	// BioZ FIFO Interrupt -> INT_2B
	  		max30001_intrpt_Location_t en_bovf_loc = MAX30001_INT_2B;   	// BioZ FIFO Overflow -> INT_2B
	  		max30001_intrpt_Location_t en_bover_loc = MAX30001_NO_INT;
	  		max30001_intrpt_Location_t en_bundr_loc = MAX30001_NO_INT;
	  		max30001_intrpt_Location_t en_bcgmon_loc = MAX30001_NO_INT;
	  	    max30001_intrpt_Location_t en_pint_loc = MAX30001_NO_INT;
	  		max30001_intrpt_Location_t en_povf_loc = MAX30001_NO_INT;
	  		max30001_intrpt_Location_t en_pedge_loc = MAX30001_NO_INT;
	  		max30001_intrpt_Location_t en_lonint_loc = MAX30001_NO_INT;
	        max30001_intrpt_Location_t en_rrint_loc = MAX30001_INT_B;    	// ECG R-to-R Detector R Event Interrupt -> INT_B
	  		max30001_intrpt_Location_t en_samp_loc = MAX30001_NO_INT;
	  		max30001_intrpt_type_t  intb_Type = MAX30001_INT_ODNR; 			//Open-Drain NMOS Driver with Internal 125kO Pullup Resistance for INTB
	  		max30001_intrpt_type_t int2b_Type = MAX30001_INT_ODNR; 			//Open-Drain NMOS Driver with Internal 125kO Pullup Resistance for INT2B

	  	  if(max30001_INT_assignment(en_enint_loc, en_eovf_loc, en_fstint_loc,
	  								 en_dcloffint_loc, en_bint_loc,  en_bovf_loc,
	  								 en_bover_loc,     en_bundr_loc, en_bcgmon_loc,
	  								 en_pint_loc,      en_povf_loc,  en_pedge_loc,
	  								 en_lonint_loc,    en_rrint_loc, en_samp_loc,
	  								 intb_Type,        int2b_Type) == -1)
	  	  printf("INT assigment failed\r\n");
	      else
	  	  printf("INT assigment done\r\n");


	 //This function sets up the MAX30001 for the ECG measurements.

	  	  	uint8_t En_ecg = MAX30001_CHANNEL_ENABLED;                  //ECG channel enabled
	  	  	uint8_t Openp = MAX30001_ECG_CON_AFE;                       //ECGP Input Switch internally connected
	  	  	uint8_t Openn = MAX30001_ECG_CON_AFE;                       //ECGN Input Switch internally connected
	  	  	uint8_t Pol = MAX30001_NON_INV;                       		//ECG Input Polarity -> non-inverted
	  	  	uint8_t Calp_sel = MAX30001_NO_CAL_SIG;                		//ECGP Calibration Selection -> No calibration signal applied
	  	  	uint8_t Caln_sel = MAX30001_NO_CAL_SIG;                		//ECGN Calibration Selection -> No calibration signal applied
	  	  	uint8_t E_fit = 0xf; 										//ECG FIFO Interrupt Threshold -> 16 (e_fit + 1)
	  	  	uint8_t Rate_ecg = 2; 										//ECG sample rate -> 128sps (0b10)
	  	  	uint8_t Gain_ecg = 0; 										//ECG Channel Gain Setting -> 20V/V (0b00)
	  	  	uint8_t Dhpf_ecg = 1; 										//ECG Channel Digital High-Pass Filter Cutoff Frequency -> 0.5Hz (0b1)
	  	  	uint8_t Dlpf_ecg = 1;  										//ECG Channel Digital Low-Pass Filter Cutoff Frequency -> 40 Hz (28.35 Hz for 128 sps)

	  	  if(max30001_ECG_InitStart(En_ecg, Openp, Openn,
	  	                            Pol,  Calp_sel,  Caln_sel,
	  	                            E_fit,  Rate_ecg,  Gain_ecg,
	  	                            Dhpf_ecg,  Dlpf_ecg) == -1)
	  	  printf("ECG_Init failed\r\n");
	  	  else
	  	  printf("ECG_Init done\r\n");

	 //This function sets up the MAX30001 for BIOZ measurement.

	  	  	uint8_t CG_mode = 1; 										//BioZ Current Generator Mode Selection - > Chopped Sources without Low Pass Filter
	  	  	uint8_t B_fit = 7; 										    //BioZ FIFO Interrupt Threshold - > 8 (B_fit + 1)
	  	  	uint8_t Rate_bioZ = 0; 										//BioZ Data Rate -> 64 sps if fmstr=32.768hz
	  	  	uint8_t Ahpf = 2; 											//BioZ/PACE Channel Analog High-Pass Filter Cutoff Frequency and Bypass -> 800 Hz
	  	  	uint8_t Ext_rbias = 0; 										//External Resistor Bias Enable -> Internal Bias Generator used
	  	  	uint8_t Gain_bioZ = 1; 										//BioZ Channel Gain Setting -> 20V/V
	  	  	uint8_t Dhpf_bioZ = 0; 										//BioZ Channel Digital High-Pass Filter Cutoff Frequency -> 00 = Bypass (DC)
	  	  	uint8_t Dlpf_bioZ = 0; 										//BioZ Channel Digital Low-Pass Filter Cutoff Frequency -> 00 = Bypass (Decimation only, no FIR filter)
	  	  	uint8_t Fcgen = 0; 											//BioZ Current Generator Modulation Frequency -> 0000 = 4*fMSTR (approximately 128kHz)
	  	  	uint8_t Cgmon = 0; 											//BioZ Current Generator Monitor - > disabled
	  	  	uint8_t Cgmag = 7; 											//BioZ Current Generator Magnitude -> 96uA
	  	  	uint8_t Phoff = 0; 											//BioZ Current Generator Modulation Phase Offset -> 0
	  	  	uint8_t Inapow_mode = 0; 									//BioZ Channel Instrumentation Amplifier (INA) Power Mode -> BioZ INA is in low power mode

	  	  if(max30001_BIOZ_InitStart(MAX30001_CHANNEL_ENABLED, MAX30001_ECG_CON_AFE, MAX30001_ECG_CON_AFE,
	  	                             MAX30001_NO_CAL_SIG, MAX30001_NO_CAL_SIG,
	  	                             CG_mode, /* uint8_t En_bioz,*/  B_fit, Rate_bioZ,
	  	                             Ahpf, Ext_rbias, Gain_bioZ,
	  	                             Dhpf_bioZ, Dlpf_bioZ,  Fcgen,
	  	                             Cgmon,  Cgmag,  Phoff, Inapow_mode) == -1)
	  	  printf("BIOZ_Init failed\r\n");
	  	  else
	  	  printf("BIOZ_Init done\r\n");


	 //This function sets up the Resistive Bias mode and also selects the master clock frequency.

	  	  	uint8_t En_rbias = 1; 										//Enable and Select Resistive Lead Bias Mode -> ECG Resistive Bias enabled if EN_ECG is also enabled
	  	  	uint8_t Rbiasv = 1; 										//Resistive Bias Mode Value Selection -> RBIAS = 100MOhm
	  	  	uint8_t Rbiasp = 1; 										//Enables Resistive Bias on Positive Input -> ECGP/BIP is connected to VMID through a resistor (selected by RBIASV).
	  	  	uint8_t Rbiasn = 1; 										//Enables Resistive Bias on Negative Input -> ECGN/BIN is connected to VMID through a resistor (selected by RBIASV).
	  	  	uint8_t Fmstr = 0;  										//Master Clock Frequency. Selects the Master Clock Frequency -> 32.768 hz

	  	  if(max30001_Rbias_FMSTR_Init(En_rbias, Rbiasv, Rbiasp, Rbiasn, Fmstr) == -1)
	  	  printf("rbias and fmstr failed\r\n");
	  	  else
	  	  printf("rbias and fmstr done\r\n");


	 //Sets up the device for RtoR measurement

	  	  	uint8_t Wndw = 3; 											//This is the width of the averaging window, which adjusts the algorithm sensitivity to the width of the QRS complex -> 0011 = 12 x RTOR_RES (default = 96ms)
	  	  	uint8_t Gain_r2r = 15;										//R-to-R Gain -> autoscale (initial gain set to 64); 2^RGAIN[3:0], plus an auto-scale option
	  	  	uint8_t Pavg = 2; 											//R-to-R Peak Averaging Weight Factor -> 8
	  	  	uint8_t Ptsf = 3; 											//R-to-R Peak Threshold Scaling Factor -> 4/16
	  	  	uint8_t Hoff = 32;											//R-to-R Minimum Hold Off -> 32
	  	  	uint8_t Ravg = 2; 											//R-to-R Interval Averaging Weight Factor -> 8
	  	  	uint8_t Rhsf = 4; 											//R-to-R Interval Hold Off Scaling Factor -> 4/8
	  	  	uint8_t Clr_rrint = 1; 										//RTOR R Detect Interrupt (RRINT) Clear Behavior -> Clear RRINT on RTOR Register Read Back

	  	  if(max30001_RtoR_InitStart(MAX30001_CHANNEL_ENABLED, Wndw, Gain_r2r,
	  	                             Pavg, Ptsf, Hoff,
	  	                             Ravg, Rhsf, Clr_rrint) == -1)
	  	  printf("R2R_init failed\r\n");
	  	  else
	  	  printf("R2R_init done\r\n");


//	  	if(max30001_reg_read(EN_INT, &max30001_en_int.all) == -1)		printf("read failed\r\n");
//	  	else 															printf("EN_INT register value = %d\r\n", (int) max30001_en_int.all);
//	  	if(max30001_reg_read(CNFG_GEN, &max30001_cnfg_gen.all) == -1)	printf("read failed\r\n");
//	  	else 															printf("CNFG_GEN register value = %d\r\n", (int) max30001_cnfg_gen.all);
//
//	  	if (max30001_reg_write(EN_INT, 2) == -1)						printf("write failed\r\n");
//
//	  	if (max30001_reg_read(EN_INT, &max30001_en_int.all) == -1)		printf("read failed\r\n");
//	  	else															printf("register value = %d\r\n", (int) max30001_en_int.all);
}

//**********************************************************************

void max30001_usb_init(char *USB_command, uint32_t regvalue){
	char buffer[100]="";

//*************************REGISTER WRITE FUNCTIONS*****************************
//**********EN_INT register*****************************************************
	if (strncmp(USB_command, "MAX_EN_INT1", 11) == 0){

		sprintf(buffer, "Received EN_INT register write command = %.6s\r\n", USB_parameter);
		printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
		CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

		//Read old register value
		if (max30001_reg_read(EN_INT, &max30001_en_int.all) == -1)
		{
			sprintf(buffer, "Read failed\r\n");
			printf("%s\n", buffer);                             //debug ispis na UART COM port
			CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
		}
		else
		{
			sprintf(buffer, "Old EN_INT register value = %X\r\n", max30001_en_int.all);
			printf("%s\n", buffer);                             //debug ispis na UART COM port
			CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
		}

		//Write new register value
		if (max30001_reg_write(EN_INT, regvalue) == -1)
		{
			sprintf(buffer, "Write failed\r\n");
			printf("%s\n", buffer);                             //debug ispis na UART COM port
			CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
		}

		//Read new register value
		if (max30001_reg_read(EN_INT, &max30001_en_int.all) == -1)
		{
			sprintf(buffer, "Read failed\r\n");
			printf("%s\n", buffer);                             //debug ispis na UART COM port
			CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
		}
		else
		{
			sprintf(buffer, "New EN_INT register value = %X\r\n", max30001_en_int.all);
			printf("%s\n", buffer);                             //debug ispis na UART COM port
			CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
		}

	}
//**********EN_INT2 register*****************************************************
		if (strncmp(USB_command, "MAX_EN_INT2", 11) == 0){

			sprintf(buffer, "Received EN_INT2 register write command = %.6s\r\n", USB_parameter);
			printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
			CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

			//Read old register value
			if (max30001_reg_read(EN_INT2, &max30001_en_int2.all) == -1)
			{
				sprintf(buffer, "Read failed\r\n");
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
			else
			{
				sprintf(buffer, "Old EN_INT2 register value = %X\r\n", max30001_en_int2.all);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}

			//Write new register value
			if (max30001_reg_write(EN_INT2, regvalue) == -1)
			{
				sprintf(buffer, "Write failed\r\n");
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}

			//Read new register value
			if (max30001_reg_read(EN_INT2, &max30001_en_int2.all) == -1)
			{
				sprintf(buffer, "Read failed\r\n");
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
			else
			{
				sprintf(buffer, "New EN_INT2 register value = %X\r\n", max30001_en_int2.all);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}

		}

//**********MNGR_INT register*****************************************************
					if (strncmp(USB_command, "MAX_MNGR_INT", 12) == 0){

						sprintf(buffer, "Received MNGR_INT register write command = %.6s\r\n", USB_parameter);
						printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
						CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

						//Read old register value
						if (max30001_reg_read(MNGR_INT, &max30001_mngr_int.all) == -1)
						{
							sprintf(buffer, "Read failed\r\n");
							printf("%s\n", buffer);                             //debug ispis na UART COM port
							CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
						}
						else
						{
							sprintf(buffer, "Old MNGR_INT register value = %X\r\n", max30001_mngr_int.all);
							printf("%s\n", buffer);                             //debug ispis na UART COM port
							CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
						}

						//Write new register value
						if (max30001_reg_write(MNGR_INT, regvalue) == -1)
						{
							sprintf(buffer, "Write failed\r\n");
							printf("%s\n", buffer);                             //debug ispis na UART COM port
							CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
						}

						//Read new register value
						if (max30001_reg_read(MNGR_INT, &max30001_mngr_int.all) == -1)
						{
							sprintf(buffer, "Read failed\r\n");
							printf("%s\n", buffer);                             //debug ispis na UART COM port
							CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
						}
						else
						{
							sprintf(buffer, "New MNGR_INT register value = %X\r\n", max30001_mngr_int.all);
							printf("%s\n", buffer);                             //debug ispis na UART COM port
							CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
						}

					}

//**********MNGR_DYN register*****************************************************
			if (strncmp(USB_command, "MAX_MNGR_DYN", 12) == 0){

				sprintf(buffer, "Received MNGR_DYN register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				if (max30001_reg_read(MNGR_DYN, &max30001_mngr_dyn.all) == -1)
				{
					sprintf(buffer, "Read failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}
				else
				{
					sprintf(buffer, "Old MNGR_DYN register value = %X\r\n", max30001_mngr_dyn.all);
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

				//Write new register value
				if (max30001_reg_write(MNGR_DYN, regvalue) == -1)
				{
					sprintf(buffer, "Write failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

				//Read new register value
				if (max30001_reg_read(MNGR_DYN, &max30001_mngr_dyn.all) == -1)
				{
					sprintf(buffer, "Read failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}
				else
				{
					sprintf(buffer, "New MNGR_DYN register value = %X\r\n", max30001_mngr_dyn.all);
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

			}

//**********CNFG_GEN register*****************************************************
	if (strncmp(USB_command, "MAX_CNFG_GEN", 12) == 0){

			sprintf(buffer, "Received CNFG_GEN register write command = %.6s\r\n", USB_parameter);
			printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
			CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

			//Read old register value
			if (max30001_reg_read(CNFG_GEN, &max30001_cnfg_gen.all) == -1)
			{
				sprintf(buffer, "Read failed\r\n");
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
			else
			{
				sprintf(buffer, "Old CNFG_GEN register value = %X\r\n", max30001_cnfg_gen.all);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}

			//Write new register value
			if (max30001_reg_write(CNFG_GEN, regvalue) == -1)
			{
				sprintf(buffer, "Write failed\r\n");
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}

			//Read new register value
			if (max30001_reg_read(CNFG_GEN, &max30001_cnfg_gen.all) == -1)
			{
				sprintf(buffer, "Read failed\r\n");
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}
			else
			{
				sprintf(buffer, "New CNFG_GEN register value = %X\r\n", max30001_cnfg_gen.all);
				printf("%s\n", buffer);                             //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
			}

		}

//**********CNFG_CAL register*****************************************************
		if (strncmp(USB_command, "MAX_CNFG_CAL", 12) == 0){

				sprintf(buffer, "Received CNFG_CAL register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				if (max30001_reg_read(CNFG_CAL, &max30001_cnfg_cal.all) == -1)
				{
					sprintf(buffer, "Read failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}
				else
				{
					sprintf(buffer, "Old CNFG_CAL register value = %X\r\n", max30001_cnfg_cal.all);
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

				//Write new register value
				if (max30001_reg_write(CNFG_CAL, regvalue) == -1)
				{
					sprintf(buffer, "Write failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

				//Read new register value
				if (max30001_reg_read(CNFG_CAL, &max30001_cnfg_cal.all) == -1)
				{
					sprintf(buffer, "Read failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}
				else
				{
					sprintf(buffer, "New CNFG_CAL register value = %X\r\n", max30001_cnfg_cal.all);
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

			}

//**********CNFG_EMUX register*****************************************************
		if (strncmp(USB_command, "MAX_CNFG_EMUX", 13) == 0){

				sprintf(buffer, "Received CNFG_EMUX register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Read old register value
				if (max30001_reg_read(CNFG_EMUX, &max30001_cnfg_emux.all) == -1)
				{
					sprintf(buffer, "Read failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}
				else
				{
					sprintf(buffer, "Old CNFG_EMUX register value = %X\r\n", max30001_cnfg_emux.all);
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

				//Write new register value
				if (max30001_reg_write(CNFG_EMUX, regvalue) == -1)
				{
					sprintf(buffer, "Write failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

				//Read new register value
				if (max30001_reg_read(CNFG_EMUX, &max30001_cnfg_emux.all) == -1)
				{
					sprintf(buffer, "Read failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}
				else
				{
					sprintf(buffer, "New CNFG_EMUX register value = %X\r\n", max30001_cnfg_emux.all);
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

			}

//**********CNFG_ECG register*****************************************************
		if (strncmp(USB_command, "MAX_CNFG_ECG", 12) == 0){

				sprintf(buffer, "Received CNFG_ECG register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

						//Read old register value
				if (max30001_reg_read(CNFG_ECG, &max30001_cnfg_ecg.all) == -1)
				{
					sprintf(buffer, "Read failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}
				else
				{
					sprintf(buffer, "Old CNFG_ECG register value = %X\r\n", max30001_cnfg_ecg.all);
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

				//Write new register value
				if (max30001_reg_write(CNFG_ECG, regvalue) == -1)
				{
					sprintf(buffer, "Write failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

				//Read new register value
				if (max30001_reg_read(CNFG_ECG, &max30001_cnfg_ecg.all) == -1)
				{
					sprintf(buffer, "Read failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}
				else
				{
					sprintf(buffer, "New CNFG_ECG register value = %X\r\n", max30001_cnfg_ecg.all);
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

			}

//**********CNFG_BMUX register*****************************************************
		if (strncmp(USB_command, "MAX_CNFG_BMUX", 13) == 0){

				sprintf(buffer, "Received CNFG_BMUX register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

						//Read old register value
				if (max30001_reg_read(CNFG_BMUX, &max30001_cnfg_bmux.all) == -1)
				{
					sprintf(buffer, "Read failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}
				else
				{
					sprintf(buffer, "Old CNFG_BMUX register value = %X\r\n", max30001_cnfg_bmux.all);
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

				//Write new register value
				if (max30001_reg_write(CNFG_BMUX, regvalue) == -1)
				{
					sprintf(buffer, "Write failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

				//Read new register value
				if (max30001_reg_read(CNFG_BMUX, &max30001_cnfg_bmux.all) == -1)
				{
					sprintf(buffer, "Read failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}
				else
				{
					sprintf(buffer, "New CNFG_BMUX register value = %X\r\n", max30001_cnfg_bmux.all);
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

			}

//**********CNFG_BIOZ register*****************************************************
		if (strncmp(USB_command, "MAX_CNFG_BIOZ", 13) == 0){

				sprintf(buffer, "Received CNFG_BIOZ register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

						//Read old register value
				if (max30001_reg_read(CNFG_BIOZ, &max30001_cnfg_bioz.all) == -1)
				{
					sprintf(buffer, "Read failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}
				else
				{
					sprintf(buffer, "Old CNFG_BIOZ register value = %X\r\n", max30001_cnfg_bioz.all);
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

				//Write new register value
				if (max30001_reg_write(CNFG_BIOZ, regvalue) == -1)
				{
					sprintf(buffer, "Write failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

				//Read new register value
				if (max30001_reg_read(CNFG_BIOZ, &max30001_cnfg_bioz.all) == -1)
				{
					sprintf(buffer, "Read failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}
				else
				{
					sprintf(buffer, "New CNFG_BIOZ register value = %X\r\n", max30001_cnfg_bioz.all);
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

			}

//**********CNFG_PACE register*****************************************************
		if (strncmp(USB_command, "MAX_CNFG_PACE", 13) == 0){

				sprintf(buffer, "Received CNFG_PACE register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

						//Read old register value
				if (max30001_reg_read(CNFG_PACE, &max30001_cnfg_pace.all) == -1)
				{
					sprintf(buffer, "Read failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}
				else
				{
					sprintf(buffer, "Old CNFG_PACE register value = %X\r\n", max30001_cnfg_pace.all);
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

				//Write new register value
				if (max30001_reg_write(CNFG_PACE, regvalue) == -1)
				{
					sprintf(buffer, "Write failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

				//Read new register value
				if (max30001_reg_read(CNFG_PACE, &max30001_cnfg_pace.all) == -1)
				{
					sprintf(buffer, "Read failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}
				else
				{
					sprintf(buffer, "New CNFG_PACE register value = %X\r\n", max30001_cnfg_pace.all);
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

			}

//**********CNFG_RTOR1 register*****************************************************
		if (strncmp(USB_command, "MAX_CNFG_RTOR1", 14) == 0){

				sprintf(buffer, "Received CNFG_RTOR1 register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

						//Read old register value
				if (max30001_reg_read(CNFG_RTOR1, &max30001_cnfg_rtor1.all) == -1)
				{
					sprintf(buffer, "Read failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}
				else
				{
					sprintf(buffer, "Old CNFG_RTOR1 register value = %X\r\n", max30001_cnfg_rtor1.all);
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

				//Write new register value
				if (max30001_reg_write(CNFG_RTOR1, regvalue) == -1)
				{
					sprintf(buffer, "Write failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

				//Read new register value
				if (max30001_reg_read(CNFG_RTOR1, &max30001_cnfg_rtor1.all) == -1)
				{
					sprintf(buffer, "Read failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}
				else
				{
					sprintf(buffer, "New CNFG_RTOR1 register value = %X\r\n", max30001_cnfg_rtor1.all);
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

			}

//**********CNFG_RTOR2 register*****************************************************
		if (strncmp(USB_command, "MAX_CNFG_RTOR2", 14) == 0){

				sprintf(buffer, "Received CNFG_RTOR2 register write command = %.6s\r\n", USB_parameter);
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

						//Read old register value
				if (max30001_reg_read(CNFG_RTOR2, &max30001_cnfg_rtor2.all) == -1)
				{
					sprintf(buffer, "Read failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}
				else
				{
					sprintf(buffer, "Old CNFG_RTOR2 register value = %X\r\n", max30001_cnfg_rtor2.all);
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

				//Write new register value
				if (max30001_reg_write(CNFG_RTOR2, regvalue) == -1)
				{
					sprintf(buffer, "Write failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

				//Read new register value
				if (max30001_reg_read(CNFG_RTOR2, &max30001_cnfg_rtor2.all) == -1)
				{
					sprintf(buffer, "Read failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}
				else
				{
					sprintf(buffer, "New CNFG_RTOR2 register value = %X\r\n", max30001_cnfg_rtor2.all);
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

			}

//*************************QUICK COMMANDS FUNCTIONS*****************************
//*******************************SW RESET***************************************
		if (strncmp(USB_command, "MAX_SW_RESET", 12) == 0){

				sprintf(buffer, "Received SW reset command\r\n");
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				if (max30001_reg_write(SW_RST, regvalue) == -1)
				{
					sprintf(buffer, "Write failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}
				else
				{
					sprintf(buffer, "SW reset command sent\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

			}
//*******************************SYNCHRONIZE***************************************
		if (strncmp(USB_command, "MAX_SYNCH", 9) == 0){

				sprintf(buffer, "Received synchronize command\r\n");
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				if (max30001_reg_write(SYNCH, regvalue) == -1)
				{
					sprintf(buffer, "Write failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}
				else
				{
					sprintf(buffer, "Synchronize command sent\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

			}
//*******************************FIFO RESET***************************************
		if (strncmp(USB_command, "MAX_FIFO_RST", 12) == 0){

				sprintf(buffer, "Received FIFO reset command\r\n");
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				if (max30001_reg_write(FIFO_RST, regvalue) == -1)
				{
					sprintf(buffer, "Write failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}
				else
				{
					sprintf(buffer, "FIFO reset command sent\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

			}
//*******************************ECG STOP***************************************
		if (strncmp(USB_command, "MAX_ECG_STOP", 12) == 0){

				sprintf(buffer, "Received ECG STOP command\r\n");
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				if (max30001_Stop_ECG() == -1)
				{
					sprintf(buffer, "Write failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}
				else
				{
					sprintf(buffer, "ECG stopped\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

			}
//*******************************BIOZ STOP***************************************
		if (strncmp(USB_command, "MAX_BIOZ_STOP", 13) == 0){

				sprintf(buffer, "Received BIOZ STOP command\r\n");
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				if (max30001_Stop_BIOZ() == -1)
				{
					sprintf(buffer, "Write failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}
				else
				{
					sprintf(buffer, "BIOZ stopped\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

			}
//*******************************RTOR STOP***************************************
		if (strncmp(USB_command, "MAX_RTOR_STOP", 13) == 0){

				sprintf(buffer, "Received RTOR STOP command\r\n");
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				if (max30001_Stop_RtoR() == -1)
				{
					sprintf(buffer, "Write failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}
				else
				{
					sprintf(buffer, "RTOR stopped\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

			}
//*******************************ECG START***************************************
		if (strncmp(USB_command, "MAX_ECG_START", 13) == 0){

				sprintf(buffer, "Received ECG START command\r\n");
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				if (max30001_Start_ECG() == -1)
				{
					sprintf(buffer, "Write failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}
				else
				{
					sprintf(buffer, "ECG started\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

			}
//*******************************BIOZ START***************************************
		if (strncmp(USB_command, "MAX_BIOZ_START", 14) == 0){

				sprintf(buffer, "Received BIOZ START command\r\n");
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				if (max30001_Start_BIOZ() == -1)
				{
					sprintf(buffer, "Write failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}
				else
				{
					sprintf(buffer, "BIOZ started\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

			}
//*******************************RTOR START***************************************
		if (strncmp(USB_command, "MAX_RTOR_START", 14) == 0){

				sprintf(buffer, "Received RTOR START command\r\n");
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				if (max30001_Start_RtoR() == -1)
				{
					sprintf(buffer, "Write failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}
				else
				{
					sprintf(buffer, "RTOR started\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

			}
//*******************************READ STATUS REGISTER***************************************
		if (strncmp(USB_command, "MAX_STATUS", 10) == 0){

				sprintf(buffer, "Received read STATUS register command\r\n");
				printf("Buffer = %s\n", buffer);                    //debug ispis na UART COM port
				CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port

				//Write new register value
				if (max30001_reg_read(STATUS, &max30001_status.all) == -1)
				{
					sprintf(buffer, "Read failed\r\n");
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}
				else
				{
					sprintf(buffer, "STATUS register value = %X\r\n", max30001_status.all);
					printf("%s\n", buffer);                             //debug ispis na UART COM port
					CDC_Transmit_FS((uint8_t *)buffer, strlen(buffer)); //ispis poruke na USB VCOM port
				}

			}
//********************************************************************************
}
