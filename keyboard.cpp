#include "keyboard.h"

KeyboardDriver::KeyboardDriver(InterruptManager* manager)
    : InterruptHandler(manager, 0x21),
      dataport(0x60),
      commandport(0x64)
{
    while (commandport.Read() & 0x1)
        dataport.Read();

    commandport.Write(0xae); // activate interrupts
    commandport.Write(0x20); // command 0x20 = read controller command byte

    uint8_t status = (dataport.Read() | 1) & ~0x10;

    commandport.Write(0x60); // command 60 = set controller command byte
    dataport.Write(status);
    dataport.Write(0xf4);
}

KeyboardDriver::~KeyboardDriver()
{ }

void printf(char*);

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t key = dataport.Read();
    printf("1234");

    if (key < 0x80) {
        switch (key) {
        case 0x02: printf("1"); break;
        case 0x03: printf("2"); break;
        case 0x04: printf("3"); break;
        case 0x05: printf("4"); break;
        case 0x06: printf("5"); break;
        case 0x07: printf("6"); break;
        case 0x08: printf("7"); break;
        case 0x09: printf("8"); break;
        case 0x0A: printf("9"); break;
        case 0x0B: printf("0"); break;

        case 0x10: printf("q"); break;
        case 0x11: printf("w"); break;
        case 0x12: printf("e"); break;
        case 0x13: printf("r"); break;
        case 0x14: printf("t"); break;
        case 0x15: printf("z"); break;
        case 0x16: printf("u"); break;
        case 0x17: printf("i"); break;
        case 0x18: printf("o"); break;
        case 0x19: printf("p"); break;

        default: {
            char* foo = "Keyboard 0x00";
            char* hex = "0123456789ABCDEF";
            foo[11] = hex[(key >> 4) & 0xF];
            foo[12] = hex[key & 0xF];
            printf(foo);
            break;
        }
        }
    }
    return esp;
}