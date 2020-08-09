/*******************************************************************************
* File Name: VGA.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_VGA_ALIASES_H) /* Pins VGA_ALIASES_H */
#define CY_PINS_VGA_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants        
***************************************/
#define VGA_0			(VGA__0__PC)
#define VGA_0_INTR	((uint16)((uint16)0x0001u << VGA__0__SHIFT))

#define VGA_1			(VGA__1__PC)
#define VGA_1_INTR	((uint16)((uint16)0x0001u << VGA__1__SHIFT))

#define VGA_2			(VGA__2__PC)
#define VGA_2_INTR	((uint16)((uint16)0x0001u << VGA__2__SHIFT))

#define VGA_3			(VGA__3__PC)
#define VGA_3_INTR	((uint16)((uint16)0x0001u << VGA__3__SHIFT))

#define VGA_4			(VGA__4__PC)
#define VGA_4_INTR	((uint16)((uint16)0x0001u << VGA__4__SHIFT))

#define VGA_5			(VGA__5__PC)
#define VGA_5_INTR	((uint16)((uint16)0x0001u << VGA__5__SHIFT))

#define VGA_INTR_ALL	 ((uint16)(VGA_0_INTR| VGA_1_INTR| VGA_2_INTR| VGA_3_INTR| VGA_4_INTR| VGA_5_INTR))

#endif /* End Pins VGA_ALIASES_H */


/* [] END OF FILE */
