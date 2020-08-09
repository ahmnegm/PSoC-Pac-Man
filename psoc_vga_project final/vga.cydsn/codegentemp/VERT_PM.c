/*******************************************************************************
* File Name: VERT_PM.c
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

#include "VERT.h"

static VERT_backupStruct VERT_backup;


/*******************************************************************************
* Function Name: VERT_SaveConfig
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
*  VERT_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void VERT_SaveConfig(void) 
{

    #if(!VERT_UsingFixedFunction)
        #if(!VERT_PWMModeIsCenterAligned)
            VERT_backup.PWMPeriod = VERT_ReadPeriod();
        #endif /* (!VERT_PWMModeIsCenterAligned) */
        VERT_backup.PWMUdb = VERT_ReadCounter();
        #if (VERT_UseStatus)
            VERT_backup.InterruptMaskValue = VERT_STATUS_MASK;
        #endif /* (VERT_UseStatus) */

        #if(VERT_DeadBandMode == VERT__B_PWM__DBM_256_CLOCKS || \
            VERT_DeadBandMode == VERT__B_PWM__DBM_2_4_CLOCKS)
            VERT_backup.PWMdeadBandValue = VERT_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(VERT_KillModeMinTime)
             VERT_backup.PWMKillCounterPeriod = VERT_ReadKillTime();
        #endif /* (VERT_KillModeMinTime) */

        #if(VERT_UseControl)
            VERT_backup.PWMControlRegister = VERT_ReadControlRegister();
        #endif /* (VERT_UseControl) */
    #endif  /* (!VERT_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: VERT_RestoreConfig
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
*  VERT_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void VERT_RestoreConfig(void) 
{
        #if(!VERT_UsingFixedFunction)
            #if(!VERT_PWMModeIsCenterAligned)
                VERT_WritePeriod(VERT_backup.PWMPeriod);
            #endif /* (!VERT_PWMModeIsCenterAligned) */

            VERT_WriteCounter(VERT_backup.PWMUdb);

            #if (VERT_UseStatus)
                VERT_STATUS_MASK = VERT_backup.InterruptMaskValue;
            #endif /* (VERT_UseStatus) */

            #if(VERT_DeadBandMode == VERT__B_PWM__DBM_256_CLOCKS || \
                VERT_DeadBandMode == VERT__B_PWM__DBM_2_4_CLOCKS)
                VERT_WriteDeadTime(VERT_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(VERT_KillModeMinTime)
                VERT_WriteKillTime(VERT_backup.PWMKillCounterPeriod);
            #endif /* (VERT_KillModeMinTime) */

            #if(VERT_UseControl)
                VERT_WriteControlRegister(VERT_backup.PWMControlRegister);
            #endif /* (VERT_UseControl) */
        #endif  /* (!VERT_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: VERT_Sleep
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
*  VERT_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void VERT_Sleep(void) 
{
    #if(VERT_UseControl)
        if(VERT_CTRL_ENABLE == (VERT_CONTROL & VERT_CTRL_ENABLE))
        {
            /*Component is enabled */
            VERT_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            VERT_backup.PWMEnableState = 0u;
        }
    #endif /* (VERT_UseControl) */

    /* Stop component */
    VERT_Stop();

    /* Save registers configuration */
    VERT_SaveConfig();
}


/*******************************************************************************
* Function Name: VERT_Wakeup
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
*  VERT_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void VERT_Wakeup(void) 
{
     /* Restore registers values */
    VERT_RestoreConfig();

    if(VERT_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        VERT_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
