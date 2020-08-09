/*******************************************************************************
* File Name: HSYNC_PM.c
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

#include "HSYNC.h"

static HSYNC_backupStruct HSYNC_backup;


/*******************************************************************************
* Function Name: HSYNC_SaveConfig
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
*  HSYNC_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void HSYNC_SaveConfig(void) 
{

    #if(!HSYNC_UsingFixedFunction)
        #if(!HSYNC_PWMModeIsCenterAligned)
            HSYNC_backup.PWMPeriod = HSYNC_ReadPeriod();
        #endif /* (!HSYNC_PWMModeIsCenterAligned) */
        HSYNC_backup.PWMUdb = HSYNC_ReadCounter();
        #if (HSYNC_UseStatus)
            HSYNC_backup.InterruptMaskValue = HSYNC_STATUS_MASK;
        #endif /* (HSYNC_UseStatus) */

        #if(HSYNC_DeadBandMode == HSYNC__B_PWM__DBM_256_CLOCKS || \
            HSYNC_DeadBandMode == HSYNC__B_PWM__DBM_2_4_CLOCKS)
            HSYNC_backup.PWMdeadBandValue = HSYNC_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(HSYNC_KillModeMinTime)
             HSYNC_backup.PWMKillCounterPeriod = HSYNC_ReadKillTime();
        #endif /* (HSYNC_KillModeMinTime) */

        #if(HSYNC_UseControl)
            HSYNC_backup.PWMControlRegister = HSYNC_ReadControlRegister();
        #endif /* (HSYNC_UseControl) */
    #endif  /* (!HSYNC_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: HSYNC_RestoreConfig
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
*  HSYNC_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void HSYNC_RestoreConfig(void) 
{
        #if(!HSYNC_UsingFixedFunction)
            #if(!HSYNC_PWMModeIsCenterAligned)
                HSYNC_WritePeriod(HSYNC_backup.PWMPeriod);
            #endif /* (!HSYNC_PWMModeIsCenterAligned) */

            HSYNC_WriteCounter(HSYNC_backup.PWMUdb);

            #if (HSYNC_UseStatus)
                HSYNC_STATUS_MASK = HSYNC_backup.InterruptMaskValue;
            #endif /* (HSYNC_UseStatus) */

            #if(HSYNC_DeadBandMode == HSYNC__B_PWM__DBM_256_CLOCKS || \
                HSYNC_DeadBandMode == HSYNC__B_PWM__DBM_2_4_CLOCKS)
                HSYNC_WriteDeadTime(HSYNC_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(HSYNC_KillModeMinTime)
                HSYNC_WriteKillTime(HSYNC_backup.PWMKillCounterPeriod);
            #endif /* (HSYNC_KillModeMinTime) */

            #if(HSYNC_UseControl)
                HSYNC_WriteControlRegister(HSYNC_backup.PWMControlRegister);
            #endif /* (HSYNC_UseControl) */
        #endif  /* (!HSYNC_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: HSYNC_Sleep
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
*  HSYNC_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void HSYNC_Sleep(void) 
{
    #if(HSYNC_UseControl)
        if(HSYNC_CTRL_ENABLE == (HSYNC_CONTROL & HSYNC_CTRL_ENABLE))
        {
            /*Component is enabled */
            HSYNC_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            HSYNC_backup.PWMEnableState = 0u;
        }
    #endif /* (HSYNC_UseControl) */

    /* Stop component */
    HSYNC_Stop();

    /* Save registers configuration */
    HSYNC_SaveConfig();
}


/*******************************************************************************
* Function Name: HSYNC_Wakeup
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
*  HSYNC_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void HSYNC_Wakeup(void) 
{
     /* Restore registers values */
    HSYNC_RestoreConfig();

    if(HSYNC_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        HSYNC_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
