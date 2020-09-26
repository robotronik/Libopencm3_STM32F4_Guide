#include "timer.h"

void _timer_setup(enum rcc_periph_clken rcc_clken, uint32_t timer_peripheral,uint32_t prescaler,uint32_t period){
  rcc_periph_clock_enable(rcc_clken);
  timer_set_mode(timer_peripheral, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
  timer_enable_break_main_output(timer_peripheral);
  timer_set_prescaler(timer_peripheral,prescaler);
  timer_set_repetition_counter(timer_peripheral, 0);
  timer_enable_preload(timer_peripheral);
  timer_continuous_mode(timer_peripheral);
  timer_set_period(timer_peripheral,period);
}
