# author  : KeeneChen
# date    : 2022.11.03-19:14:28
# details : NXP_SDK C语言点灯

CROSS_COMPILE_PREFIX ?= arm-linux-gnueabihf-
TARGET ?= ledc

CC := $(CROSS_COMPILE_PREFIX)gcc
LD := $(CROSS_COMPILE_PREFIX)ld
OBJCOPY := $(CROSS_COMPILE_PREFIX)objcopy
OBJDUMP := $(CROSS_COMPILE_PREFIX)objdump
OBJS := start.o led.o
INCLUDE := include

$(TARGET).bin : $(OBJS)
	$(LD) -T imx6ull.lds -o $(TARGET).elf $^
	$(OBJCOPY) -O binary -S $(TARGET).elf $@
	$(OBJDUMP) -D -m arm $(TARGET).elf > $(TARGET).dis

%.o:%.c
	$(CC) -Wall -nostdlib -I $(INCLUDE) -c -O2 -o $@ $<
%.o:%.s
	$(CC) -Wall -nostdlib -I $(INCLUDE) -c -O2 -o $@ $<
%.o:%.S
	$(CC) -Wall -nostdlib -I $(INCLUDE) -c -O2 -o $@ $<

	
clean:
	rm -rf *.o $(TARGET).bin $(TARGET).elf $(TARGET).dis
	
	
