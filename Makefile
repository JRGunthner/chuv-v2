#===================================================================================================
# Makefile for ShowerShow Project - Juliano Günthner
#===================================================================================================

#---------------------------------------------
# Version  
MAJOR=0
MINOR=0
BUILD=1

#---------------------------------------------
# Shower Type
SHOWER=MY_SHOWER
#SHOWER=SHOWER_SHOW

#---------------------------------------------
# Shower Type
#VOLTAGE=_127V
VOLTAGE=_220V

#--------------------------------------------
# Device Configuration
DEVICE=STM32F030
#DEVICE=STM32F031
#DEVICE=STM32F042
#DEVICE=STM32F051
#DEVICE=STM32F072

#--------------------------------------------
# STM32F0xx Devices vector table (.s file)
ifeq ($(DEVICE),STM32F030)
STARTUP=startup_stm32f030
endif
ifeq ($(DEVICE),STM32F031)
STARTUP=startup_stm32f031
endif
ifeq ($(DEVICE),STM32F042)
STARTUP=startup_stm32f042
endif
ifeq ($(DEVICE),STM32F051)
STARTUP=startup_stm32f051
endif
ifeq ($(DEVICE),STM32F072)
STARTUP=startup_stm32f072
endif

#--------------------------------------------
# Linker script for STM32F0 (.ld file)
ifeq ($(DEVICE),STM32F030)
LD_FILE=STM32F030R8_FLASH
endif
ifeq ($(DEVICE),STM32F031)
LD_FILE=STM32F031C6_FLASH
endif
ifeq ($(DEVICE),STM32F042)
LD_FILE=STM32F042C6_FLASH
endif
ifeq ($(DEVICE),STM32F051)
LD_FILE=STM32F051R8_FLASH
endif
ifeq ($(DEVICE),STM32F072)
LD_FILE=STM32F072VB_FLASH
endif


#--------------------------------------------
# StdPeriph_Lib Configuration - Path and Name for output file
STDPERIPH_DIR=./StdPeriph_Lib
STDPERIPH_LIB=stm32f0
STDPERIPH_OUTPUT=lib$(STDPERIPH_LIB)

#--------------------------------------------
# FreeRTOS Configuration - Path and Name for output file
FREERTOS_DIR=./FreeRTOS
FREERTOS_LIB=freertos
FREERTOS_OUTPUT=lib$(FREERTOS_LIB)

#--------------------------------------------
# Printf Lib
LIBPRINTF_DIR=Printf_Lib
LIBPRINTF_LIB=printf
LIBPRINTF_OUTPUT=lib$(LIBPRINTF_LIB)

#--------------------------------------------
# Drivers Folder
DRV_DIR=./drv

#---------------------------------------------
# Output File with version
OUTPUT=PLCHUDESW_$(MAJOR)_$(MINOR)_$(BUILD)_$(VOLTAGE)

#--------------------------------------------
# Compiler
TOOLS_PREFIX=arm-none-eabi-
CC=$(TOOLS_PREFIX)gcc
CP=$(TOOLS_PREFIX)objcopy
OD=$(TOOLS_PREFIX)objdump -S
SIZE=$(TOOLS_PREFIX)size

#--------------------------------------------
# Compiler Flags
CFLAGS= -DUSE_STDPERIPH_DRIVER -D$(DEVICE) -c -fno-common -O2 -Wall -g3 -mcpu=cortex-m0 -mthumb
CFLAGS+=-DVERSION_MAJOR=$(MAJOR) -DVERSION_MINOR=$(MINOR) -DVERSION_BUILD=$(BUILD) -D$(SHOWER) -D$(VOLTAGE)

#--------------------------------------------
# Includes
INCLUDES=	-I.         				\
			-I$(STDPERIPH_DIR)/cmsis  	\
			-I$(STDPERIPH_DIR)        	\
			-I$(STDPERIPH_DIR)/inc      \
			-I$(FREERTOS_DIR)       	\
			-I$(FREERTOS_DIR)/inc       \
			-I$(LIBPRINTF_DIR)
			
DVR_INCLUDES = 	-I$(DRV_DIR)/User	    \
				-I$(DRV_DIR)/Display    \
				-I$(DRV_DIR)/LED	    \
				-I$(DRV_DIR)/IrDA       \
				-I$(DRV_DIR)/AD         \
				-I$(DRV_DIR)/Flowmeter  \
				-I$(DRV_DIR)/Triac      \
				-I$(DRV_DIR)/Debug      \
				-I$(DRV_DIR)/Bluetooth  \
				-I$(DRV_DIR)/Buzzer	    \
				-I$(DRV_DIR)/RTCC	    \
				-I$(DRV_DIR)/EEPROM     \
				-I$(DRV_DIR)/Controller \
				-I$(DRV_DIR)/Touch      \
				-I$(DRV_DIR)/Voltmeter  \
				-I$(DRV_DIR)/NTC        \
				-I$(DRV_DIR)/Screen		\
				-I$(DRV_DIR)/ShowerController

#--------------------------------------------
# Linker Flags			
LDFLAGS=-static -mcpu=cortex-m0 -mthumb -mthumb-interwork -Wl,--start-group
LDFLAGS+=-T./$(STDPERIPH_DIR)/$(LD_FILE).ld -nostartfiles

#--------------------------------------------
# Source Code Files
SRC=main.c		

#--------------------------------------------
# Object Files (Made from Source code list above)
OBJS=$(SRC:.c=.o)

#--------------------------------------------
# Drivers Object Files
DRV_OBJS=$(DRV_DIR)/User/User.o				\
		$(DRV_DIR)/PWM/PWM.o				\
        $(DRV_DIR)/LED/LED.o				\
		$(DRV_DIR)/Display/Display.o		\
		$(DRV_DIR)/IrDA/IrDA.o				\
		$(DRV_DIR)/AD/AD.o					\
		$(DRV_DIR)/Flowmeter/Flowmeter.o	\
		$(DRV_DIR)/Triac/Triac.o			\
		$(DRV_DIR)/Bluetooth/Bluetooth.o	\
		$(DRV_DIR)/Debug/Debug.o			\
		$(DRV_DIR)/Buzzer/Buzzer.o			\
		$(DRV_DIR)/RTCC/RTCC.o				\
		$(DRV_DIR)/EEPROM/EEPROM.o			\
		$(DRV_DIR)/Controller/Controller.o	\
		$(DRV_DIR)/Touch/Touch.o			\
		$(DRV_DIR)/Voltmeter/Voltmeter.o	\
		$(DRV_DIR)/NTC/NTC.o				\
		$(DRV_DIR)/Screen/Screen.o			\
		$(DRV_DIR)/ShowerController/ShowerController.o

#===================================================================================================
# Build All
#===================================================================================================
all: proj
	$(CP) -O binary $(OUTPUT).elf $(OUTPUT).bin
	$(OD) $(OUTPUT).elf > $(OUTPUT).lst
	$(SIZE) -B $(OUTPUT).elf

#===================================================================================================
# Build Drivers
#===================================================================================================
drivers:
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/User
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/PWM
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/LED
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/Display
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/IrDA
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/AD
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/Flowmeter
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/Triac
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/Buzzer
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/Bluetooth
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/Debug
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/RTCC
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/EEPROM
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/Controller
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/Voltmeter
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/NTC
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/ShowerController
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/Touch
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) SHOWER=$(SHOWER) MAJOR=$(MAJOR) MINOR=$(MINOR) BUILD=$(BUILD) -C $(DRV_DIR)/Screen
		
#===================================================================================================
# Build Libs (STM32F0xx_StdPeriph_Lib and FreeRTOS)
#===================================================================================================
lib:
	$(MAKE) OUTPUT=$(LIBPRINTF_OUTPUT) -C $(LIBPRINTF_DIR)
	$(MAKE) OUTPUT=$(STDPERIPH_OUTPUT) DEVICE=$(DEVICE) STARTUP=$(STARTUP) -C $(STDPERIPH_DIR)
	$(MAKE) OUTPUT=$(FREERTOS_OUTPUT) STDPERIPH_DIR=$(STDPERIPH_DIR) -C $(FREERTOS_DIR)
	
#===================================================================================================
# Build Project
#===================================================================================================
proj: cc lib drivers
	$(CC) $(LDFLAGS) -L $(STDPERIPH_DIR) -l$(STDPERIPH_LIB) -L $(FREERTOS_DIR)  -l$(FREERTOS_LIB) -L $(LIBPRINTF_DIR) -l$(LIBPRINTF_LIB) -o $(OUTPUT).elf $(OBJS) $(DRV_OBJS)

#===================================================================================================
# Build Source Code 
#===================================================================================================
cc: $(SRC)
	$(CC) $(CFLAGS) $(INCLUDES) $(DVR_INCLUDES) $(SRC)
	
#===================================================================================================
# burn 
#===================================================================================================
burn: $(OUTPUT).bin	
	ST-LINK_CLI.exe -Q -P $(OUTPUT).bin 0x8000000 -V "after_programming" -Rst

#===================================================================================================
# Clean 
#===================================================================================================
clean:
	rm -f 	$(OBJS) \
		$(OUTPUT).map \
		$(OUTPUT).bin \
		$(OUTPUT).lst \
		$(OUTPUT).elf
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/User      	clean
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/PWM			clean
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/LED			clean
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/Display		clean
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/IrDA 		clean
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/AD 			clean
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/Flowmeter	clean
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/Triac 		clean
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/Buzzer 		clean
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/Bluetooth 	clean
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/Debug 		clean
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/RTCC 		clean
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/EEPROM 		clean
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/Controller	clean
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/Voltmeter    clean
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/NTC      	clean
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/Touch      	clean
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/Screen      	clean
	$(MAKE) DEVICE=$(DEVICE) STDPERIPH_DIR=$(STDPERIPH_DIR) FREERTOS_DIR=$(FREERTOS_DIR) -C $(DRV_DIR)/ShowerController	clean

#===================================================================================================
# Clean All (Libraries Too)
#===================================================================================================
cleanall: clean
	$(MAKE) OUTPUT=$(LIBPRINTF_OUTPUT) -C $(LIBPRINTF_DIR) clean
	$(MAKE) OUTPUT=$(STDPERIPH_OUTPUT) DEVICE=$(DEVICE) STARTUP=$(STARTUP) -C $(STDPERIPH_DIR) clean
	$(MAKE) OUTPUT=$(FREERTOS_OUTPUT) STDPERIPH_DIR=$(STDPERIPH_DIR) -C $(FREERTOS_DIR) clean
