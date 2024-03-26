#include "GPIO.h"

#include <bcm2835.h>

// int main(int argc,char **argv)
int GPIOmotor() {
    if (!bcm2835_init())
        return 1;
    bcm2835_gpio_fsel(PIN1, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(PIN2, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(PIN3, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(PIN4, BCM2835_GPIO_FSEL_OUTP);
    // while (1) {
        bcm2835_gpio_write(PIN1, HIGH);
        bcm2835_gpio_write(PIN2, HIGH);
        bcm2835_gpio_write(PIN3, HIGH);
        bcm2835_gpio_write(PIN4, HIGH);
        // bcm2835_delay(500);
        // bcm2835_gpio_write(PIN,LOW);
        // bcm2835_delay(500);
    // }
    // bcm2835_close();
    // return 0;
}

int GPIOmotoroff() {
    if (!bcm2835_init())
        return 1;
    bcm2835_gpio_fsel(PIN1, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(PIN2, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(PIN3, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(PIN4, BCM2835_GPIO_FSEL_OUTP);
    while (1) {
        bcm2835_gpio_write(PIN1, LOW);
        bcm2835_gpio_write(PIN2, LOW);
        bcm2835_gpio_write(PIN3, LOW);
        bcm2835_gpio_write(PIN4, LOW);
        // bcm2835_delay(500);
        // bcm2835_gpio_write(PIN,LOW);
        // bcm2835_delay(500);
    }
    bcm2835_close();
    // return 0;
}