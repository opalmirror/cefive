//Crt0_prx
//Includes
#define _PSP_FW_VERSION 150
#include <pspkernel.h>
#include <pspkerneltypes.h>
#include <pspmoduleinfo.h>
#include <pspiofilemgr.h>
#include <pspmodulemgr.h>
#include <pspthreadman.h>
#include <pspwlan.h>
#include <stdio.h>
#include <stdlib.h>
#include "savedata.h"
#include <pspchnnlsv.h>
#include <pspctrl.h>
#include <string.h>
#include <pspctrl_kernel.h>
#include <pspdisplay.h>
#include <pspdisplay_kernel.h>
#include <pspthreadman_kernel.h>
#include <pspumd.h>
#include "crt0_prx.h"
#include "module.h"
#include "float.h"
#include "cheat.h"
#include "block.h"
#include "cheatengine.h"
#include "cefiveui.h"
#include "cefiveconfig.h"
#include "searchengine.h"
#include "cefive.h"

extern SceUID sceKernelSearchModuleByName(unsigned char *);

//Defines
PSP_MODULE_INFO("CEFive", 0x3007, CEFIVE_VERSION_MAJ, CEFIVE_VERSION_MIN);
PSP_MAIN_THREAD_ATTR(0); //0 for kernel mode too

//Globals
unsigned char *gameDir = "ms0:/seplugins/nitePR/__________.txt";
unsigned char gameId[10];
unsigned char running = 0;
SceUID thid;
#define MAX_THREAD 64
static int thread_count_start, thread_count_now, pauseuid = -1;
static SceUID thread_buf_start[MAX_THREAD], thread_buf_now[MAX_THREAD];

//Defines
//Block flags
/*
#define BLOCK_FLAG_DMA  	(1<<4)
#define BLOCK_FLAG_FREEZE (1<<5)
#define BLOCK_FLAG_DWORD	(3<<6)
#define BLOCK_FLAG_UWORD	(2<<6)
#define BLOCK_FLAG_WORD		(1<<6)
#define BLOCK_FLAG_BYTE		(0<<6)
*/

//Cheat flags
//#define CHEAT_FLAG_SELECTED (1<<0)	//If selected, will be disabled/enabled by music button
//#define CHEAT_FLAG_CONSTANT	(1<<1)  //If 1, cheat is constantly on regardless of music button
//#define CHEAT_FLAG_FRESH    (1<<2)  //Cheat was just recently enabled/disabled
#define BLOCK_MAX 8192
#define CHEAT_MAX 512

//Globals
SceCtrlData pad;
unsigned int blockTotal = 0;
unsigned int cheatTotal = 0;
Block block[CHEATENGINE_BLOCK_MAX];
Cheat cheat[CHEATENGINE_CHEAT_MAX];
unsigned char buffer[64];
unsigned char cheatStatus = 0;
unsigned char cheatSaved = 0;
unsigned int cheatSelected = 0;
unsigned int tabSelected = 0;
unsigned char menuDrawn = 0;
//void *vram;
unsigned int menuKey = PSP_CTRL_VOLUP | PSP_CTRL_VOLDOWN;
unsigned int triggerKey = PSP_CTRL_NOTE;
unsigned int screenKey = PSP_CTRL_LTRIGGER | PSP_CTRL_SQUARE;
unsigned int cheatHz = 0;
unsigned char cheatFlash = 0;
unsigned char cheatPause = 0;
unsigned char cheatSearch = 0;
unsigned char extMenu = 0;
unsigned int extSelected[4] = { 0, 0, 0, 0 };
unsigned char extOpt = 0;
unsigned int extOptArg = 0;
unsigned int dumpNo = 0;
unsigned int cheatNo = 0;
unsigned int searchNo = 0;
unsigned int searchMax = 0;
unsigned int searchHistoryCounter = 0;
Block searchHistory[16];
unsigned int searchResultCounter = 0;
unsigned int searchAddress[100];
unsigned int browseAddress = 0x48804000;
unsigned int browseY = 0;
unsigned int browseC = 0;
unsigned int browseX = 0;
unsigned int browseLines = 16;
unsigned int decodeFormat = 0x48800000;
unsigned int trackFormat = 0x48800000;
unsigned int browseFormat = 0x48800000;
unsigned int decodeAddress = 0x48804000;
unsigned int decodeY = 0;
unsigned int decodeC = 0;
unsigned int decodeX = 0;
unsigned int trackAddress = 0x48804000;
unsigned int trackY = 0;
unsigned int trackX = 0;
unsigned int trackC = 0;
unsigned int trackBackup = 0;
unsigned char trackMode = 0;
unsigned char trackStatus = 0;
unsigned char trackPause = 0;
unsigned int cheatDMA = 0;
unsigned char cheatButtonAgeX = 0;
unsigned char cheatButtonAgeY = 0;
unsigned char searchMode = 0;
unsigned char copyMenu = 0; //0=Menu Off, 1=Menu On, Copy selected, 2=Menu On, Paste selected
unsigned int copyData = 0x08804000;
unsigned char screenTime = 0;

unsigned char fileBuffer[1536];
unsigned int fileBufferSize = 0;
unsigned int fileBufferBackup = 0;
unsigned int fileBufferFileOffset = 0;
unsigned int fileBufferOffset = 1024;
unsigned int screenNo = 0;

signed char lolDirection = 1;
unsigned char lolValue = 0;
unsigned char lolInit = 1;
char psid[16];

unsigned char screenPath[64] = { 0 };

/* TODO: This has to be declared here because of the poor declaration
 *       of hookA and should be corrected.
 */
unsigned int hookMac(unsigned char *a_mac);

Hook hookA[1] = { 
    { { 0, NULL }, 
    "sceWlan_Driver", 
    "sceWlanDrv", 
    0x0c622081,
    hookMac }, 
    };
Hook hookB[1] = { 
    { { 0, NULL }, 
    "sceOpenPSID_Service", 
    "sceOpenPSID_driver",
    0xc69bebce, 
    NULL }, 
    };
/*Hook hookD[1] =
 {
 //{ { 0, NULL }, "sceNetInet_Library", "sceNetInet", 0x05038fc7, NULL},
 //{ { 0, NULL }, "sceNet_Library", "sceNet_lib", 0x8d33c11d, NULL}, //Config - getEtherAddress
 //{ { 0, NULL }, "sceNet_Library", "sceNet", 0x0bf0a3ae, NULL}, //Netlib
 //{ { 0, NULL }, "sceWlan_Driver", "sceWlanDrv", 0x0c622081, NULL}, //Wlan
 { { 0, NULL }, "sceOpenPSID_Service", "sceOpenPSID_driver", 0xc69bebce, NULL}, //sceNetInetGetTcpcbstat
 //{ { 0, NULL }, "sceNetInet_Library", "sceNetInet", 0xc91142e4, NULL},
 };*/

static const unsigned char patchA[] = { 
    0x21, 0x88, 0x02, 0x3c, //lui v0, $8822
    0x21, 0x10, 0x42, 0x34, //ori v0, v0, $0008
    0x08, 0x00, 0x40, 0x00, //jr v0
    0x00, 0x00, 0x00, 0x00 };

static CheatEngine krCheatEngine;
static CEFiveUi krUi;
static CEFiveConfig krConfig;
static SearchEngine krSearchEngine;
static CEState krRunState;
static ECEStartState krStartState;

#define fileBufferPeek(a_out, a_ahead) if((fileBufferOffset + a_ahead) >= 1024) { fileBufferBackup=sceIoLseek(fd, 0, SEEK_CUR); sceIoLseek(fd, a_ahead, SEEK_CUR); sceIoRead(fd, &a_out, 1); sceIoLseek(fd, fileBufferBackup, SEEK_SET); } else { a_out=fileBuffer[fileBufferOffset + a_ahead]; }
#define fileBufferRead(a_out, a_size) if(fileBufferOffset == 1024) { fileBufferSize=sceIoRead(fd, fileBuffer, 1024); fileBufferOffset=0; } memcpy(a_out, &fileBuffer[fileBufferOffset], a_size); fileBufferOffset+=a_size; fileBufferFileOffset+=a_size;
#define lineClear(a_line) pspDebugScreenSetXY(0, a_line); pspDebugScreenPuts("                                                                   "); pspDebugScreenSetXY(0, a_line);

//Arrays
unsigned int decDelta[10] = { 1000000000, 100000000, 10000000, 1000000, 100000,
        10000, 1000, 100, 10, 1 };
unsigned char* searchModeName[] = {
        "  0=Same                                                        ",
        "  1=Different                                                   ",
        "  2=Greater                                                     ",
        "  3=Less                                                        ",
        "  4=Inc by    ", "  5=Dec by    ", "  6=Equal to  " };

unsigned char* trackModeName[] = { "  0=Instruction BP  ",
        "  1=Data BP         " };
//Mac Address hooking module
unsigned char cfg[] = { 'C', 'F', 'G', 0x88, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x40,
        0x80, 0x00, 0x00 };


//Functions
int module_start(SceSize args, void *argp) __attribute__((alias("_start")));
int module_stop(SceSize args, void *argp) __attribute__((alias("_stop")));
static void computeCheats();
static void freshenCheats();
static void gamePause(SceUID thid);
static void gameResume(SceUID thid);

/* TODO: This file refers to magic variables and must be included at this point.
 *       NOT COOL!  Remove.
 */
#include "screenshot.h"

/* hookMac
 *  Global Variable References:
 *      cfg
 */
unsigned int hookMac(unsigned char *a_mac) {
    memcpy(a_mac, cfg + 5, 6);
    return 0;
}

//Cheat handler
unsigned int char2hex(unsigned char *a_data, unsigned int *a_type) {
    unsigned int dword = 0;
    unsigned int power = 0;

    while (power < 8) {
        switch (a_data[power]) {
        case '_':
            break; //Freeze cheat
        case '0':
            dword |= 0 * (1 << (4 * (7 - power)));
            break;
        case '1':
            dword |= 1 * (1 << (4 * (7 - power)));
            break;
        case '2':
            dword |= 2 * (1 << (4 * (7 - power)));
            break;
        case '3':
            dword |= 3 * (1 << (4 * (7 - power)));
            break;
        case '4':
            dword |= 4 * (1 << (4 * (7 - power)));
            break;
        case '5':
            dword |= 5 * (1 << (4 * (7 - power)));
            break;
        case '6':
            dword |= 6 * (1 << (4 * (7 - power)));
            break;
        case '7':
            dword |= 7 * (1 << (4 * (7 - power)));
            break;
        case '8':
            dword |= 8 * (1 << (4 * (7 - power)));
            break;
        case '9':
            dword |= 9 * (1 << (4 * (7 - power)));
            break;
        case 'a':
        case 'A':
            dword |= 0xA * (1 << (4 * (7 - power)));
            break;
        case 'b':
        case 'B':
            dword |= 0xB * (1 << (4 * (7 - power)));
            break;
        case 'c':
        case 'C':
            dword |= 0xC * (1 << (4 * (7 - power)));
            break;
        case 'd':
        case 'D':
            dword |= 0xD * (1 << (4 * (7 - power)));
            break;
        case 'e':
        case 'E':
            dword |= 0xE * (1 << (4 * (7 - power)));
            break;
        case 'f':
        case 'F':
            dword |= 0xF * (1 << (4 * (7 - power)));
            break;
        default:
            dword >>= 4 * (8 - power);
            *a_type = power;
            return dword;
        }

        power++;
    }

    *a_type = 8;
    return dword;
}

unsigned int cheatNew(unsigned char a_size, unsigned int a_address,
        unsigned int a_value) {
    CheatEngine* prEngine = &krCheatEngine;
    Cheat* prCheat = NULL;
    Block* prBlock = NULL;
    if ((prEngine->cheat_count + 1 < CHEATENGINE_CHEAT_MAX) &&
            (prEngine->block_count + 1 < CHEATENGINE_BLOCK_MAX)) {
        prCheat = &(prEngine->cheatlist[prEngine->cheat_count]);
        cheat_set_block(prCheat, prEngine->block_count);
        cheat_clear_flags(prCheat);
        cheat_set_length(prCheat, 1);

        prBlock = &(prEngine->blocklist[prEngine->block_count]);
        prBlock->address = a_address;
        prBlock->address &= 0x0FFFFFFF;
        prBlock->address += 0x08800000;

        prBlock->flags = 0;
        prBlock->hakVal = a_value;

        switch (a_size) {
        case 1:
            block_set_byte(prBlock);
            if (cheatSaved)
                prBlock->stdVal = *((unsigned char*) (prBlock->address));
            break;
        case 2:
            block_set_word(prBlock);
            if (cheatSaved)
                prBlock->stdVal = *((unsigned short*) (prBlock->address));
            break;
        case 4:
            block_set_dword(prBlock);
            if (cheatSaved)
                prBlock->stdVal = *((unsigned int*) (prBlock->address));
            break;
        default:
            block_set_qword(prBlock);
        }

        sprintf(prCheat->name, "NEW CHEAT %d", cheatNo);
        cheatNo++;
        prEngine->cheat_count++;
        prEngine->block_count++;
    }
}

unsigned int blockAdd(int fd, unsigned char *a_data) {
    unsigned int type;
    unsigned int offset;
    unsigned char hex[8];
    CheatEngine* prEngine = &krCheatEngine;
    Block* prBlock = NULL;

    if (prEngine->block_count != CHEATENGINE_BLOCK_MAX) {
        prBlock = &(prEngine->blocklist[prEngine->block_count]);
        prBlock->flags = 0;

        sceIoLseek(fd, 1, SEEK_CUR);
        sceIoRead(fd, hex, 8);

        prBlock->address = char2hex(hex, &type);

        if (prBlock->address == 0xFFFFFFFF) {
            block_set_dma(prBlock);
            prBlock->stdVal = 0xFFFFFFFF;
        } else {
            prBlock->address &= 0x0FFFFFFF;
            prBlock->address += 0x08800000;
        }

        offset = sceIoLseek(fd, 3, SEEK_CUR);
        sceIoRead(fd, hex, 8);

        block_set_patch(prBlock, char2hex(hex, &type));

        if (hex[0] == '_') {
            block_set_freeze(prBlock);
        }

        switch (type) {
        case 2:
            block_set_byte(prBlock);
            break;
        case 4:
            block_set_word(prBlock);
            break;
        case 8:
            block_set_dword(prBlock);
            break;
        default:
            block_set_qword(prBlock);
        }

        sceIoLseek(fd, offset + type, SEEK_SET); //Reposition the cursor depending on size of Hex value

        prEngine->block_count++;
        blockTotal++;

        return 1;
    }
    return 0;
}

void cheatEnable(unsigned int a_cheat) {
    cheatengineApplyCheat(&krCheatEngine, a_cheat);
}

void cheatEnableX(unsigned int a_cheat) {
    /*
    unsigned int counter;
    unsigned char resetDMA = 0;
    cheatDMA = 0;
    Cheat* prCheat = &cheat[a_cheat];
    Block* prBlock = NULL;

    counter = prCheat->block;
    while (counter < (prCheat->block + prCheat->len)) {
        prBlock = &block[counter];
        if (prBlock->flags & BLOCK_FLAG_DMA) {
            if (prBlock->hakVal != 0xFFFFFFFF) {
                cheatDMA = *((unsigned int*) (0x08800000
                        + (prBlock->hakVal & 0x1FFFFFF))) - 0x08800000;

                if (prBlock->stdVal != cheatDMA) {
                    resetDMA = 1;
                    prBlock->stdVal = cheatDMA;
                } else {
                    resetDMA = 0;
                }
            } else {
                cheatDMA = 0;
                resetDMA = 0;
            }
        } else {
            //Backup data?
            if (((cheatDMA) && (resetDMA)) || 
                    ((prCheat->flags & CHEAT_FLAG_FRESH) &&
                    (prBlock->flags & BLOCK_FLAG_FREEZE))) {
                switch (prBlock->flags & BLOCK_FLAG_DWORD) {
                case BLOCK_FLAG_DWORD:
                    prBlock->stdVal = *((unsigned int*) (cheatDMA
                            + prBlock->address));
                    break;
                case BLOCK_FLAG_WORD:
                    prBlock->stdVal = *((unsigned short*) (cheatDMA
                            + prBlock->address));
                    break;
                case BLOCK_FLAG_BYTE:
                    prBlock->stdVal = *((unsigned char*) (cheatDMA
                            + prBlock->address));
                    break;
                }
                if (prBlock->flags & BLOCK_FLAG_FREEZE) {
                    prBlock->hakVal = prBlock->stdVal;
                }
            }

            //Apply cheat!
            switch (prBlock->flags & BLOCK_FLAG_DWORD) {
            case BLOCK_FLAG_DWORD:
                *((unsigned int*) (cheatDMA + prBlock->address))
                        = prBlock->hakVal;
                sceKernelDcacheWritebackInvalidateRange(cheatDMA
                        + prBlock->address, 4);
                sceKernelIcacheInvalidateRange(cheatDMA
                        + prBlock->address, 4);
                break;
            case BLOCK_FLAG_WORD:
                *((unsigned short*) (cheatDMA + prBlock->address))
                        = (unsigned short) prBlock->hakVal;
                sceKernelDcacheWritebackInvalidateRange(cheatDMA
                        + prBlock->address, 2);
                sceKernelIcacheInvalidateRange(cheatDMA
                        + prBlock->address, 2);
                break;
            case BLOCK_FLAG_BYTE:
                *((unsigned char*) (cheatDMA + prBlock->address))
                        = (unsigned char) prBlock->hakVal;
                sceKernelDcacheWritebackInvalidateRange(cheatDMA
                        + prBlock->address, 1);
                sceKernelIcacheInvalidateRange(cheatDMA
                        + prBlock->address, 1);
                break;
            }
        }
        counter++;
    }
    */
}

void cheatDisable(unsigned int a_cheat) {
    cheatengineResetCheat(&krCheatEngine, a_cheat);
}

void cheatDisableX(unsigned int a_cheat) {
    /*
    unsigned int counter;
    unsigned char resetDMA = 0;
    cheatDMA = 0;
    Cheat* prCheat = &cheat[a_cheat];
    Block* prBlock = NULL;

    counter = prCheat->block;
    while (counter < (prCheat->block + prCheat->len)) {
        prBlock = &block[counter];
        if (prBlock->flags & BLOCK_FLAG_DMA) {
            if (prBlock->hakVal != 0xFFFFFFFF) {
                cheatDMA = *((unsigned int*) (0x08800000
                        + (prBlock->hakVal & 0x1FFFFFF))) - 0x08800000;
                if (prBlock->stdVal != cheatDMA) {
                    resetDMA = 1;
                    prBlock->stdVal = cheatDMA;
                } else {
                    resetDMA = 0;
                }
            } else {
                cheatDMA = 0;
                resetDMA = 0;
            }
        } else if (!resetDMA) {
            switch (prBlock->flags & BLOCK_FLAG_DWORD) {
            case BLOCK_FLAG_DWORD:
                *((unsigned int*) (cheatDMA + prBlock->address))
                        = prBlock->stdVal;
                sceKernelDcacheWritebackInvalidateRange(cheatDMA
                        + prBlock->address, 4);
                sceKernelIcacheInvalidateRange(cheatDMA
                        + prBlock->address, 4);
                break;
            case BLOCK_FLAG_WORD:
                *((unsigned short*) (cheatDMA + prBlock->address))
                        = (unsigned short) prBlock->stdVal;
                sceKernelDcacheWritebackInvalidateRange(cheatDMA
                        + prBlock->address, 2);
                sceKernelIcacheInvalidateRange(cheatDMA
                        + prBlock->address, 2);
                break;
            case BLOCK_FLAG_BYTE:
                *((unsigned char*) (cheatDMA + prBlock->address))
                        = (unsigned char) prBlock->stdVal;
                sceKernelDcacheWritebackInvalidateRange(cheatDMA
                        + prBlock->address, 1);
                sceKernelIcacheInvalidateRange(cheatDMA
                        + prBlock->address, 1);
                break;
            }
        }
        counter++;
    }
    */
}

void cheatApply(unsigned char a_type) {
    // This needs to be removed once I figure out what cheatSaved is.
    if (!cheatSaved) {
        return;
    }
    cheatengineRefresh(&krCheatEngine);
}

//0=Enable/Disable CHEAT_FLAG_FRESH, -1=Enable only all (Freeze on all)
void cheatApplyX(unsigned char a_type) 
{
    // Globals In Use:
    //  cheatFlash
    //  cheatSaved
    //  cheatStatus
    //  cheatTotal
    //  cheat[]
    /*
    unsigned int counter;
    Cheat* prCheat;

    if (!cheatSaved)
        return;
    if (!a_type) {
        cheatFlash = 2;
    } //Write the cheat twice even if CWCheat mode is off

    if (cheatStatus) {
        //Enable cheats
        counter = 0;
        while (counter < cheatTotal) {
            prCheat = &cheat[counter];
            if ((prCheat->flags & CHEAT_FLAG_SELECTED) ||
                    (prCheat->flags & CHEAT_FLAG_CONSTANT)) {
                if (((!a_type) && (prCheat->flags & CHEAT_FLAG_FRESH))
                        || (a_type)) {
                    cheatEnable(counter);
                    prCheat->flags &= ~CHEAT_FLAG_FRESH;
                }
            } else if ((!a_type) && (prCheat->flags & CHEAT_FLAG_FRESH)) {
                cheatDisable(counter);
                prCheat->flags &= ~CHEAT_FLAG_FRESH;
            }
            counter++;
        }
    } else {
        //Disable cheats
        counter = 0;
        while (counter < cheatTotal) {
            prCheat = &cheat[counter];
            if (prCheat->flags & CHEAT_FLAG_CONSTANT) {
                if (((!a_type) && (prCheat->flags & CHEAT_FLAG_FRESH))
                        || (a_type)) {
                    cheatEnable(counter);
                    prCheat->flags &= ~CHEAT_FLAG_FRESH;
                }
            } else if ((!a_type) && (prCheat->flags & CHEAT_FLAG_FRESH)) {
                cheatDisable(counter);
                prCheat->flags &= ~CHEAT_FLAG_FRESH;
            }
            counter++;
        }
    }
    */
}

void cheatSave() {
    unsigned char fileChar = 0;
    unsigned char fileMisc[3];
    unsigned int fileSize = 0;
    unsigned int counter = -1;
    unsigned int scounter = 0;
    //0=Unknown/Initial, 1=Comment, 2=Waiting for \n (ignoring)
    unsigned char fileMode = 0; 
    int fd;
    int tempFd;

    //1) Open the original cheat file
    fd = sceIoOpen(gameDir, PSP_O_RDONLY, 0777);
    if (fd > 0) {
        //Find the file size
        fileSize = sceIoLseek(fd, 0, SEEK_END);
        sceIoLseek(fd, 0, SEEK_SET);

        //Initiate the read buffer
        fileBufferOffset = 1024;
        fileBufferFileOffset = 0;

        //2) Open up the temporary and get ready to regenerate it
        tempFd = sceIoOpen("ms0:/seplugins/nitePR/temp.txt", PSP_O_WRONLY
                | PSP_O_CREAT, 0777);
        if (tempFd <= 0) {
            sceIoClose(fd);
            return;
        }

        //Add the codes that are already there
        while (fileBufferFileOffset < fileSize) {
            //Read a byte
            fileBufferRead(&fileChar, 1);
            if (fileBufferSize == 0)
                break;

            //Interpret the byte based on the mode
            if (fileMode == 0) {
                //Pick a mode
                switch (fileChar) {
                case ';':
                    fileMode = 1;
                    sceIoWrite(tempFd, ";", 1);
                    break;

                case '#':
                    fileMode = 2;
                    counter++;
                    //Add a double line skip?
                    if (counter != 0) {
                        sceIoWrite(tempFd, "\r\n", 2);
                    }

                    //Is there an error...?
                    if (counter >= cheatTotal) {
                        sceIoClose(tempFd);
                        sceIoClose(fd);
                        return;
                    }

                    //Set up the subCounter
                    scounter = cheat[counter].block;

                    //Is it on by default...?
                    if (cheat[counter].flags & CHEAT_FLAG_CONSTANT) {
                        sceIoWrite(tempFd, "#!!", 3);
                    } else if (cheat[counter].flags & CHEAT_FLAG_SELECTED) {
                        sceIoWrite(tempFd, "#!", 2);
                    } else {
                        sceIoWrite(tempFd, "#", 1);
                    }
                    //Write out the name of the cheat
                    sceIoWrite(tempFd, &cheat[counter].name, strlen(
                            cheat[counter].name));
                    sceIoWrite(tempFd, "\r\n", 2);
                    break;

                case '0':
                    if ((fileBufferFileOffset) < fileSize) {
                        fileBufferPeek(fileMisc[0], 0);
                        if (fileMisc[0] == 'x') {
                            //Is there an error...?
                            if (counter == (unsigned int) -1) {
                                sceIoClose(tempFd);
                                sceIoClose(fd);
                                return;
                            }
                            if (scounter >= (cheat[counter].block
                                    + cheat[counter].len)) {
                                sceIoClose(tempFd);
                                sceIoClose(fd);
                                return;
                            }

                            //Write out the address
                            if (block[scounter].flags & BLOCK_FLAG_DMA) {
                                sprintf(buffer, "0x%08lX ",
                                        (block[scounter].address));
                                sceIoWrite(tempFd, buffer, strlen(buffer));
                            } else {
                                sprintf(buffer, "0x%08lX ",
                                        (block[scounter].address - 0x08800000));
                                sceIoWrite(tempFd, buffer, strlen(buffer));
                            }

                            //Write out the value
                            if (block[scounter].flags & BLOCK_FLAG_FREEZE) {
                                switch (block[scounter].flags & BLOCK_FLAG_DWORD) {
                                case BLOCK_FLAG_DWORD:
                                    sprintf(buffer, "0x________\r\n");
                                    break;

                                case BLOCK_FLAG_WORD:
                                    sprintf(buffer, "0x____\r\n");
                                    break;

                                case BLOCK_FLAG_BYTE:
                                    sprintf(buffer, "0x__\r\n");
                                    break;
                                }
                            } else {
                                switch (block[scounter].flags & BLOCK_FLAG_DWORD) {
                                case BLOCK_FLAG_DWORD:
                                    sprintf(buffer, "0x%08lX\r\n",
                                            block[scounter].hakVal);
                                    break;

                                case BLOCK_FLAG_WORD:
                                    sprintf(
                                            buffer,
                                            "0x%04hX\r\n",
                                            (unsigned short) block[scounter].hakVal);
                                    break;

                                case BLOCK_FLAG_BYTE:
                                    sprintf(
                                            buffer,
                                            "0x%02hX\r\n",
                                            (unsigned char) block[scounter].hakVal);
                                    break;
                                }
                            }
                            sceIoWrite(tempFd, buffer, strlen(buffer));

                            //Skip the rest
                            fileMode = 2;
                            scounter++;
                        }
                    }
                    break;
                }
            } else if (fileMode == 1) {
                //Just copy it out straight to the file
                if ((fileChar == '\r') || (fileChar == '\n')) {
                    sceIoWrite(tempFd, "\r\n", 2);
                    fileMode = 0;
                } else {
                    sceIoWrite(tempFd, &fileChar, 1);
                }
            } else if (fileMode == 2) {
                //Just wait for an '\r' or '\n'
                if ((fileChar == '\r') || (fileChar == '\n')) {
                    fileMode = 0;
                }
            }
        }

        //Close the files
        sceIoClose(tempFd);
        sceIoClose(fd);

        //Delete the old file, rename the temporary
        sceIoRemove(gameDir);
        sceIoRename("ms0:/seplugins/nitePR/temp.txt", gameDir);
    }

    //Open the file for appending
    fd = sceIoOpen(gameDir, PSP_O_CREAT | PSP_O_WRONLY | PSP_O_APPEND, 0777);
    if (fd > 0) {
        //Add any new codes
        counter++;
        if (counter != 0)
            sceIoWrite(fd, "\r\n", 2);
        while (counter < cheatTotal) {
            //Write the cheat name
            if (cheat[counter].flags & CHEAT_FLAG_CONSTANT) {
                sceIoWrite(fd, "#!!", 3);
            } else if (cheat[counter].flags & CHEAT_FLAG_SELECTED) {
                sceIoWrite(fd, "#!", 2);
            } else {
                sceIoWrite(fd, "#", 1);
            }

            //Write out the name of the cheat
            sceIoWrite(fd, &cheat[counter].name, strlen(cheat[counter].name));
            sceIoWrite(fd, "\r\n", 2);

            //Loop through the addresses
            scounter = cheat[counter].block;
            while (scounter < (cheat[counter].block + cheat[counter].len)) {
                //Write out the address
                sprintf(buffer, "0x%08lX ", (block[scounter].address
                        - 0x08800000));
                sceIoWrite(fd, buffer, strlen(buffer));

                //Write out the value
                switch (block[scounter].flags & BLOCK_FLAG_DWORD) {
                case BLOCK_FLAG_DWORD:
                    sprintf(buffer, "0x%08lX\r\n", block[scounter].hakVal);
                    break;

                case BLOCK_FLAG_WORD:
                    sprintf(buffer, "0x%04hX\r\n",
                            (unsigned short) block[scounter].hakVal);
                    break;

                case BLOCK_FLAG_BYTE:
                    sprintf(buffer, "0x%02hX\r\n",
                            (unsigned char) block[scounter].hakVal);
                    break;
                }
                sceIoWrite(fd, buffer, strlen(buffer));

                //Next address
                scounter++;
            }

            //Next cheat
            counter++;
            sceIoWrite(fd, "\r\n", 2);
        }

        //Close the file
        sceIoClose(fd);
    }
}

static void drawCopyMenu() {
    /*
    pspDebugScreenSetXY(25, 15);
    pspDebugScreenSetBackColor(0xFF000000);
    pspDebugScreenSetTextColor(0xFFFF8000);
    sprintf(buffer, "%c", 0xC9);
    pspDebugScreenPuts(buffer);
    pspDebugScreenSetBackColor(0xFFFF0000);
    pspDebugScreenSetTextColor(0xFFFF8000);
    sprintf(buffer, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 0xC3, 0xC3, 0xC3,
            0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3,
            0xC3, 0xC3, 0xC3);
    pspDebugScreenPuts(buffer);
    pspDebugScreenSetBackColor(0xFF000000);
    pspDebugScreenSetTextColor(0xFFFF8000);
    sprintf(buffer, "%c", 0xC6);
    pspDebugScreenPuts(buffer);
    pspDebugScreenSetXY(25, 16);
    pspDebugScreenSetBackColor(0xFF000000);
    pspDebugScreenSetTextColor(0xFFFF8000);
    sprintf(buffer, "%c", 0xC9);
    pspDebugScreenPuts(buffer);
    pspDebugScreenSetBackColor(0xFFFF0000);
    pspDebugScreenSetTextColor(0xFFFFFFFF);
    sprintf(buffer, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 0x20, 0x20, 0x20,
            '-', 0x20, 'C', 'o', 'p', 'i', 'e', 'r', 0x20, '-', 0x20, 0x20,
            0x20);
    pspDebugScreenPuts(buffer);
    pspDebugScreenSetBackColor(0xFF000000);
    pspDebugScreenSetTextColor(0xFFFF8000);
    sprintf(buffer, "%c", 0xC6);
    pspDebugScreenPuts(buffer);
    pspDebugScreenSetXY(25, 17);
    pspDebugScreenSetBackColor(0xFF000000);
    pspDebugScreenSetTextColor(0xFFFF8000);
    sprintf(buffer, "%c", 0xC9);
    pspDebugScreenPuts(buffer);
    pspDebugScreenSetBackColor(0xFFFF0000);
    pspDebugScreenSetTextColor(0xFFFFFFFF);
    sprintf(buffer, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 0x20, 0x20, 0x20,
            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
            0x20, 0x20, 0x20);
    pspDebugScreenPuts(buffer);
    pspDebugScreenSetBackColor(0xFF000000);
    pspDebugScreenSetTextColor(0xFFFF8000);
    sprintf(buffer, "%c", 0xC6);
    pspDebugScreenPuts(buffer);
    pspDebugScreenSetXY(25, 18);
    sprintf(buffer, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 0xC9, 0xC3,
            0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3,
            0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC6);
    pspDebugScreenPuts(buffer);
    pspDebugScreenSetXY(25, 19);
    pspDebugScreenSetBackColor(0xFF000000);
    pspDebugScreenSetTextColor(0xFFFF8000);
    sprintf(buffer, "%c%c", 0xC9, 0x20);
    pspDebugScreenPuts(buffer);
    pspDebugScreenSetBackColor(0xFF000000);
    pspDebugScreenSetTextColor(0xFFFF8000);
    if (copyMenu == 1) {
        pspDebugScreenSetTextColor(0xFF0000FF);
        pspDebugScreenPuts(">");
    } else {
        pspDebugScreenPuts(" ");
    }
    pspDebugScreenSetBackColor(0xFF000000);
    sprintf(buffer, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 'C', 'o', 'p', 'y',
            0x20, 'A', 'd', 'd', 'r', 'e', 's', 's', 0x20, 0x20);
    pspDebugScreenPuts(buffer);
    pspDebugScreenSetBackColor(0xFF000000);
    pspDebugScreenSetTextColor(0xFFFF8000);
    sprintf(buffer, "%c", 0xC6);
    pspDebugScreenPuts(buffer);
    pspDebugScreenSetXY(25, 20);
    pspDebugScreenSetBackColor(0xFF000000);
    pspDebugScreenSetTextColor(0xFFFF8000);
    sprintf(buffer, "%c%c", 0xC9, 0x20);
    pspDebugScreenPuts(buffer);
    pspDebugScreenSetBackColor(0xFF000000);
    pspDebugScreenSetTextColor(0xFFFF8000);
    if (copyMenu == 2) {
        pspDebugScreenSetTextColor(0xFF0000FF);
        pspDebugScreenPuts(">");
    } else {
        pspDebugScreenPuts(" ");
    }
    pspDebugScreenSetBackColor(0xFF000000);
    sprintf(buffer, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 'P', 'a', 's', 't',
            'e', 0x20, 'A', 'd', 'd', 'r', 'e', 's', 's', 0x20);
    pspDebugScreenPuts(buffer);
    pspDebugScreenSetBackColor(0xFF000000);
    pspDebugScreenSetTextColor(0xFFFF8000);
    sprintf(buffer, "%c", 0xC6);
    pspDebugScreenPuts(buffer);
    pspDebugScreenSetXY(25, 21);
    sprintf(buffer, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 0xC9, 0x20,
            0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
            0x20, 0x20, 0x20, 0x20, 0x20, 0xC6);
    pspDebugScreenPuts(buffer);
    pspDebugScreenSetXY(25, 22);
    sprintf(buffer, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 0x20, 0xC3,
            0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3,
            0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0x20);
    pspDebugScreenPuts(buffer);
    pspDebugScreenSetBackColor(0xFF000000);
    pspDebugScreenSetTextColor(0xFFFF8000);
    */
}

static void drawExtDiffSearch() {
    /*
    unsigned int convTotal;
    unsigned int counter;

    //Draw the tabs
    pspDebugScreenSetTextColor(0xFFFFFFFF);
    pspDebugScreenPuts("[Editing Search]");
    pspDebugScreenSetTextColor(0xFF808080);
    pspDebugScreenPuts(
            "\n--------------------------------------------------------------------");
    pspDebugScreenSetTextColor(0xFF808080);
    if (searchMode > 3) {
        pspDebugScreenPuts(
                "  Mode          Value.Hex   Value.Dec   Value.ASCII  Value.Float\n");
    } else {
        pspDebugScreenPuts(
                "  Mode                                                          \n");
    }

    //Apply the row color
    if (extSelected[0] == 0) {
        pspDebugScreenSetTextColor(0xFF8080BB);
    } else {
        pspDebugScreenSetTextColor(0xFFBBBBBB);
    }

    //Print out the mode name
    pspDebugScreenPuts(searchModeName[searchMode]);

    if (searchMode > 3) {
        //Print out the hex
        switch (searchHistory[0].flags & BLOCK_FLAG_DWORD) {
        case BLOCK_FLAG_DWORD:
            sprintf(buffer, "  0x%08lX  ", searchHistory[0].hakVal);
            break;
        case BLOCK_FLAG_WORD:
            sprintf(buffer, "  0x____%04hX  ",
                    (unsigned short) searchHistory[0].hakVal);
            break;
        case BLOCK_FLAG_BYTE:
            sprintf(buffer, "  0x______%02hX  ",
                    (unsigned char) searchHistory[0].hakVal);
            break;
        }
        pspDebugScreenPuts(buffer);

        //Print out the decimal
        switch (searchHistory[0].flags & BLOCK_FLAG_DWORD) {
        case BLOCK_FLAG_DWORD:
            sprintf(buffer, "%010u  ", searchHistory[0].hakVal);
            break;
        case BLOCK_FLAG_WORD:
            sprintf(buffer, "%010u  ",
                    (unsigned short) searchHistory[0].hakVal);
            break;
        case BLOCK_FLAG_BYTE:
            sprintf(buffer, "%010u  ",
                    (unsigned char) searchHistory[0].hakVal);
            break;
        }
        pspDebugScreenPuts(buffer);

        //Print out the ASCII
        buffer[0]
                = *((unsigned char*) (((unsigned int) &searchHistory[0].hakVal)
                        + 0));
        if ((buffer[0] <= 0x20) || (buffer[0] == 0xFF))
            buffer[0] = '.';
        buffer[1]
                = *((unsigned char*) (((unsigned int) &searchHistory[0].hakVal)
                        + 1));
        if ((buffer[1] <= 0x20) || (buffer[1] == 0xFF))
            buffer[1] = '.';
        buffer[2]
                = *((unsigned char*) (((unsigned int) &searchHistory[0].hakVal)
                        + 2));
        if ((buffer[2] <= 0x20) || (buffer[2] == 0xFF))
            buffer[2] = '.';
        buffer[3]
                = *((unsigned char*) (((unsigned int) &searchHistory[0].hakVal)
                        + 3));
        if ((buffer[3] <= 0x20) || (buffer[3] == 0xFF))
            buffer[3] = '.';
        buffer[4] = 0;
        pspDebugScreenPuts(buffer);

        //Print out the float
        pspDebugScreenPuts("         ");
        f_cvt(&searchHistory[0].hakVal, buffer, sizeof(buffer), 6,
                MODE_GENERIC);
        pspDebugScreenPuts(buffer);
    }

    //Skip a line, draw the pointer =)
    pspDebugScreenPuts("\n");
    if (extSelected[0] == 0) {
        //Skip the desired amount?
        pspDebugScreenPuts("    ");
        if (extSelected[1] != 0) //Skip Mode
        {
            pspDebugScreenPuts("              ");
            if (extSelected[1] != 1) //Skip Hex
            {
                pspDebugScreenPuts("          ");
                if (extSelected[1] != 2) //Skip Dec
                {
                    pspDebugScreenPuts("            ");
                    //Skip ASCII
                }
            }
        }

        //Skip the minimalist amount
        unsigned char tempCounter = extSelected[2];
        while (tempCounter) {
            pspDebugScreenPuts(" ");
            tempCounter--;
        }

        //Draw the symbol (Finally!!)
        if (extSelected[3]) {
            pspDebugScreenSetTextColor(0xFF0000FF);
        } else {
            pspDebugScreenSetTextColor(0xFFFF0000);
        }
        pspDebugScreenPuts("^");
    }
    pspDebugScreenPuts("\n");

    //Draw the misc menus
    pspDebugScreenSetTextColor(extSelected[0] == 1 ? 0xFF0000FF
            : 0xFF808080);
    pspDebugScreenPuts("  Search\n");
    if (searchNo) {
        pspDebugScreenSetTextColor(extSelected[0] == 2 ? 0xFF0000FF
                : 0xFF808080);
        pspDebugScreenPuts("  Undo Search\n");
    }

    //Print out results
    pspDebugScreenSetTextColor(0xFFFFFFFF);
    pspDebugScreenPuts("\n[Search Results: ");
    sprintf(buffer, "%d Found - Only showing first 100]",
            searchResultCounter);
    pspDebugScreenPuts(buffer);
    pspDebugScreenSetTextColor(0xFF808080);
    pspDebugScreenPuts(
            "\n--------------------------------------------------------------------");
    pspDebugScreenSetTextColor(0xFF808080);
    pspDebugScreenPuts(
            "  Address     Value.Hex   Value.Dec   Value.ASCII  Value.Float\n");

    //Print out the results variables
    convTotal = ((searchResultCounter > 100) ? 100
            : searchResultCounter);
    counter = 0;
    while (counter < convTotal) {
        //Scroll feature right here, in two lines =3
        if ((signed int) (counter)
                < (signed int) (((extSelected[0] - 3) - 7)
                        - ((((signed int) (extSelected[0] - 3) + 7)
                                - ((signed int) convTotal)) > 0 ? abs(
                                ((signed int) (extSelected[0] - 3) + 7)
                                        - ((signed int) convTotal)) : 0))) {
            counter++;
            continue;
        }
        if ((signed int) (counter)
                > (signed int) (((extSelected[0] - 3) + 7)
                        + (((signed int) (extSelected[0] - 3) - 7) < 0 ? abs(
                                (signed int) ((extSelected[0] - 3) - 7))
                                : 0))) {
            counter++;
            continue;
        }

        //Apply the row color
        if (counter == (extSelected[0] - 3)) {
            pspDebugScreenSetTextColor(0xFF8080BB);
        } else {
            pspDebugScreenSetTextColor(0xFFBBBBBB);
        }

        //Print out the address
        sprintf(buffer, "  0x%08lX  ", (searchAddress[counter]
                - 0x48800000));
        pspDebugScreenPuts(buffer);

        //Print out the hex
        switch (searchHistory[0].flags & BLOCK_FLAG_DWORD) {
        case BLOCK_FLAG_DWORD:
            sprintf(buffer, "0x%08lX  ",
                    *((unsigned int*) (searchAddress[counter])));
            break;
        case BLOCK_FLAG_WORD:
            sprintf(buffer, "0x____%04hX  ",
                    *((unsigned short*) (searchAddress[counter])));
            break;
        case BLOCK_FLAG_BYTE:
            sprintf(buffer, "0x______%02hX  ",
                    *((unsigned char*) (searchAddress[counter])));
            break;
        }
        pspDebugScreenPuts(buffer);

        //Print out the decimal
        switch (searchHistory[0].flags & BLOCK_FLAG_DWORD) {
        case BLOCK_FLAG_DWORD:
            sprintf(buffer, "%010lu  ",
                    *((unsigned int*) (searchAddress[counter])));
            break;
        case BLOCK_FLAG_WORD:
            sprintf(buffer, "%010lu  ",
                    *((unsigned short*) (searchAddress[counter])));
            break;
        case BLOCK_FLAG_BYTE:
            sprintf(buffer, "%010lu  ",
                    *((unsigned char*) (searchAddress[counter])));
            break;
        }
        pspDebugScreenPuts(buffer);

        //Print out the ASCII
        buffer[0] = *((unsigned char*) (searchAddress[counter] + 0));
        if ((buffer[0] <= 0x20) || (buffer[0] == 0xFF))
            buffer[0] = '.';
        if ((searchHistory[0].flags & BLOCK_FLAG_DWORD) != BLOCK_FLAG_BYTE) {
            buffer[1]
                    = *((unsigned char*) (searchAddress[counter] + 1));
            if ((buffer[1] <= 0x20) || (buffer[1] == 0xFF))
                buffer[1] = '.';
        } else {
            buffer[1] = '.';
        }
        if ((searchHistory[0].flags & BLOCK_FLAG_DWORD) == BLOCK_FLAG_DWORD) {
            buffer[2]
                    = *((unsigned char*) (searchAddress[counter] + 2));
            if ((buffer[2] <= 0x20) || (buffer[2] == 0xFF))
                buffer[2] = '.';
            buffer[3]
                    = *((unsigned char*) (searchAddress[counter] + 3));
            if ((buffer[3] <= 0x20) || (buffer[3] == 0xFF))
                buffer[3] = '.';
        } else {
            buffer[2] = buffer[3] = '.';
        }
        buffer[4] = 0;
        pspDebugScreenPuts(buffer);

        //Print out the float
        pspDebugScreenPuts("         ");
        f_cvt(searchAddress[counter], buffer, sizeof(buffer), 6,
                MODE_GENERIC);
        pspDebugScreenPuts(buffer);

        //Goto the next cheat down under
        pspDebugScreenPuts("\n");
        counter++;
    }

    //Helper
    pspDebugScreenSetXY(0, 33);
    pspDebugScreenSetTextColor(0xFFFF8000);
    if (extSelected[0] == 0) {
        pspDebugScreenPuts(
                ">< = Edit On/Off; \\|/ /|\\ <- -> = Cycle; () = Cancel               ");
    } else if ((extSelected[0] == 1) || (extSelected[0] == 2)) {
        pspDebugScreenPuts(
                ">< = Select; () = Cancel                                           ");
    } else {
        pspDebugScreenPuts(
                ">< = Add Selected Cheat; () = Cancel                               ");
    }
    */
}

static void drawExtSearch() {
    /*
    unsigned int convTotal;
    unsigned int counter;

    //Draw the tabs
    pspDebugScreenSetTextColor(0xFFFFFFFF);
    pspDebugScreenPuts("[Editing Search]");
    pspDebugScreenSetTextColor(0xFF808080);
    pspDebugScreenPuts(
            "\n--------------------------------------------------------------------");
    pspDebugScreenSetTextColor(0xFF808080);
    pspDebugScreenPuts(
            "  Value.Hex   Value.Dec   Value.ASCII  Value.Float\n");

    //Apply the row color
    if (extSelected[0] == 0) {
        pspDebugScreenSetTextColor(0xFF8080BB);
    } else {
        pspDebugScreenSetTextColor(0xFFBBBBBB);
    }

    //Print out the hex
    switch (searchHistory[0].flags & BLOCK_FLAG_DWORD) {
    case BLOCK_FLAG_DWORD:
        sprintf(buffer, "  0x%08lX  ", searchHistory[0].hakVal);
        break;
    case BLOCK_FLAG_WORD:
        sprintf(buffer, "  0x____%04hX  ",
                (unsigned short) searchHistory[0].hakVal);
        break;
    case BLOCK_FLAG_BYTE:
        sprintf(buffer, "  0x______%02hX  ",
                (unsigned char) searchHistory[0].hakVal);
        break;
    }
    pspDebugScreenPuts(buffer);

    //Print out the decimal
    switch (searchHistory[0].flags & BLOCK_FLAG_DWORD) {
    case BLOCK_FLAG_DWORD:
        sprintf(buffer, "%010u  ", searchHistory[0].hakVal);
        break;
    case BLOCK_FLAG_WORD:
        sprintf(buffer, "%010u  ",
                (unsigned short) searchHistory[0].hakVal);
        break;
    case BLOCK_FLAG_BYTE:
        sprintf(buffer, "%010u  ",
                (unsigned char) searchHistory[0].hakVal);
        break;
    }
    pspDebugScreenPuts(buffer);

    //Print out the ASCII
    buffer[0]
            = *((unsigned char*) (((unsigned int) &searchHistory[0].hakVal)
                    + 0));
    if ((buffer[0] <= 0x20) || (buffer[0] == 0xFF))
        buffer[0] = '.';
    buffer[1]
            = *((unsigned char*) (((unsigned int) &searchHistory[0].hakVal)
                    + 1));
    if ((buffer[1] <= 0x20) || (buffer[1] == 0xFF))
        buffer[1] = '.';
    buffer[2]
            = *((unsigned char*) (((unsigned int) &searchHistory[0].hakVal)
                    + 2));
    if ((buffer[2] <= 0x20) || (buffer[2] == 0xFF))
        buffer[2] = '.';
    buffer[3]
            = *((unsigned char*) (((unsigned int) &searchHistory[0].hakVal)
                    + 3));
    if ((buffer[3] <= 0x20) || (buffer[3] == 0xFF))
        buffer[3] = '.';
    buffer[4] = 0;
    pspDebugScreenPuts(buffer);

    //Print out the float
    pspDebugScreenPuts("         ");
    f_cvt(&searchHistory[0].hakVal, buffer, sizeof(buffer), 6,
            MODE_GENERIC);
    pspDebugScreenPuts(buffer);

    //Skip a line, draw the pointer =)
    pspDebugScreenPuts("\n");
    if (extSelected[0] == 0) {
        //Skip the desired amount?
        pspDebugScreenPuts("    ");
        if (extSelected[1] != 0) //Skip Hex
        {
            pspDebugScreenPuts("          ");
            if (extSelected[1] != 1) //Skip Dec
            {
                pspDebugScreenPuts("            ");
                //Skip ASCII
            }
        }

        //Skip the minimalist amount
        unsigned char tempCounter = extSelected[2];
        while (tempCounter) {
            pspDebugScreenPuts(" ");
            tempCounter--;
        }

        //Draw the symbol (Finally!!)
        if (extSelected[3]) {
            pspDebugScreenSetTextColor(0xFF0000FF);
        } else {
            pspDebugScreenSetTextColor(0xFFFF0000);
        }
        pspDebugScreenPuts("^");
    }
    pspDebugScreenPuts("\n");

    //Draw the misc menus
    pspDebugScreenSetTextColor(extSelected[0] == 1 ? 0xFF0000FF
            : 0xFF808080);
    pspDebugScreenPuts("  Search\n");
    if (searchNo) {
        pspDebugScreenSetTextColor(extSelected[0] == 2 ? 0xFF0000FF
                : 0xFF808080);
        pspDebugScreenPuts("  Undo Search\n");
    }

    //Print out results
    pspDebugScreenSetTextColor(0xFFFFFFFF);
    pspDebugScreenPuts("\n[Search Results: ");
    sprintf(buffer, "%d Found - Only showing first 100]",
            searchResultCounter);
    pspDebugScreenPuts(buffer);
    pspDebugScreenSetTextColor(0xFF808080);
    pspDebugScreenPuts(
            "\n--------------------------------------------------------------------");
    pspDebugScreenSetTextColor(0xFF808080);
    pspDebugScreenPuts(
            "  Address     Value.Hex   Value.Dec   Value.ASCII  Value.Float\n");

    //Print out the results variables
    convTotal = ((searchResultCounter > 100) ? 100
            : searchResultCounter);
    counter = 0;
    while (counter < convTotal) {
        //Scroll feature right here, in two lines =3
        if ((signed int) (counter)
                < (signed int) (((extSelected[0] - 3) - 7)
                        - ((((signed int) (extSelected[0] - 3) + 7)
                                - ((signed int) convTotal)) > 0 ? abs(
                                ((signed int) (extSelected[0] - 3) + 7)
                                        - ((signed int) convTotal)) : 0))) {
            counter++;
            continue;
        }
        if ((signed int) (counter)
                > (signed int) (((extSelected[0] - 3) + 7)
                        + (((signed int) (extSelected[0] - 3) - 7) < 0 ? abs(
                                (signed int) ((extSelected[0] - 3) - 7))
                                : 0))) {
            counter++;
            continue;
        }

        //Apply the row color
        if (counter == (extSelected[0] - 3)) {
            pspDebugScreenSetTextColor(0xFF8080BB);
        } else {
            pspDebugScreenSetTextColor(0xFFBBBBBB);
        }

        //Print out the address
        sprintf(buffer, "  0x%08lX  ", (searchAddress[counter]
                - 0x48800000));
        pspDebugScreenPuts(buffer);

        //Print out the hex
        switch (searchHistory[0].flags & BLOCK_FLAG_DWORD) {
        case BLOCK_FLAG_DWORD:
            sprintf(buffer, "0x%08lX  ",
                    *((unsigned int*) (searchAddress[counter])));
            break;
        case BLOCK_FLAG_WORD:
            sprintf(buffer, "0x____%04hX  ",
                    *((unsigned short*) (searchAddress[counter])));
            break;
        case BLOCK_FLAG_BYTE:
            sprintf(buffer, "0x______%02hX  ",
                    *((unsigned char*) (searchAddress[counter])));
            break;
        }
        pspDebugScreenPuts(buffer);

        //Print out the decimal
        switch (searchHistory[0].flags & BLOCK_FLAG_DWORD) {
        case BLOCK_FLAG_DWORD:
            sprintf(buffer, "%010lu  ",
                    *((unsigned int*) (searchAddress[counter])));
            break;
        case BLOCK_FLAG_WORD:
            sprintf(buffer, "%010lu  ",
                    *((unsigned short*) (searchAddress[counter])));
            break;
        case BLOCK_FLAG_BYTE:
            sprintf(buffer, "%010lu  ",
                    *((unsigned char*) (searchAddress[counter])));
            break;
        }
        pspDebugScreenPuts(buffer);

        //Print out the ASCII
        buffer[0] = *((unsigned char*) (searchAddress[counter] + 0));
        if ((buffer[0] <= 0x20) || (buffer[0] == 0xFF))
            buffer[0] = '.';
        if ((searchHistory[0].flags & BLOCK_FLAG_DWORD) != BLOCK_FLAG_BYTE) {
            buffer[1]
                    = *((unsigned char*) (searchAddress[counter] + 1));
            if ((buffer[1] <= 0x20) || (buffer[1] == 0xFF))
                buffer[1] = '.';
        } else {
            buffer[1] = '.';
        }
        if ((searchHistory[0].flags & BLOCK_FLAG_DWORD) == BLOCK_FLAG_DWORD) {
            buffer[2]
                    = *((unsigned char*) (searchAddress[counter] + 2));
            if ((buffer[2] <= 0x20) || (buffer[2] == 0xFF))
                buffer[2] = '.';
            buffer[3]
                    = *((unsigned char*) (searchAddress[counter] + 3));
            if ((buffer[3] <= 0x20) || (buffer[3] == 0xFF))
                buffer[3] = '.';
        } else {
            buffer[2] = buffer[3] = '.';
        }
        buffer[4] = 0;
        pspDebugScreenPuts(buffer);

        //Print out the float
        pspDebugScreenPuts("         ");
        f_cvt(searchAddress[counter], buffer, sizeof(buffer), 6,
                MODE_GENERIC);
        pspDebugScreenPuts(buffer);

        //Goto the next cheat down under
        pspDebugScreenPuts("\n");
        counter++;
    }

    //Helper
    pspDebugScreenSetXY(0, 33);
    pspDebugScreenSetTextColor(0xFFFF8000);
    if (extSelected[0] == 0) {
        if (searchNo == 0) {
            pspDebugScreenPuts(
                    ">< = Edit On/Off; \\|/ /|\\ <- -> = Cycle; [] = Alt-Type; () = Cancel");
        } else {
            pspDebugScreenPuts(
                    ">< = Edit On/Off; \\|/ /|\\ <- -> = Cycle; () = Cancel               ");
        }
    } else if ((extSelected[0] == 1) || (extSelected[0] == 2)) {
        pspDebugScreenPuts(
                ">< = Select; () = Cancel                                           ");
    } else {
        pspDebugScreenPuts(
                ">< = Add Selected Cheat; () = Cancel                               ");
    }
    */
}

static void drawExtTextSearch() {
    /*
    unsigned int convTotal;
    unsigned int counter;
    unsigned int scounter;

    //Draw the tabs
    pspDebugScreenSetTextColor(0xFFFFFFFF);
    pspDebugScreenPuts("[Editing Search]");
    pspDebugScreenSetTextColor(0xFF808080);
    pspDebugScreenPuts(
            "\n--------------------------------------------------------------------");
    pspDebugScreenSetTextColor(0xFF808080);
    pspDebugScreenPuts(
            "  Text                                                          \n");

    //Apply the row color
    if (extSelected[0] == 0) {
        pspDebugScreenSetTextColor(0xFF8080BB);
    } else {
        pspDebugScreenSetTextColor(0xFFBBBBBB);
    }

    //Print out the ASCII
    pspDebugScreenPuts("  '");
    fileBuffer[50] = 0;
    pspDebugScreenPuts(fileBuffer);

    //Skip a line, draw the pointer =)
    pspDebugScreenPuts("'\n");
    if (extSelected[0] == 0) {
        //Skip the desired amount?
        pspDebugScreenPuts("   ");

        //Skip the minimalist amount
        unsigned char tempCounter = extSelected[2];
        while (tempCounter) {
            pspDebugScreenPuts(" ");
            tempCounter--;
        }

        //Draw the symbol (Finally!!)
        if (extSelected[3]) {
            pspDebugScreenSetTextColor(0xFF0000FF);
        } else {
            pspDebugScreenSetTextColor(0xFFFF0000);
        }
        pspDebugScreenPuts("^");
    }
    pspDebugScreenPuts("\n");

    //Draw the misc menus
    pspDebugScreenSetTextColor(extSelected[0] == 1 ? 0xFF0000FF
            : 0xFF808080);
    pspDebugScreenPuts("  Search\n");

    //Print out results
    pspDebugScreenSetTextColor(0xFFFFFFFF);
    pspDebugScreenPuts("\n[Search Results: Only showing first 100]");
    pspDebugScreenSetTextColor(0xFF808080);
    pspDebugScreenPuts(
            "\n--------------------------------------------------------------------");
    pspDebugScreenSetTextColor(0xFF808080);
    pspDebugScreenPuts(
            "  Address   Text                                              \n");

    //Print out the results variables
    convTotal = ((searchResultCounter > 100) ? 100
            : searchResultCounter);
    counter = 0;
    while (counter < convTotal) {
        //Scroll feature right here, in two lines =3
        if ((signed int) (counter)
                < (signed int) (((extSelected[0] - 2) - 7)
                        - ((((signed int) (extSelected[0] - 2) + 7)
                                - ((signed int) convTotal)) > 0 ? abs(
                                ((signed int) (extSelected[0] - 2) + 7)
                                        - ((signed int) convTotal)) : 0))) {
            counter++;
            continue;
        }
        if ((signed int) (counter)
                > (signed int) (((extSelected[0] - 2) + 7)
                        + (((signed int) (extSelected[0] - 2) - 7) < 0 ? abs(
                                (signed int) ((extSelected[0] - 2) - 7))
                                : 0))) {
            counter++;
            continue;
        }

        //Apply the row color
        if (counter == (extSelected[0] - 2)) {
            pspDebugScreenSetTextColor(0xFF8080BB);
        } else {
            pspDebugScreenSetTextColor(0xFFBBBBBB);
        }

        //Print out the address
        sprintf(buffer, "  0x%08lX  '", (searchAddress[counter]
                - 0x48800000));
        pspDebugScreenPuts(buffer);

        //Print out the ASCII
        memset(buffer, 0, 17);
        scounter = 0;
        while (scounter < 16) {
            if ((searchAddress[counter] + scounter) < 0x4A000000) {
                buffer[scounter]
                        = *((unsigned char*) (searchAddress[counter]
                                + scounter));
                if ((buffer[scounter] <= 0x20) || (buffer[scounter]
                        == 0xFF))
                    buffer[scounter] = '.';
            } else {
                break;
            }
            scounter++;
        }
        pspDebugScreenPuts(buffer);

        //Goto the next cheat down under
        pspDebugScreenPuts("'\n");
        counter++;
    }

    //Helper
    pspDebugScreenSetXY(0, 33);
    pspDebugScreenSetTextColor(0xFFFF8000);
    if (extSelected[0] == 0) {
        pspDebugScreenPuts(
                ">< = Edit On/Off; \\|/ /|\\ <- -> = Cycle; [] = Trim; () = Cancel    ");
    } else if (extSelected[0] == 1) {
        pspDebugScreenPuts(
                ">< = Select; () = Cancel                                           ");
    } else {
        pspDebugScreenPuts(
                "<- -> = Scroll Address; () = Cancel                                ");
    }
    */
}

static void drawPrx() {
    /*
    unsigned int counter;

    counter = 0;
    while (counter < 10) {
        if (cheatSelected == counter) {
            //Highlight the selection
            pspDebugScreenSetTextColor(0xFF0000FF);
        } else {
            //Don't highlight the selection
            pspDebugScreenSetTextColor(0xFF808080);
        }
        switch (counter) {
        case 0:
            pspDebugScreenPuts("  Pause Game? ");
            if (cheatPause) {
                pspDebugScreenPuts("True\n");
            } else {
                pspDebugScreenPuts("False\n");
            }
            break;
        case 1:
            pspDebugScreenPuts("  Add Empty Cheat?\n");
            break;
        case 2:
            sprintf(buffer, "  Reset Codes? Slot #%d\n", dumpNo);
            pspDebugScreenPuts(buffer);
            break;
        case 3:
            sprintf(buffer, "  Dump RAM? Slot #%d\n", dumpNo);
            pspDebugScreenPuts(buffer);
            break;
        case 4:
            sprintf(buffer, "  Bytes per Line in Browser? %d\n",
                    browseLines);
            pspDebugScreenPuts(buffer);
            break;
        case 5:
            pspDebugScreenPuts("  Real Addressing in Browser? ");
            if (browseFormat == 0x40000000) {
                pspDebugScreenPuts("True\n");
            } else {
                pspDebugScreenPuts("False\n");
            }
            break;
        case 6:
            pspDebugScreenPuts("  Real Addressing in Decoder? ");
            if (decodeFormat == 0x40000000) {
                pspDebugScreenPuts("True\n");
            } else {
                pspDebugScreenPuts("False\n");
            }
            break;
        case 7:
            pspDebugScreenPuts("  Real Addressing in Logger? ");
            if (trackFormat == 0x40000000) {
                pspDebugScreenPuts("True\n");
            } else {
                pspDebugScreenPuts("False\n");
            }
            break;
        case 8:
            sprintf(buffer, "  Cheat Hz? %d/1000 seconds\n", (cheatHz
                    / 1000));
            pspDebugScreenPuts(buffer);
            break;
        case 9:
            pspDebugScreenPuts("  Save cheats?\n");
            break;
        }
        counter++;
    }
    //Helper
    pspDebugScreenSetTextColor(0xFFFF8000);
    lineClear(31)
    ;
    lineClear(32)
    ;
    switch (cheatSelected) {
    case 0:
        pspDebugScreenPuts(
                "Pauses the game while nitePR's menu is showing");
        break;
    case 1:
        pspDebugScreenPuts(
                "Adds an empty cheat to the Cheater for you to edit");
        break;
    case 2:
        pspDebugScreenPuts(
                "Uses the selected 'RAM dump' to regenerate OFF codes");
        break;
    case 3:
        pspDebugScreenPuts("Saves the Game's RAM to MemoryStick");
        break;
    case 4:
        pspDebugScreenPuts(
                "Alters the number of bytes displayed in the Browser");
        break;
    case 5:
        pspDebugScreenPuts(
                "If enabled, REAL PSP hardware addresses will be used in Browser");
        break;
    case 6:
        pspDebugScreenPuts(
                "If enabled, REAL PSP hardware addresses will be used in Decoder");
        break;
    case 7:
        pspDebugScreenPuts(
                "If enabled, REAL PSP hardware addresses will be used in Logger");
        break;
    case 8:
        lineClear(31)
        ;
        pspDebugScreenPuts(
                "nitePR only applies a cheat once - set this to anything other");
        lineClear(32)
        ;
        pspDebugScreenPuts(
                "than 0 to make it apply the same cheat every X amount of seconds");
        break;
    case 9:
        pspDebugScreenPuts("Save your cheats");
        break;
    }
    lineClear(33)
    ;
    if ((cheatSelected != 8) && (cheatSelected != 3) && (cheatSelected
            != 2)) {
        pspDebugScreenPuts(">< = Toggle; () = Cancel/Return to Game");
    } else {
        pspDebugScreenPuts(
                "<- -> = Decrement/Increment; () = Cancel/Return to Game");
    }
    */
}

static void drawSearcher() {
    /*
    unsigned int counter;
    unsigned int scounter;

    counter = 0;
    while (counter < (3 + ((!cheatSearch) * 2))) {
        //Scroll feature right here, in two lines =3
        if ((signed int) counter < (signed int) ((cheatSelected - 12)
                - ((((signed int) cheatSelected + 12)
                        - ((signed int) cheatTotal)) > 0 ? abs(
                        ((signed int) cheatSelected + 12)
                                - ((signed int) cheatTotal)) : 0))) {
            counter++;
            continue;
        }
        if ((signed int) counter > (signed int) ((cheatSelected + 12)
                + (((signed int) cheatSelected - 12) < 0 ? abs(
                        (signed int) (cheatSelected - 12)) : 0))) {
            counter++;
            continue;
        }

        if (cheatSelected == counter) {
            //Highlight the selection
            pspDebugScreenSetTextColor(0xFF0000FF);
        } else {
            //Don't highlight the selection
            pspDebugScreenSetTextColor(0xFF808080);
        }
        if (!cheatSearch) {
            switch (counter) {
            case 0:
                pspDebugScreenPuts("  Find Exact Value\n");
                break;
            case 1:
                pspDebugScreenPuts("  Find Unknown Value - 8bit\n");
                break;
            case 2:
                pspDebugScreenPuts("  Find Unknown Value - 16bit\n");
                break;
            case 3:
                pspDebugScreenPuts("  Find Unknown Value - 32bit\n");
                break;
            case 4:
                pspDebugScreenPuts("  Find Text\n");
                break;
            }
        } else {
            switch (counter) {
            case 0:
                pspDebugScreenPuts("  Continue to find Exact Value\n");
                break;
            case 1:
                pspDebugScreenPuts("  Continue to find Unknown Value\n");
                break;
            case 2:
                pspDebugScreenPuts("  Reset search\n");
                break;
            }
        }
        counter++;
    }

    //Print out search history
    pspDebugScreenSetTextColor(0xFFFFFFFF);
    pspDebugScreenPuts("\n[Search History]");
    pspDebugScreenSetTextColor(0xFF808080);
    pspDebugScreenPuts(
            "\n--------------------------------------------------------------------");
    pspDebugScreenSetTextColor(0xFF808080);
    pspDebugScreenPuts(
            "  Mode          Value.Hex   Value.Dec   Value.ASCII  Value.Float\n");
    scounter = 0;
    while (scounter < searchHistoryCounter) {
        //Apply the row color
        pspDebugScreenSetTextColor(0xFF8080BB - (scounter * 0x00080808));

        //Print out the mode
        pspDebugScreenPuts(
                searchModeName[searchHistory[scounter + 1].stdVal]);
        if (searchHistory[scounter + 1].stdVal > 3) {
            //Print out the hex
            switch (searchHistory[scounter + 1].flags & BLOCK_FLAG_DWORD) {
            case BLOCK_FLAG_DWORD:
                sprintf(buffer, "  0x%08lX  ", searchHistory[scounter
                        + 1].hakVal);
                break;
            case BLOCK_FLAG_WORD:
                sprintf(
                        buffer,
                        "  0x____%04hX  ",
                        (unsigned short) searchHistory[scounter + 1].hakVal);
                break;
            case BLOCK_FLAG_BYTE:
                sprintf(
                        buffer,
                        "  0x______%02hX  ",
                        (unsigned char) searchHistory[scounter + 1].hakVal);
                break;
            }
            pspDebugScreenPuts(buffer);

            //Print out the decimal
            switch (searchHistory[scounter + 1].flags & BLOCK_FLAG_DWORD) {
            case BLOCK_FLAG_DWORD:
                sprintf(buffer, "%010lu  ",
                        searchHistory[scounter + 1].hakVal);
                break;
            case BLOCK_FLAG_WORD:
                sprintf(
                        buffer,
                        "%010lu  ",
                        (unsigned short) searchHistory[scounter + 1].hakVal);
                break;
            case BLOCK_FLAG_BYTE:
                sprintf(
                        buffer,
                        "%010lu  ",
                        (unsigned char) searchHistory[scounter + 1].hakVal);
                break;
            }
            pspDebugScreenPuts(buffer);

            //Print out the ASCII
            buffer[0]
                    = *((unsigned char*) (((unsigned int) &searchHistory[scounter
                            + 1].hakVal) + 0));
            if ((buffer[0] <= 0x20) || (buffer[0] == 0xFF))
                buffer[0] = '.';
            buffer[1]
                    = *((unsigned char*) (((unsigned int) &searchHistory[scounter
                            + 1].hakVal) + 1));
            if ((buffer[1] <= 0x20) || (buffer[1] == 0xFF))
                buffer[1] = '.';
            buffer[2]
                    = *((unsigned char*) (((unsigned int) &searchHistory[scounter
                            + 1].hakVal) + 2));
            if ((buffer[2] <= 0x20) || (buffer[2] == 0xFF))
                buffer[2] = '.';
            buffer[3]
                    = *((unsigned char*) (((unsigned int) &searchHistory[scounter
                            + 1].hakVal) + 3));
            if ((buffer[3] <= 0x20) || (buffer[3] == 0xFF))
                buffer[3] = '.';
            buffer[4] = 0;
            pspDebugScreenPuts(buffer);

            //Print out the float
            pspDebugScreenPuts("         ");
            f_cvt(&searchHistory[scounter + 1].hakVal, buffer,
                    sizeof(buffer), 6, MODE_GENERIC);
            pspDebugScreenPuts(buffer);
        }

        //Goto the next line
        pspDebugScreenPuts("\n");

        //Increment scounter
        scounter++;
    }

    //Helper
    pspDebugScreenSetTextColor(0xFFFF8000);
    pspDebugScreenSetXY(0, 33);
    pspDebugScreenPuts(">< = Select; () = Cancel/Return to Game");
    */
}

static void drawSplashScreen() {
    /*
    pspDebugScreenSetTextColor(0xFFFFFFFF);
    pspDebugScreenPuts("[Initiation]");
    pspDebugScreenSetTextColor(0xFF808080);
    pspDebugScreenPuts(
            "\n--------------------------------------------------------------------");
    pspDebugScreenSetTextColor(0xFF0000FF);
    pspDebugScreenPuts(
            "Please tell nitePR when to generate OFF codes by pressing\nthe MUSICAL NOTE button while inside the game at least once\nand NOT when the cheat menu is showing!!!\n\n");
    pspDebugScreenSetTextColor(0xFF00FF00);
    pspDebugScreenPuts(
            "HINTs:\n - To take a screenshot, press DOWN + SQUARE\n\n - Use nitePRed.exe (it comes in the same zip as nitePR.prx) to\n   change the button assignments (such as the screenshot key combo)\n\n - Some cheats need the PRX->Cheat Hz option to be set to 15/1000\n   This is for cheats that overwrite a changing value\n\n - One can use CWCheat2NitePR.htm to convert CWCheat codes to\n   nitePR format without any issues. Look in the Doc folder\n   of the nitePR.zip file for it.\n\n");
    pspDebugScreenSetTextColor(0xFFFFFFFF);
    pspDebugScreenPuts("                           [PRESS START]\n\n");
    */
}

static void drawTabs() {
    /*
    //Draw the tabs
    pspDebugScreenSetTextColor(tabSelected == 0 ? 0xFFFFFFFF : 0xFF808080);
    pspDebugScreenPuts("[Cheater] ");
    pspDebugScreenSetTextColor(tabSelected == 1 ? 0xFFFFFFFF : 0xFF808080);
    pspDebugScreenPuts("[Searcher] ");
    pspDebugScreenSetTextColor(tabSelected == 2 ? 0xFFFFFFFF : 0xFF808080);
    pspDebugScreenPuts("[PRX] ");
    pspDebugScreenSetTextColor(tabSelected == 3 ? 0xFFFFFFFF : 0xFF808080);
    pspDebugScreenPuts("[Browser] ");
    pspDebugScreenSetTextColor(tabSelected == 4 ? 0xFFFFFFFF : 0xFF808080);
    pspDebugScreenPuts("[Decoder] ");
    pspDebugScreenSetTextColor(tabSelected == 5 ? 0xFFFFFFFF : 0xFF808080);
    pspDebugScreenPuts("[Logger] ");
    pspDebugScreenSetTextColor(tabSelected == 6 ? 0xFFFFFFFF : 0xFF808080);
    pspDebugScreenPuts("[Dasm] ");
    pspDebugScreenSetTextColor(0xFF808080);
    pspDebugScreenPuts(
            "\n--------------------------------------------------------------------");
     */
}

static void drawTracker() {
    /*
    unsigned int counter;

    pspDebugScreenSetTextColor(0xFF808080);
    pspDebugScreenPuts("  Mode              Address\n");

    //Apply the row color
    if (counter == trackY) {
        pspDebugScreenSetTextColor(0xFF8080BB);
    } else {
        pspDebugScreenSetTextColor(0xFFBBBBBB);
    }

    //Print out the mode
    pspDebugScreenPuts(trackModeName[trackMode]);

    //Print out the address
    sprintf(buffer, "0x%08lX", trackAddress - trackFormat);
    pspDebugScreenPuts(buffer);

    //Skip a line, draw the pointer =)
    pspDebugScreenPuts("\n");
    if (trackY == 0) {
        //Skip the desired amount?
        pspDebugScreenPuts("    ");
        if (trackC != 0) {
            //Skip mode?
            pspDebugScreenPuts("                  ");
        }

        //Skip the minimalist amount
        unsigned char tempCounter = trackX;
        while (tempCounter) {
            pspDebugScreenPuts(" ");
            tempCounter--;
        }

        //Draw the symbol (Finally!!)
        if (extSelected[3]) {
            pspDebugScreenSetTextColor(0xFF0000FF);
        } else {
            pspDebugScreenSetTextColor(0xFFFF0000);
        }
        pspDebugScreenPuts("^");
    }
    pspDebugScreenPuts("\n");

    //Draw misc menus
    pspDebugScreenSetTextColor(trackY == 1 ? 0xFF0000FF : 0xFF808080);
    pspDebugScreenPuts("  Enabled? ");
    if (trackStatus) {
        pspDebugScreenPuts("True\n");
    } else {
        pspDebugScreenPuts("False\n");
    }
    pspDebugScreenSetTextColor(trackY == 2 ? 0xFF0000FF : 0xFF808080);
    pspDebugScreenPuts("  Halt? ");
    if (trackPause) {
        pspDebugScreenPuts("True\n");
    } else {
        pspDebugScreenPuts("False\n");
    }

    if (trackMode == 0) {
        //Print out log
        pspDebugScreenSetTextColor(0xFFFFFFFF);
        pspDebugScreenPuts("\n[Register Log - UNIMPLEMENTED]");
        pspDebugScreenSetTextColor(0xFF808080);
        pspDebugScreenPuts(
                "\n--------------------------------------------------------------------");

        //Print out log
        //pspDebugScreenSetTextColor(0xFFBBBBBB);sprintf(buffer, "  zr = 0x%08lX        s0 = 0x%08lX\n", g_psplinkContext[0].regs.r[0], g_psplinkContext[0].regs.r[16]); pspDebugScreenPuts(buffer);
        //pspDebugScreenSetTextColor(0xFF8080BB);sprintf(buffer, "  at = 0x%08lX        s1 = 0x%08lX\n", g_psplinkContext[0].regs.r[1], g_psplinkContext[0].regs.r[17]); pspDebugScreenPuts(buffer);
        //pspDebugScreenSetTextColor(0xFFBBBBBB);sprintf(buffer, "  v0 = 0x%08lX        s2 = 0x%08lX\n", g_psplinkContext[0].regs.r[2], g_psplinkContext[0].regs.r[18]); pspDebugScreenPuts(buffer);
        //pspDebugScreenSetTextColor(0xFF8080BB);sprintf(buffer, "  v1 = 0x%08lX        s3 = 0x%08lX\n", g_psplinkContext[0].regs.r[3], g_psplinkContext[0].regs.r[19]); pspDebugScreenPuts(buffer);
        //pspDebugScreenSetTextColor(0xFFBBBBBB);sprintf(buffer, "  a0 = 0x%08lX        s4 = 0x%08lX\n", g_psplinkContext[0].regs.r[4], g_psplinkContext[0].regs.r[20]); pspDebugScreenPuts(buffer);
        //pspDebugScreenSetTextColor(0xFF8080BB);sprintf(buffer, "  a1 = 0x%08lX        s5 = 0x%08lX\n", g_psplinkContext[0].regs.r[5], g_psplinkContext[0].regs.r[21]); pspDebugScreenPuts(buffer);
        //pspDebugScreenSetTextColor(0xFFBBBBBB);sprintf(buffer, "  a2 = 0x%08lX        s6 = 0x%08lX\n", g_psplinkContext[0].regs.r[6], g_psplinkContext[0].regs.r[22]); pspDebugScreenPuts(buffer);
        //pspDebugScreenSetTextColor(0xFF8080BB);sprintf(buffer, "  a3 = 0x%08lX        s7 = 0x%08lX\n", g_psplinkContext[0].regs.r[7], g_psplinkContext[0].regs.r[23]); pspDebugScreenPuts(buffer);
        //pspDebugScreenSetTextColor(0xFFBBBBBB);sprintf(buffer, "  t0 = 0x%08lX        t8 = 0x%08lX\n", g_psplinkContext[0].regs.r[8], g_psplinkContext[0].regs.r[24]); pspDebugScreenPuts(buffer);
        //pspDebugScreenSetTextColor(0xFF8080BB);sprintf(buffer, "  t1 = 0x%08lX        t9 = 0x%08lX\n", g_psplinkContext[0].regs.r[9], g_psplinkContext[0].regs.r[25]); pspDebugScreenPuts(buffer);
        //pspDebugScreenSetTextColor(0xFFBBBBBB);sprintf(buffer, "  t2 = 0x%08lX        k0 = 0x%08lX\n", g_psplinkContext[0].regs.r[10], g_psplinkContext[0].regs.r[26]); pspDebugScreenPuts(buffer);
        //pspDebugScreenSetTextColor(0xFF8080BB);sprintf(buffer, "  t3 = 0x%08lX        k1 = 0x%08lX\n", g_psplinkContext[0].regs.r[11], g_psplinkContext[0].regs.r[27]); pspDebugScreenPuts(buffer);
        //pspDebugScreenSetTextColor(0xFFBBBBBB);sprintf(buffer, "  t4 = 0x%08lX        gp = 0x%08lX\n", g_psplinkContext[0].regs.r[12], g_psplinkContext[0].regs.r[28]); pspDebugScreenPuts(buffer);
        //pspDebugScreenSetTextColor(0xFF8080BB);sprintf(buffer, "  t5 = 0x%08lX        sp = 0x%08lX\n", g_psplinkContext[0].regs.r[13], g_psplinkContext[0].regs.r[29]); pspDebugScreenPuts(buffer);
        //pspDebugScreenSetTextColor(0xFFBBBBBB);sprintf(buffer, "  t6 = 0x%08lX        fp = 0x%08lX\n", g_psplinkContext[0].regs.r[14], g_psplinkContext[0].regs.r[30]); pspDebugScreenPuts(buffer);
        //pspDebugScreenSetTextColor(0xFF8080BB);sprintf(buffer, "  t7 = 0x%08lX        ra = 0x%08lX\n", g_psplinkContext[0].regs.r[15], g_psplinkContext[0].regs.r[31]); pspDebugScreenPuts(buffer);
    } else {
        //Print out log
        pspDebugScreenSetTextColor(0xFFFFFFFF);
        pspDebugScreenPuts("\n[Data Log - UNIMPLEMENTED]");
        pspDebugScreenSetTextColor(0xFF808080);
        pspDebugScreenPuts(
                "\n--------------------------------------------------------------------");
    }

    //Helper
    lineClear(33)
    ;
    pspDebugScreenSetTextColor(0xFFFF8000);
    if (trackY == 0) {
        pspDebugScreenPuts(
                ">< = Edit On/Off; \\|/ /|\\ <- -> = Cycle; () = Cancel");
    } else if (trackY == 3) {
        pspDebugScreenPuts(">< = Select; () = Cancel");
    } else {
        pspDebugScreenPuts(">< = Toggle; () = Cancel");
    }
     */
}

void buttonCallback(int curr, int last, void *arg) {
    int cvd = (curr & PSP_CTRL_VOLDOWN);
    int cvu = (curr & PSP_CTRL_VOLUP);
    int cno = (curr & PSP_CTRL_NOTE);
    int lvd = (last & PSP_CTRL_VOLDOWN);
    int lvu = (last & PSP_CTRL_VOLUP);
    int lno = (last & PSP_CTRL_NOTE);

    if (krUi.vram == NULL) {
        return;
    }

    /* If the UI is already showing, delegate to the ui. */
    if (krUi.running == 1) {
        CEFiveUi* prUi = &krUi;
        cefiveui_buttoncallback(curr, last, prUi);
        return;
    }

    /* If the Volume Up and Volume Down keys are being held */
    if (cvd && cvu && (!lvd || !lvu)) {
        krUi.drawn = 0;
        krRunState = CES_UIRequest;
    }

    /* If the Music Button has been pressed. */
    if (cno && !lno) {
        cheatStatus = !cheatStatus;
        if (krCheatEngine.trigger_active == 0) {
            cheatengineActivateCheats(&krCheatEngine);
        } else {
            cheatengineDeactivateCheats(&krCheatEngine);
        }
        sceKernelDelayThread(500000);
    }
}

static void computeCheats() {
    unsigned int counter;
    unsigned int scounter;
    unsigned int address;
    Cheat* prCheat = NULL;
    Block* prBlock = NULL;

    //Backup all the cheat "blocks"
    if (!cheatSaved) {
        counter = 0;
        scounter = 0;
        while (counter < blockTotal) {
            prCheat = &cheat[scounter];
            prBlock = &block[counter];
            if (prCheat->block == counter) {
                cheatDMA = 0; //Reset DNA on every new cheat
                scounter++;
            }

            // This part appears to recompute the DMA of all DMA cheats
            // and re-record the stdVal of every cheat.
            if (prBlock->flags & BLOCK_FLAG_DMA) {
                if (prBlock->hakVal != 0xFFFFFFFF) {
                    cheatDMA = *((unsigned int*) (0x08800000
                            + (prBlock->hakVal & 0x1FFFFFF)))
                            - 0x08800000;

                    if (((cheatDMA >= 0x00004000)
                            && (cheatDMA < 0x01800000)) || ((cheatDMA
                            >= 0x40004000) && (cheatDMA < 0x41800000))) {
                        prBlock->stdVal = cheatDMA;
                    }
                } else {
                    cheatDMA = 0;
                }
            } else {
                address = cheatDMA + prBlock->address;

                if (((address >= 0x08804000) && (address < 0x0A000000))
                        || ((address >= 0x48804000) && (address
                                < 0x4A000000))) {
                    switch (prBlock->flags & BLOCK_FLAG_DWORD) {
                    case BLOCK_FLAG_DWORD:
                        if (address % 4 == 0) {
                            prBlock->stdVal = *((unsigned int*) (address));
                        }
                        break;
                    case BLOCK_FLAG_WORD:
                        if (address % 2 == 0) {
                            prBlock->stdVal = *((unsigned short*) (address));
                        }
                        break;
                    case BLOCK_FLAG_BYTE:
                        prBlock->stdVal = *((unsigned char*) (address));
                        break;
                    }
                }
            }
            counter++;
        }
    }
}

static void freshenCheats() {
    cheatengineRefresh(&krCheatEngine);
}

static void freshenCheatsX() {
    Cheat* prCheat = NULL;
    //Apply the cheats accordingly (make em fresh)
    int counter = 0;
    while (counter < cheatTotal) {
        prCheat = &cheat[counter];
        if ((prCheat->flags & CHEAT_FLAG_CONSTANT) && (!cheatSaved)) {
            prCheat->flags |= CHEAT_FLAG_FRESH;
            cheatEnable(counter);
            prCheat->flags &= ~CHEAT_FLAG_FRESH;
        }
        if (prCheat->flags & CHEAT_FLAG_SELECTED) {
            prCheat->flags |= CHEAT_FLAG_FRESH;
            if (cheatStatus) {
                cheatEnable(counter);
            } else {
                cheatDisable(counter);
            }
            prCheat->flags &= ~CHEAT_FLAG_FRESH;
        }
        counter++;
    }
    cheatSaved = 1;
}

static void gamePause(SceUID thid) {
    if (pauseuid >= 0)
        return;
    pauseuid = thid;
    sceKernelGetThreadmanIdList(SCE_KERNEL_TMID_Thread, thread_buf_now,
            MAX_THREAD, &thread_count_now);
    int x, y, match;
    for (x = 0; x < thread_count_now; x++) {
        match = 0;
        SceUID tmp_thid = thread_buf_now[x];
        for (y = 0; y < thread_count_start; y++) {
            if ((tmp_thid == thread_buf_start[y]) || (tmp_thid == thid)) {
                match = 1;
                break;
            }
        }
        if (match == 0)
            sceKernelSuspendThread(tmp_thid);
    }
}

static void gameResume(SceUID thid) {
    if (pauseuid != thid)
        return;
    pauseuid = -1;
    int x, y, match;
    for (x = 0; x < thread_count_now; x++) {
        match = 0;
        SceUID tmp_thid = thread_buf_now[x];
        for (y = 0; y < thread_count_start; y++) {
            if ((tmp_thid == thread_buf_start[y]) || (tmp_thid == thid)) {
                match = 1;
                break;
            }
        }
        if (match == 0)
            sceKernelResumeThread(tmp_thid);
    }
}

void menuDraw() {
    cefiveuiRedraw(&krUi);
}

void menuDrawX() {
    /*
    unsigned int counter;
    unsigned int scounter;
    unsigned int convBase;
    unsigned int convTotal;

    //Draw the menu
    pspDebugScreenSetXY(0, 0);
    pspDebugScreenSetTextColor(0xFFFFFFFF);
    pspDebugScreenPuts("NitePR Rev. G5\nBased on Sanik's NitePR Rev. J\n");
    pspDebugScreenSetTextColor(0xFF00FFFF);
    pspDebugScreenPuts("http://sanik.imk.cx/\n");

    if (cheatStatus) {
        pspDebugScreenSetTextColor(0xFF00FF00);
        pspDebugScreenPuts("[CHEATS ARE ON]\n\n");
    } else {
        pspDebugScreenSetTextColor(0xFF0000FF);
        if (!cheatSaved) {
            pspDebugScreenPuts("[ALL CHEATS ARE OFF]\n\n");
        } else {
            pspDebugScreenPuts("[CHEATS ARE OFF]\n\n");
        }
    }

    //Draw the logo!
    telazornDraw(vram);

    //User friendly note
    if (lolInit) {
        drawSplashScreen();
        return;
    }

    //Extended/sub menus?
    if (extMenu) {
        switch (extMenu) {
        case 1: //DRAW EXT CHEAT
            cheateditorSelectCheat(&krCheatEditor, cheatpanelGetSelectedIndex(&krCheatPanel));
            cheateditorRedraw(&krCheatEditor);
            break;

        case 2: //DRAW EXT SEARCH
            drawExtSearch();
            break;

        case 3: //DRAW EXT DIFF SEARCH
            drawExtDiffSearch();
            break;

        case 4: //DRAW EXT TEXT search
            drawExtTextSearch();
            break;
        }
    } else {
        drawTabs();
        //Draw the options for the respective tab
        switch (tabSelected) {
        case 0: //DRAW CHEATER
            krCheatPanel.cheat_count = cheatTotal;
            cheatpanelRedraw(&krCheatPanel);
            cheatSelected = cheatpanelGetSelectedIndex(&krCheatPanel);
            break;

        case 1: //DRAW SEARCHER
            drawSearcher();
            break;

        case 2: //DRAW PRX
            drawPrx();
            break;

        case 3: //DRAW BROWSER
            browserRedraw();
            break;
        case 4: //DRAW DECODER
            decoderRedraw();
            break;
        case 5: //Draw tracker
            drawTracker();
            break;
        case 6: // Draw Disassembler
            disassemblerRedraw(&krDisassembler);
            break;
        }
    }

    if (copyMenu) {
        drawCopyMenu();
    }

    //Take a picture!!!
    /*if(pad.Buttons & PSP_CTRL_START)
     {
     sprintf(buffer, "ms0:/vram%d.raw", dumpNo);

     int fd;
     fd=sceIoOpen(buffer, PSP_O_WRONLY | PSP_O_CREAT, 0777);
     if(fd>0)
     {
     sceIoWrite(fd, vram, 512*272*2);
     sceIoClose(fd);
     }

     dumpNo++;
     }*/
}

int pollFrameBuffer() {
    unsigned int address = 0;
    int bufferwidth = 0;
    int pixelformat = 0;
    int r = 0;

    r = sceDisplayGetFrameBufferInternal(0, &address, &bufferwidth,
            &pixelformat, PSP_DISPLAY_SETBUF_IMMEDIATE);
    if (address == 0) {
        krUi.running = 0;
        krUi.vram = NULL;
    }
    return r;
}

void menuInput() {
    int handled = 0;
    unsigned int a_address = 0;
    unsigned int a_bufferWidth = 0;
    unsigned int a_pixelFormat = 0;
    unsigned int a_sync;

    krUi.running = 1;
    while (krUi.running == 1) {
        if (pollFrameBuffer() == 0) {
            handled = cefiveuiHandleInput(&krUi);
            cefiveuiRedraw(&krUi);
            searchengine_run(&krSearchEngine);
        } else {
            break;
        }
        sceKernelDelayThread(50000);
    }
    menuDrawn = 0;
}

void menuInputX() {
    /*
    int fd;
    int fd2;
    unsigned int counter = 0;
    unsigned int scounter = 0;
    unsigned int dcounter = 0;
    unsigned int padButtons;
    unsigned char miscType = 0;
    pad.Buttons = 0;
    int trigger = 0;
    menuDraw();

    //Loop for input
    while (1) {
        padButtons = pad.Buttons;
        sceCtrlPeekBufferPositive(&pad, 1);

        //Has the HOME button screwed up the VRAM blocks?
        unsigned int a_address = 0;
        unsigned int a_bufferWidth = 0;
        unsigned int a_pixelFormat = 0;
        unsigned int a_sync;

        sceDisplayGetFrameBufferInternal(0, &a_address, &a_bufferWidth,
                &a_pixelFormat, &a_sync);

        if (a_address == 0) {
            //Stop nitePR
            menuDrawn = 0;
            return;
        }

        //The 1st priority menu
        if (copyMenu) {
            if (pad.Buttons & PSP_CTRL_UP) {
                if (copyMenu != 1)
                    copyMenu = 1;
                menuDraw();
                sceKernelDelayThread(150000);
            } else if (pad.Buttons & PSP_CTRL_DOWN) {
                if (copyMenu != 2)
                    copyMenu = 2;
                menuDraw();
                sceKernelDelayThread(150000);
            }
            if (pad.Buttons & PSP_CTRL_CROSS) {
                if (copyMenu == 1) //Copy
                {
                    if (extMenu) {
                        if (extMenu == 1) {
                            copyData = block[extSelected[0]].address;
                        } else if (extMenu == 2) {
                            if (extSelected[0] > 2) {
                                copyData = searchAddress[extSelected[0] - 3]
                                        - 0x40000000;
                            }
                        } else if (extMenu == 3) {
                            if (extSelected[0] > 2) {
                                copyData = searchAddress[extSelected[0] - 3]
                                        - 0x40000000;
                            }
                        } else if (extMenu == 4) {
                            if (extSelected[0] > 1) {
                                copyData = searchAddress[extSelected[0] - 2]
                                        - 0x40000000;
                            }
                        }
                    } else {
                        if (tabSelected == 3) {
                            copyData = browseAddress + (browseY * browseLines);
                            copyData -= 0x40000000;
                        } else if (tabSelected == 4) {
                            copyData = decodeAddress + (decodeY * 4);
                            copyData -= 0x40000000;
                        } else if (tabSelected == 5) {
                            copyData = trackAddress;
                            copyData -= 0x40000000;
                        }
                    }
                    copyData &= 0xFFFFFFFC;

                    if (copyData < 0x08804000) {
                        copyData = 0x08804000;
                    }
                } else //Paste
                {
                    if (extMenu) {
                        if (extMenu == 1) {
                            if (!(block[extSelected[0]].flags & BLOCK_FLAG_DMA))
                                block[extSelected[0]].address = copyData;
                        }
                    } else {
                        if (tabSelected == 3) {
                            browseAddress = copyData | 0x40000000; //(browseY * browseLines)+
                            if (browseAddress > (0x4A000000
                                    - (22 * browseLines))) {
                                browseAddress = (0x4A000000
                                        - (22 * browseLines));
                            }
                        } else if (tabSelected == 4) {
                            decodeAddress = copyData | 0x40000000; //+(decodeY*4);
                            if (decodeAddress > 0x49FFFFA8) {
                                decodeAddress = 0x49FFFFA8;
                            }
                        } else if (tabSelected == 5) {
                            trackAddress = copyData + 0x40000000;
                        }
                    }
                }
                goto hideCopyMenu;
            }
            if ((padButtons & PSP_CTRL_CIRCLE) && !(pad.Buttons
                    & PSP_CTRL_CIRCLE)) {
                hideCopyMenu: pspDebugScreenInitEx(vram, 0, 0);
                copyMenu = 0;
                menuDraw();
                sceKernelDelayThread(150000);
            }
        } else if (extMenu) //Do we use extended menus?
        {
            switch (extMenu) {
            case 1: //INPUT EXT CHEAT
                if (pad.Buttons & PSP_CTRL_SELECT) {
                    copyMenu = 1;
                    menuDraw();
                    sceKernelDelayThread(150000);
                }
                if (pad.Buttons & PSP_CTRL_CROSS) {
                    extSelected[3] = !extSelected[3];
                    cheateditorCrossButton(&krCheatEditor);
                    menuDraw();
                    sceKernelDelayThread(150000);
                }
                if (pad.Buttons & PSP_CTRL_SQUARE) {
                    cheateditorCrossButton(&krCheatEditor);
                    menuDraw();
                    sceKernelDelayThread(150000);
                }

                if (extSelected[3]) {
                    if (pad.Buttons & PSP_CTRL_UP) {
                        cheateditorDpadUp(&krCheatEditor);
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else if (pad.Buttons & PSP_CTRL_DOWN) {
                        cheateditorDpadDown(&krCheatEditor);
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else {
                        cheatButtonAgeY = 0;
                    }
                } else {
                    if (pad.Buttons & PSP_CTRL_UP) {
                        cheateditorDpadUp(&krCheatEditor);
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else if (pad.Buttons & PSP_CTRL_DOWN) {
                        cheateditorDpadDown(&krCheatEditor);
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else {
                        cheatButtonAgeY = 0;
                    }
                }

                if (pad.Buttons & PSP_CTRL_LEFT) {
                    cheateditorDpadLeft(&krCheatEditor);
                    menuDraw();
                    if (cheatButtonAgeX < 12)
                        cheatButtonAgeX++;
                    sceKernelDelayThread(150000 - (10000 * cheatButtonAgeX));
                } else if (pad.Buttons & PSP_CTRL_RIGHT) {
                    cheateditorDpadRight(&krCheatEditor);
                    menuDraw();
                    if (cheatButtonAgeX < 12)
                        cheatButtonAgeX++;
                    sceKernelDelayThread(150000 - (10000 * cheatButtonAgeX));
                } else {
                    cheatButtonAgeX = 0;
                }

                if ((padButtons & PSP_CTRL_CIRCLE) && !(pad.Buttons
                        & PSP_CTRL_CIRCLE)) {
                    if (extSelected[3]) {
                        extSelected[3] = 0;
                        menuDraw();
                        sceKernelDelayThread(150000);
                    } else {
                        pspDebugScreenInitEx(vram, 0, 0);
                        extSelected[0] = extSelected[1] = extSelected[2]
                                = extSelected[3] = 0;
                        extMenu = 0;
                        if (extOpt) {
                            extSelected[0] = extOptArg;
                            pspDebugScreenInitEx(vram, 0, 0);
                            extSelected[1] = extSelected[2] = extSelected[3]
                                    = 0;
                            extMenu = 1 + extOpt;
                            cheatSearch = 1;
                        }
                        extOpt = 0;
                        menuDraw();
                        sceKernelDelayThread(150000);
                        break;
                    }
                }
                break;

            case 2: //INPUT EXT SEARCH
                if (pad.Buttons & PSP_CTRL_SELECT) {
                    copyMenu = 1;
                    menuDraw();
                    sceKernelDelayThread(150000);
                }
                if (pad.Buttons & PSP_CTRL_SQUARE) {
                    if (searchNo == 0) //Don't allow the user to change type later on!!!
                    {
                        switch (searchHistory[0].flags & BLOCK_FLAG_DWORD) {
                        case BLOCK_FLAG_BYTE:
                            searchHistory[0].flags = (searchHistory[0].flags
                                    & ~BLOCK_FLAG_DWORD) | BLOCK_FLAG_WORD;
                            break;
                        case BLOCK_FLAG_WORD:
                            searchHistory[0].flags = (searchHistory[0].flags
                                    & ~BLOCK_FLAG_DWORD) | BLOCK_FLAG_DWORD;
                            break;
                        case BLOCK_FLAG_DWORD:
                            searchHistory[0].flags = (searchHistory[0].flags
                                    & ~BLOCK_FLAG_DWORD) | BLOCK_FLAG_BYTE;
                            break;
                        }
                    }
                    menuDraw();
                    sceKernelDelayThread(150000);
                }
                if (pad.Buttons & PSP_CTRL_CROSS) {
                    if (extSelected[0] == 0) {
                        extSelected[3] = !extSelected[3];
                    } else if (extSelected[0] == 1) {
                        //Update the search history!
                        if (searchHistoryCounter < 15) {
                            searchHistoryCounter++;
                        }
                        memmove(&searchHistory[1], &searchHistory[0],
                                sizeof(Block) * (15));
                        searchHistory[1].stdVal = 6;

                        //Move the cursor back
                        extSelected[0] = 0;

                        //Is it the first search?
                        if (searchNo == 0) {
                            //Increment the search
                            searchNo++;
                            searchMax++;

                            //Setup the variables
                            searchResultCounter = 0;

                            //Open the file
                            sprintf(buffer, "ms0:/search%d.dat", searchNo);
                            fd = sceIoOpen(buffer, PSP_O_WRONLY | PSP_O_CREAT,
                                    0777);
                            if (fd > 0) {
                                //Write out the searcHistory[0] type
                                switch (searchHistory[0].flags & BLOCK_FLAG_DWORD) {
                                case BLOCK_FLAG_DWORD:
                                    if (sceIoWrite(fd, "4", 1) != 1)
                                        goto ErrorReadExactA;
                                    break;
                                case BLOCK_FLAG_WORD:
                                    if (sceIoWrite(fd, "2", 1) != 1)
                                        goto ErrorReadExactA;
                                    break;
                                case BLOCK_FLAG_BYTE:
                                    if (sceIoWrite(fd, "1", 1) != 1)
                                        goto ErrorReadExactA;
                                    break;
                                }

                                //Search!
                                counter = 0x48804000;

                                //Helper
                                while (counter < 0x4A000000) {
                                    //Helper
                                    if (!((counter - 0x48804000) & 0xFFFF)) {
                                        if (!cheatPause)
                                            sceKernelDelayThread(1500);

                                        lineClear(33);
                                        pspDebugScreenSetTextColor(0xFFFF8000);
                                        sprintf(
                                                buffer,
                                                "Task = %02d%%; () = Hold to Abort",
                                                (counter - 0x48804000)
                                                        / ((0x4A000000
                                                                - 0x48804000)
                                                                / 100));
                                        pspDebugScreenPuts(buffer);

                                        sceCtrlPeekBufferPositive(&pad, 1);

                                        if (pad.Buttons & PSP_CTRL_CIRCLE) {
                                            lineClear(33);
                                            pspDebugScreenSetTextColor(
                                                    0xFFFF8000);
                                            pspDebugScreenPuts(
                                                    "Task Aborted!!!");

                                            do {
                                                sceKernelDelayThread(150000);
                                                sceCtrlPeekBufferPositive(&pad,
                                                        1);
                                            } while (pad.Buttons
                                                    & PSP_CTRL_CIRCLE);
                                            break;
                                        }
                                    }

                                    //Check
                                    switch (searchHistory[0].flags & BLOCK_FLAG_DWORD) {
                                    case BLOCK_FLAG_DWORD:
                                        if (*((unsigned int*) (counter))
                                                == (unsigned int) searchHistory[0].hakVal) {
                                            //Add it
                                            if (sceIoWrite(fd, &counter,
                                                    sizeof(unsigned int)) != 4)
                                                goto ErrorReadExactA;
                                            if (sceIoWrite(fd,
                                                    &searchHistory[0].hakVal,
                                                    sizeof(unsigned int)) != 4)
                                                goto ErrorReadExactA;
                                            searchResultCounter++;
                                        }
                                        counter += 4;
                                        break;

                                    case BLOCK_FLAG_WORD:
                                        if (*((unsigned short*) (counter))
                                                == (unsigned short) searchHistory[0].hakVal) {
                                            //Add it
                                            if (sceIoWrite(fd, &counter,
                                                    sizeof(unsigned int)) != 4)
                                                goto ErrorReadExactA;
                                            if (sceIoWrite(fd,
                                                    &searchHistory[0].hakVal,
                                                    sizeof(unsigned short))
                                                    != 2)
                                                goto ErrorReadExactA;
                                            searchResultCounter++;
                                        }
                                        counter += 2;
                                        break;

                                    case BLOCK_FLAG_BYTE:
                                        if (*((unsigned char*) (counter))
                                                == (unsigned char) searchHistory[0].hakVal) {
                                            //Add it
                                            if (sceIoWrite(fd, &counter,
                                                    sizeof(unsigned int)) != 4)
                                                goto ErrorReadExactA;
                                            if (sceIoWrite(fd,
                                                    &searchHistory[0].hakVal,
                                                    sizeof(unsigned char)) != 1)
                                                goto ErrorReadExactA;
                                            searchResultCounter++;
                                        }
                                        counter++;
                                        break;
                                    }

                                }
                                //Close the file since we are done with the search
                                sceIoClose(fd);

                                while (1) {
                                    break;
                                    //ReadShort
                                    ErrorReadExactA: sceIoClose(fd);
                                    if (searchNo > 0)
                                        searchNo--;
                                    sceIoRemove(buffer);

                                    lineClear(33);
                                    pspDebugScreenSetTextColor(0xFF0000FF);
                                    pspDebugScreenPuts(
                                            "ERROR: MemoryStick out of Space!");
                                    sceKernelDelayThread(3000000);
                                    break;
                                }
                            } else {
                                //ERROR - file couldn't be opened - undo search attempt
                                if (searchNo > 0)
                                    searchNo--;
                            }
                        } else //Continue the search with a different exact number
                        {
                            //Increment the search
                            searchNo++;
                            searchMax++;

                            //Open the files
                            sprintf(buffer, "ms0:/search%d.dat", searchNo - 1);
                            fd = sceIoOpen(buffer, PSP_O_RDONLY, 0777);
                            sprintf(buffer, "ms0:/search%d.dat", searchNo);
                            fd2 = sceIoOpen(buffer, PSP_O_WRONLY | PSP_O_CREAT,
                                    0777);
                            if ((fd > 0) && (fd2 > 0)) {
                                //Get ready to go through and check the addresses
                                sceIoLseek(fd, 1, SEEK_SET);

                                //Write out the searcHistory[0] type
                                switch (searchHistory[0].flags & BLOCK_FLAG_DWORD) {
                                case BLOCK_FLAG_DWORD:
                                    if (sceIoWrite(fd2, "4", 1) != 1)
                                        goto ErrorReadExactB;
                                    break;
                                case BLOCK_FLAG_WORD:
                                    if (sceIoWrite(fd2, "2", 1) != 1)
                                        goto ErrorReadExactB;
                                    break;
                                case BLOCK_FLAG_BYTE:
                                    if (sceIoWrite(fd2, "1", 1) != 1)
                                        goto ErrorReadExactB;
                                    break;
                                }

                                //Loop through the list checking each one
                                counter = searchResultCounter;
                                searchResultCounter = 0;
                                while (counter > 0) {
                                    //Load it
                                    sceIoRead(fd, &scounter,
                                            sizeof(unsigned int));

                                    //Helper
                                    if (!(counter & 0x3F)) {
                                        if (!cheatPause)
                                            sceKernelDelayThread(1500);

                                        lineClear(33);
                                        pspDebugScreenSetTextColor(0xFFFF8000);
                                        sprintf(
                                                buffer,
                                                "Task = %02d%%; () = Hold to Abort",
                                                (scounter - 0x48804000)
                                                        / ((0x4A000000
                                                                - 0x48804000)
                                                                / 100));
                                        pspDebugScreenPuts(buffer);

                                        sceCtrlPeekBufferPositive(&pad, 1);

                                        if (pad.Buttons & PSP_CTRL_CIRCLE) {
                                            lineClear(33);
                                            pspDebugScreenSetTextColor(
                                                    0xFFFF8000);
                                            pspDebugScreenPuts(
                                                    "Task Aborted!!!");

                                            do {
                                                sceKernelDelayThread(150000);
                                                sceCtrlPeekBufferPositive(&pad,
                                                        1);
                                            } while (pad.Buttons
                                                    & PSP_CTRL_CIRCLE);
                                            break;
                                        }
                                    }

                                    //Check
                                    switch (searchHistory[0].flags & BLOCK_FLAG_DWORD) {
                                    case BLOCK_FLAG_DWORD:
                                        sceIoLseek(fd, 4, SEEK_CUR);
                                        if (*((unsigned int*) (scounter))
                                                == (unsigned int) searchHistory[0].hakVal) {
                                            //Add it
                                            if (sceIoWrite(fd2, &scounter,
                                                    sizeof(unsigned int)) != 4)
                                                goto ErrorReadExactB;
                                            if (sceIoWrite(fd2,
                                                    &searchHistory[0].hakVal,
                                                    sizeof(unsigned int)) != 4)
                                                goto ErrorReadExactB;
                                            searchResultCounter++;
                                        }
                                        break;

                                    case BLOCK_FLAG_WORD:
                                        sceIoLseek(fd, 2, SEEK_CUR);
                                        if (*((unsigned short*) (scounter))
                                                == (unsigned short) searchHistory[0].hakVal) {
                                            //Add it
                                            if (sceIoWrite(fd2, &scounter,
                                                    sizeof(unsigned int)) != 4)
                                                goto ErrorReadExactB;
                                            if (sceIoWrite(fd2,
                                                    &searchHistory[0].hakVal,
                                                    sizeof(unsigned short))
                                                    != 2)
                                                goto ErrorReadExactB;
                                            searchResultCounter++;
                                        }
                                        break;

                                    case BLOCK_FLAG_BYTE:
                                        sceIoLseek(fd, 1, SEEK_CUR);
                                        if (*((unsigned char*) (scounter))
                                                == (unsigned char) searchHistory[0].hakVal) {
                                            //Add it
                                            if (sceIoWrite(fd2, &scounter,
                                                    sizeof(unsigned int)) != 4)
                                                goto ErrorReadExactB;
                                            if (sceIoWrite(fd2,
                                                    &searchHistory[0].hakVal,
                                                    sizeof(unsigned char)) != 1)
                                                goto ErrorReadExactB;
                                            searchResultCounter++;
                                        }
                                        break;
                                    }

                                    //Next
                                    counter--;
                                }

                                //Close the files
                                sceIoClose(fd);
                                sceIoClose(fd2);

                                while (1) {
                                    break;
                                    //ReadShort
                                    ErrorReadExactB: sceIoClose(fd);
                                    sceIoClose(fd2);
                                    if (searchNo > 0)
                                        searchNo--;
                                    sceIoRemove(buffer);

                                    lineClear(33);
                                    pspDebugScreenSetTextColor(0xFF0000FF);
                                    pspDebugScreenPuts(
                                            "ERROR: MemoryStick out of Space!");
                                    sceKernelDelayThread(3000000);
                                    break;
                                }
                            } else {
                                //ERROR - files couldn't be opened - undo search attempt
                                sceIoClose(fd);
                                sceIoClose(fd2);
                                if (searchNo > 0)
                                    searchNo--;
                            }
                        }
                    } else if (extSelected[0] == 2) {
                        //Reset the search
                        if (searchNo > 0)
                            searchNo--;

                        //Move the cursor back
                        extSelected[0] = 0;
                    } else {
                        //Add the single cheat
                        switch (searchHistory[0].flags & BLOCK_FLAG_DWORD) {
                        case BLOCK_FLAG_DWORD:
                            cheatNew(
                                    4,
                                    searchAddress[extSelected[0] - 3]
                                            - 0x48800000,
                                    *((unsigned int*) (searchAddress[extSelected[0]
                                            - 3])));
                            break;
                        case BLOCK_FLAG_WORD:
                            cheatNew(
                                    2,
                                    searchAddress[extSelected[0] - 3]
                                            - 0x48800000,
                                    *((unsigned short*) (searchAddress[extSelected[0]
                                            - 3])));
                            break;
                        case BLOCK_FLAG_BYTE:
                            cheatNew(
                                    1,
                                    searchAddress[extSelected[0] - 3]
                                            - 0x48800000,
                                    *((unsigned char*) (searchAddress[extSelected[0]
                                            - 3])));
                            break;
                        default:
                            break;
                        }

                        //Switch to the cheat editor
                        extOptArg = extSelected[0];
                        pspDebugScreenInitEx(vram, 0, 0);
                        extSelected[1] = extSelected[2] = extSelected[3] = 0;
                        extSelected[0] = cheat[cheatTotal - 1].block;
                        cheatSelected = cheatTotal - 1;
                        extMenu = 1;
                        extOpt = 1;
                        menuDraw();
                        sceKernelDelayThread(150000);
                    }

                    //Load the file again, get the sample numbers
                    if (searchNo > 0) {
                        //Open the file
                        sprintf(buffer, "ms0:/search%d.dat", searchNo);
                        fd = sceIoOpen(buffer, PSP_O_RDONLY, 0777);
                        if (fd > 0) {
                            //Get the value size
                            sceIoRead(fd, &miscType, 1);
                            miscType -= '0';

                            switch (miscType) {
                            case 1:
                                searchHistory[0].flags
                                        = (searchHistory[0].flags & ~BLOCK_FLAG_DWORD)
                                                | BLOCK_FLAG_BYTE;
                                break;
                            case 2:
                                searchHistory[0].flags
                                        = (searchHistory[0].flags & ~BLOCK_FLAG_DWORD)
                                                | BLOCK_FLAG_WORD;
                                break;
                            case 4:
                                searchHistory[0].flags
                                        = (searchHistory[0].flags & ~BLOCK_FLAG_DWORD)
                                                | BLOCK_FLAG_DWORD;
                                break;
                            }

                            //Get the file size
                            searchResultCounter = sceIoLseek(fd, 0, SEEK_END);
                            sceIoLseek(fd, 1, SEEK_SET);
                            searchResultCounter--;
                            searchResultCounter /= (sizeof(unsigned int)
                                    + miscType);

                            //Only load the first 100
                            if (searchResultCounter > 100) {
                                for (scounter = 0; scounter < 100; scounter++) {
                                    sceIoRead(fd, &searchAddress[scounter],
                                            sizeof(unsigned int));
                                    sceIoLseek(fd, miscType, SEEK_CUR);
                                }
                            } else {
                                for (scounter = 0; scounter
                                        < searchResultCounter; scounter++) {
                                    sceIoRead(fd, &searchAddress[scounter],
                                            sizeof(unsigned int));
                                    sceIoLseek(fd, miscType, SEEK_CUR);
                                }
                            }

                            //Close the file since we are done with the search
                            sceIoClose(fd);
                        }
                    } else {
                        searchResultCounter = 0;
                    }

                    pspDebugScreenInitEx(vram, 0, 0);
                    menuDraw();
                    sceKernelDelayThread(150000);
                }

                if (extSelected[3]) {
                    if (pad.Buttons & PSP_CTRL_UP) {
                        switch (extSelected[1]) {
                        case 0:
                            searchHistory[0].hakVal += (1 << (4 * (7
                                    - extSelected[2])));
                            break;
                        case 1:
                            searchHistory[0].hakVal += decDelta[extSelected[2]];
                            break;
                        case 2:
                            searchHistory[0].hakVal += (1 << (8
                                    * (extSelected[2])));
                            break;
                        }
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else if (pad.Buttons & PSP_CTRL_DOWN) {
                        switch (extSelected[1]) {
                        case 0:
                            searchHistory[0].hakVal -= (1 << (4 * (7
                                    - extSelected[2])));
                            break;
                        case 1:
                            searchHistory[0].hakVal -= decDelta[extSelected[2]];
                            break;
                        case 2:
                            searchHistory[0].hakVal -= (1 << (8
                                    * (extSelected[2])));
                            break;
                        }
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else {
                        cheatButtonAgeY = 0;
                    }
                } else {
                    if (pad.Buttons & PSP_CTRL_UP) {
                        if (extSelected[0] > 0) {
                            extSelected[0]--;
                        }
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else if (pad.Buttons & PSP_CTRL_DOWN) {
                        if (extSelected[0] < (1 + (!!searchNo)
                                + (searchResultCounter > 100 ? 100
                                        : searchResultCounter))) {
                            extSelected[0]++;
                        }
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else {
                        cheatButtonAgeY = 0;
                    }
                }

                if (extSelected[0] == 0) {
                    if (pad.Buttons & PSP_CTRL_LEFT) {
                        extSelected[2]--;
                        switch (extSelected[1]) {
                        case 0:
                            if ((signed) extSelected[2] == -1) {
                                extSelected[2] = 0;
                            }
                            break;
                        case 1:
                            if ((signed) extSelected[2] == -1) {
                                extSelected[2] = 7;
                                extSelected[1]--;
                            }
                            break;
                        case 2:
                            if ((signed) extSelected[2] == -1) {
                                extSelected[2] = 9;
                                extSelected[1]--;
                            }
                            break;
                        }
                        menuDraw();
                        if (cheatButtonAgeX < 12)
                            cheatButtonAgeX++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeX));
                    } else if (pad.Buttons & PSP_CTRL_RIGHT) {
                        extSelected[2]++;
                        switch (extSelected[1]) {
                        case 0:
                            if (extSelected[2] > 7) {
                                extSelected[2] = 0;
                                extSelected[1]++;
                            }
                            break;
                        case 1:
                            if (extSelected[2] > 9) {
                                extSelected[2] = 0;
                                extSelected[1]++;
                            }
                            break;
                        case 2:
                            if (extSelected[2] > 3) {
                                extSelected[2] = 3;
                            }
                            break;
                        }
                        menuDraw();
                        if (cheatButtonAgeX < 12)
                            cheatButtonAgeX++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeX));
                    } else {
                        cheatButtonAgeX = 0;
                    }
                }

                if ((padButtons & PSP_CTRL_CIRCLE) && !(pad.Buttons
                        & PSP_CTRL_CIRCLE)) {
                    if (extSelected[3]) {
                        extSelected[3] = 0;
                        menuDraw();
                        sceKernelDelayThread(150000);
                    } else //Search has been aborted!
                    {
                        //Do we need to act like the search didn't even happen???
                        if (searchNo == 0) {
                            cheatSearch = 0;
                        }

                        //Go back
                        cheatSelected = 0;
                        pspDebugScreenInitEx(vram, 0, 0);
                        extSelected[0] = extSelected[1] = extSelected[2]
                                = extSelected[3] = 0;
                        extMenu = 0;
                        extOpt = 0;
                        menuDraw();
                        sceKernelDelayThread(150000);
                        break;
                    }
                }
                break;

            case 3: //INPUT EXT DIFF SEARCH
                if (pad.Buttons & PSP_CTRL_SELECT) {
                    copyMenu = 1;
                    menuDraw();
                    sceKernelDelayThread(150000);
                }
                if (pad.Buttons & PSP_CTRL_CROSS) {
                    if (extSelected[0] == 0) {
                        extSelected[3] = !extSelected[3];
                    } else if (extSelected[0] == 1) {
                        //Update the search history!
                        if (searchHistoryCounter < 15) {
                            searchHistoryCounter++;
                        }
                        memmove(&searchHistory[1], &searchHistory[0],
                                sizeof(Block) * (15));
                        searchHistory[1].stdVal = searchMode;

                        //Move the cursor back
                        extSelected[0] = 0;

                        //Is it the first search?
                        if (searchNo == 0) {
                            //Increment the search
                            searchNo++;
                            searchMax++;

                            //Setup the variables
                            searchResultCounter = 0;

                            //Open the files
                            fd = sceIoOpen("ms0:/search.ram", PSP_O_RDONLY,
                                    0777);
                            sprintf(buffer, "ms0:/search%d.dat", searchNo);
                            fd2 = sceIoOpen(buffer, PSP_O_WRONLY | PSP_O_CREAT,
                                    0777);
                            if ((fd > 0) && (fd2 > 0)) {
                                //Skip the initial 0x4000 bytes
                                sceIoLseek(fd, 0x4000, SEEK_SET);

                                //Write out the searcHistory[0] type
                                switch (searchHistory[0].flags & BLOCK_FLAG_DWORD) {
                                case BLOCK_FLAG_DWORD:
                                    if (sceIoWrite(fd2, "4", 1) != 1)
                                        goto ErrorReadDiffA;
                                    miscType = 4;
                                    break;
                                case BLOCK_FLAG_WORD:
                                    if (sceIoWrite(fd2, "2", 1) != 1)
                                        goto ErrorReadDiffA;
                                    miscType = 2;
                                    break;
                                case BLOCK_FLAG_BYTE:
                                    if (sceIoWrite(fd2, "1", 1) != 1)
                                        goto ErrorReadDiffA;
                                    miscType = 1;
                                    break;
                                }

                                //Get ready
                                counter = 0x48804000;

                                //Go!
                                while (counter < 0x4A000000) {
                                    //Load it
                                    //sceIoRead(fd, &scounter, miscType);
                                    fileBufferRead(&scounter, miscType);

                                    //Helper
                                    if (!((counter - 0x48804000) & 0xFFFF)) {
                                        if (!cheatPause)
                                            sceKernelDelayThread(1500);

                                        lineClear(33);
                                        pspDebugScreenSetTextColor(0xFFFF8000);
                                        sprintf(
                                                buffer,
                                                "Task = %02d%%; () = Hold to Abort",
                                                (counter - 0x48804000)
                                                        / ((0x4A000000
                                                                - 0x48804000)
                                                                / 100));
                                        pspDebugScreenPuts(buffer);

                                        sceCtrlPeekBufferPositive(&pad, 1);

                                        if (pad.Buttons & PSP_CTRL_CIRCLE) {
                                            lineClear(33);
                                            pspDebugScreenSetTextColor(
                                                    0xFFFF8000);
                                            pspDebugScreenPuts(
                                                    "Task Aborted!!!");

                                            do {
                                                sceKernelDelayThread(150000);
                                                sceCtrlPeekBufferPositive(&pad,
                                                        1);
                                            } while (pad.Buttons
                                                    & PSP_CTRL_CIRCLE);
                                            break;
                                        }
                                    }

                                    //Check
                                    switch (searchHistory[0].flags & BLOCK_FLAG_DWORD) {
                                    case BLOCK_FLAG_DWORD:
                                        if (searchMode == 0) {
                                            if ((unsigned int) scounter
                                                    != *((unsigned int*) (counter)))
                                                break;
                                        } else if (searchMode == 1) {
                                            if ((unsigned int) scounter
                                                    == *((unsigned int*) (counter)))
                                                break;
                                        } else if (searchMode == 2) {
                                            if ((unsigned int) scounter
                                                    >= *((unsigned int*) (counter)))
                                                break;
                                        } else if (searchMode == 3) {
                                            if ((unsigned int) scounter
                                                    <= *((unsigned int*) (counter)))
                                                break;
                                        } else if (searchMode == 4) {
                                            if ((unsigned int) ((unsigned int) scounter
                                                    + (unsigned int) searchHistory[0].hakVal)
                                                    != *((unsigned int*) (counter)))
                                                break;
                                        } else if (searchMode == 5) {
                                            if ((unsigned int) ((unsigned int) scounter
                                                    - (unsigned int) searchHistory[0].hakVal)
                                                    != *((unsigned int*) (counter)))
                                                break;
                                        }
                                        scounter = *((unsigned int*) (counter));

                                        //Add it
                                        if (sceIoWrite(fd2, &counter,
                                                sizeof(unsigned int)) != 4)
                                            goto ErrorReadDiffA;
                                        if (sceIoWrite(fd2, &scounter,
                                                sizeof(unsigned int)) != 4)
                                            goto ErrorReadDiffA;
                                        searchResultCounter++;
                                        break;

                                    case BLOCK_FLAG_WORD:
                                        if (searchMode == 0) {
                                            if ((unsigned short) scounter
                                                    != *((unsigned short*) (counter)))
                                                break;
                                        } else if (searchMode == 1) {
                                            if ((unsigned short) scounter
                                                    == *((unsigned short*) (counter)))
                                                break;
                                        } else if (searchMode == 2) {
                                            if ((unsigned short) scounter
                                                    >= *((unsigned short*) (counter)))
                                                break;
                                        } else if (searchMode == 3) {
                                            if ((unsigned short) scounter
                                                    <= *((unsigned short*) (counter)))
                                                break;
                                        } else if (searchMode == 4) {
                                            if ((unsigned short) ((unsigned short) scounter
                                                    + (unsigned short) searchHistory[0].hakVal)
                                                    != *((unsigned short*) (counter)))
                                                break;
                                        } else if (searchMode == 5) {
                                            if ((unsigned short) ((unsigned short) scounter
                                                    - (unsigned short) searchHistory[0].hakVal)
                                                    != *((unsigned short*) (counter)))
                                                break;
                                        }
                                        scounter
                                                = *((unsigned short*) (counter));

                                        //Add it
                                        if (sceIoWrite(fd2, &counter,
                                                sizeof(unsigned int)) != 4)
                                            goto ErrorReadDiffA;
                                        if (sceIoWrite(fd2, &scounter,
                                                sizeof(unsigned short)) != 2)
                                            goto ErrorReadDiffA;
                                        searchResultCounter++;
                                        break;

                                    case BLOCK_FLAG_BYTE:
                                        if (searchMode == 0) {
                                            if ((unsigned char) scounter
                                                    != *((unsigned char*) (counter)))
                                                break;
                                        } else if (searchMode == 1) {
                                            if ((unsigned char) scounter
                                                    == *((unsigned char*) (counter)))
                                                break;
                                        } else if (searchMode == 2) {
                                            if ((unsigned char) scounter
                                                    >= *((unsigned char*) (counter)))
                                                break;
                                        } else if (searchMode == 3) {
                                            if ((unsigned char) scounter
                                                    <= *((unsigned char*) (counter)))
                                                break;
                                        } else if (searchMode == 4) {
                                            if ((unsigned char) ((unsigned char) scounter
                                                    + (unsigned char) searchHistory[0].hakVal)
                                                    != *((unsigned char*) (counter)))
                                                break;
                                        } else if (searchMode == 5) {
                                            if ((unsigned char) ((unsigned char) scounter
                                                    - (unsigned char) searchHistory[0].hakVal)
                                                    != *((unsigned char*) (counter)))
                                                break;
                                        }
                                        scounter
                                                = *((unsigned char*) (counter));

                                        //Add it
                                        if (sceIoWrite(fd2, &counter,
                                                sizeof(unsigned int)) != 4)
                                            goto ErrorReadDiffA;
                                        if (sceIoWrite(fd2, &scounter,
                                                sizeof(unsigned char)) != 1)
                                            goto ErrorReadDiffA;
                                        searchResultCounter++;
                                        break;
                                    }

                                    //Next
                                    counter += miscType;
                                }

                                //Close the files
                                sceIoClose(fd);
                                sceIoClose(fd2);

                                while (1) {
                                    break;
                                    //ReadShort
                                    ErrorReadDiffA: sceIoClose(fd);
                                    sceIoClose(fd2);
                                    if (searchNo > 0)
                                        searchNo--;
                                    sceIoRemove(buffer);

                                    lineClear(33);
                                    pspDebugScreenSetTextColor(0xFF0000FF);
                                    pspDebugScreenPuts(
                                            "ERROR: MemoryStick out of Space!");
                                    sceKernelDelayThread(3000000);
                                    break;
                                }
                            } else {
                                //ERROR - files couldn't be opened - undo search attempt
                                sceIoClose(fd);
                                sceIoClose(fd2);
                                if (searchNo > 0)
                                    searchNo--;
                            }
                        } else //Continue the search with a different Diff number
                        {
                            //Increment the search
                            searchNo++;
                            searchMax++;

                            //Open the files
                            sprintf(buffer, "ms0:/search%d.dat", searchNo - 1);
                            fd = sceIoOpen(buffer, PSP_O_RDONLY, 0777);
                            sprintf(buffer, "ms0:/search%d.dat", searchNo);
                            fd2 = sceIoOpen(buffer, PSP_O_WRONLY | PSP_O_CREAT,
                                    0777);
                            if ((fd > 0) && (fd2 > 0)) {
                                //Get ready to go through and check the addresses
                                sceIoLseek(fd, 1, SEEK_SET);

                                //Write out the searcHistory[0] type
                                switch (searchHistory[0].flags & BLOCK_FLAG_DWORD) {
                                case BLOCK_FLAG_DWORD:
                                    if (sceIoWrite(fd2, "4", 1) != 1)
                                        goto ErrorReadDiffB;
                                    break;
                                case BLOCK_FLAG_WORD:
                                    if (sceIoWrite(fd2, "2", 1) != 1)
                                        goto ErrorReadDiffB;
                                    break;
                                case BLOCK_FLAG_BYTE:
                                    if (sceIoWrite(fd2, "1", 1) != 1)
                                        goto ErrorReadDiffB;
                                    break;
                                }

                                //Loop through the list checking each one
                                counter = searchResultCounter;
                                searchResultCounter = 0;
                                while (counter > 0) {
                                    //Load it
                                    sceIoRead(fd, &scounter,
                                            sizeof(unsigned int));

                                    //Helper
                                    if (!(counter & 0x3F)) {
                                        if (!cheatPause)
                                            sceKernelDelayThread(1500);

                                        lineClear(33);
                                        pspDebugScreenSetTextColor(0xFFFF8000);
                                        sprintf(
                                                buffer,
                                                "Task = %02d%%; () = Hold to Abort",
                                                (scounter - 0x48804000)
                                                        / ((0x4A000000
                                                                - 0x48804000)
                                                                / 100));
                                        pspDebugScreenPuts(buffer);

                                        sceCtrlPeekBufferPositive(&pad, 1);

                                        if (pad.Buttons & PSP_CTRL_CIRCLE) {
                                            lineClear(33);
                                            pspDebugScreenSetTextColor(
                                                    0xFFFF8000);
                                            pspDebugScreenPuts(
                                                    "Task Aborted!!!");

                                            do {
                                                sceKernelDelayThread(150000);
                                                sceCtrlPeekBufferPositive(&pad,
                                                        1);
                                            } while (pad.Buttons
                                                    & PSP_CTRL_CIRCLE);
                                            break;
                                        }
                                    }

                                    //Check
                                    switch (searchHistory[0].flags & BLOCK_FLAG_DWORD) {
                                    case BLOCK_FLAG_DWORD:
                                        sceIoRead(fd, &dcounter,
                                                sizeof(unsigned int));

                                        if (searchMode == 0) {
                                            if ((unsigned int) dcounter
                                                    != *((unsigned int*) (scounter)))
                                                break;
                                        } else if (searchMode == 1) {
                                            if ((unsigned int) dcounter
                                                    == *((unsigned int*) (scounter)))
                                                break;
                                        } else if (searchMode == 2) {
                                            if ((unsigned int) dcounter
                                                    >= *((unsigned int*) (scounter)))
                                                break;
                                        } else if (searchMode == 3) {
                                            if ((unsigned int) dcounter
                                                    <= *((unsigned int*) (scounter)))
                                                break;
                                        } else if (searchMode == 4) {
                                            if ((unsigned int) ((unsigned int) dcounter
                                                    + (unsigned int) searchHistory[0].hakVal)
                                                    != *((unsigned int*) (scounter)))
                                                break;
                                        } else if (searchMode == 5) {
                                            if ((unsigned int) ((unsigned int) dcounter
                                                    - (unsigned int) searchHistory[0].hakVal)
                                                    != *((unsigned int*) (scounter)))
                                                break;
                                        }
                                        dcounter
                                                = *((unsigned int*) (scounter));

                                        //Add it
                                        if (sceIoWrite(fd2, &scounter,
                                                sizeof(unsigned int)) != 4)
                                            goto ErrorReadDiffB;
                                        if (sceIoWrite(fd2, &dcounter,
                                                sizeof(unsigned int)) != 4)
                                            goto ErrorReadDiffB;
                                        searchResultCounter++;
                                        break;

                                    case BLOCK_FLAG_WORD:
                                        sceIoRead(fd, &dcounter,
                                                sizeof(unsigned short));

                                        if (searchMode == 0) {
                                            if ((unsigned short) dcounter
                                                    != *((unsigned short*) (scounter)))
                                                break;
                                        } else if (searchMode == 1) {
                                            if ((unsigned short) dcounter
                                                    == *((unsigned short*) (scounter)))
                                                break;
                                        } else if (searchMode == 2) {
                                            if ((unsigned short) dcounter
                                                    >= *((unsigned short*) (scounter)))
                                                break;
                                        } else if (searchMode == 3) {
                                            if ((unsigned short) dcounter
                                                    <= *((unsigned short*) (scounter)))
                                                break;
                                        } else if (searchMode == 4) {
                                            if ((unsigned short) ((unsigned short) dcounter
                                                    + (unsigned short) searchHistory[0].hakVal)
                                                    != *((unsigned short*) (scounter)))
                                                break;
                                        } else if (searchMode == 5) {
                                            if ((unsigned short) ((unsigned short) dcounter
                                                    - (unsigned short) searchHistory[0].hakVal)
                                                    != *((unsigned short*) (scounter)))
                                                break;
                                        }
                                        dcounter
                                                = *((unsigned short*) (scounter));

                                        //Add it
                                        if (sceIoWrite(fd2, &scounter,
                                                sizeof(unsigned int)) != 4)
                                            goto ErrorReadDiffB;
                                        if (sceIoWrite(fd2, &dcounter,
                                                sizeof(unsigned short)) != 2)
                                            goto ErrorReadDiffB;
                                        searchResultCounter++;
                                        break;

                                    case BLOCK_FLAG_BYTE:
                                        sceIoRead(fd, &dcounter,
                                                sizeof(unsigned char));

                                        if (searchMode == 0) {
                                            if ((unsigned char) dcounter
                                                    != *((unsigned char*) (scounter)))
                                                break;
                                        } else if (searchMode == 1) {
                                            if ((unsigned char) dcounter
                                                    == *((unsigned char*) (scounter)))
                                                break;
                                        } else if (searchMode == 2) {
                                            if ((unsigned char) dcounter
                                                    >= *((unsigned char*) (scounter)))
                                                break;
                                        } else if (searchMode == 3) {
                                            if ((unsigned char) dcounter
                                                    <= *((unsigned char*) (scounter)))
                                                break;
                                        } else if (searchMode == 4) {
                                            if ((unsigned char) ((unsigned char) dcounter
                                                    + (unsigned char) searchHistory[0].hakVal)
                                                    != *((unsigned char*) (scounter)))
                                                break;
                                        } else if (searchMode == 5) {
                                            if ((unsigned char) ((unsigned char) dcounter
                                                    - (unsigned char) searchHistory[0].hakVal)
                                                    != *((unsigned char*) (scounter)))
                                                break;
                                        }
                                        dcounter
                                                = *((unsigned char*) (scounter));

                                        //Add it
                                        if (sceIoWrite(fd2, &scounter,
                                                sizeof(unsigned int)) != 4)
                                            goto ErrorReadDiffB;
                                        if (sceIoWrite(fd2, &dcounter,
                                                sizeof(unsigned char)) != 1)
                                            goto ErrorReadDiffB;
                                        searchResultCounter++;
                                        break;
                                    }

                                    //Next
                                    counter--;
                                }

                                //Close the files
                                sceIoClose(fd);
                                sceIoClose(fd2);

                                while (1) {
                                    break;
                                    //ReadShort
                                    ErrorReadDiffB: sceIoClose(fd);
                                    sceIoClose(fd2);
                                    if (searchNo > 0)
                                        searchNo--;
                                    sceIoRemove(buffer);

                                    lineClear(33);
                                    pspDebugScreenSetTextColor(0xFF0000FF);
                                    pspDebugScreenPuts(
                                            "ERROR: MemoryStick out of Space!");
                                    sceKernelDelayThread(3000000);
                                    break;
                                }
                            } else {
                                //ERROR - files couldn't be opened - undo search attempt
                                sceIoClose(fd);
                                sceIoClose(fd2);
                                if (searchNo > 0)
                                    searchNo--;
                            }
                        }
                    } else if (extSelected[0] == 2) {
                        //Reset the search
                        if (searchNo > 0)
                            searchNo--;

                        //Move the cursor back
                        extSelected[0] = 0;
                    } else {
                        //Add the single cheat
                        switch (searchHistory[0].flags & BLOCK_FLAG_DWORD) {
                        case BLOCK_FLAG_DWORD:
                            cheatNew(
                                    4,
                                    searchAddress[extSelected[0] - 3]
                                            - 0x48800000,
                                    *((unsigned int*) (searchAddress[extSelected[0]
                                            - 3])));
                            break;
                        case BLOCK_FLAG_WORD:
                            cheatNew(
                                    2,
                                    searchAddress[extSelected[0] - 3]
                                            - 0x48800000,
                                    *((unsigned short*) (searchAddress[extSelected[0]
                                            - 3])));
                            break;
                        case BLOCK_FLAG_BYTE:
                            cheatNew(
                                    1,
                                    searchAddress[extSelected[0] - 3]
                                            - 0x48800000,
                                    *((unsigned char*) (searchAddress[extSelected[0]
                                            - 3])));
                            break;
                        default:
                            break;
                        }

                        //Switch to the cheat editor
                        extOptArg = extSelected[0];
                        pspDebugScreenInitEx(vram, 0, 0);
                        extSelected[1] = extSelected[2] = extSelected[3] = 0;
                        extSelected[0] = cheat[cheatTotal - 1].block;
                        cheatSelected = cheatTotal - 1;
                        extMenu = 1;
                        extOpt = 2;
                        menuDraw();
                        sceKernelDelayThread(150000);
                    }

                    //Load the file again, get the sample numbers
                    if (searchNo > 0) {
                        //Open the file
                        sprintf(buffer, "ms0:/search%d.dat", searchNo);
                        fd = sceIoOpen(buffer, PSP_O_RDONLY, 0777);
                        if (fd > 0) {
                            //Get the value size
                            sceIoRead(fd, &miscType, 1);
                            miscType -= '0';

                            switch (miscType) {
                            case 1:
                                searchHistory[0].flags
                                        = (searchHistory[0].flags & ~BLOCK_FLAG_DWORD)
                                                | BLOCK_FLAG_BYTE;
                                break;
                            case 2:
                                searchHistory[0].flags
                                        = (searchHistory[0].flags & ~BLOCK_FLAG_DWORD)
                                                | BLOCK_FLAG_WORD;
                                break;
                            case 4:
                                searchHistory[0].flags
                                        = (searchHistory[0].flags & ~BLOCK_FLAG_DWORD)
                                                | BLOCK_FLAG_DWORD;
                                break;
                            }

                            //Get the file size
                            searchResultCounter = sceIoLseek(fd, 0, SEEK_END);
                            sceIoLseek(fd, 1, SEEK_SET);
                            searchResultCounter--;
                            searchResultCounter /= (sizeof(unsigned int)
                                    + miscType);

                            //Only load the first 100
                            if (searchResultCounter > 100) {
                                for (scounter = 0; scounter < 100; scounter++) {
                                    sceIoRead(fd, &searchAddress[scounter],
                                            sizeof(unsigned int));
                                    sceIoLseek(fd, miscType, SEEK_CUR);
                                }
                            } else {
                                for (scounter = 0; scounter
                                        < searchResultCounter; scounter++) {
                                    sceIoRead(fd, &searchAddress[scounter],
                                            sizeof(unsigned int));
                                    sceIoLseek(fd, miscType, SEEK_CUR);
                                }
                            }

                            //Close the file since we are done with the search
                            sceIoClose(fd);
                        }
                    } else {
                        searchResultCounter = 0;
                    }

                    pspDebugScreenInitEx(vram, 0, 0);
                    menuDraw();
                    sceKernelDelayThread(150000);
                }

                if (extSelected[3]) {
                    if (pad.Buttons & PSP_CTRL_UP) {
                        switch (extSelected[1]) {
                        case 0:
                            if (searchMode < 5)
                                searchMode++;
                            break;
                        case 1:
                            searchHistory[0].hakVal += (1 << (4 * (7
                                    - extSelected[2])));
                            break;
                        case 2:
                            searchHistory[0].hakVal += decDelta[extSelected[2]];
                            break;
                        case 3:
                            searchHistory[0].hakVal += (1 << (8
                                    * (extSelected[2])));
                            break;
                        }
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else if (pad.Buttons & PSP_CTRL_DOWN) {
                        switch (extSelected[1]) {
                        case 0:
                            if (searchMode > 0)
                                searchMode--;
                            break;
                        case 1:
                            searchHistory[0].hakVal -= (1 << (4 * (7
                                    - extSelected[2])));
                            break;
                        case 2:
                            searchHistory[0].hakVal -= decDelta[extSelected[2]];
                            break;
                        case 3:
                            searchHistory[0].hakVal -= (1 << (8
                                    * (extSelected[2])));
                            break;
                        }
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else {
                        cheatButtonAgeY = 0;
                    }
                } else {
                    if (pad.Buttons & PSP_CTRL_UP) {
                        if (extSelected[0] > 0) {
                            extSelected[0]--;
                        }
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else if (pad.Buttons & PSP_CTRL_DOWN) {
                        if (extSelected[0] < (1 + (!!searchNo)
                                + (searchResultCounter > 100 ? 100
                                        : searchResultCounter))) {
                            extSelected[0]++;
                        }
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else {
                        cheatButtonAgeY = 0;
                    }
                }

                if (extSelected[0] == 0) {
                    if (pad.Buttons & PSP_CTRL_LEFT) {
                        extSelected[2]--;
                        switch (extSelected[1]) {
                        case 0:
                            if ((signed) extSelected[2] == -1) {
                                extSelected[2] = 0;
                            }
                            break;
                        case 1:
                            if ((signed) extSelected[2] == -1) {
                                extSelected[2] = 0;
                                extSelected[1]--;
                            }
                            break;
                        case 2:
                            if ((signed) extSelected[2] == -1) {
                                extSelected[2] = 7;
                                extSelected[1]--;
                            }
                            break;
                        case 3:
                            if ((signed) extSelected[2] == -1) {
                                extSelected[2] = 9;
                                extSelected[1]--;
                            }
                            break;
                        }
                        menuDraw();
                        if (cheatButtonAgeX < 12)
                            cheatButtonAgeX++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeX));
                    } else if (pad.Buttons & PSP_CTRL_RIGHT) {
                        extSelected[2]++;
                        if (searchMode < 4) {
                            extSelected[2] = 0;
                        } else {
                            switch (extSelected[1]) {
                            case 0:
                                if (extSelected[2] > 0) {
                                    extSelected[2] = 0;
                                    extSelected[1]++;
                                }
                                break;
                            case 1:
                                if (extSelected[2] > 7) {
                                    extSelected[2] = 0;
                                    extSelected[1]++;
                                }
                                break;
                            case 2:
                                if (extSelected[2] > 9) {
                                    extSelected[2] = 0;
                                    extSelected[1]++;
                                }
                                break;
                            case 3:
                                if (extSelected[2] > 3) {
                                    extSelected[2] = 3;
                                }
                                break;
                            }
                        }
                        menuDraw();
                        if (cheatButtonAgeX < 12)
                            cheatButtonAgeX++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeX));
                    } else {
                        cheatButtonAgeX = 0;
                    }
                }

                if ((padButtons & PSP_CTRL_CIRCLE) && !(pad.Buttons
                        & PSP_CTRL_CIRCLE)) {
                    if (extSelected[3]) {
                        extSelected[3] = 0;
                        menuDraw();
                        sceKernelDelayThread(150000);
                    } else //Search has been aborted!
                    {
                        //Go back
                        pspDebugScreenInitEx(vram, 0, 0);
                        extSelected[0] = extSelected[1] = extSelected[2]
                                = extSelected[3] = 0;
                        extMenu = 0;
                        extOpt = 0;
                        menuDraw();
                        sceKernelDelayThread(150000);
                        break;
                    }
                }
                break;

            case 4: //INPUT EXT TEXT SEARCH
                if (pad.Buttons & PSP_CTRL_SELECT) {
                    copyMenu = 1;
                    menuDraw();
                    sceKernelDelayThread(150000);
                }

                if (pad.Buttons & PSP_CTRL_CROSS) {
                    if (extSelected[0] == 0) {
                        extSelected[3] = !extSelected[3];
                    } else if (extSelected[0] == 1) {
                        //Move the cursor back
                        extSelected[0] = 0;

                        //Setup the variables
                        searchResultCounter = 0;

                        //Search!
                        counter = 0x48804000;

                        //Helper
                        while (counter < 0x4A000000) {
                            //Helper
                            if (!((counter - 0x48804000) & 0xFFFF)) {
                                if (!cheatPause)
                                    sceKernelDelayThread(1500);

                                lineClear(33);
                                pspDebugScreenSetTextColor(0xFFFF8000);
                                sprintf(buffer,
                                        "Task = %02d%%; () = Hold to Abort",
                                        (counter - 0x48804000) / ((0x4A000000
                                                - 0x48804000) / 100));
                                pspDebugScreenPuts(buffer);

                                sceCtrlPeekBufferPositive(&pad, 1);

                                if (pad.Buttons & PSP_CTRL_CIRCLE) {
                                    lineClear(33);
                                    pspDebugScreenSetTextColor(0xFFFF8000);
                                    pspDebugScreenPuts("Task Aborted!!!");

                                    do {
                                        sceKernelDelayThread(150000);
                                        sceCtrlPeekBufferPositive(&pad, 1);
                                    } while (pad.Buttons & PSP_CTRL_CIRCLE);
                                    break;
                                }
                            }

                            //Check
                            scounter = 0;
                            while (scounter < 50) {
                                if (counter + scounter <= 0x49FFFFFF) {
                                    unsigned char tempLetter =
                                            *((unsigned char*) (counter
                                                    + scounter));
                                    if ((tempLetter >= 0x61) && (tempLetter
                                            <= 0x7A))
                                        tempLetter -= 0x20;
                                    if (tempLetter
                                            == (unsigned char) fileBuffer[scounter]) {
                                        scounter++;
                                        if (!fileBuffer[scounter + 1]) {
                                            //Add it
                                            searchAddress[searchResultCounter]
                                                    = counter;
                                            searchResultCounter++;
                                            break;
                                        }
                                    } else {
                                        if (scounter == 0) {
                                            scounter = 1;
                                        }
                                        break;
                                    }
                                } else {
                                    scounter++;
                                    break;
                                }
                            }

                            counter += scounter;

                            if (searchResultCounter == 100)
                                break;
                        }
                    }

                    pspDebugScreenInitEx(vram, 0, 0);
                    menuDraw();
                    sceKernelDelayThread(150000);
                }

                if (extSelected[3]) {
                    if (pad.Buttons & PSP_CTRL_UP) {
                        fileBuffer[extSelected[2]]++;
                        switch (fileBuffer[extSelected[2]]) {
                        case 0x01:
                            fileBuffer[extSelected[2]] = 0x20;
                            break;
                        case 0x21:
                            fileBuffer[extSelected[2]] = 0x41;
                            break;
                        case 0x5B:
                            fileBuffer[extSelected[2]] = 0x30;
                            break;
                        case 0x3A:
                            fileBuffer[extSelected[2]] = 0x21;
                            break;
                        case 0x30:
                            fileBuffer[extSelected[2]] = 0x3A;
                            break;
                        case 0x41:
                            fileBuffer[extSelected[2]] = 0x5B;
                            break;
                        case 0x61:
                            fileBuffer[extSelected[2]] = 0x7B;
                            break;
                        case 0x7F:
                            fileBuffer[extSelected[2]] = 0x20;
                            break;
                        }

                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else if (pad.Buttons & PSP_CTRL_DOWN) {
                        fileBuffer[extSelected[2]]--;
                        switch (fileBuffer[extSelected[2]]) {
                        case 0xFF:
                            fileBuffer[extSelected[2]] = 0x7E;
                            break;
                        case 0x7A:
                            fileBuffer[extSelected[2]] = 0x60;
                            break;
                        case 0x5A:
                            fileBuffer[extSelected[2]] = 0x40;
                            break;
                        case 0x39:
                            fileBuffer[extSelected[2]] = 0x2F;
                            break;
                        case 0x20:
                            fileBuffer[extSelected[2]] = 0x39;
                            break;
                        case 0x2F:
                            fileBuffer[extSelected[2]] = 0x5A;
                            break;
                        case 0x40:
                            fileBuffer[extSelected[2]] = 0x20;
                            break;
                        case 0x1F:
                            fileBuffer[extSelected[2]] = 0x7E;
                            break;
                        }
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else {
                        cheatButtonAgeY = 0;
                    }
                } else {
                    if (pad.Buttons & PSP_CTRL_UP) {
                        if (extSelected[0] > 0) {
                            extSelected[0]--;
                        }
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else if (pad.Buttons & PSP_CTRL_DOWN) {
                        if (extSelected[0] < (1
                                + (searchResultCounter > 100 ? 100
                                        : searchResultCounter))) {
                            extSelected[0]++;
                        }
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else {
                        cheatButtonAgeY = 0;
                    }
                }

                if (extSelected[0] == 0) {
                    if (pad.Buttons & PSP_CTRL_SQUARE) {
                        memset(&fileBuffer[extSelected[2] + 1], 0, 49
                                - extSelected[2]);
                        menuDraw();
                    }

                    if (pad.Buttons & PSP_CTRL_LEFT) {
                        extSelected[2]--;
                        if ((signed) extSelected[2] == -1) {
                            extSelected[2] = 0;
                        }
                        menuDraw();
                        if (cheatButtonAgeX < 12)
                            cheatButtonAgeX++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeX));
                    } else if (pad.Buttons & PSP_CTRL_RIGHT) {
                        extSelected[2]++;
                        if (extSelected[2] == 50) {
                            extSelected[2] = 49;
                        }
                        if (fileBuffer[extSelected[2]] == 0x00)
                            fileBuffer[extSelected[2]] = 'A';
                        menuDraw();
                        if (cheatButtonAgeX < 12)
                            cheatButtonAgeX++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeX));
                    } else {
                        cheatButtonAgeX = 0;
                    }
                } else if (extSelected[0] >= 2) {
                    if (pad.Buttons & PSP_CTRL_LEFT) {
                        searchAddress[extSelected[0] - 2]--;
                        if (searchAddress[extSelected[0] - 2] < 0x48804000) {
                            searchAddress[extSelected[0] - 2] = 0x48804000;
                        }

                        menuDraw();
                        if (cheatButtonAgeX < 12)
                            cheatButtonAgeX++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeX));
                    } else if (pad.Buttons & PSP_CTRL_RIGHT) {
                        searchAddress[extSelected[0] - 2]++;
                        if (searchAddress[extSelected[0] - 2] >= 0x4A000000) {
                            searchAddress[extSelected[0] - 2] = 0x49FFFFFF;
                        }

                        menuDraw();
                        if (cheatButtonAgeX < 12)
                            cheatButtonAgeX++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeX));
                    } else {
                        cheatButtonAgeX = 0;
                    }
                }

                if ((padButtons & PSP_CTRL_CIRCLE) && !(pad.Buttons
                        & PSP_CTRL_CIRCLE)) {
                    if (extSelected[3]) {
                        extSelected[3] = 0;
                        menuDraw();
                        sceKernelDelayThread(150000);
                    } else //Search has been aborted!
                    {
                        //Go back
                        cheatSelected = 0;
                        pspDebugScreenInitEx(vram, 0, 0);
                        extSelected[0] = extSelected[1] = extSelected[2]
                                = extSelected[3] = 0;
                        extMenu = 0;
                        extOpt = 0;
                        menuDraw();
                        sceKernelDelayThread(150000);
                        break;
                    }
                }
                break;
            }
        } else {
            //Overall button inputs
            if ((pad.Buttons & PSP_CTRL_LTRIGGER) && (tabSelected != 0)) {
                pspDebugScreenInitEx(vram, 0, 0);
                cheatSelected = 0;
                extSelected[0] = extSelected[1] = extSelected[2]
                        = extSelected[3] = 0;
                extOpt = 0;
                if (tabSelected > 0) {
                    tabSelected--;
                }
                menuDraw();
                sceKernelDelayThread(150000);
            }
            if ((pad.Buttons & PSP_CTRL_RTRIGGER) && (tabSelected != 6)) {
                pspDebugScreenInitEx(vram, 0, 0);
                cheatSelected = 0;
                extSelected[0] = extSelected[1] = extSelected[2]
                        = extSelected[3] = 0;
                extOpt = 0;
                if (tabSelected < 7) {
                    tabSelected++;
                }
                menuDraw();
                sceKernelDelayThread(150000);
            }
            if ((padButtons & PSP_CTRL_CIRCLE) && !(pad.Buttons
                    & PSP_CTRL_CIRCLE)) {
                //Special case for the memory viewer
                if (extSelected[3]) {
                    extSelected[3] = 0;
                    menuDraw();
                    sceKernelDelayThread(150000);
                } else {
                    //Unregister the O key so that the user mode game doesn't pick it up
                    menuDrawn = 0;
                    return;
                }
            }
            if ((padButtons & PSP_CTRL_NOTE)) {
                lineClear(33);
                pspDebugScreenSetTextColor(0xFF0000FF);
                pspDebugScreenPuts("ERROR: Must press Music Button in game!");
                sceKernelDelayThread(1000000);
                lineClear(33);
                menuDraw();
                continue;
            }

            //This is for the warning message at the start
            if (lolInit) {
                if ((padButtons & PSP_CTRL_START) && !(pad.Buttons
                        & PSP_CTRL_START)) {
                    //Special case for the memory viewer
                    lolInit = 0;
                    pspDebugScreenInitEx(vram, 0, 0);
                    menuDraw();
                    continue;
                }

                pspDebugScreenSetXY(0, 26);
                pspDebugScreenSetTextColor(0xFF000000 | ((unsigned) lolValue
                        << 16) | ((unsigned) lolValue << 8)
                        | ((unsigned) lolValue));
                pspDebugScreenPuts(
                        "                        Please insert coin!");
                lolValue += lolDirection;
                if ((unsigned) lolValue == 255) {
                    lolDirection = -1;
                } else if ((unsigned) lolValue == 0) {
                    lolDirection = 1;
                }

                sceKernelDelayThread(1000);
                continue;
            }

            //Choose the appropriate action based on the tabSelected
            switch (tabSelected) {
            case 0: //INPUT CHEATER
                if (pad.Buttons & PSP_CTRL_UP) {
                    cheatpanelDpadUp(&krCheatPanel);
                    menuDraw();
                    if (cheatButtonAgeY < 12)
                        cheatButtonAgeY++;
                    sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                } else if (pad.Buttons & PSP_CTRL_DOWN) {
                    /*
                    if (cheatSelected < (cheatTotal - 1)) {
                        cheatSelected++;
                    } else if (cheatSelected == (cheatTotal - 1)) {
                        cheatSelected = 0;
                    }
                    */
    /*
                    cheatpanelDpadDown(&krCheatPanel);
                    menuDraw();
                    if (cheatButtonAgeY < 12)
                        cheatButtonAgeY++;
                    sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                } else {
                    cheatButtonAgeY = 0;
                }
                if (pad.Buttons & PSP_CTRL_CROSS) {
                    /*
                    cheat[cheatSelected].flags = (cheat[cheatSelected].flags
                            & (~CHEAT_FLAG_SELECTED))
                            | ((~cheat[cheatSelected].flags) & CHEAT_FLAG_SELECTED);
                    cheat[cheatSelected].flags &= ~CHEAT_FLAG_CONSTANT;
                    cheat[cheatSelected].flags |= CHEAT_FLAG_FRESH;
                    */
    /*
                    cheatpanelCrossButton(&krCheatPanel);
                    menuDraw();
                    cheatApply(0);

                    sceKernelDelayThread(150000);
                } else if (pad.Buttons & PSP_CTRL_SQUARE) {
                    /*
                    cheat[cheatSelected].flags = (cheat[cheatSelected].flags
                            & ~CHEAT_FLAG_CONSTANT) | (~cheat[cheatSelected].flags
                            & CHEAT_FLAG_CONSTANT);
                    cheat[cheatSelected].flags &= ~CHEAT_FLAG_SELECTED;
                    cheat[cheatSelected].flags |= CHEAT_FLAG_FRESH;
                    */
    /*
                    cheatpanelSquareButton(&krCheatPanel);
                    menuDraw();
                    cheatApply(0);

                    sceKernelDelayThread(150000);
                } else if (pad.Buttons & PSP_CTRL_TRIANGLE) {
                    pspDebugScreenInitEx(vram, 0, 0);
                    extSelected[1] = extSelected[2] = extSelected[3] = 0;
                    extSelected[0] = cheat[cheatSelected].block;
                    extMenu = 1;
                    extOpt = 0;
                    menuDraw();

                    sceKernelDelayThread(150000);
                }
                break;

            case 1: //INPUT SEARCHER
                if (pad.Buttons & PSP_CTRL_UP) {
                    if (cheatSelected > 0) {
                        cheatSelected--;
                    } else if (cheatSelected == 0) {
                        cheatSelected = (2 + ((!cheatSearch) * 2));
                    }
                    menuDraw();
                    if (cheatButtonAgeY < 12)
                        cheatButtonAgeY++;
                    sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                } else if (pad.Buttons & PSP_CTRL_DOWN) {
                    if (cheatSelected < (2 + ((!cheatSearch) * 2))) {
                        cheatSelected++;
                    } else if (cheatSelected == (2 + ((!cheatSearch) * 2))) {
                        cheatSelected = 0;
                    }
                    menuDraw();
                    if (cheatButtonAgeY < 12)
                        cheatButtonAgeY++;
                    sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                } else {
                    cheatButtonAgeY = 0;
                }
                if (pad.Buttons & PSP_CTRL_CROSS) {
                    if (!cheatSearch) {
                        if (cheatSelected == 0) {
                            //Goto Find exact
                            pspDebugScreenInitEx(vram, 0, 0);
                            extSelected[0] = extSelected[1] = extSelected[2]
                                    = extSelected[3] = 0;
                            extMenu = 2;
                            extOpt = 0;
                            menuDraw();
                            cheatSearch = 1;
                        } else if (cheatSelected == 4) {
                            //Goto Find text
                            searchResultCounter = 0;
                            pspDebugScreenInitEx(vram, 0, 0);
                            memset(fileBuffer, 0, 50);
                            fileBuffer[0] = 'A';
                            extSelected[0] = extSelected[1] = extSelected[2]
                                    = extSelected[3] = 0;
                            extMenu = 4;
                            extOpt = 0;
                            menuDraw();
                        } else {
                            //Dump a search dump
                            fd = sceIoOpen("ms0:/search.ram", PSP_O_WRONLY
                                    | PSP_O_CREAT, 0777);
                            if (fd > 0) {
                                if (sceIoWrite(fd, (void*) 0x08800000,
                                        0x1800000) == 0x1800000) {
                                    sceIoClose(fd);

                                    cheatSearch = 1;
                                    switch (cheatSelected) {
                                    case 1:
                                        searchHistory[0].flags
                                                = (searchHistory[0].flags
                                                        & (~BLOCK_FLAG_DWORD))
                                                        | BLOCK_FLAG_BYTE;
                                        break;
                                    case 2:
                                        searchHistory[0].flags
                                                = (searchHistory[0].flags
                                                        & (~BLOCK_FLAG_DWORD))
                                                        | BLOCK_FLAG_WORD;
                                        break;
                                    case 3:
                                        searchHistory[0].flags
                                                = (searchHistory[0].flags
                                                        & (~BLOCK_FLAG_DWORD))
                                                        | BLOCK_FLAG_DWORD;
                                        break;
                                    }

                                    pspDebugScreenInitEx(vram, 0, 0);
                                    tabSelected = 1;
                                    cheatSelected = 1;
                                    menuDraw();
                                    lineClear(33);
                                    pspDebugScreenSetTextColor(0xFFFF8000);
                                    pspDebugScreenPuts("Now, resume the game!");

                                    sceKernelDelayThread(3000000);
                                } else {
                                    sceIoClose(fd);

                                    sceIoRemove("ms0:/search.ram");

                                    lineClear(33);
                                    pspDebugScreenSetTextColor(0xFF0000FF);
                                    pspDebugScreenPuts(
                                            "ERROR: MemoryStick out of Space!");

                                    sceKernelDelayThread(3000000);
                                }
                            }
                        }
                    } else {
                        if (cheatSelected == 0) {
                            //Goto Find exact
                            pspDebugScreenInitEx(vram, 0, 0);
                            extSelected[0] = extSelected[1] = extSelected[2]
                                    = extSelected[3] = 0;
                            extMenu = 2;
                            extOpt = 0;
                            menuDraw();
                        } else if (cheatSelected == 1) {
                            //Goto Find Diff
                            pspDebugScreenInitEx(vram, 0, 0);
                            extSelected[0] = extSelected[1] = extSelected[2]
                                    = extSelected[3] = 0;
                            extMenu = 3;
                            extOpt = 0;
                            menuDraw();
                        } else if (cheatSelected == 2) {
                            sceIoRemove("ms0:/search.ram");

                            while (searchMax > 0) {
                                sprintf(buffer, "ms0:/search%d.dat", searchMax);
                                sceIoRemove(buffer);
                                searchMax--;
                            }

                            //Reset fields
                            searchNo = 0;
                            cheatSearch = 0;
                            cheatSelected = 0;
                            searchResultCounter = 0;
                        }
                    }

                    menuDraw();
                    sceKernelDelayThread(150000);
                }
                break;

            case 2: //INPUT PRX
                if (pad.Buttons & PSP_CTRL_UP) {
                    if (cheatSelected > 0) {
                        cheatSelected--;
                    } else if (cheatSelected == 0) {
                        cheatSelected = 9;
                    }
                    menuDraw();
                    if (cheatButtonAgeY < 12)
                        cheatButtonAgeY++;
                    sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                } else if (pad.Buttons & PSP_CTRL_DOWN) {
                    if (cheatSelected < 9) {
                        cheatSelected++;
                    } else if (cheatSelected == 9) {
                        cheatSelected = 0;
                    }
                    menuDraw();
                    if (cheatButtonAgeY < 12)
                        cheatButtonAgeY++;
                    sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                } else {
                    cheatButtonAgeY = 0;
                }
                if (pad.Buttons & PSP_CTRL_LEFT) {
                    if ((cheatSelected == 8) && (cheatHz > 0)) {
                        cheatHz -= 15625;
                    }
                    if ((cheatSelected == 3) && (dumpNo > 0)) {
                        dumpNo--;
                    }
                    if ((cheatSelected == 2) && (dumpNo > 0)) {
                        dumpNo--;
                    }
                    menuDraw();
                    if (cheatButtonAgeX < 12)
                        cheatButtonAgeX++;
                    sceKernelDelayThread(150000 - (10000 * cheatButtonAgeX));
                } else if (pad.Buttons & PSP_CTRL_RIGHT) {
                    if (cheatSelected == 8) {
                        cheatHz += 15625;
                    }
                    if (cheatSelected == 3) {
                        dumpNo++;
                    }
                    if (cheatSelected == 2) {
                        dumpNo++;
                    }
                    menuDraw();
                    if (cheatButtonAgeX < 12)
                        cheatButtonAgeX++;
                    sceKernelDelayThread(150000 - (10000 * cheatButtonAgeX));
                } else {
                    cheatButtonAgeX = 0;
                }
                if (pad.Buttons & PSP_CTRL_CROSS) {
                    if (cheatSelected == 0) {
                        cheatPause = !cheatPause;
                        menuDraw();

                        if (cheatPause) {
                            gamePause(thid);
                        } else {
                            gameResume(thid);
                        }
                    } else if (cheatSelected == 1) {
                        cheatNew(4, 0x4000, 0);

                        //Switch to the cheat editor
                        pspDebugScreenInitEx(vram, 0, 0);
                        extSelected[1] = extSelected[2] = extSelected[3] = 0;
                        extSelected[0] = cheat[cheatTotal - 1].block;
                        cheatSelected = cheatTotal - 1;
                        extMenu = 1;
                        extOpt = 1;
                        menuDraw();
                        sceKernelDelayThread(150000);
                    } else if (cheatSelected == 2) {
                        sprintf(buffer, "ms0:/dump%d.ram", dumpNo);

                        fd = sceIoOpen(buffer, PSP_O_RDONLY, 0777);
                        if (fd > 0) {
                            counter = 0;
                            while (counter < cheatTotal) {
                                scounter = cheat[counter].block;
                                cheatDMA = 0;
                                while (scounter < cheat[counter].block
                                        + cheat[counter].len) {
                                    if (block[scounter].flags & BLOCK_FLAG_DMA) {
                                        if (block[scounter].hakVal
                                                != 0xFFFFFFFF) {
                                            sceIoLseek(fd,
                                                    block[scounter].hakVal,
                                                    SEEK_SET);
                                            sceIoRead(fd, &cheatDMA,
                                                    sizeof(unsigned int));
                                            block[scounter].stdVal = cheatDMA;
                                        } else {
                                            cheatDMA = 0;
                                        }
                                    } else {
                                        sceIoLseek(fd, (cheatDMA
                                                + block[scounter].address)
                                                - 0x08800000, SEEK_SET);
                                        switch (block[scounter].flags
                                                & BLOCK_FLAG_DWORD) {
                                        case BLOCK_FLAG_DWORD:
                                            sceIoRead(fd,
                                                    &block[scounter].stdVal,
                                                    sizeof(unsigned int));
                                            break;
                                        case BLOCK_FLAG_WORD:
                                            sceIoRead(fd,
                                                    &block[scounter].stdVal,
                                                    sizeof(unsigned short));
                                            break;
                                        case BLOCK_FLAG_BYTE:
                                            sceIoRead(fd,
                                                    &block[scounter].stdVal,
                                                    sizeof(unsigned char));
                                            break;
                                        }
                                    }

                                    scounter++;
                                }

                                cheatDisable(counter);
                                counter++;
                            }
                            sceIoClose(fd);

                            cheatStatus = 0;
                        } else {
                            lineClear(33);
                            pspDebugScreenSetTextColor(0xFF0000FF);
                            pspDebugScreenPuts(
                                    "ERROR: Selected RAM Dump # does not exist!");
                            sceKernelDelayThread(3000000);
                        }

                        menuDraw();
                    } else if (cheatSelected == 3) {
                        sprintf(buffer, "ms0:/dump%d.ram", dumpNo);

                        fd
                                = sceIoOpen(buffer, PSP_O_WRONLY | PSP_O_CREAT,
                                        0777);
                        if (fd > 0) {
                            if (sceIoWrite(fd, (void*) 0x08800000, 0x1800000)
                                    == 0x1800000) {
                                sceIoClose(fd);

                                dumpNo++;
                            } else {
                                sceIoClose(fd);

                                sceIoRemove(buffer);

                                lineClear(33);
                                pspDebugScreenSetTextColor(0xFF0000FF);
                                pspDebugScreenPuts(
                                        "ERROR: MemoryStick out of Space!");
                                sceKernelDelayThread(3000000);
                            }
                        }
                        menuDraw();
                        sceKernelDelayThread(150000); //Delay twice
                    } else if (cheatSelected == 4) {
                        if (browseLines == 8) {
                            browseLines = 16;
                        } else {
                            browseLines = 8;
                        }
                        browseC = 0;
                        browseX = 0;
                        browseY = 0;
                        if (browseAddress < 0x48804000) {
                            browseAddress = 0x48804000;
                        }
                        if (browseAddress > (0x4A000000 - (22 * browseLines))) {
                            browseAddress = (0x4A000000 - (22 * browseLines));
                        }
                        menuDraw();
                    } else if (cheatSelected == 5) {
                        if (browseFormat == 0x48800000) {
                            browseFormat = 0x40000000;
                        } else {
                            browseFormat = 0x48800000;
                        }
                        menuDraw();
                    } else if (cheatSelected == 6) {
                        if (decodeFormat == 0x48800000) {
                            decodeFormat = 0x40000000;
                        } else {
                            decodeFormat = 0x48800000;
                        }
                        menuDraw();
                    } else if (cheatSelected == 7) {
                        if (trackFormat == 0x48800000) {
                            trackFormat = 0x40000000;
                        } else {
                            trackFormat = 0x48800000;
                        }
                        menuDraw();
                    } else if (cheatSelected == 9) {
                        cheatSave();
                        menuDraw();
                        sceKernelDelayThread(150000); //Delay twice
                    }
                    sceKernelDelayThread(150000);
                }
                break;

            case 3: //INPUT BROWSER
                if (pad.Buttons & PSP_CTRL_SELECT) {
                    copyMenu = 1;
                    menuDraw();
                    sceKernelDelayThread(150000);
                }
                if (pad.Buttons & PSP_CTRL_CROSS) {
                    extSelected[3] = !extSelected[3];
                    menuDraw();
                    sceKernelDelayThread(150000);
                }

                if (pad.Buttons & PSP_CTRL_LEFT) {
                    /*
                    browseX--;
                    switch (browseC) {
                    case 0:
                        if ((signed) browseX == -1) {
                            browseX = 0;
                        }
                        break;
                    case 1:
                        if ((signed) browseX == -1) {
                            browseX = 7;
                            browseC--;
                        }
                        break;
                    case 2:
                        if ((signed) browseX == -1) {
                            browseX = (2 * browseLines) - 1;
                            browseC--;
                        }
                        break;
                    }
                    */
    /*
                    browserDpadLeft();
                    menuDraw();
                    if (cheatButtonAgeX < 12)
                        cheatButtonAgeX++;
                    sceKernelDelayThread(150000 - (10000 * cheatButtonAgeX));
                } else if (pad.Buttons & PSP_CTRL_RIGHT) {
                    /*
                    browseX++;
                    switch (browseC) {
                    case 0:
                        if (browseX > 7) {
                            browseX = 0;
                            browseC++;
                        }
                        break;
                    case 1:
                        if (browseX > ((2 * browseLines) - 1)) {
                            browseX = 0;
                            browseC++;
                        }
                        break;
                    case 2:
                        if (browseX > (browseLines - 1)) {
                            browseX = browseLines - 1;
                        }
                        break;
                    }
                    */
    /*
                    browserDpadRight();
                    menuDraw();
                    if (cheatButtonAgeX < 12)
                        cheatButtonAgeX++;
                    sceKernelDelayThread(150000 - (10000 * cheatButtonAgeX));
                } else {
                    cheatButtonAgeX = 0;
                }

                if (extSelected[3]) {
                    if (pad.Buttons & PSP_CTRL_SQUARE) {
                        if (browseC == 1) {
                            browseC = 2;
                            browseX = browseX / 2;
                        } else if (browseC == 2) {
                            browseC = 1;
                            browseX = 2 * browseX + 1;
                        }
                        menuDraw();
                        sceKernelDelayThread(150000);
                    }
                    if (pad.Buttons & PSP_CTRL_UP) {
                        /*
                        switch (browseC) {
                        case 0:
                            browseAddress += (1 << (4 * (7 - browseX)));
                            if (browseAddress < 0x48804000) {
                                browseAddress = 0x48804000;
                            }
                            if (browseAddress > (0x4A000000
                                    - (22 * browseLines))) {
                                browseAddress = (0x4A000000
                                        - (22 * browseLines));
                            }
                            break;
                        case 1:
                            if (browseX & 1) {
                                *((unsigned char*) ((browseAddress + (browseY
                                        * browseLines)) + (browseX / 2)))
                                        += 0x01;
                            } else {
                                *((unsigned char*) ((browseAddress + (browseY
                                        * browseLines)) + (browseX / 2)))
                                        += 0x10;
                            }
                            sceKernelDcacheWritebackInvalidateRange(
                                    ((unsigned char*) ((browseAddress
                                            + (browseY * browseLines))
                                            + (browseX / 2))), 1);
                            sceKernelIcacheInvalidateRange(
                                    ((unsigned char*) ((browseAddress
                                            + (browseY * browseLines))
                                            + (browseX / 2))), 1);
                            break;
                        case 2:
                            *((unsigned char*) ((browseAddress + (browseY
                                    * browseLines)) + browseX)) += 1;
                            sceKernelDcacheWritebackInvalidateRange(
                                    ((unsigned char*) ((browseAddress
                                            + (browseY * browseLines))
                                            + browseX)), 1);
                            sceKernelIcacheInvalidateRange(
                                    ((unsigned char*) ((browseAddress
                                            + (browseY * browseLines))
                                            + browseX)), 1);
                            break;
                        }
                        */
    /*
                        browserDpadUp();
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else if (pad.Buttons & PSP_CTRL_DOWN) {
                        /*
                        switch (browseC) {
                        case 0:
                            browseAddress -= (1 << (4 * (7 - browseX)));
                            if (browseAddress < 0x48804000) {
                                browseAddress = 0x48804000;
                            }
                            if (browseAddress > (0x4A000000
                                    - (22 * browseLines))) {
                                browseAddress = (0x4A000000
                                        - (22 * browseLines));
                            }
                            break;
                        case 1:
                            if (browseX & 1) {
                                *((unsigned char*) ((browseAddress + (browseY
                                        * browseLines)) + (browseX / 2)))
                                        -= 0x01;
                            } else {
                                *((unsigned char*) ((browseAddress + (browseY
                                        * browseLines)) + (browseX / 2)))
                                        -= 0x10;
                            }
                            sceKernelDcacheWritebackInvalidateRange(
                                    ((unsigned char*) ((browseAddress
                                            + (browseY * browseLines))
                                            + (browseX / 2))), 1);
                            sceKernelIcacheInvalidateRange(
                                    ((unsigned char*) ((browseAddress
                                            + (browseY * browseLines))
                                            + (browseX / 2))), 1);
                            break;
                        case 2:
                            *((unsigned char*) ((browseAddress + (browseY
                                    * browseLines)) + browseX)) -= 1;
                            sceKernelDcacheWritebackInvalidateRange(
                                    ((unsigned char*) ((browseAddress
                                            + (browseY * browseLines))
                                            + browseX)), 1);
                            sceKernelIcacheInvalidateRange(
                                    ((unsigned char*) ((browseAddress
                                            + (browseY * browseLines))
                                            + browseX)), 1);
                            break;
                        }
                        */
    /*
                        browserDpadDown();
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else {
                        cheatButtonAgeY = 0;
                    }
                } else if (pad.Buttons & PSP_CTRL_SQUARE) {
                    if (pad.Buttons & PSP_CTRL_UP) {
                        /*
                        browseAddress -= browseLines;
                        if (browseAddress < 0x48804000) {
                            browseAddress = 0x48804000;
                        }
                        if (browseAddress > (0x4A000000 - (22 * browseLines))) {
                            browseAddress = (0x4A000000 - (22 * browseLines));
                        }
                        */
    /*
                        browserPageUp();
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else if (pad.Buttons & PSP_CTRL_DOWN) {
                        /*
                        browseAddress += browseLines;
                        if (browseAddress < 0x48804000) {
                            browseAddress = 0x48804000;
                        }
                        if (browseAddress > (0x4A000000 - (22 * browseLines))) {
                            browseAddress = (0x4A000000 - (22 * browseLines));
                        }
                        */
    /*
                        browserPageDown();
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else {
                        cheatButtonAgeY = 0;
                    }
                    if ((abs((signed) (pad.Ly - 127)) - 40) > 0) {
                        /*
                        browseAddress += (((signed) (pad.Ly - 127))
                                / browseLines) * 64;
                        if (browseAddress < 0x48804000) {
                            browseAddress = 0x48804000;
                        }
                        if (browseAddress > (0x4A000000 - (22 * browseLines))) {
                            browseAddress = (0x4A000000 - (22 * browseLines));
                        }
                        */
    /*
                        menuDraw();
                        sceKernelDelayThread(18750);
                    }
                } else {
                    if (pad.Buttons & PSP_CTRL_UP) {
                        /*
                        if (browseY > 0) {
                            browseY--;
                        } else if (browseY == 0) {
                            browseAddress -= browseLines;
                            if (browseAddress < 0x48804000) {
                                browseAddress = 0x48804000;
                            }
                            if (browseAddress > (0x4A000000
                                    - (22 * browseLines))) {
                                browseAddress = (0x4A000000
                                        - (22 * browseLines));
                            }
                        }
                        */
    /*
                        browserDpadUp();
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else if (pad.Buttons & PSP_CTRL_DOWN) {
                        /*
                        if (browseY < 21) {
                            browseY++;
                        } else if (browseY == 21) {
                            browseAddress += browseLines;
                            if (browseAddress < 0x48804000) {
                                browseAddress = 0x48804000;
                            }
                            if (browseAddress > (0x4A000000
                                    - (22 * browseLines))) {
                                browseAddress = (0x4A000000
                                        - (22 * browseLines));
                            }
                        }
                        */
    /*
                        browserDpadDown();
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else {
                        cheatButtonAgeY = 0;
                    }
                }
                break;

            case 4: //INPUT DECODER
                if (pad.Buttons & PSP_CTRL_SELECT) {
                    copyMenu = 1;
                    menuDraw();
                    sceKernelDelayThread(150000);
                }
                if (pad.Buttons & PSP_CTRL_CROSS) {
                    decoderXButton();
                    extSelected[3] = !extSelected[3];
                    menuDraw();
                    sceKernelDelayThread(150000);
                }
                if (pad.Buttons & PSP_CTRL_TRIANGLE) {
                    decoderTriangleButton();
                    menuDraw();
                    sceKernelDelayThread(150000);
                }

                if (pad.Buttons & PSP_CTRL_LEFT) {
                    decoderCursorLeft();
                    menuDraw();
                    if (cheatButtonAgeX < 12)
                        cheatButtonAgeX++;
                    sceKernelDelayThread(150000 - (10000 * cheatButtonAgeX));
                } else if (pad.Buttons & PSP_CTRL_RIGHT) {
                    decoderCursorRight();
                    menuDraw();
                    if (cheatButtonAgeX < 12)
                        cheatButtonAgeX++;
                    sceKernelDelayThread(150000 - (10000 * cheatButtonAgeX));
                } else {
                    cheatButtonAgeX = 0;
                }

                if (extSelected[3]) {
                    if (pad.Buttons & PSP_CTRL_UP) {
                        decoderCursorUp();
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else if (pad.Buttons & PSP_CTRL_DOWN) {
                        decoderCursorDown();
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else {
                        cheatButtonAgeY = 0;
                    }
                } else if (pad.Buttons & PSP_CTRL_SQUARE) {
                    if (pad.Buttons & PSP_CTRL_UP) {
                        decoderScrollUp();
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else if (pad.Buttons & PSP_CTRL_DOWN) {
                        decoderScrollDown();
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else {
                        cheatButtonAgeY = 0;
                    }
                    if ((abs((signed) (pad.Ly - 127)) - 40) > 0) {
                        /*
                        decodeAddress += (((signed) (pad.Ly - 127))
                                / browseLines) * browseLines;
                        if (decodeAddress < 0x48804000) {
                            decodeAddress = 0x48804000;
                        }
                        if (decodeAddress > 0x49FFFFA8) {
                            decodeAddress = 0x49FFFFA8;
                        }
                        */
    /*
                        menuDraw();
                        sceKernelDelayThread(18750);
                    }
                } else {
                    if (pad.Buttons & PSP_CTRL_UP) {
                        decoderCursorUp();
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else if (pad.Buttons & PSP_CTRL_DOWN) {
                        decoderCursorDown();
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else {
                        cheatButtonAgeY = 0;
                    }
                }
                break;

            case 5: //INPUT TRACKER
                if ((pad.Buttons & PSP_CTRL_SELECT) && (!trackStatus)) {
                    copyMenu = 1;
                    menuDraw();
                    sceKernelDelayThread(150000);
                }
                if (pad.Buttons & PSP_CTRL_CROSS) {
                    if ((trackY == 0) && (!trackStatus)) {
                        extSelected[3] = !extSelected[3];
                    } else if (trackY == 1) {
                        trackStatus = !trackStatus;

                        if (trackStatus) {
                            if (trackMode == 0) {
                                //Apply breakpoint
                                //trackBackup=*((unsigned int*)(trackAddress));
                                //*((unsigned int*)(trackAddress))=0x0000000D;

                                sceKernelDcacheWritebackInvalidateAll();
                                sceKernelIcacheInvalidateAll();
                            }
                        } else {
                            if (trackMode == 0) {
                                //Remove breakpoint
                                //*((unsigned int*)(trackAddress))=trackBackup;

                                sceKernelDcacheWritebackInvalidateAll();
                                sceKernelIcacheInvalidateAll();
                            }
                        }
                    } else if (trackY == 2) {
                        trackPause = !trackPause;
                    }
                    menuDraw();
                    sceKernelDelayThread(150000);
                }

                if (trackY == 0) {
                    if (pad.Buttons & PSP_CTRL_LEFT) {
                        trackX--;
                        switch (trackC) {
                        case 0:
                            if ((signed) trackX == -1) {
                                trackX = 0;
                            }
                            break;
                        case 1:
                            if ((signed) trackX == -1) {
                                trackX = 0;
                                trackC--;
                            }
                            break;
                        }
                        menuDraw();
                        if (cheatButtonAgeX < 12)
                            cheatButtonAgeX++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeX));
                    } else if (pad.Buttons & PSP_CTRL_RIGHT) {
                        trackX++;
                        switch (trackC) {
                        case 0:
                            if (trackX > 0) {
                                trackX = 0;
                                trackC++;
                            }
                            break;
                        case 1:
                            if (trackX > 7) {
                                trackX = 7;
                            }
                            break;
                        }
                        menuDraw();
                        if (cheatButtonAgeX < 12)
                            cheatButtonAgeX++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeX));
                    } else {
                        cheatButtonAgeX = 0;
                    }
                }

                if (extSelected[3]) {
                    if (pad.Buttons & PSP_CTRL_UP) {
                        if (trackC == 0) {
                            if (trackMode < 1)
                                trackMode++;
                        } else {
                            if (trackX != 7) {
                                trackAddress += (1 << (4 * (7 - trackX)));
                            } else {
                                trackAddress += 4;
                            }
                            if (trackAddress < 0x48804000) {
                                trackAddress = 0x48804000;
                            }
                            if (trackAddress > 0x49FFFFFC) {
                                trackAddress = 0x49FFFFFC;
                            }
                        }
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else if (pad.Buttons & PSP_CTRL_DOWN) {
                        if (trackC == 0) {
                            if (trackMode > 0)
                                trackMode--;
                        } else {
                            if (trackX != 7) {
                                trackAddress -= (1 << (4 * (7 - trackX)));
                            } else {
                                trackAddress -= 4;
                            }
                            if (trackAddress < 0x48804000) {
                                trackAddress = 0x48804000;
                            }
                            if (trackAddress > 0x49FFFFFC) {
                                trackAddress = 0x49FFFFFC;
                            }
                        }
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else {
                        cheatButtonAgeY = 0;
                    }
                } else {
                    if (pad.Buttons & PSP_CTRL_UP) {
                        if (trackY > trackStatus) {
                            trackY--;
                        }
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else if (pad.Buttons & PSP_CTRL_DOWN) {
                        if (trackY < 2) {
                            trackY++;
                        }
                        menuDraw();
                        if (cheatButtonAgeY < 12)
                            cheatButtonAgeY++;
                        sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                    } else {
                        cheatButtonAgeY = 0;
                    }
                }
                break;
            case 6: // Disassembler
                trigger = 0;
                if (pad.Buttons & PSP_CTRL_CIRCLE) {
                    disassemblerCircleButton(&krDisassembler);
                    trigger = 1;
                }
                if (pad.Buttons & PSP_CTRL_CROSS) {
                    disassemblerCrossButton(&krDisassembler);
                    trigger = 1;
                }
                if (pad.Buttons & PSP_CTRL_DOWN) {
                    disassemblerDpadDown(&krDisassembler);
                    trigger = 1;
                }
                if (pad.Buttons & PSP_CTRL_LEFT) {
                    disassemblerDpadLeft(&krDisassembler);
                    trigger = 1;
                }
                if (pad.Buttons & PSP_CTRL_RIGHT) {
                    disassemblerDpadRight(&krDisassembler);
                    trigger = 1;
                }
                if (pad.Buttons & PSP_CTRL_UP) {
                    disassemblerDpadUp(&krDisassembler);
                    trigger = 1;
                }
                if (trigger == 1) {
                    //menuDraw();
                    disassemblerRedraw(&krDisassembler);
                    if (cheatButtonAgeY < 12) {
                        cheatButtonAgeY++;
                    }
                    sceKernelDelayThread(150000 - (10000 * cheatButtonAgeY));
                } else {
                    cheatButtonAgeY = 0;
                }
                break;
            }
        }

        //Stop the game from freezing up
        if (!cheatPause)
            sceKernelDelayThread(15625);
    }
     */
}

int sceOpenPSIDGetOpenPSID(char *openpsid) {
    memcpy(openpsid, psid, 16);
    return 0;
}

/* loadCheats
 *
 * Global Variable References:
 *  gameDir
 */
static void loadCheats() {
    CheatEngine* prEngine = &krCheatEngine;
    Cheat* prCheat = NULL;
    int fd;
    char readbuf[256];
    //Load the cheats
    fd = sceIoOpen(gameDir, PSP_O_RDONLY, 0777);
    if (fd > 0) {
        unsigned int fileSize = sceIoLseek(fd, 0, SEEK_END);
        sceIoLseek(fd, 0, SEEK_SET);
        unsigned int fileOffset = 0;
        unsigned char commentMode = 0;
        unsigned char nameMode = 0;

        while (fileOffset < fileSize) {
            sceKernelDelayThread(1500);

            sceIoRead(fd, &readbuf, 1);

            if ((readbuf[0] == '\r') || (readbuf[0] == '\n')) {
                commentMode = 0;
                if (nameMode) {
                    prEngine->cheat_count++;
                    nameMode = 0;
                }
            } else if ((readbuf[0] == ' ') && (!nameMode)) {
            } else if (readbuf[0] == ';') {
                commentMode = 1;
                if (nameMode) {
                    prEngine->cheat_count++;
                    nameMode = 0;
                }
            } //Skip comments till next line
            else if (readbuf[0] == '#') //Read in the cheat name
            {
                if (prEngine->cheat_count >= CHEATENGINE_CHEAT_MAX) {
                    break;
                }
                prCheat = &(prEngine->cheatlist[prEngine->cheat_count]);
                cheat_set_block(prCheat, prEngine->block_count);
                prCheat->flags = 0;
                cheat_set_length(prCheat, 0);
                prCheat->name[0] = 0;
                nameMode = 1;
                cheat_setflag_fresh(prCheat);
            } else if ((readbuf[0] == '!') && (nameMode)) {
                prCheat = &(prEngine->cheatlist[prEngine->cheat_count]);
                //Cheat's selected by default
                if (cheat_is_selected(prCheat) == 1) {
                    //Two ! = selected for constant on status
                    cheat_set_constant(prCheat);
                } else {
                    //One ! = selected for music on/off button
                    cheat_set_selected(prCheat);
                }
            } else if ((!commentMode) && (nameMode)) {
                prCheat = &(prEngine->cheatlist[prEngine->cheat_count]);
                if (nameMode < 32) //1 to 31 = letters, 32=Null terminator
                {
                    prCheat->name[nameMode - 1] = readbuf[0];
                    nameMode++;
                    prCheat->name[nameMode - 1] = 0;
                }
            } else if ((!commentMode) && (!nameMode)) {
                //Add 0xAABBCCDD 0xAABBCCDD block
                if (!blockAdd(fd, readbuf)) {
                    //No more RAM?
                    if (prEngine->cheat_count != 0) {
                        prEngine->cheat_count--;
                        break;
                    }
                }
                if (prEngine->cheat_count != 0) {
                    prEngine->cheatlist[prEngine->cheat_count - 1].len++;
                }
            }

            fileOffset = sceIoLseek(fd, 0, SEEK_CUR);
        }
        sceIoClose(fd);
    }

}

static void clearSearchHistory() {
    //Clear the search history to 0
    memset(&searchHistory, 0, sizeof(Block) * 15);
    searchHistory[0].flags |= BLOCK_FLAG_DWORD;
    searchHistory[1].flags |= BLOCK_FLAG_DWORD;
    searchHistory[2].flags |= BLOCK_FLAG_DWORD;
    searchHistory[3].flags |= BLOCK_FLAG_DWORD;
    searchHistory[4].flags |= BLOCK_FLAG_DWORD;
    searchHistory[5].flags |= BLOCK_FLAG_DWORD;
    searchHistory[6].flags |= BLOCK_FLAG_DWORD;
    searchHistory[7].flags |= BLOCK_FLAG_DWORD;
    searchHistory[8].flags |= BLOCK_FLAG_DWORD;
    searchHistory[9].flags |= BLOCK_FLAG_DWORD;
    searchHistory[10].flags |= BLOCK_FLAG_DWORD;
    searchHistory[11].flags |= BLOCK_FLAG_DWORD;
    searchHistory[12].flags |= BLOCK_FLAG_DWORD;
    searchHistory[13].flags |= BLOCK_FLAG_DWORD;
    searchHistory[14].flags |= BLOCK_FLAG_DWORD;
    searchHistory[15].flags |= BLOCK_FLAG_DWORD;
}

static void waitForKernelLibrary() {
    //Wait for the kernel to boot
    sceKernelDelayThread(100000);
    while (!sceKernelFindModuleByName("sceKernelLibrary"))
        sceKernelDelayThread(100000);
    sceKernelDelayThread(100000);
}

static void findScreenshotDirectory() {
    signed int fd;

    //Find which screen directory to use
    fd = sceIoDopen("ms0:/PICTURE/");
    if (fd > 0) {
        sceIoDclose(fd);

        strcpy(screenPath, "ms0:/PICTURE/screen%d.bmp");
    } else {
        fd = sceIoDopen("ms0:/PSP/PHOTO/");
        if (fd > 0) {
            sceIoDclose(fd);

            strcpy(screenPath, "ms0:/PSP/PHOTO/screen%d.bmp");
        } else {
            if (!sceIoMkdir("ms0:/PICTURE", 0777)) {
                strcpy(screenPath, "ms0:/PICTURE/screen%d.bmp");
            } else {
                screenPath[0] = 0;
            }
        }
    }
}

static void findCorrectScreenNumber() {
    signed int fd;
    //Set the correct screen number
    screenNo = 0;
    if (screenPath[0] != 0) {
        while (1) {
            sprintf(buffer, screenPath, screenNo);
            fd = sceIoOpen(buffer, PSP_O_RDONLY, 0777);
            if (fd > 0) {
                sceIoClose(fd);
                screenNo++;
            } else {
                break;
            }
        }
    }
}

static void applyOpenPSIDPatch() {
    signed int fd;
    int skip = 0;
    //OpenPSID
    //if(cfg[19])
    {
        //Generate the patch
        *((unsigned short*) (&patchA[0]))
                = (unsigned short) (((unsigned int) sceOpenPSIDGetOpenPSID)
                        >> 16);
        *((unsigned short*) (&patchA[4]))
                = (unsigned short) ((unsigned int) &sceOpenPSIDGetOpenPSID);

        //Find the function we want!
        while (1) {
            int mod = sceKernelFindModuleByName(hookB[0].modname);
            if (mod == NULL) {
                sceKernelDelayThread(100);
                continue;
            }
            break;
        }
        unsigned int hookAddress = moduleFindFunc(
                moduleFindLibrary(
                        sceKernelSearchModuleByName(hookB[0].modname),
                        hookB[0].libname), hookB[0].nid);
        hookAddress = *((unsigned int*) hookAddress);

        //Open the PSID file
        fd = sceIoOpen("ms0:/seplugins/nitePR/nitePRimportant.bin",
                PSP_O_RDONLY, 0777);
        if (fd > 0) {
            sceIoRead(fd, psid, 16);
            sceIoClose(fd);
        } else {
            fd = sceIoOpen("ms0:/seplugins/nitePR/nitePRimportant.bin",
                    PSP_O_WRONLY | PSP_O_CREAT, 0777);
            if (fd > 0) {
                ((int(*)(char*)) hookAddress)(psid);
                sceIoWrite(fd, psid, 16);
                sceIoClose(fd);
            } else {
                skip = 1;
            }
        }

        if (skip == 0) {
            //Apply the memcpy
            sceKernelDcacheWritebackAll();
            sceKernelIcacheInvalidateAll();

            //Apply the patch
            memcpy(hookAddress, patchA, 16);

            //Apply the memcpy
            sceKernelDcacheWritebackAll();
            sceKernelIcacheInvalidateAll();
        }
    }
}

static void findGameId() {
    signed int fd;
    //Find the GAME ID
    do {
        fd = sceIoOpen("disc0:/UMD_DATA.BIN", PSP_O_RDONLY, 0777);
        sceKernelDelayThread(1000);
    } while (fd <= 0);
    sceIoRead(fd, gameId, 10);
    sceIoClose(fd);
    memcpy(&gameDir[22], gameId, 10);
    sprintf(krUi.game_id, "%s", gameId);
}

static void setupInitialVram() {
    //Set the VRAM to null, use the current screen
    pspDebugScreenInitEx(0x44000000, 0, 0);
    //vram = NULL;
    krUi.vram = NULL;
}

static void setupController() {
    //Setup the controller
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
}

static void handleScreenshot() {
    signed int fd;
    screenTime = 0;
    void *block_addr;
    void *frame_addr;
    int frame_width;
    int pixel_format;
    int sync = 1;
    u32 p;

    while (1) {
        sprintf(buffer, screenPath, screenNo);
        fd = sceIoOpen(buffer, PSP_O_RDONLY, 0777);
        if (fd > 0) {
            sceIoClose(fd);
            screenNo++;
        } else {
            break;
        }
    }

    if ((sceDisplayGetFrameBufferInternal(2, &frame_addr, &frame_width,
            &pixel_format, &sync) < 0) || (frame_addr == NULL)) {
    } else {
        p = (u32) frame_addr;
        if (p & 0x80000000) {
            p |= 0xA0000000;
        } else {
            p |= 0x40000000;
        }

        gamePause(thid);
        bitmapWrite((void *) p, NULL, pixel_format, buffer);
        gameResume(thid);

        screenNo++;
    }
}

static void showInterface() {
    CEFiveUi* prUi = &krUi;
    CEFiveConfig* prConfig = &krConfig;

    if (prConfig->pause_during_ui == 1) {
        gamePause(thid);
    }
    //Stop the game from receiving input (USER mode input block)
    sceCtrlSetButtonMasks(0xFFFF, 1); // Mask lower 16bits
    sceCtrlSetButtonMasks(0x10000, 2); // Always return HOME key

    //Setup a custom VRAM
    sceDisplaySetFrameBufferInternal(0, prUi->vram, 512, 0, 1);
    pspDebugScreenInitEx(prUi->vram, 0, 0);
    pspDebugScreenSetMaxX(69);
    pspDebugScreenSetMaxY(34);
    prUi->running = 1;
    krRunState = CES_UIShowing;
}

static void hideInterface() {
    pspDebugScreenSetBackColor((u32)0x00000000);
    pspDebugScreenClear();
    //Return the standard VRAM
    sceDisplaySetFrameBufferInternal(0, 0, 512, 0, 1);

    //Allow the game to receive input
    sceCtrlSetButtonMasks(0x10000, 0); // Unset HOME key
    sceCtrlSetButtonMasks(0xFFFF, 0); // Unset mask
    krRunState = CES_Running;
    // TODO: Get rid of this once it is no longer being used.
    menuDrawn = 0;
    gameResume(thid);
}

static void drawLoadedScreen() {
    int mode = 0;
    int width = 0;
    int height = 0;
    u32 text = (u32)0x00000000;
    static int r = 0;
    static int d = 1;

    sceDisplayGetMode(&mode, &width, &height);
    pspDebugScreenSetColorMode(mode);
    pspDebugScreenSetXY(0, 0);
    r += 1 * d;
    if (r < 0) {
        r = 0;
        d = 1;
    }
    if (r > 0xFF) {
        r = 0xFF;
        d = -1;
    }
    text |= r;
    pspDebugScreenSetTextColor(text);
    pspDebugScreenPuts("Cheat Engine of Five -- Press HOME Twice");
}

static void waitForVram() {
    //Has the HOME button been pressed?
    unsigned int a_address = 0;
    unsigned int a_bufferWidth = 0;
    unsigned int a_pixelFormat = 0;
    unsigned int a_sync;

    sceDisplayGetFrameBufferInternal(0, &a_address, &a_bufferWidth,
            &a_pixelFormat, PSP_DISPLAY_SETBUF_IMMEDIATE);

    if (a_address) {
        krUi.vram = (void*) (0xA0000000 | a_address);
        krStartState = CESS_Finished;
    } else {
        drawLoadedScreen();
    }
    sceDisplayWaitVblank();
}

static void start() {
    /* Initialize the CheatEngine */
    krCheatEngine.blocklist = block;
    krCheatEngine.cheatlist = cheat;

    cefiveconfig_init(&krConfig);
    krConfig.pause_during_ui = 0;
    sprintf(krConfig.plugins_dir, "seplugins");
    sprintf(krConfig.cefive_dir, "nitePR");

    // Initialize the SearchEngine
    searchengine_init(&krSearchEngine);
    searchengine_start(&krSearchEngine);

    /* Initialize the UI */
    krUi.prCEConfig = &krConfig;
    cefiveuiInit(&krUi, &krCheatEngine, &krSearchEngine);
    krRunState = CES_Starting;
    krStartState = CESS_WaitKernelLib;

    krConfig.rAppletConfig.rPanel.rColor.background = krUi.config.color.background;
    krConfig.rAppletConfig.rPanel.rColor.text = krUi.config.color.text;

    waitForKernelLibrary();
    findGameId();
    loadCheats();
    setupInitialVram();
    setupController();

    //Register the button callbacks
    unsigned int bmask = 0;
    bmask |= PSP_CTRL_CIRCLE;
    bmask |= PSP_CTRL_CROSS;
    bmask |= PSP_CTRL_DOWN;
    bmask |= PSP_CTRL_LEFT;
    bmask |= PSP_CTRL_LTRIGGER;
    bmask |= PSP_CTRL_NOTE;
    bmask |= PSP_CTRL_RIGHT;
    bmask |= PSP_CTRL_RTRIGGER;
    bmask |= PSP_CTRL_SELECT;
    bmask |= PSP_CTRL_SQUARE;
    bmask |= PSP_CTRL_START;
    bmask |= PSP_CTRL_TRIANGLE;
    bmask |= PSP_CTRL_UP;
    bmask |= PSP_CTRL_VOLDOWN;
    bmask |= PSP_CTRL_VOLUP;

    sceCtrlRegisterButtonCallback(3, bmask, buttonCallback, NULL);
}

int mainThread() {
    CEFiveUi* prUi = &krUi;
    SearchEngine* prSearch = &krSearchEngine;
    CheatEngine* prEngine = &krCheatEngine;
    GameInfo* prInfo = &prUi->gameinfo;

    running = 1;

    start();

    //Do the loop-de-loop
    while (running) {
        if (prUi->vram == NULL) {
        //if (krUi.vram == NULL) {
            waitForVram();
            continue;
        }
        /* Load the GameInfo struct in the UI */
        gameinfo_load(prInfo);
        //gameinfo_load(&krUi.gameinfo);
        
        if (krRunState == CES_UIRequest) {
            showInterface();
            while (prUi->running == 1) {
            //while (krUi.running == 1) {
                cefiveuiRedraw(prUi);
                //cefiveuiRedraw(&krUi);
                //cefiveuiHandleInput(&krUi);
                searchengine_run(prSearch);
                //searchengine_run(&krSearchEngine);
                cheatengineRefresh(prEngine);
                //cheatengineRefresh(&krCheatEngine);
                sceKernelDelayThread(50000);
            }
            hideInterface();
        }
        searchengine_run(prSearch);
        //searchengine_run(&krSearchEngine);
        cheatengineRefresh(prEngine);
        //cheatengineRefresh(&krCheatEngine);
        sceKernelDelayThread(50000);
    }
    return 0;
}

int _start(SceSize args, void *argp) {
    //Load the CFG
    if (cfg[4]) {
        SceModule *mod;
        while (1) {
            mod = sceKernelFindModuleByName(hookA[0].modname);
            if (mod == NULL) {
                sceKernelDelayThread(100);
                continue;
            }
            break;
        }
        moduleHookFunc(&hookA[0].modfunc, sceKernelSearchModuleByName(
                hookA[0].modname), hookA[0].libname, hookA[0].nid,
                hookA[0].func);
    }
    memcpy(&triggerKey, cfg + 11, 4);
    memcpy(&menuKey, cfg + 15, 4);
    memcpy(&screenKey, cfg + 20, 4);

    //Create thread
    sceKernelGetThreadmanIdList(SCE_KERNEL_TMID_Thread, thread_buf_start,
            MAX_THREAD, &thread_count_start);
    thid = sceKernelCreateThread("nitePRThread", &mainThread, 0x18, 0x1000, 0,
            NULL);

    //Start thread
    if (thid >= 0)
        sceKernelStartThread(thid, 0, NULL);

    return 0;
}

int _stop(SceSize args, void *argp) {
    running = 0;
    sceKernelTerminateThread(thid);
    return 0;
}

// vi:cin:et:ts=4 sw=4
