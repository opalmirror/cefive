#include "niteprio.h"

static int parse_line(CheatEngine* prEngine, const char* sLine);

int niteprio_import(CheatEngine* prEngine, const char* sFile) {
    const char* sFunc = "niteprio_import";
    SceUID fd = -1;
    char sLine[NITEPRIO_LINELEN + 1];
    int eof = 0;
    int br = 0;
    char sMsg[GEELOG_LINELEN + 1];
    SceOff fileSz = 0;
    SceOff readSz = 0;
    int linelen = 0;
    SceUInt64 res = 0;
    
    if (prEngine == NULL) {
        geelog_flog(LOG_ERROR, sFunc, "Invalid CheatEngine pointer.");
        return NITEPRIO_NULLPTR;
    }
    if (sFile == NULL) {
        geelog_flog(LOG_ERROR, sFunc, "Invalid File Name.");
        return NITEPRIO_INVFILE;
    }
    
    fd = sceIoOpenAsync(sFile, PSP_O_RDONLY, 0777);
    if (sceIoWaitAsync(fd, &res) < 0) {
        geelog_flog(LOG_ERROR, sFunc, "Error waiting for Open.");
        return NITEPRIO_IOERROR;
    }
    if (fd < 0) {
        sprintf(sMsg, "Failed to open file '%s'.", sFile);
        geelog_flog(LOG_ERROR, sFunc, sMsg);
        return NITEPRIO_IOERROR;
    }
    fileSz = sceIoLseek(fd, 0, SEEK_END);
    sprintf(sMsg, "File is %ld bytes.", fileSz);
    geelog_flog(LOG_DEBUG, sFunc, sMsg);
    sceIoLseek(fd, 0, SEEK_SET);
    
    while (eof == 0) {
        br = sceIoRead(fd, sLine[linelen], 1);
        if (br != 1) {
            eof = 1;
            continue;
        }
        readSz += br;
        if (readSz > fileSz) {
            eof = 1;
            continue;
        }
        if (sLine[linelen] == '\r' || sLine[linelen] == '\n') {
            sLine[linelen] = (char)0;
            if (parse_line(prEngine, sLine) != NITEPRIO_SUCCESS) {
                eof = 1;
                continue;
            }
            linelen = 0;
            continue;
        }
        linelen++;
    }
    
    if (sceIoClose(fd) < 0) {
        geelog_flog(LOG_ERROR, sFunc, "Failed to close file.");
        return NITEPRIO_IOERROR;
    }
    geelog_flog(LOG_INFO, sFunc, "Finished importing file.");
    return NITEPRIO_SUCCESS;
}

static int parse_line(CheatEngine* prEngine, const char* sLine) {
    const char* sFunc = "parse_line";
    Cheat* prCheat = NULL;
    char* sName = NULL;
    char sMsg[GEELOG_LINELEN + 1];
    
    if ((prEngine == NULL) || (sLine == NULL)) {
        geelog_flog(LOG_ERROR, sFunc, "Invalid Parameter pointer.");
        return NITEPRIO_NULLPTR;
    }
    if ((sLine[0] == '\r') || (sLine[0] == '\n')) {
        return NITEPRIO_SUCCESS;
    }
    sprintf(sMsg, "Parsing Line '%s'.", sLine);
    geelog_flog(LOG_DEBUG, sFunc, sMsg);
    if (sLine[0] == '#') {
        /* Begin a new Cheat. */
        geelog_flog(LOG_DEBUG, sFunc, "Adding new Cheat.");
        prCheat = cheatengine_add_cheat(prEngine);
        if (prCheat == NULL) {
            geelog_flog(LOG_ERROR, sFunc, "Failed to add Cheat.");
            return NITEPRIO_FAILURE;
        }
        sName = sLine + 1;
        while ((*sName == ' ') || (*sName == '\t')) {
            sName++;
        }
        
        sprintf(sMsg, "Assigning Cheat Name '%s'.", sName);
        geelog_flog(LOG_DEBUG, sFunc, sMsg);
        strcpy(prCheat->name, sName);
    }
    return NITEPRIO_SUCCESS;
}
