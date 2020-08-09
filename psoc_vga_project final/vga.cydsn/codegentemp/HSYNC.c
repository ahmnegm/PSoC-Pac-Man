/*******************************************************************************
* File Name: HSYNC.c
* Version 3.30
*
* Description:
*  The PWM User Module consist of an 8 or 16-bit counter with two 8 or 16-bit
*  comparitors. Each instance of this user module is capable of generating
*  two PWM outputs with the same period. The pulse width is selectable between
*  1 and 255/65535. The period is selectable between 2 and 255/65536 clocks.
*  The compare value output may be configured to be active when the present
*  counter is less than or less than/equal to the compare value.
*  A terminal count output is also provided. It generates a pulse one clock
*  width wide when the counter is equal to zero.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "HSYNC.h"

/* Error message for removed <resource> through optimization */
#ifdef HSYNC_PWMUDB_genblk1_ctrlreg__REMOVED
    #error PWM_v3_30 detected with a constant 0 for the enable or \
         constant 1 for reset. This will prevent the component from operating.
#endif /* HSYNC_PWMUDB_genblk1_ctrlreg__REMOVED */

uint8 HSYNC_initVar = 0u;


/*******************************************************************************
* Function Name: HSYNC_Start
********************************************************************************
*
* Summary:
*  The start function initializes the pwm with the default values, the
*  enables the counter to begin counting.  It does not enable interrupts,
*  the EnableInt command should be called if interrupt generation is required.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  HSYNC_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void HSYNC_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(HSYNC_initVar == 0u)
    {
        HSYNC_Init();
        HSYNC_initVar = 1u;
    }
    HSYNC_Enable();

}


/*******************************************************************************
* Function Name: HSYNC_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
*  HSYNC_Start().
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void HSYNC_Init(void) 
{
    #if (HSYNC_UsingFixedFunction || HSYNC_UseControl)
        uint8 ctrl;
    #endif /* (HSYNC_UsingFixedFunction || HSYNC_UseControl) */

    #if(!HSYNC_UsingFixedFunction)
        #if(HSYNC_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 HSYNC_interruptState;
        #endif /* (HSYNC_UseStatus) */
    #endif /* (!HSYNC_UsingFixedFunction) */

    #if (HSYNC_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        HSYNC_CONTROL |= HSYNC_CFG0_MODE;
        #if (HSYNC_DeadBand2_4)
            HSYNC_CONTROL |= HSYNC_CFG0_DB;
        #endif /* (HSYNC_DeadBand2_4) */

        ctrl = HSYNC_CONTROL3 & ((uint8 )(~HSYNC_CTRL_CMPMODE1_MASK));
        HSYNC_CONTROL3 = ctrl | HSYNC_DEFAULT_COMPARE1_MODE;

         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        HSYNC_RT1 &= ((uint8)(~HSYNC_RT1_MASK));
        HSYNC_RT1 |= HSYNC_SYNC;

        /*Enable DSI Sync all all inputs of the PWM*/
        HSYNC_RT1 &= ((uint8)(~HSYNC_SYNCDSI_MASK));
        HSYNC_RT1 |= HSYNC_SYNCDSI_EN;

    #elif (HSYNC_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = HSYNC_CONTROL & ((uint8)(~HSYNC_CTRL_CMPMODE2_MASK)) &
                ((uint8)(~HSYNC_CTRL_CMPMODE1_MASK));
        HSYNC_CONTROL = ctrl | HSYNC_DEFAULT_COMPARE2_MODE |
                                   HSYNC_DEFAULT_COMPARE1_MODE;
    #endif /* (HSYNC_UsingFixedFunction) */

    #if (!HSYNC_UsingFixedFunction)
        #if (HSYNC_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            HSYNC_AUX_CONTROLDP0 |= (HSYNC_AUX_CTRL_FIFO0_CLR);
        #else /* (HSYNC_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            HSYNC_AUX_CONTROLDP0 |= (HSYNC_AUX_CTRL_FIFO0_CLR);
            HSYNC_AUX_CONTROLDP1 |= (HSYNC_AUX_CTRL_FIFO0_CLR);
        #endif /* (HSYNC_Resolution == 8) */

        HSYNC_WriteCounter(HSYNC_INIT_PERIOD_VALUE);
    #endif /* (!HSYNC_UsingFixedFunction) */

    HSYNC_WritePeriod(HSYNC_INIT_PERIOD_VALUE);

        #if (HSYNC_UseOneCompareMode)
            HSYNC_WriteCompare(HSYNC_INIT_COMPARE_VALUE1);
        #else
            HSYNC_WriteCompare1(HSYNC_INIT_COMPARE_VALUE1);
            HSYNC_WriteCompare2(HSYNC_INIT_COMPARE_VALUE2);
        #endif /* (HSYNC_UseOneCompareMode) */

        #if (HSYNC_KillModeMinTime)
            HSYNC_WriteKillTime(HSYNC_MinimumKillTime);
        #endif /* (HSYNC_KillModeMinTime) */

        #if (HSYNC_DeadBandUsed)
            HSYNC_WriteDeadTime(HSYNC_INIT_DEAD_TIME);
        #endif /* (HSYNC_DeadBandUsed) */

    #if (HSYNC_UseStatus || HSYNC_UsingFixedFunction)
        HSYNC_SetInterruptMode(HSYNC_INIT_INTERRUPTS_MODE);
    #endif /* (HSYNC_UseStatus || HSYNC_UsingFixedFunction) */

    #if (HSYNC_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        HSYNC_GLOBAL_ENABLE |= HSYNC_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        HSYNC_CONTROL2 |= HSYNC_CTRL2_IRQ_SEL;
    #else
        #if(HSYNC_UseStatus)

            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            HSYNC_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            HSYNC_STATUS_AUX_CTRL |= HSYNC_STATUS_ACTL_INT_EN_MASK;

             /* Exit Critical Region*/
            CyExitCriticalSection(HSYNC_interruptState);

            /* Clear the FIFO to enable the HSYNC_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            HSYNC_ClearFIFO();
        #endif /* (HSYNC_UseStatus) */
    #endif /* (HSYNC_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: HSYNC_Enable
********************************************************************************
*
* Summary:
*  Enables the PWM block operation
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  This works only if software enable mode is chosen
*
*******************************************************************************/
void HSYNC_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (HSYNC_UsingFixedFunction)
        HSYNC_GLOBAL_ENABLE |= HSYNC_BLOCK_EN_MASK;
        HSYNC_GLOBAL_STBY_ENABLE |= HSYNC_BLOCK_STBY_EN_MASK;
    #endif /* (HSYNC_UsingFixedFunction) */

    /* Enable the PWM from the control register  */
    #if (HSYNC_UseControl || HSYNC_UsingFixedFunction)
        HSYNC_CONTROL |= HSYNC_CTRL_ENABLE;
    #endif /* (HSYNC_UseControl || HSYNC_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: HSYNC_Stop
********************************************************************************
*
* Summary:
*  The stop function halts the PWM, but does not change any modes or disable
*  interrupts.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  If the Enable mode is set to Hardware only then this function
*  has no effect on the operation of the PWM
*
*******************************************************************************/
void HSYNC_Stop(void) 
{
    #if (HSYNC_UseControl || HSYNC_UsingFixedFunction)
        HSYNC_CONTROL &= ((uint8)(~HSYNC_CTRL_ENABLE));
    #endif /* (HSYNC_UseControl || HSYNC_UsingFixedFunction) */

    /* Globally disable the Fixed Function Block chosen */
    #if (HSYNC_UsingFixedFunction)
        HSYNC_GLOBAL_ENABLE &= ((uint8)(~HSYNC_BLOCK_EN_MASK));
        HSYNC_GLOBAL_STBY_ENABLE &= ((uint8)(~HSYNC_BLOCK_STBY_EN_MASK));
    #endif /* (HSYNC_UsingFixedFunction) */
}

#if (HSYNC_UseOneCompareMode)
    #if (HSYNC_CompareMode1SW)


        /*******************************************************************************
        * Function Name: HSYNC_SetCompareMode
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm output when in Dither mode,
        *  Center Align Mode or One Output Mode.
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void HSYNC_SetCompareMode(uint8 comparemode) 
        {
            #if(HSYNC_UsingFixedFunction)

                #if(0 != HSYNC_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemodemasked = ((uint8)((uint8)comparemode << HSYNC_CTRL_CMPMODE1_SHIFT));
                #else
                    uint8 comparemodemasked = comparemode;
                #endif /* (0 != HSYNC_CTRL_CMPMODE1_SHIFT) */

                HSYNC_CONTROL3 &= ((uint8)(~HSYNC_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
                HSYNC_CONTROL3 |= comparemodemasked;

            #elif (HSYNC_UseControl)

                #if(0 != HSYNC_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemode1masked = ((uint8)((uint8)comparemode << HSYNC_CTRL_CMPMODE1_SHIFT)) &
                                                HSYNC_CTRL_CMPMODE1_MASK;
                #else
                    uint8 comparemode1masked = comparemode & HSYNC_CTRL_CMPMODE1_MASK;
                #endif /* (0 != HSYNC_CTRL_CMPMODE1_SHIFT) */

                #if(0 != HSYNC_CTRL_CMPMODE2_SHIFT)
                    uint8 comparemode2masked = ((uint8)((uint8)comparemode << HSYNC_CTRL_CMPMODE2_SHIFT)) &
                                               HSYNC_CTRL_CMPMODE2_MASK;
                #else
                    uint8 comparemode2masked = comparemode & HSYNC_CTRL_CMPMODE2_MASK;
                #endif /* (0 != HSYNC_CTRL_CMPMODE2_SHIFT) */

                /*Clear existing mode */
                HSYNC_CONTROL &= ((uint8)(~(HSYNC_CTRL_CMPMODE1_MASK |
                                            HSYNC_CTRL_CMPMODE2_MASK)));
                HSYNC_CONTROL |= (comparemode1masked | comparemode2masked);

            #else
                uint8 temp = comparemode;
            #endif /* (HSYNC_UsingFixedFunction) */
        }
    #endif /* HSYNC_CompareMode1SW */

#else /* UseOneCompareMode */

    #if (HSYNC_CompareMode1SW)


        /*******************************************************************************
        * Function Name: HSYNC_SetCompareMode1
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm or pwm1 output
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void HSYNC_SetCompareMode1(uint8 comparemode) 
        {
            #if(0 != HSYNC_CTRL_CMPMODE1_SHIFT)
                uint8 comparemodemasked = ((uint8)((uint8)comparemode << HSYNC_CTRL_CMPMODE1_SHIFT)) &
                                           HSYNC_CTRL_CMPMODE1_MASK;
            #else
                uint8 comparemodemasked = comparemode & HSYNC_CTRL_CMPMODE1_MASK;
            #endif /* (0 != HSYNC_CTRL_CMPMODE1_SHIFT) */

            #if (HSYNC_UseControl)
                HSYNC_CONTROL &= ((uint8)(~HSYNC_CTRL_CMPMODE1_MASK)); /*Clear existing mode */
                HSYNC_CONTROL |= comparemodemasked;
            #endif /* (HSYNC_UseControl) */
        }
    #endif /* HSYNC_CompareMode1SW */

#if (HSYNC_CompareMode2SW)


    /*******************************************************************************
    * Function Name: HSYNC_SetCompareMode2
    ********************************************************************************
    *
    * Summary:
    *  This function writes the Compare Mode for the pwm or pwm2 output
    *
    * Parameters:
    *  comparemode:  The new compare mode for the PWM output. Use the compare types
    *                defined in the H file as input arguments.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void HSYNC_SetCompareMode2(uint8 comparemode) 
    {

        #if(0 != HSYNC_CTRL_CMPMODE2_SHIFT)
            uint8 comparemodemasked = ((uint8)((uint8)comparemode << HSYNC_CTRL_CMPMODE2_SHIFT)) &
                                                 HSYNC_CTRL_CMPMODE2_MASK;
        #else
            uint8 comparemodemasked = comparemode & HSYNC_CTRL_CMPMODE2_MASK;
        #endif /* (0 != HSYNC_CTRL_CMPMODE2_SHIFT) */

        #if (HSYNC_UseControl)
            HSYNC_CONTROL &= ((uint8)(~HSYNC_CTRL_CMPMODE2_MASK)); /*Clear existing mode */
            HSYNC_CONTROL |= comparemodemasked;
        #endif /* (HSYNC_UseControl) */
    }
    #endif /*HSYNC_CompareMode2SW */

#endif /* UseOneCompareMode */


#if (!HSYNC_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: HSYNC_WriteCounter
    ********************************************************************************
    *
    * Summary:
    *  Writes a new counter value directly to the counter register. This will be
    *  implemented for that currently running period and only that period. This API
    *  is valid only for UDB implementation and not available for fixed function
    *  PWM implementation.
    *
    * Parameters:
    *  counter:  The period new period counter value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  The PWM Period will be reloaded when a counter value will be a zero
    *
    *******************************************************************************/
    void HSYNC_WriteCounter(uint8 counter) \
                                       
    {
        CY_SET_REG8(HSYNC_COUNTER_LSB_PTR, counter);
    }


    /*******************************************************************************
    * Function Name: HSYNC_ReadCounter
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current value of the counter.  It doesn't matter
    *  if the counter is enabled or running.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  The current value of the counter.
    *
    *******************************************************************************/
    uint8 HSYNC_ReadCounter(void) 
    {
        /* Force capture by reading Accumulator */
        /* Must first do a software capture to be able to read the counter */
        /* It is up to the user code to make sure there isn't already captured data in the FIFO */
          (void)CY_GET_REG8(HSYNC_COUNTERCAP_LSB_PTR_8BIT);

        /* Read the data from the FIFO */
        return (CY_GET_REG8(HSYNC_CAPTURE_LSB_PTR));
    }

    #if (HSYNC_UseStatus)


        /*******************************************************************************
        * Function Name: HSYNC_ClearFIFO
        ********************************************************************************
        *
        * Summary:
        *  This function clears all capture data from the capture FIFO
        *
        * Parameters:
        *  None
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void HSYNC_ClearFIFO(void) 
        {
            while(0u != (HSYNC_ReadStatusRegister() & HSYNC_STATUS_FIFONEMPTY))
            {
                (void)HSYNC_ReadCapture();
            }
        }

    #endif /* HSYNC_UseStatus */

#endif /* !HSYNC_UsingFixedFunction */


/*******************************************************************************
* Function Name: HSYNC_WritePeriod
********************************************************************************
*
* Summary:
*  This function is used to change the period of the counter.  The new period
*  will be loaded the next time terminal count is detected.
*
* Parameters:
*  period:  Period value. May be between 1 and (2^Resolution)-1.  A value of 0
*           will result in the counter remaining at zero.
*
* Return:
*  None
*
*******************************************************************************/
void HSYNC_WritePeriod(uint8 period) 
{
    #if(HSYNC_UsingFixedFunction)
        CY_SET_REG16(HSYNC_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG8(HSYNC_PERIOD_LSB_PTR, period);
    #endif /* (HSYNC_UsingFixedFunction) */
}

#if (HSYNC_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: HSYNC_WriteCompare
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value when the PWM is in Dither
    *  mode. The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  compared to the compare value based on the compare mode defined in
    *  Dither Mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  This function is only available if the PWM mode parameter is set to
    *  Dither Mode, Center Aligned Mode or One Output Mode
    *
    *******************************************************************************/
    void HSYNC_WriteCompare(uint8 compare) \
                                       
    {
        #if(HSYNC_UsingFixedFunction)
            CY_SET_REG16(HSYNC_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(HSYNC_COMPARE1_LSB_PTR, compare);
        #endif /* (HSYNC_UsingFixedFunction) */

        #if (HSYNC_PWMMode == HSYNC__B_PWM__DITHER)
            #if(HSYNC_UsingFixedFunction)
                CY_SET_REG16(HSYNC_COMPARE2_LSB_PTR, (uint16)(compare + 1u));
            #else
                CY_SET_REG8(HSYNC_COMPARE2_LSB_PTR, (compare + 1u));
            #endif /* (HSYNC_UsingFixedFunction) */
        #endif /* (HSYNC_PWMMode == HSYNC__B_PWM__DITHER) */
    }


#else


    /*******************************************************************************
    * Function Name: HSYNC_WriteCompare1
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value.  The compare output will
    *  reflect the new value on the next UDB clock.  The compare output will be
    *  driven high when the present counter value is less than or less than or
    *  equal to the compare register, depending on the mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void HSYNC_WriteCompare1(uint8 compare) \
                                        
    {
        #if(HSYNC_UsingFixedFunction)
            CY_SET_REG16(HSYNC_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(HSYNC_COMPARE1_LSB_PTR, compare);
        #endif /* (HSYNC_UsingFixedFunction) */
    }


    /*******************************************************************************
    * Function Name: HSYNC_WriteCompare2
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare value, for compare1 output.
    *  The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  less than or less than or equal to the compare register, depending on the
    *  mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void HSYNC_WriteCompare2(uint8 compare) \
                                        
    {
        #if(HSYNC_UsingFixedFunction)
            CY_SET_REG16(HSYNC_COMPARE2_LSB_PTR, compare);
        #else
            CY_SET_REG8(HSYNC_COMPARE2_LSB_PTR, compare);
        #endif /* (HSYNC_UsingFixedFunction) */
    }
#endif /* UseOneCompareMode */

#if (HSYNC_DeadBandUsed)


    /*******************************************************************************
    * Function Name: HSYNC_WriteDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function writes the dead-band counts to the corresponding register
    *
    * Parameters:
    *  deadtime:  Number of counts for dead time
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void HSYNC_WriteDeadTime(uint8 deadtime) 
    {
        /* If using the Dead Band 1-255 mode then just write the register */
        #if(!HSYNC_DeadBand2_4)
            CY_SET_REG8(HSYNC_DEADBAND_COUNT_PTR, deadtime);
        #else
            /* Otherwise the data has to be masked and offset */
            /* Clear existing data */
            HSYNC_DEADBAND_COUNT &= ((uint8)(~HSYNC_DEADBAND_COUNT_MASK));

            /* Set new dead time */
            #if(HSYNC_DEADBAND_COUNT_SHIFT)
                HSYNC_DEADBAND_COUNT |= ((uint8)((uint8)deadtime << HSYNC_DEADBAND_COUNT_SHIFT)) &
                                                    HSYNC_DEADBAND_COUNT_MASK;
            #else
                HSYNC_DEADBAND_COUNT |= deadtime & HSYNC_DEADBAND_COUNT_MASK;
            #endif /* (HSYNC_DEADBAND_COUNT_SHIFT) */

        #endif /* (!HSYNC_DeadBand2_4) */
    }


    /*******************************************************************************
    * Function Name: HSYNC_ReadDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function reads the dead-band counts from the corresponding register
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Dead Band Counts
    *
    *******************************************************************************/
    uint8 HSYNC_ReadDeadTime(void) 
    {
        /* If using the Dead Band 1-255 mode then just read the register */
        #if(!HSYNC_DeadBand2_4)
            return (CY_GET_REG8(HSYNC_DEADBAND_COUNT_PTR));
        #else

            /* Otherwise the data has to be masked and offset */
            #if(HSYNC_DEADBAND_COUNT_SHIFT)
                return ((uint8)(((uint8)(HSYNC_DEADBAND_COUNT & HSYNC_DEADBAND_COUNT_MASK)) >>
                                                                           HSYNC_DEADBAND_COUNT_SHIFT));
            #else
                return (HSYNC_DEADBAND_COUNT & HSYNC_DEADBAND_COUNT_MASK);
            #endif /* (HSYNC_DEADBAND_COUNT_SHIFT) */
        #endif /* (!HSYNC_DeadBand2_4) */
    }
#endif /* DeadBandUsed */

#if (HSYNC_UseStatus || HSYNC_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: HSYNC_SetInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  This function configures the interrupts mask control of theinterrupt
    *  source status register.
    *
    * Parameters:
    *  uint8 interruptMode: Bit field containing the interrupt sources enabled
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void HSYNC_SetInterruptMode(uint8 interruptMode) 
    {
        CY_SET_REG8(HSYNC_STATUS_MASK_PTR, interruptMode);
    }


    /*******************************************************************************
    * Function Name: HSYNC_ReadStatusRegister
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current state of the status register.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current status register value. The status register bits are:
    *  [7:6] : Unused(0)
    *  [5]   : Kill event output
    *  [4]   : FIFO not empty
    *  [3]   : FIFO full
    *  [2]   : Terminal count
    *  [1]   : Compare output 2
    *  [0]   : Compare output 1
    *
    *******************************************************************************/
    uint8 HSYNC_ReadStatusRegister(void) 
    {
        return (CY_GET_REG8(HSYNC_STATUS_PTR));
    }

#endif /* (HSYNC_UseStatus || HSYNC_UsingFixedFunction) */


#if (HSYNC_UseControl)


    /*******************************************************************************
    * Function Name: HSYNC_ReadControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the control register. This API is available
    *  only if the control register is not removed.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current control register value
    *
    *******************************************************************************/
    uint8 HSYNC_ReadControlRegister(void) 
    {
        uint8 result;

        result = CY_GET_REG8(HSYNC_CONTROL_PTR);
        return (result);
    }


    /*******************************************************************************
    * Function Name: HSYNC_WriteControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Sets the bit field of the control register. This API is available only if
    *  the control register is not removed.
    *
    * Parameters:
    *  uint8 control: Control register bit field, The status register bits are:
    *  [7]   : PWM Enable
    *  [6]   : Reset
    *  [5:3] : Compare Mode2
    *  [2:0] : Compare Mode2
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void HSYNC_WriteControlRegister(uint8 control) 
    {
        CY_SET_REG8(HSYNC_CONTROL_PTR, control);
    }

#endif /* (HSYNC_UseControl) */


#if (!HSYNC_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: HSYNC_ReadCapture
    ********************************************************************************
    *
    * Summary:
    *  Reads the capture value from the capture FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: The current capture value
    *
    *******************************************************************************/
    uint8 HSYNC_ReadCapture(void) 
    {
        return (CY_GET_REG8(HSYNC_CAPTURE_LSB_PTR));
    }

#endif /* (!HSYNC_UsingFixedFunction) */


#if (HSYNC_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: HSYNC_ReadCompare
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare output when the PWM Mode parameter is
    *  set to Dither mode, Center Aligned mode, or One Output mode.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value
    *
    *******************************************************************************/
    uint8 HSYNC_ReadCompare(void) 
    {
        #if(HSYNC_UsingFixedFunction)
            return ((uint8)CY_GET_REG16(HSYNC_COMPARE1_LSB_PTR));
        #else
            return (CY_GET_REG8(HSYNC_COMPARE1_LSB_PTR));
        #endif /* (HSYNC_UsingFixedFunction) */
    }

#else


    /*******************************************************************************
    * Function Name: HSYNC_ReadCompare1
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare1 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint8 HSYNC_ReadCompare1(void) 
    {
        return (CY_GET_REG8(HSYNC_COMPARE1_LSB_PTR));
    }


    /*******************************************************************************
    * Function Name: HSYNC_ReadCompare2
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare2 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint8 HSYNC_ReadCompare2(void) 
    {
        return (CY_GET_REG8(HSYNC_COMPARE2_LSB_PTR));
    }

#endif /* (HSYNC_UseOneCompareMode) */


/*******************************************************************************
* Function Name: HSYNC_ReadPeriod
********************************************************************************
*
* Summary:
*  Reads the period value used by the PWM hardware.
*
* Parameters:
*  None
*
* Return:
*  uint8/16: Period value
*
*******************************************************************************/
uint8 HSYNC_ReadPeriod(void) 
{
    #if(HSYNC_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(HSYNC_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(HSYNC_PERIOD_LSB_PTR));
    #endif /* (HSYNC_UsingFixedFunction) */
}

#if ( HSYNC_KillModeMinTime)


    /*******************************************************************************
    * Function Name: HSYNC_WriteKillTime
    ********************************************************************************
    *
    * Summary:
    *  Writes the kill time value used by the hardware when the Kill Mode
    *  is set to Minimum Time.
    *
    * Parameters:
    *  uint8: Minimum Time kill counts
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void HSYNC_WriteKillTime(uint8 killtime) 
    {
        CY_SET_REG8(HSYNC_KILLMODEMINTIME_PTR, killtime);
    }


    /*******************************************************************************
    * Function Name: HSYNC_ReadKillTime
    ********************************************************************************
    *
    * Summary:
    *  Reads the kill time value used by the hardware when the Kill Mode is set
    *  to Minimum Time.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8: The current Minimum Time kill counts
    *
    *******************************************************************************/
    uint8 HSYNC_ReadKillTime(void) 
    {
        return (CY_GET_REG8(HSYNC_KILLMODEMINTIME_PTR));
    }

#endif /* ( HSYNC_KillModeMinTime) */

/* [] END OF FILE */
