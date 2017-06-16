#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "types.h"
#include "interrupts.hpp"
#include "port.hpp"

class KeyboardDriver : public InterruptHandler {

    Port8Bit dataport;
    Port8Bit commandport;

  public:
    KeyboardDriver(InterruptManager* manager);
    ~KeyboardDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
};

#endif