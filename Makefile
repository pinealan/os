GPPPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = obj/loader.o \
          obj/gdt.o \
		  obj/multitasking.o \
          obj/hardware/port.o \
          obj/hardware/interrupts.o \
          obj/hardware/interruptstubs.o \
		  obj/hardware/pci.o \
          obj/drivers/driver.o \
          obj/drivers/keyboard.o \
          obj/drivers/mouse.o \
          obj/kernel.o

all: mykernel.iso

clean.o:
	rm -rf obj

clean.bin:
	rm mykernel.iso mykernel.bin

obj/%.o: %.cpp
	mkdir -p $(@D)
	g++ $(GPPPARAMS) -o $@ -c $<

obj/%.o: %.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $<

%.s: %.cpp
	g++ $(GPPPARAMS) -o $@ -S $<

mykernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

mykernel.iso: mykernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout=0'                     >> iso/boot/grub/grub.cfg
	echo 'set default=0'                     >> iso/boot/grub/grub.cfg
	echo ''                                  >> iso/boot/grub/grub.cfg
	echo 'menuentry "My Operating System" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/mykernel.bin'    >> iso/boot/grub/grub.cfg
	echo '  boot'                            >> iso/boot/grub/grub.cfg
	echo '}'                                 >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	rm -rf iso

install: mykernel.bin
	sudo cp $< /boot/mykernel.bin