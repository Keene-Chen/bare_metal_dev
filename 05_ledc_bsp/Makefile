# author  : KeeneChen
# date    : 2022.11.04-16:17:25
# details : 项目模块化封装,重构Makefile,基于NXP_SDK的C语言闪烁LED

# >>> 配置编译器选项 >>>
TARGET ?= ledc
CROSS_COMPILE_PREFIX ?= arm-linux-gnueabihf-
CC      := $(CROSS_COMPILE_PREFIX)gcc
LD      := $(CROSS_COMPILE_PREFIX)ld
OBJCOPY := $(CROSS_COMPILE_PREFIX)objcopy
OBJDUMP := $(CROSS_COMPILE_PREFIX)objdump
# <<< 配置编译器选项 <<<

# >>> 搜索替换头文件和源文件 >>>
# 指定头文件和源文件路径
INCDIRS   := imx6ull   \
             bsp/clk   \
             bsp/delay \
             bsp/led
SRCDIRS   := src       \
             bsp/clk   \
             bsp/delay \
             bsp/led

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
	$(LD) -Tsrc/imx6ull.lds -o $(TARGET).elf $^
	$(OBJCOPY) -O binary -S $(TARGET).elf $@
	$(OBJDUMP) -D -m arm $(TARGET).elf > $(TARGET).dis

$(SOBJS) : obj/%.o : %.s
	$(CC) -Wall -nostdlib -c -O2 $(INCLUDE) -o $@ $<

$(COBJS) : obj/%.o : %.c
	$(CC) -Wall -nostdlib -c -O2 $(INCLUDE) -o $@ $<
# <<< 静态编译 <<<

# >>> 伪目标配置 >>>
.PHONY:

# show 伪目标用于查看变量
show:
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
	rm -rf $(TARGET).elf $(TARGET).dis $(TARGET).bin $(OBJS)
# <<< 伪目标配置 <<<