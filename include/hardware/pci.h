#ifndef __PCI_H
#define __PCI_H

#include <common/types.h>
#include <drivers/driver.h>
#include <hardware/port.h>
#include <hardware/interrupts.h>

enum BaseAddressRegisterType {
    MemoryMapping = 0,
    InputOutput = 1
};

class BaseAddressRegister {
  public:
    bool prefetchable;
    uint8_t* address;
    uint32_t size;
    BaseAddressRegisterType type;
};

class PCIDeviceDescriptor {
  public:
    uint32_t portBase;
    uint32_t interrupt;

    uint16_t bus;
    uint16_t device;
    uint16_t function;

    uint16_t vendor_id;
    uint16_t device_id;

    uint8_t class_id;
    uint8_t subclass_id;
    uint8_t interface_id;

    uint8_t revision;

    PCIDeviceDescriptor();
    ~PCIDeviceDescriptor();
};

class PCIController {
    Port32Bit dataPort;
    Port32Bit commandPort;

  public:
    PCIController();
    ~PCIController();

    uint32_t Read(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffest);
    void Write(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset, uint32_t value);
    bool DeviceHasFunctions(uint16_t bus, uint16_t device);

    void SelectDrivers(DriverManager* driverManager, InterruptManager* interrupts);
    Driver* GetDriver(PCIDeviceDescriptor dev, InterruptManager* interrupts);
    PCIDeviceDescriptor GetDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function);
    BaseAddressRegister GetBaseAddressRegister(uint16_t bus, uint16_t deice, uint16_t function, uint16_t bar);
};

#endif