/*******************************************************************************
* File Name: VGA.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_VGA_H) /* Pins VGA_H */
#define CY_PINS_VGA_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "VGA_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 VGA__PORT == 15 && ((VGA__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    VGA_Write(uint8 value);
void    VGA_SetDriveMode(uint8 mode);
uint8   VGA_ReadDataReg(void);
uint8   VGA_Read(void);
void    VGA_SetInterruptMode(uint16 position, uint16 mode);
uint8   VGA_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the VGA_SetDriveMode() function.
     *  @{
     */
        #define VGA_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define VGA_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define VGA_DM_RES_UP          PIN_DM_RES_UP
        #define VGA_DM_RES_DWN         PIN_DM_RES_DWN
        #define VGA_DM_OD_LO           PIN_DM_OD_LO
        #define VGA_DM_OD_HI           PIN_DM_OD_HI
        #define VGA_DM_STRONG          PIN_DM_STRONG
        #define VGA_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define VGA_MASK               VGA__MASK
#define VGA_SHIFT              VGA__SHIFT
#define VGA_WIDTH              6u

/* Interrupt constants */
#if defined(VGA__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in VGA_SetInterruptMode() function.
     *  @{
     */
        #define VGA_INTR_NONE      (uint16)(0x0000u)
        #define VGA_INTR_RISING    (uint16)(0x0001u)
        #define VGA_INTR_FALLING   (uint16)(0x0002u)
        #define VGA_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define VGA_INTR_MASK      (0x01u) 
#endif /* (VGA__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define VGA_PS                     (* (reg8 *) VGA__PS)
/* Data Register */
#define VGA_DR                     (* (reg8 *) VGA__DR)
/* Port Number */
#define VGA_PRT_NUM                (* (reg8 *) VGA__PRT) 
/* Connect to Analog Globals */                                                  
#define VGA_AG                     (* (reg8 *) VGA__AG)                       
/* Analog MUX bux enable */
#define VGA_AMUX                   (* (reg8 *) VGA__AMUX) 
/* Bidirectional Enable */                                                        
#define VGA_BIE                    (* (reg8 *) VGA__BIE)
/* Bit-mask for Aliased Register Access */
#define VGA_BIT_MASK               (* (reg8 *) VGA__BIT_MASK)
/* Bypass Enable */
#define VGA_BYP                    (* (reg8 *) VGA__BYP)
/* Port wide control signals */                                                   
#define VGA_CTL                    (* (reg8 *) VGA__CTL)
/* Drive Modes */
#define VGA_DM0                    (* (reg8 *) VGA__DM0) 
#define VGA_DM1                    (* (reg8 *) VGA__DM1)
#define VGA_DM2                    (* (reg8 *) VGA__DM2) 
/* Input Buffer Disable Override */
#define VGA_INP_DIS                (* (reg8 *) VGA__INP_DIS)
/* LCD Common or Segment Drive */
#define VGA_LCD_COM_SEG            (* (reg8 *) VGA__LCD_COM_SEG)
/* Enable Segment LCD */
#define VGA_LCD_EN                 (* (reg8 *) VGA__LCD_EN)
/* Slew Rate Control */
#define VGA_SLW                    (* (reg8 *) VGA__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define VGA_PRTDSI__CAPS_SEL       (* (reg8 *) VGA__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define VGA_PRTDSI__DBL_SYNC_IN    (* (reg8 *) VGA__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define VGA_PRTDSI__OE_SEL0        (* (reg8 *) VGA__PRTDSI__OE_SEL0) 
#define VGA_PRTDSI__OE_SEL1        (* (reg8 *) VGA__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define VGA_PRTDSI__OUT_SEL0       (* (reg8 *) VGA__PRTDSI__OUT_SEL0) 
#define VGA_PRTDSI__OUT_SEL1       (* (reg8 *) VGA__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define VGA_PRTDSI__SYNC_OUT       (* (reg8 *) VGA__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(VGA__SIO_CFG)
    #define VGA_SIO_HYST_EN        (* (reg8 *) VGA__SIO_HYST_EN)
    #define VGA_SIO_REG_HIFREQ     (* (reg8 *) VGA__SIO_REG_HIFREQ)
    #define VGA_SIO_CFG            (* (reg8 *) VGA__SIO_CFG)
    #define VGA_SIO_DIFF           (* (reg8 *) VGA__SIO_DIFF)
#endif /* (VGA__SIO_CFG) */

/* Interrupt Registers */
#if defined(VGA__INTSTAT)
    #define VGA_INTSTAT            (* (reg8 *) VGA__INTSTAT)
    #define VGA_SNAP               (* (reg8 *) VGA__SNAP)
    
	#define VGA_0_INTTYPE_REG 		(* (reg8 *) VGA__0__INTTYPE)
	#define VGA_1_INTTYPE_REG 		(* (reg8 *) VGA__1__INTTYPE)
	#define VGA_2_INTTYPE_REG 		(* (reg8 *) VGA__2__INTTYPE)
	#define VGA_3_INTTYPE_REG 		(* (reg8 *) VGA__3__INTTYPE)
	#define VGA_4_INTTYPE_REG 		(* (reg8 *) VGA__4__INTTYPE)
	#define VGA_5_INTTYPE_REG 		(* (reg8 *) VGA__5__INTTYPE)
#endif /* (VGA__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_VGA_H */


/* [] END OF FILE */
