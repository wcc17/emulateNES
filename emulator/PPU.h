//
// Created by Christian Curry on 6/5/16.
//

#ifndef EMULATENES_PPU_H
#define EMULATENES_PPU_H

#include <_types/_uint8_t.h>
#include <_types/_uint16_t.h>

class PPU {
    //these registers should only be written to
    const uint8_t PPU_CONTROL_REGISTER_1 = 0x2000;
    const uint8_t PPU_CONTROL_REGISTER_2 = 0x2001;
    const uint8_t PPU_STATUS_REGISTER = 0x2002;

    //ppus registers mostly located in I/O registers section of CPU memory at $2000-$2007 and $4014
    //any address above $3FFF is wrapped around, making $4000-$FFFF a mirror of $0000-$3FFF
    //reading/writing PPU can be done using $2006/$2007 in CPU memory. usually done during V-blank at the end of a frame
    //ppu memory uses 16-bit addresses, so two writes to $2006 are required to set teh address required
    //data can be read from or written to $2007. The first read from $2007 is invalid and data will be buffered and returned on the next read. does not apply to color palletes
    uint8_t ppu_videoRAM[0x4000];

    //PPU also has a seperate 256 byte area of memory, SPR-RAM to store the sprite attributes. the sprites themselves can be found in the pattern tables
    uint8_t ppu_spriteRAM[0x100];

    //bit 7 of $2000 can be used to disabled NMIs. Clear the bit
    //NES supports both 8x8 and 8x16 sprites. setting bit 5 of $2000 will switch to 8x16 sprites

    //the next address in PPU memory to read or write from will be incremented after each I/O occurs.
    //the value to incrememnet by is adjusted by setting the value of bit 2 of $2000. If clear, the address is incremented by 1(horizontal, if set 32(vertical)

    //using $2001, the background can be hidden by clearing bit 3 and sprites can be hidden by clearing bit 4

    //$2002 is read only (status register). CPU uses to check PPU status.
    //Bit 7 is set by the PPU to indicate that vblank is occuring
    //bit (5 and 6?) relate to sprites.
    //bit 4 indicates whether the PPU is willing to accept writes to VRAM, it clear then writes are ignored
    //when a read from $2002 occurs, bit 7 is reset to 0 as are $2005 and $2006

};


#endif //EMULATENES_PPU_H
