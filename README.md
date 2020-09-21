# STM32F401REGuide
how to flash a program on a nucleo F401RE using libopencm3

# Step by step !

1. Install git and clone this repository (cf git conference/tutorial)
    * `git clone --recurse-submodules URL_OF_THE_REPO`
    * We will now assume your working directory is this folder ! (`cd` into it please)
2. Install the necessary tools to cross compile and flash your code
    * `sudo pacman -S arm-none-eabi-gcc arm-none-eabi-newlib openocd stlink`
3. We now have to build the library that read and writes in the STM registeries for us : libopencm3 !
    * `cd hal_common/libopencm3`
    * `make`
    * `cd ../..`
5. Give yourself the rights to access the port to talk the card
    * `make install_udev`
6. Now we can build and flash the program with a simple:
    * `make`

The magic of everything that just happened will kindly be explained by a robotronik member :)

# Quick Overview
Gives you some hints about what file do what to know where to start your exploration of the project.

* `Makefile` contains the recipe to build our project and flash it also specifies where our files are hidden
* `compile_command.json` file contains the location of all files for your modern text editor/IDE
* `README.md` is where you actually are
* `install_udev.sh` is a script to give acces to the port used in the Step by step
* `doxygen` and `doxygenConf` are used to generate documentation for the project (cf. Doxygen part for more information)
* `hal_common` contains the `linker_scrpits` and all `libopencm3` files
* `lowlevel` contains the code for your peripheral as Clock, GPIOs, Timers with the .h in `lowlevel/include`
* `mainTest.c` is the main program

# Explanation
Step by step explanation of the code. At any moment you may refer to the documentation (cf Doxygen part) 
Only **mainTest.c**, __Motlowlevel/*.c__ and __lowlevel/include/*.h__ will be edited

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

In main we start with all **setup and initialisation**

The the main code that most probably **loop** on itself

## Clock
In all libopencm3 project you start with the clock. It is normally a very simple module for the system_clock and implementing delay.

Be aware of uC specific **architecture** the function may vary from a uC to another (e. g. F3 to F4)

In setup it is important to know the **core frequency** of your uC (84 MHz for the STM32F4)

It is almost the **same** for all project. You can probably copy paste it and only change the frequency.

## GPIO
GPIO are all the input/ output of the uC.

They can be used normaly in 4 possibilities:
* Digital Input
* Digital Output
* Analog Pin
* Alternate Function

They are 3 setup methods for all purposes.

### Digital I/O
The simplest one is to use a pin as an digital I/O, the setup is then:
1. Enable **Clock** on Port (definition as RCC_GPIOX with X the port)
2. Setup **mode** as an input or output with a pull-up, pull-down or neither
3. Finally only for **output**, configuartion of the output (gpio output type, gpio pin speed)

### Alternate Function
Pins on a uC are limited therefor multiple fuction are used on any pin, function are for example timer controlled pin, communication pin (uart, spi,etc. )

All Information are found in the alternate function mapping in the datasheet (cf Hardware Documentation)

Same setup as Digital I/O with mode af then set **alternate function** to correct af (number in the mapping)

### TODO: Analog Pin Setup

## Example 1: Blink a LED
We will use **clock** and **gpio**, if you run in an issue or want to debug the code further see Debug with uart

Example is already done on master branch. On your local branch you can delete lowlevel/gpio.c, lowlevel/led.c, lowlevel/include/gpio.h, lowlevel/include/led.h.

1. Let's setup the clock in main
2. Create or Edit gpio.h and gpio.c

	`touch lowlevel/gpio.c`

	`touch lowlevel/include/gpio.h`
	
3. For GPIO we only need a setup function `_gpio_setup_pin`

	in `_gpio_setup_pin`:

	1. Parameters are rcc_clken, port, pin, mode

	2. Clock enbale is done via `rcc_periph_clock_enable`

		`rcc_periph_clock_enable(rcc_clken)`

	3. Setup mode via `gpio_mode_setup`

		`gpio_mode_setup(port,mode,GPIO_PUPD_NONE,pin)`

		* *GPIO_PUPD_NONE* stands for: neither Pull up or Pull down

	4. If we want to setup an output, we need to configure output via `gpio_set_output_options`

		`gpio_set_output_options(port, GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ, pin)`

		* Type is push-pull or open drain, we normally use *GPIO_OTYPE_PP* 

		* Speed is normally *GPIO_OSPEED_50MHZ*

4. We want a LED module, let's create it

	`touch lowlevel/led.c`

	`touch lowlevel/include/led.h`

We want to setup the led and have an user function to blink it

5. Prototypes in `led.h`

	`void led_setup()`

	`void led_blink()`

We want to use the onboard LED PA5

6. Definitions

	1. port is A 

		`#define LED_PORT GPIOA`

	2. pin is 5 

		`#define LED_PIN GPIO5`

	3. rcc_clken should be rcc for our port (A)

		`#define LED_GPIO_RCC RCC_GPIOA`

7. Write `led_setup` using `_gpio_setup_pin`

	PA5 should be an output to control LED

	`_gpio_setup_pin(LED_GPIO_RCC, LED_PORT, LED_PIN, GPIO_MODE_OUTPUT)`

8. Write `led_blink` using libopencm3

	In libopencm3 operation on digital I/O are clear, set, toggle, get.

	We want to toggle LED state by calling the function

	`gpio_toggle(LED_PORT, LED_PIN)`
	 
9. Write a program

	For example a blinking led at a given frequency/delay

	```
	led_setup();

	while(1){
	      led_blink();
	      delay_ms(delay);
	}
	```

10. **Party**

## Timers

A **timer** is a sequential circuit that **counts** by increasing or decreasing a register. It **resets** and **generates a event**, often used as an **interrupt**, when said register exceeds or falls behind a **predefined value**.

We can then use said **event** to control a pin (output channel) as an **alternate function**.

So there are 3 functions we want in our timer module:
	* Setup/configure timer
	* Start timer
	* Setup/configure output channel

### Setup Timer
A STM32F4 as a lot of timer with different possibilities. For illustartion purpose we will use only Timer 1 and 2, because they are the most potent one.

The most important parameters for a timer are the **prescaler** and the **period**.
	* The prescaler transforms the clock frequency to a lower frequency easier to aprehend for the user. Most of the time we for example divide the clock to make a 1 us tick
	* The period is then the temporal distance between two equal values

1. As the timer is also a periph we need to enable the **clock**
2. Choose a **mode** for your Timer (it will not be explained in details as it is in most case not changed from example 2)
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
A given Timer as most of the time multiple possible output **channels**. Therefore it is important in the choice of the timer to check the timer but the ouput channel too.

The most important parameters for the output channel are its purpose ( **mode** ) and its **value**
	* The mode are defined use in the uC as PWM (cf. timer_set_oc_mode in libopencm3 documentation)
	* The value defines the value of the timer on wich the event takes place

We first have to setup the **gpio** in alternate_function mode
1. See GPIO for the 4 needed steps
	It is important to check the af number for your chosen timer

After the gpio is fully setup we can setup the output channel
2. **Disable** the output channel to not generate unforseen event
3. Choose the **mode** for your output channel
4. Enable **preload**
5. Choose the **value**
6. **Enable** the output channel

## Example 2: Generate a PWM signal with a given duty cycle on pin
We will use **clock** and **gpio**, if you run in an issue or want to debug the code further see Debug with uart

Example is already done on master branch. On your local branch you can delete lowlevel/gpio.c, lowlevel/led.c, lowlevel/include/gpio.h, lowlevel/include/led.h.


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
Microcontroller used: STM32F401RE on a Nulceo-64 board, main doc:
* [Reference Manual STM32F401 line, doc RM0368, 847 pages](https://www.st.com/resource/en/reference_manual/dm00096844-stm32f401xbc-and-stm32f401xde-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
* [STM32F401xD/xE datasheet, 135 pages](https://www.st.com/resource/en/datasheet/stm32f401re.pdf)
    * Alternate function mapping, p45, table 9
* [Reference STM Nuleo-64 Board, doc UM1724, 69 pages](https://www.st.com/resource/en/user_manual/dm00105823-stm32-nucleo64-boards-mb1136-stmicroelectronics.pdf)

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
    * lowlevel fuctions that must be as general as possible to setup the hardware config (ex: timer fuctions)
    * lowlevel modules with the functions called by the user (ex: motor module, with setup and speed/dir fuctions)
    * rolling unit level (ex: control engineering)
