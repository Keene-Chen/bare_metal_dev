# author  : KeeneChen
# date    : 2022.11.27-12:05:20
# details : SPI驱动

# >>> 配置编译器选项 >>>
TARGET  ?= spi
CROSS_COMPILE_PREFIX ?= arm-linux-gnueabihf-
CC      := $(CROSS_COMPILE_PREFIX)gcc
LD      := $(CROSS_COMPILE_PREFIX)ld
OBJCOPY := $(CROSS_COMPILE_PREFIX)objcopy
OBJDUMP := $(CROSS_COMPILE_PREFIX)objdump
CFLAGS  := -Wall -nostdlib -std=gnu11
CFLAGS  += -fno-builtin -Wa,-mimplicit-it=thumb -march=armv7-a -mfpu=neon-vfpv4 -mfloat-abi=hard
LIBPATH	:= -lgcc -L ~/IMX6ULL/toolchain/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/lib/gcc/arm-linux-gnueabihf/4.9.4
# <<< 配置编译器选项 <<<

# >>> 搜索替换头文件和源文件 >>>
# 二进制文件生成目录
BINDIR    := bin

# 指定头文件和源文件路径
INCDIRS   := imx6ull        \
             stdio/include  \
             bsp            \
             bsp/clk        \
             bsp/delay      \
             bsp/beep       \
             bsp/led        \
             bsp/gpio       \
             bsp/int        \
             bsp/exti       \
             bsp/epit       \
             bsp/key        \
             bsp/key_filter \
             bsp/uart       \
             bsp/lcd        \
             bsp/rtc        \
             bsp/i2c        \
             bsp/ap3216c    \
             bsp/spi        \
             bsp/icm20608
SRCDIRS   := src            \
             stdio/lib      \
             bsp/clk        \
             bsp/beep       \
             bsp/delay      \
             bsp/led        \
             bsp/gpio       \
             bsp/int        \
             bsp/exti       \
             bsp/epit       \
             bsp/key        \
             bsp/key_filter \
             bsp/uart       \
             bsp/lcd        \
             bsp/rtc        \
             bsp/i2c        \
             bsp/ap3216c    \
             bsp/spi        \
             bsp/icm20608

# 在编译中加入 -I参数用于搜索头文件
INCLUDE   := $(patsubst %,-I %, $(INCDIRS))

# 循环遍历源文件名称
SFILES    := $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.s))
CFILES    := $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))

# 获取源文件名称不带路径
SFILEBASE := $(notdir $(SFILES))
CFILEBASE := $(notdir $(CFILES))

# 替换.s和.c文件为.o
SOBJS     := $(patsubst %, obj/%, $(SFILEBASE:.s=.o))
COBJS     := $(patsubst %, obj/%, $(CFILEBASE:.c=.o))
OBJS      := $(SOBJS) $(COBJS)

# 指定源文件搜索路径
VPATH     := $(SRCDIRS)
# <<< 搜索替换头文件和源文件 <<<

# >>> 静态编译 >>>
$(TARGET).bin : $(OBJS)
	$(LD) -T src/imx6ull.lds -o $(TARGET).elf $^ $(LIBPATH)
	$(OBJCOPY) -O binary -S $(TARGET).elf $@
	$(OBJDUMP) -D -m arm $(TARGET).elf > $(TARGET).dis
	mv $(TARGET).elf $(TARGET).dis $(TARGET).bin bin

$(SOBJS) : obj/%.o : %.s
	$(CC) $(CFLAGS) -c -O2 $(INCLUDE) -o $@ $<

$(COBJS) : obj/%.o : %.c
	$(CC) $(CFLAGS) -c -O2 $(INCLUDE) -o $@ $<
# <<< 静态编译 <<<

# >>> 伪目标配置 >>>
.PHONY:

# flash 烧写bin文件到tf卡
flash:
	$(BINDIR)/imxdownload $(BINDIR)/$(TARGET).bin /dev/sdb

# show 伪目标用于查看变量
show:
	@echo BINDIR     = $(BINDIR)
	@echo INCLUDE    = $(INCLUDE)
	@echo SFILES     = $(SFILES)
	@echo CFILES     = $(CFILES)
	@echo SFILEBASE  = $(SFILEBASE)
	@echo CFILEBASE  = $(CFILEBASE)
	@echo SOBJS      = $(SOBJS)
	@echo COBJS      = $(COBJS)
	@echo OBJS       = $(OBJS)

# clean 伪目标用于清除生成的目标文件
clean:
	rm -rf $(BINDIR)/$(TARGET).elf $(BINDIR)/$(TARGET).dis $(BINDIR)/$(TARGET).bin $(OBJS)
# <<< 伪目标配置 <<<