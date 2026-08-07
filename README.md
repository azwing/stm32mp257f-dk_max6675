This sample shows haw to use SPI on the STM32MP257F-DK discovery kit to read a max6675 thermocouple converter.

Prerequisites:
set up stm32 developement tools and sources on your linux 
see https://wiki.st.com/stm32mpu/wiki/Getting_started/STM32MP2_boards/STM32MP257x-DK/Develop_on_Arm_Cortex-A35/Modify,_rebuild_and_reload_the_Linux_kernel


the difficulty was not the code itself but the configuration of kit to get SPI to work on linux.

First edit /linux-6.6.129/arch/arm64/boot/dts/st/stm32mp257f-dk.dts to enable SPI and configure the chip select GPIO
Then compile it and upload it to /boot/stm32mp257f-dk.dtb on the discovery kit
reboot and you can see spi is actvated by issuing:

$> lsmod | grep spi
spidev                 28672  0
spi_stm32              40960  0

and 

$> ls -l /dev/spi*
crw------- 1 root root 153, 0 Aug  7 10:23 /dev/spidev0.0

Then you can compile the max6675.c file and upload it via scp on the discovery kit

