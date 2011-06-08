#include "niteprio.h"

static unsigned int char2hex(unsigned char *a_data, unsigned int *a_type);
static int parse_line(CheatEngine* prEngine, const char* sLine);

static unsigned int char2hex(unsigned char *a_data, unsigned int *a_type) {
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

int niteprio_import(CheatEngine* prEngine, const char* sFile) {
    SceUID fd = -1;
    SceOff fileSz = 0;
    SceOff bytes = 0;
    char sLine[256];
    SceOff llen = 0;
    int r = 0;
    char cIn = 0;
    
    fd = sceIoOpen(sFile, PSP_O_RDONLY, 0777);
    if (fd < 0) {
        return;
    }
    fileSz = sceIoLseek(fd, 0, SEEK_END);
    sceIoLseek(fd, 0, SEEK_SET);
    
    while (bytes < fileSz) {
        r = sceIoRead(fd, &cIn, 1);
        if (r <= 0) {
            break;
        }
        bytes++;
        if (cIn == '\n' || cIn == '\r') {
            sLine[llen] = '\0';
            if (llen > 0) {
                parse_line(prEngine, sLine);
            }
            llen = 0;
        } else {
            sLine[llen] = cIn;
            llen++;
        }
    }
    sceIoCloseAsync(fd);
}

static int parse_line(CheatEngine* prEngine, const char* sLine) {
    const char* sFunc = "parse_line";
    Cheat* prCheat = NULL;
    Block* prBlock = NULL;
    char* sName = NULL;
    char* sHex = NULL;
    int llen = 0;
    int inname = 0;
    unsigned int btype = 0;
    unsigned int value = 0;
    
    if (sLine == NULL) {
        return -1;
    }
    llen = strlen(sLine);
    if (llen <= 0) {
        return 0;
    }
    if (sLine[0] == '#') {
        if (llen <= 1) {
            geelog_flog(LOG_ERROR, sFunc, "Malformed Cheat Line.");
            return -1;
        }
        prCheat = cheatengine_add_cheat(prEngine);
        if (prCheat == NULL) {
            geelog_flog(LOG_ERROR, sFunc, "Failed to add Cheat.");
            return -1;
        }
        inname = 0;
        sName = &sLine[1];
        while (inname == 0) {
            if (*sName == '!') {
                if (cheat_is_selected(prCheat)) {
                    cheat_set_constant(prCheat);
                } else {
                    cheat_set_selected(prCheat);
                }
                sName++;
                continue;
            }
            if (*sName == ' ') {
                sName++;
                continue;
            }
            inname = 1;
        }
        if (sName != NULL) {
            strncpy(prCheat->name, sName, CHEAT_NAME_LEN);
        }
    }
    if (sLine[0] == ';') {
        /* Comment Mode */
    }
    if (sLine[0] == '0') {
        prCheat = cheatengine_get_cheat(prEngine, prEngine->cheat_count - 1);
        if (prCheat == NULL) {
            geelog_flog(LOG_ERROR, sFunc, "Could not get last Cheat.");
            return -1;
        }
        if (llen < 10) {
            geelog_flog(LOG_ERROR, sFunc, "Malformed Block Line.");
        }
        if (sLine[1] == 'x' || sLine[1] == 'X') {
            prBlock = cheatengine_add_block(prEngine);
            if (prBlock == NULL) {
                geelog_flog(LOG_ERROR, sFunc, "Failed to add Block.");
                return -1;
            }
            if (prCheat->len == 0) {
                prCheat->block = prEngine->block_count - 1;
            }
            prCheat->len++;
            sHex = &sLine[2];
            value = char2hex(sHex, &btype);
            if (value == 0xFFFFFFFF) {
                block_set_dma(prBlock);
                prBlock->stdVal = 0xFFFFFFFF;
            } else {
                if (value < 0x08800000) {
                    prBlock->address = value + 0x08800000;
                } else {
                    prBlock->address = value;
                }
            }
            sHex += 10;
            if (*sHex != 'x' && *sHex != 'X') {
                geelog_flog(LOG_ERROR, sFunc, "Malformed Block Value.");
                return -1;
            }
            sHex++;
            value = char2hex(sHex, &btype);
            prBlock->hakVal = value;
            switch (btype) {
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
            
        }
    }
    return 0;
}
