/*******************************************************************************
* File Name: VSYNC.h
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

#if !defined(CY_PWM_VSYNC_H)
#define CY_PWM_VSYNC_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 VSYNC_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define VSYNC_Resolution                     (8u)
#define VSYNC_UsingFixedFunction             (0u)
#define VSYNC_DeadBandMode                   (0u)
#define VSYNC_KillModeMinTime                (0u)
#define VSYNC_KillMode                       (0u)
#define VSYNC_PWMMode                        (0u)
#define VSYNC_PWMModeIsCenterAligned         (0u)
#define VSYNC_DeadBandUsed                   (0u)
#define VSYNC_DeadBand2_4                    (0u)

#if !defined(VSYNC_PWMUDB_genblk8_stsreg__REMOVED)
    #define VSYNC_UseStatus                  (0u)
#else
    #define VSYNC_UseStatus                  (0u)
#endif /* !defined(VSYNC_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(VSYNC_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define VSYNC_UseControl                 (1u)
#else
    #define VSYNC_UseControl                 (0u)
#endif /* !defined(VSYNC_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define VSYNC_UseOneCompareMode              (1u)
#define VSYNC_MinimumKillTime                (1u)
#define VSYNC_EnableMode                     (0u)

#define VSYNC_CompareMode1SW                 (0u)
#define VSYNC_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define VSYNC__B_PWM__DISABLED 0
#define VSYNC__B_PWM__ASYNCHRONOUS 1
#define VSYNC__B_PWM__SINGLECYCLE 2
#define VSYNC__B_PWM__LATCHED 3
#define VSYNC__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define VSYNC__B_PWM__DBMDISABLED 0
#define VSYNC__B_PWM__DBM_2_4_CLOCKS 1
#define VSYNC__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define VSYNC__B_PWM__ONE_OUTPUT 0
#define VSYNC__B_PWM__TWO_OUTPUTS 1
#define VSYNC__B_PWM__DUAL_EDGE 2
#define VSYNC__B_PWM__CENTER_ALIGN 3
#define VSYNC__B_PWM__DITHER 5
#define VSYNC__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define VSYNC__B_PWM__LESS_THAN 1
#define VSYNC__B_PWM__LESS_THAN_OR_EQUAL 2
#define VSYNC__B_PWM__GREATER_THAN 3
#define VSYNC__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define VSYNC__B_PWM__EQUAL 0
#define VSYNC__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!VSYNC_UsingFixedFunction)
        uint8 PWMUdb;               /* PWM Current Counter value  */
        #if(!VSYNC_PWMModeIsCenterAligned)
            uint8 PWMPeriod;
        #endif /* (!VSYNC_PWMModeIsCenterAligned) */
        #if (VSYNC_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (VSYNC_UseStatus) */

        /* Backup for Deadband parameters */
        #if(VSYNC_DeadBandMode == VSYNC__B_PWM__DBM_256_CLOCKS || \
            VSYNC_DeadBandMode == VSYNC__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(VSYNC_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (VSYNC_KillModeMinTime) */

        /* Backup control register */
        #if(VSYNC_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (VSYNC_UseControl) */

    #endif /* (!VSYNC_UsingFixedFunction) */

}VSYNC_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    VSYNC_Start(void) ;
void    VSYNC_Stop(void) ;

#if (VSYNC_UseStatus || VSYNC_UsingFixedFunction)
    void  VSYNC_SetInterruptMode(uint8 interruptMode) ;
    uint8 VSYNC_ReadStatusRegister(void) ;
#endif /* (VSYNC_UseStatus || VSYNC_UsingFixedFunction) */

#define VSYNC_GetInterruptSource() VSYNC_ReadStatusRegister()

#if (VSYNC_UseControl)
    uint8 VSYNC_ReadControlRegister(void) ;
    void  VSYNC_WriteControlRegister(uint8 control)
          ;
#endif /* (VSYNC_UseControl) */

#if (VSYNC_UseOneCompareMode)
   #if (VSYNC_CompareMode1SW)
       void    VSYNC_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (VSYNC_CompareMode1SW) */
#else
    #if (VSYNC_CompareMode1SW)
        void    VSYNC_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (VSYNC_CompareMode1SW) */
    #if (VSYNC_CompareMode2SW)
        void    VSYNC_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (VSYNC_CompareMode2SW) */
#endif /* (VSYNC_UseOneCompareMode) */

#if (!VSYNC_UsingFixedFunction)
    uint8   VSYNC_ReadCounter(void) ;
    uint8 VSYNC_ReadCapture(void) ;

    #if (VSYNC_UseStatus)
            void VSYNC_ClearFIFO(void) ;
    #endif /* (VSYNC_UseStatus) */

    void    VSYNC_WriteCounter(uint8 counter)
            ;
#endif /* (!VSYNC_UsingFixedFunction) */

void    VSYNC_WritePeriod(uint8 period)
        ;
uint8 VSYNC_ReadPeriod(void) ;

#if (VSYNC_UseOneCompareMode)
    void    VSYNC_WriteCompare(uint8 compare)
            ;
    uint8 VSYNC_ReadCompare(void) ;
#else
    void    VSYNC_WriteCompare1(uint8 compare)
            ;
    uint8 VSYNC_ReadCompare1(void) ;
    void    VSYNC_WriteCompare2(uint8 compare)
            ;
    uint8 VSYNC_ReadCompare2(void) ;
#endif /* (VSYNC_UseOneCompareMode) */


#if (VSYNC_DeadBandUsed)
    void    VSYNC_WriteDeadTime(uint8 deadtime) ;
    uint8   VSYNC_ReadDeadTime(void) ;
#endif /* (VSYNC_DeadBandUsed) */

#if ( VSYNC_KillModeMinTime)
    void VSYNC_WriteKillTime(uint8 killtime) ;
    uint8 VSYNC_ReadKillTime(void) ;
#endif /* ( VSYNC_KillModeMinTime) */

void VSYNC_Init(void) ;
void VSYNC_Enable(void) ;
void VSYNC_Sleep(void) ;
void VSYNC_Wakeup(void) ;
void VSYNC_SaveConfig(void) ;
void VSYNC_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define VSYNC_INIT_PERIOD_VALUE          (6u)
#define VSYNC_INIT_COMPARE_VALUE1        (6u)
#define VSYNC_INIT_COMPARE_VALUE2        (63u)
#define VSYNC_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    VSYNC_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    VSYNC_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    VSYNC_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    VSYNC_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define VSYNC_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  VSYNC_CTRL_CMPMODE2_SHIFT)
#define VSYNC_DEFAULT_COMPARE1_MODE      (uint8)((uint8)0u <<  VSYNC_CTRL_CMPMODE1_SHIFT)
#define VSYNC_INIT_DEAD_TIME             (1u)


/********************************
*         Registers
******************************** */

#if (VSYNC_UsingFixedFunction)
   #define VSYNC_PERIOD_LSB              (*(reg16 *) VSYNC_PWMHW__PER0)
   #define VSYNC_PERIOD_LSB_PTR          ( (reg16 *) VSYNC_PWMHW__PER0)
   #define VSYNC_COMPARE1_LSB            (*(reg16 *) VSYNC_PWMHW__CNT_CMP0)
   #define VSYNC_COMPARE1_LSB_PTR        ( (reg16 *) VSYNC_PWMHW__CNT_CMP0)
   #define VSYNC_COMPARE2_LSB            (0x00u)
   #define VSYNC_COMPARE2_LSB_PTR        (0x00u)
   #define VSYNC_COUNTER_LSB             (*(reg16 *) VSYNC_PWMHW__CNT_CMP0)
   #define VSYNC_COUNTER_LSB_PTR         ( (reg16 *) VSYNC_PWMHW__CNT_CMP0)
   #define VSYNC_CAPTURE_LSB             (*(reg16 *) VSYNC_PWMHW__CAP0)
   #define VSYNC_CAPTURE_LSB_PTR         ( (reg16 *) VSYNC_PWMHW__CAP0)
   #define VSYNC_RT1                     (*(reg8 *)  VSYNC_PWMHW__RT1)
   #define VSYNC_RT1_PTR                 ( (reg8 *)  VSYNC_PWMHW__RT1)

#else
   #if (VSYNC_Resolution == 8u) /* 8bit - PWM */

       #if(VSYNC_PWMModeIsCenterAligned)
           #define VSYNC_PERIOD_LSB      (*(reg8 *)  VSYNC_PWMUDB_sP8_pwmdp_u0__D1_REG)
           #define VSYNC_PERIOD_LSB_PTR  ((reg8 *)   VSYNC_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #else
           #define VSYNC_PERIOD_LSB      (*(reg8 *)  VSYNC_PWMUDB_sP8_pwmdp_u0__F0_REG)
           #define VSYNC_PERIOD_LSB_PTR  ((reg8 *)   VSYNC_PWMUDB_sP8_pwmdp_u0__F0_REG)
       #endif /* (VSYNC_PWMModeIsCenterAligned) */

       #define VSYNC_COMPARE1_LSB        (*(reg8 *)  VSYNC_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define VSYNC_COMPARE1_LSB_PTR    ((reg8 *)   VSYNC_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define VSYNC_COMPARE2_LSB        (*(reg8 *)  VSYNC_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define VSYNC_COMPARE2_LSB_PTR    ((reg8 *)   VSYNC_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define VSYNC_COUNTERCAP_LSB      (*(reg8 *)  VSYNC_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define VSYNC_COUNTERCAP_LSB_PTR  ((reg8 *)   VSYNC_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define VSYNC_COUNTER_LSB         (*(reg8 *)  VSYNC_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define VSYNC_COUNTER_LSB_PTR     ((reg8 *)   VSYNC_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define VSYNC_CAPTURE_LSB         (*(reg8 *)  VSYNC_PWMUDB_sP8_pwmdp_u0__F1_REG)
       #define VSYNC_CAPTURE_LSB_PTR     ((reg8 *)   VSYNC_PWMUDB_sP8_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(VSYNC_PWMModeIsCenterAligned)
               #define VSYNC_PERIOD_LSB      (*(reg16 *) VSYNC_PWMUDB_sP8_pwmdp_u0__D1_REG)
               #define VSYNC_PERIOD_LSB_PTR  ((reg16 *)  VSYNC_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #else
               #define VSYNC_PERIOD_LSB      (*(reg16 *) VSYNC_PWMUDB_sP8_pwmdp_u0__F0_REG)
               #define VSYNC_PERIOD_LSB_PTR  ((reg16 *)  VSYNC_PWMUDB_sP8_pwmdp_u0__F0_REG)
            #endif /* (VSYNC_PWMModeIsCenterAligned) */

            #define VSYNC_COMPARE1_LSB       (*(reg16 *) VSYNC_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define VSYNC_COMPARE1_LSB_PTR   ((reg16 *)  VSYNC_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define VSYNC_COMPARE2_LSB       (*(reg16 *) VSYNC_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define VSYNC_COMPARE2_LSB_PTR   ((reg16 *)  VSYNC_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define VSYNC_COUNTERCAP_LSB     (*(reg16 *) VSYNC_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define VSYNC_COUNTERCAP_LSB_PTR ((reg16 *)  VSYNC_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define VSYNC_COUNTER_LSB        (*(reg16 *) VSYNC_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define VSYNC_COUNTER_LSB_PTR    ((reg16 *)  VSYNC_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define VSYNC_CAPTURE_LSB        (*(reg16 *) VSYNC_PWMUDB_sP8_pwmdp_u0__F1_REG)
            #define VSYNC_CAPTURE_LSB_PTR    ((reg16 *)  VSYNC_PWMUDB_sP8_pwmdp_u0__F1_REG)
        #else
            #if(VSYNC_PWMModeIsCenterAligned)
               #define VSYNC_PERIOD_LSB      (*(reg16 *) VSYNC_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
               #define VSYNC_PERIOD_LSB_PTR  ((reg16 *)  VSYNC_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #else
               #define VSYNC_PERIOD_LSB      (*(reg16 *) VSYNC_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
               #define VSYNC_PERIOD_LSB_PTR  ((reg16 *)  VSYNC_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
            #endif /* (VSYNC_PWMModeIsCenterAligned) */

            #define VSYNC_COMPARE1_LSB       (*(reg16 *) VSYNC_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define VSYNC_COMPARE1_LSB_PTR   ((reg16 *)  VSYNC_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define VSYNC_COMPARE2_LSB       (*(reg16 *) VSYNC_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define VSYNC_COMPARE2_LSB_PTR   ((reg16 *)  VSYNC_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define VSYNC_COUNTERCAP_LSB     (*(reg16 *) VSYNC_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define VSYNC_COUNTERCAP_LSB_PTR ((reg16 *)  VSYNC_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define VSYNC_COUNTER_LSB        (*(reg16 *) VSYNC_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define VSYNC_COUNTER_LSB_PTR    ((reg16 *)  VSYNC_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define VSYNC_CAPTURE_LSB        (*(reg16 *) VSYNC_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
            #define VSYNC_CAPTURE_LSB_PTR    ((reg16 *)  VSYNC_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define VSYNC_AUX_CONTROLDP1          (*(reg8 *)  VSYNC_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)
       #define VSYNC_AUX_CONTROLDP1_PTR      ((reg8 *)   VSYNC_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (VSYNC_Resolution == 8) */

   #define VSYNC_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  VSYNC_PWMUDB_sP8_pwmdp_u0__A1_REG)
   #define VSYNC_AUX_CONTROLDP0          (*(reg8 *)  VSYNC_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)
   #define VSYNC_AUX_CONTROLDP0_PTR      ((reg8 *)   VSYNC_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (VSYNC_UsingFixedFunction) */

#if(VSYNC_KillModeMinTime )
    #define VSYNC_KILLMODEMINTIME        (*(reg8 *)  VSYNC_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define VSYNC_KILLMODEMINTIME_PTR    ((reg8 *)   VSYNC_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (VSYNC_KillModeMinTime ) */

#if(VSYNC_DeadBandMode == VSYNC__B_PWM__DBM_256_CLOCKS)
    #define VSYNC_DEADBAND_COUNT         (*(reg8 *)  VSYNC_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define VSYNC_DEADBAND_COUNT_PTR     ((reg8 *)   VSYNC_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define VSYNC_DEADBAND_LSB_PTR       ((reg8 *)   VSYNC_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define VSYNC_DEADBAND_LSB           (*(reg8 *)  VSYNC_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(VSYNC_DeadBandMode == VSYNC__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (VSYNC_UsingFixedFunction)
        #define VSYNC_DEADBAND_COUNT         (*(reg8 *)  VSYNC_PWMHW__CFG0)
        #define VSYNC_DEADBAND_COUNT_PTR     ((reg8 *)   VSYNC_PWMHW__CFG0)
        #define VSYNC_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << VSYNC_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define VSYNC_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define VSYNC_DEADBAND_COUNT         (*(reg8 *)  VSYNC_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define VSYNC_DEADBAND_COUNT_PTR     ((reg8 *)   VSYNC_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define VSYNC_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << VSYNC_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define VSYNC_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (VSYNC_UsingFixedFunction) */
#endif /* (VSYNC_DeadBandMode == VSYNC__B_PWM__DBM_256_CLOCKS) */



#if (VSYNC_UsingFixedFunction)
    #define VSYNC_STATUS                 (*(reg8 *) VSYNC_PWMHW__SR0)
    #define VSYNC_STATUS_PTR             ((reg8 *) VSYNC_PWMHW__SR0)
    #define VSYNC_STATUS_MASK            (*(reg8 *) VSYNC_PWMHW__SR0)
    #define VSYNC_STATUS_MASK_PTR        ((reg8 *) VSYNC_PWMHW__SR0)
    #define VSYNC_CONTROL                (*(reg8 *) VSYNC_PWMHW__CFG0)
    #define VSYNC_CONTROL_PTR            ((reg8 *) VSYNC_PWMHW__CFG0)
    #define VSYNC_CONTROL2               (*(reg8 *) VSYNC_PWMHW__CFG1)
    #define VSYNC_CONTROL3               (*(reg8 *) VSYNC_PWMHW__CFG2)
    #define VSYNC_GLOBAL_ENABLE          (*(reg8 *) VSYNC_PWMHW__PM_ACT_CFG)
    #define VSYNC_GLOBAL_ENABLE_PTR      ( (reg8 *) VSYNC_PWMHW__PM_ACT_CFG)
    #define VSYNC_GLOBAL_STBY_ENABLE     (*(reg8 *) VSYNC_PWMHW__PM_STBY_CFG)
    #define VSYNC_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) VSYNC_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define VSYNC_BLOCK_EN_MASK          (VSYNC_PWMHW__PM_ACT_MSK)
    #define VSYNC_BLOCK_STBY_EN_MASK     (VSYNC_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define VSYNC_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define VSYNC_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define VSYNC_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define VSYNC_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define VSYNC_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define VSYNC_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define VSYNC_CTRL_ENABLE            (uint8)((uint8)0x01u << VSYNC_CTRL_ENABLE_SHIFT)
    #define VSYNC_CTRL_RESET             (uint8)((uint8)0x01u << VSYNC_CTRL_RESET_SHIFT)
    #define VSYNC_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << VSYNC_CTRL_CMPMODE2_SHIFT)
    #define VSYNC_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << VSYNC_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define VSYNC_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define VSYNC_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << VSYNC_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define VSYNC_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define VSYNC_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define VSYNC_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define VSYNC_STATUS_TC_INT_EN_MASK_SHIFT            (VSYNC_STATUS_TC_SHIFT - 4u)
    #define VSYNC_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define VSYNC_STATUS_CMP1_INT_EN_MASK_SHIFT          (VSYNC_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define VSYNC_STATUS_TC              (uint8)((uint8)0x01u << VSYNC_STATUS_TC_SHIFT)
    #define VSYNC_STATUS_CMP1            (uint8)((uint8)0x01u << VSYNC_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define VSYNC_STATUS_TC_INT_EN_MASK              (uint8)((uint8)VSYNC_STATUS_TC >> 4u)
    #define VSYNC_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)VSYNC_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define VSYNC_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define VSYNC_RT1_MASK              (uint8)((uint8)0x03u << VSYNC_RT1_SHIFT)
    #define VSYNC_SYNC                  (uint8)((uint8)0x03u << VSYNC_RT1_SHIFT)
    #define VSYNC_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define VSYNC_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << VSYNC_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define VSYNC_SYNCDSI_EN            (uint8)((uint8)0x0Fu << VSYNC_SYNCDSI_SHIFT)


#else
    #define VSYNC_STATUS                (*(reg8 *)   VSYNC_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define VSYNC_STATUS_PTR            ((reg8 *)    VSYNC_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define VSYNC_STATUS_MASK           (*(reg8 *)   VSYNC_PWMUDB_genblk8_stsreg__MASK_REG)
    #define VSYNC_STATUS_MASK_PTR       ((reg8 *)    VSYNC_PWMUDB_genblk8_stsreg__MASK_REG)
    #define VSYNC_STATUS_AUX_CTRL       (*(reg8 *)   VSYNC_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define VSYNC_CONTROL               (*(reg8 *)   VSYNC_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define VSYNC_CONTROL_PTR           ((reg8 *)    VSYNC_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define VSYNC_CTRL_ENABLE_SHIFT      (0x07u)
    #define VSYNC_CTRL_RESET_SHIFT       (0x06u)
    #define VSYNC_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define VSYNC_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define VSYNC_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define VSYNC_CTRL_ENABLE            (uint8)((uint8)0x01u << VSYNC_CTRL_ENABLE_SHIFT)
    #define VSYNC_CTRL_RESET             (uint8)((uint8)0x01u << VSYNC_CTRL_RESET_SHIFT)
    #define VSYNC_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << VSYNC_CTRL_CMPMODE2_SHIFT)
    #define VSYNC_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << VSYNC_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define VSYNC_STATUS_KILL_SHIFT          (0x05u)
    #define VSYNC_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define VSYNC_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define VSYNC_STATUS_TC_SHIFT            (0x02u)
    #define VSYNC_STATUS_CMP2_SHIFT          (0x01u)
    #define VSYNC_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define VSYNC_STATUS_KILL_INT_EN_MASK_SHIFT          (VSYNC_STATUS_KILL_SHIFT)
    #define VSYNC_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (VSYNC_STATUS_FIFONEMPTY_SHIFT)
    #define VSYNC_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (VSYNC_STATUS_FIFOFULL_SHIFT)
    #define VSYNC_STATUS_TC_INT_EN_MASK_SHIFT            (VSYNC_STATUS_TC_SHIFT)
    #define VSYNC_STATUS_CMP2_INT_EN_MASK_SHIFT          (VSYNC_STATUS_CMP2_SHIFT)
    #define VSYNC_STATUS_CMP1_INT_EN_MASK_SHIFT          (VSYNC_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define VSYNC_STATUS_KILL            (uint8)((uint8)0x00u << VSYNC_STATUS_KILL_SHIFT )
    #define VSYNC_STATUS_FIFOFULL        (uint8)((uint8)0x01u << VSYNC_STATUS_FIFOFULL_SHIFT)
    #define VSYNC_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << VSYNC_STATUS_FIFONEMPTY_SHIFT)
    #define VSYNC_STATUS_TC              (uint8)((uint8)0x01u << VSYNC_STATUS_TC_SHIFT)
    #define VSYNC_STATUS_CMP2            (uint8)((uint8)0x01u << VSYNC_STATUS_CMP2_SHIFT)
    #define VSYNC_STATUS_CMP1            (uint8)((uint8)0x01u << VSYNC_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define VSYNC_STATUS_KILL_INT_EN_MASK            (VSYNC_STATUS_KILL)
    #define VSYNC_STATUS_FIFOFULL_INT_EN_MASK        (VSYNC_STATUS_FIFOFULL)
    #define VSYNC_STATUS_FIFONEMPTY_INT_EN_MASK      (VSYNC_STATUS_FIFONEMPTY)
    #define VSYNC_STATUS_TC_INT_EN_MASK              (VSYNC_STATUS_TC)
    #define VSYNC_STATUS_CMP2_INT_EN_MASK            (VSYNC_STATUS_CMP2)
    #define VSYNC_STATUS_CMP1_INT_EN_MASK            (VSYNC_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define VSYNC_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define VSYNC_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define VSYNC_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define VSYNC_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define VSYNC_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* VSYNC_UsingFixedFunction */

#endif  /* CY_PWM_VSYNC_H */


/* [] END OF FILE */
