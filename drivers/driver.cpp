#include <drivers/driver.h>

Driver::Driver()
{ }

Driver::~Driver()
{ }

void Driver::Activate()
{ }

int Driver::Reset()
{
    return 0;
}

void Driver::Deactivate()
{ }

DriverManager::DriverManager()
{
    numDrivers = 0;
}

void DriverManager::AddDriver(Driver* drv)
{
    drivers[numDrivers] = drv;
    numDrivers++;
}

#include <common/types.h>

void printk(char*);
void printh(uint8_t);

void DriverManager::ActivateAll()
{
    printk("Found ");
    printh(numDrivers);
    printk(" number of drivers\n");
    for(int i = 0; i < numDrivers; i++)
        drivers[i]->Activate();
}
