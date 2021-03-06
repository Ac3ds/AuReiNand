/*
*   patches.c
*       by Reisyukaku
*   Copyright (c) 2015 All Rights Reserved
*/

#include "patches.h"
#include "memory.h"

/**************************************************
*                   Patches
**************************************************/

/*
*   MPU
*/
u8 mpu[0x2C] = {    //MPU shit
    0x03, 0x00, 0x36, 0x00, 0x00, 0x00, 0x10, 0x10, 0x01, 0x00, 0x00, 0x01, 0x03, 0x00, 0x36, 0x00, 
    0x00, 0x00, 0x00, 0x20, 0x01, 0x01, 0x01, 0x01, 0x03, 0x06, 0x20, 0x00, 0x00, 0x00, 0x00, 0x08, 
    0x01, 0x01, 0x01, 0x01, 0x03, 0x06, 0x1C, 0x00, 0x00, 0x00, 0x02, 0x08
    };

u8 nandRedir[0x08] = {0x00, 0x4C, 0xA0, 0x47, 0xC0, 0xA5, 0x01, 0x08};    //Branch to emunand function

/*
*   Sig checks
*/
u8 sigPat1[2] = {0x00, 0x20};
u8 sigPat2[4] = {0x00, 0x20, 0x70, 0x47};
u8 FIRMblock[4] = {0x00, 0x20, 0xC0, 0x46};
u8 emuInstr[5] = {0xA5, 0x01, 0x08, 0x30, 0xA5};

/**************************************************
*                   Functions
**************************************************/

void getSignatures(void *pos, u32 size, u32 *off, u32 *off2){
    //Look for signature checks
    unsigned char pattern[] = {0xC0, 0x1C, 0x76, 0xE7};
    unsigned char pattern2[] = {0xB5, 0x22, 0x4D, 0x0C};

    *off = (u32)memsearch(pos, pattern, size, 4);
    *off2 = (u32)memsearch(pos, pattern2, size, 4) - 1;
}

void getReboot(void *pos, u32 size, u32 *off, u32 *off2){
    //Look for FIRM reboot code
    unsigned char pattern[] = {0x8D, 0xE5, 0x00, 0xC0, 0x91};
    unsigned char pattern2[] = {0xF0, 0x4F, 0x2D, 0xE9, 0x3C};

    *off = (u32)memsearch(pos, pattern, size, 5) + 2;
    *off2 = (u32)memsearch(pos, pattern2, size, 5);
}

void getFIRMWrite(void *pos, u32 size, u32 *off){
    //Look for FIRM writing code
    void *firmwrite = memsearch(pos, "exe:", size, 4);
    unsigned char pattern[] = {0x00, 0x28, 0x01, 0xDA};

    *off = (u32)memsearch(firmwrite - 0x100, pattern, 0x100, 4);
}