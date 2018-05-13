//
// Created by Christian Curry on 2/4/17.
//

#ifndef EMULATENES_PPUCONSTANTS_H
#define EMULATENES_PPUCONSTANTS_H

//VPHB SINN
/**
 * NMI enable (V)
 * PPU master/slave (P)
 * sprite height (H)
 * background Tile select (B)
 * sprite tile select (S)
 * increment mode (I)
 * nametable select (NN)
 */
const uint16_t PPU_CTRL = 0x2000;

//BGRs bMmG
/**
 * color emphasis (BGR)
 * sprite enable (s)
 * background enable (b)
 * sprite left column enable (M)
 * background left column enable (m)
 * greyscale (G)
 */
const uint16_t PPU_MASK = 0x2001;

//VSO- ----
/**
 * vblank (v)
 * sprite 0 hit (S)
 * sprite overflow (O)
 * read resets write pair for $2005/$2006
 */
const uint16_t PPU_STATUS = 0x2002;

//Object attribute memory
//aaaa aaaa
//OAM read/write address
const uint16_t OAM_ADDR = 0x2003;

//Object attribute data
//dddd dddd
//OAM data read/write
const uint16_t OAM_DATA = 0x2004;

//xxxx xxxx
//fine scroll position (two writes: X, Y)
const uint16_t PPU_SCROLL = 0x2005;

//aaaa aaaa
//PPU read/write address (two writes, MSB, LSB)
//This is the VRAM address (v)
const uint16_t PPU_ADDR = 0x2006;

//dddd dddd
//PPU data read/write
const uint16_t PPU_DATA = 0x2007;

//aaaa aaaa
//OAM DMA high address
const uint16_t OAM_DMA = 0x4014;

#endif //EMULATENES_PPUCONSTANTS_H
