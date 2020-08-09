/*******************************************************************************
* File Name: HORIZ.h
* Version 3.30
*
* Description:
*  Contains the prototypes and constants for the functions available to the
*  PWM user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_PWM_HORIZ_H)
#define CY_PWM_HORIZ_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 HORIZ_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define HORIZ_Resolution                     (8u)
#define HORIZ_UsingFixedFunction             (0u)
#define HORIZ_DeadBandMode                   (0u)
#define HORIZ_KillModeMinTime                (0u)
#define HORIZ_KillMode                       (0u)
#define HORIZ_PWMMode                        (1u)
#define HORIZ_PWMModeIsCenterAligned         (0u)
#define HORIZ_DeadBandUsed                   (0u)
#define HORIZ_DeadBand2_4                    (0u)

#if !defined(HORIZ_PWMUDB_genblk8_stsreg__REMOVED)
    #define HORIZ_UseStatus                  (0u)
#else
    #define HORIZ_UseStatus                  (0u)
#endif /* !defined(HORIZ_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(HORIZ_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define HORIZ_UseControl                 (1u)
#else
    #define HORIZ_UseControl                 (0u)
#endif /* !defined(HORIZ_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define HORIZ_UseOneCompareMode              (0u)
#define HORIZ_MinimumKillTime                (1u)
#define HORIZ_EnableMode                     (0u)

#define HORIZ_CompareMode1SW                 (0u)
#define HORIZ_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define HORIZ__B_PWM__DISABLED 0
#define HORIZ__B_PWM__ASYNCHRONOUS 1
#define HORIZ__B_PWM__SINGLECYCLE 2
#define HORIZ__B_PWM__LATCHED 3
#define HORIZ__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define HORIZ__B_PWM__DBMDISABLED 0
#define HORIZ__B_PWM__DBM_2_4_CLOCKS 1
#define HORIZ__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define HORIZ__B_PWM__ONE_OUTPUT 0
#define HORIZ__B_PWM__TWO_OUTPUTS 1
#define HORIZ__B_PWM__DUAL_EDGE 2
#define HORIZ__B_PWM__CENTER_ALIGN 3
#define HORIZ__B_PWM__DITHER 5
#define HORIZ__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define HORIZ__B_PWM__LESS_THAN 1
#define HORIZ__B_PWM__LESS_THAN_OR_EQUAL 2
#define HORIZ__B_PWM__GREATER_THAN 3
#define HORIZ__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define HORIZ__B_PWM__EQUAL 0
#define HORIZ__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!HORIZ_UsingFixedFunction)
        uint8 PWMUdb;               /* PWM Current Counter value  */
        #if(!HORIZ_PWMModeIsCenterAligned)
            uint8 PWMPeriod;
        #endif /* (!HORIZ_PWMModeIsCenterAligned) */
        #if (HORIZ_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (HORIZ_UseStatus) */

        /* Backup for Deadband parameters */
        #if(HORIZ_DeadBandMode == HORIZ__B_PWM__DBM_256_CLOCKS || \
            HORIZ_DeadBandMode == HORIZ__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(HORIZ_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (HORIZ_KillModeMinTime) */

        /* Backup control register */
        #if(HORIZ_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (HORIZ_UseControl) */

    #endif /* (!HORIZ_UsingFixedFunction) */

}HORIZ_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    HORIZ_Start(void) ;
void    HORIZ_Stop(void) ;

#if (HORIZ_UseStatus || HORIZ_UsingFixedFunction)
    void  HORIZ_SetInterruptMode(uint8 interruptMode) ;
    uint8 HORIZ_ReadStatusRegister(void) ;
#endif /* (HORIZ_UseStatus || HORIZ_UsingFixedFunction) */

#define HORIZ_GetInterruptSource() HORIZ_ReadStatusRegister()

#if (HORIZ_UseControl)
    uint8 HORIZ_ReadControlRegister(void) ;
    void  HORIZ_WriteControlRegister(uint8 control)
          ;
#endif /* (HORIZ_UseControl) */

#if (HORIZ_UseOneCompareMode)
   #if (HORIZ_CompareMode1SW)
       void    HORIZ_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (HORIZ_CompareMode1SW) */
#else
    #if (HORIZ_CompareMode1SW)
        void    HORIZ_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (HORIZ_CompareMode1SW) */
    #if (HORIZ_CompareMode2SW)
        void    HORIZ_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (HORIZ_CompareMode2SW) */
#endif /* (HORIZ_UseOneCompareMode) */

#if (!HORIZ_UsingFixedFunction)
    uint8   HORIZ_ReadCounter(void) ;
    uint8 HORIZ_ReadCapture(void) ;

    #if (HORIZ_UseStatus)
            void HORIZ_ClearFIFO(void) ;
    #endif /* (HORIZ_UseStatus) */

    void    HORIZ_WriteCounter(uint8 counter)
            ;
#endif /* (!HORIZ_UsingFixedFunction) */

void    HORIZ_WritePeriod(uint8 period)
        ;
uint8 HORIZ_ReadPeriod(void) ;

#if (HORIZ_UseOneCompareMode)
    void    HORIZ_WriteCompare(uint8 compare)
            ;
    uint8 HORIZ_ReadCompare(void) ;
#else
    void    HORIZ_WriteCompare1(uint8 compare)
            ;
    uint8 HORIZ_ReadCompare1(void) ;
    void    HORIZ_WriteCompare2(uint8 compare)
            ;
    uint8 HORIZ_ReadCompare2(void) ;
#endif /* (HORIZ_UseOneCompareMode) */


#if (HORIZ_DeadBandUsed)
    void    HORIZ_WriteDeadTime(uint8 deadtime) ;
    uint8   HORIZ_ReadDeadTime(void) ;
#endif /* (HORIZ_DeadBandUsed) */

#if ( HORIZ_KillModeMinTime)
    void HORIZ_WriteKillTime(uint8 killtime) ;
    uint8 HORIZ_ReadKillTime(void) ;
#endif /* ( HORIZ_KillModeMinTime) */

void HORIZ_Init(void) ;
void HORIZ_Enable(void) ;
void HORIZ_Sleep(void) ;
void HORIZ_Wakeup(void) ;
void HORIZ_SaveConfig(void) ;
void HORIZ_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define HORIZ_INIT_PERIOD_VALUE          (167u)
#define HORIZ_INIT_COMPARE_VALUE1        (1u)
#define HORIZ_INIT_COMPARE_VALUE2        (40u)
#define HORIZ_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    HORIZ_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    HORIZ_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    HORIZ_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    HORIZ_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define HORIZ_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  HORIZ_CTRL_CMPMODE2_SHIFT)
#define HORIZ_DEFAULT_COMPARE1_MODE      (uint8)((uint8)2u <<  HORIZ_CTRL_CMPMODE1_SHIFT)
#define HORIZ_INIT_DEAD_TIME             (1u)


/********************************
*         Registers
******************************** */

#if (HORIZ_UsingFixedFunction)
   #define HORIZ_PERIOD_LSB              (*(reg16 *) HORIZ_PWMHW__PER0)
   #define HORIZ_PERIOD_LSB_PTR          ( (reg16 *) HORIZ_PWMHW__PER0)
   #define HORIZ_COMPARE1_LSB            (*(reg16 *) HORIZ_PWMHW__CNT_CMP0)
   #define HORIZ_COMPARE1_LSB_PTR        ( (reg16 *) HORIZ_PWMHW__CNT_CMP0)
   #define HORIZ_COMPARE2_LSB            (0x00u)
   #define HORIZ_COMPARE2_LSB_PTR        (0x00u)
   #define HORIZ_COUNTER_LSB             (*(reg16 *) HORIZ_PWMHW__CNT_CMP0)
   #define HORIZ_COUNTER_LSB_PTR         ( (reg16 *) HORIZ_PWMHW__CNT_CMP0)
   #define HORIZ_CAPTURE_LSB             (*(reg16 *) HORIZ_PWMHW__CAP0)
   #define HORIZ_CAPTURE_LSB_PTR         ( (reg16 *) HORIZ_PWMHW__CAP0)
   #define HORIZ_RT1                     (*(reg8 *)  HORIZ_PWMHW__RT1)
   #define HORIZ_RT1_PTR                 ( (reg8 *)  HORIZ_PWMHW__RT1)

#else
   #if (HORIZ_Resolution == 8u) /* 8bit - PWM */

       #if(HORIZ_PWMModeIsCenterAligned)
           #define HORIZ_PERIOD_LSB      (*(reg8 *)  HORIZ_PWMUDB_sP8_pwmdp_u0__D1_REG)
           #define HORIZ_PERIOD_LSB_PTR  ((reg8 *)   HORIZ_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #else
           #define HORIZ_PERIOD_LSB      (*(reg8 *)  HORIZ_PWMUDB_sP8_pwmdp_u0__F0_REG)
           #define HORIZ_PERIOD_LSB_PTR  ((reg8 *)   HORIZ_PWMUDB_sP8_pwmdp_u0__F0_REG)
       #endif /* (HORIZ_PWMModeIsCenterAligned) */

       #define HORIZ_COMPARE1_LSB        (*(reg8 *)  HORIZ_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define HORIZ_COMPARE1_LSB_PTR    ((reg8 *)   HORIZ_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define HORIZ_COMPARE2_LSB        (*(reg8 *)  HORIZ_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define HORIZ_COMPARE2_LSB_PTR    ((reg8 *)   HORIZ_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define HORIZ_COUNTERCAP_LSB      (*(reg8 *)  HORIZ_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define HORIZ_COUNTERCAP_LSB_PTR  ((reg8 *)   HORIZ_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define HORIZ_COUNTER_LSB         (*(reg8 *)  HORIZ_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define HORIZ_COUNTER_LSB_PTR     ((reg8 *)   HORIZ_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define HORIZ_CAPTURE_LSB         (*(reg8 *)  HORIZ_PWMUDB_sP8_pwmdp_u0__F1_REG)
       #define HORIZ_CAPTURE_LSB_PTR     ((reg8 *)   HORIZ_PWMUDB_sP8_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(HORIZ_PWMModeIsCenterAligned)
               #define HORIZ_PERIOD_LSB      (*(reg16 *) HORIZ_PWMUDB_sP8_pwmdp_u0__D1_REG)
               #define HORIZ_PERIOD_LSB_PTR  ((reg16 *)  HORIZ_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #else
               #define HORIZ_PERIOD_LSB      (*(reg16 *) HORIZ_PWMUDB_sP8_pwmdp_u0__F0_REG)
               #define HORIZ_PERIOD_LSB_PTR  ((reg16 *)  HORIZ_PWMUDB_sP8_pwmdp_u0__F0_REG)
            #endif /* (HORIZ_PWMModeIsCenterAligned) */

            #define HORIZ_COMPARE1_LSB       (*(reg16 *) HORIZ_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define HORIZ_COMPARE1_LSB_PTR   ((reg16 *)  HORIZ_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define HORIZ_COMPARE2_LSB       (*(reg16 *) HORIZ_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define HORIZ_COMPARE2_LSB_PTR   ((reg16 *)  HORIZ_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define HORIZ_COUNTERCAP_LSB     (*(reg16 *) HORIZ_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define HORIZ_COUNTERCAP_LSB_PTR ((reg16 *)  HORIZ_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define HORIZ_COUNTER_LSB        (*(reg16 *) HORIZ_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define HORIZ_COUNTER_LSB_PTR    ((reg16 *)  HORIZ_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define HORIZ_CAPTURE_LSB        (*(reg16 *) HORIZ_PWMUDB_sP8_pwmdp_u0__F1_REG)
            #define HORIZ_CAPTURE_LSB_PTR    ((reg16 *)  HORIZ_PWMUDB_sP8_pwmdp_u0__F1_REG)
        #else
            #if(HORIZ_PWMModeIsCenterAligned)
               #define HORIZ_PERIOD_LSB      (*(reg16 *) HORIZ_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
               #define HORIZ_PERIOD_LSB_PTR  ((reg16 *)  HORIZ_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #else
               #define HORIZ_PERIOD_LSB      (*(reg16 *) HORIZ_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
               #define HORIZ_PERIOD_LSB_PTR  ((reg16 *)  HORIZ_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
            #endif /* (HORIZ_PWMModeIsCenterAligned) */

            #define HORIZ_COMPARE1_LSB       (*(reg16 *) HORIZ_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define HORIZ_COMPARE1_LSB_PTR   ((reg16 *)  HORIZ_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define HORIZ_COMPARE2_LSB       (*(reg16 *) HORIZ_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define HORIZ_COMPARE2_LSB_PTR   ((reg16 *)  HORIZ_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define HORIZ_COUNTERCAP_LSB     (*(reg16 *) HORIZ_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define HORIZ_COUNTERCAP_LSB_PTR ((reg16 *)  HORIZ_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define HORIZ_COUNTER_LSB        (*(reg16 *) HORIZ_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define HORIZ_COUNTER_LSB_PTR    ((reg16 *)  HORIZ_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define HORIZ_CAPTURE_LSB        (*(reg16 *) HORIZ_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
            #define HORIZ_CAPTURE_LSB_PTR    ((reg16 *)  HORIZ_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define HORIZ_AUX_CONTROLDP1          (*(reg8 *)  HORIZ_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)
       #define HORIZ_AUX_CONTROLDP1_PTR      ((reg8 *)   HORIZ_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (HORIZ_Resolution == 8) */

   #define HORIZ_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  HORIZ_PWMUDB_sP8_pwmdp_u0__A1_REG)
   #define HORIZ_AUX_CONTROLDP0          (*(reg8 *)  HORIZ_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)
   #define HORIZ_AUX_CONTROLDP0_PTR      ((reg8 *)   HORIZ_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (HORIZ_UsingFixedFunction) */

#if(HORIZ_KillModeMinTime )
    #define HORIZ_KILLMODEMINTIME        (*(reg8 *)  HORIZ_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define HORIZ_KILLMODEMINTIME_PTR    ((reg8 *)   HORIZ_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (HORIZ_KillModeMinTime ) */

#if(HORIZ_DeadBandMode == HORIZ__B_PWM__DBM_256_CLOCKS)
    #define HORIZ_DEADBAND_COUNT         (*(reg8 *)  HORIZ_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define HORIZ_DEADBAND_COUNT_PTR     ((reg8 *)   HORIZ_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define HORIZ_DEADBAND_LSB_PTR       ((reg8 *)   HORIZ_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define HORIZ_DEADBAND_LSB           (*(reg8 *)  HORIZ_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(HORIZ_DeadBandMode == HORIZ__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (HORIZ_UsingFixedFunction)
        #define HORIZ_DEADBAND_COUNT         (*(reg8 *)  HORIZ_PWMHW__CFG0)
        #define HORIZ_DEADBAND_COUNT_PTR     ((reg8 *)   HORIZ_PWMHW__CFG0)
        #define HORIZ_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << HORIZ_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define HORIZ_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define HORIZ_DEADBAND_COUNT         (*(reg8 *)  HORIZ_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define HORIZ_DEADBAND_COUNT_PTR     ((reg8 *)   HORIZ_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define HORIZ_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << HORIZ_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define HORIZ_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (HORIZ_UsingFixedFunction) */
#endif /* (HORIZ_DeadBandMode == HORIZ__B_PWM__DBM_256_CLOCKS) */



#if (HORIZ_UsingFixedFunction)
    #define HORIZ_STATUS                 (*(reg8 *) HORIZ_PWMHW__SR0)
    #define HORIZ_STATUS_PTR             ((reg8 *) HORIZ_PWMHW__SR0)
    #define HORIZ_STATUS_MASK            (*(reg8 *) HORIZ_PWMHW__SR0)
    #define HORIZ_STATUS_MASK_PTR        ((reg8 *) HORIZ_PWMHW__SR0)
    #define HORIZ_CONTROL                (*(reg8 *) HORIZ_PWMHW__CFG0)
    #define HORIZ_CONTROL_PTR            ((reg8 *) HORIZ_PWMHW__CFG0)
    #define HORIZ_CONTROL2               (*(reg8 *) HORIZ_PWMHW__CFG1)
    #define HORIZ_CONTROL3               (*(reg8 *) HORIZ_PWMHW__CFG2)
    #define HORIZ_GLOBAL_ENABLE          (*(reg8 *) HORIZ_PWMHW__PM_ACT_CFG)
    #define HORIZ_GLOBAL_ENABLE_PTR      ( (reg8 *) HORIZ_PWMHW__PM_ACT_CFG)
    #define HORIZ_GLOBAL_STBY_ENABLE     (*(reg8 *) HORIZ_PWMHW__PM_STBY_CFG)
    #define HORIZ_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) HORIZ_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define HORIZ_BLOCK_EN_MASK          (HORIZ_PWMHW__PM_ACT_MSK)
    #define HORIZ_BLOCK_STBY_EN_MASK     (HORIZ_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define HORIZ_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define HORIZ_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define HORIZ_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define HORIZ_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define HORIZ_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define HORIZ_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define HORIZ_CTRL_ENABLE            (uint8)((uint8)0x01u << HORIZ_CTRL_ENABLE_SHIFT)
    #define HORIZ_CTRL_RESET             (uint8)((uint8)0x01u << HORIZ_CTRL_RESET_SHIFT)
    #define HORIZ_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << HORIZ_CTRL_CMPMODE2_SHIFT)
    #define HORIZ_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << HORIZ_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define HORIZ_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define HORIZ_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << HORIZ_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define HORIZ_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define HORIZ_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define HORIZ_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define HORIZ_STATUS_TC_INT_EN_MASK_SHIFT            (HORIZ_STATUS_TC_SHIFT - 4u)
    #define HORIZ_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define HORIZ_STATUS_CMP1_INT_EN_MASK_SHIFT          (HORIZ_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define HORIZ_STATUS_TC              (uint8)((uint8)0x01u << HORIZ_STATUS_TC_SHIFT)
    #define HORIZ_STATUS_CMP1            (uint8)((uint8)0x01u << HORIZ_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define HORIZ_STATUS_TC_INT_EN_MASK              (uint8)((uint8)HORIZ_STATUS_TC >> 4u)
    #define HORIZ_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)HORIZ_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define HORIZ_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define HORIZ_RT1_MASK              (uint8)((uint8)0x03u << HORIZ_RT1_SHIFT)
    #define HORIZ_SYNC                  (uint8)((uint8)0x03u << HORIZ_RT1_SHIFT)
    #define HORIZ_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define HORIZ_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << HORIZ_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define HORIZ_SYNCDSI_EN            (uint8)((uint8)0x0Fu << HORIZ_SYNCDSI_SHIFT)


#else
    #define HORIZ_STATUS                (*(reg8 *)   HORIZ_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define HORIZ_STATUS_PTR            ((reg8 *)    HORIZ_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define HORIZ_STATUS_MASK           (*(reg8 *)   HORIZ_PWMUDB_genblk8_stsreg__MASK_REG)
    #define HORIZ_STATUS_MASK_PTR       ((reg8 *)    HORIZ_PWMUDB_genblk8_stsreg__MASK_REG)
    #define HORIZ_STATUS_AUX_CTRL       (*(reg8 *)   HORIZ_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define HORIZ_CONTROL               (*(reg8 *)   HORIZ_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define HORIZ_CONTROL_PTR           ((reg8 *)    HORIZ_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define HORIZ_CTRL_ENABLE_SHIFT      (0x07u)
    #define HORIZ_CTRL_RESET_SHIFT       (0x06u)
    #define HORIZ_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define HORIZ_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define HORIZ_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define HORIZ_CTRL_ENABLE            (uint8)((uint8)0x01u << HORIZ_CTRL_ENABLE_SHIFT)
    #define HORIZ_CTRL_RESET             (uint8)((uint8)0x01u << HORIZ_CTRL_RESET_SHIFT)
    #define HORIZ_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << HORIZ_CTRL_CMPMODE2_SHIFT)
    #define HORIZ_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << HORIZ_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define HORIZ_STATUS_KILL_SHIFT          (0x05u)
    #define HORIZ_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define HORIZ_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define HORIZ_STATUS_TC_SHIFT            (0x02u)
    #define HORIZ_STATUS_CMP2_SHIFT          (0x01u)
    #define HORIZ_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define HORIZ_STATUS_KILL_INT_EN_MASK_SHIFT          (HORIZ_STATUS_KILL_SHIFT)
    #define HORIZ_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (HORIZ_STATUS_FIFONEMPTY_SHIFT)
    #define HORIZ_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (HORIZ_STATUS_FIFOFULL_SHIFT)
    #define HORIZ_STATUS_TC_INT_EN_MASK_SHIFT            (HORIZ_STATUS_TC_SHIFT)
    #define HORIZ_STATUS_CMP2_INT_EN_MASK_SHIFT          (HORIZ_STATUS_CMP2_SHIFT)
    #define HORIZ_STATUS_CMP1_INT_EN_MASK_SHIFT          (HORIZ_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define HORIZ_STATUS_KILL            (uint8)((uint8)0x00u << HORIZ_STATUS_KILL_SHIFT )
    #define HORIZ_STATUS_FIFOFULL        (uint8)((uint8)0x01u << HORIZ_STATUS_FIFOFULL_SHIFT)
    #define HORIZ_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << HORIZ_STATUS_FIFONEMPTY_SHIFT)
    #define HORIZ_STATUS_TC              (uint8)((uint8)0x01u << HORIZ_STATUS_TC_SHIFT)
    #define HORIZ_STATUS_CMP2            (uint8)((uint8)0x01u << HORIZ_STATUS_CMP2_SHIFT)
    #define HORIZ_STATUS_CMP1            (uint8)((uint8)0x01u << HORIZ_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define HORIZ_STATUS_KILL_INT_EN_MASK            (HORIZ_STATUS_KILL)
    #define HORIZ_STATUS_FIFOFULL_INT_EN_MASK        (HORIZ_STATUS_FIFOFULL)
    #define HORIZ_STATUS_FIFONEMPTY_INT_EN_MASK      (HORIZ_STATUS_FIFONEMPTY)
    #define HORIZ_STATUS_TC_INT_EN_MASK              (HORIZ_STATUS_TC)
    #define HORIZ_STATUS_CMP2_INT_EN_MASK            (HORIZ_STATUS_CMP2)
    #define HORIZ_STATUS_CMP1_INT_EN_MASK            (HORIZ_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define HORIZ_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define HORIZ_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define HORIZ_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define HORIZ_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define HORIZ_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* HORIZ_UsingFixedFunction */

#endif  /* CY_PWM_HORIZ_H */


/* [] END OF FILE */
