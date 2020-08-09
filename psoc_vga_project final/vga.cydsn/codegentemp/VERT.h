/*******************************************************************************
* File Name: VERT.h
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

#if !defined(CY_PWM_VERT_H)
#define CY_PWM_VERT_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 VERT_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define VERT_Resolution                     (16u)
#define VERT_UsingFixedFunction             (0u)
#define VERT_DeadBandMode                   (0u)
#define VERT_KillModeMinTime                (0u)
#define VERT_KillMode                       (0u)
#define VERT_PWMMode                        (0u)
#define VERT_PWMModeIsCenterAligned         (0u)
#define VERT_DeadBandUsed                   (0u)
#define VERT_DeadBand2_4                    (0u)

#if !defined(VERT_PWMUDB_genblk8_stsreg__REMOVED)
    #define VERT_UseStatus                  (0u)
#else
    #define VERT_UseStatus                  (0u)
#endif /* !defined(VERT_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(VERT_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define VERT_UseControl                 (1u)
#else
    #define VERT_UseControl                 (0u)
#endif /* !defined(VERT_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define VERT_UseOneCompareMode              (1u)
#define VERT_MinimumKillTime                (1u)
#define VERT_EnableMode                     (0u)

#define VERT_CompareMode1SW                 (0u)
#define VERT_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define VERT__B_PWM__DISABLED 0
#define VERT__B_PWM__ASYNCHRONOUS 1
#define VERT__B_PWM__SINGLECYCLE 2
#define VERT__B_PWM__LATCHED 3
#define VERT__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define VERT__B_PWM__DBMDISABLED 0
#define VERT__B_PWM__DBM_2_4_CLOCKS 1
#define VERT__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define VERT__B_PWM__ONE_OUTPUT 0
#define VERT__B_PWM__TWO_OUTPUTS 1
#define VERT__B_PWM__DUAL_EDGE 2
#define VERT__B_PWM__CENTER_ALIGN 3
#define VERT__B_PWM__DITHER 5
#define VERT__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define VERT__B_PWM__LESS_THAN 1
#define VERT__B_PWM__LESS_THAN_OR_EQUAL 2
#define VERT__B_PWM__GREATER_THAN 3
#define VERT__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define VERT__B_PWM__EQUAL 0
#define VERT__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!VERT_UsingFixedFunction)
        uint16 PWMUdb;               /* PWM Current Counter value  */
        #if(!VERT_PWMModeIsCenterAligned)
            uint16 PWMPeriod;
        #endif /* (!VERT_PWMModeIsCenterAligned) */
        #if (VERT_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (VERT_UseStatus) */

        /* Backup for Deadband parameters */
        #if(VERT_DeadBandMode == VERT__B_PWM__DBM_256_CLOCKS || \
            VERT_DeadBandMode == VERT__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(VERT_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (VERT_KillModeMinTime) */

        /* Backup control register */
        #if(VERT_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (VERT_UseControl) */

    #endif /* (!VERT_UsingFixedFunction) */

}VERT_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    VERT_Start(void) ;
void    VERT_Stop(void) ;

#if (VERT_UseStatus || VERT_UsingFixedFunction)
    void  VERT_SetInterruptMode(uint8 interruptMode) ;
    uint8 VERT_ReadStatusRegister(void) ;
#endif /* (VERT_UseStatus || VERT_UsingFixedFunction) */

#define VERT_GetInterruptSource() VERT_ReadStatusRegister()

#if (VERT_UseControl)
    uint8 VERT_ReadControlRegister(void) ;
    void  VERT_WriteControlRegister(uint8 control)
          ;
#endif /* (VERT_UseControl) */

#if (VERT_UseOneCompareMode)
   #if (VERT_CompareMode1SW)
       void    VERT_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (VERT_CompareMode1SW) */
#else
    #if (VERT_CompareMode1SW)
        void    VERT_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (VERT_CompareMode1SW) */
    #if (VERT_CompareMode2SW)
        void    VERT_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (VERT_CompareMode2SW) */
#endif /* (VERT_UseOneCompareMode) */

#if (!VERT_UsingFixedFunction)
    uint16   VERT_ReadCounter(void) ;
    uint16 VERT_ReadCapture(void) ;

    #if (VERT_UseStatus)
            void VERT_ClearFIFO(void) ;
    #endif /* (VERT_UseStatus) */

    void    VERT_WriteCounter(uint16 counter)
            ;
#endif /* (!VERT_UsingFixedFunction) */

void    VERT_WritePeriod(uint16 period)
        ;
uint16 VERT_ReadPeriod(void) ;

#if (VERT_UseOneCompareMode)
    void    VERT_WriteCompare(uint16 compare)
            ;
    uint16 VERT_ReadCompare(void) ;
#else
    void    VERT_WriteCompare1(uint16 compare)
            ;
    uint16 VERT_ReadCompare1(void) ;
    void    VERT_WriteCompare2(uint16 compare)
            ;
    uint16 VERT_ReadCompare2(void) ;
#endif /* (VERT_UseOneCompareMode) */


#if (VERT_DeadBandUsed)
    void    VERT_WriteDeadTime(uint8 deadtime) ;
    uint8   VERT_ReadDeadTime(void) ;
#endif /* (VERT_DeadBandUsed) */

#if ( VERT_KillModeMinTime)
    void VERT_WriteKillTime(uint8 killtime) ;
    uint8 VERT_ReadKillTime(void) ;
#endif /* ( VERT_KillModeMinTime) */

void VERT_Init(void) ;
void VERT_Enable(void) ;
void VERT_Sleep(void) ;
void VERT_Wakeup(void) ;
void VERT_SaveConfig(void) ;
void VERT_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define VERT_INIT_PERIOD_VALUE          (805u)
#define VERT_INIT_COMPARE_VALUE1        (38u)
#define VERT_INIT_COMPARE_VALUE2        (0u)
#define VERT_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    VERT_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    VERT_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    VERT_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    VERT_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define VERT_DEFAULT_COMPARE2_MODE      (uint8)((uint8)4u <<  VERT_CTRL_CMPMODE2_SHIFT)
#define VERT_DEFAULT_COMPARE1_MODE      (uint8)((uint8)1u <<  VERT_CTRL_CMPMODE1_SHIFT)
#define VERT_INIT_DEAD_TIME             (1u)


/********************************
*         Registers
******************************** */

#if (VERT_UsingFixedFunction)
   #define VERT_PERIOD_LSB              (*(reg16 *) VERT_PWMHW__PER0)
   #define VERT_PERIOD_LSB_PTR          ( (reg16 *) VERT_PWMHW__PER0)
   #define VERT_COMPARE1_LSB            (*(reg16 *) VERT_PWMHW__CNT_CMP0)
   #define VERT_COMPARE1_LSB_PTR        ( (reg16 *) VERT_PWMHW__CNT_CMP0)
   #define VERT_COMPARE2_LSB            (0x00u)
   #define VERT_COMPARE2_LSB_PTR        (0x00u)
   #define VERT_COUNTER_LSB             (*(reg16 *) VERT_PWMHW__CNT_CMP0)
   #define VERT_COUNTER_LSB_PTR         ( (reg16 *) VERT_PWMHW__CNT_CMP0)
   #define VERT_CAPTURE_LSB             (*(reg16 *) VERT_PWMHW__CAP0)
   #define VERT_CAPTURE_LSB_PTR         ( (reg16 *) VERT_PWMHW__CAP0)
   #define VERT_RT1                     (*(reg8 *)  VERT_PWMHW__RT1)
   #define VERT_RT1_PTR                 ( (reg8 *)  VERT_PWMHW__RT1)

#else
   #if (VERT_Resolution == 8u) /* 8bit - PWM */

       #if(VERT_PWMModeIsCenterAligned)
           #define VERT_PERIOD_LSB      (*(reg8 *)  VERT_PWMUDB_sP16_pwmdp_u0__D1_REG)
           #define VERT_PERIOD_LSB_PTR  ((reg8 *)   VERT_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #else
           #define VERT_PERIOD_LSB      (*(reg8 *)  VERT_PWMUDB_sP16_pwmdp_u0__F0_REG)
           #define VERT_PERIOD_LSB_PTR  ((reg8 *)   VERT_PWMUDB_sP16_pwmdp_u0__F0_REG)
       #endif /* (VERT_PWMModeIsCenterAligned) */

       #define VERT_COMPARE1_LSB        (*(reg8 *)  VERT_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define VERT_COMPARE1_LSB_PTR    ((reg8 *)   VERT_PWMUDB_sP16_pwmdp_u0__D0_REG)
       #define VERT_COMPARE2_LSB        (*(reg8 *)  VERT_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define VERT_COMPARE2_LSB_PTR    ((reg8 *)   VERT_PWMUDB_sP16_pwmdp_u0__D1_REG)
       #define VERT_COUNTERCAP_LSB      (*(reg8 *)  VERT_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define VERT_COUNTERCAP_LSB_PTR  ((reg8 *)   VERT_PWMUDB_sP16_pwmdp_u0__A1_REG)
       #define VERT_COUNTER_LSB         (*(reg8 *)  VERT_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define VERT_COUNTER_LSB_PTR     ((reg8 *)   VERT_PWMUDB_sP16_pwmdp_u0__A0_REG)
       #define VERT_CAPTURE_LSB         (*(reg8 *)  VERT_PWMUDB_sP16_pwmdp_u0__F1_REG)
       #define VERT_CAPTURE_LSB_PTR     ((reg8 *)   VERT_PWMUDB_sP16_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(VERT_PWMModeIsCenterAligned)
               #define VERT_PERIOD_LSB      (*(reg16 *) VERT_PWMUDB_sP16_pwmdp_u0__D1_REG)
               #define VERT_PERIOD_LSB_PTR  ((reg16 *)  VERT_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #else
               #define VERT_PERIOD_LSB      (*(reg16 *) VERT_PWMUDB_sP16_pwmdp_u0__F0_REG)
               #define VERT_PERIOD_LSB_PTR  ((reg16 *)  VERT_PWMUDB_sP16_pwmdp_u0__F0_REG)
            #endif /* (VERT_PWMModeIsCenterAligned) */

            #define VERT_COMPARE1_LSB       (*(reg16 *) VERT_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define VERT_COMPARE1_LSB_PTR   ((reg16 *)  VERT_PWMUDB_sP16_pwmdp_u0__D0_REG)
            #define VERT_COMPARE2_LSB       (*(reg16 *) VERT_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define VERT_COMPARE2_LSB_PTR   ((reg16 *)  VERT_PWMUDB_sP16_pwmdp_u0__D1_REG)
            #define VERT_COUNTERCAP_LSB     (*(reg16 *) VERT_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define VERT_COUNTERCAP_LSB_PTR ((reg16 *)  VERT_PWMUDB_sP16_pwmdp_u0__A1_REG)
            #define VERT_COUNTER_LSB        (*(reg16 *) VERT_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define VERT_COUNTER_LSB_PTR    ((reg16 *)  VERT_PWMUDB_sP16_pwmdp_u0__A0_REG)
            #define VERT_CAPTURE_LSB        (*(reg16 *) VERT_PWMUDB_sP16_pwmdp_u0__F1_REG)
            #define VERT_CAPTURE_LSB_PTR    ((reg16 *)  VERT_PWMUDB_sP16_pwmdp_u0__F1_REG)
        #else
            #if(VERT_PWMModeIsCenterAligned)
               #define VERT_PERIOD_LSB      (*(reg16 *) VERT_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
               #define VERT_PERIOD_LSB_PTR  ((reg16 *)  VERT_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #else
               #define VERT_PERIOD_LSB      (*(reg16 *) VERT_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
               #define VERT_PERIOD_LSB_PTR  ((reg16 *)  VERT_PWMUDB_sP16_pwmdp_u0__16BIT_F0_REG)
            #endif /* (VERT_PWMModeIsCenterAligned) */

            #define VERT_COMPARE1_LSB       (*(reg16 *) VERT_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define VERT_COMPARE1_LSB_PTR   ((reg16 *)  VERT_PWMUDB_sP16_pwmdp_u0__16BIT_D0_REG)
            #define VERT_COMPARE2_LSB       (*(reg16 *) VERT_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define VERT_COMPARE2_LSB_PTR   ((reg16 *)  VERT_PWMUDB_sP16_pwmdp_u0__16BIT_D1_REG)
            #define VERT_COUNTERCAP_LSB     (*(reg16 *) VERT_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define VERT_COUNTERCAP_LSB_PTR ((reg16 *)  VERT_PWMUDB_sP16_pwmdp_u0__16BIT_A1_REG)
            #define VERT_COUNTER_LSB        (*(reg16 *) VERT_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define VERT_COUNTER_LSB_PTR    ((reg16 *)  VERT_PWMUDB_sP16_pwmdp_u0__16BIT_A0_REG)
            #define VERT_CAPTURE_LSB        (*(reg16 *) VERT_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
            #define VERT_CAPTURE_LSB_PTR    ((reg16 *)  VERT_PWMUDB_sP16_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define VERT_AUX_CONTROLDP1          (*(reg8 *)  VERT_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)
       #define VERT_AUX_CONTROLDP1_PTR      ((reg8 *)   VERT_PWMUDB_sP16_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (VERT_Resolution == 8) */

   #define VERT_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  VERT_PWMUDB_sP16_pwmdp_u0__A1_REG)
   #define VERT_AUX_CONTROLDP0          (*(reg8 *)  VERT_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)
   #define VERT_AUX_CONTROLDP0_PTR      ((reg8 *)   VERT_PWMUDB_sP16_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (VERT_UsingFixedFunction) */

#if(VERT_KillModeMinTime )
    #define VERT_KILLMODEMINTIME        (*(reg8 *)  VERT_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define VERT_KILLMODEMINTIME_PTR    ((reg8 *)   VERT_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (VERT_KillModeMinTime ) */

#if(VERT_DeadBandMode == VERT__B_PWM__DBM_256_CLOCKS)
    #define VERT_DEADBAND_COUNT         (*(reg8 *)  VERT_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define VERT_DEADBAND_COUNT_PTR     ((reg8 *)   VERT_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define VERT_DEADBAND_LSB_PTR       ((reg8 *)   VERT_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define VERT_DEADBAND_LSB           (*(reg8 *)  VERT_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(VERT_DeadBandMode == VERT__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (VERT_UsingFixedFunction)
        #define VERT_DEADBAND_COUNT         (*(reg8 *)  VERT_PWMHW__CFG0)
        #define VERT_DEADBAND_COUNT_PTR     ((reg8 *)   VERT_PWMHW__CFG0)
        #define VERT_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << VERT_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define VERT_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define VERT_DEADBAND_COUNT         (*(reg8 *)  VERT_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define VERT_DEADBAND_COUNT_PTR     ((reg8 *)   VERT_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define VERT_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << VERT_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define VERT_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (VERT_UsingFixedFunction) */
#endif /* (VERT_DeadBandMode == VERT__B_PWM__DBM_256_CLOCKS) */



#if (VERT_UsingFixedFunction)
    #define VERT_STATUS                 (*(reg8 *) VERT_PWMHW__SR0)
    #define VERT_STATUS_PTR             ((reg8 *) VERT_PWMHW__SR0)
    #define VERT_STATUS_MASK            (*(reg8 *) VERT_PWMHW__SR0)
    #define VERT_STATUS_MASK_PTR        ((reg8 *) VERT_PWMHW__SR0)
    #define VERT_CONTROL                (*(reg8 *) VERT_PWMHW__CFG0)
    #define VERT_CONTROL_PTR            ((reg8 *) VERT_PWMHW__CFG0)
    #define VERT_CONTROL2               (*(reg8 *) VERT_PWMHW__CFG1)
    #define VERT_CONTROL3               (*(reg8 *) VERT_PWMHW__CFG2)
    #define VERT_GLOBAL_ENABLE          (*(reg8 *) VERT_PWMHW__PM_ACT_CFG)
    #define VERT_GLOBAL_ENABLE_PTR      ( (reg8 *) VERT_PWMHW__PM_ACT_CFG)
    #define VERT_GLOBAL_STBY_ENABLE     (*(reg8 *) VERT_PWMHW__PM_STBY_CFG)
    #define VERT_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) VERT_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define VERT_BLOCK_EN_MASK          (VERT_PWMHW__PM_ACT_MSK)
    #define VERT_BLOCK_STBY_EN_MASK     (VERT_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define VERT_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define VERT_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define VERT_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define VERT_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define VERT_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define VERT_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define VERT_CTRL_ENABLE            (uint8)((uint8)0x01u << VERT_CTRL_ENABLE_SHIFT)
    #define VERT_CTRL_RESET             (uint8)((uint8)0x01u << VERT_CTRL_RESET_SHIFT)
    #define VERT_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << VERT_CTRL_CMPMODE2_SHIFT)
    #define VERT_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << VERT_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define VERT_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define VERT_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << VERT_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define VERT_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define VERT_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define VERT_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define VERT_STATUS_TC_INT_EN_MASK_SHIFT            (VERT_STATUS_TC_SHIFT - 4u)
    #define VERT_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define VERT_STATUS_CMP1_INT_EN_MASK_SHIFT          (VERT_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define VERT_STATUS_TC              (uint8)((uint8)0x01u << VERT_STATUS_TC_SHIFT)
    #define VERT_STATUS_CMP1            (uint8)((uint8)0x01u << VERT_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define VERT_STATUS_TC_INT_EN_MASK              (uint8)((uint8)VERT_STATUS_TC >> 4u)
    #define VERT_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)VERT_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define VERT_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define VERT_RT1_MASK              (uint8)((uint8)0x03u << VERT_RT1_SHIFT)
    #define VERT_SYNC                  (uint8)((uint8)0x03u << VERT_RT1_SHIFT)
    #define VERT_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define VERT_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << VERT_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define VERT_SYNCDSI_EN            (uint8)((uint8)0x0Fu << VERT_SYNCDSI_SHIFT)


#else
    #define VERT_STATUS                (*(reg8 *)   VERT_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define VERT_STATUS_PTR            ((reg8 *)    VERT_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define VERT_STATUS_MASK           (*(reg8 *)   VERT_PWMUDB_genblk8_stsreg__MASK_REG)
    #define VERT_STATUS_MASK_PTR       ((reg8 *)    VERT_PWMUDB_genblk8_stsreg__MASK_REG)
    #define VERT_STATUS_AUX_CTRL       (*(reg8 *)   VERT_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define VERT_CONTROL               (*(reg8 *)   VERT_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define VERT_CONTROL_PTR           ((reg8 *)    VERT_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define VERT_CTRL_ENABLE_SHIFT      (0x07u)
    #define VERT_CTRL_RESET_SHIFT       (0x06u)
    #define VERT_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define VERT_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define VERT_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define VERT_CTRL_ENABLE            (uint8)((uint8)0x01u << VERT_CTRL_ENABLE_SHIFT)
    #define VERT_CTRL_RESET             (uint8)((uint8)0x01u << VERT_CTRL_RESET_SHIFT)
    #define VERT_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << VERT_CTRL_CMPMODE2_SHIFT)
    #define VERT_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << VERT_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define VERT_STATUS_KILL_SHIFT          (0x05u)
    #define VERT_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define VERT_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define VERT_STATUS_TC_SHIFT            (0x02u)
    #define VERT_STATUS_CMP2_SHIFT          (0x01u)
    #define VERT_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define VERT_STATUS_KILL_INT_EN_MASK_SHIFT          (VERT_STATUS_KILL_SHIFT)
    #define VERT_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (VERT_STATUS_FIFONEMPTY_SHIFT)
    #define VERT_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (VERT_STATUS_FIFOFULL_SHIFT)
    #define VERT_STATUS_TC_INT_EN_MASK_SHIFT            (VERT_STATUS_TC_SHIFT)
    #define VERT_STATUS_CMP2_INT_EN_MASK_SHIFT          (VERT_STATUS_CMP2_SHIFT)
    #define VERT_STATUS_CMP1_INT_EN_MASK_SHIFT          (VERT_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define VERT_STATUS_KILL            (uint8)((uint8)0x00u << VERT_STATUS_KILL_SHIFT )
    #define VERT_STATUS_FIFOFULL        (uint8)((uint8)0x01u << VERT_STATUS_FIFOFULL_SHIFT)
    #define VERT_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << VERT_STATUS_FIFONEMPTY_SHIFT)
    #define VERT_STATUS_TC              (uint8)((uint8)0x01u << VERT_STATUS_TC_SHIFT)
    #define VERT_STATUS_CMP2            (uint8)((uint8)0x01u << VERT_STATUS_CMP2_SHIFT)
    #define VERT_STATUS_CMP1            (uint8)((uint8)0x01u << VERT_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define VERT_STATUS_KILL_INT_EN_MASK            (VERT_STATUS_KILL)
    #define VERT_STATUS_FIFOFULL_INT_EN_MASK        (VERT_STATUS_FIFOFULL)
    #define VERT_STATUS_FIFONEMPTY_INT_EN_MASK      (VERT_STATUS_FIFONEMPTY)
    #define VERT_STATUS_TC_INT_EN_MASK              (VERT_STATUS_TC)
    #define VERT_STATUS_CMP2_INT_EN_MASK            (VERT_STATUS_CMP2)
    #define VERT_STATUS_CMP1_INT_EN_MASK            (VERT_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define VERT_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define VERT_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define VERT_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define VERT_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define VERT_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* VERT_UsingFixedFunction */

#endif  /* CY_PWM_VERT_H */


/* [] END OF FILE */
