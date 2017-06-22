#include <hardware/port.h>
#include <hardware/interrupts.h>
#include <drivers/driver.h>
#include <common/types.h>

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

    uint32_t read(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset);
    void write(uint16_t bus, uint16_t device, uint16_t function, uint32_tr registeroffset, uint32_t value);
    bool hasFunctions(uint32_t bus, uint16_t device);

    void selectDrivers(DriverManager* driveManger);
    PCIDeviceDescriptor gatDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function);
};