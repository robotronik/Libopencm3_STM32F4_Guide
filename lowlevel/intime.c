#include "intime.h"

void timer_setup_interrupt(){
    _timer_setup(TIM_RCC,TIM,TIM_PRESCALE,TIM_PERIOD);
    _timer_setup_output_c(TIM,TIM_OC_ID,TIM_OC_MODE,0);

    nvic_enable_irq(TIM_NVIC);
    timer_enable_irq(TIM, TIM_DIER_CCIE);

    _timer_start(TIM);
}

void tim4_isr(){
    if (timer_get_flag(TIM, TIM_SR_CCIF))
	{
        //do something
        fprintf(stderr,"interrupt from timer\n");
        gpio_toggle(GPIOA,GPIO5);

		timer_clear_flag(TIM, TIM_SR_CCIF);
	}
}
