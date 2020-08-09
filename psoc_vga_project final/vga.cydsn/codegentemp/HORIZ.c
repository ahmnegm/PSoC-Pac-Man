/*******************************************************************************
* File Name: HORIZ.c
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

#include "HORIZ.h"

/* Error message for removed <resource> through optimization */
#ifdef HORIZ_PWMUDB_genblk1_ctrlreg__REMOVED
    #error PWM_v3_30 detected with a constant 0 for the enable or \
         constant 1 for reset. This will prevent the component from operating.
#endif /* HORIZ_PWMUDB_genblk1_ctrlreg__REMOVED */

uint8 HORIZ_initVar = 0u;


/*******************************************************************************
* Function Name: HORIZ_Start
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
*  HORIZ_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void HORIZ_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(HORIZ_initVar == 0u)
    {
        HORIZ_Init();
        HORIZ_initVar = 1u;
    }
    HORIZ_Enable();

}


/*******************************************************************************
* Function Name: HORIZ_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
*  HORIZ_Start().
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void HORIZ_Init(void) 
{
    #if (HORIZ_UsingFixedFunction || HORIZ_UseControl)
        uint8 ctrl;
    #endif /* (HORIZ_UsingFixedFunction || HORIZ_UseControl) */

    #if(!HORIZ_UsingFixedFunction)
        #if(HORIZ_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 HORIZ_interruptState;
        #endif /* (HORIZ_UseStatus) */
    #endif /* (!HORIZ_UsingFixedFunction) */

    #if (HORIZ_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        HORIZ_CONTROL |= HORIZ_CFG0_MODE;
        #if (HORIZ_DeadBand2_4)
            HORIZ_CONTROL |= HORIZ_CFG0_DB;
        #endif /* (HORIZ_DeadBand2_4) */

        ctrl = HORIZ_CONTROL3 & ((uint8 )(~HORIZ_CTRL_CMPMODE1_MASK));
        HORIZ_CONTROL3 = ctrl | HORIZ_DEFAULT_COMPARE1_MODE;

         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        HORIZ_RT1 &= ((uint8)(~HORIZ_RT1_MASK));
        HORIZ_RT1 |= HORIZ_SYNC;

        /*Enable DSI Sync all all inputs of the PWM*/
        HORIZ_RT1 &= ((uint8)(~HORIZ_SYNCDSI_MASK));
        HORIZ_RT1 |= HORIZ_SYNCDSI_EN;

    #elif (HORIZ_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = HORIZ_CONTROL & ((uint8)(~HORIZ_CTRL_CMPMODE2_MASK)) &
                ((uint8)(~HORIZ_CTRL_CMPMODE1_MASK));
        HORIZ_CONTROL = ctrl | HORIZ_DEFAULT_COMPARE2_MODE |
                                   HORIZ_DEFAULT_COMPARE1_MODE;
    #endif /* (HORIZ_UsingFixedFunction) */

    #if (!HORIZ_UsingFixedFunction)
        #if (HORIZ_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            HORIZ_AUX_CONTROLDP0 |= (HORIZ_AUX_CTRL_FIFO0_CLR);
        #else /* (HORIZ_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            HORIZ_AUX_CONTROLDP0 |= (HORIZ_AUX_CTRL_FIFO0_CLR);
            HORIZ_AUX_CONTROLDP1 |= (HORIZ_AUX_CTRL_FIFO0_CLR);
        #endif /* (HORIZ_Resolution == 8) */

        HORIZ_WriteCounter(HORIZ_INIT_PERIOD_VALUE);
    #endif /* (!HORIZ_UsingFixedFunction) */

    HORIZ_WritePeriod(HORIZ_INIT_PERIOD_VALUE);

        #if (HORIZ_UseOneCompareMode)
            HORIZ_WriteCompare(HORIZ_INIT_COMPARE_VALUE1);
        #else
            HORIZ_WriteCompare1(HORIZ_INIT_COMPARE_VALUE1);
            HORIZ_WriteCompare2(HORIZ_INIT_COMPARE_VALUE2);
        #endif /* (HORIZ_UseOneCompareMode) */

        #if (HORIZ_KillModeMinTime)
            HORIZ_WriteKillTime(HORIZ_MinimumKillTime);
        #endif /* (HORIZ_KillModeMinTime) */

        #if (HORIZ_DeadBandUsed)
            HORIZ_WriteDeadTime(HORIZ_INIT_DEAD_TIME);
        #endif /* (HORIZ_DeadBandUsed) */

    #if (HORIZ_UseStatus || HORIZ_UsingFixedFunction)
        HORIZ_SetInterruptMode(HORIZ_INIT_INTERRUPTS_MODE);
    #endif /* (HORIZ_UseStatus || HORIZ_UsingFixedFunction) */

    #if (HORIZ_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        HORIZ_GLOBAL_ENABLE |= HORIZ_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        HORIZ_CONTROL2 |= HORIZ_CTRL2_IRQ_SEL;
    #else
        #if(HORIZ_UseStatus)

            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            HORIZ_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            HORIZ_STATUS_AUX_CTRL |= HORIZ_STATUS_ACTL_INT_EN_MASK;

             /* Exit Critical Region*/
            CyExitCriticalSection(HORIZ_interruptState);

            /* Clear the FIFO to enable the HORIZ_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            HORIZ_ClearFIFO();
        #endif /* (HORIZ_UseStatus) */
    #endif /* (HORIZ_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: HORIZ_Enable
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
void HORIZ_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (HORIZ_UsingFixedFunction)
        HORIZ_GLOBAL_ENABLE |= HORIZ_BLOCK_EN_MASK;
        HORIZ_GLOBAL_STBY_ENABLE |= HORIZ_BLOCK_STBY_EN_MASK;
    #endif /* (HORIZ_UsingFixedFunction) */

    /* Enable the PWM from the control register  */
    #if (HORIZ_UseControl || HORIZ_UsingFixedFunction)
        HORIZ_CONTROL |= HORIZ_CTRL_ENABLE;
    #endif /* (HORIZ_UseControl || HORIZ_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: HORIZ_Stop
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
void HORIZ_Stop(void) 
{
    #if (HORIZ_UseControl || HORIZ_UsingFixedFunction)
        HORIZ_CONTROL &= ((uint8)(~HORIZ_CTRL_ENABLE));
    #endif /* (HORIZ_UseControl || HORIZ_UsingFixedFunction) */

    /* Globally disable the Fixed Function Block chosen */
    #if (HORIZ_UsingFixedFunction)
        HORIZ_GLOBAL_ENABLE &= ((uint8)(~HORIZ_BLOCK_EN_MASK));
        HORIZ_GLOBAL_STBY_ENABLE &= ((uint8)(~HORIZ_BLOCK_STBY_EN_MASK));
    #endif /* (HORIZ_UsingFixedFunction) */
}

#if (HORIZ_UseOneCompareMode)
    #if (HORIZ_CompareMode1SW)


        /*******************************************************************************
        * Function Name: HORIZ_SetCompareMode
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
        void HORIZ_SetCompareMode(uint8 comparemode) 
        {
            #if(HORIZ_UsingFixedFunction)

                #if(0 != HORIZ_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemodemasked = ((uint8)((uint8)comparemode << HORIZ_CTRL_CMPMODE1_SHIFT));
                #else
                    uint8 comparemodemasked = comparemode;
                #endif /* (0 != HORIZ_CTRL_CMPMODE1_SHIFT) */

                HORIZ_CONTROL3 &= ((uint8)(~HORIZ_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
                HORIZ_CONTROL3 |= comparemodemasked;

            #elif (HORIZ_UseControl)

                #if(0 != HORIZ_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemode1masked = ((uint8)((uint8)comparemode << HORIZ_CTRL_CMPMODE1_SHIFT)) &
                                                HORIZ_CTRL_CMPMODE1_MASK;
                #else
                    uint8 comparemode1masked = comparemode & HORIZ_CTRL_CMPMODE1_MASK;
                #endif /* (0 != HORIZ_CTRL_CMPMODE1_SHIFT) */

                #if(0 != HORIZ_CTRL_CMPMODE2_SHIFT)
                    uint8 comparemode2masked = ((uint8)((uint8)comparemode << HORIZ_CTRL_CMPMODE2_SHIFT)) &
                                               HORIZ_CTRL_CMPMODE2_MASK;
                #else
                    uint8 comparemode2masked = comparemode & HORIZ_CTRL_CMPMODE2_MASK;
                #endif /* (0 != HORIZ_CTRL_CMPMODE2_SHIFT) */

                /*Clear existing mode */
                HORIZ_CONTROL &= ((uint8)(~(HORIZ_CTRL_CMPMODE1_MASK |
                                            HORIZ_CTRL_CMPMODE2_MASK)));
                HORIZ_CONTROL |= (comparemode1masked | comparemode2masked);

            #else
                uint8 temp = comparemode;
            #endif /* (HORIZ_UsingFixedFunction) */
        }
    #endif /* HORIZ_CompareMode1SW */

#else /* UseOneCompareMode */

    #if (HORIZ_CompareMode1SW)


        /*******************************************************************************
        * Function Name: HORIZ_SetCompareMode1
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
        void HORIZ_SetCompareMode1(uint8 comparemode) 
        {
            #if(0 != HORIZ_CTRL_CMPMODE1_SHIFT)
                uint8 comparemodemasked = ((uint8)((uint8)comparemode << HORIZ_CTRL_CMPMODE1_SHIFT)) &
                                           HORIZ_CTRL_CMPMODE1_MASK;
            #else
                uint8 comparemodemasked = comparemode & HORIZ_CTRL_CMPMODE1_MASK;
            #endif /* (0 != HORIZ_CTRL_CMPMODE1_SHIFT) */

            #if (HORIZ_UseControl)
                HORIZ_CONTROL &= ((uint8)(~HORIZ_CTRL_CMPMODE1_MASK)); /*Clear existing mode */
                HORIZ_CONTROL |= comparemodemasked;
            #endif /* (HORIZ_UseControl) */
        }
    #endif /* HORIZ_CompareMode1SW */

#if (HORIZ_CompareMode2SW)


    /*******************************************************************************
    * Function Name: HORIZ_SetCompareMode2
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
    void HORIZ_SetCompareMode2(uint8 comparemode) 
    {

        #if(0 != HORIZ_CTRL_CMPMODE2_SHIFT)
            uint8 comparemodemasked = ((uint8)((uint8)comparemode << HORIZ_CTRL_CMPMODE2_SHIFT)) &
                                                 HORIZ_CTRL_CMPMODE2_MASK;
        #else
            uint8 comparemodemasked = comparemode & HORIZ_CTRL_CMPMODE2_MASK;
        #endif /* (0 != HORIZ_CTRL_CMPMODE2_SHIFT) */

        #if (HORIZ_UseControl)
            HORIZ_CONTROL &= ((uint8)(~HORIZ_CTRL_CMPMODE2_MASK)); /*Clear existing mode */
            HORIZ_CONTROL |= comparemodemasked;
        #endif /* (HORIZ_UseControl) */
    }
    #endif /*HORIZ_CompareMode2SW */

#endif /* UseOneCompareMode */


#if (!HORIZ_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: HORIZ_WriteCounter
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
    void HORIZ_WriteCounter(uint8 counter) \
                                       
    {
        CY_SET_REG8(HORIZ_COUNTER_LSB_PTR, counter);
    }


    /*******************************************************************************
    * Function Name: HORIZ_ReadCounter
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
    uint8 HORIZ_ReadCounter(void) 
    {
        /* Force capture by reading Accumulator */
        /* Must first do a software capture to be able to read the counter */
        /* It is up to the user code to make sure there isn't already captured data in the FIFO */
          (void)CY_GET_REG8(HORIZ_COUNTERCAP_LSB_PTR_8BIT);

        /* Read the data from the FIFO */
        return (CY_GET_REG8(HORIZ_CAPTURE_LSB_PTR));
    }

    #if (HORIZ_UseStatus)


        /*******************************************************************************
        * Function Name: HORIZ_ClearFIFO
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
        void HORIZ_ClearFIFO(void) 
        {
            while(0u != (HORIZ_ReadStatusRegister() & HORIZ_STATUS_FIFONEMPTY))
            {
                (void)HORIZ_ReadCapture();
            }
        }

    #endif /* HORIZ_UseStatus */

#endif /* !HORIZ_UsingFixedFunction */


/*******************************************************************************
* Function Name: HORIZ_WritePeriod
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
void HORIZ_WritePeriod(uint8 period) 
{
    #if(HORIZ_UsingFixedFunction)
        CY_SET_REG16(HORIZ_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG8(HORIZ_PERIOD_LSB_PTR, period);
    #endif /* (HORIZ_UsingFixedFunction) */
}

#if (HORIZ_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: HORIZ_WriteCompare
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
    void HORIZ_WriteCompare(uint8 compare) \
                                       
    {
        #if(HORIZ_UsingFixedFunction)
            CY_SET_REG16(HORIZ_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(HORIZ_COMPARE1_LSB_PTR, compare);
        #endif /* (HORIZ_UsingFixedFunction) */

        #if (HORIZ_PWMMode == HORIZ__B_PWM__DITHER)
            #if(HORIZ_UsingFixedFunction)
                CY_SET_REG16(HORIZ_COMPARE2_LSB_PTR, (uint16)(compare + 1u));
            #else
                CY_SET_REG8(HORIZ_COMPARE2_LSB_PTR, (compare + 1u));
            #endif /* (HORIZ_UsingFixedFunction) */
        #endif /* (HORIZ_PWMMode == HORIZ__B_PWM__DITHER) */
    }


#else


    /*******************************************************************************
    * Function Name: HORIZ_WriteCompare1
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
    void HORIZ_WriteCompare1(uint8 compare) \
                                        
    {
        #if(HORIZ_UsingFixedFunction)
            CY_SET_REG16(HORIZ_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(HORIZ_COMPARE1_LSB_PTR, compare);
        #endif /* (HORIZ_UsingFixedFunction) */
    }


    /*******************************************************************************
    * Function Name: HORIZ_WriteCompare2
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
    void HORIZ_WriteCompare2(uint8 compare) \
                                        
    {
        #if(HORIZ_UsingFixedFunction)
            CY_SET_REG16(HORIZ_COMPARE2_LSB_PTR, compare);
        #else
            CY_SET_REG8(HORIZ_COMPARE2_LSB_PTR, compare);
        #endif /* (HORIZ_UsingFixedFunction) */
    }
#endif /* UseOneCompareMode */

#if (HORIZ_DeadBandUsed)


    /*******************************************************************************
    * Function Name: HORIZ_WriteDeadTime
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
    void HORIZ_WriteDeadTime(uint8 deadtime) 
    {
        /* If using the Dead Band 1-255 mode then just write the register */
        #if(!HORIZ_DeadBand2_4)
            CY_SET_REG8(HORIZ_DEADBAND_COUNT_PTR, deadtime);
        #else
            /* Otherwise the data has to be masked and offset */
            /* Clear existing data */
            HORIZ_DEADBAND_COUNT &= ((uint8)(~HORIZ_DEADBAND_COUNT_MASK));

            /* Set new dead time */
            #if(HORIZ_DEADBAND_COUNT_SHIFT)
                HORIZ_DEADBAND_COUNT |= ((uint8)((uint8)deadtime << HORIZ_DEADBAND_COUNT_SHIFT)) &
                                                    HORIZ_DEADBAND_COUNT_MASK;
            #else
                HORIZ_DEADBAND_COUNT |= deadtime & HORIZ_DEADBAND_COUNT_MASK;
            #endif /* (HORIZ_DEADBAND_COUNT_SHIFT) */

        #endif /* (!HORIZ_DeadBand2_4) */
    }


    /*******************************************************************************
    * Function Name: HORIZ_ReadDeadTime
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
    uint8 HORIZ_ReadDeadTime(void) 
    {
        /* If using the Dead Band 1-255 mode then just read the register */
        #if(!HORIZ_DeadBand2_4)
            return (CY_GET_REG8(HORIZ_DEADBAND_COUNT_PTR));
        #else

            /* Otherwise the data has to be masked and offset */
            #if(HORIZ_DEADBAND_COUNT_SHIFT)
                return ((uint8)(((uint8)(HORIZ_DEADBAND_COUNT & HORIZ_DEADBAND_COUNT_MASK)) >>
                                                                           HORIZ_DEADBAND_COUNT_SHIFT));
            #else
                return (HORIZ_DEADBAND_COUNT & HORIZ_DEADBAND_COUNT_MASK);
            #endif /* (HORIZ_DEADBAND_COUNT_SHIFT) */
        #endif /* (!HORIZ_DeadBand2_4) */
    }
#endif /* DeadBandUsed */

#if (HORIZ_UseStatus || HORIZ_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: HORIZ_SetInterruptMode
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
    void HORIZ_SetInterruptMode(uint8 interruptMode) 
    {
        CY_SET_REG8(HORIZ_STATUS_MASK_PTR, interruptMode);
    }


    /*******************************************************************************
    * Function Name: HORIZ_ReadStatusRegister
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
    uint8 HORIZ_ReadStatusRegister(void) 
    {
        return (CY_GET_REG8(HORIZ_STATUS_PTR));
    }

#endif /* (HORIZ_UseStatus || HORIZ_UsingFixedFunction) */


#if (HORIZ_UseControl)


    /*******************************************************************************
    * Function Name: HORIZ_ReadControlRegister
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
    uint8 HORIZ_ReadControlRegister(void) 
    {
        uint8 result;

        result = CY_GET_REG8(HORIZ_CONTROL_PTR);
        return (result);
    }


    /*******************************************************************************
    * Function Name: HORIZ_WriteControlRegister
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
    void HORIZ_WriteControlRegister(uint8 control) 
    {
        CY_SET_REG8(HORIZ_CONTROL_PTR, control);
    }

#endif /* (HORIZ_UseControl) */


#if (!HORIZ_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: HORIZ_ReadCapture
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
    uint8 HORIZ_ReadCapture(void) 
    {
        return (CY_GET_REG8(HORIZ_CAPTURE_LSB_PTR));
    }

#endif /* (!HORIZ_UsingFixedFunction) */


#if (HORIZ_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: HORIZ_ReadCompare
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
    uint8 HORIZ_ReadCompare(void) 
    {
        #if(HORIZ_UsingFixedFunction)
            return ((uint8)CY_GET_REG16(HORIZ_COMPARE1_LSB_PTR));
        #else
            return (CY_GET_REG8(HORIZ_COMPARE1_LSB_PTR));
        #endif /* (HORIZ_UsingFixedFunction) */
    }

#else


    /*******************************************************************************
    * Function Name: HORIZ_ReadCompare1
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
    uint8 HORIZ_ReadCompare1(void) 
    {
        return (CY_GET_REG8(HORIZ_COMPARE1_LSB_PTR));
    }


    /*******************************************************************************
    * Function Name: HORIZ_ReadCompare2
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
    uint8 HORIZ_ReadCompare2(void) 
    {
        return (CY_GET_REG8(HORIZ_COMPARE2_LSB_PTR));
    }

#endif /* (HORIZ_UseOneCompareMode) */


/*******************************************************************************
* Function Name: HORIZ_ReadPeriod
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
uint8 HORIZ_ReadPeriod(void) 
{
    #if(HORIZ_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(HORIZ_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(HORIZ_PERIOD_LSB_PTR));
    #endif /* (HORIZ_UsingFixedFunction) */
}

#if ( HORIZ_KillModeMinTime)


    /*******************************************************************************
    * Function Name: HORIZ_WriteKillTime
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
    void HORIZ_WriteKillTime(uint8 killtime) 
    {
        CY_SET_REG8(HORIZ_KILLMODEMINTIME_PTR, killtime);
    }


    /*******************************************************************************
    * Function Name: HORIZ_ReadKillTime
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
    uint8 HORIZ_ReadKillTime(void) 
    {
        return (CY_GET_REG8(HORIZ_KILLMODEMINTIME_PTR));
    }

#endif /* ( HORIZ_KillModeMinTime) */

/* [] END OF FILE */
