/*******************************************************************************
* File Name: PIXEL_PM.c
* Version 1.80
*
* Description:
*  This file contains the setup, control, and status commands to support 
*  the component operation in the low power mode. 
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "PIXEL.h"

/* Check for removal by optimization */
#if !defined(PIXEL_Sync_ctrl_reg__REMOVED)

static PIXEL_BACKUP_STRUCT  PIXEL_backup = {0u};

    
/*******************************************************************************
* Function Name: PIXEL_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PIXEL_SaveConfig(void) 
{
    PIXEL_backup.controlState = PIXEL_Control;
}


/*******************************************************************************
* Function Name: PIXEL_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*
*******************************************************************************/
void PIXEL_RestoreConfig(void) 
{
     PIXEL_Control = PIXEL_backup.controlState;
}


/*******************************************************************************
* Function Name: PIXEL_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for entering the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PIXEL_Sleep(void) 
{
    PIXEL_SaveConfig();
}


/*******************************************************************************
* Function Name: PIXEL_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component after waking up from the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PIXEL_Wakeup(void)  
{
    PIXEL_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
