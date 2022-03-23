#===================================================================================================
# FreeRTOS 8.1.2 Makefile for STM32F0xx - Juliano Günthner
#===================================================================================================

#--------------------------------------------
# Output File Name
ifndef OUTPUT
$(warning OUTPUT not defined - Assuming "libfreertos")
OUTPUT = libfreertos
endif

#--------------------------------------------
# Output File Name
ifndef STDPERIPH_DIR
$(warning STDPERIPH_DIR not defined - Assuming "./StdPeriph_Lib")
STDPERIPH_DIR = ./StdPeriph_Lib
endif

#--------------------------------------------
# Compiler
TOOLS_PREFIX=arm-none-eabi-
CC=$(TOOLS_PREFIX)gcc
AS=$(TOOLS_PREFIX)as
AR=$(TOOLS_PREFIX)ar

#--------------------------------------------
# Compiler Flags
CFLAGS = -c -fno-common -O2 -Wall -g -mcpu=cortex-m0 -mthumb

#--------------------------------------------
# Includes
INCLUDES =	-I. 				\
			-I./inc				\
			-I.$(STDPERIPH_DIR)
			
#--------------------------------------------
# Source Code Files
SRCS=	src/croutine.c         \
		src/event_groups.c     \
		src/heap_1.c           \
		src/list.c	           \
		src/port.c	           \
		src/queue.c	           \
		src/tasks.c	           \
		src/timers.c

#--------------------------------------------
# Object Files (Made from Source code list above)
OBJS=$(SRCS:.c=.o)

#===================================================================================================
# Build All
#===================================================================================================
all: $(OUTPUT).a

#===================================================================================================
# Build object files from source code
#===================================================================================================
%.o:%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $^ -o $@

#===================================================================================================
# Build FreeRTOS static library
#===================================================================================================
$(OUTPUT).a: $(OBJS)
	$(AR) -r $@ $(OBJS) 

#===================================================================================================
# Clean 
#===================================================================================================
clean:
	rm -f 	$(OBJS) $(OUTPUT).a




