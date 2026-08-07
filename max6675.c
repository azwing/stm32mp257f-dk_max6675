#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define SPI_DEVICE "/dev/spidev0.0"

int main() {
    int fd = open(SPI_DEVICE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open SPI device");
        return EXIT_FAILURE;
    }

    // Configure SPI
    uint8_t mode = SPI_MODE_0;  // or SPI_MODE_1
    uint8_t bits = 8;
    uint32_t speed = 4000000;   // 4 MHz (MAX6675 max is 4.3 MHz)

    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0) {
        perror("Failed to set SPI mode");
        close(fd);
        return EXIT_FAILURE;
    }

    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) {
        perror("Failed to set bits per word");
        close(fd);
        return EXIT_FAILURE;
    }

    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
        perror("Failed to set max speed");
        close(fd);
        return EXIT_FAILURE;
    }

    // Send dummy bytes to clock out data
    uint8_t tx[2] = {0x00, 0x00};
    uint8_t rx[2] = {0x00, 0x00};

    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = 2,                // 2 bytes = 16 bits
        .speed_hz = speed,
        .bits_per_word = bits,
    };

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 0) {
        perror("Failed to transfer SPI message");
        close(fd);
        return EXIT_FAILURE;
    }

    // Parse the 16-bit response
    uint16_t raw = (rx[0] << 8) | rx[1];

    // Check for thermocouple open circuit
    if (raw & 0x0004) {
        printf("ERROR: Thermocouple open circuit!\n");
        close(fd);
        return -1;
    }

    // Extract temperature (12-bit value, bits D14-D3)
    float temperature = (raw >> 3) * 0.25f;
    printf("%.2f\n", temperature);

    close(fd);
    return 0;
}
