/**********************************************************************
* $Id$			TLV320AIC3106				2013-08-26
*//**
* @file			TLV320AIC3106.h
* @brief		Header file for TLV320AIC3106 codec Driver
* @version	1.0
* @date			2013/8/26 11:29:58
* @author		HaoZhilei
*
* Copyright(C) 2013, Tianjin 712 Communication & Broadcasting Co., Ltd
* All rights reserved.
*
**********************************************************************/
#ifndef __AIC3106__
#define __AIC3106__

#define FUNC_OK          0x00
#define FUNC_ERR         0xFF







#define CODEC_I2C 						LPC_I2C0
#define AIC3106_ADDR  				0x18					/* MFP1=0 MFP0=0 see TLV320AIC3106.pdf P21 */

/**
 * @brief TLV320AIC3106 Register Address
 */
typedef enum {
	REG_Page_Select=0x00,                                         /* reg #000 */
	REG_Software_Reset,                                           /* reg #001 */
	REG_Codec_Sample_Rate_Select,                                 /* reg #002 */
	REG_PLL_Programming_A,                                        /* reg #003 */
	REG_PLL_Programming_B,                                        /* reg #004 */
	REG_PLL_Programming_C,                                        /* reg #005 */
	REG_PLL_Programming_D,                                        /* reg #006 */
	REG_Codec_Datapath_Setup,                                     /* reg #007 */
	REG_Audio_Serial_Data_Interface_Ctrl_A,                       /* reg #008 */
	REG_Audio_Serial_Data_Interface_Ctrl_B,                       /* reg #009 */
	REG_Audio_Serial_Data_Interface_Ctrl_C,                       /* reg #010 */
	REG_Audio_Codec_Overflow_Flag,                                /* reg #011 */
	REG_Audio_Codec_Digital_Filter_Ctrl,                          /* reg #012 */
	REG_Headset__or__Button_Press_Detection_A,                    /* reg #013 */
	REG_Headset__or__Button_Press_Detection_B,                    /* reg #014 */
	REG_Left_ADC_PGA_Gain_Ctrl,                                   /* reg #015 */
	REG_Right_ADC_PGA_Gain_Ctrl,                                  /* reg #016 */
	REG_MIC3L_or_R_to_Left_ADC_Ctrl,                              /* reg #017 */
	REG_MIC3L_or_R_to_Right_ADC_Ctrl,                             /* reg #018 */
	REG_LINE1L_to_Left_ADC_Ctrl,                                  /* reg #019 */
	REG_LINE2L_to_Left_ADC_Ctrl,                                  /* reg #020 */
	REG_LINE1R_to_Left_ADC_Ctrl,                                  /* reg #021 */
	REG_LINE1R_to_Right_ADC_Ctrl,                                 /* reg #022 */
	REG_LINE2R_to_Right_ADC_Ctrl,                                 /* reg #023 */
	REG_LINE1L_to_Right_ADC_Ctrl,                                 /* reg #024 */
	REG_MICBIAS_Ctrl,                                             /* reg #025 */
	REG_Left_AGC_Ctrl_A,                                          /* reg #026 */
	REG_Left_AGC_Ctrl_B,                                          /* reg #027 */
	REG_Left_AGC_Ctrl_C,                                          /* reg #028 */
	REG_Right_AGC_Ctrl_A,                                         /* reg #029 */
	REG_Right_AGC_Ctrl_B,                                         /* reg #030 */
	REG_Right_AGC_Ctrl_C,                                         /* reg #031 */
	REG_Left_AGC_Gain,                                            /* reg #032 */
	REG_Right_AGC_Gain,                                           /* reg #033 */
	REG_Left_AGC_Noise_Gate_Debounce,                             /* reg #034 */
	REG_Right_AGC_Noise_Gate_Debounce,                            /* reg #035 */
	REG_ADC_Flag,                                                 /* reg #036 */
	REG_DAC_Power_and_Output_Driver_Ctrl,                         /* reg #037 */
	REG_High_Power_Output_Driver_Ctrl,                            /* reg #038 */
	REG_Reserved1,  /* Reg39: Reserved Registers */
	REG_High_Power_Output_Stage_Ctrl,                             /* reg #040 */
	REG_DAC_Output_Switching_Ctrl,                                /* reg #041 */
	REG_Output_Driver_Pop_Reduction,                              /* reg #042 */
	REG_Left_DAC_Digital_Volume_Ctrl,                             /* reg #043 */
	REG_Right_DAC_Digital_Volume_Ctrl,                            /* reg #044 */
	REG_LINE2L_to_HPLOUT_Volume_Ctrl,                             /* reg #045 */
	REG_PGA_L_to_HPLOUT_Volume_Ctrl,                              /* reg #046 */
	REG_DAC_L1_to_HPLOUT_Volume_Ctrl,                             /* reg #047 */
	REG_LINE2R_to_HPLOUT_Volume_Ctrl,                             /* reg #048 */
	REG_PGA_R_to_HPLOUT_Volume_Ctrl,                              /* reg #049 */
	REG_DAC_R1_to_HPLOUT_Volume_Ctrl,                             /* reg #050 */
	REG_HPLOUT_Output_Level_Ctrl,                                 /* reg #051 */
	REG_LINE2L_to_HPLCOM_Volume_Ctrl,                             /* reg #052 */
	REG_PGA_L_to_HPLCOM_Volume_Ctrl,                              /* reg #053 */
	REG_DAC_L1_to_HPLCOM_Volume_Ctrl,                             /* reg #054 */
	REG_LINE2R_to_HPLCOM_Volume_Ctrl,                             /* reg #055 */
	REG_PGA_R_to_HPLCOM_Volume_Ctrl,                              /* reg #056 */
	REG_DAC_R1_to_HPLCOM_Volume_Ctrl,                             /* reg #057 */
	REG_HPLCOM_Output_Level_Ctrl,                                 /* reg #058 */
	REG_LINE2L_to_HPROUT_Volume_Ctrl,                             /* reg #059 */
	REG_PGA_L_to_HPROUT_Volume_Ctrl,                              /* reg #060 */
	REG_DAC_L1_to_HPROUT_Volume_Ctrl,                             /* reg #061 */
	REG_LINE2R_to_HPROUT_Volume_Ctrl,                             /* reg #062 */
	REG_PGA_R_to_HPROUT_Volume_Ctrl,                              /* reg #063 */
	REG_DAC_R1_to_HPROUT_Volume_Ctrl,                             /* reg #064 */
	REG_HPROUT_Output_Level_Ctrl,                                 /* reg #065 */
	REG_LINE2L_to_HPRCOM_Volume_Ctrl,                             /* reg #066 */
	REG_PGA_L_to_HPRCOM_Volume_Ctrl,                              /* reg #067 */
	REG_DAC_L1_to_HPRCOM_Volume_Ctrl,                             /* reg #068 */
	REG_LINE2R_to_HPRCOM_Volume_Ctrl,                             /* reg #069 */
	REG_PGA_R_to_HPRCOM_Volume_Ctrl,                              /* reg #070 */
	REG_DAC_R1_to_HPRCOM_Volume_Ctrl,                             /* reg #071 */
	REG_HPRCOM_Output_Level_Ctrl,                                 /* reg #072 */
	REG_LINE2L_to_MONO_LOP_or_M_Volume_Ctrl,                      /* reg #073 */
	REG_PGA_L_to_MONO_LOP_or_M_Volume_Ctrl,                       /* reg #074 */
	REG_DAC_L1_to_MONO_LOP_or_M_Volume_Ctrl,                      /* reg #075 */
	REG_LINE2R_to_MONO_LOP_or_M_Volume_Ctrl,                      /* reg #076 */
	REG_PGA_R_to_MONO_LOP_or_M_Volume_Ctrl,                       /* reg #077 */
	REG_DAC_R1_to_MONO_LOP_or_M_Volume_Ctrl,                      /* reg #078 */
	REG_MONO_LOP_or_M_Output_Level_Ctrl,                          /* reg #079 */
	REG_LINE2L_to_LEFT_LOP_or_M_Volume_Ctrl,                      /* reg #080 */
	REG_PGA_L_to_LEFT_LOP_or_M_Volume_Ctrl,                       /* reg #081 */
	REG_DAC_L1_to_LEFT_LOP_or_M_Volume_Ctrl,                      /* reg #082 */
	REG_LINE2R_to_LEFT_LOP_or_M_Volume_Ctrl,                      /* reg #083 */
	REG_PGA_R_to_LEFT_LOP_or_M_Volume_Ctrl,                       /* reg #084 */
	REG_DAC_R1_to_LEFT_LOP_or_M_Volume_Ctrl,                      /* reg #085 */
	REG_LEFT_LOP_or_M_Output_Level_Ctrl,                          /* reg #086 */
	REG_LINE2L_to_RIGHT_LOP_or_M_Volume_Ctrl,                     /* reg #087 */
	REG_PGA_L_to_RIGHT_LOP_or_M_Volume_Ctrl,                      /* reg #088 */
	REG_DAC_L1_to_RIGHT_LOP_or_M_Volume_Ctrl,                     /* reg #089 */
	REG_LINE2R_to_RIGHT_LOP_or_M_Volume_Ctrl,                     /* reg #090 */
	REG_PGA_R_to_RIGHT_LOP_or_M_Volume_Ctrl,                      /* reg #091 */
	REG_DAC_R1_to_RIGHT_LOP_or_M_Volume_Ctrl,                     /* reg #092 */
	REG_RIGHT_LOP_or_M_Output_Level_Ctrl,                         /* reg #093 */
	REG_Module_Power_Status,                                      /* reg #094 */
	REG_Output_Driver_Short_Circuit_Detection_Status,             /* reg #095 */
	REG_Sticky_Interrupt_Flags,                                   /* reg #096 */
	REG_Real_Time_Interrupt_Flags,                                /* reg #097 */
	REG_GPIO1_Ctrl,                                               /* reg #098 */
	REG_GPIO2_Ctrl,                                               /* reg #099 */
	REG_Additional_GPIO_Ctrl_A,                                   /* reg #100 */
	REG_Additional_GPIO_Ctrl_B,                                   /* reg #101 */
	REG_Clock_Generation_Ctrl,                                    /* reg #102 */
	REG_Left_AGC_New_Programmable_Attack_Time,                    /* reg #103 */
	REG_Left_AGC_New_Programmable_Decay_Time,                     /* reg #104 */
	REG_Right_AGC_New_Programmable_Attack_Time,                   /* reg #105 */
	REG_Right_AGC_New_Programmable_Decay_Time,                    /* reg #106 */
	REG_New_Programmable_ADC_Digital_Path_and_I2C_Bus_Condition,  /* reg #107 */
	REG_Passive_Analog_Signal_Bypass_Selection_During_Powerdown,  /* reg #108 */
	REG_DAC_Quiescent_Current_Adjustment,													/* reg #109 */
	REG_Reserved2  /* Register 110-127: Reserved Registers */     
}AIC3106_REG;



/**
 * @brief TLV320AIC3106 PGA gain set (Page0/Reg15~16)
 */
typedef enum {
	GAIN_left_PGA,
	GAIN_right_PGA
}AIC3106_GAIN_PGA;

#define Muted_PGA_Gain				(1<<7)


/**
 * @brief TLV320AIC3106 Input class (Page0/Reg17~24)
 */
typedef enum {
	LINE1L_to_left_PGA,
	LINE2L_to_left_PGA,
	LINE1R_to_left_PGA,
	LINE1R_to_right_PGA,
	LINE2R_to_right_PGA,
	LINE1L_to_right_PGA,
	
	MIC3L_to_left_PGA,
	MIC3R_to_left_PGA,
	MIC3L_to_right_PGA,
	MIC3R_to_right_PGA
}AIC3106_Input;
/**
 * @brief TLV320AIC3106 Input Level Control: Decay (Page0/Reg17~24)
 */
#define DecayInput_0 								0
#define DecayInput_1_5							1
#define DecayInput_3 								2
#define DecayInput_4_5 							3
#define DecayInput_6 								4
#define DecayInput_7_5 							5
#define DecayInput_9 								6
#define DecayInput_10_5							7
#define DecayInput_12 							8
/** Input Level Control for Left ADC PGA Mix: not connected */
#define Input_PGA_Not_Connected 		(0x0F)




/**
 * @brief TLV320AIC3106 AGC class
 */
typedef enum {
	CFG_left_AGC,
	CFG_right_AGC
}AIC3106_AGC;

/**
 * @brief TLV320AIC3106 AGC set (Page0/Reg26~35)
 */
typedef struct {
	unsigned char enable;					/**< AGC Enable */
	unsigned char target_level;		/**< AGC Target Level */
	unsigned char attack_time;		/**< AGC Attack Time */
	unsigned char decay_time;			/**< AGC Decay Time */
	unsigned char max_gain;				/**< AGC Maximum Gain Allowed */
	unsigned char hysteresis;			/**< Noise Gate Hysteresis Level Control */
	unsigned char noise_hold;			/**< AGC Noise Threshold Control */
	unsigned char clip_stepping;	/**< AGC Clip Stepping Control */
	unsigned char gain;						/**< Channel Gain Applied by AGC Algorithm */
	unsigned char noise_debounce;	/**< AGC Noise Detection Debounce Control */
	unsigned char signal_debounce;/**< AGC Signal Detection Debounce Control */
}AIC3106_AGC_CFG_Type;


	
/**
 * @brief TLV320AIC3106 Digital Volume set (Page0/Reg43~44)
 */
typedef enum {
	Left_DAC_Digital_Volume,
	Right_DAC_Digital_Volume
}AIC3106_Digital_Volume;
/** DAC channel is muted */
#define Digital_Mute_Vol	(1<<7)


/**
 * @brief TLV320AIC3106 Analog Volume set (Page0/Reg45~50,Reg52~57,Reg59~64,Reg66~71,Reg73~78,Reg80~85,Reg87~92)
 */
typedef enum {
	VOL_LINE2L_to_HPLOUT=0,
	VOL_PGA_L_to_HPLOUT,          
	VOL_DAC_L1_to_HPLOUT,         
	VOL_LINE2R_to_HPLOUT,         
	VOL_PGA_R_to_HPLOUT,          
	VOL_DAC_R1_to_HPLOUT,
													/* HPLOUT Output Control */
	VOL_LINE2L_to_HPLCOM=7,         
	VOL_PGA_L_to_HPLCOM,          
	VOL_DAC_L1_to_HPLCOM,         
	VOL_LINE2R_to_HPLCOM,         
	VOL_PGA_R_to_HPLCOM,          
	VOL_DAC_R1_to_HPLCOM,         
													/* HPLCOM Output Control */
	VOL_LINE2L_to_HPROUT=14,         
	VOL_PGA_L_to_HPROUT,          
	VOL_DAC_L1_to_HPROUT,         
	VOL_LINE2R_to_HPROUT,         
	VOL_PGA_R_to_HPROUT,          
	VOL_DAC_R1_to_HPROUT,         
													/* HPROUT Output Control */
	VOL_LINE2L_to_HPRCOM=21,         
	VOL_PGA_L_to_HPRCOM,          
	VOL_DAC_L1_to_HPRCOM,         
	VOL_LINE2R_to_HPRCOM,         
	VOL_PGA_R_to_HPRCOM,          
	VOL_DAC_R1_to_HPRCOM,         
													/* HPRCOM Output Control */
	VOL_LINE2L_to_MONO_LOP_or_M=28,  
	VOL_PGA_L_to_MONO_LOP_or_M,   
	VOL_DAC_L1_to_MONO_LOP_or_M,  
	VOL_LINE2R_to_MONO_LOP_or_M,  
	VOL_PGA_R_to_MONO_LOP_or_M,   
	VOL_DAC_R1_to_MONO_LOP_or_M,  
													/* MONO_LOP/M Output Control */
	VOL_LINE2L_to_LEFT_LOP_or_M=35,  
	VOL_PGA_L_to_LEFT_LOP_or_M,   
	VOL_DAC_L1_to_LEFT_LOP_or_M,  
	VOL_LINE2R_to_LEFT_LOP_or_M,  
	VOL_PGA_R_to_LEFT_LOP_or_M,   
	VOL_DAC_R1_to_LEFT_LOP_or_M,  
													/* LEFT_LOP/M Output Control */
	VOL_LINE2L_to_RIGHT_LOP_or_M=42, 
	VOL_PGA_L_to_RIGHT_LOP_or_M,  
	VOL_DAC_L1_to_RIGHT_LOP_or_M, 
	VOL_LINE2R_to_RIGHT_LOP_or_M, 
	VOL_PGA_R_to_RIGHT_LOP_or_M,  
	VOL_DAC_R1_to_RIGHT_LOP_or_M
													/* RIGHT_LOP/M Output Control */
}AIC3106_Analog_Volume;

/**
 * @brief TLV320AIC3106 Output level class (Page0/Reg51,58,65,72,79,86,93)
 */
typedef enum {
	HPLOUT_Output=0,
	HPLCOM_Output,
	HPROUT_Output,
	HPRCOM_Output,
	MONO_LOP_or_M_Output,
	LEFT_LOP_or_M_Output,
	RIGHT_LOP_or_M_Output
}AIC3106_Output;

/**
 * @brief TLV320AIC3106 Output Level Control (Page0/Reg51,58,65,72,79,86,93)
 */
typedef struct {
	unsigned char output_level;	/**< Output Level Control */
	unsigned char mute;
	unsigned char drive;				/**< Power Down Drive Control */
	unsigned char power_up;			/**< Power Control */
}AIC3106_OUTPUT_LEVEL_CTRL_Type;

/**
 * @brief TLV320AIC3106 Output Driver Control (Page0/Reg37,38)
 */
typedef enum {
	HPLCOM_Differ_HPLOUT=0,
	HPLCOM_VCM,
	HPLCOM_Single,
	HPRCOM_Differ_HPROUT,
	HPRCOM_VCM,
	HPRCOM_Single,
	HPRCOM_Differ_HPLCOM,
	HPRCOM_Feedback,
	NONE_Output_Driver
}AIC3106_Output_Driver;






void AIC3106_Init(void);
unsigned char AIC3106_Reg_write(AIC3106_REG reg, unsigned char data);
unsigned char AIC3106_Reg_read(AIC3106_REG reg, unsigned char *data);
unsigned char AIC3106_Clk_set(unsigned int ClockFreq, unsigned char bit);
unsigned char AIC3106_Gain_PGA_set(AIC3106_GAIN_PGA pga, unsigned char gain);
unsigned char AIC3106_Input_control(AIC3106_Input in, unsigned char differ, unsigned char gain);
unsigned char AIC3106_AGC_set(AIC3106_AGC agc, AIC3106_AGC_CFG_Type *cfg);
unsigned char AIC3106_Digital_Vol_set(AIC3106_Digital_Volume dv, unsigned char vol);
unsigned char AIC3106_Analog_Vol_set(AIC3106_Analog_Volume av, unsigned char vol);
unsigned char AIC3106_Output_control(AIC3106_Output out, AIC3106_Output_Driver drv, unsigned char gain);


#endif /* __LPC4357_AIC3106__ */
