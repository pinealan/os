#include "types.h"
#include "gdt.h"
#include "port.h"
#include "interrupts.h"
#include "keyboard.h"
#include "mouse.h"

void putc(char c) {
	static uint16_t* VideoMemory = (uint16_t*) 0xb8000;

	static uint8_t x = 0;
	static uint8_t y = 0;

	switch (c) {
	case '\n':
		x = 0;
		y++;
		break;
	case '\r':
		x = 0;
		break;
	case '\t':
		x += 4;
		break;
	default:
		VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | c;
		x++;
	}

	if (x >= 80) {
		x = 0;
		y++;
	}

	if (y >= 25) {

		// Scroll screen by one line
		for (x = 0; x < 80; x++) {
			for (y = 0; y < 24; y++) {
				VideoMemory[80 * y + x] = VideoMemory[80 * (y + 1) + x];
			}
		}

		// Clear bottom line
		for (x = 0; x < 80; x++)
			VideoMemory[80 * 24 + x] = (VideoMemory[80 * 24 + x] & 0xFF00) | ' ';
		x = 0;
		y = 24;
	}
}

void printf(char* str) {
	for (int i = 0; str[i] != '\0'; ++i)
		putc(str[i]);
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors() {
	for(constructor* i = &start_ctors; i != &end_ctors; i++)
		(*i)();
}

extern "C" void kernelMain(void* mutliboot_structure, uint32_t magicnumber) {
	printf("Hello world!\n");
	printf("Lorem ipsum\n");
	printf("My first operating system!!\n");

	GlobalDescriptorTable gdt;
	InterruptManager interrupts(0x20, &gdt);
	KeyboardDriver keyboard(&interrupts);

	interrupts.Activate();

	while(1);
}