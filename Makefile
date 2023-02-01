##########################################################################################################################
# File automatically-generated by tool: [projectgenerator] version: [3.18.0-B7] date: [Mon Jan 30 12:30:15 ICT 2023] 
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------

######################################
# Include dev_code
######################################
include dev_code/dev_code.mk

######################################
# target
######################################
TARGET = blood_pressure_monitor_v2


######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -O2


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
STM32L496/Core/Src/main.c \
STM32L496/Core/Src/freertos.c \
STM32L496/Core/Src/stm32l4xx_it.c \
STM32L496/Core/Src/stm32l4xx_hal_msp.c \
STM32L496/Core/Src/stm32l4xx_hal_timebase_tim.c \
STM32L496/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.c \
STM32L496/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c.c \
STM32L496/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c_ex.c \
STM32L496/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.c \
STM32L496/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.c \
STM32L496/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.c \
STM32L496/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.c \
STM32L496/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.c \
STM32L496/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c \
STM32L496/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.c \
STM32L496/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma_ex.c \
STM32L496/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.c \
STM32L496/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.c \
STM32L496/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.c \
STM32L496/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_exti.c \
STM32L496/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim.c \
STM32L496/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim_ex.c \
STM32L496/Core/Src/system_stm32l4xx.c \
STM32L496/Middlewares/Third_Party/FreeRTOS/Source/croutine.c \
STM32L496/Middlewares/Third_Party/FreeRTOS/Source/event_groups.c \
STM32L496/Middlewares/Third_Party/FreeRTOS/Source/list.c \
STM32L496/Middlewares/Third_Party/FreeRTOS/Source/queue.c \
STM32L496/Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c \
STM32L496/Middlewares/Third_Party/FreeRTOS/Source/tasks.c \
STM32L496/Middlewares/Third_Party/FreeRTOS/Source/timers.c \
STM32L496/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c \
STM32L496/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c \
STM32L496/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c \
STM32L496/Core/Src/usart.c \
STM32L496/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart.c \
STM32L496/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart_ex.c \
$(DEV_CODE_SOURCES)

# ASM sources
ASM_SOURCES =  \
STM32L496/startup_stm32l496xx.s


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32L496xx


# AS includes
AS_INCLUDES =  \
-ICore\Inc

# C includes
C_INCLUDES =  \
-ISTM32L496/Core/Inc \
-ISTM32L496/Drivers/STM32L4xx_HAL_Driver/Inc \
-ISTM32L496/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy \
-ISTM32L496/Middlewares/Third_Party/FreeRTOS/Source/include \
-ISTM32L496/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 \
-ISTM32L496/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F \
-ISTM32L496/Drivers/CMSIS/Device/ST/STM32L4xx/Include \
-ISTM32L496/Drivers/CMSIS/Include \
$(DEV_CODE_INCLUDES)


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -Wfatal-errors -fdata-sections -ffunction-sections

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -Wfatal-errors -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32L496/STM32L496AGIx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	@echo Compiling $< 
	@$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	@echo Compiling $< 
	@$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	@echo Linking $@
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@echo Creating $@
	@$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@echo Creating $@
	@$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	@-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

#######################################
# flash firmware to MCU
#######################################
stm32cube_programmer_path := STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin
stm32cube_programmer_filename := STM32_Programmer_CLI.exe
stm32cube_programmer_filepath := $(stm32cube_programmer_path)/$(stm32cube_programmer_filename)

program_files_win32 = /c/Program Files (x86)
program_files_win64 = /c/Program Files

program_files_path := $(program_files_win64)

# fall back to 32bit program files if 64bit is not found
ifeq ("$(shell find "$(program_files_win64)/$(stm32cube_programmer_filepath)")","")
program_files_path := $(program_files_win32)
endif

st_stm32cube_programmer_path := $(program_files_path)/$(stm32cube_programmer_path)
st_stm32cube_programmer_filepath := $(program_files_path)/$(stm32cube_programmer_filepath)

flash: all
	"$(st_stm32cube_programmer_path)/$(stm32cube_programmer_filename)" -c port=SWD -d build/$(TARGET).elf -hardRst

# *** EOF ***
