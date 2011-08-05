/**
  * This is reversed code from Darkfrost_v4 made by demon450 using nitePR/MKultra/PSPLink/PSP docs source code.
  * The code does not (and will not) fully match the original darkfrost_v4 source in detail,
  * but overall it does. Please keep in mind that this code can change in any new revision (even already reversed code).
  *
  * The main concern is the reversing of the mainThread function
  * to find out how Darkfrost has been worked out.
  * 
  * CONTRIBUTION: If you want to contribute, please download this code file and the ASM code of Darkfrost
  *               and send me the new file to be uploaded. You can contact me through my profile on 
  *               http://www.onehitgamer.com/forum/conversations/add?to=PSP_Lord. Your name will then be added
  *               to the contributers.
  * 
  * REVISION: 11
  *             (Revision Info: mainThread has been reversed, only buttonCallback is not complete
  *                             for now. Version is currently not compilable.)  
  *
  * Reversed by: PSP_Lord (Thanks to TyRaNiD for his prxtool.)
**/

/* includes */
#include <pspkernel.h>
#include <pspmodulemgr.h>
#include <pspiofilemgr.h>
#include <pspdisplay.h>
#include <pspdisplay_kernel.h>
#include <pspctrl.h>
#include <pspctrl_kernel.h>
#include <string.h>
#include <stdint.h>
#include <psputility_sysparam.h>

#include "screenshot.h" //written by TyRaNiD

extern SceUID sceKernelSearchModuleByName(const char *); //ModuleMgrForKernel_04B7BD22

PSP_MODULE_INFO("darkfrost", 0x3007, 1, 2); //attributes same as nitePR, what a surprise...
PSP_MAIN_THREAD_ATTR(0);

/* structures */

//struct 0x000228C8
typedef struct _UnkStruct_228C8 {
     u16 val1; //0
     u16 val2; //2
     u8  val3; //4
     u8  val4; //5
} UnkStruct_228C8;


/* global variables */

int thid; //0x00041974
int running; //0x000212E4
int idCount; //0x000226AC
int idCount2; //0x000226B0
int screenShotNumber; //0x000213E0
u8 syphonFilterCombatOps; //0x000212EC
u8 resistanceRetribution; //0x000212ED
u8 validScreenShotCombo; //0x0002130A
u8 isPsnGame; //0x000212E8
u8 menuDrawn; //0x00021309
u8 screenShotRequest; //0x000213D1

//why no structure?
u32 colorPrimary = 0xFFFFFFFF; //0x0000207A4
u32 colorSecondary = 0xFFF6CF00; //0x0000207A8
u32 colorTurshiary = 0xFFFF0000; //0x0000207AC
u32 colorBackground = 0xFF300000; //0x0000207B0
u32 colorBrowser1 = 0xFFFFFFFF; //0x00002077C
u32 colorBrowser2 = 0xFFFFFFFF; //0x000020780

//why no structure, why demon, why?
u32 menuKey = 0x00300000; //0x0002075C -- PSP_CTRL_VOLUP | PSP_CTRL_VOLDOWN
u32 cheatTriggerKey = 0x00800000; //0x00020760 -- PSP_CTRL_NOTE
u32 screenShotKey = 0x00008040; //0x00020764 -- PSP_CTRL_SQUARE | PSP_CTRL_DOWN

char psid[16]; //0x00041D30
char gameID[10]; //0x00040D28
char *gameDirNitePR = "ms0:/seplugins/nitePR/__________.txt"; //0x00020758
char *gameDirDF = "ms0:/darkfrost/codes/__________.txt"; //0x00020754
char screenShotPathTemplate[43] = "ms0:/darkfrost/screenshots/screenshot%d.bmp" ; //0x000207F0
char buffer[64]; //0x00041978
char firmPart1[4]; //0x00041D28
char firmPart2[1]; //0x00041D40
char nickName[25]; //0x00041958
char notePadContent[41]; //0x000419B8

void *vram; //0x00041D2C

SceUID readBuf[64]; //0x000226B4
SceUID readBuf2[64]; //0x000227B4
SceUID tempId; //0x00020AC0

UnkStruct_228C8 unkStruct_228C8; //0x000228C8

SceModule *mod = NULL;
SceLibraryEntryTable *libEntryTable = NULL;

/* unknown Variables */
char unk_212F4;
char unk_22130;
char unk_2130C;
char unk_213D1;
char unk_20768;
char unk_213D0;
char unk_21300;
char unk_212F0;
char unk_213E7;
char unk_212F8;


/* Functions */
void patchPsnGames();
void loadCheats();
int blockAdd(int, char *);
void setButtonConfig();
void setColorConfig();
void readNotePad();
void psnablerGetFirmVersion();
char encryptByte(int, char);
void manipulatePsid(char *);
void resolvePsid(char *);
u32 *findNidAddress(SceLibraryEntryTable *, u32);
void getGameID();
SceLibraryEntryTable *findLibEntryTableByModule(SceUID, const char *);
void countScreenShots();
void sub_0000F384();


static const unsigned char patchA[]= {
     0x21, 0x88, 0x02, 0x3c, //lui v0, $8821
     0x21, 0x10, 0x42, 0x34, //ori v0, v0, $1021
     0x08, 0x00, 0x40, 0x00, //jr v0
     0x00, 0x00, 0x00, 0x00 //nop
}; 


/* 0x00005C70
 * 
 * TODO: Needs to be reversed completely
 */ 
void buttonCallback(int curr, int last, void *arg) {

    //0x00005C80	
    if (vram == NULL) {
        return; //0x00005CC4
    }
    //0x00005C94 && 0x00005CF8
    if ((menuKey & curr) == menuKey && !menuDrawn) {
      
        menuDrawn = 1; //0x00005D20
        
        //0x00005D18    
        if (!(unk_212F4 < unk_22130)) {
             return; //0x00005CC4
        }
        unk_22130 = 0; //0x00005D28 -- 0x00022130: 0x00, 0x00022131: 0x00, 0x00022132: 0x00, 0x00022133: 0x00
        return; //0x00005CC4
    }   
    else if ((screenShotKey & curr) == screenShotKey && !menuDrawn) { //0x00005CA8 && 0x00005D30
    
              screenShotRequest = 1; //0x00005D54
    }
         
    /* TODO: Needs to be further reversed! */    
}


/* Subroutine sub_0000380C - Address 0x0000380C
 *
 * global variables:
 *	  SceUID tempId; //0x00020AC0
 * 	  u32 idCount; //0x000226AC
 *        u32 idCount2; //0x000226B0
 *
 *@return void
 */
void gamePause(SceUID thid) {

    int count; 
    int count2;
    SceUID tempThid;

    if (tempId < 0) { //0x00003838
    
        tempId = thid; //0x00003884
        sceKernelGetThreadmanIdList(SCE_KERNEL_TMID_Thread, readBuf2, sizeof readBuf2 / sizeof (SceUID), idCount2); //0x00003880
       
        for (count = 0; count < idCount2; count++) { //0x0000389C / 0x0000388C && 0x000038FC / 0x000038F4 && 0x00003900
        
             tempThid = readBuf2[count]; //0x000038B0
            
             for (count2 = 0; count2 < idCount; count2++) { //0x000038CC / 0x000038E0 / 0x000038D8 && 0x000038D4
            
                  if (tempThid != readBuf[count2] || tempThid != thid) { //0x000038B8 / 0x000038D0 && 0x000038C0                
                      ;
                  }
	          else {
                        break;
                  }                       
            }
	    sceKernelSuspendThread(tempThid);
        }
    }               	
}

/* Subroutine sub_0000392C - Address 0x0000392C
 *
 * global variables:
 *	  SceUID tempId; //0x00020AC0
 * 	  u32 idCount; //0x000226AC
 *        u32 idCount2; //0x000226B0
 *
 * @return void
 */
void gameResume(SceUID thid) {
 
    int count;
    int count2;
    SceUID tempThid;
    
    if (tempId != thid) { //0x00003954
        
        tempId = -1; //0x00003994
        for (count = 0; count < idCount2; count++) { //0x00003990

            tempThid = readBuf2[count]; //0x000039BC
            for (count2 = 0; count2 < idCount; count2++) { //0x000039D8 && 0x000039B8 / 0x000039EC && 0x000039E4
               
                if(tempThid!= readBuf[count2] || tempThid != tempId) { //0x000039C4 && 0x000039CC && 0x000039DC	
                   ;
                }
                else {
                      break;
                }
            }            
            sceKernelResumeThread(tempThid);	
        }
    }
}
		   		       

//sub_00000000 -- analyzed function by comparing both MKultra and darkfrost compiled source code
u32 char2hex(unsigned char *hexString, u32 *val){        
         
   u32 dword=0;
   u32 power=0;
  
   while(power < 8) {
               
         switch(hexString[power]) {
                               
             case '_': break;
             case '0': dword|=0 * (1 << (4*(7-power))); break;
             case '1': dword|=1 * (1 << (4*(7-power))); break;
             case '2': dword|=2 * (1 << (4*(7-power))); break;
             case '3': dword|=3 * (1 << (4*(7-power))); break;
             case '4': dword|=4 * (1 << (4*(7-power))); break;
             case '5': dword|=5 * (1 << (4*(7-power))); break;
             case '6': dword|=6 * (1 << (4*(7-power))); break;
             case '7': dword|=7 * (1 << (4*(7-power))); break;
             case '8': dword|=8 * (1 << (4*(7-power))); break;
             case '9': dword|=9 * (1 << (4*(7-power))); break;
             case 'a':case 'A': dword|=0xA * (1 << (4*(7-power))); break;
             case 'b':case 'B': dword|=0xB * (1 << (4*(7-power))); break;
             case 'c':case 'C': dword|=0xC * (1 << (4*(7-power))); break;
             case 'd':case 'D': dword|=0xD * (1 << (4*(7-power))); break;
             case 'e':case 'E': dword|=0xE * (1 << (4*(7-power))); break;
             case 'f':case 'F': dword|=0xF * (1 << (4*(7-power))); break;
             default: dword>>=4*(8-power); *val=power; return dword;
         }
         power++;
   }
  
   val=8;
   return dword;
}


/* Subroutine sub_000197FC - Address 0x000197FC 
 * 
 * The mainThread function of Darkfrost_V4
 *
 */
int mainThread() {

    int retVal;
    int fd;
    SceUID id;
    u32 *nidAddr;
    
	sceKernelDelayThread(100000); //Wait 100 milliseconds -- why are you stopping the thread?
	
	while (sceKernelFindModuleByName("sceKernelLibrary") == NULL) {  //LoadCoreForKernel_CF8A41B1
	       sceKernelDelayThread(200000); //wait 200 milliseconds
	}
	
	retVal = sceIoMkdir("ms0:/darkfrost", 0777);
	if (retVal == 0) { //0x000198A4
		
		//loc_00019EE0
		sceIoMkdir("ms0:/darkfrost/", 0777);
        }
	retVal = sceIoMkdir("ms0:/darkfrost/searches", 0777);
	if (retVal == 0) { //0x000198BC
		
		//loc_00019ECC
		sceIoMkdir("ms0:/darkfrost/searches/", 0777);
        }
			
	retVal = sceIoMkdir("ms0:/darkfrost/codes", 0777);
	if (retVal == 0) { //0x000198D4:
			
		//loc_00019EB8
		sceIoMkdir("ms0:/darkfrost/codes/", 0777);
        }
	retVal = sceIoMkdir("ms0:/darkfrost/config", 0777);
	if (retVal == 0) { //0x000198EC
				
	    //loc_00019EA4
		sceIoMkdir("ms0:/darkfrost/config/", 0777);
        }
	retVal = sceIoMkdir("ms0:/darkfrost/dumps", 0777);
	if (retVal == 0) { //0x00019904
					
		//loc_00019E90
		sceIoMkdir("ms0:/darkfrost/dumps/", 0777);
        }
	retVal = sceIoMkdir("ms0:/darkfrost/screenshots", 0777);
	if (retVal == 0) { //0x0001991C
							
        //loc_00019E7C
        sceIoMkdir("ms0:/darkfrost/screenshots/", 0777);
	}
	
	//sub_00003768
	countScreenShots();
	
	//0x0001998C
	while (sceKernelFindModuleByName("sceOpenPSID_Service") == NULL) {  //LoadCoreForKernel_CF8A41B1
		   sceKernelDelayThread(100); //0x00019984 -- ThreadManForKernel_CEADEB47
	}
	//0x0001999C...useless, FindModuleByName already returns pointer to SceModule struct holding modname, modid, etc...
	id = sceKernelSearchModuleByName("sceOpenPSID_Service");	
        libEntryTable = findLibEntryTableByModule(id, "sceOpenPSID_driver"); //0x000199AC	
        //0x000199C0
	nidAddr = findNidAddress(libEntryTable, 0xC69BEBCE); //0xC69BEBCE is the NID of sceOpenPSIDGetOpenPSID
	
	//Open the PSID file
	fd = sceIoOpen("ms0:/darkfrost/config/DFimportant.bin", PSP_O_RDONLY, 0777); //0x000199D4
	if (fd > 0) {
          
            //0x000199F0
            sceIoRead(fd, psid, sizeof psid); //global variable "psid" reference
            sceIoClose(fd);
        }
        //loc_00019EF4
        else {
             fd = sceIoOpen("ms0:/darkfrost/config/DFimportant.bin", PSP_O_CREAT | PSP_O_WRONLY, 0777);
             //0x00019F04
             if (fd > 0) {
               
                 //0x00019F20
                 psid = *(char *) nidAddr;
                 sceIoWrite(fd, psid, sizeof psid); //original MKULTRA code: ((int(*)(char*))hookAddress)(psid);
                 sceIoClose(fd);
             }
             //loc_00019F90
             else {
                  //loc_00019A50
                  goto loc_00019A68;
             }     
        }
        //code copying...look at nitePR source
        //00019A00
	sceKernelDcacheWritebackAll();
	sceKernelIcacheInvalidateAll();
	
	//0x00019A10 - 0x00019A3C
	memcpy(nidAddr, patchA, 16);
		
	//0x00019A40
	sceKernelDcacheWritebackAll();
	sceKernelIcacheInvalidateAll();
	
	//skip patch
	loc_00019A68:
                
         int length = strlen(gameID); //gameID[10] (0x00040D28)              
         if (length != 10) { //0x00019A70       
                   
             getGameID(); //0x00019A60
         }     
         //0x00019A88
         if (strncmp(gameID, "NPUG-80114", sizeof gameID) == 0) { //Syphon Filter Combat Ops [US]
                           
             syphonFilterCombatOps = 1; //0x00019E28
         }
             //0x00019E44
             else if (strncmp(gameID, "UCUS-98668", sizeof gameID) == 0) { //Resistance Retribution [US]                 
                      resistanceRetribution = 1; //0x00019AB4
             }
             //0x00019E60
             else if (strncmp(gameID, "UCES-01184", sizeof gameID) == 0) {  //Resistance Retribution [EU]       
                      resistanceRetribution = 1; //0x00019AB4 
             } 
             
        //0x00019AB8   
        psnablerGetFirmVersion();
        //0x00019AC4       
        if (syphonFilterCombatOps) {
           
            isPsnGame = 1; //0x00019D18
        }  
        //0x00019AD4
        else if (resistanceRetribution) {
                isPsnGame = 1; //0x00019D18
        }
       
       //0x00019AE8
       sceUtilityGetSystemParamString(PSP_SYSTEMPARAM_ID_STRING_NICKNAME, nickName, sizeof nickName);
       readNotePad(); //0x00019AF0
       setColorConfig(); //0x00019AF8
       setButtonConfig(); //0x00019B00
       
       loadCheats(); //0x00019B08
       resolvePsid(psid); //0x00019B10
       
       pspDebugScreenInitEx(0x44000000, 0, 0); //0x00019B20 -- pointer to beginning of VRAM Memory
       vram = NULL;
       
       //0x00019B2C
       sceCtrlSetSamplingCycle(0); 
       sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
            
       u32 bmask = menuKey | cheatTriggerKey | screenShotKey; //0x00019B3C - 0x00019B50    
       sceCtrlRegisterButtonCallback(3, bmask, buttonCallback, NULL); //0x00019B68
                        
       while (running) { //0x00019B8C
       
           if (vram == NULL) { //0x00019B98
           
               u32 intFb_address = 0;
               u32 intFb_bufferWidth = 0;
               u32 intFb_pixelFormat = 0;
               u32 intFb_sync = 0;
           
               sceDisplayGetFrameBufferInternal(0, &intFb_address, &intFb_bufferWidth, &intFb_pixelFormat, &intFb_sync); //0x00019DFC -- sceDisplay_driver_5B5AEFAD
               
               if (intFb_address) { //0x00019E08
                  
                   vram = (void *) (intFb_address | 0xA0000000); //0x00019E14
               }
               else {              
               	     u32 pmode = 0;
               	     u32 pwidth = 0;
               	     u32 pheight = 0;
               	     	
                     sceDisplayGetMode(&pmode, &pwidth, &pheight); //0x00019F40
                     
                     pspDebugScreenSetColorMode(pmode); //0x00019F48
                     pspDebugScreenSetXY(22, 0); //0x00019F54
                     pspDebugScreenSetTextColor(0xFFFF0000); //0x00019F5C     
                     pspDebugScreenPuts("Welcome to DarkFrost v4!"); //0x00019F68
                     pspDebugScreenSetXY(10, 1); //0x00019F74
                     pspDebugScreenPuts("Double Tap the \"HOME\" button then press Vol +, -"); //0x00019F80      
               }              
               sceKernelDelayThread(1500); //0x00019E18  
               continue; //0x00019E20         	
           }
	   if (menuDrawn) { //0x00019BA0
	   
	        sceCtrlSetButtonMasks(0xFFFF, 1); //0x00019D20 -- sceCtrl_driver_7CA723DC
	        sceCtrlSetButtonMasks(1, 2); //0x00019D2C
	        
	        sceDisplaySetFrameBufferInternal(0, vram, 512, 0, 1); //0x00019D44 -- sceDisplay_driver_63E22A26
	        pspDebugScreenInitEx(vram, 0, 0); //0x00019D54
	        
	        if (unk_2130C != 0) { //0x00019D64
	           
	            gamePause(thid); //0x00019DC4           
	        }
	        
	        sub_0000F384(); //0x00019D70 - sub man function
	        gameResume(thid); //0x00019D78
	        
	        sceDisplaySetFrameBufferInternal(0, 0, 512, 0); //0x00019D90
	        
	        sceCtrlSetButtonMasks(1, 0); //0x00019D9C
	        sceCtrlSetButtonMasks(0xFFFF, 0); //0x00019DA8
	    }
	    
	    if (unk_213D1 != 0 && screenShotPathTemplate[0] != 0) { //0x00019BE8 &&0x00019BF4
	    
	        unk_213D1 = 0; //0x00019C04
	        sprintf(buffer, screenShotPathTemplate, screenShotNumber); //0x00019C24
	        fd = sceIoOpen(buffer, PSP_O_RDONLY, 0777); //0x00019C34
	        while (fd > 0) { //0x00019C3C
	       
	               sceIoClose(fd); //0x00019C08
	               screenShotNumber++;
	               sprintf(buffer, screenShotPathTemplate, screenShotNumber); //0x00019C24
	               fd = sceIoOpen(buffer, PSP_O_RDONLY, 0777); //0x00019C34	           
	        }
	       
	        u32 intFb_address = 0;
                u32 intFb_bufferWidth = 0;
                u32 intFb_pixelFormat = 0;
                u32 intFb_sync = 0;
               
                u32 tempFrameBuffer;
	       
	        retVal = sceDisplayGetFrameBufferInternal(2, &intFb_address, &intFb_bufferWidth, &intFb_pixelFormat, &intFb_sync); //0x00019C54
	        if (retVal == 0) { //0x00019C5C
	       
	            if (intFb_address != 0) { //0x00019C68
	           
	                if(intFb_address < 0) { //0x00019C70
	               
	                   tempFrameBuffer = (intFb_address | 0x40000000); //0x00019C78	                  
	                }
	                else {
	                      tempFrameBuffer = (intFb_address | 0xA0000000); //0x00019E78
	                }
	               
	                gamePause(thid); //0x00019C80 
	                bitmapWrite((void *)tempFrameBuffer, 0, intFb_pixelFormat, buffer); //0x00019C94 -- Thanks To TyRaNiD
	                gameResume(thid); //0x00019CA0
	               
	                screenShotNumber++; //0x00019CB0	               
	            }   
	        }
	       
	        patchPsnGames(); //0x00019CB4
	       
	        if (unk_20768 == 0) { //0x00019CCC
	       
	            sceKernelDelayThread(500000); //0x00019CC8           
	        }
	        else {
	              sceKernelDelayThread(unk_20768); //0x00019CC8
	        }
           }
        }
       return 0;
}


/* Subroutine sub_00000348 - Address 0x00000348
 *
 * global variables:  
 *        u8 isPsnGame; //0x000212E8  
 *        u8 syphonFilterCombatOps; //0x000212EC
 *        u8 resistanceRetribution; //0x000212ED
 *        char firmPart1[4]; //0x00041D28
 *        char firmPart2[1]; //0x00041D40  
 *        
 * @return void
 */   
void patchPsnGames() {

    if (isPsnGame) { //0x00000350

	if (syphonFilterCombatOps) { //0x0000035C

   	    int unk_1 = *0x09FAC940;
   	    if (unk_1 == 0x09E37980) { //0x00000378
   	 
   	        *0x09D27ACC = firmPart1; //0x000003FC
   	        *0x09D27AD0 = firmPart2; //0x00000400
   	        *0x09D27A84 = firmPart1; //0x00000404
   	        *0x09D27A88 = firmPart2; //0x0000040C   	        
   	    }
   	}
   	if (resistanceRetribution) { //0x00000388
   
   	    int unk_1 = *0x09FAA949; //0x00000394
   	    if (unk_1 == 0x09D2E178) { //0x000003A0
   	        
   	        *0x09D2E2C4 = firmPart1; //0x000003CC
   	        *0x09D2E2C8 = firmPart2; //0x000003D0
   	        *0x09D2E27C = firmPart1; //0x000003D4
   	        *0x09D2E280 = firmPart2; //0x000003DC
   	    }
   	}
     }
}
   
   

/* Subroutine sub_00004D64 - Address 0x00004D64
 *
 * global variables: 
 *        unkStruct_228C8; //0x000228C8
 *        char *gameDirNitePR = "ms0:/seplugins/nitePR/__________.txt"; //0x00020758 
 *        char *gameDirDF = "ms0:/darkfrost/codes/__________.txt" //0x00020754
 *        char buffer[64]; //0x00041978  
 *        char unk_213D0;
 *        char unk_21300; 
 *        char unk_212F0;
 *        char unk_212F4;
 *        char unk_212F8;
 *        char unk_213E7;
 *        
 * @return void
 */   
void loadCheats() {

    int fd;
    int pos;
    char unk_$s1;
    char unk_$s3;

    if (unk_213D0 != 0) { //0x00004D98
    
         int count = 0; //0x0000510C
         unk_21300 = count; //0x0000511C
         while (count < unk_212F4) { //0x00005118 && 0x00005160
              
                int unk_1 = unk_21300 * 46; //0x00005138   
                      
                unkStruct_228C8[unk_1].val1 = 0; //0x00005148
                unkStruct_228C8[unk_1].val2 = 0; //0x0000514C
                unkStruct_228C8[unk_1].val3 = 0; //0x00005154 	              
                strcpy(unkStruct_228C8[unk_1].val4, NULL); //0x00005150         
        }
        unk_212F0 = 0; //0x00005170
        unk_212F4 = 0; //0x00005174
        sceKernelDelayThread(1500); //0x00005178         
    }       	
	   
    fd = sceIoOpen((unk_213E7 != 0) ? gameDirNitePR : gameDirDF, PSP_O_RDONLY, 0777); //0x00004DA8 && 0x00004DBC
	    
    if (fd > 0) { //0x00004DC4
	        
	pos = sceIoLseek(fd, 0, SEEK_END); //0x00004DD8
	sceIoLseek(fd, 0, SEEK_SET); //0x00004DF0
	        
	while (unk_212F4 < pos) {  //0x00004F8C
	        
	       sceIoRead(fd, buffer, 1); //0x00004E1C
	        
	       if (buffer[0] == '\r' || buffer[0] == '\n') { //0x00004E28 && 0x00004E30 -- 0xA = Linefeed, 0xD = Carriage return
	      
	           if (unk_$s1 == 0) { //0x00004F6C
	                
	               sceIoLseek(fd, 0, SEEK_CUR); //0x00004F80	               
	           }
	           else {	                   
	                 unk_212F4++; //0x00004FA4
	                 unk_$s1 = 0; //0x00004FC4
	           }
	            sceIoLseek(fd, 0, SEEK_CUR); //0x00004FB0	                   
	       }
	       else if (buffer[0] == ';') { //0x00004E38
	                        
	                if (unk_$s1 == 0) { //0x00004FD0
	                            
	                    sceIoLseek(fd, 0, SEEK_CUR); //0x00004FE4
	                    unk_$s3 = 1; //0x00004FF4
	                }	                         
	                else {
	                      sceIoLseek(fd, 0, SEEK_CUR); //0x0000507C
	                      unk_212F4++; //0x00005080	                               
	                }
	       }
	       else if (buffer[0] == '#') { //0x00004E40
	                        
	                if (unk_212F4 < 2000) { //0x00005008
	                        
	                    int unk_1 = unk_212F4 * 46; //0x00005010
	                    //unk1 = unk1 + 0x228C8; //0x00005038
	                            
	                    unkStruct_228C8[unk_1].val1 = unk_212F0; //0x0000503C
	                    unkStruct_228C8[unk_1].val4 = 0; //0x00005040
	                    unkStruct_228C8[unk_1].val3 = 0; //0x00005044
	                    unkStruct_228C8[unk_1].val2 = 0; //0x00005048
	                    sceIoLseek(fd, 0, SEEK_CUR); //0x0000504C	                   	    
	                }
	                else {
	                      break; //loc_00004EC0
	                 }
	       }
	       else if (buffer[0] == '!') { //0x00004E48
	                        
	                if (unk_$s1 != 0) { //0x00004F04
	                           
	                    int unk_1 = unk_212F4 * 46; //0x00004F1C
	                           
	                    char unk_2 = unkStruct_228C8[unk_1].val3; //0x00004F24
	                    char unk_3 = unk_2 | 1; //0x00004F30
	                    if (unk_2 & 1) { //0x00004F2C
	                               
	                        unk_3 = unk_2 | 2; //0x00004F34
	                        unk_3 = unk_3 & 0xFE; //0x00004F3C
	                        unkStruct_228C8[unk_1].val3 = unk_3; //0x00004F40
	                               
	                        sceIoLseek(fd, 0, SEEK_CUR); //0x00004F50
	               	    }
	               	    else {
	               		  unkStruct_228C8[unk_1].val3 = unkStruct_228C8[unk_1].val3 | 1; //0x00004F30 && 0x00005184
	               		  sceIoLseek(fd, 0, SEEK_CUR); //0x00005194
	               	    }
	               	}
	       }
	       if (unk_$s3 == 0) { //0x00004E50
	               	
	           if (unk_$s1 == 0) { //0x00004E58
	               	        
	               int retVal = blockAdd(fd, buffer); //0x00004E64
	               if (retVal != 0) { //0x00004E6C
	               	           
	               	   if (unk_212F4 != 0) { //0x00004E74
	               	              
	               	       int unk_1 = (unk_212F4 -1) * 46; //0x00004E7C
	               	       unkStruct_228C8[unk_1].val2 = unkStruct_228C8[unk_1].val2 + 1; //0x00004E9C					      
			   }
			   sceIoLseek(fd, 0, SEEK_CUR); //0x00004EAC  
	               }
	               else {
	                     if (unk_212F4 != 0) { //0x000051B0
	                                 
	                         unk_212F4 = -1; //0x000051C4
	                         break; //0x000051C0
	                     }
	                     else {
	                           sceIoLseek(fd, 0, SEEK_CUR); //0x00004EAC
	                     }	                                    
	               }
	           }
	           else {
	                 if (unk_$s1 < 41) { //0x00004E5C && 0x0000509C
	                             
	                     int unk_1 = unk_212F4 * 46; //0x000050B0
	                     int unk_2 = unk_$s1 + 1; //0x000050AC
                             int unk_3 = unk_1 + unk_$s1; //0x000050D0
	                             
	                     unkStruct_228C8[unk_1].(unk_3)[unk_3 + 4] = buffer[0]; //0x000050DC
	                     unk_2 = unk_2 | 0xFF; //0x000050D4
	                             
	                     unkStruct_228C8[unk_1].(unk_2 & 0xFF)[unk_1 + unk_$s1 + 4] = 0; //0x000050E8	                             
	                 }
	                 sceIoLseek(fd, 0, SEEK_CUR); //0x000050E4
	           }
	       }
	       else {
	             sceIoLseek(fd, 0, SEEK_CUR); //0x000050E4
	       }	                  
	}
	sceIoClose(fd); //0x00004EC0:
	unk_212F8 = unk_212F4; //0x00004EF8 	            
    } 
}


/* Subroutine sub_000045E4 - Address 0x000045E4
 *
 * global variables:
 *        u32 menuKey = 0x00300000; //0x0002075C
 * 	  u32 cheatTriggerKey = 0x00800000; //0x00020760
 *	  u32 screenShotKey 0x00008040; //0x00020764       
 *        
 * @return void
 */   
void setButtonConfig() {

     u8 iCounter = 0; //inner while loop counter
     u8 counter = 0; //while loop counter
     int hexVal;
     int pos;
     int fd;
     char button[8]; //0x00004630
     
     //0x00004624
     fd = sceIoOpen("ms0:/darkfrost/config/buttons.txt", PSP_O_RDONLY, 0777);
     if (fd > 0) { //0x0000429C
     
         while (counter < 3) { //0x00004798
     
                while (iCounter < 100) {
               
                       pos = sceIoLseek(fd, 0, SEEK_CUR); //0x00004668 
                       sceIoRead(fd, button, 1); //0x0000467C
                       counter++;
                       if (button[0] == 'x' || button[0] == 'X') { //0x00004688 && 0x0000464C
                           break;
                       } 
                 }
         
                 sceIoRead(fd, button, sizeof button); //0x00004698       
                  
                 //0x000046A0 
                 switch(counter) {
                      //0x000046BC        
                      case 0:
                          menuKey = char2hex(button, &hexVal);  //0x000046CC
                          break; 
                      //0x000046A4       
                      case 1:                          
                          cheatTriggerKey = char2hex(button, &hexVal); //0x00004804
                          break;
                      //0x000046AC       
                      case 2:
                          screenShotKey = char2hex(button, &hexVal);
                          break;                          
                 }
                 sceIoLseek(fd, pos + hexVal, SEEK_SET); //0x0000478C
         }
         //0x000047AC
         if (screenShotKey != 0x8040) {
             //0x00004730                      
             if (screenShotKey == 0x10000001) { 
                       
                 validScreenShotCombo = 1; //0x000047B8                                      
             }
                  //0x00004738
                 else if (screenShotKey == 0x340) {
                                      
                          validScreenShotCombo = 1; //0x000047B8 
                 }
              //0x0000473C   
             else { 
                   screenShotKey == 0x8040; //0x00004748
                   validScreenShotCombo = 0; //0x0000474C
             }
         }
         //0x000047B4    
         else { 
               validScreenShotCombo = 1; //0x000047B8  
         }                                                  
     }
}


/* Subroutine sub_0000425C - Address 0x0000425C
 *
 * global variables:
 *	  u32 colorPrimary = 0xFFFFFFFF; //0x0000207A4
 *	  u32 colorSecondary = 0xFFF6CF00; //0x0000207A8
 *	  u32 colorTurshiary = 0xFFFF0000; //0x0000207AC
 *	  u32 colorBackground = 0xFF300000; //0x0000207B0
 *	  u32 colorBrowser1 = 0xFFFFFFFF; //0x00002077C
 *	  u32 colorBrowser2 = 0xFFFFFFFF; //0x000020780
 *        
 * @return void
 */   
void setColorConfig() {

     int fd;
     int pos;
     u32 hexVal;
     char color[8]; //0x000042C8
     u8 iCounter = 0; //0x000042E8 -- inner while-loop counter
     u8 counter = 0; //$s4 -- while-loop counter
     
     //0x00004294
     fd = sceIoOpen("ms0:/darkfrost/config/colors.txt", PSP_O_RDONLY, 0777);
     if (fd > 0 ) { //0x0000429C
     
         while (counter < 6) { //0x00004340 && 0x0000445C
     
     	 	while (iCounter < 100) { //0x000042F8
         
               	       pos = sceIoLseek(fd, 0, SEEK_CUR); //0x0000430C
               	       sceIoRead(fd, color, 1); //0x00004320
         
                       if (color[0] == 'x' || color[0] == 'X') { //0x0000432C
                   
                           break; //0x0000432C
                       }
                       iCounter++;
                }       
                //0x00004334
                sceIoRead(fd, color, sizeof color); //0x00004338
               
                switch (counter) {
               
                     case 0:                 	
                          colorPrimary =  char2hex(color, &hexVal); //0x000044C8
                     	  break;
                     	  
                     case 1:
                     	 colorSecondary =  char2hex(color, &hexVal); //0x000044B0
                     	 break;
                     	  
                     case 2:
                     	    
                     	 colorTurshiary =  char2hex(color, &hexVal); //0x00004498
                     	 break; 
                     	 
                     case 3:
                     	    
                     	 colorBackground =  char2hex(color, &hexVal); //0x00004480
                     	 break; 
                     	 
                     case 4:
                     	    
                     	 colorBrowser1 =  char2hex(color, &hexVal); //0x0000444C
                     	 break; 
                     	 
                     case 5:
                     	    
                     	 colorBrowser2 =  char2hex(color, &hexVal); //0x00004394
                     	 break; 
                }                   	      
                sceIoLseek(fd, pos + hexVal, SEEK_SET) //0x0000446C
                counter++;
     	 }
     	 
     	 if (colorPrimary == 0) { //0x000043C4
     	 
     	     colorPrimary = 0xFFFFFFFF; //0x0000451C   	     
         }
         
         if (colorSecondary == 0) { //0x000043D4
         
             colorSecondary = 0xFFF6CF00; //0x0000453C
         }
         
         if (colorTurshiary == 0) { //0x000043E4
         
             colorTurshiary = 0xFFFF0000; //0x00004558           
         }
        
         if (colorBackground == 0) { //0x000043F4
        
             colorBackground = 0xFF300000; //0x00004574
         }
        
         if (colorBrowser1 == 0) { //0x00004404
        
             colorBrowser1 == 0xFFFFFFFF; //0x00004590
         }
        
         if (colorBrowser2 == 0) { //0x00004410
        
             colorBrowser2 == 0xFFFFFFFF; //0x000045D0
         } 
     }
}


/* Subroutine sub_000005C4 - Address 0x000005C4 
 *
 * global variables:
 *        char notePadContent[41] //0x000419B8
 *
 * @return void
 */   
void readNotePad() {
     
     int fd;
     
     //0x000005E0
     fd = sceIoOpen("ms0:/darkfrost/config/notepad.txt", PSP_O_RDONLY, 0777);
     if (fd > 0) { //0x000005E8
     
         sceIoRead(fd, notePadContent, sizeof notePadContent); //0x000005FC
         sceIoClose(fd); //0x00000604
         
         if(notePadContent == NULL) { //0x00000610
            notePadContent = "A"; //0x00000630
         }
     }
     //loc_00000648    
     else { 
          notePadContent = "A"; //0x00000630
    }
}


/* Subroutine sub_0000066C - Address 0x0000066C  
 *
 * global variables:
 *        char firmPart1[4]; //0x00041D28
 *        char firmPart[1]; //0x00041D40
 *
 * @return void
 */         
void psnablerGetFirmVersion() {
     
     int fd;
     
     //0x00000684
     fd = sceIoOpen("ms0:/darkfrost/config/psnabler.txt", PSP_O_RDONLY, 0777);
     if (fd > 0) {
            
            //0x000006C0
            sceIoRead(fd, firmPart1, 4);
            //0x000006E0
            sceIoRead(fd, firmPart2, 1);
            //0x000006F4
            sceIoClose(fd);
     }
     //0x00000694
     else {
          firmPart1 = "06.3";
          firmPart2 = "9";
     }    
 }
 
 /* Subroutine sub_0001BC64 - Address 0x0001BC64
 *
 * global variables:    
 *        
 * @return a (manipulated) character value.
 */   
char encryptByte(int key, char source) {
     
    int res;
    int unk1;
    char unk2;   
    char unk3;   
    u8 counter = 0;

    res = source * 10; //0x0001BC68 	
    unk1 = key ^ source; //0x0001BC70
    
    while (counter < 3) { //0x0001BCBC
    
    	   unk3 = (counter + res) ^ unk3; //0x0001BC84
    	   unk3 = unk3 / unk1; //0x0001BC8C
    	  
    	   unk3 = unk3 ^ unk2; //0x0001BC98   	  
    	   unk3 = unk3 | unk2; //0x0001BC9C
    	  
    	   unk3 = unk3 ^ key; //0x0001BCA8
    	  
    	   if (unk3 < 128)  { //0x0001BCA4
    	      
    	       unk3 = (key + 1) & 0xFF; //0x0001BCC0   	      
    	   }
    	   else {
    	  	 unk3 = unk3 * key; //0x0001BCAC  	  
    	   }
    	     	  
    	   counter = counter++; //0x0001BCB4
    }
       
    char unk4 = unk3; //0x0001BCCC
    while (unk4 > 256) { //0x0001BCC8 && 0x0001BCE4
           
    	   unk4 = unk4 / unk4; //0x0001BCD4
    }
       
    char unk5 = unk4 & 0xFF; //0x0001BCEC -- seb (Sign-Extended Byte)
    if (unk5 == 1) { //0x0001BCF4
       
        unk5 =  (key+1) & 0xFF;  //0x0001BD00   
    }
    
    return unk5; //0x0001BD04   
}

 
 /* Subroutine sub_0001BD0C - Address 0x0001BD0C
 *
 * global variables:     
 *        
 * @return void
 */   
void manipulatePsid(char *psid) {

    u8 i = 0;
    int keyVal = 0x00F583E3;
    
    while (keyVal < 0x00F583F3) { //0x0001BD44
 
    	   psid[i] = encryptByte(keyVal, psid[i]); //0x0001BD38
    	   keyVal++; //0x0001BD3C
    	   i++; //0x0001BD48
    }  
}
 
 
 /* Subroutine sub_0001BD64 - Address 0x0001BD64
 *
 * global variables:
 *        char psid[16] //0x00041D30     
 *        
 * @return void
 */   
void resolvePsid(char *psid) {

    int fd;

    fd = sceIoOpen("ms0:/seplugins/nitePR/nitePRimportant.bin", PSP_O_WRONLY, 0777); //0x0001BD84
    sceIoRead(fd, psid, sizeof psid); //0x0001BD98
    
    manipulatePsid(psid); 
    
    sceIoWrite(fd, psid, sizeof psid); //0x0001BDB0
    sceIoClose(fd); //0x0001BDC8  
}

 
/* Subroutine sub_00019F98 - Address 0x00019F98  
 *
 * global variables:
 *        
 * @param libEnt - Pointer to a ::SceLibraryEntryTable to get its NID table.
 * @param nid - The NID to look for.
 *
 * @return Pointer to NID address if found, otherwise 0 (NULL).
 */     
u32 *findNidAddress(SceLibraryEntryTable *libEnt, u32 nid) {

    
    u8 libVStubCount; //9
    u16 libStubCount; //10
    u32 *nidtable; //12
    
    int i;
    int count;
    
    //0x00019F98
    if (libEnt == NULL) {
        return (u32 *)0; //0x00019FA4
    }
    
    libStubCount = libEnt->stubcount; //0x00019FA8
    nidtable = (u32 *)libEnt->entrytable; //0x00019FB0
    //0x00019FAC
    if (libStubCount == 0) {
        return (u32 *)0;
    }
    //0x00019FCC
    /* It is both i = i + 4 and i = i + 1 but thanks to C pointer arithmetic we can express both values in one variable usng the latter */
    for (i = 0; i < libStubCount; i++) { 
         //0x00019FD8
         if (nidtable[i] == nid) {               
               
             libVStubCount = libEnt->vstubcount; //0x00019FE0
             count = libVStubCount + libStubCount; //0x00019FE4
             count = count + i; //0x00019FE8
        
             return (u32 *)(nidtable + (count << 2)); //0x00019FF0 -- return pointer to i-th element of nidtable with the same NID  
         }            
    }
    return (u32 *)0;
 }
    

/* Subroutine sub_000051C8 - Address 0x000051C8 
 *
 * global variables:
 *        char gameID[10]; //0x00040D28
 *
 * @return void
 */         
void getGameID() {
     
     int fd;
     char tempGameID[10]; //0x00040D28

     fd = sceIoOpen("disc0:/UMD_DATA.BIN", PSP_O_RDONLY, 0777); //0x000051E8
     //0x000051F0
     if (fd > 0) {         
         sceIoRead(fd, tempGameID, sizeof tempGameID); //0x00005204
         sceIoClose(fd);
     }
     //loc_000052AC
     else {
          tempGameID = "XHOMEBREWX";
     }
   
     strcpy(gameID, tempGameID) //0x00005220 
     //0x00005228 - 0x00005290
     memcpy(&gameDirDF[21], gameID, sizeof gameID);
     memcpy(&gameDirNitePR[22], gameID, sizeof gameID);
}
     

/* Subroutine sub_0001A1AC - Address 0x0001A1AC 
 *
 * Additional Information:
 *            The function part 0x0001A268 - 0x0001A340 won't be executed.
 *
 * global variables:
 *        None                 
 *
 * @param uid The UID describing the module to be checked.
 * @param modName Pointer to a module name which will be compared 
 *                with library names of the module being checked.
 *
 * @return Pointer to a ::SceLibraryEntryTable, otherwise NULL.
 */         
SceLibraryEntryTable *findLibEntryTableByModule(SceUID uid, const char *libName) {
    
    int unk_modVal;
    u32 ent_next; //64
    u32 stub_top; //72
    u32 ent_size; //68
    u32 ent_end;
    u32 ent_left;
    
    SceModule *mod = NULL;
    
    mod = sceKernelFindModuleByUID(uid); //LoadCoreForKernel_CCE4A157
    
    //If the module is loaded as kernel module
    unk_modVal = (mod & 0xFF000000); 
    if (unk_modVal == 0x88000000) { //0x0001A1D8
        
        ent_next = mod->ent_top; //0x0001A1DC
        stub_top = mod->stub_top; //0x0001A200
        ent_left = stub_top - ent_next; //0x0001A204
        
        //0x0001A208 - 0x0001A20C
        if (ent_left < 40) {
                     
            return NULL; //0001A1E4
        }        
        //0x0001A214
        ent_size = mod->ent_size;
        ent_end = ent_next + ent_size; //0x0001A218
        //0x0001A21C - 0x0001A220 && 0x0001A258
        while (ent_next < ent_end) {
        
               SceLibraryEntryTable *libEnt = (SceLibraryEntryTable *)ent_next;
               //0x0001A230
               if (libEnt != NULL) {
                   //0x0001A238    
                   if (strcmp(libEnt->libname, libName) == 0) {
                
                       return ent_next; //$s0 is returned holding pointer to SceLibraryEntryTable
                   } 
                   else {
                        //0x0001A250
                        ent_next = ent_next + (libEnt->len << 2); 
                  }
               }
        }
    }
    return NULL; //0x0001A260
}


/* Subroutine sub_00003768 - Address 0x00003768 
 *
 * global variables:
 *        int screenShotNumber = 0; //0x000213e0
 *        char buffer[64]; //0x00041978
 *        char screenShotPathTemplate[43]; //0x000207F0
 *
 * @return void
 */
void countScreenShots() {

    int fd;	
    screenShotNumber = 0; 
	
    //0x000037CC	
    sprintf(buffer, screenShotPathTemplate, screenShotNumber); 
    while (fd = sceIoOpen(buffer ,PSP_O_RDONLY, 0777) >= 0) { //0x000037E4
				      
	   sceIoClose(fd); //0x000037B0
	   screenShotNumber++; //0x000037BC
	   sprintf(buffer, screenShotPathTemplate, screenShotNumber);
    }
}
		

//Subroutine module_start - Address 0x000004AC  
int module_start(SceSize args, void *argp) {

    while (sceKernelFindModuleByName("sceWlan_Driver") == NULL) {  //LoadCoreForKernel_CF8A41B1
	   sceKernelDelayThread(100); //ThreadManForKernel_CEADEB47
    }
	
    //What is that...Mkultra backdoor stuff!
    sceKernelSearchModuleByName("sceWlan_Driver");
	
    sceKernelGetThreadmanIdList(SCE_KERNEL_TMID_Thread, readBuf, sizeof readBuf / sizeof (SceUID), &idCount); 	
    thid = sceKernelCreateThread("DarkFrostThread", mainThread, 0x18, 0, NULL);
    if (thid >= 0) {
	sceKernelStartThread(thid, 0, NULL);
    }
	
    return 0;
}

//Subroutine module_stop - Address 0x00000410 
int module_stop(SceSize args, void *argp) {

    running = 0;
    sceKernelTerminateThread(thid);
	
    return 0;
}