/*******************************************************************************
* File Name: Rx.h  
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

#if !defined(CY_PINS_Rx_H) /* Pins Rx_H */
#define CY_PINS_Rx_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Rx_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Rx__PORT == 15 && ((Rx__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Rx_Write(uint8 value);
void    Rx_SetDriveMode(uint8 mode);
uint8   Rx_ReadDataReg(void);
uint8   Rx_Read(void);
void    Rx_SetInterruptMode(uint16 position, uint16 mode);
uint8   Rx_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Rx_SetDriveMode() function.
     *  @{
     */
        #define Rx_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Rx_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Rx_DM_RES_UP          PIN_DM_RES_UP
        #define Rx_DM_RES_DWN         PIN_DM_RES_DWN
        #define Rx_DM_OD_LO           PIN_DM_OD_LO
        #define Rx_DM_OD_HI           PIN_DM_OD_HI
        #define Rx_DM_STRONG          PIN_DM_STRONG
        #define Rx_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Rx_MASK               Rx__MASK
#define Rx_SHIFT              Rx__SHIFT
#define Rx_WIDTH              1u

/* Interrupt constants */
#if defined(Rx__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Rx_SetInterruptMode() function.
     *  @{
     */
        #define Rx_INTR_NONE      (uint16)(0x0000u)
        #define Rx_INTR_RISING    (uint16)(0x0001u)
        #define Rx_INTR_FALLING   (uint16)(0x0002u)
        #define Rx_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Rx_INTR_MASK      (0x01u) 
#endif /* (Rx__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Rx_PS                     (* (reg8 *) Rx__PS)
/* Data Register */
#define Rx_DR                     (* (reg8 *) Rx__DR)
/* Port Number */
#define Rx_PRT_NUM                (* (reg8 *) Rx__PRT) 
/* Connect to Analog Globals */                                                  
#define Rx_AG                     (* (reg8 *) Rx__AG)                       
/* Analog MUX bux enable */
#define Rx_AMUX                   (* (reg8 *) Rx__AMUX) 
/* Bidirectional Enable */                                                        
#define Rx_BIE                    (* (reg8 *) Rx__BIE)
/* Bit-mask for Aliased Register Access */
#define Rx_BIT_MASK               (* (reg8 *) Rx__BIT_MASK)
/* Bypass Enable */
#define Rx_BYP                    (* (reg8 *) Rx__BYP)
/* Port wide control signals */                                                   
#define Rx_CTL                    (* (reg8 *) Rx__CTL)
/* Drive Modes */
#define Rx_DM0                    (* (reg8 *) Rx__DM0) 
#define Rx_DM1                    (* (reg8 *) Rx__DM1)
#define Rx_DM2                    (* (reg8 *) Rx__DM2) 
/* Input Buffer Disable Override */
#define Rx_INP_DIS                (* (reg8 *) Rx__INP_DIS)
/* LCD Common or Segment Drive */
#define Rx_LCD_COM_SEG            (* (reg8 *) Rx__LCD_COM_SEG)
/* Enable Segment LCD */
#define Rx_LCD_EN                 (* (reg8 *) Rx__LCD_EN)
/* Slew Rate Control */
#define Rx_SLW                    (* (reg8 *) Rx__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Rx_PRTDSI__CAPS_SEL       (* (reg8 *) Rx__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Rx_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Rx__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Rx_PRTDSI__OE_SEL0        (* (reg8 *) Rx__PRTDSI__OE_SEL0) 
#define Rx_PRTDSI__OE_SEL1        (* (reg8 *) Rx__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Rx_PRTDSI__OUT_SEL0       (* (reg8 *) Rx__PRTDSI__OUT_SEL0) 
#define Rx_PRTDSI__OUT_SEL1       (* (reg8 *) Rx__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Rx_PRTDSI__SYNC_OUT       (* (reg8 *) Rx__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Rx__SIO_CFG)
    #define Rx_SIO_HYST_EN        (* (reg8 *) Rx__SIO_HYST_EN)
    #define Rx_SIO_REG_HIFREQ     (* (reg8 *) Rx__SIO_REG_HIFREQ)
    #define Rx_SIO_CFG            (* (reg8 *) Rx__SIO_CFG)
    #define Rx_SIO_DIFF           (* (reg8 *) Rx__SIO_DIFF)
#endif /* (Rx__SIO_CFG) */

/* Interrupt Registers */
#if defined(Rx__INTSTAT)
    #define Rx_INTSTAT            (* (reg8 *) Rx__INTSTAT)
    #define Rx_SNAP               (* (reg8 *) Rx__SNAP)
    
	#define Rx_0_INTTYPE_REG 		(* (reg8 *) Rx__0__INTTYPE)
#endif /* (Rx__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Rx_H */


/* [] END OF FILE */
