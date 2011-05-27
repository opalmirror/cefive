/* Cheat Engine of Five
 * Authors:
 * Sir Gee of Five
 * PSP_Lord
 */

//Crt0_prx
//Includes
//#define _PSP_FW_VERSION 150
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
#include "geelog.h"
#include "niteprio.h"

extern SceUID sceKernelSearchModuleByName(unsigned char *);

//Defines
PSP_MODULE_INFO("CEFive", 0x3007, CEFIVE_VERSION_MAJ, CEFIVE_VERSION_MIN);
PSP_MAIN_THREAD_ATTR(0); //0 for kernel mode too

//Globals
//unsigned char *gameDir = "ms0:/seplugins/nitePR/__________.txt";
static char gameDir[256];
unsigned char gameId[10];
unsigned char running = 0;
SceUID thid;
#define MAX_THREAD 64
static int thread_count_start, thread_count_now, pauseuid = -1;
static SceUID thread_buf_start[MAX_THREAD], thread_buf_now[MAX_THREAD];

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
//int module_start(SceSize args, void *argp) __attribute__((alias("_start")));
//int module_stop(SceSize args, void *argp) __attribute__((alias("_stop")));
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

void cheatEnable(unsigned int a_cheat) {
    cheatengineApplyCheat(&krCheatEngine, a_cheat);
}

void cheatDisable(unsigned int a_cheat) {
    cheatengineResetCheat(&krCheatEngine, a_cheat);
}

void cheatApply(unsigned char a_type) {
    // This needs to be removed once I figure out what cheatSaved is.
    if (!cheatSaved) {
        return;
    }
    cheatengineRefresh(&krCheatEngine);
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
}

static void drawExtDiffSearch() {
}

static void drawExtSearch() {
}

static void drawExtTextSearch() {
}

static void drawPrx() {
}

static void drawSearcher() {
}

static void drawSplashScreen() {
}

static void drawTabs() {
}

static void drawTracker() {
}

void buttonCallback(int curr, int last, void *arg) {
    int cvd = (curr & PSP_CTRL_VOLDOWN);
    int cvu = (curr & PSP_CTRL_VOLUP);
    int cno = (curr & PSP_CTRL_NOTE);
    int lvd = (last & PSP_CTRL_VOLDOWN);
    int lvu = (last & PSP_CTRL_VOLUP);
    int lno = (last & PSP_CTRL_NOTE);

    if (krUi.vram == NULL) {
        geelog_log(LOG_WARN, 
                "buttonCallback: Current VRAM pointer is NULL.");
        return;
    }

    /* If the UI is already showing, delegate to the ui. */
    if (krUi.running == 1) {
        CEFiveUi* prUi = &krUi;
        if (cvd && cvu && (!lvd || !lvu)) {
            geelog_log(LOG_DEBUG, 
                    "buttonCallback: User Interface Requested while visible.");
        }
        cefiveui_buttoncallback(curr, last, prUi);
        return;
    }

    /* If the Volume Up and Volume Down keys are being held */
    if (cvd && cvu && (!lvd || !lvu)) {
        geelog_log(LOG_DEBUG,
                "buttonCallback: User Interface Requested.");
        krUi.drawn = 0;
        krRunState = CES_UIRequest;
    }

    /* If the Music Button has been pressed. */
    if (cno && !lno) {
        cheatStatus = !cheatStatus;
        if (krCheatEngine.trigger_active == 0) {
            geelog_log(LOG_DEBUG, "buttonCallback: Activating Cheats.");
            cheatengineActivateCheats(&krCheatEngine);
        } else {
            geelog_log(LOG_DEBUG, 
                    "buttonCallback: Deactivating Cheats.");
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

int sceOpenPSIDGetOpenPSID(char *openpsid) {
    memcpy(openpsid, psid, 16);
    return 0;
}

static void clearSearchHistory() {
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
    sprintf(gameDir, "ms0:/seplugins/nitePR/%s.txt", gameId);
    sprintf(krUi.game_id, "%s", gameId);
    strcpy(krConfig.game_id, gameId);
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

/* Pause the game if configured to do so, stop the game from receiving button
 * input, then swap the vram for the debug frame buffer, and initialize the 
 * screen.
 */
static void showInterface() {
    CEFiveUi* prUi = &krUi;
    CEFiveConfig* prConfig = &krConfig;

    if (prConfig->pause_during_ui == 1) {
        geelog_log(LOG_DEBUG, "showInterface: Pausing Game.");
        gamePause(thid);
    }
    
    geelog_log(LOG_DEBUG, "showInterface: Blocking input from Game.");
    //Stop the game from receiving input (USER mode input block)
    sceCtrlSetButtonMasks(0xFFFF, 1); // Mask lower 16bits
    sceCtrlSetButtonMasks(0x10000, 2); // Always return HOME key

    geelog_log(LOG_DEBUG, "showInterface: Initializing frame buffer.");
    //Setup a custom VRAM
    sceDisplaySetFrameBufferInternal(0, prUi->vram, 512, 0, 1);
    pspDebugScreenInitEx(prUi->vram, 0, 0);
    pspDebugScreenSetMaxX(69);
    pspDebugScreenSetMaxY(34);
    pspDebugScreenClearLineDisable();
    prUi->running = 1;
    geelog_log(LOG_DEBUG, 
            "showInterface: Setting Run State to UI Showing.");
    krRunState = CES_UIShowing;
}


/* Hide the Cheat Engine of Five interface, return to the game screen, return
 * input to the game, attempt to save the configuration.
 */
static void hideInterface() {
    CEFiveConfig* prConfig = &krConfig;
    
    geelog_log(LOG_DEBUG, "hideInterface: Clearing Interface.");
    pspDebugScreenSetBackColor((u32)0x00000000);
    pspDebugScreenClear();
    
    geelog_log(LOG_DEBUG, "hideInterface: Returning frame buffer.");
    //Return the standard VRAM
    sceDisplaySetFrameBufferInternal(0, 0, 512, 0, 1);

    geelog_log(LOG_DEBUG, "hideInterface: Clearing input mask.");
    //Allow the game to receive input
    sceCtrlSetButtonMasks(0x10000, 0); // Unset HOME key
    sceCtrlSetButtonMasks(0xFFFF, 0); // Unset mask
    
    geelog_log(LOG_DEBUG, "hideInterface: Saving configuration.");
    /* Try to save the current CEFiveConfig. */
    cefiveconfig_save(prConfig, "ms0:/seplugins/CEFive.cdf");
    krRunState = CES_Running;
    geelog_log(LOG_DEBUG, "hideInterface: Resuming Game.");
    gameResume(thid);
}

/** Draw the Loaded Screen
*
* @param mode u8 specifying the color mode assigned in waitForVram()
*/
static void drawLoadedScreen(u8 mode) {
    u32 text = (u32)0x00000000;
    static int r = 0;
    static int d = 1;
    
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

/** Wait for Video RAM to be available.  Display a message over the default
 * active FrameBuffer prompting the user to press the HOME button twice.
 * 
 */
static void waitForVram() {
    //Has the HOME button been pressed?
    unsigned int a_address = 0;
    unsigned int a_bufferWidth = 0;
    unsigned int a_pixelFormat = 0;
    unsigned int a_sync;
    
    /* variable to hold the pixelFormat of default frame buffer */
    unsigned int a_pixelFormat2 = 0;
    
    /* retrieve the pixel format of the default frame buffer we setted up to print the info line */
    sceDisplayGetFrameBuf(&a_address, &a_bufferWidth, &a_pixelFormat2, &a_sync);

    sceDisplayGetFrameBufferInternal(0, &a_address, &a_bufferWidth,
            &a_pixelFormat, PSP_DISPLAY_SETBUF_IMMEDIATE);
       

    if (a_address) {
        krUi.vram = (void*) (0xA0000000 | a_address);
        krStartState = CESS_Finished;     
    }
        /* apply the correct color mode so the info line will be printed in correct size */
        else if(a_pixelFormat2 > 2) {
            drawLoadedScreen(3);
        }
    else {
            drawLoadedScreen(0);
    }
    sceDisplayWaitVblank();
}

static void start() {
    CEFiveConfig* prCfg = &krConfig;
    CheatEngine* prEng = &krCheatEngine;
    SearchEngine* prSearch = &krSearchEngine;
    CEFiveUi* prUi = &krUi;
    int r = 0;

    /* Initialize the CeFive Configuration. */
    geelog_log(LOG_DEBUG, "start: Initializing CEFiveConfig.");
    cefiveconfig_init(prCfg);
    
    /* Attempt to load the current configuration. */
    geelog_log(LOG_DEBUG, "start: Loading configuration.");
    r = cefiveconfig_load(prCfg, "ms0:/seplugins/CEFive.cdf");
    if (r != CEFIVECONFIG_SUCCESS) {
        geelog_log(LOG_ERROR, "start: Error loading configuration.");
    } else {
        geelog_log(LOG_INFO, "start: Loaded configuration.");
    }
    sprintf(prCfg->plugins_dir, "seplugins");
    sprintf(prCfg->cefive_dir, "nitePR");

    geelog_log(LOG_DEBUG, "start: Initializing CheatEngine.");
    /* Initialize the CheatEngine */
    r = cheatengineInit(prEng, prCfg, cheat, block);

    geelog_log(LOG_DEBUG, "start: Initializing SearchEngine.");
    // Initialize the SearchEngine
    searchengine_init(prSearch);
    searchengine_start(prSearch);

    geelog_log(LOG_DEBUG, "start: Initializing UI.");
    /* Initialize the UI */
    prUi->prCEConfig = prCfg;
    cefiveuiInit(prUi, prEng, prSearch);
    krRunState = CES_Starting;
    krStartState = CESS_WaitKernelLib;

    /* I was stoned or something
    krConfig.rAppletConfig.rPanel.rColor.background = krUi.config.color.background;
    krConfig.rAppletConfig.rPanel.rColor.text = krUi.config.color.text;
    */
    
    geelog_log(LOG_DEBUG, "start: waiting for Kernel.");
    waitForKernelLibrary();
    geelog_log(LOG_DEBUG, "start: loading Game Id.");
    findGameId();
    geelog_log(LOG_DEBUG, "start: loading cheats.");
    niteprio_import(prEng, gameDir);
    
    geelog_log(LOG_DEBUG, "start: Getting Initial VRAM.");
    setupInitialVram();
    geelog_log(LOG_DEBUG, "start: Initializing Controller.");
    setupController();

    geelog_log(LOG_DEBUG, "start: Registering Button Callbacks.");
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
    geelog_log(LOG_DEBUG, "start: started.");
}

/* Main thread of cefive.
 */
int mainThread() {
    const char* sFunc = "mainThread";
    CEFiveUi* prUi = &krUi;
    SearchEngine* prSearch = &krSearchEngine;
    CheatEngine* prEngine = &krCheatEngine;
    GameInfo* prInfo = &prUi->gameinfo;

    running = 1;

    geelog_flog(LOG_DEBUG, sFunc, "Starting CEFive.");
    start();
    geelog_flog(LOG_DEBUG, sFunc, "CEFive Started.");

    //Do the loop-de-loop
    while (running) {
        if (prUi->vram == NULL) {
            waitForVram();
            continue;
        }
        if (prInfo->loaded == 0) {
            geelog_flog(LOG_DEBUG, sFunc, "Loading Game Info.");
            gameinfo_load(prInfo);
        }
        if (krRunState == CES_UIRequest) {
            geelog_flog(LOG_DEBUG, sFunc, "Showing Interface.");
            showInterface();
            while (prUi->running == 1) {
                if (prUi->vram == NULL) {
                    geelog_flog(LOG_DEBUG, sFunc, 
                            "Invalid VRAM Pointer, shutting down UI.");
                    prUi->running = 0;
                    continue;
                }
                if (krRunState == CES_UIRequest) {
                    geelog_flog(LOG_DEBUG, sFunc, 
                            "UI Requested, Showing Interface.");
                    showInterface();
                }
                cefiveui_update_controls(prUi);
                cefiveuiRedraw(prUi);
                searchengine_run(prSearch);
                cheatengineRefresh(prEngine);
                sceKernelDelayThread(50000);
            }
            geelog_flog(LOG_DEBUG, sFunc, "Hiding Interface.");
            hideInterface();
        }
        searchengine_run(prSearch);
        cheatengineRefresh(prEngine);
        sceKernelDelayThread(50000);
    }
    
    geelog_flog(LOG_INFO, sFunc, "CEFive Stopped.");
    geelog_stop();
    return 0;
}

int module_start(SceSize args, void *argp) {
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

    /* Start the Logger. */
    geelog_init("ms0:/seplugins/CEFive.log");
    geelog_start(LOG_DEBUG);
    
    //Create thread
    sceKernelGetThreadmanIdList(SCE_KERNEL_TMID_Thread, thread_buf_start,
            MAX_THREAD, &thread_count_start);
    thid = sceKernelCreateThread("CEFive_Thread", &mainThread, 0x18, 0x1000, 0,
            NULL);

    //Start thread
    if (thid >= 0)
        sceKernelStartThread(thid, 0, NULL);

    return 0;
}

int module_stop(SceSize args, void *argp) {
    running = 0;
    geelog_stop();
    sceKernelTerminateThread(thid);
    return 0;
}
