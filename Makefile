# GNU ARM Embedded Toolchain
CC=arm-none-eabi-gcc
CXX=arm-none-eabi-g++
#LD=arm-none-eabi-ld
LD = arm-none-eabi-g++
AR=arm-none-eabi-ar
AS=arm-none-eabi-as
CP=arm-none-eabi-objcopy
OD=arm-none-eabi-objdump
 
# Build Parameters: MCU Flags, Definitions, Includes,
#                   Compile Flags, Linker Script, Linker Flags
ADD_LIBS=--specs=rdimon.specs -Wl,--start-group -lgcc -lc -lm
MCFLAGS=-mcpu=cortex-m4 -mthumb -mlittle-endian -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb-interwork -fdata-sections -ffunction-sections -Wall -std=gnu11 -O0 $(ADD_LIBS)
DEFS=-DUSE_STDPERIPH_DRIVER -DSTM32F4XX -DSTM32F407VG

INCLUDES= -I. -I./CMSIS/Include -I./CMSIS/Device/ST/STM32F4xx/Include -I./STM32F4xx_SPL/inc \
-I./USB/inc \
-I./FAT_FS \
-I./LCD \
-I./MY_LIB \
-I./Audio \
-I./FILE_MAN \
-I./Dispatcher \
-I./IMAGE_LIB \
-I./USART

SOURCES_DIR = STM32F4xx_SPL/src USB/src FAT_FS LCD MY_LIB Audio FILE_MAN Dispatcher IMAGE_LIB USART

CFLAGS=-c -g $(MCFLAGS) $(DEFS) $(INCLUDES)
LDSCRIPT = STM32F407VG_FLASH.ld
LDFLAGS=-T $(LDSCRIPT) $(MCFLAGS) -Wl,--gc-sections
 
# Inputs: C Sources, Assembler Sources
# syscalls.c
SOURCES = stm32f4xx_it.c system_stm32f4xx.c delay.c \
misc.c \
stm32f4xx_gpio.c \
stm32f4xx_rcc.c \
stm32f4xx_exti.c \
stm32f4xx_syscfg.c \
stm32f4xx_tim.c \
stm32f4xx_fsmc.c \
stm32f4xx_usart.c \
stm32_ub_fatfs.c \
stm32_ub_usbdisk.c \
stm32_ub_usb_msc_host.c \
usb_bsp.c \
usb_core.c \
usb_hcd.c \
usb_hcd_int.c \
usbh_core.c \
usbh_hcs.c \
usbh_ioreq.c \
usbh_msc_bot.c \
usbh_msc_core.c \
usbh_msc_scsi.c \
usbh_stdreq.c \
usbh_usr.c \
diskio.c \
ff.c \
ccsbcs.c \
AsciiLib.c \
my_str_lib.c \
SSD1289.c \
TouchLib.c \
GesturesHandler.c \
stm32f4xx_dac.c \
stm32f4xx_dma.c \
audio_dac.c \
stm32f4xx_spi.c \
file_man.c \
dispatcher.c \
ListView.c \
icons_menu.c \
gdisp_image_bmp.c \
timer.c \
AudioPlayer.c \
usart_init.c \
gif.c

CXXSOURCES = main.cpp

ASMSOURCES=startup_stm32f407xx.s
ALLSRC = $(notdir $(SOURCES))
 
# Outputs: Object Files, ELF Executable &
#          Converted ELF Executable to Intel HEX format
OBJECTS=$(ALLSRC:%.c=OBJ/%.o)
OBJECTS+=$(CXXSOURCES:%.cpp=OBJ/%.o)
OBJECTS+=$(ASMSOURCES:%.s=OBJ/%.o)

EXECUTABLE=OUT/myproject.elf
TARGET=OUT/myproject.hex
BIN=OUT/myproject.bin

VPATH = $(SOURCES_DIR) 

# Build Rules
.PHONY: release
release: $(TARGET)

.PHONY: all
all: $(TARGET)

.PHONY: debug
debug: CFLAGS+=-g # Add debug flag
debug: $(TARGET) 

$(TARGET): $(EXECUTABLE)
	@echo 'Generate target: $@'
	$(CP) -O ihex $< $@
	@echo "Objcopy from ELF to IHEX complete!\n"
	$(MAKE) --no-print-directory post-build

$(EXECUTABLE): $(OBJECTS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@
	@echo "Linking complete!\n"

OBJ/%.o: %.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	$(CC) $(CFLAGS) $< -o $@
	@echo "Finished building: "$<"!\n"

OBJ/%.o: %.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	$(CXX) $(CFLAGS) $< -o $@
	@echo "Finished building: "$<"!\n"

OBJ/%.o: %.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU Assembler'
	$(CC) $(CFLAGS) $< -o $@
	@echo "Finished building: "$<"!\n"

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXECUTABLE) $(TARGET) $(BIN)
	
.SECONDARY: post-build
post-build:
	arm-none-eabi-objcopy -O binary $(EXECUTABLE) $(BIN)
	@echo 'Performing post-build steps'
	arm-none-eabi-size --totals $(EXECUTABLE)
	@echo ' '
	
.PHONY: load
load:
	st-flash write $(BIN) 0x8000000
#arm-none-eabi-gdb -x gdbcommands
