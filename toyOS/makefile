#默认编译器设置
ASM		= nasm
DASM	= ndisasm
CC		= gcc
LD		= ld

CFLAGS	= -I include/ -fpack-struct -fno-builtin\
		  -fno-stack-protector -ansi -c

LDFLAGS	= -Ttext 0 -e main -N --oformat binary

OBJS	= kernel/main.o kernel/pm.o lib/disp.o kernel/io.s.o \
		  kernel/traps.o kernel/interrupt.s.o kernel/i8259a.o \
		  kernel/sched.o lib/string.o kernel/system_call.s.o \
		  kernel/sys.o kernel/keybord.o kernel/tty.o lib/stdio.o 

SYSTEM	= out/system 

default:$(SYSTEM)
	make copy
	#make run

kernel/main.o:kernel/main.c include/disp.h include/kernel/pm.h \
			  include/kernel/io.h include/kernel/i8259a.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/pm.o:kernel/pm.c include/kernel/config.h include/kernel/pm.h \
			include/kernel/traps.h
	$(CC) $(CFLAGS) -o $@ $<
	
kernel/traps.o:kernel/traps.c include/kernel/traps.h \
			   include/kernel/pm.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/i8259a.o:kernel/i8259a.c include/kernel/i8259a.h
	$(CC) $(CFLAGS) -o $@ $<
	
kernel/sched.o:kernel/sched.c
	$(CC) $(CFLAGS) -o $@ $<
	
kernel/sys.o:kernel/sys.c include/kernel/sys.h
	$(CC) $(CFLAGS) -o $@ $<
	
kernel/keybord.o:kernel/keybord.c include/kernel/keybord.h
	$(CC) $(CFLAGS) -o $@ $<
	
kernel/tty.o:kernel/tty.c
	$(CC) $(CFLAGS) -o $@ $<
		
kernel/system_call.s.o:kernel/system_call.asm
	$(ASM) -f elf $< -o $@

kernel/io.s.o:kernel/io.asm
	$(ASM) -f elf $< -o $@
	
kernel/interrupt.s.o:kernel/interrupt.asm
	$(ASM) -f elf $< -o $@
	
lib/disp.o:lib/disp.c include/disp.h
	$(CC) $(CFLAGS) -o $@ $<

lib/string.o:lib/string.c include/string.h
	$(CC) $(CFLAGS) -o $@ $<

lib/stdio.o:lib/stdio.c include/stdio.h include/stdarg.h
	$(CC) $(CFLAGS) -o $@ $<
	
$(SYSTEM):$(OBJS)
	$(LD) -o $(SYSTEM) $(LDFLAGS) $(OBJS)


clean:
	find -name "*.o" | xargs rm -f
	rm -f out/system
	
run:
	bochs -qf bochs/bochsrc
	
copy:
	sudo mount -o loop bochs/neur.img /media/neur
	sudo cp out/system /media/neur -v
	sleep 0.1
	sudo umount -v /media/neur 

