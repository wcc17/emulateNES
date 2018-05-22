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

    if(totalCycles > 30000 && !isPPUReady) {
        isPPUReady = true;
    }

    if(scanline < 240) {
        //visible scanlines (0 - 239)
        renderScanLine();
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

    //TODO: need to check if ppuCycles are done for the current scanline before increasing. may require refactoring of renderScanLine method
    scanline++;

    //TODO: OAMADDR is set to 0 during each of ticks 257-320 (the sprite tile loading interval) of the pre-render and visible scanlines.

}

uint8_t nameTableByte;
uint8_t tileColorBit3And2;
uint8_t patternTableLowerPlane;
void PPU::renderScanLine() {

    uint8_t scrollY = scanline >> 3;
    uint8_t tileRowIndex = (scanline + scrollY) % 8; //which of the 8 rows in a tile

    if(ppuClockCycle == 0) {
        //idle cycle
    } else if(ppuClockCycle < 257){
        //TODO: the following might not be happening at the exact correct cyle, need to re-evaluate
        switch(ppuClockCycle) {
            case 0:
                //nameTableByte will be the tile index in the name table
                nameTableByte = getNameTableByte();
                break;
            case 1:
                tileColorBit3And2 = getAttributeValue();
                break;
            case 2:
                //get lower plane (plane 0) of pattern table
                patternTableLowerPlane = getPatternTableByte(false, nameTableByte, tileRowIndex, 0);
                break;
            case 3:
                //TODO: happening in the wrong place
                renderTile(tileRowIndex);
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;

        }
    } else {
        ppuClockCycle = 0;
    }

    //TODO: reset the ppuClockCycle when this scanline is rendered

    ppuClockCycle++;
    totalCycles++;
}

void PPU::renderTile(uint8_t tileRowIndex) {
    int startBit = 7;
    int endBit = 0;
    uint8_t patternTableHigherPlane = getPatternTableByte(false, nameTableByte, tileRowIndex, 1);

    //TODO: scrolling. need to figure out fineXScroll (loopyX). will be set in one of the register writes, see skinny docs on where
    //TODO: taken from neschan
//    int tile = (int)(scanline / 6) / 8; //gets a number 0-8 depending on scanline
//    if(loopyX > 0) {
//        if(tile == 0) {
//            startBit = 7 - loopyX;
//        } else if(tile == 32) {
//            endBit = 7 - loopyX + 1;
//        } else if(tile > 32) {
//            return;
//        }
//    } else {
//        //render exactly 32 tiles
//        if(tile > 31) return;
//    }

    for(int i = startBit; i >= endBit; --i) {
        uint8_t columnBit = 0x01 << i; //move first bit over i spots to get the correct column

        //will be used along with tilePaletteBit3and2 variable
        //basically: get lower plane bit and move to first bit, get high bit and move to second bit, combine the two into tilePaletteBit1
        uint8_t tileColorBit1 = ((patternTableLowerPlane & columnBit) >> i) | ((patternTableHigherPlane & columnBit) >> i << 1);

        uint8_t color = tileColorBit3And2 | tileColorBit1;

        //TODO: how to get this? should i consider that this is a background pixel?
        uint8_t actualPixelColor = color;
//        uint8_t actualPixelColor = getPaletteColor(color);

        //TODO: should be using SCREEN_WIDTH variable here, not 256
//        uint16_t frameAddress = uint16_t(scanline) * 256;
        pixels[scanline][i] = actualPixelColor;
    }
//
//        if (frame_addr >= sizeof(_frame_buffer_1))
//            continue;
//        _frame_buffer[frame_addr] = _pixel_cycle[i];
//
//        // record the palette index just for sprite 0 hit detection
//        // the detection use palette 0 instead of actual color
//        _frame_buffer_bg[frame_addr] = tile_palette_bit01;
//    }
//
//    // Increment X position
//    if ((_ppu_addr & 0x1f) == 0x1f)
//    {
//        // Wrap to the next name table
//        _ppu_addr &= ~0x1f;
//        _ppu_addr ^= 0x0400;
//    }
//    else
//    {
//        _ppu_addr++;
//    }
}

uint8_t PPU::getNameTableByte() {
    //last two bits of PPU_CTRL are the name table select.
    //00000011 AND whatever8bitvalue will give the selection we want
    uint8_t selection = 0x03 & this->readMemoryLocation(PPU_CTRL);

    if(selection > 0x03) {
        printf("Name table selection should only be 0-3\n");
    }

    uint16_t nameTableAddress = getNameTableByteAddress(selection);
    return memory->readVRAM(nameTableAddress);
}

uint16_t PPU::getNameTableByteAddress(uint8_t nameTableSelection) {
    uint16_t nameTableStartAddress = 0x2000;

    //TODO: need a better way to do this now that its clear
    if(rom->horizontalMirroring) {
        //$2000 equals $2400 and $2800 equals $2C00
        switch(nameTableSelection) {
            case 0x00:
                nameTableStartAddress = 0x2000;
                break;
            case 0x01:
                nameTableStartAddress = 0x2000;
                break;
            case 0x02:
                nameTableStartAddress = 0x2800;
                break;
            case 0x03:
                nameTableStartAddress = 0x2800;
                break;
        }
    } else if(rom->verticalMirroring) {
        //$2000 equals $2800 and $2400 equals $2c00
        switch(nameTableSelection) {
            case 0x00:
                nameTableStartAddress = 0x2000;
                break;
            case 0x01:
                nameTableStartAddress = 0x2400;
                break;
            case 0x02:
                nameTableStartAddress = 0x2000;
                break;
            case 0x03:
                nameTableStartAddress = 0x2400;
                break;
        }
    } else {
        switch(nameTableSelection) {
            case 0x00:
                nameTableStartAddress = 0x2000;
                break;
            case 0x01:
                nameTableStartAddress = 0x2400;
                break;
            case 0x02:
                nameTableStartAddress = 0x2800;
                break;
            case 0x03:
                nameTableStartAddress = 0x2c00;
                break;
        }
    }

    //
    //bits 0-11 of PPU_ADDR hold the nametable address (-$2000) which is added to 0x2000 (where name table starts in cpu mem)
    return nameTableStartAddress | (loopyV & 0x0FFF);
}

uint8_t PPU::getAttributeValue() {
    //loopyV
    //leftmost bit ignored
    //yyy NN YYYYY XXXXX
    //y - fine Y, N nametable select, Y coarseY, X coarseX

    //one of the 32 columns in the name table
    uint8_t tileColumn = loopyV & 0x1f; //get first 5 bits for nametable tile column (starting from far right) coarseX
    uint8_t tileRow = (loopyV & 0x3e0) >> 5; //get the coarseY bits (bit shifting 5 because 3e0 is greater than a byte
    uint8_t tileAttributeColumn = (tileColumn >> 2) & 0x7; //get last three bits of tileColumn for attribute column.
    uint8_t tileAttributeRow = (tileRow >> 2) &0x7;

    //23c0 is base attribute table address (0x2000 + 960 bytes (3c0))
    //loopyV & 0c000 will account for if we're in first name table or second name table
    //last two operations with attributeRow and column will account for those values within the attribute table (will tack them on to the end)
    uint16_t attributeTableAddress = 0x23c0 | (loopyV & 0x0c00) | (tileAttributeRow << 3) | tileAttributeColumn;
    uint8_t attributeByte = memory->readVRAM(attributeTableAddress);

    //we have our attribute byte now
    //each byte represents a 4x4 group of tiles on the screen (4 quadrants of 4 tiles each)
    //4 tiles per 1 quadrant
    //each quadrant serves 16x16 pixels
    //we have the attribute value (attributeByte), but need to figoure out which of the bits below we're looking at (depending on what tile we're on)
    /**
    7654 3210
    |||| ||++- Color bits 3-2 for top left quadrant of this byte
    |||| ++--- Color bits 3-2 for top right quadrant of this byte
    ||++------ Color bits 3-2 for bottom left quadrant of this byte
    ++-------- Color bits 3-2 for bottom right quadrant of this byte
     */
    uint8_t quadrantId = (tileRow & 0x2) + ((tileColumn & 0x2) >> 1); //will be a value 0-3
    //get bits 3 and 2 for color. 1st bit will be gotten from pattern table
    uint8_t colorBit3And2 = (attributeByte & (0x3 << (quadrantId * 2))) >> (quadrantId * 2); //I didn't take time to understand this

    return (colorBit3And2 << 2);
}

uint8_t PPU::getPatternTableByte(bool isSprite, uint8_t tileIndex, uint8_t tileRowIndex, int bitPlane) {
    //TODO: to read pattern table
    //pattern table made up of shapes of tiles for background and sprites
    //each tile is 16 bytes (made of two planes)
    //first plane controls bit 0 of the color. the second plane controls bit 1
    //any pixel whose color is 0 is background/transparent
    //https://wiki.nesdev.com/w/index.php/PPU_pattern_tables
    //pattern table is divided into two 256-tile sections
    //in cpu memory:
    //$0000-$0FFF	$1000	Pattern table 0 (left)
    //$1000-$1FFF	$1000	Pattern Table 1 (right)

    //retrieving a pattern table byte:
    //TODO: not sure which address to use to actually read
    //value written to PPU_CTRL (2000) (3rdth bit for sprite, 4th bit for background) controls whether bkgrnd and sprites
    // use (left) or (right) pattern tables.
    //
    //^left half (0000-0fff), right half (1000-1fff) so just set left most bit
    //ignore that PPU_CTRL 4th bit if not in (8x8 mode) (sprite only)
    //what all the bits mean in a pattern table address:
    //0-2: fine y offset, the row number within a tile
    //3: bit plane (0: lower, 1: higher) 2 planes in each tile
    //4-7: tile column
    //8-11: tile row
    //12: half of sprite table (0: left, 1: right) -- should be something OAMADDR related
    //13:  pattern table is at 0000-1fff. most significant bit for pattern table size
    //i guess the rest don't matter so that its below 1fff

    //check if sprite or background. sprite = 3rd bit, background = 4th bit
    int bitToCheck = (isSprite) ? 3 : 4;
    bool leftOrRight = util.checkBit(memory->directReadMemoryLocation(PPU_CTRL), bitToCheck);

    uint16_t tileAddress = isSprite ? ((memory->directReadMemoryLocation(PPU_CTRL) & 0x8) << 0x8)
                                    : ((memory->directReadMemoryLocation(PPU_CTRL) & 0x10) << 0x8);
    tileAddress |= (tileIndex << 4);

    return memory->directReadMemoryLocation(tileAddress | (bitPlane << 3) | tileRowIndex);
}

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
            //TODO: is this correct? should be using oamAddrValue as the address? or is it put in loopyV?
            uint8_t oamAddrValue = memory->directReadMemoryLocation(OAM_ADDR);
            returnValue = ppuLatch = primaryOAM[oamAddrValue];
            break;
        }
        case PPU_SCROLL:
            break;
        case PPU_DATA: {
            //TODO: should i be checking for this first bad read? if not should i be incrementing? or will programmer know to account for the increment on second read?
            returnValue = memory->readVRAM(loopyV);

            uint8_t ppuCtrl = memory->directReadMemoryLocation(PPU_CTRL);
            loopyV += (ppuCtrl == 0) ? 0x01 : 0x20; //increment by 1 or 32
            break;
        }
        case OAM_DMA:
            break;
    }

    return returnValue;
}

void PPU::writeMemoryLocation(uint16_t address, uint8_t data) {

    bool shouldWrite = true;

    uint8_t ppuAddr = memory->directReadMemoryLocation(PPU_ADDR);
    memory->directWriteMemoryLocation(PPU_ADDR, ppuAddr & 0x3FFF);
    switch(address) {
        case PPU_CTRL:
            if(!isPPUReady) {
                //TODO: ppu needs to wait like 30k cycles before doing this
                shouldWrite = false;
            } else {
                loopyT &= 0xf3ff; //(0000110000000000) zero out the two bits we're looking for
                loopyT |= (data & 3) << 10; //get the two bits from data and or with loopyT (where we cleared them earlier)
            }
            break;
        case PPU_MASK:
            if(!isPPUReady) {
                //TODO: ppu needs to wait like 30k cycles before doing this
                shouldWrite = false;
            }
            break;
        case OAM_ADDR:
            //TODO: write to latch
            memory->directWriteMemoryLocation(OAM_ADDR, data);
            break;
        case OAM_DATA: {
            //TODO: write to latch
            uint8_t oamAddr = memory->directReadMemoryLocation(OAM_ADDR);
            primaryOAM[memory->directReadMemoryLocation(OAM_ADDR)] = data;
            memory->directWriteMemoryLocation(OAM_ADDR, oamAddr+1);
            break;
        }
        case PPU_SCROLL:
            if(!firstWriteRecieved) {
                //from skinny docs
                loopyT &= 0xffe0;
                loopyT |= (data & 0xf8) >> 3;
                //loopyX = data & 0x07;

                firstWriteRecieved = true;
            } else {
                //from skinny docs
                loopyT &= 0xfc1f;
                loopyT |= (data & 0xf8) << 2;
                loopyT &= 0x8ff;
                loopyT |= (data & 0x07) << 12;

                firstWriteRecieved = false;
            }

            break;
        case PPU_ADDR:
            if(!firstWriteRecieved) {
                //first write
                //write high byte of the address
                //TODO: skinny docs say first two bits should be left out, but i saw some emulators that didn't do that, not doing it here
                loopyT &= (uint16_t)(data | 0xFF00); //get the first 8 bits, put in lower half of latch
                firstWriteRecieved = true;
            } else {
                //second write
                loopyT = (loopyT << 8) + data;
                firstWriteRecieved = false;

                loopyV = loopyT;
            }

            //set fineY bit?

            break;
        case PPU_DATA: {
            //if bit 4 of PPU_STATUS is set to 0, don't write here
            if (util.checkBit(memory->directReadMemoryLocation(PPU_STATUS), 4)) {
                break;
            }

            //write data to address in vram in loopyV. should have been loaded in two writes for PPU_ADDR case
            memory->writeVRAM(loopyV, data);

            //increment ppuAddress by 1 or 32 depending on ppu_ctrl
            uint8_t ppuCtrl = memory->directReadMemoryLocation(PPU_CTRL);
            loopyV += (ppuCtrl == 0) ? 0x01 : 0x20;
            break;
        }
        case OAM_DMA:
            //TODO: this stops stuff and writes 256 bytes into ppu vram
//            memory->writeVRAM(address, data);
            break;
    }

    if(shouldWrite) {
        memory->directWriteMemoryLocation(address, data);
    }
}