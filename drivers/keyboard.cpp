#include <drivers/keyboard.h>

void printk(char*);
void printh(uint8_t);

KeyboardDriver::KeyboardDriver(InterruptManager* manager)
    : InterruptHandler(manager, 0x21),
      dataport(0x60),
      commandport(0x64)
{ }

KeyboardDriver::~KeyboardDriver()
{ }

void KeyboardDriver::Activate() {
    while (commandport.Read() & 0x1)
        dataport.Read();

    commandport.Write(0xae); // activate interrupts
    commandport.Write(0x20); // command 0x20 = read controller command byte

    uint8_t status = (dataport.Read() | 1) & ~0x10;
    commandport.Write(0x60); // command 60 = set controller command byte
    dataport.Write(status);

    dataport.Write(0xf4);
}

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t key = dataport.Read();

    if (key < 0x80) {
        switch (key) {
        case 0x29: printk("`"); break;
        case 0x02: printk("1"); break;
        case 0x03: printk("2"); break;
        case 0x04: printk("3"); break;
        case 0x05: printk("4"); break;
        case 0x06: printk("5"); break;
        case 0x07: printk("6"); break;
        case 0x08: printk("7"); break;
        case 0x09: printk("8"); break;
        case 0x0A: printk("9"); break;
        case 0x0B: printk("0"); break;
        case 0x0C: printk("-"); break;
        case 0x0D: printk("="); break;

        case 0x10: printk("q"); break;
        case 0x11: printk("w"); break;
        case 0x12: printk("e"); break;
        case 0x13: printk("r"); break;
        case 0x14: printk("t"); break;
        case 0x15: printk("z"); break;
        case 0x16: printk("u"); break;
        case 0x17: printk("i"); break;
        case 0x18: printk("o"); break;
        case 0x19: printk("p"); break;
        case 0x1A: printk("["); break;
        case 0x1B: printk("]"); break;

        case 0x1E: printk("a"); break;
        case 0x1F: printk("s"); break;
        case 0x20: printk("d"); break;
        case 0x21: printk("f"); break;
        case 0x22: printk("g"); break;
        case 0x23: printk("h"); break;
        case 0x24: printk("j"); break;
        case 0x25: printk("k"); break;
        case 0x26: printk("l"); break;
        case 0x27: printk(";"); break;
        case 0x28: printk("'"); break;

        case 0x2C: printk("z"); break;
        case 0x2D: printk("x"); break;
        case 0x2E: printk("c"); break;
        case 0x2F: printk("v"); break;
        case 0x30: printk("b"); break;
        case 0x31: printk("n"); break;
        case 0x32: printk("m"); break;
        case 0x33: printk(","); break;
        case 0x34: printk("."); break;
        case 0x35: printk("/"); break;

        case 0x39: printk(" "); break;
        case 0x0E: break; // Backsapce
        case 0x2B: printk("\\"); break;
        case 0x48: break; // Up arrow
        case 0x4B: break; // Left arrow
        case 0x50: break; // Down arrow
        case 0x4D: break; // Right arrow
        case 0x0F: printk("\t"); break;
        case 0x1C: printk("\n"); break;

        default:
            printk("Keyboard 0x");
            printh(key);
            printk("\n");
            break;
        }
    } else {
        //printh(key);
    }
    return esp;
}