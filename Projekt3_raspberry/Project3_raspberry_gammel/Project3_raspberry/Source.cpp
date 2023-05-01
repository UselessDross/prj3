#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define DEVICE_ADDRESS 0x12 // I2C slave address of the PSoC device

int main()
{
    int file;
    char filename[20];
    sprintf(filename, "/dev/i2c-1");

    if ((file = open(filename, O_RDWR)) < 0) {
        printf("Failed to open the i2c bus");
        return 1;
    }

    if (ioctl(file, I2C_SLAVE, DEVICE_ADDRESS) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
        return 1;
    }

    char command[] = "Hello, PSoC!"; // Command to send to the PSoC device
    char response[32] = { 0 }; // Buffer to hold the response from the PSoC device

    // Write the command to the PSoC device
    if (write(file, command, sizeof(command)) != sizeof(command)) {
        printf("Failed to write to the i2c bus.\n");
        return 1;
    }

    // Wait for the PSoC device to process the command
    usleep(10000); // Wait for 10ms

    // Read the response from the PSoC device
    if (read(file, response, sizeof(response)) != sizeof(response)) {
        printf("Failed to read from the i2c bus.\n");
        return 1;
    }

    printf("Response from PSoC: %s\n", response);

    close(file);
    return 0;
}
