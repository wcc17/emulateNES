//
// Created by Christian Curry on 6/5/16.
//

#include "PPU.h"

PPU::PPU(Memory* memory, ROM* rom, CPU* cpu) {
    this->memory = memory;
    this->rom = rom;
    this->cpu = cpu;
    memory->ppu = this;
}

void PPU::onPowerUp() {
    //? = unknown
    //x = irrelevant
    //+ = often set
    //U = unchanged
    memory->directWriteMemoryLocation(PPU_CTRL, 0x00);
    memory->directWriteMemoryLocation(PPU_MASK, 0x00);
    memory->directWriteMemoryLocation(PPU_STATUS, 0xA0); //+0+x xxxx
    memory->directWriteMemoryLocation(OAM_ADDR, 0x00);
    memory->directWriteMemoryLocation(PPU_SCROLL, 0x00);
    memory->directWriteMemoryLocation(PPU_ADDR, 0x00);
    memory->directWriteMemoryLocation(PPU_DATA, 0x00);

    //TODO: odd frame????
    //TODO: OAM -- pattern
    //TODO: NT RAM (external, in Control Deck)	mostly $FF
    //TODO: CHR RAM (external, in Game Pak)	unspecified pattern
}

void PPU::onReset() {
    //? = unknown
    //x = irrelevant
    //+ = often set
    //U = unchanged
    memory->directWriteMemoryLocation(PPU_CTRL, 0x00);
    memory->directWriteMemoryLocation(PPU_MASK, 0x00);
    memory->directWriteMemoryLocation(PPU_STATUS, (memory->directReadMemoryLocation(PPU_STATUS) & 0x80)); //U??x xxxx
    memory->directWriteMemoryLocation(OAM_ADDR, 0x00);
    memory->directWriteMemoryLocation(PPU_SCROLL, 0x00);
    memory->directWriteMemoryLocation(PPU_ADDR, 0x00);
    memory->directWriteMemoryLocation(PPU_DATA, 0x00);
}

void PPU::execute() {
    render();
}

void PPU::render() {

    //TODO:will be moved inside one of these i think
    renderScanLine();

    if(scanline < 240) {
        //visible scanlines (0 - 239)
//        renderBackground();
    } else if(scanline < 241) {
        //post render scanline (240)
    } else if(scanline < 261) {
        //241 is the first vblank scanline. do generate nmi interrupt work
        if(scanline == 241) {
            handleNMIInterrupt();
        }
        //vertical blanking lines(241-260)
    } else if(scanline == 261) {
        //pre-render scanline (261)
        scanline = 0;
    }

    //OAMADDR is set to 0 during each of ticks 257-320 (the sprite tile loading interval) of the pre-render and visible scanlines.

}

//void PPU::renderBackground() {
//
//}

void PPU::renderScanLine() {

    //Some general rules
    //External PPU memory can be accessed every 2 PPU clock cycles
    //With 341 clock cycles per scanline, the PPU makes 170 memory addresses per scanline
    //After the 170th fetch, the PPU does nothing for 1 clock cycle.
    //A single pixel is rendered every clock cycle

    //1. Memory fetch phase 1 thru 128
    //This process is repeated 32 times (32 tiles in a scanline)
    //Each of these 4 things takes 1 clock cycle each,
    //all repeated 32 times = 128 clock cycles in 1 scanline
    //1. Fetch name table byte
    //2. Attribute table byte
    //3. Pattern table bitmap #0
    //4. Pattern table bitmap #1


    if(ppuClockCycle == 0) {
         //idle cycle
    } else if(ppuClockCycle < 257){
        int phase = ppuClockCycle % 4;
        switch(phase) {
            case 0: {
                //fetch name table byte
                uint8_t nameTableByte = getNameTableByte();
                break;
            }
            case 1: {
                //fetch attribute table byte
//                uint8_t attributeTableByte = getAttributeTableByte();
                break;
            }
//            case 2:
////                //fetch pattern table bitmap #0
//                break;
//            case 3:
////                //fetch pattern table bitmap #1
//                break;
        }
    }

    scanline++;
}

//void PPU::renderVisibleScanLine() {
    //during all visible scanlines, the ppu scans through OAM (ppu memory (vram))
    //determines which sprites to render on the next scanline
    //sprites found are copies into secondary OAM
    //if more than 8 are found, sprite over flow flag ---> TODO:

    //during each pixel clock (341 per scanline):
    //cycles 1-64: secondary oam (32-byte buffer) is initialized to $FF
    //TODO: during this, reads from $2004 will return $FF

    //cycles65-256: sprite evaluation
    //on odd cycles, data is read from primary OAM

    //on even cycles, data is written to secondary OAM (unless secondary OAM is full, in which case it will read the value at that spot)
    //1. starting at n = 0, read a sprites y-coordinate (OAM[n][0], copying it to the next open slot in secondary OAM (unless 8 sprites have been found, then the write is ignored)
    //  1a. If Y-coordinate is in range, copy remaining bytes of sprite data (OAM[n][1] thru OAM[n][3]) into secondary OAM.

    //2. increment n
    //  2a. if n h as overflowed back to zero (all 64 sprites evaluated, go to #4)
    //  2b. If less than 8 sprites have been found, go to 1
    //  2c. If exactly 8 sprites have been found, disable writes to secondary OAM because it is full

    //3. starting at m = 0, evaluate OAM[n][m] as a Y-coordinate
//}

uint8_t PPU::getNameTableByte() {
    //last two bits of PPU_CTRL are the name table select.
    //00000011 AND whatever8bitvalue will give the selection we want
    uint8_t selection = 0x03 & this->readMemoryLocation(PPU_CTRL);

    if(selection > 0x03) {
        printf("Name table selection should only be 0-3\n");
    }

    uint16_t nameTableAddress = getNameTableAddress(selection);
    return memory->readVRAM(nameTableAddress);
}

uint16_t PPU::getNameTableAddress(uint8_t nameTableSelection) {
    uint16_t address = 0;

    if(rom->horizontalMirroring) {
        //$2000 equals $2400 and $2800 equals $2C00
        switch(nameTableSelection) {
            case 0x00:
                address = 0x2000;
                break;
            case 0x01:
                address = 0x2000;
                break;
            case 0x02:
                address = 0x2800;
                break;
            case 0x03:
                address = 0x2800;
                break;
        }
    } else if(rom->verticalMirroring) {
        //$2000 equals $2800 and $2400 equals $2c00
        switch(nameTableSelection) {
            case 0x00:
                address = 0x2000;
                break;
            case 0x01:
                address = 0x2400;
                break;
            case 0x02:
                address = 0x2000;
                break;
            case 0x03:
                address = 0x2400;
                break;
        }
    } else {
        switch(nameTableSelection) {
            case 0x00:
                address = 0x2000;
                break;
            case 0x01:
                address = 0x2400;
                break;
            case 0x02:
                address = 0x2800;
                break;
            case 0x03:
                address = 0x2c00;
                break;
        }
    }

    //
    //bits 0-11 of PPU_ADDR hold the nametable address (-$2000)
    address += (memory->directReadMemoryLocation(PPU_ADDR) & 0x0FFF);
    return address;
}

//uint8_t PPU::getAttributeTableByte() {
//}

//uint16_t PPU::getAttributeTableAddress(uint16_t addressTableAddress) {
//    return (nameTableAddress + 0x3C0) //0x3C0 end of name table and start of attribute table
//}

//void PPU::readPatternTable() {
//    //TODO: to read pattern table
//    //pattern table made up of shapes of tiles for background and sprites
//    //each tile is 16 bytes (made of two planes)
//    //first plane controls bit 0 of the color. the second plane controls bit 1
//    //any pixel whose color is 0 is background/transparent
//    //https://wiki.nesdev.com/w/index.php/PPU_pattern_tables
//    //pattern table is divided into two 256-tile sections
//    //in cpu memory:
//    //$0000-$0FFF	$1000	Pattern table 0 (left)
//    //$1000-$1FFF	$1000	Pattern Table 1 (right)
//
//    //retrieving a pattern table byte:
//    //TODO: not sure which address to use to actually read
//    //value written to PPU_CTRL (2000) (3rdth bit for sprite, 4th bit for background) controls whether bkgrnd and sprites
//    // use (left) or (right) pattern tables.
//    //
//    //^left half (0000-0fff), right half (1000-1fff) so just set left most bit
//    //ignore that PPU_CTRL 4th bit if not in (8x8 mode) (sprite only)
//    //what all the bits mean in a pattern table address:
//    //0-2: fine y offset, the row number within a tile
//    //3: bit plane (0: lower, 1: higher) 2 planes in each tile
//    //4-7: tile column
//    //8-11: tile row
//    //12: half of sprite table (0: left, 1: right) -- should be something OAMADDR related
//    //13:  pattern table is at 0000-1fff. most significant bit for pattern table size
//    //i guess the rest don't matter so that its below 1fff
//}

void PPU::handleNMIInterrupt() {
    if(!util.checkBit(this->readMemoryLocation(PPU_CTRL), 7)) {
        printf("Vblank generated\n");
        //TODO: have to make sure cpu increment behaves properly after setting the program counter here for NMI interrupt
        cpu->raiseNMIInterrupt();
    }
}

uint8_t PPU::readMemoryLocation(uint16_t address) {

    //in case one of the registers changes things
//    uint8_t oldValue = memory->directReadMemoryLocation(address);
    uint8_t returnValue = memory->directReadMemoryLocation(address);

    switch(address) {
        case PPU_STATUS: {
            //clear D7 (ppu_status)
            uint8_t ppuStatus = memory->directReadMemoryLocation(address);
            ppuStatus &= 0x7F; //set the 7th bit (far left)

            memory->directWriteMemoryLocation(PPU_SCROLL, 0x00);
            memory->directWriteMemoryLocation(PPU_ADDR, 0x00);

            loopyT = 0x0f;

            break;
        }
        case OAM_DATA: {
            uint8_t oamAddrValue = memory->directReadMemoryLocation(OAM_ADDR);
            returnValue = ppuLatch = memory->readVRAM(oamAddrValue);
            break;
        }
        case PPU_SCROLL:
            break;
        case PPU_ADDR:
            break;
        case PPU_DATA:
            break;
        case OAM_DMA:
            break;
    }

    return returnValue;
}

void PPU::writeMemoryLocation(uint16_t address, uint8_t data) {

    memory->directWriteMemoryLocation(address, data);

    switch(address) {
        case PPU_CTRL:
            break;
        case PPU_MASK:
            break;
        case OAM_ADDR:
            //memory already written correctly above. OAM_DATA will write a value to this address in ppu memory when used
            break;
        case OAM_DATA: {
            uint8_t oamAddrValue = memory->directReadMemoryLocation(OAM_ADDR);

            //TODO: check for vertical or forced blanking. if so, don't increment
            memory->writeVRAM(oamAddrValue, data); //write value to address stored in OAM_ADDR
            memory->directWriteMemoryLocation(OAM_ADDR, oamAddrValue + 1); //then increment that address
            break;
        }
        case PPU_SCROLL:
            break;
        case PPU_ADDR:
            if(!ppuAddrFirstWrite) {
                //first write
                //write high byte of the address
                loopyT &= (uint16_t)(data | 0xFF00); //get the first 8 bits, put in lower half of latch
                ppuAddrFirstWrite = true;
            } else {
                //second write
                loopyT = (loopyT << 8) + data;
                memory->directWriteMemoryLocation(PPU_ADDR, loopyT);
                ppuAddrFirstWrite = false;

                loopyV = loopyT;
            }

            //set fineY bit?

            break;
        case PPU_DATA:
            //if bit 4 of PPU_STATUS is set to 0, don't write here
            if(util.checkBit(memory->directReadMemoryLocation(PPU_STATUS), 4)) {
                break;
            }

            if(util.checkBit(memory->directReadMemoryLocation(PPU_CTRL), 2) == 0) {
                memory->directWriteMemoryLocation(PPU_ADDR, memory->directReadMemoryLocation(PPU_ADDR)+0x01);
            } else {
                memory->directWriteMemoryLocation(PPU_ADDR, memory->directReadMemoryLocation(PPU_ADDR)+0x20);
            }
            break;
        case OAM_DMA:
            memory->writeVRAM(address, data);
            break;
    }

    if(address > 0x1fff && address < 0x4000) {
        memory->writeVRAM(address, data);
    }
}