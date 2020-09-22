/**
 * @file
 * 
 * This file is part of STM32F401REGuide
 * 
 * @brief This function implements external interrupts coming from the blue
 * button PC13
 *
 * @date  09/2020  
 * 
 * Licence :
 * 
 * Robotronik Phelma
 * @author NPXav Benano Trukbidule
*/
#include "exti.h"

void _limit_switch_init(uint32_t exti,uint32_t gpio_port,uint8_t interrupt_number,
        enum exti_trigger_type trig){
    exti_disable_request(exti);

    rcc_periph_clock_enable(RCC_SYSCFG);

    //enable the entry in the vector table of interruption (this table says
    //hey there is an interrupt now you must go there to see which code to 
    //execute)
    nvic_enable_irq(interrupt_number);
    nvic_set_priority(interrupt_number, 1);

    //plug the exti with the right gpio port
    exti_select_source(exti,gpio_port);

    //choose the type of event that will trigger the exti
    exti_set_trigger(exti,trig);
    
    //ajout selon Benoit
    exti_enable_request(exti);

}



void button_switch_init(){
    fprintf(stderr,"initialisation du boutton\n");
    //we simply setup the GPIO as an input
    //_gpio_setup_pin(BUTTON_RCC,BUTTON_PORT,BUTTON_PIN,GPIO_MODE_INPUT);
    /* Enable clock on port*/
    rcc_periph_clock_enable(BUTTON_RCC);
	/* Setup pin as input or output and no pull up or pull down */
    gpio_mode_setup(BUTTON_PORT,GPIO_MODE_INPUT,GPIO_PUPD_PULLUP,BUTTON_PIN);
    
    //we plug the EXTI peripheral with our gpio
    _limit_switch_init(BUTTON_EXTI,BUTTON_PORT,BUTTON_NVIC_INTERRUPT_NUMBER,
            EXTI_TRIGGER_FALLING);
}


/* Interruption routine for the button press */
void exti15_10_isr(){
    fprintf(stderr,"j'entre dans l'interuption\n");
    //checking the origin of the interrupt
    if(exti_get_flag_status(BUTTON_EXTI)){
        //do something like toggle the led !
        gpio_toggle(LED_PORT, LED_PIN);
        //sweet debug message
        fprintf(stderr,"interrupt from the blue button yay !\n");

        //when we leave, we have to lower the flag to tell the uC that yes we 
        //managed the interruption it is safe to go on or else the uC will reenter
        //the interruption right after leaving it => what an ugly loop! 
        exti_reset_request(BUTTON_EXTI);
    }
}

