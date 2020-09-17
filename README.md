# STM32F401REGuide
how to flash a blinky program on a nucleo F401RE using libopencm3

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
4. Give yourself the rights to access the port to talk the card
    * `make install_udev`
5. Now we can build and flash the program with a simple:
    * `make`

The magic of everything that just happened will kindly be explained by a robotronik member :)

# Quick explanation
Gives you some hints about what file do what to know where to start your exploration of the project.

* The `Makefile` contains the recipe to build our project and flash it also specifies where our files are hidden
* If you are running a modern text editor/IDE it knows where the files are thanks to the `compile_command.json` file
* We have to initialize the clock of the µcontroller in `clock.c`
* We tell what input/output should do thanks to GPIO initilization procedures `gpio.c`
* The main algorithm of the blinky program is in `mainTest.c`
* Headers are located in `lowlevel/include` and you should go read them, they have documentation !

#TODO
Give hints about how to implement interruption from the blue button PC13



## usual README of our repositories

Reminder: To clone the submodule in the same time, use `git clone --recurse-submodules`, then don't forget to build libopencm3. You need to execute `make install_udev` one time to add the permission to flash.
If you forgot to clone with submodule just run `git submodule update --init --recursive`

To compile and flash you need gcc-arm-none-eabi, st-link and  openocd

To build: `mainTest.elf`
To flash: `mainTest.flash`
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
