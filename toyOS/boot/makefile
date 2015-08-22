#默认编译器设置
ASM		= nasm		#汇编编译器
DASM	= ndisasm		#反汇编编译器
CC		= gcc			#C语言编译器
LD		= ld			#链接器


#mbr.asm将被编译成bin格式
 boots:mbr.asm config.inc boot.asm pm.inc setup.asm
	$(ASM) -f bin mbr.asm
	$(ASM) -f bin boot.asm
	$(ASM) -f bin setup.asm
	
clean:
	rm -f mbr
	rm -f boot
	rm -f setup

copy:boot setup
	dd if=boot of=../bochs/neur.img bs=512 count=1 conv=notrunc
	sudo mount -o loop ../bochs/neur.img /media/neur
	sudo cp setup /media/neur -v 
	sleep 0.01
	sudo umount /media/neur

