/*******************************************************************************
* File Name: CapSense_Controller_Pins.h
* Version 3.50
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CAPSENSE_CSD_Pins_CapSense_Controller_H)
#define CY_CAPSENSE_CSD_Pins_CapSense_Controller_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "CapSense_Controller.h"


/***************************************
*        Function Prototypes
***************************************/

void CapSense_Controller_SetAllSensorsDriveMode(uint8 mode) ;
void CapSense_Controller_SetAllCmodsDriveMode(uint8 mode) ;
#if (CapSense_Controller_CURRENT_SOURCE == CapSense_Controller_EXTERNAL_RB)
    void CapSense_Controller_SetAllRbsDriveMode(uint8 mode) ;    
#endif  /* (CapSense_Controller_CURRENT_SOURCE == CapSense_Controller_EXTERNAL_RB) */   


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define CapSense_Controller_DM_ALG_HIZ         (PIN_DM_ALG_HIZ)
#define CapSense_Controller_DM_DIG_HIZ         (PIN_DM_DIG_HIZ)
#define CapSense_Controller_DM_RES_UP          (PIN_DM_RES_UP)
#define CapSense_Controller_DM_RES_DWN         (PIN_DM_RES_DWN)
#define CapSense_Controller_DM_OD_LO           (PIN_DM_OD_LO)
#define CapSense_Controller_DM_OD_HI           (PIN_DM_OD_HI)
#define CapSense_Controller_DM_STRONG          (PIN_DM_STRONG)
#define CapSense_Controller_DM_RES_UPDWN       (PIN_DM_RES_UPDWN)

/* PC registers defines for sensors */
#define CapSense_Controller_PortCH0__ButtonLeft__BTN  CapSense_Controller_PortCH0__ButtonLeft__BTN__PC
#define CapSense_Controller_PortCH0__ButtonRight__BTN  CapSense_Controller_PortCH0__ButtonRight__BTN__PC
#define CapSense_Controller_PortCH0__ButtonUp__BTN  CapSense_Controller_PortCH0__ButtonUp__BTN__PC
#define CapSense_Controller_PortCH0__ButtonDown__BTN  CapSense_Controller_PortCH0__ButtonDown__BTN__PC
/* For Cmods*/
#define CapSense_Controller_CmodCH0_Cmod_CH0       CapSense_Controller_CmodCH0__Cmod_CH0__PC


#endif /* (CY_CAPSENSE_CSD_Pins_CapSense_Controller_H) */


/* [] END OF FILE */
