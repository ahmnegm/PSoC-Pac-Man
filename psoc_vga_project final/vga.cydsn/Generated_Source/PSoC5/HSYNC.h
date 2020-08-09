/*******************************************************************************
* File Name: HSYNC.h
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

#if !defined(CY_PWM_HSYNC_H)
#define CY_PWM_HSYNC_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 HSYNC_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define HSYNC_Resolution                     (8u)
#define HSYNC_UsingFixedFunction             (0u)
#define HSYNC_DeadBandMode                   (0u)
#define HSYNC_KillModeMinTime                (0u)
#define HSYNC_KillMode                       (0u)
#define HSYNC_PWMMode                        (0u)
#define HSYNC_PWMModeIsCenterAligned         (0u)
#define HSYNC_DeadBandUsed                   (0u)
#define HSYNC_DeadBand2_4                    (0u)

#if !defined(HSYNC_PWMUDB_genblk8_stsreg__REMOVED)
    #define HSYNC_UseStatus                  (0u)
#else
    #define HSYNC_UseStatus                  (0u)
#endif /* !defined(HSYNC_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(HSYNC_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define HSYNC_UseControl                 (1u)
#else
    #define HSYNC_UseControl                 (0u)
#endif /* !defined(HSYNC_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define HSYNC_UseOneCompareMode              (1u)
#define HSYNC_MinimumKillTime                (1u)
#define HSYNC_EnableMode                     (0u)

#define HSYNC_CompareMode1SW                 (0u)
#define HSYNC_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define HSYNC__B_PWM__DISABLED 0
#define HSYNC__B_PWM__ASYNCHRONOUS 1
#define HSYNC__B_PWM__SINGLECYCLE 2
#define HSYNC__B_PWM__LATCHED 3
#define HSYNC__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define HSYNC__B_PWM__DBMDISABLED 0
#define HSYNC__B_PWM__DBM_2_4_CLOCKS 1
#define HSYNC__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define HSYNC__B_PWM__ONE_OUTPUT 0
#define HSYNC__B_PWM__TWO_OUTPUTS 1
#define HSYNC__B_PWM__DUAL_EDGE 2
#define HSYNC__B_PWM__CENTER_ALIGN 3
#define HSYNC__B_PWM__DITHER 5
#define HSYNC__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define HSYNC__B_PWM__LESS_THAN 1
#define HSYNC__B_PWM__LESS_THAN_OR_EQUAL 2
#define HSYNC__B_PWM__GREATER_THAN 3
#define HSYNC__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define HSYNC__B_PWM__EQUAL 0
#define HSYNC__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!HSYNC_UsingFixedFunction)
        uint8 PWMUdb;               /* PWM Current Counter value  */
        #if(!HSYNC_PWMModeIsCenterAligned)
            uint8 PWMPeriod;
        #endif /* (!HSYNC_PWMModeIsCenterAligned) */
        #if (HSYNC_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (HSYNC_UseStatus) */

        /* Backup for Deadband parameters */
        #if(HSYNC_DeadBandMode == HSYNC__B_PWM__DBM_256_CLOCKS || \
            HSYNC_DeadBandMode == HSYNC__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(HSYNC_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (HSYNC_KillModeMinTime) */

        /* Backup control register */
        #if(HSYNC_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (HSYNC_UseControl) */

    #endif /* (!HSYNC_UsingFixedFunction) */

}HSYNC_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    HSYNC_Start(void) ;
void    HSYNC_Stop(void) ;

#if (HSYNC_UseStatus || HSYNC_UsingFixedFunction)
    void  HSYNC_SetInterruptMode(uint8 interruptMode) ;
    uint8 HSYNC_ReadStatusRegister(void) ;
#endif /* (HSYNC_UseStatus || HSYNC_UsingFixedFunction) */

#define HSYNC_GetInterruptSource() HSYNC_ReadStatusRegister()

#if (HSYNC_UseControl)
    uint8 HSYNC_ReadControlRegister(void) ;
    void  HSYNC_WriteControlRegister(uint8 control)
          ;
#endif /* (HSYNC_UseControl) */

#if (HSYNC_UseOneCompareMode)
   #if (HSYNC_CompareMode1SW)
       void    HSYNC_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (HSYNC_CompareMode1SW) */
#else
    #if (HSYNC_CompareMode1SW)
        void    HSYNC_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (HSYNC_CompareMode1SW) */
    #if (HSYNC_CompareMode2SW)
        void    HSYNC_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (HSYNC_CompareMode2SW) */
#endif /* (HSYNC_UseOneCompareMode) */

#if (!HSYNC_UsingFixedFunction)
    uint8   HSYNC_ReadCounter(void) ;
    uint8 HSYNC_ReadCapture(void) ;

    #if (HSYNC_UseStatus)
            void HSYNC_ClearFIFO(void) ;
    #endif /* (HSYNC_UseStatus) */

    void    HSYNC_WriteCounter(uint8 counter)
            ;
#endif /* (!HSYNC_UsingFixedFunction) */

void    HSYNC_WritePeriod(uint8 period)
        ;
uint8 HSYNC_ReadPeriod(void) ;

#if (HSYNC_UseOneCompareMode)
    void    HSYNC_WriteCompare(uint8 compare)
            ;
    uint8 HSYNC_ReadCompare(void) ;
#else
    void    HSYNC_WriteCompare1(uint8 compare)
            ;
    uint8 HSYNC_ReadCompare1(void) ;
    void    HSYNC_WriteCompare2(uint8 compare)
            ;
    uint8 HSYNC_ReadCompare2(void) ;
#endif /* (HSYNC_UseOneCompareMode) */


#if (HSYNC_DeadBandUsed)
    void    HSYNC_WriteDeadTime(uint8 deadtime) ;
    uint8   HSYNC_ReadDeadTime(void) ;
#endif /* (HSYNC_DeadBandUsed) */

#if ( HSYNC_KillModeMinTime)
    void HSYNC_WriteKillTime(uint8 killtime) ;
    uint8 HSYNC_ReadKillTime(void) ;
#endif /* ( HSYNC_KillModeMinTime) */

void HSYNC_Init(void) ;
void HSYNC_Enable(void) ;
void HSYNC_Sleep(void) ;
void HSYNC_Wakeup(void) ;
void HSYNC_SaveConfig(void) ;
void HSYNC_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define HSYNC_INIT_PERIOD_VALUE          (17u)
#define HSYNC_INIT_COMPARE_VALUE1        (17u)
#define HSYNC_INIT_COMPARE_VALUE2        (63u)
#define HSYNC_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    HSYNC_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    HSYNC_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    HSYNC_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    HSYNC_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define HSYNC_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  HSYNC_CTRL_CMPMODE2_SHIFT)
#define HSYNC_DEFAULT_COMPARE1_MODE      (uint8)((uint8)0u <<  HSYNC_CTRL_CMPMODE1_SHIFT)
#define HSYNC_INIT_DEAD_TIME             (1u)


/********************************
*         Registers
******************************** */

#if (HSYNC_UsingFixedFunction)
   #define HSYNC_PERIOD_LSB              (*(reg16 *) HSYNC_PWMHW__PER0)
   #define HSYNC_PERIOD_LSB_PTR          ( (reg16 *) HSYNC_PWMHW__PER0)
   #define HSYNC_COMPARE1_LSB            (*(reg16 *) HSYNC_PWMHW__CNT_CMP0)
   #define HSYNC_COMPARE1_LSB_PTR        ( (reg16 *) HSYNC_PWMHW__CNT_CMP0)
   #define HSYNC_COMPARE2_LSB            (0x00u)
   #define HSYNC_COMPARE2_LSB_PTR        (0x00u)
   #define HSYNC_COUNTER_LSB             (*(reg16 *) HSYNC_PWMHW__CNT_CMP0)
   #define HSYNC_COUNTER_LSB_PTR         ( (reg16 *) HSYNC_PWMHW__CNT_CMP0)
   #define HSYNC_CAPTURE_LSB             (*(reg16 *) HSYNC_PWMHW__CAP0)
   #define HSYNC_CAPTURE_LSB_PTR         ( (reg16 *) HSYNC_PWMHW__CAP0)
   #define HSYNC_RT1                     (*(reg8 *)  HSYNC_PWMHW__RT1)
   #define HSYNC_RT1_PTR                 ( (reg8 *)  HSYNC_PWMHW__RT1)

#else
   #if (HSYNC_Resolution == 8u) /* 8bit - PWM */

       #if(HSYNC_PWMModeIsCenterAligned)
           #define HSYNC_PERIOD_LSB      (*(reg8 *)  HSYNC_PWMUDB_sP8_pwmdp_u0__D1_REG)
           #define HSYNC_PERIOD_LSB_PTR  ((reg8 *)   HSYNC_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #else
           #define HSYNC_PERIOD_LSB      (*(reg8 *)  HSYNC_PWMUDB_sP8_pwmdp_u0__F0_REG)
           #define HSYNC_PERIOD_LSB_PTR  ((reg8 *)   HSYNC_PWMUDB_sP8_pwmdp_u0__F0_REG)
       #endif /* (HSYNC_PWMModeIsCenterAligned) */

       #define HSYNC_COMPARE1_LSB        (*(reg8 *)  HSYNC_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define HSYNC_COMPARE1_LSB_PTR    ((reg8 *)   HSYNC_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define HSYNC_COMPARE2_LSB        (*(reg8 *)  HSYNC_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define HSYNC_COMPARE2_LSB_PTR    ((reg8 *)   HSYNC_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define HSYNC_COUNTERCAP_LSB      (*(reg8 *)  HSYNC_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define HSYNC_COUNTERCAP_LSB_PTR  ((reg8 *)   HSYNC_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define HSYNC_COUNTER_LSB         (*(reg8 *)  HSYNC_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define HSYNC_COUNTER_LSB_PTR     ((reg8 *)   HSYNC_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define HSYNC_CAPTURE_LSB         (*(reg8 *)  HSYNC_PWMUDB_sP8_pwmdp_u0__F1_REG)
       #define HSYNC_CAPTURE_LSB_PTR     ((reg8 *)   HSYNC_PWMUDB_sP8_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(HSYNC_PWMModeIsCenterAligned)
               #define HSYNC_PERIOD_LSB      (*(reg16 *) HSYNC_PWMUDB_sP8_pwmdp_u0__D1_REG)
               #define HSYNC_PERIOD_LSB_PTR  ((reg16 *)  HSYNC_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #else
               #define HSYNC_PERIOD_LSB      (*(reg16 *) HSYNC_PWMUDB_sP8_pwmdp_u0__F0_REG)
               #define HSYNC_PERIOD_LSB_PTR  ((reg16 *)  HSYNC_PWMUDB_sP8_pwmdp_u0__F0_REG)
            #endif /* (HSYNC_PWMModeIsCenterAligned) */

            #define HSYNC_COMPARE1_LSB       (*(reg16 *) HSYNC_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define HSYNC_COMPARE1_LSB_PTR   ((reg16 *)  HSYNC_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define HSYNC_COMPARE2_LSB       (*(reg16 *) HSYNC_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define HSYNC_COMPARE2_LSB_PTR   ((reg16 *)  HSYNC_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define HSYNC_COUNTERCAP_LSB     (*(reg16 *) HSYNC_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define HSYNC_COUNTERCAP_LSB_PTR ((reg16 *)  HSYNC_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define HSYNC_COUNTER_LSB        (*(reg16 *) HSYNC_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define HSYNC_COUNTER_LSB_PTR    ((reg16 *)  HSYNC_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define HSYNC_CAPTURE_LSB        (*(reg16 *) HSYNC_PWMUDB_sP8_pwmdp_u0__F1_REG)
            #define HSYNC_CAPTURE_LSB_PTR    ((reg16 *)  HSYNC_PWMUDB_sP8_pwmdp_u0__F1_REG)
        #else
            #if(HSYNC_PWMModeIsCenterAligned)
               #define HSYNC_PERIOD_LSB      (*(reg16 *) HSYNC_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
               #define HSYNC_PERIOD_LSB_PTR  ((reg16 *)  HSYNC_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #else
               #define HSYNC_PERIOD_LSB      (*(reg16 *) HSYNC_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
               #define HSYNC_PERIOD_LSB_PTR  ((reg16 *)  HSYNC_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
            #endif /* (HSYNC_PWMModeIsCenterAligned) */

            #define HSYNC_COMPARE1_LSB       (*(reg16 *) HSYNC_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define HSYNC_COMPARE1_LSB_PTR   ((reg16 *)  HSYNC_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define HSYNC_COMPARE2_LSB       (*(reg16 *) HSYNC_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define HSYNC_COMPARE2_LSB_PTR   ((reg16 *)  HSYNC_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define HSYNC_COUNTERCAP_LSB     (*(reg16 *) HSYNC_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define HSYNC_COUNTERCAP_LSB_PTR ((reg16 *)  HSYNC_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define HSYNC_COUNTER_LSB        (*(reg16 *) HSYNC_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define HSYNC_COUNTER_LSB_PTR    ((reg16 *)  HSYNC_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define HSYNC_CAPTURE_LSB        (*(reg16 *) HSYNC_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
            #define HSYNC_CAPTURE_LSB_PTR    ((reg16 *)  HSYNC_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define HSYNC_AUX_CONTROLDP1          (*(reg8 *)  HSYNC_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)
       #define HSYNC_AUX_CONTROLDP1_PTR      ((reg8 *)   HSYNC_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (HSYNC_Resolution == 8) */

   #define HSYNC_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  HSYNC_PWMUDB_sP8_pwmdp_u0__A1_REG)
   #define HSYNC_AUX_CONTROLDP0          (*(reg8 *)  HSYNC_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)
   #define HSYNC_AUX_CONTROLDP0_PTR      ((reg8 *)   HSYNC_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (HSYNC_UsingFixedFunction) */

#if(HSYNC_KillModeMinTime )
    #define HSYNC_KILLMODEMINTIME        (*(reg8 *)  HSYNC_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define HSYNC_KILLMODEMINTIME_PTR    ((reg8 *)   HSYNC_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (HSYNC_KillModeMinTime ) */

#if(HSYNC_DeadBandMode == HSYNC__B_PWM__DBM_256_CLOCKS)
    #define HSYNC_DEADBAND_COUNT         (*(reg8 *)  HSYNC_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define HSYNC_DEADBAND_COUNT_PTR     ((reg8 *)   HSYNC_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define HSYNC_DEADBAND_LSB_PTR       ((reg8 *)   HSYNC_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define HSYNC_DEADBAND_LSB           (*(reg8 *)  HSYNC_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(HSYNC_DeadBandMode == HSYNC__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (HSYNC_UsingFixedFunction)
        #define HSYNC_DEADBAND_COUNT         (*(reg8 *)  HSYNC_PWMHW__CFG0)
        #define HSYNC_DEADBAND_COUNT_PTR     ((reg8 *)   HSYNC_PWMHW__CFG0)
        #define HSYNC_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << HSYNC_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define HSYNC_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define HSYNC_DEADBAND_COUNT         (*(reg8 *)  HSYNC_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define HSYNC_DEADBAND_COUNT_PTR     ((reg8 *)   HSYNC_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define HSYNC_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << HSYNC_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define HSYNC_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (HSYNC_UsingFixedFunction) */
#endif /* (HSYNC_DeadBandMode == HSYNC__B_PWM__DBM_256_CLOCKS) */



#if (HSYNC_UsingFixedFunction)
    #define HSYNC_STATUS                 (*(reg8 *) HSYNC_PWMHW__SR0)
    #define HSYNC_STATUS_PTR             ((reg8 *) HSYNC_PWMHW__SR0)
    #define HSYNC_STATUS_MASK            (*(reg8 *) HSYNC_PWMHW__SR0)
    #define HSYNC_STATUS_MASK_PTR        ((reg8 *) HSYNC_PWMHW__SR0)
    #define HSYNC_CONTROL                (*(reg8 *) HSYNC_PWMHW__CFG0)
    #define HSYNC_CONTROL_PTR            ((reg8 *) HSYNC_PWMHW__CFG0)
    #define HSYNC_CONTROL2               (*(reg8 *) HSYNC_PWMHW__CFG1)
    #define HSYNC_CONTROL3               (*(reg8 *) HSYNC_PWMHW__CFG2)
    #define HSYNC_GLOBAL_ENABLE          (*(reg8 *) HSYNC_PWMHW__PM_ACT_CFG)
    #define HSYNC_GLOBAL_ENABLE_PTR      ( (reg8 *) HSYNC_PWMHW__PM_ACT_CFG)
    #define HSYNC_GLOBAL_STBY_ENABLE     (*(reg8 *) HSYNC_PWMHW__PM_STBY_CFG)
    #define HSYNC_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) HSYNC_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define HSYNC_BLOCK_EN_MASK          (HSYNC_PWMHW__PM_ACT_MSK)
    #define HSYNC_BLOCK_STBY_EN_MASK     (HSYNC_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define HSYNC_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define HSYNC_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define HSYNC_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define HSYNC_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define HSYNC_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define HSYNC_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define HSYNC_CTRL_ENABLE            (uint8)((uint8)0x01u << HSYNC_CTRL_ENABLE_SHIFT)
    #define HSYNC_CTRL_RESET             (uint8)((uint8)0x01u << HSYNC_CTRL_RESET_SHIFT)
    #define HSYNC_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << HSYNC_CTRL_CMPMODE2_SHIFT)
    #define HSYNC_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << HSYNC_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define HSYNC_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define HSYNC_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << HSYNC_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define HSYNC_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define HSYNC_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define HSYNC_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define HSYNC_STATUS_TC_INT_EN_MASK_SHIFT            (HSYNC_STATUS_TC_SHIFT - 4u)
    #define HSYNC_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define HSYNC_STATUS_CMP1_INT_EN_MASK_SHIFT          (HSYNC_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define HSYNC_STATUS_TC              (uint8)((uint8)0x01u << HSYNC_STATUS_TC_SHIFT)
    #define HSYNC_STATUS_CMP1            (uint8)((uint8)0x01u << HSYNC_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define HSYNC_STATUS_TC_INT_EN_MASK              (uint8)((uint8)HSYNC_STATUS_TC >> 4u)
    #define HSYNC_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)HSYNC_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define HSYNC_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define HSYNC_RT1_MASK              (uint8)((uint8)0x03u << HSYNC_RT1_SHIFT)
    #define HSYNC_SYNC                  (uint8)((uint8)0x03u << HSYNC_RT1_SHIFT)
    #define HSYNC_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define HSYNC_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << HSYNC_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define HSYNC_SYNCDSI_EN            (uint8)((uint8)0x0Fu << HSYNC_SYNCDSI_SHIFT)


#else
    #define HSYNC_STATUS                (*(reg8 *)   HSYNC_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define HSYNC_STATUS_PTR            ((reg8 *)    HSYNC_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define HSYNC_STATUS_MASK           (*(reg8 *)   HSYNC_PWMUDB_genblk8_stsreg__MASK_REG)
    #define HSYNC_STATUS_MASK_PTR       ((reg8 *)    HSYNC_PWMUDB_genblk8_stsreg__MASK_REG)
    #define HSYNC_STATUS_AUX_CTRL       (*(reg8 *)   HSYNC_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define HSYNC_CONTROL               (*(reg8 *)   HSYNC_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define HSYNC_CONTROL_PTR           ((reg8 *)    HSYNC_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define HSYNC_CTRL_ENABLE_SHIFT      (0x07u)
    #define HSYNC_CTRL_RESET_SHIFT       (0x06u)
    #define HSYNC_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define HSYNC_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define HSYNC_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define HSYNC_CTRL_ENABLE            (uint8)((uint8)0x01u << HSYNC_CTRL_ENABLE_SHIFT)
    #define HSYNC_CTRL_RESET             (uint8)((uint8)0x01u << HSYNC_CTRL_RESET_SHIFT)
    #define HSYNC_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << HSYNC_CTRL_CMPMODE2_SHIFT)
    #define HSYNC_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << HSYNC_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define HSYNC_STATUS_KILL_SHIFT          (0x05u)
    #define HSYNC_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define HSYNC_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define HSYNC_STATUS_TC_SHIFT            (0x02u)
    #define HSYNC_STATUS_CMP2_SHIFT          (0x01u)
    #define HSYNC_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define HSYNC_STATUS_KILL_INT_EN_MASK_SHIFT          (HSYNC_STATUS_KILL_SHIFT)
    #define HSYNC_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (HSYNC_STATUS_FIFONEMPTY_SHIFT)
    #define HSYNC_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (HSYNC_STATUS_FIFOFULL_SHIFT)
    #define HSYNC_STATUS_TC_INT_EN_MASK_SHIFT            (HSYNC_STATUS_TC_SHIFT)
    #define HSYNC_STATUS_CMP2_INT_EN_MASK_SHIFT          (HSYNC_STATUS_CMP2_SHIFT)
    #define HSYNC_STATUS_CMP1_INT_EN_MASK_SHIFT          (HSYNC_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define HSYNC_STATUS_KILL            (uint8)((uint8)0x00u << HSYNC_STATUS_KILL_SHIFT )
    #define HSYNC_STATUS_FIFOFULL        (uint8)((uint8)0x01u << HSYNC_STATUS_FIFOFULL_SHIFT)
    #define HSYNC_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << HSYNC_STATUS_FIFONEMPTY_SHIFT)
    #define HSYNC_STATUS_TC              (uint8)((uint8)0x01u << HSYNC_STATUS_TC_SHIFT)
    #define HSYNC_STATUS_CMP2            (uint8)((uint8)0x01u << HSYNC_STATUS_CMP2_SHIFT)
    #define HSYNC_STATUS_CMP1            (uint8)((uint8)0x01u << HSYNC_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define HSYNC_STATUS_KILL_INT_EN_MASK            (HSYNC_STATUS_KILL)
    #define HSYNC_STATUS_FIFOFULL_INT_EN_MASK        (HSYNC_STATUS_FIFOFULL)
    #define HSYNC_STATUS_FIFONEMPTY_INT_EN_MASK      (HSYNC_STATUS_FIFONEMPTY)
    #define HSYNC_STATUS_TC_INT_EN_MASK              (HSYNC_STATUS_TC)
    #define HSYNC_STATUS_CMP2_INT_EN_MASK            (HSYNC_STATUS_CMP2)
    #define HSYNC_STATUS_CMP1_INT_EN_MASK            (HSYNC_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define HSYNC_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define HSYNC_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define HSYNC_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define HSYNC_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define HSYNC_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* HSYNC_UsingFixedFunction */

#endif  /* CY_PWM_HSYNC_H */


/* [] END OF FILE */
