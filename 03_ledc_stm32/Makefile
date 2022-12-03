# author  : KeeneChen
# date    : 2022.11.03-17:03:23
# details : I.MX6ULL C语言点灯

objs := start.o led.o

ledc.bin:$(objs)
    # -T     指定链接脚本
    # -Ttext 使用指定的地址作为代码段的起点
    # -Tdata 使用指定的地址作为数据段的起点
    # -Tbss  使用指定的地址作为未初始化的数据段的起点
	arm-linux-gnueabihf-ld -T imx6ull.lds -o ledc.elf $^
	arm-linux-gnueabihf-objcopy -O binary -S ledc.elf $@
	arm-linux-gnueabihf-objdump -D -m arm ledc.elf > ledc.dis
	
%.o:%.s
    # -Wall 表示显示编译时所有警告
    # -nostdlib 表示不链接系统标准启动文件和库文件
    # -O2 表示编译器优化等级
	arm-linux-gnueabihf-gcc -Wall -nostdlib -c -O2 -o $@ $<
	
%.o:%.S
	arm-linux-gnueabihf-gcc -Wall -nostdlib -c -O2 -o $@ $<
	
%.o:%.c
	arm-linux-gnueabihf-gcc -Wall -nostdlib -c -O2 -o $@ $<
	
clean:
	rm -rf *.o ledc.bin ledc.elf ledc.dis
	
	
