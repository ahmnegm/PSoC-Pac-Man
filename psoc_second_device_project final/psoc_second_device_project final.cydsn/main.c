/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    CyGlobalIntEnable;
    
    //UART
    UART_Start();
    
    //LCD
    LCD_Char_1_Start();					// initialize lcd
	LCD_Char_1_ClearDisplay();

    for(;;)
    {
        if (!(UART_ReadRxStatus() & UART_RX_STS_FIFO_NOTEMPTY)) {
            continue;
        }
        
        //get score via UART
        uint8 score = UART_GetChar();
        
        //display score on LCD screen
        LCD_Char_1_ClearDisplay();
        LCD_Char_1_PrintString("Score: ");
		LCD_Char_1_PrintNumber(score);
        
        //artificial delay for LCD screen
        CyDelay(55);
    }
}

/* [] END OF FILE */
