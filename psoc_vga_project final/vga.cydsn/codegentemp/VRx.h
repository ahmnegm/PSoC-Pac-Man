/*******************************************************************************
* File Name: VRx.h  
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

#if !defined(CY_PINS_VRx_H) /* Pins VRx_H */
#define CY_PINS_VRx_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "VRx_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 VRx__PORT == 15 && ((VRx__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    VRx_Write(uint8 value);
void    VRx_SetDriveMode(uint8 mode);
uint8   VRx_ReadDataReg(void);
uint8   VRx_Read(void);
void    VRx_SetInterruptMode(uint16 position, uint16 mode);
uint8   VRx_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the VRx_SetDriveMode() function.
     *  @{
     */
        #define VRx_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define VRx_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define VRx_DM_RES_UP          PIN_DM_RES_UP
        #define VRx_DM_RES_DWN         PIN_DM_RES_DWN
        #define VRx_DM_OD_LO           PIN_DM_OD_LO
        #define VRx_DM_OD_HI           PIN_DM_OD_HI
        #define VRx_DM_STRONG          PIN_DM_STRONG
        #define VRx_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define VRx_MASK               VRx__MASK
#define VRx_SHIFT              VRx__SHIFT
#define VRx_WIDTH              1u

/* Interrupt constants */
#if defined(VRx__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in VRx_SetInterruptMode() function.
     *  @{
     */
        #define VRx_INTR_NONE      (uint16)(0x0000u)
        #define VRx_INTR_RISING    (uint16)(0x0001u)
        #define VRx_INTR_FALLING   (uint16)(0x0002u)
        #define VRx_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define VRx_INTR_MASK      (0x01u) 
#endif /* (VRx__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define VRx_PS                     (* (reg8 *) VRx__PS)
/* Data Register */
#define VRx_DR                     (* (reg8 *) VRx__DR)
/* Port Number */
#define VRx_PRT_NUM                (* (reg8 *) VRx__PRT) 
/* Connect to Analog Globals */                                                  
#define VRx_AG                     (* (reg8 *) VRx__AG)                       
/* Analog MUX bux enable */
#define VRx_AMUX                   (* (reg8 *) VRx__AMUX) 
/* Bidirectional Enable */                                                        
#define VRx_BIE                    (* (reg8 *) VRx__BIE)
/* Bit-mask for Aliased Register Access */
#define VRx_BIT_MASK               (* (reg8 *) VRx__BIT_MASK)
/* Bypass Enable */
#define VRx_BYP                    (* (reg8 *) VRx__BYP)
/* Port wide control signals */                                                   
#define VRx_CTL                    (* (reg8 *) VRx__CTL)
/* Drive Modes */
#define VRx_DM0                    (* (reg8 *) VRx__DM0) 
#define VRx_DM1                    (* (reg8 *) VRx__DM1)
#define VRx_DM2                    (* (reg8 *) VRx__DM2) 
/* Input Buffer Disable Override */
#define VRx_INP_DIS                (* (reg8 *) VRx__INP_DIS)
/* LCD Common or Segment Drive */
#define VRx_LCD_COM_SEG            (* (reg8 *) VRx__LCD_COM_SEG)
/* Enable Segment LCD */
#define VRx_LCD_EN                 (* (reg8 *) VRx__LCD_EN)
/* Slew Rate Control */
#define VRx_SLW                    (* (reg8 *) VRx__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define VRx_PRTDSI__CAPS_SEL       (* (reg8 *) VRx__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define VRx_PRTDSI__DBL_SYNC_IN    (* (reg8 *) VRx__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define VRx_PRTDSI__OE_SEL0        (* (reg8 *) VRx__PRTDSI__OE_SEL0) 
#define VRx_PRTDSI__OE_SEL1        (* (reg8 *) VRx__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define VRx_PRTDSI__OUT_SEL0       (* (reg8 *) VRx__PRTDSI__OUT_SEL0) 
#define VRx_PRTDSI__OUT_SEL1       (* (reg8 *) VRx__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define VRx_PRTDSI__SYNC_OUT       (* (reg8 *) VRx__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(VRx__SIO_CFG)
    #define VRx_SIO_HYST_EN        (* (reg8 *) VRx__SIO_HYST_EN)
    #define VRx_SIO_REG_HIFREQ     (* (reg8 *) VRx__SIO_REG_HIFREQ)
    #define VRx_SIO_CFG            (* (reg8 *) VRx__SIO_CFG)
    #define VRx_SIO_DIFF           (* (reg8 *) VRx__SIO_DIFF)
#endif /* (VRx__SIO_CFG) */

/* Interrupt Registers */
#if defined(VRx__INTSTAT)
    #define VRx_INTSTAT            (* (reg8 *) VRx__INTSTAT)
    #define VRx_SNAP               (* (reg8 *) VRx__SNAP)
    
	#define VRx_0_INTTYPE_REG 		(* (reg8 *) VRx__0__INTTYPE)
#endif /* (VRx__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_VRx_H */


/* [] END OF FILE */
