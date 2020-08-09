/*******************************************************************************
* File Name: VSYNC_PM.c
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

#include "VSYNC.h"

static VSYNC_backupStruct VSYNC_backup;


/*******************************************************************************
* Function Name: VSYNC_SaveConfig
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
*  VSYNC_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void VSYNC_SaveConfig(void) 
{

    #if(!VSYNC_UsingFixedFunction)
        #if(!VSYNC_PWMModeIsCenterAligned)
            VSYNC_backup.PWMPeriod = VSYNC_ReadPeriod();
        #endif /* (!VSYNC_PWMModeIsCenterAligned) */
        VSYNC_backup.PWMUdb = VSYNC_ReadCounter();
        #if (VSYNC_UseStatus)
            VSYNC_backup.InterruptMaskValue = VSYNC_STATUS_MASK;
        #endif /* (VSYNC_UseStatus) */

        #if(VSYNC_DeadBandMode == VSYNC__B_PWM__DBM_256_CLOCKS || \
            VSYNC_DeadBandMode == VSYNC__B_PWM__DBM_2_4_CLOCKS)
            VSYNC_backup.PWMdeadBandValue = VSYNC_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(VSYNC_KillModeMinTime)
             VSYNC_backup.PWMKillCounterPeriod = VSYNC_ReadKillTime();
        #endif /* (VSYNC_KillModeMinTime) */

        #if(VSYNC_UseControl)
            VSYNC_backup.PWMControlRegister = VSYNC_ReadControlRegister();
        #endif /* (VSYNC_UseControl) */
    #endif  /* (!VSYNC_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: VSYNC_RestoreConfig
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
*  VSYNC_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void VSYNC_RestoreConfig(void) 
{
        #if(!VSYNC_UsingFixedFunction)
            #if(!VSYNC_PWMModeIsCenterAligned)
                VSYNC_WritePeriod(VSYNC_backup.PWMPeriod);
            #endif /* (!VSYNC_PWMModeIsCenterAligned) */

            VSYNC_WriteCounter(VSYNC_backup.PWMUdb);

            #if (VSYNC_UseStatus)
                VSYNC_STATUS_MASK = VSYNC_backup.InterruptMaskValue;
            #endif /* (VSYNC_UseStatus) */

            #if(VSYNC_DeadBandMode == VSYNC__B_PWM__DBM_256_CLOCKS || \
                VSYNC_DeadBandMode == VSYNC__B_PWM__DBM_2_4_CLOCKS)
                VSYNC_WriteDeadTime(VSYNC_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(VSYNC_KillModeMinTime)
                VSYNC_WriteKillTime(VSYNC_backup.PWMKillCounterPeriod);
            #endif /* (VSYNC_KillModeMinTime) */

            #if(VSYNC_UseControl)
                VSYNC_WriteControlRegister(VSYNC_backup.PWMControlRegister);
            #endif /* (VSYNC_UseControl) */
        #endif  /* (!VSYNC_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: VSYNC_Sleep
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
*  VSYNC_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void VSYNC_Sleep(void) 
{
    #if(VSYNC_UseControl)
        if(VSYNC_CTRL_ENABLE == (VSYNC_CONTROL & VSYNC_CTRL_ENABLE))
        {
            /*Component is enabled */
            VSYNC_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            VSYNC_backup.PWMEnableState = 0u;
        }
    #endif /* (VSYNC_UseControl) */

    /* Stop component */
    VSYNC_Stop();

    /* Save registers configuration */
    VSYNC_SaveConfig();
}


/*******************************************************************************
* Function Name: VSYNC_Wakeup
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
*  VSYNC_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void VSYNC_Wakeup(void) 
{
     /* Restore registers values */
    VSYNC_RestoreConfig();

    if(VSYNC_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        VSYNC_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
