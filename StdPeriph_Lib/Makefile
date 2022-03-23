#===================================================================================================
# STM32F0xx_StdPeriph_Lib 1.4.0 Makefile - Juliano Günthner
#===================================================================================================

#--------------------------------------------
# Output File Name
ifndef OUTPUT
$(warning OUTPUT not defined - Assuming "libstm32f0")
OUTPUT = libstm32f0
endif

#--------------------------------------------
# Device
ifndef DEVICE
$(warning DEVICE not defined - Assuming "STM32F051")
DEVICE=STM32F051
endif

#--------------------------------------------
# STM32F0xx Devices vector table (.s file)
ifndef STARTUP
$(warning STARTUP not defined - Assuming "startup_stm32f0xx")
STARTUP=startup_stm32f0xx
endif

#--------------------------------------------
# Compiler
TOOLS_PREFIX=arm-none-eabi-
CC=$(TOOLS_PREFIX)gcc
AS=$(TOOLS_PREFIX)as
AR=$(TOOLS_PREFIX)ar

#--------------------------------------------
# Compiler Flags
CFLAGS = -DUSE_STDPERIPH_DRIVER -D$(DEVICE) -c -fno-common -O2 -Wall -g -mcpu=cortex-m0 -mthumb

#--------------------------------------------
# Linker Flags
ASFLAGS=-mcpu=cortex-m0 -I./cmsis -I. -gdwarf-2 -gdwarf-2

#--------------------------------------------
# Includes
INCLUDES=	-I. 		\
			-I./inc 	\
			-I./cmsis

#--------------------------------------------
# Source Code Files
SRCS=	src/stm32f0xx_adc.c	      \
		src/stm32f0xx_can.c	      \
		src/stm32f0xx_cec.c       \
		src/stm32f0xx_comp.c	  \
		src/stm32f0xx_crc.c	      \
		src/stm32f0xx_crs.c	      \
		src/stm32f0xx_dac.c	      \
		src/stm32f0xx_dbgmcu.c    \
		src/stm32f0xx_dma.c	      \
		src/stm32f0xx_exti.c	  \
		src/stm32f0xx_flash.c	  \
		src/stm32f0xx_gpio.c      \
		src/stm32f0xx_i2c.c	      \
		src/stm32f0xx_iwdg.c	  \
		src/stm32f0xx_misc.c	  \
		src/stm32f0xx_pwr.c       \
		src/stm32f0xx_rcc.c	      \
		src/stm32f0xx_rtc.c	      \
		src/stm32f0xx_spi.c	      \
		src/stm32f0xx_syscfg.c    \
		src/stm32f0xx_tim.c	      \
		src/stm32f0xx_usart.c	  \
		src/stm32f0xx_wwdg.c	  \
		system_stm32f0xx.c

#--------------------------------------------
# Object Files (Made from Source code list above)
OBJS=$(SRCS:.c=.o)

#===================================================================================================
# Build All
#===================================================================================================
all: $(OUTPUT).a

#===================================================================================================
# Build Startup
#===================================================================================================
startup:
	$(CC) $(CFLAGS) $(INCLUDES) -c ./$(STARTUP).s -o $(STARTUP).o

#===================================================================================================
# Build object files from source code
#===================================================================================================	
%.o:%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $^ -o $@	
	
#===================================================================================================
# Build StdPeriph static library
#===================================================================================================
$(OUTPUT).a: $(OBJS) startup
	$(AR) -r $@ $(STARTUP).o $(OBJS) 

#===================================================================================================
# Clean 
#===================================================================================================
clean:
	rm -f 	$(OBJS) $(OUTPUT).a $(STARTUP).o




