#include <hardware/pci.h>

PCIDeviceDescriptor::PCIDeviceDescriptor()
{ }

PCIDeviceDescriptor::~PCIDeviceDescriptor()
{ }

PCIController::PCIController()
    : dataPort(0xCFC),
      commandPort(0xCF8)
{ }

PCIController::~PCIController()
{ }

uint32_t PCIController::Read(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset)
{
    uint32_t id =
        0x1 << 31
        | ((bus & 0xFF) << 16)
        | ((device & 0x1F) << 11)
        | ((function & 0x07) << 8)
        | (registeroffset & 0xFC);
    commandPort.Write(id);
    uint32_t result = dataPort.Read();
    return result >> (8 * (registeroffset % 4));
}

void PCIController::Write(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset, uint32_t value)
{
    uint32_t id =
        0x1 << 31
        | ((bus & 0xFF) << 16)
        | ((device & 0x1F) << 11)
        | ((function & 0x07) << 8)
        | (registeroffset & 0xFC);
    commandPort.Write(id);
    dataPort.Write(value);
}

bool PCIController::DeviceHasFunctions(uint16_t bus, uint16_t device)
{
    return Read(bus, device, 0, 0x0E) & (1 << 7);
}

void printk(char*);
void printh(uint8_t);

void PCIController::SelectDrivers(DriverManager* driverManager)
{
    for (int bus = 0; bus < 8; bus++) {
        for (int device = 0; device < 32; device++) {
            int nFunctions = DeviceHasFunctions(bus, device) ? 8 : 1;
            for (int function = 0; function < nFunctions; function++) {
                PCIDeviceDescriptor dev = GetDeviceDescriptor(bus, device, function);
                if (dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF)
                    break;

                printk("PCI BUS ");
                printh(bus & 0xFF);

                printk(", DEVICE ");
                printh(device & 0xFF);

                printk(", FUNCTION ");
                printh(function & 0xFF);

                printk(" = VENDOR ");
                printh((dev.vendor_id & 0xFF00) >> 8);
                printh(dev.vendor_id & 0xFF);

                printk(", DEVICE ");
                printh((dev.device_id & 0xFF00) >> 8);
                printh(dev.device_id & 0xFF);
                printk("\n");
            }
        }
    }
}

PCIDeviceDescriptor PCIController::GetDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function)
{
    PCIDeviceDescriptor result;

    result.bus = bus;
    result.device = device;
    result.function = function;

    result.vendor_id = Read(bus, device, function, 0x00);
    result.device_id = Read(bus, device, function, 0x02);

    result.class_id = Read(bus, device, function, 0x0B);
    result.subclass_id = Read(bus, device, function, 0x0A);
    result.interface_id = Read(bus, device, function, 0x09);

    result.revision = Read(bus, device, function, 0x08);
    result.interrupt = Read(bus, device, function, 0x3C);

    return result;
}