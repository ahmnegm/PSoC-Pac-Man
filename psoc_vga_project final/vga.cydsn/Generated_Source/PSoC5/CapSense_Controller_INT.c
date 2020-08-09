/*******************************************************************************
* File Name: CapSense_Controller_INT.c
* Version 3.50
*
* Description:
*  This file provides the source code of Interrupt Service Routine (ISR)
*  for CapSense CSD component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2013, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "CapSense_Controller.h"



/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START CapSense_Controller_IsrCH_ISR_intc` */

/* `#END` */


#if (CapSense_Controller_DESIGN_TYPE == CapSense_Controller_ONE_CHANNEL_DESIGN)
    /*******************************************************************************
    * Function Name: CapSense_Controller_IsrCH0_ISR
    ********************************************************************************
    *
    * Summary:
    *  This ISR is executed when measure window is closed. The measure window 
    *  depends on scan resolution parameter of sensor. 
    *  The ISR has two modes: 
    *   1. Scans single sensor and stores measure result.
    *   2. Stores result of current measure and starts next scan till all 
    *      enabled sensors will be scanned.
    *  This interrupt handler only used in one channel designs.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  CapSense_Controller_csv - used to provide status and mode of scanning process.
    *   Mode - single scan or scan of all enabled sensors.
    *   Status - scan in progress or ready for new scan.
    *  CapSense_Controller_sensorIndex - used to store sensor scanning sensor number.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    CY_ISR(CapSense_Controller_IsrCH0_ISR)
    {
        #ifdef CapSense_Controller_ONE_CHANNEL_CH0_ISR_ENTRY_CALLBACK
            CapSense_Controller_IsrCH0_ISR_EntryCallback();
        #endif /* CapSense_Controller_ONE_CHANNEL_CH0_ISR_ENTRY_CALLBACK */

        /*  Place your Interrupt code here. */
        /* `#START CapSense_Controller_ONE_CHANNEL_IsrCH0_ISR_ENTER` */

        /* `#END` */

        /* Save results and disable sensor */
        CapSense_Controller_PostScan(CapSense_Controller_sensorIndex);
                
        if ((CapSense_Controller_csv & CapSense_Controller_SW_CTRL_SINGLE_SCAN) != 0u)
        {
            CapSense_Controller_csv &= (uint8)~CapSense_Controller_SW_STS_BUSY;
        }
        else
        {
            /* Proceed the scanning */
            CapSense_Controller_sensorIndex = CapSense_Controller_FindNextSensor(CapSense_Controller_sensorIndex);
            
            /* Check end of scan */
            if(CapSense_Controller_sensorIndex < CapSense_Controller_TOTAL_SENSOR_COUNT)
            {
                CapSense_Controller_PreScan(CapSense_Controller_sensorIndex);
            }
            else
            {
                CapSense_Controller_csv &= (uint8)~CapSense_Controller_SW_STS_BUSY;
            } 
        }

        /*  Place your Interrupt code here. */
        /* `#START CapSense_Controller_ONE_CHANNEL_IsrCH0_ISR_EXIT` */

        /* `#END` */

        #ifdef CapSense_Controller_ONE_CHANNEL_CH0_ISR_EXIT_CALLBACK
            CapSense_Controller_IsrCH0_ISR_ExitCallback();
        #endif /* CapSense_Controller_ONE_CHANNEL_CH0_ISR_EXIT_CALLBACK */
    }
 #endif  /* (CapSense_Controller_DESIGN_TYPE == CapSense_Controller_ONE_CHANNEL_DESIGN) */


 #if (CapSense_Controller_DESIGN_TYPE == CapSense_Controller_TWO_CHANNELS_DESIGN)   
    /*******************************************************************************
    * Function Name: CapSense_Controller_IsrCH0_ISR
    ********************************************************************************
    *
    * Summary:
    *  This ISR is executed when measure window is closed. The measure window 
    *  depends on scan resolution parameter of sensor. 
    *  The ISR has two modes: 
    *   1. Scans single sensor or pair of sensors and stores measure result.
    *   2. Stores results of current measure when complete scan of sensors pair or 
    *      sensor scan and starts next enabled sensors pair or sensor scan. 
    *  This interrupt handler only used in two channel designs.
    *  The synchronization is added to scanning process. The ISR channel 0 or 
    *  channel 1 stores the result and starts next scan. It depends which channel
    *  scans longer. The biggest scan resolution parameter of sensor within pair 
    *  defines scanning time of pair.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  CapSense_Controller_csv - used to provide status and mode of scanning process.
    *   Mode - single scan or scan of all enabled sensors.
    *   Status - scan in progress or ready for new scan.
    *  CapSense_Controller_sensorIndex - used to store sensor scanning sensor number.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    CY_ISR(CapSense_Controller_IsrCH0_ISR)
    {
        #ifdef CapSense_Controller_TWO_CHANNEL_CH0_ISR_ENTRY_CALLBACK
            CapSense_Controller_IsrCH0_ISR_EntryCallback();
        #endif /* CapSense_Controller_TWO_CHANNEL_CH0_ISR_ENTRY_CALLBACK */

        /*  Place your Interrupt code here. */
        /* `#START CapSense_Controller_TWO_CHANNELS_IsrCH0_ISR_ENTER` */

        /* `#END` */
        
        CapSense_Controller_CONTROL_REG &= ~CapSense_Controller_CTRL_WINDOW_EN__CH0;
        CapSense_Controller_PostScanCh0(CapSense_Controller_sensorIndex);
        
        if((CapSense_Controller_CONTROL_REG & CapSense_Controller_CTRL_WINDOW_EN__CH1) == 0u)
        {
            if ((CapSense_Controller_csv & CapSense_Controller_SW_CTRL_SINGLE_SCAN) != 0u)
            {
                CapSense_Controller_csv &= (uint8)~CapSense_Controller_SW_STS_BUSY;
            }
            else
            {
                /* Procced the scanning */
                CapSense_Controller_sensorIndex = CapSense_Controller_FindNextPair(CapSense_Controller_sensorIndex);
                
                /* Check end of scan conditions */
                if((CapSense_Controller_sensorIndex < CapSense_Controller_TOTAL_SENSOR_COUNT__CH0) || 
                   (CapSense_Controller_sensorIndex < CapSense_Controller_TOTAL_SENSOR_COUNT__CH1))
                {
                    CapSense_Controller_PreScan(CapSense_Controller_sensorIndex);
                }
                else
                {
                    CapSense_Controller_csv &= (uint8)~CapSense_Controller_SW_STS_BUSY;
                }
            }
        }
        else
        {
            /* Do nothing */
        }

		/*  Place your Interrupt code here. */
        /* `#START CapSense_Controller_TWO_CHANNELS_IsrCH0_ISR_EXIT` */

        /* `#END` */

        #ifdef CapSense_Controller_TWO_CHANNEL_CH0_ISR_EXIT_CALLBACK
            CapSense_Controller_IsrCH0_ISR_ExitCallback();
        #endif /* CapSense_Controller_TWO_CHANNEL_CH0_ISR_EXIT_CALLBACK */
    }

    
    /*******************************************************************************
    * Function Name: CapSense_Controller_IsrCH1_ISR
    ********************************************************************************
    *
    * Summary:
    *  This ISR is executed when measure window is closed. The measure window 
    *  depends on scan resolution parameter of sensor. 
    *  The ISR has two modes: 
    *   1. Scans single sensor or pair of sensors and stores measure result.
    *   2. Stores results of current measure when complete scan of sensors pair or 
    *      sensor scan and starts next enabled sensors pair or sensor scan. 
    *  This interrupt handler only used in two channel designs.
    *  The synchronization is added to scanning process. The ISR channel 0 or 
    *  channel 1 stores the result and starts next scan. It depends which channel
    *  scans longer. The biggest scan resolution parameter of sensor within pair 
    *  defines scanning time of pair.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Global Variables:
    *  CapSense_Controller_csv - used to provide status and mode of scanning process.
    *   Mode - single scan or scan of all enabled sensors.
    *   Status - scan in progress or ready for new scan.
    *  CapSense_Controller_sensorIndex - used to store sensor scanning sensor number.
    *
    * Reentrant:
    *  No
    *
    *******************************************************************************/
    CY_ISR(CapSense_Controller_IsrCH1_ISR)
    {
        #ifdef CapSense_Controller_TWO_CHANNEL_CH1_ISR_ENTRY_CALLBACK
            CapSense_Controller_IsrCH1_ISR_EntryCallback();
        #endif /* CapSense_Controller_TWO_CHANNEL_CH1_ISR_ENTRY_CALLBACK */

        /*  Place your Interrupt code here */
        /* `#START CapSense_Controller_TWO_CHANNELS_IsrCH1_ISR_ENTER` */

        /* `#END` */
        
        CapSense_Controller_CONTROL_REG &= (uint8)~CapSense_Controller_CTRL_WINDOW_EN__CH1;
        CapSense_Controller_PostScanCh1(CapSense_Controller_sensorIndex + CapSense_Controller_TOTAL_SENSOR_COUNT__CH0);
        
        if((CapSense_Controller_CONTROL_REG & CapSense_Controller_CTRL_WINDOW_EN__CH0) == 0u)
        {
            if ((CapSense_Controller_csv & CapSense_Controller_SW_CTRL_SINGLE_SCAN) != 0u)
            {
                CapSense_Controller_csv &= (uint8)~CapSense_Controller_SW_STS_BUSY;
            }
            else 
            {
                /* Procced the scanning */
                CapSense_Controller_sensorIndex = CapSense_Controller_FindNextPair(CapSense_Controller_sensorIndex);
                
                /* Check end of scan conditions */
                if((CapSense_Controller_sensorIndex < CapSense_Controller_TOTAL_SENSOR_COUNT__CH0) || 
                   (CapSense_Controller_sensorIndex < CapSense_Controller_TOTAL_SENSOR_COUNT__CH1))
                {
                    CapSense_Controller_PreScan(CapSense_Controller_sensorIndex);
                }
                else
                {
                    CapSense_Controller_csv &= ~CapSense_Controller_SW_STS_BUSY;
                }
            }
        }
        else
        {
            /* Do nothing */
        }

		/*  Place your Interrupt code here */
        /* `#START CapSense_Controller_TWO_CHANNELS_IsrCH1_ISR_EXIT` */

        /* `#END` */

        #ifdef CapSense_Controller_TWO_CHANNEL_CH1_ISR_EXIT_CALLBACK
            CapSense_Controller_IsrCH1_ISR_ExitCallback();
        #endif /* CapSense_Controller_TWO_CHANNEL_CH1_ISR_EXIT_CALLBACK */
    }
#endif  /* (CapSense_Controller_DESIGN_TYPE == CapSense_Controller_TWO_CHANNELS_DESIGN) */


/* [] END OF FILE */
