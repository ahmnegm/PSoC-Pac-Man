/*******************************************************************************
* File Name: CapSense_Controller_TunerHelper.h
* Version 3.50
*
* Description:
*  This file provides constants and structure declarations for the tunner hepl
*  APIs for CapSense CSD component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CAPSENSE_CSD_TUNERHELPER_CapSense_Controller_H)
#define CY_CAPSENSE_CSD_TUNERHELPER_CapSense_Controller_H

#include "CapSense_Controller.h"
#include "CapSense_Controller_CSHL.h"
#if (CapSense_Controller_TUNER_API_GENERATE)
    #include "CapSense_Controller_MBX.h"
    #include "EZI2C.h"
#endif /* (CapSense_Controller_TUNER_API_GENERATE) */


/***************************************
*     Constants for mailboxes
***************************************/

#define CapSense_Controller_DEFAULT_MAILBOXES_NUMBER   (1u)


/***************************************
*        Function Prototypes
***************************************/

void CapSense_Controller_TunerStart(void) ;
void CapSense_Controller_TunerComm(void) ;

#if (CapSense_Controller_TUNER_API_GENERATE)
    CapSense_Controller_NO_STRICT_VOLATILE void CapSense_Controller_ProcessAllWidgets(volatile CapSense_Controller_OUTBOX *outbox)
	                                        					;

    extern volatile CapSense_Controller_MAILBOXES CapSense_Controller_mailboxesComm;
#endif /* (CapSense_Controller_TUNER_API_GENERATE) */

#endif  /* (CY_CAPSENSE_CSD_TUNERHELPER_CapSense_Controller_H)*/


/* [] END OF FILE */
