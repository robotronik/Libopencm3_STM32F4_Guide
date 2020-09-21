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
    //plug the exti with the right gpio port
    exti_select_source(exti,gpio_port);
    //choose the type of event that will trigger the exti
    exti_set_trigger(exti,trig);
    //enable the entry in the vector table of intteruption (this table says
    //hey there is an interrupt now you must go there to see which code to 
    //execute)
    nvic_enable_irq(interrupt_number);
}



void button_switch_init(){
    //we simply setup the GPIO as an input
    _gpio_setup_pin(BUTTON_RCC,BUTTON_PORT,BUTTON_PIN,GPIO_MODE_INPUT);
    //we plug the EXTI peripheral with our gpio
    _limit_switch_init(BUTTON_EXTI,BUTTON_PORT,BUTTON_NVIC_INTERRUPT_NUMBER,
            EXTI_TRIGGER_RISING);
}

void exti15_10_isr(){
    //checking the origin of the interrupt
    if(exti_get_flag_status(BUTTON_EXTI)){
        //do something like toggle the led !
        gpio_toggle(LED_PORT, LED_PIN);
        //sweet debug message
        fprintf(stderr,"interrupt from the blue button yay !");

        //when we leave, we have to lower the flag to tell the uC that yes we 
        //managed the interruption it is safe to go on or else the uC will reenter
        //the interruption right after leaving it => what an ugly loop! 
        exti_reset_request(BUTTON_EXTI);
    }
}

