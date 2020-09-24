# STM32F401RE Guide
How to flash and write a program on a nucleo F401RE using libopencm3

# Step by step !

1. Install git and clone this repository (see git conference/tutorial)
    * You need a ssh key on your account to clone libopencm3
    * `git clone --recurse-submodules URL_OF_THE_REPO`
    * We will now assume your working directory is this folder ! (`cd` into it please)
2. Install the necessary tools to cross compile and flash your code
    * `sudo pacman -S arm-none-eabi-gcc arm-none-eabi-newlib openocd stlink`
3. We now have to build the library that reads and writes in the STM registers for us : libopencm3!
    * `cd hal_common/libopencm3`
    * `make`
    * `cd -`
5. Give yourself the rights to access the port to talk to the card
    * `make install_udev`
6. Now we can build and flash the program with a simple:
    * `make`

The magic of everything that just happened will kindly be explained by a robotronik member :)
    
# Quick Overview
Gives you some hints about what file does what to know where to start your exploration of the project.

* `Makefile` contains the recipe to build our project and flash it also specifies where our files are hidden
* `compile_command.json` file contains the location of all files for your modern text editor/IDE
* `README.md` is where you actually are
* `install_udev.sh` is a script to give access to the port used in the Step by step
* `doxygen` and `doxygenConf` are used to generate documentation for the project (see Doxygen part for more information)
* `hal_common` contains the `linker_scrpits` and all `libopencm3` files
* `lowlevel` contains the code for your peripheral as Clock, GPIOs, Timers with the .h in `lowlevel/include`
* `mainTest.c` is the main program

# Explanation
Step by step explanation of the code. At any moment you may refer to the documentation (see Doxygen part) 

Only **mainTest.c**, __lowlevel/*.c__ and __lowlevel/include/*.h__ will be edited

For all example it is necessary to use the libopencm3 f4 documentation (I recommend to look up every function used to become Familiar with the definition and parameters in libopencm3)

[Documentation libopencm3 STM32F4](http://libopencm3.org/docs/latest/stm32f4/html/modules.html)

## Usual Structure

### lowlevel/include/*.h
You will first find a header with a **brief** description of the peripheral the **date** and the **author**, feel free to contact them if you encounter any problem

A section with **include** from std, libopencm3, lowlevel/include

A section with **definition** where all peripheral information are chosen/edited and all constant

Then a section with all **prototypes** for function with a **brief** description of the function and description of all **param**eters

### lowlevel/*.c
Including all needed .h files (in lowlevel/include)

Function **starting with _** are not to be called in the main program.

The first function is normally a **setup** used to initialize the peripheral

### main
Including all peripheral .h

In main we start with all **setup and initialization**

The main code that most probably **loop** on itself

## Clock
In all libopencm3 projects you start with the clock. It is normally a very simple module for the system_clock and implementing delay.

Be aware of each μC specific **architecture** the function may vary from a μC to another (e. g. F3 to F4)

In setup, it is important to know the **core frequency** of your μC (84 MHz for the STM32F4)

It is almost the **same** for all projects. You can probably copy paste it and only change the frequency.

It implements all temporal functions of the uC and as to be setup first and enabled for all needed peripheral. We also implement a delay for the uC.

## GPIO
GPIO are all the input/ output of the μC.

They can be used normally in 4 possible mode:
* Digital Input
* Digital Output
* Alternate Function
* Analog Pin


They are 3 setup methods that cover all purposes.

### Digital I/O
The simplest one is to use a pin as a digital I/O, the setup is then:
1. Enable **Clock** on Port (definition as RCC_GPIOX with X the port)
2. Setup **mode** as an input or output with a pull-up, pull-down or neither
3. Finally, only for **output**, configuration of the output (GPIO output type, GPIO pin speed)

### Alternate Function
Pins on a μC are limited therefor multiple function are used on any pin, the function are for example timer controlled pin, communication pin (uart, spi, etc.)

All Information are found in the alternate function mapping in the datasheet (see Hardware Documentation)

Same setup as Digital I/O with the mode AF then set **alternate function** to correct AF (number in the mapping)

### TODO: Analog Pin Setup

## Example 1: Blink a LED
We will use **clock** and **GPIO**, if you run into an issue or want to debug the code further see Debug with uart

Example is already done on the master branch. On your local branch you can delete lowlevel/gpio.c, lowlevel/led.c, lowlevel/include/led.h.
`rm lowlevel/gpio.c lowlevel/led.c lowlevel/include/led.h`
You also need to empty the mainTest.c file. Please replace the content to:
```
#include "clock.h"
#include "uart.h"
#include "gpio.h"
#include "led.h"

int main() {
    //setup
    clock_setup();

    //Your code here    
    
}
```

1. Let's setup the clock in main

    `clock_setup()`
    
2. Create or Edit gpio.c

	`touch lowlevel/gpio.c`

3. For GPIO we only need a setup function `_gpio_setup_pin`

    * Include `gpio.h` in `gpio.c` and copy your prototypes
    
	in `gpio.c` we edit `_gpio_setup_pin`:

	1. Parameters are clken, port, pin, mode

	2. Clock enable is done via `rcc_periph_clock_enable`

		`rcc_periph_clock_enable(clken)`

	3. Setup mode via `gpio_mode_setup`

		`gpio_mode_setup(port,mode,GPIO_PUPD_NONE,pin)`

		* *GPIO_PUPD_NONE* stands for: neither Pull up or Pull down

	4. If we want to setup an output, we need to configure output via `gpio_set_output_options`

	    * In the documentation we find a definition of possible mode: *GPIO_MODE_INPUT*, *GPIO_MODE_OUTPUT*, *GPIO_MODE_AF*, *GPIO_MODE_ANALOG*

	    * We want to check for an output and then setup output

		`gpio_set_output_options(port, GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ, pin)`

		* Type is push-pull or open drain, we normally use *GPIO_OTYPE_PP* 

		* Speed is normally *GPIO_OSPEED_50MHZ*

4. We want a LED module, let's create it

	`touch lowlevel/led.c`

	`touch lowlevel/include/led.h`



We want to setup the led and have an user function to blink it

5. We will use all we wrote in `gpio` so we have to include it

    `#include "gpio.h"`

6. Prototypes in `led.h`

	`void led_setup()`

	`void led_blink()`

We want to use the onboard LED PA5

7. Definitions

	1. port is A 

		`#define LED_PORT GPIOA`

	2. pin is 5 

		`#define LED_PIN GPIO5`

	3. clken should be RCC for our port (A)

		`#define LED_GPIO_RCC RCC_GPIOA`

8. Write `led_setup` in `led.c` using `_gpio_setup_pin`

	PA5 should be an output to control LED

	`_gpio_setup_pin(LED_GPIO_RCC, LED_PORT, LED_PIN, GPIO_MODE_OUTPUT)`

9. Write `led_blink` using libopencm3

	In libopencm3 operation on digital I/O are: clear, set, toggle, get.

	We want to toggle LED state by calling the function

	`gpio_toggle(LED_PORT, LED_PIN)`
	 
10. Write a program in main (`mainTest.c`)

	For example a blinking led at a given frequency/delay

	```
	led_setup();

	while(1){
	      led_blink();
	      delay_ms(1000);
	}
	```

11. **Party**

## Timers

A **timer** is a sequential circuit that **counts** by increasing or decreasing a register. It **resets** and **generates a event**, often used as an **interrupt**, when said register exceeds or falls behind a **predefined value**.

We can then use said **event** to control an output channel (for example a pin as an **alternate function**).

So there are 3 functions we want in our timer module:
* Setup/configure timer
* Start timer
* Setup/configure output channel

### Setup Timer
A STM32F4 as a lot of timer with different possibilities. For illustration purpose we will only use Timer 1 and 2, because they are the most potent one.

The most important parameters for a timer are the **prescaler** and the **period**.
* The prescaler transforms the clock frequency to a lower frequency easier to apprehend for the user. Most of the time we for example divide the clock to make a 1 us tick
* The period is then the temporal distance between two equal values

1. As the timer is also a peripheral we need to enable the **clock**
2. Choose a **mode** for the Timer (it will not be explained in details as it is in most case not changed from example 2)
3. For advanced Timer, you need to allow **breaks**
4. Set the **prescaler**
5. Choose the **starting value**
6. Enable **preload**
7. Choose if the timer should run **continuously**
8. Set the **period** 

### Start Timer
After the setup we need to start the timer. 

1. Generate a **update** event
2. Enable **counter**

### Setup Output Channel
A given Timer as most of the time multiple possible output **channels**. Therefore it is important in the choice of the timer to check the timer, but the output channel too.

The most important parameters for the output channel are its purpose ( **mode** ) and its **value**
* The mode are defined use in the μC as PWM  (see timer_set_oc_mode in libopencm3 documentation)
* The value defines the value of the timer on which the event takes place

We first have to setup the **GPIO** in alternate_function mode

1. See GPIO for the 4 needed steps

	It is important to check the AF number for your chosen timer

After the GPIO is fully setup we can setup the output channel

2. **Disable** the output channel to not generate unforeseen event

3. Choose the **mode** for your output channel

4. Enable **preload**

5. Choose the **value**

6. **Enable** the output channel

## Example 2: Generate a PWM signal with a given pulse width on pin
We will use **clock**, **gpio** and **timer**, if you run in an issue or want to debug the code further see Debug with uart

Example is already done on master branch. On your local branch you can delete lowlevel/timer.c, lowlevel/pwm.c, lowlevel/include/timer.h, lowlevel/include/timer.h.
You can also use gpio from the previous example

1. Let's setup the clock in main

2. Create or Edit timer.h and timer.c

	`touch lowlevel/timer.c`

	`touch lowlevel/include/timer.h`

3. For Timer we first need a setup function `_timer_setup`

    in `_timer_setup`:

    1. Parameters are rcc_clken, timer_peripheral, prescaler, period

    2. Clock enable is done via `rcc_periph_clock_enable`

    	`rcc_periph_clock_enable(rcc_clken)`

    3. Set timer mode via `timer_set_mode`

        `timer_set_mode(timer_peripheral, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP)`

        * *TIM_CR1_CKD_CK_INT* means no division ratio for the prescaled timer
        * *TIM_CR1_CMS_EDGE* means aligned on edge
        * *TIM_CR1_DIR_UP* means the timer is counting up

    4. For advanced timer, enable break via `timer_enable_break_main_output`

    5. Set prescaler via `timer_set_prescaler`

    6. Set the starting value either as a parameter or 0 via `timer_set_repetition_counter`

    7. Enable preload via `timer_enable_preload`

    8. We choose to count in continuous mode via `timer_continuous_mode`

    9. Set period via `timer_set_period`

4. Then we need a start timer function `_timer_start`

    1. Generate the update event via `timer_generate_event(timer_peripheral, TIM_EGR_UG)`
        * *TIM_EGR_UG* is the update event

    2. Enable counter via `timer_enable_counter`

5. Function to setup output channel `_timer_setup_output_c`
    They can be multiple channel active for one timer so this function could be called more than one time per timer

    OC_id, OC_mode, OC_value are specific to the function of your timer 

    Because we are using a pin we need to disable it first and enable it afterward

    1. Disable oc via `timer_disable_oc_output`

    2. Set mode via `timer_set_oc_mode`

    3. Enable preload via `timer_enable_oc_preload`

    4. Set oc value, value that create an event via `timer_set_oc_value`

    5. Enable oc via `timer_enable_oc_output`

6. Add a function to `gpio` to setup alternate function
    
    in `_gpio_setup_pin` change `gpio_mode_setup` to this:

    `gpio_mode_setup(gpio_port,` **GPIO_MODE_AF** `,GPIO_PUPD_NONE,gpio_pin)`

    and add `gpio_set_af` with the new parameter gpio_altfun

    `gpio_set_af(gpio_port, gpio_altfun, gpio_pin)`

7. We want a PWM module, let's create it

	`touch lowlevel/pwm.c`

	`touch lowlevel/include/pwm.h`

We want to setup the PWM and have an user function to change the pulse width

8. Prototypes in `pwm.h`

    `void pwm_setup()`

    `void pwm_set_pulse_width(uint32_t pulse_width)`

Looking at the alternate function mapping we choose TIM1 and PA10. PA10 is the 3rd channel of TIM1 ( *TIM_OC3* ). And OC from TIM1 is the alternate function 1 from PA10 ( *GPIO_AF1* ).

We choose a prescaler of 84 to get tick in us and a period of 20 000 us = 20 ms <=> 50 Hz. Imagine for example the driving of a servo.

We want to make a PWM and gladly there are already PWM mode on STM32F4: *TIM_OCM_PWM1* (High then Low) and *TIM_OCM_PWM2* (Low then High)

9. Definitions

    ```
    #define PWM_PRESCALE (84)
    #define PWM_PERIOD (20000)
    
    #define PWM_TIM     TIM1
    #define PWM_TIM_RCC     RCC_TIM1
    #define PWM_GPIO_RCC_EN     RCC_GPIOA
    #define PWM_PORT_EN     GPIOA
    #define PWM_PIN_EN      GPIO10
    
    #define PWM_AF      GPIO_AF1
    #define PWM_OC_ID   TIM_OC3
    #define PWM_OC_MODE     TIM_OCM_PWM1
    ```

10. Write `pwm_setup` using `gpio` and `timer`

    ```
    _timer_setup(PWM_TIM_RCC, PWM_TIM, PWM_PRESCALE, PWM_PERIOD);
    _gpio_setup_pin_af(PWM_GPIO_RCC_EN, PWM_PORT_EN, PWM_PIN_EN, PWM_AF);
    _timer_setup_output_c(PWM_TIM, PWM_OC_ID, PWM_OC_MODE, 0);
    _timer_start(PWM_TIM);
    ```

11. Write `pwm_set_pulse_width` changing the oc_value

    `timer_set_oc_value(PWM_TIM, PWM_OC_ID, pulse_width)`

12. Write a program

    Varying the pulse width in time

    ```
    pwm_setup();

    uint32_t pw = 10;

    while (1) {
        pw = (pw+100)%20000;
        pwm_set_pulse_width(pw);
        delay_ms(100);
    }
    ```

13. **Party Harder**

**Bonus**: Using a timer enables to do anything in the main parallel to the timer. We can for example imagine a modification to example 1 using a timer so we can execute code while the LED is blinking.

# Exemple 3: Generate an interrupt from a GPIO output thanks to the EXTI perpipheral

1. You first need to enable the clock SYSCFG which will handle the EXTI(for
external interrupt). The idea is that your GPIO input will change state and
when this change happens it will raise a flag that is a signal for the CPU
to say hey something happened there. And that is the EXTI peripheral that handles
this signal. We have to link the exti line with our GPIO.
* Write `exti_setup` (use `rcc_periph_clock_enable`)

2. The function `_limit_switch_init` will be our universal function to 
initialize limit switches (or any exti interrupt from GPIO).
* Disable requests to avoid false activation during setup with 
`exti_disable_request`
* Select which GPIO port will connect to exti number something. All pins of the
same number are multiplexed into one exti so for exemple GPIO A1;B1;C1 all
all connect to EXTI_1 `exti_select_source`
* Now we can choose which type of event on the exti line will actually raise the
flag to notify an event happened, it could be a rising edge, a falling one or 
maybe we want both to trigger the flag. `exti_set_trigger`

* Now we have to enable the EXTI interrupt `exti_enable_request` and enable the entry that will point
and tell the µC what code (the interrupt routine= a function) should be executed
when it sees the risen flag(this information is contained in the NVIC for
nested vector interrupt control) `nvic_enable_irq`.

3. We will now write a function to initialize the interrupts coming from the
blue button (on PC13). It is the function `button_switch_init`
We have to initialize the GPIO as an input as we did before.
* Start the clock of the port `rcc_periph_clock_enable`
* Setup the port as input `gpio_mode_setup`
* Use `_limit_switch_init` to plug the exti line with the GPIO
* Choose the priority of interrupts with `nvic_set_priority`

# usual README of our repositories

Reminder: To clone the submodule in the same time, use `git clone --recurse-submodules`, then don't forget to build libopencm3. You need to execute `make install_udev` one time to add the permission to flash.
If you forgot to clone with submodule just run `git submodule update --init --recursive`

To compile and flash you need gcc-arm-none-eabi, st-link and  openocd

To build: `make mainTest.elf`
To flash: `make mainTest.flash`
To clean: `make clean`

## Software documentation
You can access the documentation generated from the code with doxygen (see the doxygen paragraph to generate it) in the doxygen/html or latex.

## Hardware documentation
μC used: STM32F401RE on a Nucleo-64 board, main doc:
* [Reference Manual STM32F401 line, doc RM0368, 847 pages](https://www.st.com/resource/en/reference_manual/dm00096844-stm32f401xbc-and-stm32f401xde-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
* [STM32F401xD/xE datasheet, 135 pages](https://www.st.com/resource/en/datasheet/stm32f401re.pdf)
    * Alternate function mapping, p45, table 9
* [Reference STM Nucleo-64 Board, doc UM1724, 69 pages](https://www.st.com/resource/en/user_manual/dm00105823-stm32-nucleo64-boards-mb1136-stmicroelectronics.pdf)

## Doxygen

Generating the documentation with doxygen:
--> install doxygen on your system

--> run `doxygen doxygenConf` from the project directory

--> the documentation can then be read from doxygen/html/index.html in a browser

--> to generate the pdf for the github (or for yourself !) go into the latex directory `cd doxygen/latex` and run `make`.

Note: You must have a latex distribution on your computer that has `pdflatex` command.

## Generate the database for your IDE
--> to update the compile_command.json you can use bear (available in the AUR)
--> run `make clean`
--> run `bear /compilation command/`

## Debug with uart
--> install picocom
--> find your card `ls /dev` . It should be /dev/ttyACM0
--> run picocom with `picocom -b 9600 /dev/ttyACM0`
--> in main setup uart
--> in code use fprintf(stderr,message) to debug

## Coding style

* Tabs are spaces = 4

* Column = 80

* Brackets :
    ```
    def peripheral_action_subjectofaction(params){
        code...
        some more code...
    }
    ```
* Function naming
    * _ is the separator
    * peripheral_action_subjectofaction
    * function starting with _ are private and should not be called in high level code
  
* Variables
    * my_var

* Documentation using doxygen
    * comment the function interface in the .h files (javadoc like)
    * detail the function in .c files

* We envision three levels for the code :
    * lowlevel functions that must be as general as possible to setup the hardware config (ex: timer functions)
    * lowlevel modules with the functions called by the user (ex: motor module, with setup and speed/dir functions)
    * rolling unit level (ex: control engineering)
