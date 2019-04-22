#ifndef TUNGSTEN_C_BUS_H
#define TUNGSTEN_C_BUS_H

#define PXA255_ROM_START_ADDRESS 0x00000000
#define PXA255_RAM_START_ADDRESS 0xA0000000
#define PXA255_LCD_START_ADDRESS 0x44000000
#define TUNGSTEN_C_ROM_SIZE (4 * 0x100000)//4mb ROM
#define TUNGSTEN_C_RAM_SIZE (64 * 0x100000)//64mb RAM
#define PXA255_LCD_SIZE 0x00001000

#define PXA255_START_BANK(address) ((address) >> 26)

#endif
