/*******************************************************************************
* File Name: VRy.h  
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

#if !defined(CY_PINS_VRy_H) /* Pins VRy_H */
#define CY_PINS_VRy_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "VRy_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 VRy__PORT == 15 && ((VRy__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    VRy_Write(uint8 value);
void    VRy_SetDriveMode(uint8 mode);
uint8   VRy_ReadDataReg(void);
uint8   VRy_Read(void);
void    VRy_SetInterruptMode(uint16 position, uint16 mode);
uint8   VRy_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the VRy_SetDriveMode() function.
     *  @{
     */
        #define VRy_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define VRy_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define VRy_DM_RES_UP          PIN_DM_RES_UP
        #define VRy_DM_RES_DWN         PIN_DM_RES_DWN
        #define VRy_DM_OD_LO           PIN_DM_OD_LO
        #define VRy_DM_OD_HI           PIN_DM_OD_HI
        #define VRy_DM_STRONG          PIN_DM_STRONG
        #define VRy_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define VRy_MASK               VRy__MASK
#define VRy_SHIFT              VRy__SHIFT
#define VRy_WIDTH              1u

/* Interrupt constants */
#if defined(VRy__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in VRy_SetInterruptMode() function.
     *  @{
     */
        #define VRy_INTR_NONE      (uint16)(0x0000u)
        #define VRy_INTR_RISING    (uint16)(0x0001u)
        #define VRy_INTR_FALLING   (uint16)(0x0002u)
        #define VRy_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define VRy_INTR_MASK      (0x01u) 
#endif /* (VRy__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define VRy_PS                     (* (reg8 *) VRy__PS)
/* Data Register */
#define VRy_DR                     (* (reg8 *) VRy__DR)
/* Port Number */
#define VRy_PRT_NUM                (* (reg8 *) VRy__PRT) 
/* Connect to Analog Globals */                                                  
#define VRy_AG                     (* (reg8 *) VRy__AG)                       
/* Analog MUX bux enable */
#define VRy_AMUX                   (* (reg8 *) VRy__AMUX) 
/* Bidirectional Enable */                                                        
#define VRy_BIE                    (* (reg8 *) VRy__BIE)
/* Bit-mask for Aliased Register Access */
#define VRy_BIT_MASK               (* (reg8 *) VRy__BIT_MASK)
/* Bypass Enable */
#define VRy_BYP                    (* (reg8 *) VRy__BYP)
/* Port wide control signals */                                                   
#define VRy_CTL                    (* (reg8 *) VRy__CTL)
/* Drive Modes */
#define VRy_DM0                    (* (reg8 *) VRy__DM0) 
#define VRy_DM1                    (* (reg8 *) VRy__DM1)
#define VRy_DM2                    (* (reg8 *) VRy__DM2) 
/* Input Buffer Disable Override */
#define VRy_INP_DIS                (* (reg8 *) VRy__INP_DIS)
/* LCD Common or Segment Drive */
#define VRy_LCD_COM_SEG            (* (reg8 *) VRy__LCD_COM_SEG)
/* Enable Segment LCD */
#define VRy_LCD_EN                 (* (reg8 *) VRy__LCD_EN)
/* Slew Rate Control */
#define VRy_SLW                    (* (reg8 *) VRy__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define VRy_PRTDSI__CAPS_SEL       (* (reg8 *) VRy__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define VRy_PRTDSI__DBL_SYNC_IN    (* (reg8 *) VRy__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define VRy_PRTDSI__OE_SEL0        (* (reg8 *) VRy__PRTDSI__OE_SEL0) 
#define VRy_PRTDSI__OE_SEL1        (* (reg8 *) VRy__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define VRy_PRTDSI__OUT_SEL0       (* (reg8 *) VRy__PRTDSI__OUT_SEL0) 
#define VRy_PRTDSI__OUT_SEL1       (* (reg8 *) VRy__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define VRy_PRTDSI__SYNC_OUT       (* (reg8 *) VRy__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(VRy__SIO_CFG)
    #define VRy_SIO_HYST_EN        (* (reg8 *) VRy__SIO_HYST_EN)
    #define VRy_SIO_REG_HIFREQ     (* (reg8 *) VRy__SIO_REG_HIFREQ)
    #define VRy_SIO_CFG            (* (reg8 *) VRy__SIO_CFG)
    #define VRy_SIO_DIFF           (* (reg8 *) VRy__SIO_DIFF)
#endif /* (VRy__SIO_CFG) */

/* Interrupt Registers */
#if defined(VRy__INTSTAT)
    #define VRy_INTSTAT            (* (reg8 *) VRy__INTSTAT)
    #define VRy_SNAP               (* (reg8 *) VRy__SNAP)
    
	#define VRy_0_INTTYPE_REG 		(* (reg8 *) VRy__0__INTTYPE)
#endif /* (VRy__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_VRy_H */


/* [] END OF FILE */
