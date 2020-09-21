#include "gpio.h"

void _gpio_setup_pin_af(enum rcc_periph_clken rcc_clken, uint32_t gpio_port,
        uint16_t gpio_pin, uint8_t gpio_altfun){
	/* Enable clock on port*/
	rcc_periph_clock_enable(rcc_clken);

	/* Setup pin as altfun and no pull up or pull down */
	gpio_mode_setup(gpio_port,GPIO_MODE_AF,GPIO_PUPD_NONE,gpio_pin);
	
	/* Setup pin as push-pull with a default speed of 50 MHz */
	gpio_set_output_options(gpio_port, GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ, gpio_pin); 

	/* Setup alternate function */    
	gpio_set_af(gpio_port, gpio_altfun, gpio_pin);
}

void _gpio_setup_pin(enum rcc_periph_clken clken, uint32_t port,uint16_t pin,
        uint8_t mode){
    /* Enable clock on port*/
    rcc_periph_clock_enable(clken);
	/* Setup pin as input or output and no pull up or pull down */
    gpio_mode_setup(port,mode,GPIO_PUPD_NONE,pin);

    if(mode==GPIO_MODE_OUTPUT){
		gpio_set_output_options(port, GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ, pin); 
    }
}


