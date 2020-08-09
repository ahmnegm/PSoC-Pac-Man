/*******************************************************************************
* File Name: CapSense_Controller_Pins.c
* Version 3.50
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "CapSense_Controller_Pins.h"


/*******************************************************************************
* Function Name: CapSense_Controller_SetAllSensorsDriveMode
********************************************************************************
*
* Summary:
*  Sets the drive mode for the all pins used by capacitive sensors within 
*  CapSense component.
* 
* Parameters:  
*  mode: Desired drive mode.
*
* Return: 
*  None
*
*******************************************************************************/
void CapSense_Controller_SetAllSensorsDriveMode(uint8 mode) 
{
    /* Set pins drive mode */
    CyPins_SetPinDriveMode(CapSense_Controller_PortCH0__ButtonLeft__BTN, mode);
    CyPins_SetPinDriveMode(CapSense_Controller_PortCH0__ButtonRight__BTN, mode);
    CyPins_SetPinDriveMode(CapSense_Controller_PortCH0__ButtonUp__BTN, mode);
    CyPins_SetPinDriveMode(CapSense_Controller_PortCH0__ButtonDown__BTN, mode);

}



/*******************************************************************************
* Function Name: CapSense_Controller_SetAllCmodsDriveMode
********************************************************************************
*
* Summary:
*  Sets the drive mode for the all pins used by Cmod capacitors within CapSense 
*  component.
* 
* Parameters:  
*  mode: Desired drive mode.
*
* Return: 
*  None
*
*******************************************************************************/
void CapSense_Controller_SetAllCmodsDriveMode(uint8 mode) 
{
   /* Set pins drive mode */
    CyPins_SetPinDriveMode(CapSense_Controller_CmodCH0_Cmod_CH0, mode);

}


#if (CapSense_Controller_CURRENT_SOURCE == CapSense_Controller_EXTERNAL_RB)
    /*******************************************************************************
    * Function Name: CapSense_Controller_SetAllRbsDriveMode
    ********************************************************************************
    *
    * Summary:
    *  Sets the drive mode for the all pins used by bleed resistors (Rb) within 
    *  CapSense component. Only available when Current Source is external resistor.
    * 
    * Parameters:  
    *  mode: Desired drive mode.
    *
    * Return: 
    *  None
    *
    *******************************************************************************/
    void CapSense_Controller_SetAllRbsDriveMode(uint8 mode) 
    {
        /* Set pins drive mode */
      
    }
#endif  /* (CapSense_Controller_CURRENT_SOURCE == CapSense_Controller_EXTERNAL_RB) */


/* [] END OF FILE */
