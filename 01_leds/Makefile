# author  : KeeneChen
# date    : 2022.11.02-12:24:52
# details : I.MX6ULL 汇编点灯
# warning : 最终烧录的imxdownload生成的load.imx文件并非bin文件

led.bin:led.s
    # 生成目标文件
	arm-linux-gnueabihf-gcc -g -c led.s -o led.o

    # 链接文件 -Ttext 链接地址
	arm-linux-gnueabihf-ld -Ttext 0X87800000 led.o -o led.elf

    # 将elf可执行文件转换为bin文件
	arm-linux-gnueabihf-objcopy -O binary -S -g led.elf led.bin

    # 将elf可执行文件转换成汇编文件
	arm-linux-gnueabihf-objdump -D led.elf > led.dis

clean:
	rm -rf *.o led.bin led.elf led.dis