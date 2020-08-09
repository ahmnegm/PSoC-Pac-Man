/*******************************************************************************
* File Name: VSYNC.c
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

#include "VSYNC.h"

/* Error message for removed <resource> through optimization */
#ifdef VSYNC_PWMUDB_genblk1_ctrlreg__REMOVED
    #error PWM_v3_30 detected with a constant 0 for the enable or \
         constant 1 for reset. This will prevent the component from operating.
#endif /* VSYNC_PWMUDB_genblk1_ctrlreg__REMOVED */

uint8 VSYNC_initVar = 0u;


/*******************************************************************************
* Function Name: VSYNC_Start
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
*  VSYNC_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void VSYNC_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(VSYNC_initVar == 0u)
    {
        VSYNC_Init();
        VSYNC_initVar = 1u;
    }
    VSYNC_Enable();

}


/*******************************************************************************
* Function Name: VSYNC_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
*  VSYNC_Start().
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void VSYNC_Init(void) 
{
    #if (VSYNC_UsingFixedFunction || VSYNC_UseControl)
        uint8 ctrl;
    #endif /* (VSYNC_UsingFixedFunction || VSYNC_UseControl) */

    #if(!VSYNC_UsingFixedFunction)
        #if(VSYNC_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 VSYNC_interruptState;
        #endif /* (VSYNC_UseStatus) */
    #endif /* (!VSYNC_UsingFixedFunction) */

    #if (VSYNC_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        VSYNC_CONTROL |= VSYNC_CFG0_MODE;
        #if (VSYNC_DeadBand2_4)
            VSYNC_CONTROL |= VSYNC_CFG0_DB;
        #endif /* (VSYNC_DeadBand2_4) */

        ctrl = VSYNC_CONTROL3 & ((uint8 )(~VSYNC_CTRL_CMPMODE1_MASK));
        VSYNC_CONTROL3 = ctrl | VSYNC_DEFAULT_COMPARE1_MODE;

         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        VSYNC_RT1 &= ((uint8)(~VSYNC_RT1_MASK));
        VSYNC_RT1 |= VSYNC_SYNC;

        /*Enable DSI Sync all all inputs of the PWM*/
        VSYNC_RT1 &= ((uint8)(~VSYNC_SYNCDSI_MASK));
        VSYNC_RT1 |= VSYNC_SYNCDSI_EN;

    #elif (VSYNC_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = VSYNC_CONTROL & ((uint8)(~VSYNC_CTRL_CMPMODE2_MASK)) &
                ((uint8)(~VSYNC_CTRL_CMPMODE1_MASK));
        VSYNC_CONTROL = ctrl | VSYNC_DEFAULT_COMPARE2_MODE |
                                   VSYNC_DEFAULT_COMPARE1_MODE;
    #endif /* (VSYNC_UsingFixedFunction) */

    #if (!VSYNC_UsingFixedFunction)
        #if (VSYNC_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            VSYNC_AUX_CONTROLDP0 |= (VSYNC_AUX_CTRL_FIFO0_CLR);
        #else /* (VSYNC_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            VSYNC_AUX_CONTROLDP0 |= (VSYNC_AUX_CTRL_FIFO0_CLR);
            VSYNC_AUX_CONTROLDP1 |= (VSYNC_AUX_CTRL_FIFO0_CLR);
        #endif /* (VSYNC_Resolution == 8) */

        VSYNC_WriteCounter(VSYNC_INIT_PERIOD_VALUE);
    #endif /* (!VSYNC_UsingFixedFunction) */

    VSYNC_WritePeriod(VSYNC_INIT_PERIOD_VALUE);

        #if (VSYNC_UseOneCompareMode)
            VSYNC_WriteCompare(VSYNC_INIT_COMPARE_VALUE1);
        #else
            VSYNC_WriteCompare1(VSYNC_INIT_COMPARE_VALUE1);
            VSYNC_WriteCompare2(VSYNC_INIT_COMPARE_VALUE2);
        #endif /* (VSYNC_UseOneCompareMode) */

        #if (VSYNC_KillModeMinTime)
            VSYNC_WriteKillTime(VSYNC_MinimumKillTime);
        #endif /* (VSYNC_KillModeMinTime) */

        #if (VSYNC_DeadBandUsed)
            VSYNC_WriteDeadTime(VSYNC_INIT_DEAD_TIME);
        #endif /* (VSYNC_DeadBandUsed) */

    #if (VSYNC_UseStatus || VSYNC_UsingFixedFunction)
        VSYNC_SetInterruptMode(VSYNC_INIT_INTERRUPTS_MODE);
    #endif /* (VSYNC_UseStatus || VSYNC_UsingFixedFunction) */

    #if (VSYNC_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        VSYNC_GLOBAL_ENABLE |= VSYNC_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        VSYNC_CONTROL2 |= VSYNC_CTRL2_IRQ_SEL;
    #else
        #if(VSYNC_UseStatus)

            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            VSYNC_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            VSYNC_STATUS_AUX_CTRL |= VSYNC_STATUS_ACTL_INT_EN_MASK;

             /* Exit Critical Region*/
            CyExitCriticalSection(VSYNC_interruptState);

            /* Clear the FIFO to enable the VSYNC_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            VSYNC_ClearFIFO();
        #endif /* (VSYNC_UseStatus) */
    #endif /* (VSYNC_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: VSYNC_Enable
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
void VSYNC_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (VSYNC_UsingFixedFunction)
        VSYNC_GLOBAL_ENABLE |= VSYNC_BLOCK_EN_MASK;
        VSYNC_GLOBAL_STBY_ENABLE |= VSYNC_BLOCK_STBY_EN_MASK;
    #endif /* (VSYNC_UsingFixedFunction) */

    /* Enable the PWM from the control register  */
    #if (VSYNC_UseControl || VSYNC_UsingFixedFunction)
        VSYNC_CONTROL |= VSYNC_CTRL_ENABLE;
    #endif /* (VSYNC_UseControl || VSYNC_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: VSYNC_Stop
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
void VSYNC_Stop(void) 
{
    #if (VSYNC_UseControl || VSYNC_UsingFixedFunction)
        VSYNC_CONTROL &= ((uint8)(~VSYNC_CTRL_ENABLE));
    #endif /* (VSYNC_UseControl || VSYNC_UsingFixedFunction) */

    /* Globally disable the Fixed Function Block chosen */
    #if (VSYNC_UsingFixedFunction)
        VSYNC_GLOBAL_ENABLE &= ((uint8)(~VSYNC_BLOCK_EN_MASK));
        VSYNC_GLOBAL_STBY_ENABLE &= ((uint8)(~VSYNC_BLOCK_STBY_EN_MASK));
    #endif /* (VSYNC_UsingFixedFunction) */
}

#if (VSYNC_UseOneCompareMode)
    #if (VSYNC_CompareMode1SW)


        /*******************************************************************************
        * Function Name: VSYNC_SetCompareMode
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
        void VSYNC_SetCompareMode(uint8 comparemode) 
        {
            #if(VSYNC_UsingFixedFunction)

                #if(0 != VSYNC_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemodemasked = ((uint8)((uint8)comparemode << VSYNC_CTRL_CMPMODE1_SHIFT));
                #else
                    uint8 comparemodemasked = comparemode;
                #endif /* (0 != VSYNC_CTRL_CMPMODE1_SHIFT) */

                VSYNC_CONTROL3 &= ((uint8)(~VSYNC_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
                VSYNC_CONTROL3 |= comparemodemasked;

            #elif (VSYNC_UseControl)

                #if(0 != VSYNC_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemode1masked = ((uint8)((uint8)comparemode << VSYNC_CTRL_CMPMODE1_SHIFT)) &
                                                VSYNC_CTRL_CMPMODE1_MASK;
                #else
                    uint8 comparemode1masked = comparemode & VSYNC_CTRL_CMPMODE1_MASK;
                #endif /* (0 != VSYNC_CTRL_CMPMODE1_SHIFT) */

                #if(0 != VSYNC_CTRL_CMPMODE2_SHIFT)
                    uint8 comparemode2masked = ((uint8)((uint8)comparemode << VSYNC_CTRL_CMPMODE2_SHIFT)) &
                                               VSYNC_CTRL_CMPMODE2_MASK;
                #else
                    uint8 comparemode2masked = comparemode & VSYNC_CTRL_CMPMODE2_MASK;
                #endif /* (0 != VSYNC_CTRL_CMPMODE2_SHIFT) */

                /*Clear existing mode */
                VSYNC_CONTROL &= ((uint8)(~(VSYNC_CTRL_CMPMODE1_MASK |
                                            VSYNC_CTRL_CMPMODE2_MASK)));
                VSYNC_CONTROL |= (comparemode1masked | comparemode2masked);

            #else
                uint8 temp = comparemode;
            #endif /* (VSYNC_UsingFixedFunction) */
        }
    #endif /* VSYNC_CompareMode1SW */

#else /* UseOneCompareMode */

    #if (VSYNC_CompareMode1SW)


        /*******************************************************************************
        * Function Name: VSYNC_SetCompareMode1
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
        void VSYNC_SetCompareMode1(uint8 comparemode) 
        {
            #if(0 != VSYNC_CTRL_CMPMODE1_SHIFT)
                uint8 comparemodemasked = ((uint8)((uint8)comparemode << VSYNC_CTRL_CMPMODE1_SHIFT)) &
                                           VSYNC_CTRL_CMPMODE1_MASK;
            #else
                uint8 comparemodemasked = comparemode & VSYNC_CTRL_CMPMODE1_MASK;
            #endif /* (0 != VSYNC_CTRL_CMPMODE1_SHIFT) */

            #if (VSYNC_UseControl)
                VSYNC_CONTROL &= ((uint8)(~VSYNC_CTRL_CMPMODE1_MASK)); /*Clear existing mode */
                VSYNC_CONTROL |= comparemodemasked;
            #endif /* (VSYNC_UseControl) */
        }
    #endif /* VSYNC_CompareMode1SW */

#if (VSYNC_CompareMode2SW)


    /*******************************************************************************
    * Function Name: VSYNC_SetCompareMode2
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
    void VSYNC_SetCompareMode2(uint8 comparemode) 
    {

        #if(0 != VSYNC_CTRL_CMPMODE2_SHIFT)
            uint8 comparemodemasked = ((uint8)((uint8)comparemode << VSYNC_CTRL_CMPMODE2_SHIFT)) &
                                                 VSYNC_CTRL_CMPMODE2_MASK;
        #else
            uint8 comparemodemasked = comparemode & VSYNC_CTRL_CMPMODE2_MASK;
        #endif /* (0 != VSYNC_CTRL_CMPMODE2_SHIFT) */

        #if (VSYNC_UseControl)
            VSYNC_CONTROL &= ((uint8)(~VSYNC_CTRL_CMPMODE2_MASK)); /*Clear existing mode */
            VSYNC_CONTROL |= comparemodemasked;
        #endif /* (VSYNC_UseControl) */
    }
    #endif /*VSYNC_CompareMode2SW */

#endif /* UseOneCompareMode */


#if (!VSYNC_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: VSYNC_WriteCounter
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
    void VSYNC_WriteCounter(uint8 counter) \
                                       
    {
        CY_SET_REG8(VSYNC_COUNTER_LSB_PTR, counter);
    }


    /*******************************************************************************
    * Function Name: VSYNC_ReadCounter
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
    uint8 VSYNC_ReadCounter(void) 
    {
        /* Force capture by reading Accumulator */
        /* Must first do a software capture to be able to read the counter */
        /* It is up to the user code to make sure there isn't already captured data in the FIFO */
          (void)CY_GET_REG8(VSYNC_COUNTERCAP_LSB_PTR_8BIT);

        /* Read the data from the FIFO */
        return (CY_GET_REG8(VSYNC_CAPTURE_LSB_PTR));
    }

    #if (VSYNC_UseStatus)


        /*******************************************************************************
        * Function Name: VSYNC_ClearFIFO
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
        void VSYNC_ClearFIFO(void) 
        {
            while(0u != (VSYNC_ReadStatusRegister() & VSYNC_STATUS_FIFONEMPTY))
            {
                (void)VSYNC_ReadCapture();
            }
        }

    #endif /* VSYNC_UseStatus */

#endif /* !VSYNC_UsingFixedFunction */


/*******************************************************************************
* Function Name: VSYNC_WritePeriod
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
void VSYNC_WritePeriod(uint8 period) 
{
    #if(VSYNC_UsingFixedFunction)
        CY_SET_REG16(VSYNC_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG8(VSYNC_PERIOD_LSB_PTR, period);
    #endif /* (VSYNC_UsingFixedFunction) */
}

#if (VSYNC_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: VSYNC_WriteCompare
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
    void VSYNC_WriteCompare(uint8 compare) \
                                       
    {
        #if(VSYNC_UsingFixedFunction)
            CY_SET_REG16(VSYNC_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(VSYNC_COMPARE1_LSB_PTR, compare);
        #endif /* (VSYNC_UsingFixedFunction) */

        #if (VSYNC_PWMMode == VSYNC__B_PWM__DITHER)
            #if(VSYNC_UsingFixedFunction)
                CY_SET_REG16(VSYNC_COMPARE2_LSB_PTR, (uint16)(compare + 1u));
            #else
                CY_SET_REG8(VSYNC_COMPARE2_LSB_PTR, (compare + 1u));
            #endif /* (VSYNC_UsingFixedFunction) */
        #endif /* (VSYNC_PWMMode == VSYNC__B_PWM__DITHER) */
    }


#else


    /*******************************************************************************
    * Function Name: VSYNC_WriteCompare1
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
    void VSYNC_WriteCompare1(uint8 compare) \
                                        
    {
        #if(VSYNC_UsingFixedFunction)
            CY_SET_REG16(VSYNC_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(VSYNC_COMPARE1_LSB_PTR, compare);
        #endif /* (VSYNC_UsingFixedFunction) */
    }


    /*******************************************************************************
    * Function Name: VSYNC_WriteCompare2
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
    void VSYNC_WriteCompare2(uint8 compare) \
                                        
    {
        #if(VSYNC_UsingFixedFunction)
            CY_SET_REG16(VSYNC_COMPARE2_LSB_PTR, compare);
        #else
            CY_SET_REG8(VSYNC_COMPARE2_LSB_PTR, compare);
        #endif /* (VSYNC_UsingFixedFunction) */
    }
#endif /* UseOneCompareMode */

#if (VSYNC_DeadBandUsed)


    /*******************************************************************************
    * Function Name: VSYNC_WriteDeadTime
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
    void VSYNC_WriteDeadTime(uint8 deadtime) 
    {
        /* If using the Dead Band 1-255 mode then just write the register */
        #if(!VSYNC_DeadBand2_4)
            CY_SET_REG8(VSYNC_DEADBAND_COUNT_PTR, deadtime);
        #else
            /* Otherwise the data has to be masked and offset */
            /* Clear existing data */
            VSYNC_DEADBAND_COUNT &= ((uint8)(~VSYNC_DEADBAND_COUNT_MASK));

            /* Set new dead time */
            #if(VSYNC_DEADBAND_COUNT_SHIFT)
                VSYNC_DEADBAND_COUNT |= ((uint8)((uint8)deadtime << VSYNC_DEADBAND_COUNT_SHIFT)) &
                                                    VSYNC_DEADBAND_COUNT_MASK;
            #else
                VSYNC_DEADBAND_COUNT |= deadtime & VSYNC_DEADBAND_COUNT_MASK;
            #endif /* (VSYNC_DEADBAND_COUNT_SHIFT) */

        #endif /* (!VSYNC_DeadBand2_4) */
    }


    /*******************************************************************************
    * Function Name: VSYNC_ReadDeadTime
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
    uint8 VSYNC_ReadDeadTime(void) 
    {
        /* If using the Dead Band 1-255 mode then just read the register */
        #if(!VSYNC_DeadBand2_4)
            return (CY_GET_REG8(VSYNC_DEADBAND_COUNT_PTR));
        #else

            /* Otherwise the data has to be masked and offset */
            #if(VSYNC_DEADBAND_COUNT_SHIFT)
                return ((uint8)(((uint8)(VSYNC_DEADBAND_COUNT & VSYNC_DEADBAND_COUNT_MASK)) >>
                                                                           VSYNC_DEADBAND_COUNT_SHIFT));
            #else
                return (VSYNC_DEADBAND_COUNT & VSYNC_DEADBAND_COUNT_MASK);
            #endif /* (VSYNC_DEADBAND_COUNT_SHIFT) */
        #endif /* (!VSYNC_DeadBand2_4) */
    }
#endif /* DeadBandUsed */

#if (VSYNC_UseStatus || VSYNC_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: VSYNC_SetInterruptMode
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
    void VSYNC_SetInterruptMode(uint8 interruptMode) 
    {
        CY_SET_REG8(VSYNC_STATUS_MASK_PTR, interruptMode);
    }


    /*******************************************************************************
    * Function Name: VSYNC_ReadStatusRegister
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
    uint8 VSYNC_ReadStatusRegister(void) 
    {
        return (CY_GET_REG8(VSYNC_STATUS_PTR));
    }

#endif /* (VSYNC_UseStatus || VSYNC_UsingFixedFunction) */


#if (VSYNC_UseControl)


    /*******************************************************************************
    * Function Name: VSYNC_ReadControlRegister
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
    uint8 VSYNC_ReadControlRegister(void) 
    {
        uint8 result;

        result = CY_GET_REG8(VSYNC_CONTROL_PTR);
        return (result);
    }


    /*******************************************************************************
    * Function Name: VSYNC_WriteControlRegister
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
    void VSYNC_WriteControlRegister(uint8 control) 
    {
        CY_SET_REG8(VSYNC_CONTROL_PTR, control);
    }

#endif /* (VSYNC_UseControl) */


#if (!VSYNC_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: VSYNC_ReadCapture
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
    uint8 VSYNC_ReadCapture(void) 
    {
        return (CY_GET_REG8(VSYNC_CAPTURE_LSB_PTR));
    }

#endif /* (!VSYNC_UsingFixedFunction) */


#if (VSYNC_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: VSYNC_ReadCompare
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
    uint8 VSYNC_ReadCompare(void) 
    {
        #if(VSYNC_UsingFixedFunction)
            return ((uint8)CY_GET_REG16(VSYNC_COMPARE1_LSB_PTR));
        #else
            return (CY_GET_REG8(VSYNC_COMPARE1_LSB_PTR));
        #endif /* (VSYNC_UsingFixedFunction) */
    }

#else


    /*******************************************************************************
    * Function Name: VSYNC_ReadCompare1
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
    uint8 VSYNC_ReadCompare1(void) 
    {
        return (CY_GET_REG8(VSYNC_COMPARE1_LSB_PTR));
    }


    /*******************************************************************************
    * Function Name: VSYNC_ReadCompare2
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
    uint8 VSYNC_ReadCompare2(void) 
    {
        return (CY_GET_REG8(VSYNC_COMPARE2_LSB_PTR));
    }

#endif /* (VSYNC_UseOneCompareMode) */


/*******************************************************************************
* Function Name: VSYNC_ReadPeriod
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
uint8 VSYNC_ReadPeriod(void) 
{
    #if(VSYNC_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(VSYNC_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(VSYNC_PERIOD_LSB_PTR));
    #endif /* (VSYNC_UsingFixedFunction) */
}

#if ( VSYNC_KillModeMinTime)


    /*******************************************************************************
    * Function Name: VSYNC_WriteKillTime
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
    void VSYNC_WriteKillTime(uint8 killtime) 
    {
        CY_SET_REG8(VSYNC_KILLMODEMINTIME_PTR, killtime);
    }


    /*******************************************************************************
    * Function Name: VSYNC_ReadKillTime
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
    uint8 VSYNC_ReadKillTime(void) 
    {
        return (CY_GET_REG8(VSYNC_KILLMODEMINTIME_PTR));
    }

#endif /* ( VSYNC_KillModeMinTime) */

/* [] END OF FILE */
