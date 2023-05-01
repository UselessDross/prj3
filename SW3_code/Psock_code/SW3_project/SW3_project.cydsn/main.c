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

#define BUFFER_SIZE 256

uint8_t buffer[BUFFER_SIZE];
uint16_t bufferIndex = 0;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Start the I2C slave component */
    I2CS_Start();

    for (;;)
    {
        /* Check if there is data available to read from the I2C bus */
        if (I2CS_SlaveStatus() & I2CS_SSTAT_RD_CMPLT)
        {
            /* Read the data from the I2C bus */
            uint8_t len = I2CS_SlaveGetReadBufSize();
            I2CS_SlaveClearReadBuf();
            I2CS_SlaveInitReadBuf(buffer, len);

            /* Process the data */
            printf("Received data: %s\n", buffer);

            /* Respond to the data */
            char response[] = "Hello, Raspberry Pi!";
            I2CS_SlaveClearWriteBuf();
            I2CS_SlaveInitWriteBuf(response, sizeof(response));
        }

        /* Check if there is space available to write to the I2C bus */
        if (I2CS_SlaveStatus() & I2CS_SSTAT_WR_CMPLT)
        {
            /* Write data to the I2C bus */
            uint8_t len = I2CS_SlaveGetWriteBufSize();
            I2CS_SlaveClearWriteBuf();
            printf("Writing data: %s\n", buffer);
        }

        /* Delay to prevent overloading the processor */
        CyDelay(1);
    }
}
