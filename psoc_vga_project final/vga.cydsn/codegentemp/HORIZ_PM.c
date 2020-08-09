/*******************************************************************************
* File Name: HORIZ_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "HORIZ.h"

static HORIZ_backupStruct HORIZ_backup;


/*******************************************************************************
* Function Name: HORIZ_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  HORIZ_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void HORIZ_SaveConfig(void) 
{

    #if(!HORIZ_UsingFixedFunction)
        #if(!HORIZ_PWMModeIsCenterAligned)
            HORIZ_backup.PWMPeriod = HORIZ_ReadPeriod();
        #endif /* (!HORIZ_PWMModeIsCenterAligned) */
        HORIZ_backup.PWMUdb = HORIZ_ReadCounter();
        #if (HORIZ_UseStatus)
            HORIZ_backup.InterruptMaskValue = HORIZ_STATUS_MASK;
        #endif /* (HORIZ_UseStatus) */

        #if(HORIZ_DeadBandMode == HORIZ__B_PWM__DBM_256_CLOCKS || \
            HORIZ_DeadBandMode == HORIZ__B_PWM__DBM_2_4_CLOCKS)
            HORIZ_backup.PWMdeadBandValue = HORIZ_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(HORIZ_KillModeMinTime)
             HORIZ_backup.PWMKillCounterPeriod = HORIZ_ReadKillTime();
        #endif /* (HORIZ_KillModeMinTime) */

        #if(HORIZ_UseControl)
            HORIZ_backup.PWMControlRegister = HORIZ_ReadControlRegister();
        #endif /* (HORIZ_UseControl) */
    #endif  /* (!HORIZ_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: HORIZ_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  HORIZ_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void HORIZ_RestoreConfig(void) 
{
        #if(!HORIZ_UsingFixedFunction)
            #if(!HORIZ_PWMModeIsCenterAligned)
                HORIZ_WritePeriod(HORIZ_backup.PWMPeriod);
            #endif /* (!HORIZ_PWMModeIsCenterAligned) */

            HORIZ_WriteCounter(HORIZ_backup.PWMUdb);

            #if (HORIZ_UseStatus)
                HORIZ_STATUS_MASK = HORIZ_backup.InterruptMaskValue;
            #endif /* (HORIZ_UseStatus) */

            #if(HORIZ_DeadBandMode == HORIZ__B_PWM__DBM_256_CLOCKS || \
                HORIZ_DeadBandMode == HORIZ__B_PWM__DBM_2_4_CLOCKS)
                HORIZ_WriteDeadTime(HORIZ_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(HORIZ_KillModeMinTime)
                HORIZ_WriteKillTime(HORIZ_backup.PWMKillCounterPeriod);
            #endif /* (HORIZ_KillModeMinTime) */

            #if(HORIZ_UseControl)
                HORIZ_WriteControlRegister(HORIZ_backup.PWMControlRegister);
            #endif /* (HORIZ_UseControl) */
        #endif  /* (!HORIZ_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: HORIZ_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  HORIZ_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void HORIZ_Sleep(void) 
{
    #if(HORIZ_UseControl)
        if(HORIZ_CTRL_ENABLE == (HORIZ_CONTROL & HORIZ_CTRL_ENABLE))
        {
            /*Component is enabled */
            HORIZ_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            HORIZ_backup.PWMEnableState = 0u;
        }
    #endif /* (HORIZ_UseControl) */

    /* Stop component */
    HORIZ_Stop();

    /* Save registers configuration */
    HORIZ_SaveConfig();
}


/*******************************************************************************
* Function Name: HORIZ_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  HORIZ_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void HORIZ_Wakeup(void) 
{
     /* Restore registers values */
    HORIZ_RestoreConfig();

    if(HORIZ_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        HORIZ_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
