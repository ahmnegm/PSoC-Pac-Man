/*******************************************************************************
* File Name: NEWLINE.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_NEWLINE_H)
#define CY_ISR_NEWLINE_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void NEWLINE_Start(void);
void NEWLINE_StartEx(cyisraddress address);
void NEWLINE_Stop(void);

CY_ISR_PROTO(NEWLINE_Interrupt);

void NEWLINE_SetVector(cyisraddress address);
cyisraddress NEWLINE_GetVector(void);

void NEWLINE_SetPriority(uint8 priority);
uint8 NEWLINE_GetPriority(void);

void NEWLINE_Enable(void);
uint8 NEWLINE_GetState(void);
void NEWLINE_Disable(void);

void NEWLINE_SetPending(void);
void NEWLINE_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the NEWLINE ISR. */
#define NEWLINE_INTC_VECTOR            ((reg32 *) NEWLINE__INTC_VECT)

/* Address of the NEWLINE ISR priority. */
#define NEWLINE_INTC_PRIOR             ((reg8 *) NEWLINE__INTC_PRIOR_REG)

/* Priority of the NEWLINE interrupt. */
#define NEWLINE_INTC_PRIOR_NUMBER      NEWLINE__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable NEWLINE interrupt. */
#define NEWLINE_INTC_SET_EN            ((reg32 *) NEWLINE__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the NEWLINE interrupt. */
#define NEWLINE_INTC_CLR_EN            ((reg32 *) NEWLINE__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the NEWLINE interrupt state to pending. */
#define NEWLINE_INTC_SET_PD            ((reg32 *) NEWLINE__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the NEWLINE interrupt. */
#define NEWLINE_INTC_CLR_PD            ((reg32 *) NEWLINE__INTC_CLR_PD_REG)


#endif /* CY_ISR_NEWLINE_H */


/* [] END OF FILE */
