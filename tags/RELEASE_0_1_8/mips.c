/* mips.c
 *  MIPSr4 Instruction Decoding
 * Author:
 *  Sir Gee of Five
 */

#include "mips.h"
#include <stdio.h>
#include <pspkerneltypes.h>

unsigned char mipsNum[16];
static const char *mipsRegisterArray[]={
    "zero", "at", "v0", "v1", 
    "a0", "a1", "a2", "a3", 
    "t0", "t1", "t2", "t3", 
    "t4", "t5", "t6", "t7", 
    "s0", "s1", "s2", "s3", 
    "s4", "s5", "s6", "s7", 
    "t8", "t9", "k0", "k1", 
    "gp", "sp", "fp", "ra"};

static const char *mipsFpRegNames[] = {
    "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7",
    "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15",
    "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23",
    "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
};

static const char *mipsControlRegister[]={
    "Index", "EntryLo0", "Context", "Wired", 
    "BadVAddr", "EntryHi", "Status", "EPC", 
    "Config", "WatchLO", "XContext", "$22", 
    "Debug", "ECC", "TagLo", "ErrorEPC"};

static void decodeVCOP(char* buffer, unsigned int i_inst, unsigned int i_addr);
static char* mipsGetFormat(int fmt);
static short mipsGetImmediate(unsigned int i_inst);
static int mipsGetInstrIndex(unsigned int i_inst);
static unsigned char mipsGetOperand0(unsigned int i_inst);
static unsigned char mipsGetOperand1(unsigned int i_inst);
static unsigned char mipsGetOperand2(unsigned int i_inst);
static unsigned char mipsGetOperand3(unsigned int i_inst);
static void mipsType0(char *, const char *);
static void mipsType1(char *, const char *, int, int, short);
static void mipsType1c(char *, const char *, int, int, short);
static void mipsType1f(char *, const char *, int, int, short);
static void mipsType2(char *, const char *, int, int);
static void mipsType2f(char *, const char *, int, int);
static void mipsType3(char *, const char *, int, int, short);
static void mipsType4(char *, const char *, int, int, int);
static void mipsType5(char *, const char *, int, short);
static void mipsType6(char *, const char *, short, int);
static void mipsType7(char *, const char *, int, int, int);
static void mipsType8(char *, const char *, int);
static void mipsType9(char *, const char *, int, int, int);
static void mipsType10(char *, const char *, unsigned int);
static void mipsType11(char *, const char *, int, int, unsigned int);
static void mipsType12(char *, const char *, int, unsigned int);
static void mipsType13(char *, const char *, int, int);
static void mipsTypeCcDest(char *, const char *, int, unsigned int);
static void mipsTypeCcFsFt(char *, const char *, int, int, int);
static void mipsTypeFdFrFsFt(char *buffer, const char *mnem, int iFd, int iFr, int iFs, int iFt);
static void mipsTypeFdFs(char *, const char *, int, int);
static void mipsTypeFdFsCc(char *, const char *, int, int, int);
static void mipsTypeFdFsFt(char *, const char *, int, int, int);
static void mipsTypeFdFsFtRs(char *, const char *, int, int, int, int);
static void mipsTypeFdFsRt(char *, const char *, int, int, int);
static void mipsTypeFsIndexBase(char *buffer, const char *mnem, int iFs, int iIndex, int iBase);
static void mipsTypeRtOffsetBase(char *, const char *, int, short, int);
static void mipsTypeRtRdSel(char* buffer, const char* mnem, 
        int i_rt, int i_rd, int i_sel);
static void mipsTypeRtRsPosSize(char *, const char *, int, int, int, int);
static void mipsTypeVrd(char* buffer, const char* mnem, int iRd);
static void mipsTypeVrdA(char* buffer, const char* mnem, int iRd, int iA);
static void mipsTypeVrdVrs(char* buffer, const char* mnem, int iRd, int iRs);
static void mipsTypeVrdVrsScale(char* buffer, const char* mnem,
        int iRd, int iRs, int scale);
static void mipsTypeVrdVrsVrt(char* buffer, const char* mnem,
        int iRd, int iRs, int iRt);

static void mipsAddOperand(MipsInstruction *prIns, EOperandType otype, char *name, int value) {
    if (prIns == NULL) {
        return;
    }
    MipsOperand *opnd = &(prIns->operands[prIns->operand_count]);
    opnd->operandtype = otype;
    opnd->name = name;
    opnd->value = value;
    prIns->operand_count++;
}

/* mips_decode_type_i
 *  Decode a MIPS Type "I" (Immediate) Instruction.
 *  [---- --][-- ---][- ----][---- ---- ---- ----]
 *  [opcode ][rs    ][rt    ][immediate          ]
 */
void mips_decode_type_i(MipsInstruction* prIns) {
    if (prIns == NULL) {
        return;
    }
    prIns->opcode = (int)mipsGetOpCode(prIns->value);
    int rs = (int)mipsGetOperand0(prIns->value);
    int rt = (int)mipsGetOperand1(prIns->value);
    int imm = (int)mipsGetImmediate(prIns->value);
    mipsAddOperand(prIns, MOT_GPREG, "rs", rs);
    mipsAddOperand(prIns, MOT_GPREG, "rt", rt);
    mipsAddOperand(prIns, MOT_IMMEDIATE, "immediate", imm);
}

/* mips_decode_type_j
 *  Decode a MIPS Type "J" (Jump) Instruction.
 *  [---- --][-- ---- ---- ---- ---- ---- ----]
 *  [opcode ][instr_index                     ]
 */
void mips_decode_type_j(MipsInstruction* prIns) {
    if (prIns == NULL) {
        return;
    }
    prIns->opcode = (int)mipsGetOpCode(prIns->value);
    int instrindex = mipsGetInstrIndex(prIns->value);
    mipsAddOperand(prIns, MOT_INSTRINDEX, "instr_index", instrindex);
}

/* mips_decode_type_r
 *  Decode a MIPS Type "R" (Register) Instruction.
 *  [---- --][-- ---][- ----][---- -][--- --][-- ----]
 *  [opcode ][rs    ][rt    ][rd    ][sa    ][function]
 */
void mips_decode_type_r(MipsInstruction* prIns) {
    if (prIns == NULL) {
        return;
    }
    prIns->opcode = (int)mipsGetOpCode(prIns->value);
    int rs = (int)mipsGetOperand0(prIns->value);
    int rt = (int)mipsGetOperand1(prIns->value);
    int rd = (int)mipsGetOperand2(prIns->value);
    int sa = (int)mipsGetOperand3(prIns->value);
    int func = (int)mipsGetFunction(prIns->value);
    mipsAddOperand(prIns, MOT_GPREG, "rs", rs);
    mipsAddOperand(prIns, MOT_GPREG, "rt", rt);
    mipsAddOperand(prIns, MOT_GPREG, "rd", rd);
    mipsAddOperand(prIns, MOT_SA, "sa", sa);
}

SceUInt32 mipsGetBranchDestination(unsigned int i_inst, unsigned int i_addr) {
    SceUInt32 dest = 0;
    int offset = (int)mipsGetImmediate(i_inst);
    dest = i_addr + 4 + (offset << 2);
    return dest;
}

static char* mipsGetFormat(int fmt) {
    if (fmt == 0x10) {
        return "s";
    }
    if (fmt == 0x11) {
        return "d";
    }
    if (fmt == 0x14) {
        return "w";
    }
    if (fmt == 0x15) {
        return "l";
    }
    if (fmt == 0x16) {
        return "ps";
    }
    return "?";
}

unsigned char mipsGetFunction(unsigned int i_inst) {
    unsigned char uc_func;
    uc_func = (unsigned char)(i_inst & 0x3F);
    return uc_func;
}

static short mipsGetImmediate(unsigned int i_inst) {
    short un_imm;
    un_imm = (short)(i_inst & 0xFFFF);
    return un_imm;
}

static int mipsGetInstrIndex(unsigned int i_inst) {
    unsigned int index = 0;
    index = i_inst & 0x03FFFFFF;
    return index;
}

SceUInt32 mipsGetJumpDestination(unsigned int i_inst, unsigned int i_addr) {
    SceUInt32 dest = 0;
    unsigned int pref = (i_addr + 4) & 0x0C000000;
    dest = i_inst & 0x03FFFFFF;
    dest <<= 2;
    dest |= pref;
    return dest;
}

unsigned char mipsGetOpCode(unsigned int i_inst) {
    unsigned char uc_opcode;
    uc_opcode = (unsigned char)(((i_inst & 0xFC000000) >> 26) & 0x3F);
    return uc_opcode;
}

static unsigned char mipsGetOperand0(unsigned int i_inst) {
    unsigned char uc_oper;
    uc_oper = (unsigned char)(((i_inst & 0x03E00000) >> 21) & 0x1F);
    return uc_oper;
}

static unsigned char mipsGetOperand1(unsigned int i_inst) {
    unsigned char uc_oper;
    uc_oper = (unsigned char)(((i_inst & 0x001F0000) >> 16) & 0x1F);
    return uc_oper;
}

static unsigned char mipsGetOperand2(unsigned int i_inst) {
    unsigned char uc_oper;
    uc_oper = (unsigned char)(((i_inst & 0x0000F800) >> 11) & 0x1F);
    return uc_oper;
}

static unsigned char mipsGetOperand3(unsigned int i_inst) {
    unsigned char uc_oper;
    uc_oper = (unsigned char)(((i_inst & 0x000007C0) >> 6) & 0x1F);
    return uc_oper;
}

static unsigned short mipsGetUnsignedImmediate(unsigned int i_inst) {
    unsigned short un_val;
    un_val = (unsigned short)(i_inst & 0x0000FFFF);
    return un_val;
}

static char* mipsGetVformat(int f, int F) {
    if (f == 0 && F == 0) {
        return "s";
    }
    if (f == 0 && F == 1) {
        return "p";
    }
    if (f == 1 && F == 0) {
        return "t";
    }
    if (f == 1 && F == 1) {
        return "q";
    }
    return "?";
}

static int mipsGetVoperand0(unsigned int i_inst) {
    int val = (i_inst & 0x03800000) >> 23;
    return val;
}

static int mipsGetVoperand1(unsigned int i_inst) {
    int val = (i_inst & 0x007F0000) >> 16;
    return val;
}

static int mipsGetVoperand2(unsigned int i_inst) {
    int val = (i_inst & 0x00008000) >> 15;
    return val;
}

static int mipsGetVoperand3(unsigned int i_inst) {
    int val = (i_inst & 0x00007F00) >> 8;
    return val;
}

static int mipsGetVoperand4(unsigned int i_inst) {
    int val = (i_inst & 0x00000080) >> 7;
    return val;
}

static int mipsGetVoperand5(unsigned int i_inst) {
    int val = i_inst & 0x0000007F;
    return val;
}

/* mipsInABSfmt
 *  Floating Point Absolute Value
 *  ABS.S fd, fs
 *  ABS.D fd, fs
 *  [010001][-----][00000][-----][-----][000101]
 *  [COP1  ][fmt  ][0    ][fs   ][fd   ][ABS   ]
 */
static void mipsInABSfmt(char *buffer, unsigned int i_inst) {
    int fmt = (int)mipsGetOperand0(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    char mnem[9];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "abs.%s", sfmt);
    mipsTypeFdFs(buffer, mnem, fd, fs);
}

/* decodeABSfmt
 *  Floating Point Absolute Value
 *  ABS.S fd, fs
 *  ABS.D fd, fs
 *  [010001][-----][00000][-----][-----][000101]
 *  [COP1  ][fmt  ][0    ][fs   ][fd   ][ABS   ]
 */
static void decodeABSfmt(MipsInstruction *prIns) {
    if (prIns == NULL) {
        return;
    }
    int fmt = (int)mipsGetOperand0(prIns->value);
    int fs = (int)mipsGetOperand2(prIns->value);
    int fd = (int)mipsGetOperand3(prIns->value);
    char mnem[9];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "abs.%s", sfmt);
    prIns->mnemonic = mnem;
    prIns->description = "Floating Point Absolute Value";
    mipsAddOperand(prIns, MOT_FPREG, "fd", fd);
    mipsAddOperand(prIns, MOT_FPREG, "fs", fs);
}

/* mipsInADD
 *  Add Word
 *  ADD rd, rs, rt
 *  [000000][-----][-----][-----][00000][100000]
 * [SPECIAL][rs   ][rt   ][rd   ][0    ][ADD   ]
 */
static void mipsInADD(char *buffer, unsigned int i_inst) {
    const char *mnem = "add";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType4(buffer, mnem, rd, rs, rt);
}

/* decodeADD
 *  Add Word
 *  ADD rd, rs, rt
 *  [000000][-----][-----][-----][00000][100000]
 * [SPECIAL][rs   ][rt   ][rd   ][0    ][ADD   ]
 */
static void decodeADD(MipsInstruction *prIns) {
    if (prIns == NULL) {
        return;
    }
    int rs = (int)mipsGetOperand0(prIns->value);
    int rt = (int)mipsGetOperand1(prIns->value);
    int rd = (int)mipsGetOperand2(prIns->value);
    prIns->mnemonic = "add";
    prIns->description = "Add Word";
    mipsAddOperand(prIns, MOT_GPREG, "rd", rd);
    mipsAddOperand(prIns, MOT_GPREG, "rs", rs);
    mipsAddOperand(prIns, MOT_GPREG, "rt", rt);
}

/* mipsInADDfmt
 *  Floating Point Add
 *  ADD.S fd, fs, ft
 *  ADD.D fd, fs, ft
 *  ADD.PS fd, fs, ft
 *  [010001][-----][-----][-----][-----][000000]
 *  [COP1  ][fmt  ][ft   ][fs   ][fd   ][AND   ]
 */
static void mipsInADDfmt(char *buffer, unsigned int i_inst) {
    int fmt = (int)mipsGetOperand0(i_inst);
    int ft = (int)mipsGetOperand1(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    char mnem[9];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "add.%s", sfmt);
    mipsTypeFdFsFt(buffer, mnem, fd, fs, ft);
}

/* decodeADDfmt
 *  Floating Point Add
 *  ADD.S fd, fs, ft
 *  ADD.D fd, fs, ft
 *  ADD.PS fd, fs, ft
 *  [010001][-----][-----][-----][-----][000000]
 *  [COP1  ][fmt  ][ft   ][fs   ][fd   ][AND   ]
 */
static void decodeADDfmt(MipsInstruction *prIns) {
    if (prIns == NULL) {
        return;
    }
    int fmt = (int)mipsGetOperand0(prIns->value);
    int ft = (int)mipsGetOperand1(prIns->value);
    int fs = (int)mipsGetOperand2(prIns->value);
    int fd = (int)mipsGetOperand3(prIns->value);
    char mnem[9];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "add.%s", sfmt);
    prIns->mnemonic = mnem;
    prIns->description = "Floating Point Add";
    mipsAddOperand(prIns, MOT_FPREG, "fd", fd);
    mipsAddOperand(prIns, MOT_FPREG, "fs", fs);
    mipsAddOperand(prIns, MOT_FPREG, "ft", ft);
}

/* mipsInADDI
 *  Add Immediate Word
 *  ADDI rt, rs, immediate
 *  [001000][-----][-----][----------------]
 *  [ADDI  ][rs   ][rt   ][immediate       ]
 */
static void mipsInADDI(char *buffer, unsigned int i_inst) {
    const char *mnem = "addi";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    unsigned short imm = mipsGetUnsignedImmediate(i_inst);
    mipsType3(buffer, mnem, rs, rt, imm);
}

/* decodeADDI
 *  Add Immediate Word
 *  ADDI rt, rs, immediate
 *  [001000][-----][-----][----------------]
 *  [ADDI  ][rs   ][rt   ][immediate       ]
 */
static void decodeADDI(MipsInstruction* prIns) {
    if (prIns == NULL) {
        return;
    }
    int rs = (int)mipsGetOperand0(prIns->value);
    int rt = (int)mipsGetOperand1(prIns->value);
    short imm = mipsGetImmediate(prIns->value);
    prIns->mnemonic = "addi";
    prIns->description = "Add Immediate Word";
    mipsAddOperand(prIns, MOT_GPREG, "rt", rt);
    mipsAddOperand(prIns, MOT_GPREG, "rs", rs);
    mipsAddOperand(prIns, MOT_IMMEDIATE, "immediate", imm);
}

/* mipsInADDIU
 *  Add Immediate Unsigned Word
 *  ADDIU rt, rs, immediate
 *  [001001][-----][-----][----------------]
 *  [ADDIU ][rs   ][rt   ][immediate       ]
 */
static void mipsInADDIU(char *buffer, unsigned int i_inst) {
    const char *mnem = "addiu";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    unsigned short imm = mipsGetUnsignedImmediate(i_inst);
    mipsType3(buffer, mnem, rs, rt, imm);
}

/* decodeADDIU
 *  Add Immediate Unsigned Word
 *  ADDIU rt, rs, immediate
 *  [001001][-----][-----][----------------]
 *  [ADDIU ][rs   ][rt   ][immediate       ]
 */
static void decodeADDIU(MipsInstruction* prIns) {
    if (prIns == NULL) {
        return;
    }
    int rs = (int)mipsGetOperand0(prIns->value);
    int rt = (int)mipsGetOperand1(prIns->value);
    short imm = mipsGetImmediate(prIns->value);
    prIns->mnemonic = "addiu";
    prIns->description = "Add Immediate Unsigned Word";
    mipsAddOperand(prIns, MOT_GPREG, "rt", rt);
    mipsAddOperand(prIns, MOT_GPREG, "rs", rs);
    mipsAddOperand(prIns, MOT_IMMEDIATE, "immediate", imm);
}

/* mipsInADDU
 *  Add Unsigned Word
 *  ADDU rd, rs, rt
 *  [000000][-----][-----][-----][00000][100001]
 * [SPECIAL][rs   ][rt   ][rd   ][0    ][ADDU  ]
 */
static void mipsInADDU(char *buffer, unsigned int i_inst) {
    const char *mnem = "addu";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType4(buffer, mnem, rd, rs, rt);
}

/* decodeADDU
 *  Add Unsigned Word
 *  ADDU rd, rs, rt
 *  [000000][-----][-----][-----][00000][100001]
 * [SPECIAL][rs   ][rt   ][rd   ][0    ][ADDU  ]
 */
static void decodeADDU(MipsInstruction* prIns) {
    if (prIns == NULL) {
        return;
    }
    int rs = (int)mipsGetOperand0(prIns->value);
    int rt = (int)mipsGetOperand1(prIns->value);
    int rd = (int)mipsGetOperand2(prIns->value);
    prIns->mnemonic = "addu";
    prIns->description = "Add Unsigned Word";
    mipsAddOperand(prIns, MOT_GPREG, "rd", rd);
    mipsAddOperand(prIns, MOT_GPREG, "rt", rt);
    mipsAddOperand(prIns, MOT_GPREG, "rs", rs);
}

/* mipsInALNVPS
 *  Floating Point Align Variable
 *  ALNV.PS fd, fs, ft, rs
 *  [010011][-----][-----][-----][-----][011110]
 *  [COP1X ][rs   ][ft   ][fs   ][fd   ][ALNV.PS]
 */
static void mipsInALNVPS(char *buffer, unsigned int i_inst) {
    const char *mnem = "alnv.ps";
    unsigned char rs = mipsGetOperand0(i_inst);
    unsigned char ft = mipsGetOperand1(i_inst);
    unsigned char fs = mipsGetOperand2(i_inst);
    unsigned char fd = mipsGetOperand3(i_inst);
    mipsTypeFdFsFtRs(buffer, mnem, fd, fs, ft, rs);
}

/* decodeALNVPS
 *  Floating Point Align Variable
 *  ALNV.PS fd, fs, ft, rs
 *  [010011][-----][-----][-----][-----][011110]
 *  [COP1X ][rs   ][ft   ][fs   ][fd   ][ALNV.PS]
 */
static void decodeALNVPS(MipsInstruction* prIns) {
    if (prIns == NULL) {
        return;
    }
    int rs = (int)mipsGetOperand0(prIns->value);
    int ft = (int)mipsGetOperand1(prIns->value);
    int fs = (int)mipsGetOperand2(prIns->value);
    int fd = (int)mipsGetOperand3(prIns->value);
    prIns->mnemonic = "alnv.ps";
    prIns->description = "Floating Point Align Variable";
    mipsAddOperand(prIns, MOT_FPREG, "fd", fd);
    mipsAddOperand(prIns, MOT_FPREG, "fs", fs);
    mipsAddOperand(prIns, MOT_FPREG, "ft", ft);
    mipsAddOperand(prIns, MOT_GPREG, "rs", rs);
}

/* mipsInAND
 *  And
 *  AND rd, rs, rt
 *  [000000][-----][-----][-----][00000][100100]
 * [SPECIAL][rs   ][rt   ][rd   ][0    ][AND   ]
 */
static void mipsInAND(char *buffer, unsigned int i_inst) {
    const char *mnem = "and";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType4(buffer, mnem, rd, rs, rt);
}

/* decodeAND
 *  And
 *  AND rd, rs, rt
 *  [000000][-----][-----][-----][00000][100100]
 * [SPECIAL][rs   ][rt   ][rd   ][0    ][AND   ]
 */
static void decodeAND(MipsInstruction* prIns) {
    if (prIns == NULL) {
        return;
    }
    int rs = (int)mipsGetOperand0(prIns->value);
    int rt = (int)mipsGetOperand1(prIns->value);
    int rd = (int)mipsGetOperand2(prIns->value);
    prIns->mnemonic = "and";
    prIns->description = "And";
    mipsAddOperand(prIns, MOT_GPREG, "rd", rd);
    mipsAddOperand(prIns, MOT_GPREG, "rt", rt);
    mipsAddOperand(prIns, MOT_GPREG, "rs", rs);
}

/* mipsInANDI
 *  And Immediate
 *  ANDI rt, rs, immediate
 *  [001100][-----][-----][----------------]
 *  [ANDI  ][rs   ][rt   ][immediate       ]
 */
static void mipsInANDI(char *buffer, unsigned int i_inst) {
    const char *mnem = "andi";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    short imm = mipsGetImmediate(i_inst);
    mipsType3(buffer, mnem, rs, rt, imm);
}

/* decodeANDI
 *  And Immediate
 *  ANDI rt, rs, immediate
 *  [001100][-----][-----][----------------]
 *  [ANDI  ][rs   ][rt   ][immediate       ]
 */
static void decodeANDI(MipsInstruction* prIns) {
    if (prIns == NULL) {
        return;
    }
    int rs = (int)mipsGetOperand0(prIns->value);
    int rt = (int)mipsGetOperand1(prIns->value);
    short imm = mipsGetImmediate(prIns->value);
    prIns->mnemonic = "andi";
    prIns->description = "And Immediate";
    mipsAddOperand(prIns, MOT_GPREG, "rt", rt);
    mipsAddOperand(prIns, MOT_GPREG, "rs", rs);
    mipsAddOperand(prIns, MOT_IMMEDIATE, "immediate", imm);
}

/* mipsInBC1
 *  Branch of FP Conditional
 *  BC1F offset (cc = 0 implied)
 *  BC1F cc, offset
 *  BC1FL offset (cc = 0 implied)
 *  BC1FL cc, offset
 *  BC1T offset (cc = 0 implied)
 *  BC1T cc, offset
 *  BC1TL offset (cc = 0 implied)
 *  BC1TL cc, offset
 *  [010001][01000][---][-] [-] [----------------]
 *  [COP1  ][BC   ][cc ][nd][tf][offset          ]
 */
static void mipsInBC1(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    char *mnem;
    int op1 = (int)mipsGetOperand1(i_inst);
    int offset = (int)mipsGetImmediate(i_inst);
    int cc = (op1 & 0x1C) >> 2;
    int nd = (op1 & 2) >> 1;
    int tf = op1 & 1;
    unsigned int dest = (i_addr & 0x0FFFFFFF) + 4 + (offset << 2);
    if (tf == 0) {
        if (nd == 0) {
            mnem = "bc1f";
        } else {
            mnem = "bc1fl";
        }
    } else {
        if (nd == 0) {
            mnem = "bc1t";
        } else {
            mnem = "bc1tl";
        }
    }
    if (cc == 0) {
        mipsType10(buffer, mnem, dest);
    } else {
        mipsTypeCcDest(buffer, mnem, cc, dest);
    }
}
 
/* mipsInBC2
 *  Branch of COP2 Conditional
 *  BC2F offset (cc = 0 implied)
 *  BC2F cc, offset
 *  BC2FL offset (cc = 0 implied)
 *  BC2FL cc, offset
 *  BC2T offset (cc = 0 implied)
 *  BC2T cc, offset
 *  BC2TL offset (cc = 0 implied)
 *  BC2TL cc, offset
 *  [010010][01000][---][-] [-] [----------------]
 *  [COP2  ][BC   ][cc ][nd][tf][offset          ]
 */
static void mipsInBC2(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    char *mnem;
    int op1 = (int)mipsGetOperand1(i_inst);
    int offset = (int)mipsGetImmediate(i_inst);
    int cc = (op1 & 0x1C) >> 2;
    int nd = (op1 & 2) >> 1;
    int tf = op1 & 1;
    unsigned int dest = (i_addr & 0x0FFFFFFF) + 4 + (offset << 2);
    if (tf == 0) {
        if (nd == 0) {
            mnem = "bc2f";
        } else {
            mnem = "bc2fl";
        }
    } else {
        if (nd == 0) {
            mnem = "bc2t";
        } else {
            mnem = "bc2tl";
        }
    }
    if (cc == 0) {
        mipsType10(buffer, mnem, dest);
    } else {
        mipsTypeCcDest(buffer, mnem, cc, dest);
    }
}

/* mipsInBEQ
 *  Branch on Equal
 *  BEQ rs, rt, offset
 *  [000100][-----][-----][----------------]
 *  [BEQ   ][rs   ][rt   ][offset          ]
 */
static void mipsInBEQ(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    const char *mnem = "beq";
    int i_rs = (int)mipsGetOperand0(i_inst);
    int i_rt = (int)mipsGetOperand1(i_inst);
    short offset = mipsGetImmediate(i_inst);
    unsigned int dest = (i_addr & 0x0FFFFFFF) + 4 + (offset << 2);
    mipsType11(buffer, mnem, i_rs, i_rt, dest);
}

/* decodeBEQ
 *  Branch on Equal
 *  BEQ rs, rt, offset
 *  [000100][-----][-----][----------------]
 *  [BEQ   ][rs   ][rt   ][offset          ]
 */
static void decodeBEQ(MipsInstruction* prIns) {
    if (prIns == NULL) {
        return;
    }
    mips_decode_type_i(prIns);
    prIns->operands[2].operandtype = MOT_OFFSET;
    prIns->mnemonic = "beq";
    prIns->description = "Branch on Equal";
}

/* mipsInBEQL
 *  Branch on Equal Likely
 *  BEQL rs, rt, offset
 *  [010100][-----][-----][----------------]
 *  [BEQ   ][rs   ][rt   ][offset          ]
 */
static void mipsInBEQL(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    const char *mnem = "beql";
    int i_rs = (int)mipsGetOperand0(i_inst);
    int i_rt = (int)mipsGetOperand1(i_inst);
    short offset = mipsGetImmediate(i_inst);
    unsigned int dest = (i_addr & 0x0FFFFFFF) + 4 + (offset << 2);
    mipsType11(buffer, mnem, i_rs, i_rt, dest);
}

/* decodeBEQL
 *  Branch on Equal Likely
 *  BEQL rs, rt, offset
 *  [010100][-----][-----][----------------]
 *  [BEQ   ][rs   ][rt   ][offset          ]
 */
static void decodeBEQL(MipsInstruction* prIns) {
    if (prIns == NULL) {
        return;
    }
    mips_decode_type_i(prIns);
    prIns->operands[2].operandtype = MOT_OFFSET;
    prIns->mnemonic = "beql";
    prIns->description = "Branch on Equal Likely";
}

/* mipsInBGEZ
 *  Branch on Greater Than or Equal to Zero
 *  BGEZ rs, offset
 *  [000001][-----][00001][----------------]
 *  [REGIMM][rs   ][BGEZ ][offset          ]
 */
static void mipsInBGEZ(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    const char *mnem = "bgez";
    int rs = (int)mipsGetOperand0(i_inst);
    short offset = mipsGetImmediate(i_inst);
    unsigned int dest = (i_addr + 4 + (offset << 2)) & 0x0FFFFFFF;
    mipsType12(buffer, mnem, rs, dest);
}

/* mipsInBGEZAL
 *  Branch on Greater Than or Equal to Zero and Link
 *  BGEZAL rs, offset
 *  [000001][-----][10001] [----------------]
 *  [REGIMM][rs   ][BGEZAL][offset          ]
 */
static void mipsInBGEZAL(char *buffer, unsigned int i_inst, 
        unsigned int i_addr) {
    const char *mnem = "bgezal";
    int rs = (int)mipsGetOperand0(i_inst);
    short offset = mipsGetImmediate(i_inst);
    unsigned int dest = (i_addr + 4 + (offset << 2)) & 0x0FFFFFFF;
    mipsType12(buffer, mnem, rs, dest);
}

/* mipsInBGEZALL
 *  Branch on Greater Than or Equal to Zero and Link Likely
 *  BGEZALL rs, offset
 *  [000001][-----][10011]  [----------------]
 *  [REGIMM][rs   ][BGEZALL][offset          ]
 */
static void mipsInBGEZALL(char *buffer, unsigned int i_inst, 
        unsigned int i_addr) {
    const char *mnem = "bgezall";
    int rs = (int)mipsGetOperand0(i_inst);
    short offset = mipsGetImmediate(i_inst);
    unsigned int dest = (i_addr + 4 + (offset << 2)) & 0x0FFFFFFF;
    mipsType12(buffer, mnem, rs, dest);
}

/* mipsInBGEZL
 *  Branch on Less Than Zero Likely
 *  BGEZL rs, offset
 *  [000001][-----][00011][----------------]
 *  [REGIMM][rs   ][BGEZL][offset          ]
 */
static void mipsInBGEZL(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    const char *mnem = "bgezl";
    int rs = (int)mipsGetOperand0(i_inst);
    short offset = mipsGetImmediate(i_inst);
    unsigned int dest = (i_addr + 4 + (offset << 2)) & 0x0FFFFFFF;
    mipsType12(buffer, mnem, rs, dest);
}

/* mipsInBGTZ
 *  Branch on Greater Than Zero
 *  BGTZ rs, offset
 *  [000111][-----][00000][----------------]
 *  [BGTZ  ][rs   ][0    ][offset          ]
 */
static void mipsInBGTZ(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    const char *mnem = "bgtz";
    int rs = (int)mipsGetOperand0(i_inst);
    short offset = mipsGetImmediate(i_inst);
    unsigned int dest = (i_addr + 4 + (offset << 2)) & 0x0FFFFFFF;
    mipsType12(buffer, mnem, rs, dest);
}

/* mipsInBGTZL
 *  Branch on Greater Than Zero Likely
 *  BGTZL rs, offset
 *  [010111][-----][00000][----------------]
 *  [BGTZL ][rs   ][0    ][offset          ]
 */
static void mipsInBGTZL(char *buffer, unsigned int i_inst, 
        unsigned int i_addr) {
    const char *mnem = "bgtzl";
    int rs = (int)mipsGetOperand0(i_inst);
    short offset = mipsGetImmediate(i_inst);
    unsigned int dest = (i_addr + 4 + (offset << 2)) & 0x0FFFFFFF;
    mipsType12(buffer, mnem, rs, dest);
}

/* mipsInBLEZ
 *  Branch on Less Than or Equal to Zero
 *  BLEZ rs, offset
 *  [000110][-----][00000][----------------]
 *  [BLEZ  ][rs   ][0    ][offset          ]
 */
static void mipsInBLEZ(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    const char *mnem = "blez";
    int rs = (int)mipsGetOperand0(i_inst);
    short offset = mipsGetImmediate(i_inst);
    unsigned int dest = (i_addr + 4 + (offset << 2)) & 0x0FFFFFFF;
    mipsType12(buffer, mnem, rs, dest);
}

/* mipsInBLEZL
 *  Branch on Less Than or Equal to Zero Likely
 *  BLEZL rs, offset
 *  [010110][-----][00000][----------------]
 *  [BLEZL ][rs   ][0    ][offset          ]
 */
static void mipsInBLEZL(char *buffer, unsigned int i_inst, 
        unsigned int i_addr) {
    const char *mnem = "blezl";
    int rs = (int)mipsGetOperand0(i_inst);
    short offset = mipsGetImmediate(i_inst);
    unsigned int dest = (i_addr + 4 + (offset << 2)) & 0x0FFFFFFF;
    mipsType12(buffer, mnem, rs, dest);
}

/* mipsInBLTZ
 *  Branch on Less Than Zero
 *  BLTZ rs, offset
 *  [000001][-----][00000][----------------]
 *  [REGIMM][rs   ][BLTZ ][offset          ]
 */
static void mipsInBLTZ(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    const char *mnem = "bltz";
    int rs = (int)mipsGetOperand0(i_inst);
    short offset = mipsGetImmediate(i_inst);
    unsigned int dest = (i_addr + 4 + (offset << 2)) & 0x0FFFFFFF;
    mipsType12(buffer, mnem, rs, dest);
}

/* mipsInBLTZAL
 *  Branch on Less Than Zero and Link
 *  BLTZAL rs, offset
 *  [000001][-----][10000] [----------------]
 *  [REGIMM][rs   ][BLTZAL][offset          ]
 */
static void mipsInBLTZAL(char *buffer, unsigned int i_inst, 
        unsigned int i_addr) {
    const char *mnem = "bltzal";
    int rs = (int)mipsGetOperand0(i_inst);
    short offset = mipsGetImmediate(i_inst);
    unsigned int dest = (i_addr + 4 + (offset << 2)) & 0x0FFFFFFF;
    mipsType12(buffer, mnem, rs, dest);
}
    
/* mipsInBLTZALL
 *  Branch on Less Than Zero and Link Likely
 *  BLTZALL rs, offset
 *  [000001][-----][10010]  [----------------]
 *  [REGIMM][rs   ][BLTZALL][offset          ]
 */
static void mipsInBLTZALL(char *buffer, unsigned int i_inst, 
        unsigned int i_addr) {
    const char *mnem = "bltzall";
    int rs = (int)mipsGetOperand0(i_inst);
    short offset = mipsGetImmediate(i_inst);
    unsigned int dest = (i_addr + 4 + (offset << 2)) & 0x0FFFFFFF;
    mipsType12(buffer, mnem, rs, dest);
}

 /* mipsInBLTZL
 *  Branch on Less Than Zero Likely
 *  BLTZL rs, offset
 *  [000001][-----][00010][----------------]
 *  [REGIMM][rs   ][BLTZL][offset          ]
 */
static void mipsInBLTZL(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    const char *mnem = "bltzl";
    int rs = (int)mipsGetOperand0(i_inst);
    short offset = mipsGetImmediate(i_inst);
    unsigned int dest = (i_addr + 4 + (offset << 2)) & 0x0FFFFFFF;
    mipsType12(buffer, mnem, rs, dest);
}

/* mipsInBNE
 *  Branch on Not Equal
 *  BNE rs, rt, offset
 *  [000101][-----][-----][----------------]
 *  [BNE   ][rs   ][rt   ][offset          ]
 */
static void mipsInBNE(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    const char *mnem = "bne";
    int i_rs = (int)mipsGetOperand0(i_inst);
    int i_rt = (int)mipsGetOperand1(i_inst);
    short offset = mipsGetImmediate(i_inst);
    unsigned int dest = (i_addr & 0x0FFFFFFF) + 4 + (offset << 2);
    mipsType11(buffer, mnem, i_rs, i_rt, dest);
}

/* mipsInBNEL
 *  Branch on Not Equal Likely
 *  BNEL rs, rt, offset
 *  [010101][-----][-----][----------------]
 *  [BNEL  ][rs   ][rt   ][offset          ]
 */
static void mipsInBNEL(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    const char *mnem = "bnel";
    int i_rs = (int)mipsGetOperand0(i_inst);
    int i_rt = (int)mipsGetOperand1(i_inst);
    short offset = mipsGetImmediate(i_inst);
    unsigned int dest = (i_addr & 0x0FFFFFFF) + 4 + (offset << 2);
    mipsType11(buffer, mnem, i_rs, i_rt, dest);
}

/* mipsInBREAK
 *  Breakpoint
 *  BREAK
 *  [000000][--------------------][001101]
 * [SPECIAL][code                ][BREAK ]
 */
static void mipsInBREAK(char *buffer, unsigned int i_inst) {
    mipsType0(buffer, "break");
}

/* mipsInCACHE
 *  Perform Cache Operation
 *  CACHE op, offset(base)
 *  [101111][-----][-----][----------------]
 *  [CACHE ][base ][op   ][offset          ]
 */
static void mipsInCACHE(char *buffer, unsigned int i_inst) {
    const char *mnem = "cache";
    int i_base = (int)mipsGetOperand0(i_inst);
    int i_op = (int)mipsGetOperand1(i_inst);
    short offset = mipsGetImmediate(i_inst);
    mipsType1c(buffer, mnem, i_base, i_op, offset);
}

/* mipsInCcondfmt
 *  Floating Point Compare
 *  C.cond.S fs, ft (cc = 0 implied)
 *  C.cond.D fs, ft (cc = 0 implied)
 *  C.cond.PS fs, ft (cc = 0 implied)
 *  C.cond.S cc, fs, ft
 *  C.cond.D cc, fs, ft
 *  C.cond.PS cc, fs, ft
 *  [010001][-----][-----][-----][---][0][-][11][----]
 *  [COP1  ][fmt  ][ft   ][fs   ][cc ][0][A][FC][cond]
 */
static void mipsInCcondfmt(char *buffer, unsigned int i_inst) {
    int fmt = (int)mipsGetOperand0(i_inst);
    int ft = (int)mipsGetOperand1(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int op3 = (int)mipsGetOperand3(i_inst);
    int op4 = (int)mipsGetFunction(i_inst);
    int cc = (op3 & 0x1C) >> 2;
    int a = (op3 & 1);
    int cond = op4 & 0xF;
    char *sfmt;
    char *scond;
    char mnem[9];
    
    switch (cond) {
        case 0:
            scond = "f";
            break;
        case 1:
            scond = "un";
            break;
        case 2:
            scond = "eq";
            break;
        case 3:
            scond = "ueq";
            break;
        case 4:
            scond = "olt";
            break;
        case 5:
            scond = "ult";
            break;
        case 6:
            scond = "ole";
            break;
        case 7:
            scond = "ule";
            break;
        case 8:
            scond = "sf";
            break;
        case 9:
            scond = "ngle";
            break;
        case 0xA:
            scond = "seq";
            break;
        case 0xB:
            scond = "ngl";
            break;
        case 0xC:
            scond = "lt";
            break;
        case 0xD:
            scond = "nge";
            break;
        case 0xE:
            scond = "le";
            break;
        case 0xF:
            scond = "ngt";
            break;
    }
    sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "c.%s.%s", scond, sfmt);
    if (cc == 0) {
        mipsTypeFdFs(buffer, mnem, fs, ft);
    } else {
        mipsTypeCcFsFt(buffer, mnem, cc, fs, ft);
    }
}

/* mipsInCEILLfmt
 *  Floating Point Ceiling Convert to Long Fixed Point
 *  CEIL.L.S fd, fs
 *  CEIL.L.D fd, fs
 *  [010001][-----][00000][-----][-----][001010]
 *  [COP1  ][fmt  ][0    ][fs   ][fd   ][CEIL.L]
 */
static void mipsInCEILLfmt(char *buffer, unsigned int i_inst) {
    int fmt = (int)mipsGetOperand0(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    char mnem[10];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "ceil.l.%s", sfmt);
    mipsTypeFdFs(buffer, mnem, fd, fs);
}

/* mipsInCEILWfmt
 *  Floating Point Ceiling Convert to Word Fixed Point
 *  CEIL.W.S fd, fs
 *  CEIL.W.D fd, fs
 *  [010001][-----][00000][-----][-----][001110]
 *  [COP1  ][fmt  ][0    ][fs   ][fd   ][CEIL.W]
 */
static void mipsInCEILWfmt(char *buffer, unsigned int i_inst) {
    int fmt = (int)mipsGetOperand0(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    char mnem[10];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "ceil.w.%s", sfmt);
    mipsTypeFdFs(buffer, mnem, fd, fs);
}

/* mipsInCFC1
 *  Move Control Word From Floating Point
 *  CFC1 rt, fs
 *  [010001][00010][-----][-----][00000000000]
 *  [COP1  ][CF   ][rt   ][fs   ][0          ]
 */
static void mipsInCFC1(char *buffer, unsigned int i_inst) {
    const char *mnem = "cfc1";
    int rt = (int)mipsGetOperand1(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    mipsType2f(buffer, mnem, rt, fs);
}

/* mipsInCFC2
 *  Move Control Word From Floating Point
 *  CFC1 rt, fs
 *  [010010][00010][-----][-----][00000000000]
 *  [COP2  ][CF   ][rt   ][fs   ][0          ]
 */
static void mipsInCFC2(char *buffer, unsigned int i_inst) {
    const char *mnem = "cfc2";
    int rt = (int)mipsGetOperand1(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    mipsType2f(buffer, mnem, rt, fs);
}

/* mipsInCLO
 *  Count Leading Ones in Word
 *  CLO rd, rs
 *  [011100]  [-----][-----][-----][00000][100001]
 *  [SPECIAL2][rs   ][rt   ][rd   ][0    ][CLO   ]
 */
static void mipsInCLO(char *buffer, unsigned int i_inst) {
    const char *mnem = "clo";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType2(buffer, mnem, rd, rs);
}

/* mipsInCLZ
 *  Count Leading Zeros in Word
 *  CLZ rd, rs
 *  [011100]  [-----][-----][-----][00000][100000]
 *  [SPECIAL2][rs   ][rt   ][rd   ][0    ][CLZ   ]
 */
static void mipsInCLZ(char *buffer, unsigned int i_inst) {
    const char *mnem = "clz";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType2(buffer, mnem, rd, rs);
}

/* mipsInCTC1
 *  Move Control Word to Floating Point
 *  CTC1 rt, fs
 *  [010001][00110][-----][-----][00000000000]
 *  [COP1  ][CT   ][rt   ][fs   ][0          ]
 */
static void mipsInCTC1(char *buffer, unsigned int i_inst) {
    const char *mnem = "ctc1";
    int rt = (int)mipsGetOperand1(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    mipsType2f(buffer, mnem, rt, fs);
}

/* mipsInCTC2
 *  Move Control Word to Coprocessor 2
 *  CTC2 rt, rd
 *  [010010][00110][-----][-----][00000000000]
 *  [COP2  ][CT   ][rt   ][rd   ][0          ]
 */
static void mipsInCTC2(char *buffer, unsigned int i_inst) {
    const char *mnem = "ctc2";
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType2(buffer, mnem, rt, rd);
}

/* mipsInCVTDfmt
 *  Floating Point Convert to Double Floating Point
 *  CVT.D.S fd, fs
 *  CVT.D.W fd, fs
 *  CVT.D.L fd, fs
 *  [010001][-----][00000][-----][-----][100001]
 *  [COP1  ][fmt  ][0    ][fs   ][fd   ][CVT.D ]
 */
static void mipsInCVTDfmt(char *buffer, unsigned int i_inst) {
    int fmt = (int)mipsGetOperand0(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    char mnem[10];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "cvt.d.%s", sfmt);
    mipsTypeFdFs(buffer, mnem, fd, fs);
}

/* mipsInCVTLfmt
 *  Floating Point Convert to Long Fixed Point
 *  CVT.L.S fd, fs
 *  CVT.L.D fd, fs
 *  [010001][-----][00000][-----][-----][100001]
 *  [COP1  ][fmt  ][0    ][fs   ][fd   ][CVT.D ]
 */
static void mipsInCVTLfmt(char *buffer, unsigned int i_inst) {
    int fmt = (int)mipsGetOperand0(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    char mnem[10];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "cvt.l.%s", sfmt);
    mipsTypeFdFs(buffer, mnem, fd, fs);
}

/* mipsInCVTSfmt
 *  Floating Point Convert to Single Floating Point
 *  CVT.S.D fd, fs
 *  CVT.S.W fd, fs
 *  CVT.S.L fd, fs
 *  [010001][-----][00000][-----][-----][100000]
 *  [COP1  ][fmt  ][0    ][fs   ][fd   ][CVT.S ]
 */
static void mipsInCVTSfmt(char *buffer, unsigned int i_inst) {
    int fmt = (int)mipsGetOperand0(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    char mnem[10];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "cvt.s.%s", sfmt);
    mipsTypeFdFs(buffer, mnem, fd, fs);
}

/* mipsInCVTSPL
 *  Floating Point Convert Pair Lower to Single Floating Point
 *  CVT.S.PL fd, fs
 *  [010001][10110][00000][-----][-----][101000]
 *  [COP1  ][fmt  ][0    ][fs   ][fd   ][CVT.S.PL]
 */
static void mipsInCVTSPL(char *buffer, unsigned int i_inst) {
    const char *mnem = "cvt.s.pl";
    int fmt = (int)mipsGetOperand0(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    mipsTypeFdFs(buffer, mnem, fd, fs);
}

/* mipsInCVTPSS
 *  Floating Point Convert Pair to Paired Single
 *  CVT.PS.S fd, fs, ft
 *  [010001][10000][-----][-----][-----][100110]
 *  [COP1  ][fmt  ][ft   ][fs   ][fd   ][CVT.PS]
 */
static void mipsInCVTPSS(char *buffer, unsigned int i_inst) {
    const char *mnem = "cvt.ps.s";
    int ft = (int)mipsGetOperand1(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    mipsTypeFdFsFt(buffer, mnem, fd, fs, ft);
}

/* mipsInCVTSPU
 *  Floating Point Convert Pair Upper to Single Floating Point
 *  CVT.S.PU fd, fs
 *  [010001][10110][00000][-----][-----][100000]
 *  [COP1  ][fmt  ][0    ][fs   ][fd   ][CVT.S.PU]
 */
static void mipsInCVTSPU(char *buffer, unsigned int i_inst) {
    const char *mnem = "cvt.s.pu";
    int fmt = (int)mipsGetOperand0(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    mipsTypeFdFs(buffer, mnem, fd, fs);
}

/* mipsInCVTWfmt
 *  Floating Point Convert to Word Fixed Point Point
 *  CVT.W.S fd, fs
 *  CVT.W.D fd, fs
 *  [010001][-----][00000][-----][-----][100100]
 *  [COP1  ][fmt  ][0    ][fs   ][fd   ][CVT.W ]
 */
static void mipsInCVTWfmt(char *buffer, unsigned int i_inst) {
    int fmt = (int)mipsGetOperand0(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    char mnem[10];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "cvt.w.%s", sfmt);
    mipsTypeFdFs(buffer, mnem, fd, fs);
}

/* mipsInDERET
 *  Debug Exception Return
 *  DERET
 *  [010000][1] [0000000000000000000][011111]
 *  [COP0  ][CO][0                  ][DERET ]
 */
static void mipsInDERET(char *buffer, unsigned int i_inst) {
    mipsType0(buffer, "deret");
}

/* mipsInDIV
 *  Divide Word
 *  DIV rs, rt
 *  [000000][-----][-----][0000000000][011010]
 * [SPECIAL][rs   ][rt   ][0         ][DIV   ]
 */
static void mipsInDIV(char *buffer, unsigned int i_inst) {
    const char *mnem = "div";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    mipsType2(buffer, mnem, rs, rt);
}

/* mipsInDIVfmt
 *  Floating Point Divide
 *  SUB.S fd, fs, ft
 *  SUB.D fd, fs, ft
 *  [010001][-----][-----][-----][-----][000011]
 *  [COP1  ][fmt  ][ft   ][fs   ][fd   ][DIV   ]
 */
static void mipsInDIVfmt(char *buffer, unsigned int i_inst) {
    int fmt = (int)mipsGetOperand0(i_inst);
    int ft = (int)mipsGetOperand1(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    char mnem[10];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "div.%s", sfmt);
    mipsTypeFdFsFt(buffer, mnem, fd, fs, ft);
}

/* mipsInDIVU
 *  Divide Unsigned Word
 *  DIVU rs, rt
 *  [000000][-----][-----][0000000000][011011]
 * [SPECIAL][rs   ][rt   ][0         ][DIVU  ]
 */
static void mipsInDIVU(char *buffer, unsigned int i_inst) {
    const char *mnem = "divu";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    mipsType2(buffer, mnem, rs, rt);
}

/* mipsInERET
 *  Exception Return
 *  ERET
 *  [010000][1] [0000000000000000000][011000]
 *  [COP0  ][CO][0                  ][ERET  ]
 */
static void mipsInERET(char *buffer, unsigned int i_inst) {
    mipsType0(buffer, "eret");
}

/* mipsInEXT
 *  Extract Bit Field
 *  EXT rt, rs, pos, size
 *  [011111]  [-----][-----][-----][-----][000000]
 *  [SPECIAL3][rs   ][rt   ][msbd ][lsb  ][EXT   ]
 */
static void mipsInEXT(char *buffer, unsigned int i_inst) {
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    int size = (int)mipsGetOperand2(i_inst) + 1;
    int pos = (int)mipsGetOperand3(i_inst);
    mipsTypeRtRsPosSize(buffer, "ext", rt, rs, pos, size);
}

/* mipsInFLOORLfmt
 *  Floating Point Floor Convert to Long Fixed Point
 *  FLOOR.L.S fd, fs
 *  FLOOR.L.D fd, fs
 *  [010001][-----][00000][-----][-----][001011]
 *  [COP1  ][fmt  ][0    ][fs   ][fd   ][FLOOR.L]
 */
static void mipsInFLOORLfmt(char *buffer, unsigned int i_inst) {
    int fmt = (int)mipsGetOperand0(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    char mnem[10];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "floor.l.%s", sfmt);
    mipsTypeFdFs(buffer, mnem, fd, fs);
}

/* mipsInFLOORWfmt
 *  Floating Point Floor Convert to Word Fixed Point
 *  FLOOR.W.S fd, fs
 *  FLOOR.W.D fd, fs
 *  [010001][-----][00000][-----][-----][001111]
 *  [COP1  ][fmt  ][0    ][fs   ][fd   ][FLOOR.W]
 */
static void mipsInFLOORWfmt(char *buffer, unsigned int i_inst) {
    int fmt = (int)mipsGetOperand0(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    char mnem[10];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "floor.w.%s", sfmt);
    mipsTypeFdFs(buffer, mnem, fd, fs);
}

/* mipsInINS
 *  Insert Bit Field
 *  INS rt, rs, pos, size
 *  [011111]  [-----][-----][-----][-----][000100]
 *  [SPECIAL3][rs   ][rt   ][msbd ][lsb  ][INS   ]
 */
static void mipsInINS(char *buffer, unsigned int i_inst) {
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    int size = (int)mipsGetOperand2(i_inst) + 1;
    int pos = (int)mipsGetOperand3(i_inst);
    mipsTypeRtRsPosSize(buffer, "ins", rt, rs, pos, size);
}

/* mipsInJ
 *  Jump
 *  J target
 *  [000010][--------------------------]
 *  [J     ][instr_index               ]
 */
static void mipsInJ(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    const char *mnem = "j";
    unsigned int instr_index = ((i_inst & 0x03FFFFFF) << 2);
    unsigned int dest = ((i_addr + 4) & 0x0C000000) | instr_index;
    mipsType10(buffer, mnem, dest);
}

/* mipsInJAL
 *  Jump and Link
 *  JAL target
 *  [000011][--------------------------]
 *  [JAL   ][instr_index               ]
 */
static void mipsInJAL(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    const char *mnem = "jal";
    unsigned int instr_index = ((i_inst & 0x03FFFFFF) << 2);
    unsigned int dest = ((i_addr + 4) & 0x0C000000) | instr_index;
    mipsType10(buffer, mnem, dest);
}

/* mipsInJALR
 *  Jump and Link Register
 *  JALR rs (rd = 31 implied)
 *  JALR rd, rs
 *  [000000][-----][00000][-----][-----][001001]
 * [SPECIAL][rs   ][0    ][rd   ][hint ][JALR  ]
 */
static void mipsInJALR(char *buffer, unsigned int i_inst) {
    const char *mnem = "jalr";
    int rs = (int)mipsGetOperand0(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    if (rd == 31) {
        mipsType8(buffer, mnem, rs);
    } else {
        mipsType2(buffer, mnem, rd, rs);
    }
}

/* mipsInJR
 *  Jump Register
 *  JR rs
 *  [000000][-----][0000000000][-----][001000]
 * [SPECIAL][rs   ][0         ][hint ][JR    ]
 */
static void mipsInJR(char *buffer, unsigned int i_inst) {
    const char *mnem = "jr";
    int rs = (int)mipsGetOperand0(i_inst);
    mipsType8(buffer, mnem, rs);
}

/* mipsInLB
 *  Load Byte
 *  LB rt, offset(base)
 *  [100000][-----][-----][----------------]
 *  [LBU   ][base ][rt   ][offset          ]
 */
static void mipsInLB(char *buffer, unsigned int i_inst) {
    const char *mnem = "lb";
    int i_base = (int)mipsGetOperand0(i_inst);
    int i_rt = (int)mipsGetOperand1(i_inst);
    short offset = mipsGetImmediate(i_inst);
    mipsType1(buffer, mnem, i_base, i_rt, offset);
}

/* mipsInLBU
 *  Load Byte Unsigned
 *  LBU rt, offset(base)
 *  [100100][-----][-----][----------------]
 *  [LBU   ][base ][rt   ][offset          ]
 */
static void mipsInLBU(char *buffer, unsigned int i_inst) {
    const char *mnem = "lbu";
    int i_base = (int)mipsGetOperand0(i_inst);
    int i_rt = (int)mipsGetOperand1(i_inst);
    short offset = mipsGetImmediate(i_inst);
    mipsType1(buffer, mnem, i_base, i_rt, offset);
}

/* mipsInLDC1
 *  Load Doubleword to Floating Point
 *  LWC1 ft, offset(base)
 *  [110101][-----][-----][----------------]
 *  [LDC1  ][base ][ft   ][offset          ]
 */
static void mipsInLDC1(char *buffer, unsigned int i_inst) {
    const char *mnem = "ldc1";
    int base = (int)mipsGetOperand0(i_inst);
    int ft = (int)mipsGetOperand1(i_inst);
    short offset = mipsGetImmediate(i_inst);
    mipsType1f(buffer, mnem, base, ft, offset);
}

/* mipsInLDC2
 *  LoadVector.Quadword Relative to Address
 *  lv.q rt, offset(base)
 *  [110110][-----][-----][----------------]
 *  [lv.q  ][base ][rt   ][offset          ]
 */
static void mipsInLDC2(char *buffer, unsigned int i_inst) {
    const char *mnem = "lv.q";
    int base = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    short offset = mipsGetImmediate(i_inst);
    mipsType1(buffer, mnem, base, rt, offset);
}

/* mipsInLDXC1
 *  Load Doubleword Indexed to Floating Point
 *  LDXC1 fd, index(base)
 *  [010011][-----][-----][00000][-----][000001]
 *  [COP1X ][base ][index][0    ][fd   ][LDXC1 ]
 */
static void mipsInLDXC1(char *buffer, unsigned int i_inst) {
    const char *mnem = "ldxc1";
    int base = (int)mipsGetOperand0(i_inst);
    int index = (int)mipsGetOperand1(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    mipsTypeFsIndexBase(buffer, mnem, fd, index, base);
}

/* mipsInLH
 *  Load Halfword
 *  LH rt, offset(base)
 *  [100001][-----][-----][----------------]
 *  [LH    ][base ][rt   ][offset          ]
 */
static void mipsInLH(char *buffer, unsigned int i_inst) {
    const char *mnem = "lh";
    int i_base = (int)mipsGetOperand0(i_inst);
    int i_rt = (int)mipsGetOperand1(i_inst);
    short offset = mipsGetImmediate(i_inst);
    mipsType1(buffer, mnem, i_base, i_rt, offset);
}

/* mipsInLHU
 *  Load Halfword Unsigned
 *  LHU rt, offset(base)
 *  [100101][-----][-----][----------------]
 *  [LHU   ][base ][rt   ][offset          ]
 */
static void mipsInLHU(char *buffer, unsigned int i_inst) {
    const char *mnem = "lhu";
    int i_base = (int)mipsGetOperand0(i_inst);
    int i_rt = (int)mipsGetOperand1(i_inst);
    short offset = mipsGetImmediate(i_inst);
    mipsType1(buffer, mnem, i_base, i_rt, offset);
}

/* mipsInLL
 *  Load Linked Word
 *  LL rt, offset(base)
 *  [110000][-----][-----][----------------]
 *  [LL    ][base ][rt   ][offset          ]
 */
static void mipsInLL(char *buffer, unsigned int i_inst) {
    const char *mnem = "ll";
    int base = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    short offset = mipsGetImmediate(i_inst);
    mipsType1(buffer, mnem, base, rt, offset);
}

/* mipsInLUI
 *  Load Upper Immediate
 *  LUI rt, immediate
 *  [001111][00000][-----][----------------]
 *  [LUI   ][0    ][rt   ][immediate       ]
 */
static void mipsInLUI(char *buffer, unsigned int i_inst) {
    const char *mnem = "lui";
    int rt = (int)mipsGetOperand1(i_inst);
    short imm = mipsGetImmediate(i_inst);
    mipsType5(buffer, mnem, rt, imm);
}

/* mipsInLUXC1
 *  Load Doubleword Indexed Unaligned to Floating Point
 *  LUXC1 fd, index(base)
 *  [010011][-----][-----][00000][-----][000000]
 *  [COP1X ][base ][index][0    ][fd   ][LUXC1 ]
 */
static void mipsInLUXC1(char *buffer, unsigned int i_inst) {
    const char *mnem = "luxc1";
    int base = (int)mipsGetOperand0(i_inst);
    int index = (int)mipsGetOperand1(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    mipsTypeFsIndexBase(buffer, mnem, fd, index, base);
}

/* mipsInLW
 *  Load Word
 *  LW rt, offset(base)
 *  [100011][-----][-----][----------------]
 *  [LW    ][base ][rt   ][offset          ]
 */
static void mipsInLW(char *buffer, unsigned int i_inst) {
    const char *mnem = "lw";
    int i_base = (int)mipsGetOperand0(i_inst);
    int i_rt = (int)mipsGetOperand1(i_inst);
    short offset = mipsGetImmediate(i_inst);
    mipsType1(buffer, mnem, i_base, i_rt, offset);
}

/* mipsInLWC1
 *  Load Word to Floating Point
 *  LWC1 ft, offset(base)
 *  [110001][-----][-----][----------------]
 *  [LWC1  ][base ][ft   ][offset          ]
 */
static void mipsInLWC1(char *buffer, unsigned int i_inst) {
    const char *mnem = "lwc1";
    int base = (int)mipsGetOperand0(i_inst);
    int ft = (int)mipsGetOperand1(i_inst);
    short offset = mipsGetImmediate(i_inst);
    mipsType1f(buffer, mnem, base, ft, offset);
}

/* mipsInLWC2
 *  Load Word to Coprocessor 2
 *  LWC2 rt, offset(base)
 *  [110001][-----][-----][----------------]
 *  [LWC2  ][base ][rt   ][offset          ]
 */
static void mipsInLWC2(char *buffer, unsigned int i_inst) {
    const char *mnem = "lwc2";
    int base = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    short offset = mipsGetImmediate(i_inst);
    mipsType1(buffer, mnem, base, rt, offset);
}

/* mipsInLWL
 *  Load Word Left
 *  LWL rt, offset(base)
 *  [100010][-----][-----][----------------]
 *  [LWL   ][base ][rt   ][offset          ]
 */
static void mipsInLWL(char *buffer, unsigned int i_inst) {
    const char *mnem = "lwl";
    int i_base = (int)mipsGetOperand0(i_inst);
    int i_rt = (int)mipsGetOperand1(i_inst);
    short offset = mipsGetImmediate(i_inst);
    mipsType1(buffer, mnem, i_base, i_rt, offset);
}

/* mipsInLWR
 *  Load Word Right
 *  LWR rt, offset(base)
 *  [100110][-----][-----][----------------]
 *  [LWR   ][base ][rt   ][offset          ]
 */
static void mipsInLWR(char *buffer, unsigned int i_inst) {
    const char *mnem = "lwr";
    int i_base = (int)mipsGetOperand0(i_inst);
    int i_rt = (int)mipsGetOperand1(i_inst);
    short offset = mipsGetImmediate(i_inst);
    mipsType1(buffer, mnem, i_base, i_rt, offset);
}

/* mipsInLWXC1
 *  Load Word Indexed to Floating Point
 *  LWXC1 fd, index(base)
 *  [010011][-----][-----][00000][-----][000000]
 *  [COP1X ][base ][index][0    ][fd   ][LWXC1 ]
 */
static void mipsInLWXC1(char *buffer, unsigned int i_inst) {
    const char *mnem = "lwxc1";
    int base = (int)mipsGetOperand0(i_inst);
    int index = (int)mipsGetOperand1(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    mipsTypeFsIndexBase(buffer, mnem, fd, index, base);
}

/* mipsInMADD
 *  Multiply and Add Word to Hi, Lo
 *  MADD rs, rt
 *  [011100]  [-----][-----][00000][00000][000000]
 *  [SPECIAL2][rs   ][rt   ][0    ][0    ][MADD  ]
 */
static void mipsInMADD(char *buffer, unsigned int i_inst) {
    const char *mnem = "madd";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    mipsType2(buffer, mnem, rs, rt);
}

/* mipsInMADDfmt
 *  Floating Point Multiply Add
 *  MADD.S fd, fr, fs, ft
 *  MADD.D fd, fr, fs, ft
 *  MADD.PS fd, fr, fs, ft
 *  [010011][-----][-----][-----][-----][100] [---]
 *  [COP1X ][fr   ][ft   ][fs   ][fd   ][MADD][fmt]
 */
static void mipsInMADDfmt(char *buffer, unsigned int i_inst) {
    int fd = (int)mipsGetOperand0(i_inst);
    int fr = (int)mipsGetOperand0(i_inst);
    int fs = (int)mipsGetOperand0(i_inst);
    int ft = (int)mipsGetOperand0(i_inst);
    int fmt = (i_inst & 0x7);
    char mnem[10];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "madd.%s", sfmt);
    mipsTypeFdFrFsFt(buffer, mnem, fd, fr, fs, ft);
}

/* mipsInMADDU
 *  Multiply and Add Unsigned Word to Hi, Lo
 *  MADDU rs, rt
 *  [011100]  [-----][-----][00000][00000][000001]
 *  [SPECIAL2][rs   ][rt   ][0    ][0    ][MADDU ]
 */
static void mipsInMADDU(char *buffer, unsigned int i_inst) {
    const char *mnem = "maddu";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    mipsType2(buffer, mnem, rs, rt);
}

/* mipsInMFC0
 *  Move from Coprocessor 0
 *  MFC0 rt, rd
 *  MFC0 rt, rd, sel
 *  [010000][00000][-----][-----][00000000][---]
 *  [COP0  ][MF   ][rt   ][rd   ][0       ][sel]
 */
static void mipsInMFC0(char *buffer, unsigned int i_inst) {
    const char *mnem = "mfc0";
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    int sel = i_inst & 0x00000007;
    if (sel == 0) {
        mipsType2(buffer, mnem, rt, rd);
    } else {
        //mipsType7(buffer, mnem, rt, rd, sel);
        mipsTypeRtRdSel(buffer, mnem, rt, rd, sel);
    }
}

/* mipsInMFC1
 *  Move Word from Floating Point
 *  MFC1 rt, fs
 *  [010001][00000][-----][-----][00000000000]
 *  [COP1  ][MF   ][rt   ][fs   ][0          ]
 */
static void mipsInMFC1(char *buffer, unsigned int i_inst) {
    const char *mnem = "mfc1";
    int rt = (int)mipsGetOperand1(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    mipsType2f(buffer, mnem, rt, fs);
}

/* mipsInMFC2
 *  Move from Coprocessor 2
 *  MFC2 rt, rd
 *  MFC2 rt, rd, sel
 *  [010010][00000][-----][-----][-----------]
 *  [COP2  ][MF   ][rt   ][rd   ][Impl       ]
 */
static void mipsInMFC2(char *buffer, unsigned int i_inst) {
    const char *mnem = "mfc2";
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    int sel = i_inst & 0x0000007F;
    if (sel == 0) {
        mipsType2(buffer, mnem, rt, rd);
    } else {
        mipsType7(buffer, mnem, rt, rd, sel);
    }
}

/* mipsInMFHC1
 *  Move Word From High Half of Floating Point Register
 *  MFHC1 rt, fs
 *  [010001][00011][-----][-----][00000000000]
 *  [COP1  ][MFH  ][rt   ][fs   ][0          ]
 */
static void mipsInMFHC1(char *buffer, unsigned int i_inst) {
    const char *mnem = "mfhc1";
    int rt = (int)mipsGetOperand1(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    mipsType2f(buffer, mnem, rt, fs);
}

/* mipsInMFHC2
 *  Move Word From High Half of Coprocessor 2 Register
 *  MFHC2 rt, rd
 *  [010010][00011][-----][-----][00000000000]
 *  [COP2  ][MFH  ][rt   ][rd   ][0          ]
 */
static void mipsInMFHC2(char *buffer, unsigned int i_inst) {
    const char *mnem = "mfhc2";
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType2(buffer, mnem, rt, rd);
}

/* mipsInMFHI
 *  Move From HI Register
 *  MFHI rd
 *  [000000][0000000000][-----][00000][010000]
 * [SPECIAL][0         ][rd   ][0    ][MFHI  ]
 */
static void mipsInMFHI(char *buffer, unsigned int i_inst) {
    const char *mnem = "mfhi";
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType8(buffer, mnem, rd);
}

/* mipsInMFLO
 *  Move From LO Register
 *  MFLO rd
 *  [000000][0000000000][-----][00000][010000]
 * [SPECIAL][0         ][rd   ][0    ][MFLO  ]
 */
static void mipsInMFLO(char *buffer, unsigned int i_inst) {
    const char *mnem = "mflo";
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType8(buffer, mnem, rd);
}

/* mipsInMFIC
 *  Allegrex MFIC
 *  MFIC rt, rd
 *  [011100]  [00000][00000][00000][00000][100100]
 *  [SPECIAL2][0    ][rt   ][rd   ][     ][MFIC  ]
 */
static void mipsInMFIC(char *buffer, unsigned int i_inst) {
    const char *mnem = "mfic";
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType2(buffer, mnem, rt, rd);
}

/* mipsInMFMC0
 *  Disable Interrupts / Enable Interrupts
 *  DI
 *  DI rt
 *  [010000][01011][-----][01100][00000][0] [00][000]
 *  [COP0  ][MFMC0][rt   ][12   ][0    ][sc][0 ][0  ]
 */
static void mipsInMFMC0(char *buffer, unsigned int i_inst) {
    int rt = (int)mipsGetOperand1(i_inst);
    int sc = (i_inst & 0x20) >> 5;
    if (sc == 0) {
        mipsType8(buffer, "di", rt);
    } else {
        mipsType8(buffer, "ei", rt);
    }
}

/* mipsInMOVfmt
 *  Floating Point Move
 *  MOV.S fd, fs
 *  MOV.D fd, fs
 *  MOV.PS fd, fs
 *  [010001][-----][00000][-----][-----][000110]
 *  [COP1  ][fmt  ][0    ][fs   ][fd   ][MOV   ]
 */
static void mipsInMOVfmt(char *buffer, unsigned int i_inst) {
    int fmt = (int)mipsGetOperand0(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    char mnem[10];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "mov.%s", sfmt);
    mipsType13(buffer, mnem, fd, fs);
}

/* mipsInMOVC1
 *  Move Conditional on Floating Point True/False
 *  MOVF rd, rs, cc
 *  MOVT rd, rs, cc
 *  [000000][-----][---][0][-][-----][00000][000001]
 * [SPECIAL][rs   ][cc ][0][t][rd   ][0    ][MOVC1 ]
 */
static void mipsInMOVC1(char *buffer, unsigned int i_inst) {
    int rs = (int)mipsGetOperand0(i_inst);
    int o1 = (int)mipsGetOperand1(i_inst);
    int cc = (o1 & 0x1C) >> 2;
    int tf = (o1 & 0x01);
    int rd = (int)mipsGetOperand2(i_inst);
    if (tf == 1) {
        mipsType9(buffer, "movt", rd, rs, cc);
    } else {
        mipsType9(buffer, "movf", rd, rs, cc);
    }
}

/* mipsInMOVCFfmt
 *  Floating Point Move Conditional
 *  MOVF.S fd, fs, cc
 *  MOVF.D fd, fs, cc
 *  MOVF.PS fd, fs, cc
 *  [010001][-----][---][0][-] [-----][-----][001011]
 *  [COP1  ][fmt  ][cc ][0][tf][fs   ][fd   ][FLOOR.L]
 */
static void mipsInMOVCFfmt(char *buffer, unsigned int i_inst) {
    const char *mn = "mov";
    char *cond;
    char *format;
    char mnem[10];
    int fmt = (int)mipsGetOperand0(i_inst);
    int op1 = (int)mipsGetOperand1(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    int cc = (op1 & 0x1C) >> 2;
    int tf = op1 & 1;
    format = mipsGetFormat(fmt);
    if (tf == 1) {
        cond = "t";
    } else {
        cond = "f";
    }
    sprintf(mnem, "%s%s.%s", mn, cond, format);
    mipsTypeFdFsCc(buffer, mnem, fd, fs, cc);
}

/* mipsInMOVN
 *  Move Conditional on Not Zero
 *  MOVN rd, rs, rt
 *  [000000][-----][-----][-----][00000][001011]
 * [SPECIAL][rs   ][rt   ][rd   ][0    ][MOVN  ]
 */
static void mipsInMOVN(char *buffer, unsigned int i_inst) {
    const char *mnem = "movn";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType4(buffer, mnem, rd, rs, rt);
}

/* mipsInMOVNfmt
 *  Floating Point Move Conditional on Not Zero
 *  MOVN.S fd, fs, rt
 *  MOVN.D fd, fs, rt
 *  MOVN.PS fd, fs, rt
 *  [010001][-----][-----][-----][-----][010011]
 *  [COP1  ][fmt  ][rt   ][fs   ][fd   ][MOVN  ]
 */
static void mipsInMOVNfmt(char *buffer, unsigned int i_inst) {
    int fmt = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    char mnem[10];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "movn.%s", sfmt);
    mipsTypeFdFsRt(buffer, mnem, fd, fs, rt);
}

/* mipsInMOVZ
 *  Move Conditional on Zero
 *  MOVZ rd, rs, rt
 *  [000000][-----][-----][-----][00000][001010]
 * [SPECIAL][rs   ][rt   ][rd   ][0    ][MOVZ  ]
 */
static void mipsInMOVZ(char *buffer, unsigned int i_inst) {
    const char *mnem = "movz";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType4(buffer, mnem, rd, rs, rt);
}

/* mipsInMOVZfmt
 *  Floating Point Move Conditional on Zero
 *  MOVZ.S fd, fs, rt
 *  MOVZ.D fd, fs, rt
 *  MOVZ.PS fd, fs, rt
 *  [010001][-----][-----][-----][-----][010010]
 *  [COP1  ][fmt  ][rt   ][fs   ][fd   ][MOVZ  ]
 */
static void mipsInMOVZfmt(char *buffer, unsigned int i_inst) {
    int fmt = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    char mnem[10];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "movz.%s", sfmt);
    mipsTypeFdFsRt(buffer, mnem, fd, fs, rt);
}

/* mipsInMSUB
 *  Multiply and Subtract Word to Hi, Lo
 *  MSUB rs, rt
 *  [011100]  [-----][-----][00000][00000][000100]
 *  [SPECIAL2][rs   ][rt   ][0    ][0    ][MSUB  ]
 */
static void mipsInMSUB(char *buffer, unsigned int i_inst) {
    const char *mnem = "msub";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    mipsType2(buffer, mnem, rs, rt);
}

/* mipsInMSUBfmt
 *  Floating Point Multiply Subtract
 *  MSUB.S fd, fr, fs, ft
 *  MSUB.D fd, fr, fs, ft
 *  MSUB.PS fd, fr, fs, ft
 *  [010011][-----][-----][-----][-----][101] [---]
 *  [COP1X ][fr   ][ft   ][fs   ][fd   ][MSUB][fmt]
 */
static void mipsInMSUBfmt(char *buffer, unsigned int i_inst) {
    int fd = (int)mipsGetOperand0(i_inst);
    int fr = (int)mipsGetOperand0(i_inst);
    int fs = (int)mipsGetOperand0(i_inst);
    int ft = (int)mipsGetOperand0(i_inst);
    int fmt = (i_inst & 0x7);
    char mnem[10];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "msub.%s", sfmt);
    mipsTypeFdFrFsFt(buffer, mnem, fd, fr, fs, ft);
}

/* mipsInMSUBU
 *  Multiply and Subtract Word to Hi, Lo
 *  MSUBU rs, rt
 *  [011100]  [-----][-----][00000][00000][000101]
 *  [SPECIAL2][rs   ][rt   ][0    ][0    ][MSUBU ]
 */
static void mipsInMSUBU(char *buffer, unsigned int i_inst) {
    const char *mnem = "msubu";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    mipsType2(buffer, mnem, rs, rt);
}


/* mipsInMTC0
 *  Move to Coprocessor 0
 *  MTC0 rt, rd
 *  MTC0 rt, rd, sel
 *  [010000][00100][-----][-----][00000000][---]
 *  [COP0  ][MT   ][rt   ][rd   ][0       ][sel]
 */
static void mipsInMTC0(char *buffer, unsigned int i_inst) {
    const char *mnem = "mtc0";
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    int sel = i_inst & 0x00000007;
    if (sel == 0) {
        mipsType2(buffer, mnem, rt, rd);
    } else {
        //mipsType7(buffer, mnem, rt, rd, sel);
        mipsTypeRtRdSel(buffer, mnem, rt, rd, sel);
    }
}

/* mipsInMTC1
 *  Move Word to Floating Point
 *  MTC1 rt, fs
 *  [010001][00100][-----][-----][00000000000]
 *  [COP1  ][MT   ][rt   ][fs   ][0          ]
 */
static void mipsInMTC1(char *buffer, unsigned int i_inst) {
    const char *mnem = "mtc1";
    int rt = (int)mipsGetOperand1(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    mipsType2f(buffer, mnem, rt, fs);
}

/* mipsInMTC2
 *  Move Word to Coprocessor 2
 *  MTC2 rt, rd
 *  [010010][00100][-----][-----][00000000000]
 *  [COP2  ][MT   ][rt   ][rd   ][0          ]
 */
static void mipsInMTC2(char *buffer, unsigned int i_inst) {
    const char *mnem = "mtc2";
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType2(buffer, mnem, rt, rd);
}

/* mipsInMTHC1
 *  Move Word to High Half of Floating Point Register
 *  MTHC1 rt, fs
 *  [010001][00111][-----][-----][00000000000]
 *  [COP1  ][MTH  ][rt   ][fs   ][0          ]
 */
static void mipsInMTHC1(char *buffer, unsigned int i_inst) {
    const char *mnem = "mthc1";
    int rt = (int)mipsGetOperand1(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    mipsType2f(buffer, mnem, rt, fs);
}

/* mipsInMTHC2
 *  Move Word to High Half of Coprocessor 2 Register
 *  MTHC2 rt, rd
 *  [010010][00111][-----][-----][00000000000]
 *  [COP2  ][MTH  ][rt   ][rd   ][0          ]
 */
static void mipsInMTHC2(char *buffer, unsigned int i_inst) {
    const char *mnem = "mthc2";
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType2(buffer, mnem, rt, rd);
}

/* mipsInMTHI
 *  Move to HI Register
 *  MTHI rs
 *  [000000][-----][000000000000000][010001]
 * [SPECIAL][rs   ][0              ][MTHI  ]
 */
static void mipsInMTHI(char *buffer, unsigned int i_inst) {
    const char *mnem = "mthi";
    int rs = (int)mipsGetOperand0(i_inst);
    mipsType8(buffer, mnem, rs);
}

/* mipsInMTIC
 *  Allegrex MTIC
 *  MTIC rt, rd
 *  [011100]  [00000][00000][00000][00000][100110]
 *  [SPECIAL2][0    ][rt   ][rd   ][     ][MTIC  ]
 */
static void mipsInMTIC(char *buffer, unsigned int i_inst) {
    const char *mnem = "mtic";
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType2(buffer, mnem, rt, rd);
}

/* mipsInMTLO
 *  Move to LO Register
 *  MTLO rs
 *  [000000][-----][000000000000000][010011]
 * [SPECIAL][rs   ][0              ][MTLO  ]
 */
static void mipsInMTLO(char *buffer, unsigned int i_inst) {
    const char *mnem = "mtlo";
    int rs = (int)mipsGetOperand0(i_inst);
    mipsType8(buffer, mnem, rs);
}

/* mipsInMUL
 *  Multiply Word to GPR
 *  MUL rd, rs, rt
 *  [011100]  [-----][-----][-----][00000][000010]
 *  [SPECIAL2][rs   ][rt   ][rd   ][0    ][MUL   ]
 */
static void mipsInMUL(char *buffer, unsigned int i_inst) {
    const char *mnem = "mul";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType4(buffer, mnem, rs, rt, rd);
}

/* mipsInMULfmt
 *  Floating Point Multiply
 *  MUL.S fd, fs, ft
 *  MUL.D fd, fs, ft
 *  MUL.PS fd, fs, ft
 *  [010001][-----][-----][-----][-----][000010]
 *  [COP1  ][fmt  ][ft   ][fs   ][fd   ][MUL   ]
 */
static void mipsInMULfmt(char *buffer, unsigned int i_inst) {
    int fmt = (int)mipsGetOperand0(i_inst);
    int ft = (int)mipsGetOperand1(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    char mnem[10];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "mul.%s", sfmt);
    mipsTypeFdFsFt(buffer, mnem, fd, fs, ft);
}

/* mipsInMULT
 *  Multiply Word
 *  MULT rs, rt
 *  [000000][-----][-----][0000000000][011000]
 * [SPECIAL][rs   ][rt   ][0         ][MULT  ]
 */
static void mipsInMULT(char *buffer, unsigned int i_inst) {
    const char *mnem = "mult";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    mipsType2(buffer, mnem, rs, rt);
}

/* mipsInMULTU
 *  Multiply Unsigned Word
 *  MULTU rs, rt
 *  [000000][-----][-----][0000000000][011001]
 * [SPECIAL][rs   ][rt   ][0         ][MULTU ]
 */
static void mipsInMULTU(char *buffer, unsigned int i_inst) {
    const char *mnem = "multu";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    mipsType2(buffer, mnem, rs, rt);
}

/* mipsInNEGfmt
 *  Floating Point Negate
 *  NEG.S fd, fs
 *  NEG.D fd, fs
 *  [010001][-----][00000][-----][-----][000111]
 *  [COP1  ][fmt  ][0    ][fs   ][fd   ][NEG   ]
 */
static void mipsInNEGfmt(char *buffer, unsigned int i_inst) {
    int fmt = (int)mipsGetOperand0(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    char mnem[10];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "neg.%s", sfmt);
    mipsTypeFdFs(buffer, mnem, fd, fs);
}

/* mipsInNMADDfmt
 *  Floating Point Negative Multiply Add
 *  NMADD.S fd, fr, fs, ft
 *  NMADD.D fd, fr, fs, ft
 *  NMADD.PS fd, fr, fs, ft
 *  [010011][-----][-----][-----][-----][110]  [---]
 *  [COP1X ][fr   ][ft   ][fs   ][fd   ][NMADD][fmt]
 */
static void mipsInNMADDfmt(char *buffer, unsigned int i_inst) {
    int fd = (int)mipsGetOperand0(i_inst);
    int fr = (int)mipsGetOperand0(i_inst);
    int fs = (int)mipsGetOperand0(i_inst);
    int ft = (int)mipsGetOperand0(i_inst);
    int fmt = (i_inst & 0x7);
    char mnem[10];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "nmadd.%s", sfmt);
    mipsTypeFdFrFsFt(buffer, mnem, fd, fr, fs, ft);
}

/* mipsInNMSUBfmt
 *  Floating Point Negative Multiply Subtract
 *  NMSUB.S fd, fr, fs, ft
 *  NMSUB.D fd, fr, fs, ft
 *  NMSUB.PS fd, fr, fs, ft
 *  [010011][-----][-----][-----][-----][110]  [---]
 *  [COP1X ][fr   ][ft   ][fs   ][fd   ][NMSUB][fmt]
 */
static void mipsInNMSUBfmt(char *buffer, unsigned int i_inst) {
    int fd = (int)mipsGetOperand0(i_inst);
    int fr = (int)mipsGetOperand0(i_inst);
    int fs = (int)mipsGetOperand0(i_inst);
    int ft = (int)mipsGetOperand0(i_inst);
    int fmt = (i_inst & 0x7);
    char mnem[10];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "nmsub.%s", sfmt);
    mipsTypeFdFrFsFt(buffer, mnem, fd, fr, fs, ft);
}

/* mipsInNOR
 *  Not Or
 *  NOR rd, rs, rt
 *  [000000][-----][-----][-----][00000][100111]
 * [SPECIAL][rs   ][rt   ][rd   ][0    ][NOR   ]
 */
static void mipsInNOR(char *buffer, unsigned int i_inst) {
    const char *mnem = "nor";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType4(buffer, mnem, rd, rs, rt);
}

/* mipsInOR
 *  Or
 *  OR rd, rs, rt
 *  [000000][-----][-----][-----][00000][100101]
 * [SPECIAL][rs   ][rt   ][rd   ][0    ][OR    ]
 */
static void mipsInOR(char *buffer, unsigned int i_inst) {
    const char *mnem = "or";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType4(buffer, mnem, rd, rs, rt);
}

/* mipsInORI
 *  Or Immediate
 *  ORI rs, rt, immediate
 *  [001101][-----][-----][----------------]
 *  [ORI   ][rs   ][rt   ][immediate       ]
 */
static void mipsInORI(char *buffer, unsigned int i_inst) {
    const char *mnem = "ori";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    short imm = mipsGetImmediate(i_inst);
    mipsType3(buffer, mnem, rs, rt, imm);
}

/* mipsInPLLPS
 *  Pair Lower Lower
 *  PLL.PS fd, fs, ft
 *  [010001][10110][-----][-----][-----][101100]
 *  [COP1  ][fmt  ][ft   ][fs   ][fd   ][PLL   ]
 */
static void mipsInPLLPS(char *buffer, unsigned int i_inst) {
    const char *mnem = "pll.ps";
    int ft = (int)mipsGetOperand0(i_inst);
    int fs = (int)mipsGetOperand1(i_inst);
    int fd = (int)mipsGetOperand2(i_inst);
    mipsTypeFdFsFt(buffer, mnem, fd, fs, ft);
}

/* mipsInPLUPS
 *  Pair Lower Upper
 *  PLU.PS fd, fs, ft
 *  [010001][10110][-----][-----][-----][101101]
 *  [COP1  ][fmt  ][ft   ][fs   ][fd   ][PLU   ]
 */
static void mipsInPLUPS(char *buffer, unsigned int i_inst) {
    const char *mnem = "plu.ps";
    int ft = (int)mipsGetOperand0(i_inst);
    int fs = (int)mipsGetOperand1(i_inst);
    int fd = (int)mipsGetOperand2(i_inst);
    mipsTypeFdFsFt(buffer, mnem, fd, fs, ft);
}

/* mipsInPREF
 *  Prefetch
 *  PREF hint, offset(base)
 *  [110011][-----][-----][----------------]
 *  [PREF  ][base ][hint ][offset          ]
 */
static void mipsInPREF(char *buffer, unsigned int i_inst) {
    const char *mnem = "pref";
    int base = (int)mipsGetOperand0(i_inst);
    int hint = (int)mipsGetOperand1(i_inst);
    short offset = mipsGetImmediate(i_inst);
    mipsType1c(buffer, mnem, base, hint, offset);
}

/* mipsInPREFX
 *  Prefetch Indexed
 *  PREFX hint, index(base)
 *  [010011][-----][-----][-----][00000][001111]
 *  [COP1X ][base ][index][hint ][0    ][PREFX ]
 */
static void mipsInPREFX(char *buffer, unsigned int i_inst) {
    const char *mnem = "prefx";
    int base = (int)mipsGetOperand0(i_inst);
    int index = (int)mipsGetOperand1(i_inst);
    int hint = (int)mipsGetOperand2(i_inst);
    mipsType1c(buffer, mnem, base, hint, index);
}

/* mipsInPULPS
 *  Pair Upper Lower
 *  PUL.PS fd, fs, ft
 *  [010001][10110][-----][-----][-----][101110]
 *  [COP1  ][fmt  ][ft   ][fs   ][fd   ][PUL   ]
 */
static void mipsInPULPS(char *buffer, unsigned int i_inst) {
    const char *mnem = "pul.ps";
    int ft = (int)mipsGetOperand0(i_inst);
    int fs = (int)mipsGetOperand1(i_inst);
    int fd = (int)mipsGetOperand2(i_inst);
    mipsTypeFdFsFt(buffer, mnem, fd, fs, ft);
}

/* mipsInPUUPS
 *  Pair Upper Upper
 *  PUU.PS fd, fs, ft
 *  [010001][10110][-----][-----][-----][101111]
 *  [COP1  ][fmt  ][ft   ][fs   ][fd   ][PUU   ]
 */
static void mipsInPUUPS(char *buffer, unsigned int i_inst) {
    const char *mnem = "puu.ps";
    int ft = (int)mipsGetOperand0(i_inst);
    int fs = (int)mipsGetOperand1(i_inst);
    int fd = (int)mipsGetOperand2(i_inst);
    mipsTypeFdFsFt(buffer, mnem, fd, fs, ft);
}

/* mipsInRDHWR
 *  Read Hardware Register
 *  RDHWR rt, rd
 *  [011111]  [00000][-----][-----][-----][111011]
 *  [SPECIAL3][0    ][rt   ][rd   ][0    ][RDHWR ]
 */
static void mipsInRDHWR(char *buffer, unsigned int i_inst) {
    const char *mnem = "rdhwr";
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType2(buffer, mnem, rt, rd);
}

/* mipsInRDPGPR
 *  Read GPR from Previous Shadow Set
 *  RDPGPR rd, rt
 *  [010000][01010] [-----][-----][00000000000]
 *  [COP0  ][RDPGPR][rt   ][rd   ][0          ]
 */
static void mipsInRDPGPR(char *buffer, unsigned int i_inst) {
    const char *mnem = "rdpgpr";
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType2(buffer, mnem, rd, rt);
}

/* mipsInRECIPfmt
 *  Reciprocal Approximation
 *  RECIP.S fd, fs
 *  RECIP.D fd, fs
 *  [010001][-----][00000][-----][-----][010101]
 *  [COP1  ][fmt  ][0    ][fs   ][fd   ][RECIP ]
 */
static void mipsInRECIPfmt(char *buffer, unsigned int i_inst) {
    int fmt = (int)mipsGetOperand0(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    char mnem[10];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "recip.%s", sfmt);
    mipsTypeFdFs(buffer, mnem, fd, fs);
}

/* mipsInROUNDLfmt
 *  Floating Point Round to Long Fixed Point
 *  ROUND.L.S fd, fs
 *  ROUND.L.D fd, fs
 *  [010001][-----][00000][-----][-----][001000]
 *  [COP1  ][fmt  ][0    ][fs   ][fd   ][ROUND.L]
 */
static void mipsInROUNDLfmt(char *buffer, unsigned int i_inst) {
    int fmt = (int)mipsGetOperand0(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    char mnem[10];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "round.l.%s", sfmt);
    mipsTypeFdFs(buffer, mnem, fd, fs);
}

/* mipsInROUNDWfmt
 *  Floating Point Round to Word Fixed Point
 *  ROUND.W.S fd, fs
 *  ROUND.W.D fd, fs
 *  [010001][-----][00000][-----][-----][001100]
 *  [COP1  ][fmt  ][0    ][fs   ][fd   ][ROUND.W]
 */
static void mipsInROUNDWfmt(char *buffer, unsigned int i_inst) {
    int fmt = (int)mipsGetOperand0(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    char mnem[10];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "round.w.%s", sfmt);
    mipsTypeFdFs(buffer, mnem, fd, fs);
}

/* mipsInRSQRTfmt
 *  Reciprocal Square Root Approximation
 *  RSQRT.S fd, fs
 *  RSQRT.D fd, fs
 *  [010001][-----][00000][-----][-----][010110]
 *  [COP1  ][fmt  ][0    ][fs   ][fd   ][RSQRT ]
 */
static void mipsInRSQRTfmt(char *buffer, unsigned int i_inst) {
    int fmt = (int)mipsGetOperand0(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    char mnem[10];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "rsqrt.%s", sfmt);
    mipsTypeFdFs(buffer, mnem, fd, fs);
}

/* mipsInSB
 *  Store Byte
 *  SB rt, offset(base)
 *  [101000][-----][-----][----------------]
 *  [SB    ][base ][rt   ][offset          ]
 */
static void mipsInSB(char *buffer, unsigned int i_inst) {
    const char *mnem = "sb";
    int i_base = (int)mipsGetOperand0(i_inst);
    int i_rt = (int)mipsGetOperand1(i_inst);
    short offset = mipsGetImmediate(i_inst);
    mipsType1(buffer, mnem, i_base, i_rt, offset);
}

/* mipsInSC
 *  Store Conditional Word
 *  SC rt, offset(base)
 *  [111000][-----][-----][----------------]
 *  [SC    ][base ][rt   ][offset          ]
 */
static void mipsInSC(char *buffer, unsigned int i_inst) {
    const char *mnem = "sc";
    int i_base = (int)mipsGetOperand0(i_inst);
    int i_rt = (int)mipsGetOperand1(i_inst);
    short offset = mipsGetImmediate(i_inst);
    mipsType1(buffer, mnem, i_base, i_rt, offset);
}

/* mipsInSDBBP
 *  Software Debug Breakpoint
 *  SDBBP code
 *  [011100]  [--------------------][111111]
 *  [SPECIAL2][code                ][SDBBP ]
 */
static void mipsInSDBBP(char *buffer, unsigned int i_inst) {
    const char *mnem = "sdbbp";
    unsigned int code = (i_inst & 0x03FFFFC0) >> 6;
    mipsType10(buffer, mnem, code);
}

/* mipsInSDC1
 *  Store Doubleword from Floating Point
 *  SDC1 ft, offset(base)
 *  [111101][-----][-----][----------------]
 *  [SDC1  ][base ][ft   ][offset          ]
 */
static void mipsInSDC1(char *buffer, unsigned int i_inst) {
    const char *mnem = "sdc1";
    int i_base = (int)mipsGetOperand0(i_inst);
    int i_ft = (int)mipsGetOperand1(i_inst);
    short offset = mipsGetImmediate(i_inst);
    mipsType1f(buffer, mnem, i_base, i_ft, offset);
}

/* mipsInSDC2
 *  StoreVector.Quadword Relative to Address
 *  sv.q rt, offset(base)
 *  [111110][-----][-----][----------------]
 *  [sv.q  ][base ][rt   ][offset          ]
 */
static void mipsInSDC2(char *buffer, unsigned int i_inst) {
    const char *mnem = "sv.q";
    int i_base = (int)mipsGetOperand0(i_inst);
    int i_rt = (int)mipsGetOperand1(i_inst);
    short offset = mipsGetImmediate(i_inst);
    mipsType1(buffer, mnem, i_base, i_rt, offset);
}

/* mipsInSDXC1
 *  Store Doubleword Indexed from Floating Point
 *  SDCX1 fs, index(base)
 *  [010001][-----][-----][-----][00000][001001]
 *  [COP1X ][base ][index][fs   ][0    ][SDXC1 ]
 */
static void mipsInSDXC1(char *buffer, unsigned int i_inst) {
    const char *mnem = "sdxc1";
    int base = (int)mipsGetOperand0(i_inst);
    int index = (int)mipsGetOperand1(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    mipsTypeFsIndexBase(buffer, mnem, fs, index, base);
}

/* mipsInSEB
 *  Sign-Extend Byte
 *  SEB rd, rt
 *  [011111]  [00000][-----][-----][10000][100000]
 *  [SPECIAL3][0    ][rt   ][rd   ][SEB  ][BSHFL ]
 */
static void mipsInSEB(char *buffer, unsigned int i_inst) {
    const char *mnem = "seb";
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType2(buffer, mnem, rd, rt);
}

/* mipsInSEH
 *  Sign-Extend Halfword
 *  SEH rd, rt
 *  [011111]  [00000][-----][-----][11000][100000]
 *  [SPECIAL3][0    ][rt   ][rd   ][SEH  ][BSHFL ]
 */
static void mipsInSEH(char *buffer, unsigned int i_inst) {
    const char *mnem = "seh";
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType2(buffer, mnem, rd, rt);
}

/* mipsInSH
 *  Store Halfword
 *  SH rt, offset(base)
 *  [101001][-----][-----][----------------]
 *  [SH    ][base ][rt   ][offset          ]
 */
static void mipsInSH(char *buffer, unsigned int i_inst) {
    const char *mnem = "sh";
    int i_base = (int)mipsGetOperand0(i_inst);
    int i_rt = (int)mipsGetOperand1(i_inst);
    short offset = mipsGetImmediate(i_inst);
    mipsType1(buffer, mnem, i_base, i_rt, offset);
}

/* mipsInSLL
 *  Shift Word Left Logical
 *  SLL rd, rt, sa
 *  [000000][00000][-----][-----][-----][000000]
 *  [SPECIAL][0   ][rt   ][rd   ][sa   ][SLL   ]
 */
static void mipsInSLL(char *buffer, unsigned int i_inst) {
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    int sa = (int)mipsGetOperand3(i_inst);
    if (rt == 0 && rd == 0) {
        switch(sa) {
            case 0:
                mipsType0(buffer, "nop");
                break;
            case 1:
                mipsType0(buffer, "ssnop");
                break;
            case 3:
                mipsType0(buffer, "ehb");
                break;
        }
        return;
    }
    mipsType7(buffer, "sll", rd, rt, sa);
}

/* mipsInSLLV
 *  Shift Word Left Logical Variable
 *  SLLV rd, rt, rs
 *  [000000][-----][-----][-----][00000][000100]
 * [SPECIAL][rs   ][rt   ][rd   ][0    ][SLLV  ]
 */
static void mipsInSLLV(char *buffer, unsigned int i_inst) {
    const char *mnem = "sllv";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType4(buffer, mnem, rd, rs, rt);
}

/* mipsInSLT
 *  Set on Less Than
 *  SLT rd, rs, rt
 *  [000000][-----][-----][-----][00000][101010]
 * [SPECIAL][rs   ][rt   ][rd   ][0    ][SLT   ]
 */
static void mipsInSLT(char *buffer, unsigned int i_inst) {
    const char *mnem = "slt";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType4(buffer, mnem, rd, rs, rt);
}

/* mipsInSLTI
 *  Set on Less Than Immediate
 *  SLTI rs, rt, immediate
 *  [001010][-----][-----][----------------]
 *  [SLTI  ][rs   ][rt   ][immediate       ]
 */
static void mipsInSLTI(char *buffer, unsigned int i_inst) {
    const char *mnem = "slti";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    short imm = mipsGetImmediate(i_inst);
    mipsType3(buffer, mnem, rs, rt, imm);
}

/* mipsInSLTIU
 *  Set on Less Than Immediate Unsigned
 *  SLTIU rs, rt, immediate
 *  [001011][-----][-----][----------------]
 *  [SLTIU ][rs   ][rt   ][immediate       ]
 */
static void mipsInSLTIU(char *buffer, unsigned int i_inst) {
    const char *mnem = "sltiu";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    unsigned short imm = mipsGetUnsignedImmediate(i_inst);
    mipsType3(buffer, mnem, rs, rt, imm);
}

/* mipsInSLTU
 *  Set on Less Than Unsigned
 *  SLTU rd, rs, rt
 *  [000000][-----][-----][-----][00000][101011]
 * [SPECIAL][rs   ][rt   ][rd   ][0    ][SLTU  ]
 */
static void mipsInSLTU(char *buffer, unsigned int i_inst) {
    const char *mnem = "sltu";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType4(buffer, mnem, rd, rs, rt);
}

/* mipsInSQRTfmt
 *  Floating Point Square Root
 *  SQRT.S fd, fs
 *  SQRT.D fd, fs
 *  [010001][-----][00000][-----][-----][000100]
 *  [COP1  ][fmt  ][0    ][fs   ][fd   ][SQRT  ]
 */
static void mipsInSQRTfmt(char *buffer, unsigned int i_inst) {
    int fmt = (int)mipsGetOperand0(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    char mnem[10];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "sqrt.%s", sfmt);
    mipsTypeFdFs(buffer, mnem, fd, fs);
}

/* mipsInSRA
 *  Shift Word Right Arithmetic
 *  SRA rd, rt, sa
 *  [000000][00000][-----][-----][-----][000011]
 *  [SPECIAL][0   ][rt   ][rd   ][sa   ][SRA   ]
 */
static void mipsInSRA(char *buffer, unsigned int i_inst) {
    const char *mnem = "sra";
    int i_rt = (int)mipsGetOperand1(i_inst);
    int i_rd = (int)mipsGetOperand2(i_inst);
    int i_sa = (int)mipsGetOperand3(i_inst);
    mipsType7(buffer, mnem, i_rd, i_rt, i_sa);
}

/* mipsInSRAV
 *  Shift Word Right Arithmetic Variable
 *  SRAV rd, rt, rs
 *  [000000][-----][-----][-----][00000][000100]
 * [SPECIAL][rs   ][rt   ][rd   ][0    ][SRAV  ]
 */
static void mipsInSRAV(char *buffer, unsigned int i_inst) {
    const char *mnem = "srav";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType4(buffer, mnem, rd, rt, rs);
}

/* mipsInSRL
 *  Shift Word Right Logical
 *  SRL rd, rt, sa
 *  ROTR rd, rt, sa
 *  [000000][0000][0][-----][-----][-----][000010]
 * [SPECIAL][0   ][R][rt   ][rd   ][sa   ][SRL   ]
 */
static void mipsInSRL(char *buffer, unsigned int i_inst) {
    int i_rt = (int)mipsGetOperand1(i_inst);
    int i_rd = (int)mipsGetOperand2(i_inst);
    int i_sa = (int)mipsGetOperand3(i_inst);
    int o0 = (int)mipsGetOperand0(i_inst);
    int r = (o0 & 0x1);
    if (r == 1) {
        mipsType7(buffer, "rotr", i_rd, i_rt, i_sa);
    } else {
        mipsType7(buffer, "srl", i_rd, i_rt, i_sa);
    }
}

/* mipsInSRLV
 *  Shift Word Right Logical Variable
 *  SRLV rd, rt, rs
 *  [000000][-----][-----][-----][0000][0][000110]
 * [SPECIAL][rs   ][rt   ][rd   ][0   ][R][SRLV  ]
 */
static void mipsInSRLV(char *buffer, unsigned int i_inst) {
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    int o3 = (int)mipsGetOperand3(i_inst);
    int r = o3 & 1;
    if (r == 1) {
        mipsType4(buffer, "rotrv", rd, rs, rt);
    } else {
        mipsType4(buffer, "srlv", rd, rs, rt);
    }
}

/* mipsInSUB
 *  Subtract Word
 *  SUB rd, rs, rt
 *  [000000][-----][-----][-----][00000][100010]
 * [SPECIAL][rs   ][rt   ][rd   ][0    ][SUB   ]
 */
static void mipsInSUB(char *buffer, unsigned int i_inst) {
    const char *mnem = "sub";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType4(buffer, mnem, rd, rs, rt);
}

/* mipsInSUBfmt
 *  Floating Point Subtract
 *  SUB.S fd, fs, ft
 *  SUB.D fd, fs, ft
 *  SUB.PS fd, fs, ft
 *  [010001][-----][-----][-----][-----][000001]
 *  [COP1  ][fmt  ][ft   ][fs   ][fd   ][SUB   ]
 */
static void mipsInSUBfmt(char *buffer, unsigned int i_inst) {
    int fmt = (int)mipsGetOperand0(i_inst);
    int ft = (int)mipsGetOperand1(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    char mnem[10];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "sub.%s", sfmt);
    mipsTypeFdFsFt(buffer, mnem, fd, fs, ft);
}

/* mipsInSUBU
 *  Subtract Unsigned Word
 *  SUBU rd, rs, rt
 *  [000000][-----][-----][-----][00000][100011]
 * [SPECIAL][rs   ][rt   ][rd   ][0    ][SUBU  ]
 */
static void mipsInSUBU(char *buffer, unsigned int i_inst) {
    const char *mnem = "subu";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType4(buffer, mnem, rd, rs, rt);
}

/* mipsInSUXC1
 *  Store Doubleword Indexed Unaligned from Floating Point
 *  SUCX1 fs, index(base)
 *  [010001][-----][-----][-----][00000][001101]
 *  [COP1X ][base ][index][fs   ][0    ][SUXC1 ]
 */
static void mipsInSUXC1(char *buffer, unsigned int i_inst) {
    const char *mnem = "suxc1";
    int base = (int)mipsGetOperand0(i_inst);
    int index = (int)mipsGetOperand1(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    mipsTypeFsIndexBase(buffer, mnem, fs, index, base);
}

/* mipsInSW
 *  Store Word
 *  SW rt, offset(base)
 *  [101011][-----][-----][----------------]
 *  [SW    ][base ][rt   ][offset          ]
 */
static void mipsInSW(char *buffer, unsigned int i_inst) {
    const char *mnem = "sw";
    int i_base = (int)mipsGetOperand0(i_inst);
    int i_rt = (int)mipsGetOperand1(i_inst);
    short offset = mipsGetImmediate(i_inst);
    mipsType1(buffer, mnem, i_base, i_rt, offset);
}

/* mipsInSWC1
 *  Store Word from Floating Point
 *  SWC1 ft, offset(base)
 *  [111001][-----][-----][----------------]
 *  [SWC1  ][base ][ft   ][offset          ]
 */
static void mipsInSWC1(char *buffer, unsigned int i_inst) {
    const char *mnem = "swc1";
    int i_base = (int)mipsGetOperand0(i_inst);
    int i_ft = (int)mipsGetOperand1(i_inst);
    short offset = mipsGetImmediate(i_inst);
    mipsType1f(buffer, mnem, i_base, i_ft, offset);
}

/* mipsInSWC2
 *  Store Word from Coprocessor 2
 *  SWC2 rt, offset(base)
 *  [111010][-----][-----][----------------]
 *  [SWC2  ][base ][rt   ][offset          ]
 */
static void mipsInSWC2(char *buffer, unsigned int i_inst) {
    const char *mnem = "swc2";
    int i_base = (int)mipsGetOperand0(i_inst);
    int i_rt = (int)mipsGetOperand1(i_inst);
    short offset = mipsGetImmediate(i_inst);
    mipsType1(buffer, mnem, i_base, i_rt, offset);
}

/* mipsInSWL
 *  Store Word Left
 *  SWL rt, offset(base)
 *  [101010][-----][-----][----------------]
 *  [SWL   ][base ][rt   ][offset          ]
 */
static void mipsInSWL(char *buffer, unsigned int i_inst) {
    const char *mnem = "swl";
    int i_base = (int)mipsGetOperand0(i_inst);
    int i_rt = (int)mipsGetOperand1(i_inst);
    short offset = mipsGetImmediate(i_inst);
    mipsType1(buffer, mnem, i_base, i_rt, offset);
}

/* mipsInSWR
 *  Store Word Right
 *  SWR rt, offset(base)
 *  [101110][-----][-----][----------------]
 *  [SWR   ][base ][rt   ][offset          ]
 */
static void mipsInSWR(char *buffer, unsigned int i_inst) {
    const char *mnem = "swr";
    int i_base = (int)mipsGetOperand0(i_inst);
    int i_rt = (int)mipsGetOperand1(i_inst);
    short offset = mipsGetImmediate(i_inst);
    mipsType1(buffer, mnem, i_base, i_rt, offset);
}

/* mipsInSWXC1
 *  Store Word Indexed from Floating Point
 *  SWCX1 fs, index(base)
 *  [010001][-----][-----][-----][00000][001000]
 *  [COP1X ][base ][index][fs   ][0    ][SWXC1 ]
 */
static void mipsInSWXC1(char *buffer, unsigned int i_inst) {
    const char *mnem = "swxc1";
    int base = (int)mipsGetOperand0(i_inst);
    int index = (int)mipsGetOperand1(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    mipsTypeFsIndexBase(buffer, mnem, fs, index, base);
}

/* mipsInSYNC
 *  Synchronize Shared Memory
 *  SYNC
 *  [000000][000000000000000][-----][001111]
 * [SPECIAL][0              ][stype][SYNC  ]
 */
static void mipsInSYNC(char *buffer, unsigned int i_instr) {
    mipsType0(buffer, "sync");
}

/* mipsInSYNCI
 *  Syncronize Caches to Make Instruction Writes Effective
 *  SYNCI offset(base)
 *  [000001][-----][11111][----------------]
 *  [REGIMM][base ][SYNCI][offset          ]
 */
static void mipsInSYNCI(char *buffer, unsigned int i_instr) {
    const char *mnem = "synci";
    int base = (int)mipsGetOperand0(i_instr);
    short offset = mipsGetImmediate(i_instr);
    mipsType6(buffer, mnem, offset, base);
}

/* mipsInSYSCALL
 *  System Call
 *  SYSCALL
 *  [000000][--------------------][001100]
 * [SPECIAL][code                ][SYSCALL]
 */
static void mipsInSYSCALL(char *buffer, unsigned int i_instr) {
    mipsType0(buffer, "syscall");
}

/* mipsInTEQ
 *  Trap if Equal
 *  TEQ rs, rt
 *  [000000][-----][-----][----------][110100]
 * [SPECIAL][rs   ][rt   ][code      ][TEQ   ]
 */
static void mipsInTEQ(char *buffer, unsigned int i_inst) {
    const char *mnem = "teq";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    mipsType2(buffer, mnem, rs, rt);
}

/* mipsInTEQI
 *  Trap if Equal Immediate
 *  TEQI rs, immediate
 *  [000001][-----][01100][----------------]
 *  [REGIMM][rs   ][TEQI ][immediate       ]
 */
static void mipsInTEQI(char *buffer, unsigned int i_inst) {
    const char *mnem = "teqi";
    int rs = (int)mipsGetOperand0(i_inst);
    unsigned short imm = mipsGetUnsignedImmediate(i_inst);
    mipsType5(buffer, mnem, rs, imm);
}

/* mipsInTGE
 *  Trap if Greater or Equal
 *  TGE rs, rt
 *  [000000][-----][-----][----------][110000]
 * [SPECIAL][rs   ][rt   ][code      ][TGE   ]
 */
static void mipsInTGE(char *buffer, unsigned int i_inst) {
    const char *mnem = "tge";
    int rs = (int)mipsGetOperand0(i_inst);
    int rt = (int)mipsGetOperand1(i_inst);
    mipsType2(buffer, mnem, rs, rt);
}

/* mipsInTGEI
 *  Trap if Greater or Equal Immediate
 *  TGEI rs, immediate
 *  [000001][-----][01000][----------------]
 *  [REGIMM][rs   ][TGEI ][immediate       ]
 */
static void mipsInTGEI(char *buffer, unsigned int i_inst) {
    const char *mnem = "tgei";
    int rs = (int)mipsGetOperand0(i_inst);
    unsigned short imm = mipsGetUnsignedImmediate(i_inst);
    mipsType5(buffer, mnem, rs, imm);
}

/* mipsInTGEIU
 *  Trap if Greater or Equal Immediate Unsigned
 *  TGEIU rs, immediate
 *  [000001][-----][01001][----------------]
 *  [REGIMM][rs   ][TGEIU][immediate       ]
 */
static void mipsInTGEIU(char *buffer, unsigned int i_inst) {
    const char *mnem = "tgeiu";
    int rs = (int)mipsGetOperand0(i_inst);
    unsigned short imm = mipsGetUnsignedImmediate(i_inst);
    mipsType5(buffer, mnem, rs, imm);
}

/* mipsInTGEU
 *  Trap if Greater or Equal Unsigned
 *  TGEU rs, rt
 *  [000000][-----][-----][----------][110001]
 * [SPECIAL][rs   ][rt   ][code      ][TGEU  ]
 */
static void mipsInTGEU(char *buffer, unsigned int i_inst) {
    const char *mnem = "tgeu";
    int i_rs = (int)mipsGetOperand0(i_inst);
    int i_rt = (int)mipsGetOperand1(i_inst);
    mipsType2(buffer, mnem, i_rs, i_rt);
}

/* mipsInTLBP
 *  Probe TLB for Matching Entry
 *  TLBP
 *  [010000][1] [0000000000000000000][001000]
 *  [COP0  ][CO][0                  ][TLBP  ]
 */
static void mipsInTLBP(char *buffer, unsigned int i_inst) {
    mipsType0(buffer, "tlbp");
}

/* mipsInTLBR
 *  Read Indexed TLB Entry
 *  TLBR
 *  [010000][1] [0000000000000000000][000001]
 *  [COP0  ][CO][0                  ][TLBR  ]
 */
static void mipsInTLBR(char *buffer, unsigned int i_inst) {
    mipsType0(buffer, "tlbr");
}

/* mipsInTLBWI
 *  Write Indexed TLB Entry
 *  TLBWI
 *  [010000][1] [0000000000000000000][000010]
 *  [COP0  ][CO][0                  ][TLBWI ]
 */
static void mipsInTLBWI(char *buffer, unsigned int i_inst) {
    mipsType0(buffer, "tlbwi");
}

/* mipsInTLBWR
 *  Write Random TLB Entry
 *  TLBWR
 *  [010000][1] [0000000000000000000][000110]
 *  [COP0  ][CO][0                  ][TLBWR ]
 */
static void mipsInTLBWR(char *buffer, unsigned int i_inst) {
    mipsType0(buffer, "tlbwr");
}

/* mipsInTLT
 *  Trap if Less Than
 *  TLT rs, rt
 *  [000000][-----][-----][----------][110010]
 * [SPECIAL][rs   ][rt   ][code      ][TLT   ]
 */
static void mipsInTLT(char *buffer, unsigned int i_inst) {
    const char *mnem = "tlt";
    int i_rs = (int)mipsGetOperand0(i_inst);
    int i_rt = (int)mipsGetOperand1(i_inst);
    mipsType2(buffer, mnem, i_rs, i_rt);
}

/* mipsInTLTI
 *  Trap if Less Than Immediate
 *  TLTI rs, immediate
 *  [000001][-----][01010][----------------]
 *  [REGIMM][rs   ][TLTI ][immediate       ]
 */
static void mipsInTLTI(char *buffer, unsigned int i_instr) {
    const char *mnem = "tlti";
    int i_rs = (int)mipsGetOperand0(i_instr);
    unsigned short imm = mipsGetUnsignedImmediate(i_instr);
    mipsType5(buffer, mnem, i_rs, imm);
}

/* mipsInTLTIU
 *  Trap if Less Than Immediate Unsigned
 *  TLTI rs, immediate
 *  [000001][-----][01011][----------------]
 *  [REGIMM][rs   ][TLTIU][immediate       ]
 */
static void mipsInTLTIU(char *buffer, unsigned int i_instr) {
    const char *mnem = "tltiu";
    int i_rs = (int)mipsGetOperand0(i_instr);
    unsigned short imm = mipsGetUnsignedImmediate(i_instr);
    mipsType5(buffer, mnem, i_rs, imm);
}

/* mipsInTLTU
 *  Trap if Less Than Unsigned
 *  TLTU rs, rt
 *  [000000][-----][-----][----------][110011]
 * [SPECIAL][rs   ][rt   ][code      ][TLTU  ]
 */
static void mipsInTLTU(char *buffer, unsigned int i_inst) {
    const char *mnem = "tltu";
    int i_rs = (int)mipsGetOperand0(i_inst);
    int i_rt = (int)mipsGetOperand1(i_inst);
    mipsType2(buffer, mnem, i_rs, i_rt);
}

/* mipsInTNE
 *  Trap if Not Equal
 *  TNE rs, rt
 *  [000000][-----][-----][----------][110110]
 * [SPECIAL][rs   ][rt   ][code      ][TNE   ]
 */
static void mipsInTNE(char *buffer, unsigned int i_inst) {
    const char *mnem = "tne";
    int i_rs = (int)mipsGetOperand0(i_inst);
    int i_rt = (int)mipsGetOperand1(i_inst);
    mipsType2(buffer, mnem, i_rs, i_rt);
}

/* mipsInTNEI
 *  Trap if Not Equal Immediate
 *  TNEI rs, immediate
 *  [000001][-----][01110][----------------]
 *  [REGIMM][rs   ][TNEI ][immediate       ]
 */
static void mipsInTNEI(char *buffer, unsigned int i_inst) {
    const char *mnem = "tnei";
    int i_rs = (int)mipsGetOperand0(i_inst);
    short imm = mipsGetImmediate(i_inst);
    mipsType5(buffer, mnem, i_rs, imm);
}

/* mipsInTRUNCLfmt
 *  Floating Point Truncate to Long Fixed Point
 *  TRUNC.L.S fd, fs
 *  TRUNC.L.D fd, fs
 *  [010001][-----][00000][-----][-----][001001]
 *  [COP1  ][fmt  ][0    ][fs   ][fd   ][TRUNC.L]
 */
static void mipsInTRUNCLfmt(char *buffer, unsigned int i_inst) {
    int fmt = (int)mipsGetOperand0(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    char mnem[10];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "trunc.l.%s", sfmt);
    mipsTypeFdFs(buffer, mnem, fd, fs);
}

/* mipsInTRUNCWfmt
 *  Floating Point Truncate to Word Fixed Point
 *  TRUNC.W.S fd, fs
 *  TRUNC.W.D fd, fs
 *  [010001][-----][00000][-----][-----][001101]
 *  [COP1  ][fmt  ][0    ][fs   ][fd   ][TRUNC.W]
 */
static void mipsInTRUNCWfmt(char *buffer, unsigned int i_inst) {
    int fmt = (int)mipsGetOperand0(i_inst);
    int fs = (int)mipsGetOperand2(i_inst);
    int fd = (int)mipsGetOperand3(i_inst);
    char mnem[10];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(mnem, "trunc.w.%s", sfmt);
    mipsTypeFdFs(buffer, mnem, fd, fs);
}

/* mipsInVABSfmt
 *  Vector Absolute Value (Single/Pair/Triple/Quad)
 *  vabs.s rd, rs
 *  vabs.p rd, rs
 *  vabs.t rd, rs
 *  vabs.q rd, rs
 *  [110100][000][0000001][-][-------][-][-------]
 *  [vabs. ][0  ][1      ][f][rs     ][F][rd     ]
 *  0x34, 0x0, 0x1
 */
static void mipsInVABSfmt(char* buffer, unsigned int i_inst) {
    int f = mipsGetVoperand2(i_inst);
    int rs = mipsGetVoperand3(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vadd.%s", sfmt);
    mipsTypeVrdVrs(buffer, mnem, rd, rs);
}

/* mipsInVADDfmt
 *  Vector Add (Single/Pair/Triple/Quad)
 *  vadd.s rd, rs, rt
 *  vadd.p rd, rs, rt
 *  vadd.t rd, rs, rt
 *  vadd.q rd, rs, rt
 *  [011000][000][-------][-][-------][-][-------]
 *  [vadd. ][0  ][rt     ][f][rs     ][F][rd     ]
 *  0x18, 0x0
 */
static void mipsInVADDfmt(char* buffer, unsigned int i_inst) {
    int rt = mipsGetVoperand1(i_inst);
    int f = mipsGetVoperand2(i_inst);
    int rs = mipsGetVoperand3(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vadd.%s", sfmt);
    mipsTypeVrdVrsVrt(buffer, mnem, rd, rs, rt);
}

/* mipsInVASINfmt
 *  Vector Arc Sin (Single/Pair/Triple/Quad)
 *  vasin.s rs, rd
 *  vasin.p rs, rd
 *  vasin.t rs, rd
 *  vasin.q rs, rd
 *  [110100][000][0010111][-][-------][-][-------]
 *  [0x34  ][0  ][0x17   ][f][rs     ][F][rd     ]
 *  0x34, 0x0, 0x17
 */
static void mipsInVASINfmt(char* buffer, unsigned int i_inst) {
    int f = mipsGetVoperand2(i_inst);
    int rs = mipsGetVoperand3(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vasin.%s", sfmt);
    mipsTypeVrdVrs(buffer, mnem, rs, rd);
}

/* mipsInVCOSfmt
 *  Vector Cosin (Single/Pair/Triple/Quad)
 *  vcos.s rs, rd
 *  vcos.p rs, rd
 *  vcos.t rs, rd
 *  vcos.q rs, rd
 *  [110100][000][0010011][-][-------][-][-------]
 *  [0x34  ][0  ][0x13   ][f][rs     ][F][rd     ]
 *  0x34, 0x0, 0x13
 */
static void mipsInVCOSfmt(char* buffer, unsigned int i_inst) {
    int f = mipsGetVoperand2(i_inst);
    int rs = mipsGetVoperand3(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vcos.%s", sfmt);
    mipsTypeVrdVrs(buffer, mnem, rs, rd);
}

/* mipsInVCSTfmt
 *  Vector Store Constant (Single/Pair/Triple/Quad)
 *  vcst.s rd, a
 *  vcst.p rd, a
 *  vcst.t rd, a
 *  vcst.q rd, a
 *  [110100][000][11][-----][-][0000000][-][-------]
 *  [VCOP  ][0  ][3 ][a    ][f][0      ][F][rd     ]
 *  0x34, 0, *0x3
 */
static void mipsInVCSTfmt(char* buffer, unsigned int i_inst) {
    int op1 = mipsGetVoperand1(i_inst);
    int a = op1 & 0x1F;
    int f = mipsGetVoperand2(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vcst.%s", sfmt);
    mipsTypeVrdA(buffer, mnem, rd, a);
}

/* mipsInVDHPfmt
 *  Vector Homogenous Dot Product (Single/Pair/Triple/Quad)
 *  vdhp.p rd, rs, rt
 *  vdhp.t rd, rs, rt
 *  vdhp.q rd, rs, rt
 *  [011001][100][-------][-][-------][-][-------]
 *  [vdhp. ][4  ][rt     ][f][rs     ][F][rd     ]
 *  0x19, 0x4
 */
static void mipsInVDHPfmt(char* buffer, unsigned int i_inst) {
    int rt = mipsGetVoperand1(i_inst);
    int f = mipsGetVoperand2(i_inst);
    int rs = mipsGetVoperand3(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vdhp.%s", sfmt);
    mipsTypeVrdVrsVrt(buffer, mnem, rd, rs, rt);
}

/* mipsInVDIVfmt
 *  Vector Divide (Single/Pair/Triple/Quad)
 *  vdiv.s rd, rs, rt
 *  vdiv.p rd, rs, rt
 *  vdiv.t rd, rs, rt
 *  vdiv.q rd, rs, rt
 *  [011000][111][-------][-][-------][-][-------]
 *  [vdiv. ][7  ][rt     ][f][rs     ][F][rd     ]
 *  0x18, 0x7
 */
static void mipsInVDIVfmt(char* buffer, unsigned int i_inst) {
    int rt = mipsGetVoperand1(i_inst);
    int f = mipsGetVoperand2(i_inst);
    int rs = mipsGetVoperand3(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vdiv.%s", sfmt);
    mipsTypeVrdVrsVrt(buffer, mnem, rd, rs, rt);
}

/* mipsInVDOTfmt
 *  Vector Dot Product (Single/Pair/Triple/Quad)
 *  vdot.p rd, rs, rt
 *  vdot.t rd, rs, rt
 *  vdot.q rd, rs, rt
 *  [011001][001][-------][-][-------][-][-------]
 *  [vdot. ][1  ][rt     ][f][rs     ][F][rd     ]
 *  0x19, 0x1
 */
static void mipsInVDOTfmt(char* buffer, unsigned int i_inst) {
    int rt = mipsGetVoperand1(i_inst);
    int f = mipsGetVoperand2(i_inst);
    int rs = mipsGetVoperand3(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vdot.%s", sfmt);
    mipsTypeVrdVrsVrt(buffer, mnem, rd, rs, rt);
}

/* mipsInVEXP2fmt
 *  Vector Exp2 (Single/Pair/Triple/Quad)
 *  vexp2.s rs, rd
 *  vexp2.p rs, rd
 *  vexp2.t rs, rd
 *  vexp2.q rs, rd
 *  [110100][000][0010100][-][-------][-][-------]
 *  [0x34  ][0  ][0x14   ][f][rs     ][F][rd     ]
 *  0x34, 0x0, 0x14
 */
static void mipsInVEXP2fmt(char* buffer, unsigned int i_inst) {
    int f = mipsGetVoperand2(i_inst);
    int rs = mipsGetVoperand3(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vexp2.%s", sfmt);
    mipsTypeVrdVrs(buffer, mnem, rs, rd);
}

/* mipsInVF2INfmt
 *  Vector Float to Integer - Round Near
 *  vf2in.s rd, rs, scale
 *  vf2in.p rd, rs, scale
 *  vf2in.t rd, rs, scale
 *  vf2in.q rd, rs, scale
 *  [110100][100][-------][-][-------][-][-------]
 *  [VCOP  ][4  ][scale  ][f][rs     ][F][rd     ]
 *  0x34, 0x4
 */
static void mipsInVF2INfmt(char* buffer, unsigned int i_inst) {
    int scale = mipsGetVoperand1(i_inst);
    int f = mipsGetVoperand2(i_inst);
    int rs = mipsGetVoperand3(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vf2in.%s", sfmt);
    mipsTypeVrdVrsScale(buffer, mnem, rd, rs, scale);
}

/* mipsInVI2Ffmt
 *  Vector Integer to Float
 *  vi2f.s rd, rs, scale
 *  vi2f.p rd, rs, scale
 *  vi2f.t rd, rs, scale
 *  vi2f.q rd, rs, scale
 *  [110100][101][-------][-][-------][-][-------]
 *  [VCOP  ][5  ][scale  ][f][rs     ][F][rd     ]
 *  0x34, 0x4
 */
static void mipsInVI2Ffmt(char* buffer, unsigned int i_inst) {
    int scale = mipsGetVoperand1(i_inst);
    int f = mipsGetVoperand2(i_inst);
    int rs = mipsGetVoperand3(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vi2f.%s", sfmt);
    mipsTypeVrdVrsScale(buffer, mnem, rd, rs, scale);
}

/* mipsInVI2Sfmt
 *  Vector Integer to Short (Pair/Quad)
 *  vi2s.p rd, rs
 *  vi2s.q rd, rs
 *  [110100][000][0111111][-][-------][-][-------]
 *  [VCOP  ][0  ][VI2S.  ][f][rs     ][F][rd     ]
 *  0x34, 0x0, 0x3F
 */
static void mipsInVI2Sfmt(char* buffer, unsigned int i_inst) {
    int f = mipsGetVoperand2(i_inst);
    int rs = mipsGetVoperand3(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vi2s.%s", sfmt);
    mipsTypeVrdVrs(buffer, mnem, rs, rd);
}

/* mipsInVI2UCQ
 *  Vector Integer to Unsigned Char Quad
 *  vi2uc.q rd, rs
 *  [110100][000][0111100][1][-------][1][-------]
 *  [VCOP  ][0  ][VI2UC.Q][f][rs     ][F][rd     ]
 *  0x34, 0x0, 0x3C
 */
static void mipsInVI2UCQ(char* buffer, unsigned int i_inst) {
    int rs = mipsGetVoperand3(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    mipsTypeVrdVrs(buffer, "vi2uc.q", rd, rs);
}

/* mipsInVIDTfmt
 *  Vector Load Identity (Pair/Triple/Quad)
 *  vidt.p rd
 *  vidt.t rd
 *  vidt.q rd
 *  [110100][000][0000011][-][0000000][-][-------]
 *  [0x34  ][0  ][3      ][f][0      ][F][rd     ]
 *  0x34, 0x0, 0x3
 */
static void mipsInVIDTfmt(char* buffer, unsigned int i_inst) {
    int f = mipsGetVoperand2(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vidt.%s", sfmt);
    mipsTypeVrd(buffer, mnem, rd);
}

/* mipsInVLOG2fmt
 *  Vector Log2 (Single/Pair/Triple/Quad)
 *  vlog2.s rs, rd
 *  vlog2.p rs, rd
 *  vlog2.t rs, rd
 *  vlog2.q rs, rd
 *  [110100][000][0010101][-][-------][-][-------]
 *  [0x34  ][0  ][0x15   ][f][rs     ][F][rd     ]
 *  0x34, 0x0, 0x15
 */
static void mipsInVLOG2fmt(char* buffer, unsigned int i_inst) {
    int f = mipsGetVoperand2(i_inst);
    int rs = mipsGetVoperand3(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vlog2.%s", sfmt);
    mipsTypeVrdVrs(buffer, mnem, rs, rd);
}

/* mipsInVMAXfmt
 *  Vector Maximum Value (Single/Pair/Triple/Quad)
 *  vmax.s rd, rs, rt
 *  vmax.p rd, rs, rt
 *  vmax.t rd, rs, rt
 *  vmax.q rd, rs, rt
 *  [011011][011][-------][-][-------][-][-------]
 *  [vmax. ][3  ][rt     ][f][rs     ][F][rd     ]
 *  0x1B, 0x3
 */
static void mipsInVMAXfmt(char* buffer, unsigned int i_inst) {
    int rt = mipsGetVoperand1(i_inst);
    int f = mipsGetVoperand2(i_inst);
    int rs = mipsGetVoperand3(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vmax.%s", sfmt);
    mipsTypeVrdVrsVrt(buffer, mnem, rd, rs, rt);
}

/* mipsInVMINfmt
 *  Vector Minimum Value (Single/Pair/Triple/Quad)
 *  vmin.s rd, rs, rt
 *  vmin.p rd, rs, rt
 *  vmin.t rd, rs, rt
 *  vmin.q rd, rs, rt
 *  [011011][010][-------][-][-------][-][-------]
 *  [vmin. ][2  ][rt     ][f][rs     ][F][rd     ]
 *  0x1B, 0x2
 */
static void mipsInVMINfmt(char* buffer, unsigned int i_inst) {
    int rt = mipsGetVoperand1(i_inst);
    int f = mipsGetVoperand2(i_inst);
    int rs = mipsGetVoperand3(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vmin.%s", sfmt);
    mipsTypeVrdVrsVrt(buffer, mnem, rd, rs, rt);
}

/* mipsInVMULfmt
 *  Vector Multiply (Single/Pair/Triple/Quad)
 *  vmul.s rd, rs, rt
 *  vmul.p rd, rs, rt
 *  vmul.t rd, rs, rt
 *  vmul.q rd, rs, rt
 *  [011001][000][-------][-][-------][-][-------]
 *  [vmul. ][0  ][rt     ][f][rs     ][F][rd     ]
 *  0x19, 0x0
 */
static void mipsInVMULfmt(char* buffer, unsigned int i_inst) {
    int rt = mipsGetVoperand1(i_inst);
    int f = mipsGetVoperand2(i_inst);
    int rs = mipsGetVoperand3(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vmul.%s", sfmt);
    mipsTypeVrdVrsVrt(buffer, mnem, rd, rs, rt);
}

/* mipsInVNEGfmt
 *  Vector Negate (Single/Pair/Triple/Quad)
 *  vneg.s rd, rs
 *  vneg.p rd, rs
 *  vneg.t rd, rs
 *  vneg.q rd, rs
 *  [110100][000][0000010][-][-------][-][-------]
 *  [vneg. ][0  ][2      ][f][rs     ][F][rd     ]
 *  0x34, 0x0, 0x2
 */
static void mipsInVNEGfmt(char* buffer, unsigned int i_inst) {
    int f = mipsGetVoperand2(i_inst);
    int rs = mipsGetVoperand3(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vneg.%s", sfmt);
    mipsTypeVrdVrs(buffer, mnem, rd, rs);
}

/* mipsInVNRCPfmt
 *  Vector Negative Reciprocal (Single/Pair/Triple/Quad)
 *  vnrcp.s rs, rd
 *  vnrcp.p rs, rd
 *  vnrcp.t rs, rd
 *  vnrcp.q rs, rd
 *  [110100][000][0011000][-][-------][-][-------]
 *  [0x34  ][0  ][0x18   ][f][rs     ][F][rd     ]
 *  0x34, 0x0, 0x18
 */
static void mipsInVNRCPfmt(char* buffer, unsigned int i_inst) {
    int f = mipsGetVoperand2(i_inst);
    int rs = mipsGetVoperand3(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vnrcp.%s", sfmt);
    mipsTypeVrdVrs(buffer, mnem, rs, rd);
}

/* mipsInVNSINfmt
 *  Vector Negative Sin (Single/Pair/Triple/Quad)
 *  vnsin.s rs, rd
 *  vnsin.p rs, rd
 *  vnsin.t rs, rd
 *  vnsin.q rs, rd
 *  [110100][000][0011010][-][-------][-][-------]
 *  [VCOP  ][0  ][NSIN.  ][f][rs     ][F][rd     ]
 *  0x34, 0x0, 0x1A
 */
static void mipsInVNSINfmt(char* buffer, unsigned int i_inst) {
    int f = mipsGetVoperand2(i_inst);
    int rs = mipsGetVoperand3(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vnsin.%s", sfmt);
    mipsTypeVrdVrs(buffer, mnem, rs, rd);
}

/* mipsInVONEfmt
 *  Set Vector One (Single/Pair/Triple/Quad)
 *  vone.s rd
 *  vone.p rd
 *  vone.t rd
 *  vone.q rd
 *  [110100][000][0000111][-][0000000][-][-------]
 *  [0x34  ][0  ][7      ][f][0      ][F][rd     ]
 *  0x34, 0x0, 0x7
 */
static void mipsInVONEfmt(char* buffer, unsigned int i_inst) {
    int f = mipsGetVoperand2(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vone.%s", sfmt);
    mipsTypeVrd(buffer, mnem, rd);
}

/* mipsInVRCPfmt
 *  Vector Reciprocal (Single/Pair/Triple/Quad)
 *  vrcp.s rs, rd
 *  vrcp.p rs, rd
 *  vrcp.t rs, rd
 *  vrcp.q rs, rd
 *  [110100][000][0010000][-][-------][-][-------]
 *  [0x34  ][0  ][0x10   ][f][rs     ][F][rd     ]
 *  0x34, 0x0, 0x10
 */
static void mipsInVRCPfmt(char* buffer, unsigned int i_inst) {
    int f = mipsGetVoperand2(i_inst);
    int rs = mipsGetVoperand3(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vrcp.%s", sfmt);
    mipsTypeVrdVrs(buffer, mnem, rs, rd);
}

/* mipsInVREXP2fmt
 *  Vector Reciprocal Exp2 (Single/Pair/Triple/Quad)
 *  vrexp2.s rs, rd
 *  vrexp2.p rs, rd
 *  vrexp2.t rs, rd
 *  vrexp2.q rs, rd
 *  [110100][000][0011100][-][-------][-][-------]
 *  [VCOP  ][0  ][REXP2. ][f][rs     ][F][rd     ]
 *  0x34, 0x0, 0x1C
 */
static void mipsInVREXP2fmt(char* buffer, unsigned int i_inst) {
    int f = mipsGetVoperand2(i_inst);
    int rs = mipsGetVoperand3(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vrexp2.%s", sfmt);
    mipsTypeVrdVrs(buffer, mnem, rs, rd);
}

/* mipsInVRSQfmt
 *  Vector Reciprocal Square Root (Single/Pair/Triple/Quad)
 *  vrsq.s rs, rd
 *  vrsq.p rs, rd
 *  vrsq.t rs, rd
 *  vrsq.q rs, rd
 *  [110100][000][0010001][-][-------][-][-------]
 *  [0x34  ][0  ][0x11   ][f][rs     ][F][rd     ]
 *  0x34, 0x0, 0x10
 */
static void mipsInVRSQfmt(char* buffer, unsigned int i_inst) {
    int f = mipsGetVoperand2(i_inst);
    int rs = mipsGetVoperand3(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vrsq.%s", sfmt);
    mipsTypeVrdVrs(buffer, mnem, rs, rd);
}

/* mipsInVSGNfmt
 *  Vector Sign (Single/Pair/Triple/Quad)
 *  vsgn.s rd, rs
 *  vsgn.p rd, rs
 *  vsgn.t rd, rs
 *  vsgn.q rd, rs
 *  [110100][000][1001010][-][-------][-][-------]
 *  [VCOP  ][0  ][VSGN.  ][f][rs     ][F][rd     ]
 *  0x34, 0x0, 0x4A
 */
static void mipsInVSGNfmt(char* buffer, unsigned int i_inst) {
    int f = mipsGetVoperand2(i_inst);
    int rs = mipsGetVoperand3(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vsgn.%s", sfmt);
    mipsTypeVrdVrs(buffer, mnem, rd, rs);
}

/* mipsInVSINfmt
 *  Vector Sin (Single/Pair/Triple/Quad)
 *  vsin.s rs, rd
 *  vsin.p rs, rd
 *  vsin.t rs, rd
 *  vsin.q rs, rd
 *  [110100][000][0010010][-][-------][-][-------]
 *  [0x34  ][0  ][0x12   ][f][rs     ][F][rd     ]
 *  0x34, 0x0, 0x12
 */
static void mipsInVSINfmt(char* buffer, unsigned int i_inst) {
    int f = mipsGetVoperand2(i_inst);
    int rs = mipsGetVoperand3(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vsin.%s", sfmt);
    mipsTypeVrdVrs(buffer, mnem, rs, rd);
}

/* mipsInVSQRTfmt
 *  Vector Square Root (Single/Pair/Triple/Quad)
 *  vsqrt.s rs, rd
 *  vsqrt.p rs, rd
 *  vsqrt.t rs, rd
 *  vsqrt.q rs, rd
 *  [110100][000][0010110][-][-------][-][-------]
 *  [0x34  ][0  ][0x16   ][f][rs     ][F][rd     ]
 *  0x34, 0x0, 0x16
 */
static void mipsInVSQRTfmt(char* buffer, unsigned int i_inst) {
    int f = mipsGetVoperand2(i_inst);
    int rs = mipsGetVoperand3(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vsqrt.%s", sfmt);
    mipsTypeVrdVrs(buffer, mnem, rs, rd);
}

/* mipsInVSUBfmt
 *  Vector Subtract (Single/Pair/Triple/Quad)
 *  vsub.s rd, rs, rt
 *  vsub.p rd, rs, rt
 *  vsub.t rd, rs, rt
 *  vsub.q rd, rs, rt
 *  [011010][000][-------][-][-------][-][-------]
 *  [vsub. ][0  ][rt     ][f][rs     ][F][rd     ]
 *  0x1A, 0x0
 */
static void mipsInVSUBfmt(char* buffer, unsigned int i_inst) {
    int rt = mipsGetVoperand1(i_inst);
    int f = mipsGetVoperand2(i_inst);
    int rs = mipsGetVoperand3(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vsub.%s", sfmt);
    mipsTypeVrdVrsVrt(buffer, mnem, rd, rs, rt);
}

/* mipsInVZEROfmt
 *  Set Vector Zero (Single/Pair/Triple/Quad)
 *  vzero.s rd
 *  vzero.p rd
 *  vzero.t rd
 *  vzero.q rd
 *  [110100][000][0000110][-][0000000][-][-------]
 *  [0x34  ][0  ][6      ][f][0      ][F][rd     ]
 *  0x34, 0x0, 0x6
 */
static void mipsInVZEROfmt(char* buffer, unsigned int i_inst) {
    int f = mipsGetVoperand2(i_inst);
    int F = mipsGetVoperand4(i_inst);
    int rd = mipsGetVoperand5(i_inst);
    char mnem[10];
    char* sfmt = mipsGetVformat(f, F);
    sprintf(mnem, "vzero.%s", sfmt);
    mipsTypeVrd(buffer, mnem, rd);
}

/* mipsInWAIT
 *  Enter Standby Mode
 *  WAIT
 *  [010000][1] [0000000000000000000][100000]
 *  [COP0  ][CO][0                  ][WAIT  ]
 */
static void mipsInWAIT(char *buffer, unsigned int i_inst) {
    mipsType0(buffer, "wait");
}

/* mipsInWRPGPR
 *  Write to GPR in Previous Shadow Set
 *  WRPGPR rd, rt
 *  [010000][01110] [-----][-----][00000000000]
 *  [COP0  ][WRPGPR][rt   ][rd   ][0          ]
 */
static void mipsInWRPGPR(char *buffer, unsigned int i_inst) {
    const char *mnem = "wrpgpr";
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType2(buffer, mnem, rd, rt);
}

/* mipsInWSBH
 *  Word Swap Bytes Within Halfwords
 *  WSBH rd, rt
 *  [011111]  [00000][-----][-----][00010][100000]
 *  [SPECIAL3][0    ][rt   ][rd   ][WSBH ][BSHFL ]
 */
static void mipsInWSBH(char *buffer, unsigned int i_inst) {
    const char *mnem = "wsbh";
    int rt = (int)mipsGetOperand1(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    mipsType2(buffer, mnem, rd, rt);
}

/* mipsInXOR
 *  Exclusive Or
 *  OR rd, rs, rt
 *  [000000][-----][-----][-----][00000][100110]
 * [SPECIAL][rs   ][rt   ][rd   ][0    ][XOR   ]
 */
static void mipsInXOR(char *buffer, unsigned int i_inst) {
    const char *mnem = "xor";
    int i_rs = (int)mipsGetOperand0(i_inst);
    int i_rt = (int)mipsGetOperand1(i_inst);
    int i_rd = (int)mipsGetOperand2(i_inst);
    mipsType4(buffer, mnem, i_rd, i_rs, i_rt);
}

/* mipsInXORI
 *  Exclusive Or Immediate
 *  XORI rs, rt, immediate
 *  [001110][-----][-----][----------------]
 *  [XORI  ][rs   ][rt   ][immediate       ]
 */
static void mipsInXORI(char *buffer, unsigned int i_inst) {
    const char *mnem = "xori";
    int i_rs = (int)mipsGetOperand0(i_inst);
    int i_rt = (int)mipsGetOperand1(i_inst);
    short imm = mipsGetImmediate(i_inst);
    mipsType3(buffer, mnem, i_rs, i_rt, imm);
}

/* mipsType0
 *  Render a Type 0 instruction.
 *  [MNEM]
 */
static void mipsType0(char *buffer, const char *mnem) {
    sprintf(buffer, "%s", mnem);
}

/* mipsType1
 *  Render a Type 1 instruction.
 *  [MNEM] rt, offset(base)
 */
static void mipsType1(char *buffer, const char *mnem, int i_base, int i_rt, 
        short offset) {
    const char *base = mipsRegisterArray[i_base];
    const char *rt = mipsRegisterArray[i_rt];
    sprintf(buffer, "%-8s $%s, 0x%X($%s)", mnem, rt, offset, base);
}

/* mipsType1c
 *  Render a Type 1 cache instruction.
 *  [MNEM] op, offset(base)
 */
static void mipsType1c(char *buffer, const char *mnem, int i_base, int i_op,
        short offset) {
    const char *base = mipsRegisterArray[i_base];
    sprintf(buffer, "%-8s 0x%X, 0x%X($%s)", mnem, i_op, offset, base);
}

/* mipsType1f
 *  Render a Type 1f instruction.
 *  [MNEM] ft, offset(base)
 */
static void mipsType1f(char *buffer, const char *mnem, int i_base, int i_rt, 
        short offset) {
    const char *base = mipsRegisterArray[i_base];
    const char *rt = mipsFpRegNames[i_rt];
    sprintf(buffer, "%-8s $%s, 0x%X($%s)", mnem, rt, offset, base);
}

/* mipsType2
 *  Render a Type 2 instruction.
 *  [MNEM] rs, rt
 */
static void mipsType2(char *buffer, const char *mnem, int i_rs, int i_rt) {
    const char *rs = mipsRegisterArray[i_rs];
    const char *rt = mipsRegisterArray[i_rt];
    sprintf(buffer, "%-8s $%s, $%s", mnem, rs, rt);
}

/* mipsType2f
 *  Render a Type 2f instruction.
 *  [MNEM] rt, fs
 */
static void mipsType2f(char *buffer, const char *mnem, int i_rt, int i_fs) {
    const char *rt = mipsRegisterArray[i_rt];
    const char *fs = mipsFpRegNames[i_fs];
    sprintf(buffer, "%-8s $%s, $%s", mnem, rt, fs);
}

/* mipsType3
 *  Render a Type 3 instruction.
 *  [MNEM] rs, rt, immediate
 */
static void mipsType3(char *buffer, const char *mnem, int i_rs, int i_rt,
        short imm) {
    const char *rs = mipsRegisterArray[i_rs];
    const char *rt = mipsRegisterArray[i_rt];
    sprintf(buffer, "%-8s $%s, $%s, 0x%04X", mnem, rs, rt, (unsigned short)imm);
}

/* mipsType4
 *  Render a Type 4 instruction.
 *  [MNEM] rd, rs, rt
 */
static void mipsType4(char *buffer, const char *mnem, int i_rd, int i_rs, int i_rt) {
    const char *rd = mipsRegisterArray[i_rd];
    const char *rs = mipsRegisterArray[i_rs];
    const char *rt = mipsRegisterArray[i_rt];
    sprintf(buffer, "%-8s $%s, $%s, $%s", mnem, rd, rs, rt);
}

/* mipsType5
 *  Render a Type5 instruction.
 *  [MNEM] rs, immediate
 */
static void mipsType5(char *buffer, const char *mnem, int i_rs, short n_imm) {
    const char *rs = mipsRegisterArray[i_rs];
    sprintf(buffer, "%-8s $%s, 0x%X", mnem, rs, (unsigned short)n_imm);
}

/* mipsType6
 *  Render a Type6 instruction.
 *  [MNEM] offset(base)
 */
static void mipsType6(char *buffer, const char *mnem, short offset, int i_base) {
    const char *base = mipsRegisterArray[i_base];
    sprintf(buffer, "%-8s 0x%X($%s)", mnem, offset, base);
}

/* mipsType7
 *  Render a Type 7 instruction.
 *  [MNEM] rd, rt, sa
 */
static void mipsType7(char *buffer, const char *mnem, int i_rd, int i_rt, int sa) {
    const char *rd = mipsRegisterArray[i_rd];
    const char *rt = mipsRegisterArray[i_rt];
    sprintf(buffer, "%-8s $%s, $%s, 0x%X", mnem, rd, rt, sa);
}

/* mipsType8
 *  Render a Type 8 instruction.
 *  [MNEM] rs
 */
static void mipsType8(char *buffer, const char *mnem, int i_rs) {
    const char *rs = mipsRegisterArray[i_rs];
    sprintf(buffer, "%-8s $%s", mnem, rs);
}

/* mipsType9
 *  Render a Type 9 instruction.
 *  [MNEM] rd, rs, cc
 */
static void mipsType9(char *buffer, const char *mnem, int i_rd, int i_rs, int i_cc) {
    const char *rd = mipsRegisterArray[i_rd];
    const char *rs = mipsRegisterArray[i_rs];
    sprintf(buffer, "%-8s $%s, $%s, 0x%X", mnem, rd, rs, i_cc);
}

/* mipsType10
 *  Render a Type 10 instruction.
 *  [MNEM] target
 */
static void mipsType10(char *buffer, const char *mnem, unsigned int target) {
    sprintf(buffer, "%-8s 0x%08X", mnem, target);
}

/* mipsType11
 *  Render a Type 11 instruction.
 *  [MNEM] rs, rt, target
 */
static void mipsType11(char *buffer, const char *mnem, int i_rs, int i_rt,
        unsigned int target) {
    const char *rs = mipsRegisterArray[i_rs];
    const char *rt = mipsRegisterArray[i_rt];
    sprintf(buffer, "%-8s $%s, $%s, 0x%08X", mnem, rs, rt, target);
}

/* mipsType12
 *  Render a Type 12 instruction.
 *  [MNEM] rs, target
 */
static void mipsType12(char *buffer, const char *mnem, int i_rs, 
        unsigned int target) {
    const char *rs = mipsRegisterArray[i_rs];
    sprintf(buffer, "%-8s $%s, 0x%08X", mnem, rs, target);
}

/* mipsType13
 *  Render a Type 13 instruction.
 *  [MNEM] fd, fs
 */
static void mipsType13(char *buffer, const char *mnem, int i_fd, int i_fs) {
    const char *fd = mipsFpRegNames[i_fd];
    const char *fs = mipsFpRegNames[i_fs];
    sprintf(buffer, "%-8s $%s, $%s", mnem, fd, fs);
}

/* mipsTypeCcDest
 *  Render a Type 'cc, dest' instruction.
 *  [MNEM] cc, dest
 */
static void mipsTypeCcDest(char *buffer, const char *mnem, int cc, unsigned int dest) {
    sprintf(buffer, "%-8s %d, 0x%08X", mnem, cc, dest);
}

/* mipsTypeCcFsFt
 *  Render a Type 'cc, fs, ft' instruction.
 *  [MNEM] cc, fs, ft
 */
static void mipsTypeCcFsFt(char *buffer, const char *mnem, int cc, int i_fs, int i_ft) {
    const char *fs = mipsFpRegNames[i_fs];
    const char *ft = mipsFpRegNames[i_ft];
    sprintf(buffer, "%-8s %d, $%s, $%s", mnem, cc, fs, ft);
}

/* mipsTypeFdFrFsFt
 *  Render a Type 'fd, fr, fs, ft' instruction.
 *  [MNEM] fd, fr, fs, ft
 */
static void mipsTypeFdFrFsFt(char *buffer, const char *mnem, int iFd, int iFr, int iFs, int iFt) {
    const char *fd = mipsFpRegNames[iFd];
    const char *fr = mipsFpRegNames[iFr];
    const char *fs = mipsFpRegNames[iFs];
    const char *ft = mipsFpRegNames[iFt];
    sprintf(buffer, "%-8s $%s, $%s, $%s, $%s", mnem, fd, fr, fs, ft);
}

/* mipsTypeFdFs
 *  Render a Type 'fd, fs' instruction.
 *  [MNEM] fd, fs
 */
static void mipsTypeFdFs(char *buffer, const char *mnem, int i_fd, int i_fs) {
    const char *fd = mipsFpRegNames[i_fd];
    const char *fs = mipsFpRegNames[i_fs];
    sprintf(buffer, "%-8s $%s, $%s", mnem, fd, fs);
}

/* mipsTypeFdFsCc
 *  Render a Type 'fd, fs, cc' instruction.
 *  [MNEM] fd, fs, cc
 */
static void mipsTypeFdFsCc(char *buffer, const char *mnem, int i_fd, int i_fs, int i_cc) {
    const char *fd = mipsFpRegNames[i_fd];
    const char *fs = mipsFpRegNames[i_fs];
    sprintf(buffer, "%-8s $%s, $%s, %d", mnem, fd, fs, i_cc);
}

/* mipsTypeFdFsFt
 *  Render a Type 'fd, fs, ft' instruction.
 *  [MNEM] fd, fs, ft
 */
static void mipsTypeFdFsFt(char *buffer, const char *mnem, int iFd, int iFs, int iFt) {
    const char *fd = mipsFpRegNames[iFd];
    const char *fs = mipsFpRegNames[iFs];
    const char *ft = mipsFpRegNames[iFt];
    sprintf(buffer, "%-8s $%s, $%s, $%s", mnem, fd, fs, ft);
}

/* mipsTypeFdFsFtRs
 *  Render a Type 'fd, fs, ft, rs' instruction.
 *  [MNEM] fd, fs, ft, rs
 */
static void mipsTypeFdFsFtRs(char *buffer, const char *mnem, int iFd, int iFs, int iFt, int iRs) {
    const char *fd = mipsFpRegNames[iFd];
    const char *fs = mipsFpRegNames[iFs];
    const char *ft = mipsFpRegNames[iFt];
    const char *rs = mipsRegisterArray[iRs];
    sprintf(buffer, "%-8s $%s, $%s, $%s, $%s", mnem, fd, fs, ft, rs);
}

/* mipsTypeFdFsRt
 *  Render a Type 'fd, fs, rt' instruction.
 *  [MNEM] fd, fs, rt
 */
static void mipsTypeFdFsRt(char *buffer, const char *mnem, int iFd, int iFs, int iRt) {
    const char *fd = mipsFpRegNames[iFd];
    const char *fs = mipsFpRegNames[iFs];
    const char *rt = mipsRegisterArray[iRt];
    sprintf(buffer, "%-8s $%s, $%s, $%s", mnem, fd, fs, rt);
}

/* mipsTypeFsIndexBase
 *  Render a Type 'fs, index(base)' instruction.
 *  [MNEM] fs, index(base)
 */
static void mipsTypeFsIndexBase(char *buffer, const char *mnem, int iFs, int iIndex, int iBase) {
    const char *fs = mipsFpRegNames[iFs];
    const char *index = mipsRegisterArray[iIndex];
    const char *base = mipsRegisterArray[iBase];
    sprintf(buffer, "%-8s $%s, $%s($%s)", mnem, fs, index, base);
}

/* mipsTypeRtOffsetBase
 *  Render a Type 'rt, offset(base)' instruction.
 *  [MNEM] rt, offset(base)
 */
static void mipsTypeRtOffsetBase(char *buffer, const char *mnem, int i_rt, short offset, int i_base) {
    const char *base = mipsRegisterArray[i_base];
    const char *rt = mipsRegisterArray[i_rt];
    sprintf(buffer, "%-8s $%s, 0x%X($%s)", mnem, rt, offset, base);
}

/* mipsTypeRtRdSel
 *  Render a Type 'rt, rd, sel' instruction.
 *  [MNEM] rt, rd, sel
 */
static void mipsTypeRtRdSel(char* buffer, const char* mnem, 
        int i_rt, int i_rd, int i_sel) {
    const char *rt = mipsRegisterArray[i_rt];
    const char *rd = mipsRegisterArray[i_rd];
    const char *sel = mipsControlRegister[i_sel];
    sprintf(buffer, "%-8s $%s, $%s, $%s", mnem, rt, rd, sel);
}

/* mipsTypeRtRsPosSize
 *  Render a Type 'rt, rs, pos, size' instruction.
 *  [MNEM] rt, rs, pos, size
 */
static void mipsTypeRtRsPosSize(char *buffer, const char *mnem, int iRt, int iRs, int iPos, int iSize) {
    const char *rt = mipsRegisterArray[iRt];
    const char *rs = mipsRegisterArray[iRs];
    sprintf(buffer, "%-8s $%s, $%s, %d, %d", mnem, rt, rs, iPos, iSize);
}

static void mipsTypeVrd(char* buffer, const char* mnem, int iRd) {
    sprintf(buffer, "%-8s $v%d", mnem, iRd);
}

static void mipsTypeVrdA(char* buffer, const char* mnem, int iRd, int iA) {
    char* sCn = "0";
    char buf[3];
    switch (iA) {
        case 1:
            sCn = "HUGE";
            break;
        case 2:
            sCn = "SQRT(2)";
            break;
        case 3:
            sCn = "1/SQRT(2)";
            break;
        case 4:
            sCn = "2/SQRT(PI)";
            break;
        case 5:
            sCn = "2/PI";
            break;
        case 6:
            sCn = "1/PI";
            break;
        case 7:
            sCn = "PI/4";
            break;
        case 8:
            sCn = "PI/2";
            break;
        case 9:
            sCn = "PI";
            break;
        case 10:
            sCn = "E";
            break;
        case 11:
            sCn = "LOG2E";
            break;
        case 12:
            sCn = "LOG10E";
            break;
        case 13:
            sCn = "LN2";
            break;
        case 14:
            sCn = "LN10";
            break;
        case 15:
            sCn = "2*PI";
            break;
        case 16:
            sCn = "PI/6";
            break;
        case 17:
            sCn = "LOG10TWO";
            break;
        case 18:
            sCn = "LOG2TEN";
            break;
        case 19:
            sCn = "SQRT(3)/2";
            break;
        default:
            sprintf(buf, "%d", iA);
            sCn = buf;
    }
    sprintf(buffer, "%-8s $v%d, %s", mnem, iRd, sCn);
}

static void mipsTypeVrdVrs(char* buffer, const char* mnem, int iRd, int iRs) {
    sprintf(buffer, "%-8s $v%d, $v%d", mnem, iRd, iRs);
}

static void mipsTypeVrdVrsScale(char* buffer, const char* mnem,
        int iRd, int iRs, int scale) {
    sprintf(buffer, "%-8s $v%d, $v%d, %d", mnem, iRd, iRs, scale);
}

static void mipsTypeVrdVrsVrt(char* buffer, const char* mnem,
        int iRd, int iRs, int iRt) {
    sprintf(buffer, "%-8s $v%d, $v%d, $v%d", mnem, iRd, iRs, iRt);
}

void mipsDecode(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    unsigned char uc_opcode;
    unsigned char uc_func;
    unsigned char uc_rt;

    uc_opcode = mipsGetOpCode(i_inst);
    switch (uc_opcode) {
        case 0: /* SPECIAL */
            decodeSPECIAL(buffer, i_inst, i_addr);
            break;
        case 1: /* REGIMM */
            decodeREGIMM(buffer, i_inst, i_addr);
            break;
        case 0x02: /* J */
            mipsInJ(buffer, i_inst, i_addr);
            break;
        case 0x03: /* JAL */
            mipsInJAL(buffer, i_inst, i_addr);
            break;
        case 0x04: /* BEQ */
            mipsInBEQ(buffer, i_inst, i_addr);
            break;
        case 0x05: /* BNE */
            mipsInBNE(buffer, i_inst, i_addr);
            break;
        case 0x06: /* BLEZ */
            mipsInBLEZ(buffer, i_inst, i_addr);
            break;
        case 0x07: /* BGTZ */
            mipsInBGTZ(buffer, i_inst, i_addr);
            break;
        case 0x08: /* ADDI */
            mipsInADDI(buffer, i_inst);
            break;
        case 0x09: /* ADDIU */
            mipsInADDIU(buffer, i_inst);
            break;
        case 0x0A: /* SLTI */
            mipsInSLTI(buffer, i_inst);
            break;
        case 0x0B: /* SLTIU */
            mipsInSLTIU(buffer, i_inst);
            break;
        case 0x0C: /* ANDI */
            mipsInANDI(buffer, i_inst);
            break;
        case 0x0D: /* ORI */
            mipsInORI(buffer, i_inst);
            break;
        case 0x0E: /* XORI */
            mipsInXORI(buffer, i_inst);
            break;
        case 0x0F: /* LUI */
            mipsInLUI(buffer, i_inst);
            break;
        case 0x10: /* *COP0 */
            decodeCOP0(buffer, i_inst, i_addr);
            break;
        case 0x11: /* *COP1 */
            decodeCOP1(buffer, i_inst, i_addr);
            break;
        case 0x12: /* *COP2 */
            decodeCOP2(buffer, i_inst, i_addr);
            break;
        case 0x13: /* *COP1X */
            decodeCOP1X(buffer, i_inst, i_addr);
            break;
        case 0x14: /* BEQL */
            mipsInBEQL(buffer, i_inst, i_addr);
            break;
        case 0x15: /* BNEL */
            mipsInBNEL(buffer, i_inst, i_addr);
            break;
        case 0x16: /* BLEZL */
            mipsInBLEZL(buffer, i_inst, i_addr);
            break;
        case 0x17: /* BGTZL */
            mipsInBGTZL(buffer, i_inst, i_addr);
            break;
        case 0x1C: /* *SPECIAL2 */
            decodeSPECIAL2(buffer, i_inst, i_addr);
            break;
        case 0x1F: /* *SPECIAL3 */
            decodeSPECIAL3(buffer, i_inst, i_addr);
            break;
        case 0x20: /* LB */
            mipsInLB(buffer, i_inst);
            break;
        case 0x21: /* LH */
            mipsInLH(buffer, i_inst);
            break;
        case 0x22: /* LWL */
            mipsInLWL(buffer, i_inst);
            break;
        case 0x23: /* LW */
            mipsInLW(buffer, i_inst);
            break;
        case 0x24: /* LBU */
            mipsInLBU(buffer, i_inst);
            break;
        case 0x25: /* LHU */
            mipsInLHU(buffer, i_inst);
            break;
        case 0x26: /* LWR */
            mipsInLWR(buffer, i_inst);
            break;
        case 0x28: /* SB */
            mipsInSB(buffer, i_inst);
            break;
        case 0x29: /* SH */
            mipsInSH(buffer, i_inst);
            break;
        case 0x2A: /* SWL */
            mipsInSWL(buffer, i_inst);
            break;
        case 0x2B: /* SW */
            mipsInSW(buffer, i_inst);
            break;
        case 0x2E: /* SWR */
            mipsInSWR(buffer, i_inst);
            break;
        case 0x2F: /* CACHE */
            mipsInCACHE(buffer, i_inst);
            break;
        case 0x30: /* LL */
            mipsInLL(buffer, i_inst);
            break;
        case 0x31: /* LWC1 */
            mipsInLWC1(buffer, i_inst);
            break;
        case 0x32: /* LWC2 */
            mipsInLWC2(buffer, i_inst);
            break;
        case 0x33: /* PREF */
            mipsInPREF(buffer, i_inst);
            break;
        case 0x34: /* *VCOP */
            decodeVCOP(buffer, i_inst, i_addr);
            break;
        case 0x35: /* LDC1 */
            mipsInLDC1(buffer, i_inst);
            break;
        case 0x36: /* LDC2 */
            mipsInLDC2(buffer, i_inst);
            break;
        case 0x38: /* SC */
            mipsInSC(buffer, i_inst);
            break;
        case 0x39: /* SWC1 */
            mipsInSWC1(buffer, i_inst);
            break;
        case 0x3A: /* SWC2 */
            mipsInSWC2(buffer, i_inst);
            break;
        case 0x3D: /* SDC1 */
            mipsInSDC1(buffer, i_inst);
            break;
        case 0x3E: /* SDC2 */
            mipsInSDC2(buffer, i_inst);
            break;
        default:
            sprintf(buffer, "???");
            break;
    }
}

static void decodeBSHFL(char *buffer, unsigned int i_inst, 
        unsigned int i_addr) {
    int sa = (int)mipsGetOperand3(i_inst);
    switch (sa) {
        case 0x02: /* WSBH */
            mipsInWSBH(buffer, i_inst);
            break;
        case 0x10: /* SEB */
            mipsInSEB(buffer, i_inst);
            break;
        case 0x18: /* SEH */
            mipsInSEH(buffer, i_inst);
            break;
        default:
            sprintf(buffer, "???");
            break;
    }
}

static void decodeCOP0(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    int rs = (int)mipsGetOperand0(i_inst);
    int func = (int)mipsGetFunction(i_inst);
    if (rs > 0x10) {
        switch (func) {
            case 0x01: /* TLBR */
                mipsInTLBR(buffer, i_inst);
                break;
            case 0x02: /* TLBWI */
                mipsInTLBWI(buffer, i_inst);
                break;
            case 0x06: /* TLBWR */
                mipsInTLBWR(buffer, i_inst);
                break;
            case 0x08: /* TLBP */
                mipsInTLBP(buffer, i_inst);
                break;
            case 0x18: /* ERET */
                mipsInERET(buffer, i_inst);
                break;
            case 0x1F: /* DERET */
                mipsInDERET(buffer, i_inst);
                break;
            case 0x20: /* WAIT */
                mipsInWAIT(buffer, i_inst);
                break;
            default:
                sprintf(buffer, "???");
                break;
        }
    } else {
        switch (rs) {
            case 0x00: /* MFC0 */
                mipsInMFC0(buffer, i_inst);
                break;
            case 0x04: /* MTC0 */
                mipsInMTC0(buffer, i_inst);
                break;
            case 0x0A: /* RDPGPR */
                mipsInRDPGPR(buffer, i_inst);
                break;
            case 0x0B: /* *MFMC0 */
                mipsInMFMC0(buffer, i_inst);
                break;
            case 0x0E: /* WRPGPR */
                mipsInWRPGPR(buffer, i_inst);
                break;
            default:
                sprintf(buffer, "???");
                break;
        }
    }
}

static void decodeCOP1(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    int rs = (int)mipsGetOperand0(i_inst);
    switch (rs) {
        case 0x00: /* MFC1 */
            mipsInMFC1(buffer, i_inst);
            break;
        case 0x02: /* CFC1 */
            mipsInCFC1(buffer, i_inst);
            break;
        case 0x03: /* MFHC1 */
            mipsInMFHC1(buffer, i_inst);
            break;
        case 0x04: /* MTC1 */
            mipsInMTC1(buffer, i_inst);
            break;
        case 0x06: /* CTC1 */
            mipsInCTC1(buffer, i_inst);
            break;
        case 0x07: /* MTHC1 */
            mipsInMTHC1(buffer, i_inst);
            break;
        case 0x08: /* *BC1 */
            mipsInBC1(buffer, i_inst, i_addr);
            break;
        case 0x09: /* *BC1ANY2 */
            sprintf(buffer, "*BC1ANY2");
            break;
        case 0x0A: /* *BC1ANY4 */
            sprintf(buffer, "*BC1ANY4");
            break;
        case 0x10: /* *S */
            decodeCOP1S(buffer, i_inst, i_addr);
            break;
        case 0x11: /* *D */
            decodeCOP1D(buffer, i_inst, i_addr);
            break;
        case 0x14: /* *W */
            decodeCOP1W(buffer, i_inst, i_addr);
            break;
        case 0x15: /* *L */
            decodeCOP1L(buffer, i_inst, i_addr);
            break;
        case 0x16: /* *PS */
            decodeCOP1PS(buffer, i_inst, i_addr);
            break;
        default:
            sprintf(buffer, "???");
    }
}

static void decodeCOP1D(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    int func = (int)mipsGetFunction(i_inst);
    switch (func) {
        case 0x00: /* ADD.D */
            mipsInADDfmt(buffer, i_inst);
            break;
        case 0x01: /* SUB.D */
            mipsInSUBfmt(buffer, i_inst);
            break;
        case 0x02: /* MUL.D */
            mipsInMULfmt(buffer, i_inst);
            break;
        case 0x03: /* DIV.D */
            mipsInDIVfmt(buffer, i_inst);
            break;
        case 0x04: /* SQRT.D */
            mipsInSQRTfmt(buffer, i_inst);
            break;
        case 0x05: /* ABS.D */
            mipsInABSfmt(buffer, i_inst);
            break;
        case 0x06: /* MOV.D */
            mipsInMOVfmt(buffer, i_inst);
            break;
        case 0x07: /* NEG.D */
            mipsInNEGfmt(buffer, i_inst);
            break;
        case 0x08: /* ROUND.L.D */
            mipsInROUNDLfmt(buffer, i_inst);
            break;
        case 0x09: /* TRUNC.L.D */
            mipsInTRUNCLfmt(buffer, i_inst);
            break;
        case 0x0A: /* CEIL.L.D */
            mipsInCEILLfmt(buffer, i_inst);
            break;
        case 0x0B: /* FLOOR.L.D */
            mipsInFLOORLfmt(buffer, i_inst);
            break;
        case 0x0C: /* ROUND.W.D */
            mipsInROUNDWfmt(buffer, i_inst);
            break;
        case 0x0D: /* TRUNC.W.D */
            mipsInTRUNCWfmt(buffer, i_inst);
            break;
        case 0x0E: /* CEIL.W.D */
            mipsInCEILWfmt(buffer, i_inst);
            break;
        case 0x0F: /* FLOOR.W.D */
            mipsInFLOORWfmt(buffer, i_inst);
            break;
        case 0x11: /* MOVCF.D */
            mipsInMOVCFfmt(buffer, i_inst);
            break;
        case 0x12: /* MOVZ.D */
            mipsInMOVZfmt(buffer, i_inst);
            break;
        case 0x13: /* MOVN.D */
            mipsInMOVNfmt(buffer, i_inst);
            break;
        case 0x15: /* RECIP.D */
            mipsInRECIPfmt(buffer, i_inst);
            break;
        case 0x16: /* RSQRT.D */
            mipsInRSQRTfmt(buffer, i_inst);
            break;
        case 0x1C: /* RECIP2.D */
            sprintf(buffer, "RECIP2.D");
            break;
        case 0x1D: /* RECIP1.D */
            sprintf(buffer, "RECIP1.D");
            break;
        case 0x1E: /* RSQRT1.D */
            sprintf(buffer, "RSQRT1.D");
            break;
        case 0x1F: /* RSQRT2.D */
            sprintf(buffer, "RSQRT2.D");
            break;
        case 0x21: /* CVT.S.D */
            mipsInCVTSfmt(buffer, i_inst);
            break;
        case 0x24: /* CVT.W.D */
            mipsInCVTWfmt(buffer, i_inst);
            break;
        case 0x25: /* CVT.L.D */
            mipsInCVTDfmt(buffer, i_inst);
            break;
        case 0x30: /* C.F.D */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x31: /* C.UN.D */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x32: /* C.EQ.D */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x33: /* C.UEQ.D */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x34: /* C.OLT.D */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x35: /* C.ULT.D */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x36: /* C.OLE.D */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x37: /* C.ULE.D */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x38: /* C.SF.D */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x39: /* C.NGLE.D */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x3A: /* C.SEQ.D */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x3B: /* C.NGL.D */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x3C: /* C.LT.D */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x3D: /* C.NGE.D */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x3E: /* C.LE.D */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x3F: /* C.NGT.D */
            mipsInCcondfmt(buffer, i_inst);
            break;
        default:
            sprintf(buffer, "???");
    }
}

static void decodeCOP1L(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    int func = (int)mipsGetFunction(i_inst);
    switch (func) {
        case 0x21: /* CVT.S.L */
            mipsInCVTSfmt(buffer, i_inst);
            break;
        case 0x22: /* CVT.D.L */
            mipsInCVTDfmt(buffer, i_inst);
            break;
        case 0x26: /* CVT.PS.PW */
            sprintf(buffer, "CVT.PS.PW");
            break;
        default:
            sprintf(buffer, "???");
    }
}

static void decodeCOP1PS(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    int func = (int)mipsGetFunction(i_inst);
    switch (func) {
        case 0x00: /* ADD.PS */
            mipsInADDfmt(buffer, i_inst);
            break;
        case 0x01: /* SUB.PS */
            mipsInSUBfmt(buffer, i_inst);
            break;
        case 0x02: /* MUL.PS */
            mipsInMULfmt(buffer, i_inst);
            break;
        case 0x05: /* ABS.PS */
            mipsInABSfmt(buffer, i_inst);
            break;
        case 0x06: /* MOV.PS */
            mipsInMOVfmt(buffer, i_inst);
            break;
        case 0x07: /* NEG.PS */
            mipsInNEGfmt(buffer, i_inst);
            break;
        case 0x11: /* MOVCF.PS */
            mipsInMOVCFfmt(buffer, i_inst);
            break;
        case 0x12: /* MOVZ.PS */
            mipsInMOVZfmt(buffer, i_inst);
            break;
        case 0x13: /* MOVN.PS */
            mipsInMOVNfmt(buffer, i_inst);
            break;
        case 0x18: /* ADDR.PS */
            sprintf(buffer, "ADDR.PS");
            break;
        case 0x1A: /* MULR.PS */
            sprintf(buffer, "MULR.PS");
            break;
        case 0x1C: /* RECIP2.PS */
            sprintf(buffer, "RECIP2.PS");
            break;
        case 0x1D: /* RECIP1.PS */
            sprintf(buffer, "RECIP1.PS");
            break;
        case 0x1E: /* RSQRT1.PS */
            sprintf(buffer, "RSQRT1.PS");
            break;
        case 0x1F: /* RSQRT2.PS */
            sprintf(buffer, "RSQRT2.PS");
            break;
        case 0x20: /* CVT.S.PU */
            mipsInCVTSPU(buffer, i_inst);
            break;
        case 0x24: /* CVT.PW.PS */
            sprintf(buffer, "CVT.PW.PS");
            break;
        case 0x28: /* CVT.S.PL */
            mipsInCVTSPL(buffer, i_inst);
            break;
        case 0x2C: /* PLL.PS */
            mipsInPLLPS(buffer, i_inst);
            break;
        case 0x2D: /* PLU.PS */
            mipsInPLUPS(buffer, i_inst);
            break;
        case 0x2E: /* PUL.PS */
            mipsInPULPS(buffer, i_inst);
            break;
        case 0x2F: /* PUU.PS */
            mipsInPUUPS(buffer, i_inst);
            break;
        case 0x30: /* C.F.PS */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x31: /* C.UN.PS */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x32: /* C.EQ.PS */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x33: /* C.UEQ.PS */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x34: /* C.OLT.PS */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x35: /* C.ULT.PS */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x36: /* C.OLE.PS */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x37: /* C.ULE.PS */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x38: /* C.SF.PS */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x39: /* C.NGLE.PS */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x3A: /* C.SEQ.PS */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x3B: /* C.NGL.PS */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x3C: /* C.LT.PS */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x3D: /* C.NGE.PS */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x3E: /* C.LE.PS */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x3F: /* C.NGT.PS */
            mipsInCcondfmt(buffer, i_inst);
            break;
        default:
            sprintf(buffer, "???");
    }
}

static void decodeCOP1S(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    int func = (int)mipsGetFunction(i_inst);
    switch (func) {
        case 0x00: /* ADD.S */
            mipsInADDfmt(buffer, i_inst);
            break;
        case 0x01: /* SUB.S */
            mipsInSUBfmt(buffer, i_inst);
            break;
        case 0x02: /* MUL.S */
            mipsInMULfmt(buffer, i_inst);
            break;
        case 0x03: /* DIV.S */
            mipsInDIVfmt(buffer, i_inst);
            break;
        case 0x04: /* SQRT.S */
            mipsInSQRTfmt(buffer, i_inst);
            break;
        case 0x05: /* ABS.S */
            mipsInABSfmt(buffer, i_inst);
            break;
        case 0x06: /* MOV.S */
            mipsInMOVfmt(buffer, i_inst);
            break;
        case 0x07: /* NEG.S */
            mipsInNEGfmt(buffer, i_inst);
            break;
        case 0x08: /* ROUND.L.S */
            mipsInROUNDLfmt(buffer, i_inst);
            break;
        case 0x09: /* TRUNC.L.S */
            mipsInTRUNCLfmt(buffer, i_inst);
            break;
        case 0x0A: /* CEIL.L.S */
            mipsInCEILLfmt(buffer, i_inst);
            break;
        case 0x0B: /* FLOOR.L.S */
            mipsInFLOORLfmt(buffer, i_inst);
            break;
        case 0x0C: /* ROUND.W.S */
            mipsInROUNDWfmt(buffer, i_inst);
            break;
        case 0x0D: /* TRUNC.W.S */
            mipsInTRUNCWfmt(buffer, i_inst);
            break;
        case 0x0E: /* CEIL.W.S */
            mipsInCEILWfmt(buffer, i_inst);
            break;
        case 0x0F: /* FLOOR.W.S */
            mipsInFLOORWfmt(buffer, i_inst);
            break;
        case 0x11: /* MOVCF.S */
            mipsInMOVCFfmt(buffer, i_inst);
            break;
        case 0x12: /* MOVZ.S */
            mipsInMOVZfmt(buffer, i_inst);
            break;
        case 0x13: /* MOVN.S */
            mipsInMOVNfmt(buffer, i_inst);
            break;
        case 0x15: /* RECIP.S */
            mipsInRECIPfmt(buffer, i_inst);
            break;
        case 0x16: /* RSQRT.S */
            mipsInRSQRTfmt(buffer, i_inst);
            break;
        case 0x1C: /* RECIP2.S */
            sprintf(buffer, "RECIP2.S");
            break;
        case 0x1D: /* RECIP1.S */
            sprintf(buffer, "RECIP1.S");
            break;
        case 0x1E: /* RSQRT1.S */
            sprintf(buffer, "RSQRT1.S");
            break;
        case 0x1F: /* RSQRT2.S */
            sprintf(buffer, "RSQRT2.S");
            break;
        case 0x21: /* CVT.D.S */
            mipsInCVTDfmt(buffer, i_inst);
            break;
        case 0x24: /* CVT.W.S */
            mipsInCVTWfmt(buffer, i_inst);
            break;
        case 0x25: /* CVT.L.S */
            mipsInCVTLfmt(buffer, i_inst);
            break;
        case 0x26: /* CVT.PS.S */
            mipsInCVTPSS(buffer, i_inst);
            break;
        case 0x30: /* C.F.S */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x31: /* C.UN.S */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x32: /* C.EQ.S */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x33: /* C.UEQ.S */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x34: /* C.OLT.S */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x35: /* C.ULT.S */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x36: /* C.OLE.S */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x37: /* C.ULE.S */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x38: /* C.SF.S */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x39: /* C.NGLE.S */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x3A: /* C.SEQ.S */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x3B: /* C.NGL.S */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x3C: /* C.LT.S */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x3D: /* C.NGE.S */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x3E: /* C.LE.S */
            mipsInCcondfmt(buffer, i_inst);
            break;
        case 0x3F: /* C.NGT.S */
            mipsInCcondfmt(buffer, i_inst);
            break;
        default:
            sprintf(buffer, "???");
    }
}

static void decodeCOP1W(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    int func = (int)mipsGetFunction(i_inst);
    switch (func) {
        case 0x20: /* CVT.S.W */
            mipsInCVTSfmt(buffer, i_inst);
            break;
        case 0x21: /* CVT.D.W */
            mipsInCVTDfmt(buffer, i_inst);
            break;
        case 0x26: /* CVT.PS.PW */
            sprintf(buffer, "CVT.PS.PW");
            break;
        default:
            sprintf(buffer, "???");
    }
}

static void decodeCOP2(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    int rs = (int)mipsGetOperand0(i_inst);
    switch (rs) {
        case 0x00: /* MFC2 */
            mipsInMFC2(buffer, i_inst);
            break;
        case 0x02: /* CFC2 */
            mipsInCFC2(buffer, i_inst);
            break;
        case 0x03: /* MFHC2 */
            mipsInMFHC2(buffer, i_inst);
            break;
        case 0x04: /* MTC2 */
            mipsInMTC2(buffer, i_inst);
            break;
        case 0x06: /* CTC2 */
            mipsInCTC2(buffer, i_inst);
            break;
        case 0x07: /* MTHC2 */
            mipsInMTHC2(buffer, i_inst);
            break;
        case 0x08: /* BC2 */
            mipsInBC2(buffer, i_inst, i_addr);
            break;
        default:
            sprintf(buffer, "???");
    }
}

static void decodeCOP1X(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    int func = (int)mipsGetFunction(i_inst);
    switch (func) {
        case 0x00: /* LWXC1 */
            mipsInLWXC1(buffer, i_inst);
            break;
        case 0x01: /* LDXC1 */
            mipsInLDXC1(buffer, i_inst);
            break;
        case 0x05: /* LUXC1 */
            mipsInLUXC1(buffer, i_inst);
            break;
        case 0x08: /* SWXC1 */
            mipsInSWXC1(buffer, i_inst);
            break;
        case 0x09: /* SDXC1 */
            mipsInSDXC1(buffer, i_inst);
            break;
        case 0x0D: /* SUXC1 */
            mipsInSUXC1(buffer, i_inst);
            break;
        case 0x0F: /* PREFX */
            mipsInPREFX(buffer, i_inst);
            break;
        case 0x1E: /* ALNV.PS */
            mipsInALNVPS(buffer, i_inst);
            break;
        case 0x20: /* MADD.S */
            mipsInMADDfmt(buffer, i_inst);
            break;
        case 0x21: /* MADD.D */
            mipsInMADDfmt(buffer, i_inst);
            break;
        case 0x26: /* MADD.PS */
            mipsInMADDfmt(buffer, i_inst);
            break;
        case 0x28: /* MSUB.S */
            mipsInMSUBfmt(buffer, i_inst);
            break;
        case 0x29: /* MSUB.D */
            mipsInMSUBfmt(buffer, i_inst);
            break;
        case 0x2E: /* MSUB.PS */
            mipsInMSUBfmt(buffer, i_inst);
            break;
        case 0x30: /* NMADD.S */
            mipsInNMADDfmt(buffer, i_inst);
            break;
        case 0x31: /* NMADD.D */
            mipsInNMADDfmt(buffer, i_inst);
            break;
        case 0x36: /* NMADD.PS */
            mipsInNMADDfmt(buffer, i_inst);
            break;
        case 0x38: /* NMSUB.S */
            mipsInNMSUBfmt(buffer, i_inst);
            break;
        case 0x39: /* NMSUB.D */
            mipsInNMSUBfmt(buffer, i_inst);
            break;
        case 0x3E: /* NMSUB.PS */
            mipsInNMSUBfmt(buffer, i_inst);
            break;
        default:
            sprintf(buffer, "???");
    }
}

static void decodeREGIMM(char *buffer, unsigned int i_inst,
        unsigned int i_addr) {
    int rt = (int)mipsGetOperand1(i_inst);
    switch (rt) {
        case 0x00: /* BLTZ */
            mipsInBLTZ(buffer, i_inst, i_addr);
            break;
        case 0x01: /* BGEZ */
            mipsInBGEZ(buffer, i_inst, i_addr);
            break;
        case 0x02: /* BLTZL */
            mipsInBLTZL(buffer, i_inst, i_addr);
            break;
        case 0x03: /* BGEZL */
            mipsInBGEZL(buffer, i_inst, i_addr);
            break;
        case 0x08: /* TGEI */
            mipsInTGEI(buffer, i_inst);
            break;
        case 0x09: /* TGEIU */
            mipsInTGEIU(buffer, i_inst);
            break;
        case 0x0A: /* TLTI */
            mipsInTLTI(buffer, i_inst);
            break;
        case 0x0B: /* TLTIU */
            mipsInTLTIU(buffer, i_inst);
            break;
        case 0x0C: /* TEQI */
            mipsInTEQI(buffer, i_inst);
            break;
        case 0x0E: /* TNEI */
            mipsInTNEI(buffer, i_inst);
            break;
        case 0x10: /* BLTZAL */
            mipsInBLTZAL(buffer, i_inst, i_addr);
            break;
        case 0x11: /* BGEZAL */
            mipsInBGEZAL(buffer, i_inst, i_addr);
            break;
        case 0x12: /* BLTZALL */
            mipsInBLTZALL(buffer, i_inst, i_addr);
            break;
        case 0x13: /* BGEZALL */
            mipsInBGEZALL(buffer, i_inst, i_addr);
            break;
        case 0x1F: /* SYNCI */
            mipsInSYNCI(buffer, i_inst);
            break;
        default:
            sprintf(buffer, "???");
            break;
    }
}

static void decodeSPECIAL(char *buffer, unsigned int i_inst,
        unsigned int i_addr) {
    int func = (int)mipsGetFunction(i_inst);
    switch (func) {
        case 0x00: /* SLL */
            mipsInSLL(buffer, i_inst);
            break;
        case 0x01: /* *MOVC1 */
            mipsInMOVC1(buffer, i_inst);
            break;
        case 0x02: /* *SRL */
            mipsInSRL(buffer, i_inst);
            break;
        case 0x03: /* SRA */
            mipsInSRA(buffer, i_inst);
            break;
        case 0x04: /* SLLV */
            mipsInSLLV(buffer, i_inst);
            break;
        case 0x06: /* *SRLV */
            mipsInSRLV(buffer, i_inst);
            break;
        case 0x07: /* SRAV */
            mipsInSRAV(buffer, i_inst);
            break;
        case 0x08: /* JR */
            mipsInJR(buffer, i_inst);
            break;
        case 0x09: /* JALR */
            mipsInJALR(buffer, i_inst);
            break;
        case 0x0A: /* MOVZ */
            mipsInMOVZ(buffer, i_inst);
            break;
        case 0x0B: /* MOVN */
            mipsInMOVN(buffer, i_inst);
            break;
        case 0x0C: /* SYSCALL */
            mipsInSYSCALL(buffer, i_inst);
            break;
        case 0x0D: /* BREAK */
            mipsInBREAK(buffer, i_inst);
            break;
        case 0x0F: /* SYNC */
            mipsInSYNC(buffer, i_inst);
            break;
        case 0x10: /* MFHI */
            mipsInMFHI(buffer, i_inst);
            break;
        case 0x11: /* MTHI */
            mipsInMTHI(buffer, i_inst);
            break;
        case 0x12: /* MFLO */
            mipsInMFLO(buffer, i_inst);
            break;
        case 0x13: /* MTLO */
            mipsInMTLO(buffer, i_inst);
            break;
        case 0x18: /* MULT */
            mipsInMULT(buffer, i_inst);
            break;
        case 0x19: /* MULTU */
            mipsInMULTU(buffer, i_inst);
            break;
        case 0x1A: /* DIV */
            mipsInDIV(buffer, i_inst);
            break;
        case 0x1B: /* DIVU */
            mipsInDIVU(buffer, i_inst);
            break;
        case 0x20: /* ADD */
            mipsInADD(buffer, i_inst);
            break;
        case 0x21: /* ADDU */
            mipsInADDU(buffer, i_inst);
            break;
        case 0x22: /* SUB */
            mipsInSUB(buffer, i_inst);
            break;
        case 0x23: /* SUBU */
            mipsInSUBU(buffer, i_inst);
            break;
        case 0x24: /* AND */
            mipsInAND(buffer, i_inst);
            break;
        case 0x25: /* OR */
            mipsInOR(buffer, i_inst);
            break;
        case 0x26: /* XOR */
            mipsInXOR(buffer, i_inst);
            break;
        case 0x27: /* NOR */
            mipsInNOR(buffer, i_inst);
            break;
        case 0x2A: /* SLT */
            mipsInSLT(buffer, i_inst);
            break;
        case 0x2B: /* SLTU */
            mipsInSLTU(buffer, i_inst);
            break;
        case 0x30: /* TGE */
            mipsInTGE(buffer, i_inst);
            break;
        case 0x31: /* TGEU */
            mipsInTGEU(buffer, i_inst);
            break;
        case 0x32: /* TLT */
            mipsInTLT(buffer, i_inst);
            break;
        case 0x33: /* TLTU */
            mipsInTLTU(buffer, i_inst);
            break;
        case 0x34: /* TEQ */
            mipsInTEQ(buffer, i_inst);
            break;
        case 0x36: /* TNE */
            mipsInTNE(buffer, i_inst);
            break;
        default:
            sprintf(buffer, "???");
            break;
    }
}

static void decodeSPECIAL2(char *buffer, unsigned int i_inst,
        unsigned int i_addr) {
    int func = (int)mipsGetFunction(i_inst);
    switch (func) {
        case 0x00: /* MADD */
            mipsInMADD(buffer, i_inst);
            break;
        case 0x01: /* MADDU */
            mipsInMADDU(buffer, i_inst);
            break;
        case 0x02: /* MUL */
            mipsInMUL(buffer, i_inst);
            break;
        case 0x04: /* MSUB */
            mipsInMSUB(buffer, i_inst);
            break;
        case 0x05: /* MSUBU */
            mipsInMSUBU(buffer, i_inst);
            break;
        case 0x20: /* CLZ */
            mipsInCLZ(buffer, i_inst);
            break;
        case 0x21: /* CLO */
            mipsInCLO(buffer, i_inst);
            break;
        case 0x24: /* MFIC */
            mipsInMFIC(buffer, i_inst);
            break;
        case 0x26: /* MTIC */
            mipsInMTIC(buffer, i_inst);
            break;
        case 0x3F: /* SDBBP */
            mipsInSDBBP(buffer, i_inst);
            break;
        default:
            sprintf(buffer, "???");
            break;
    }
}

static void decodeSPECIAL3(char *buffer, unsigned int i_inst,
        unsigned int i_addr) {
    int func = (int)mipsGetFunction(i_inst);
    switch (func) {
        case 0x00: /* EXT */
            mipsInEXT(buffer, i_inst);
            break;
        case 0x04: /* INS */
            mipsInINS(buffer, i_inst);
            break;
        case 0x20: /* *BSHFL */
            decodeBSHFL(buffer, i_inst, i_addr);
            break;
        case 0x3B: /* RDHWR */
            mipsInRDHWR(buffer, i_inst);
            break;
        default:
            sprintf(buffer, "???");
            break;
    }
}

static void decodeVCOP(char* buffer, unsigned int i_inst, unsigned int i_addr) {
    int sel = mipsGetVoperand0(i_inst);
    int rt = mipsGetVoperand1(i_inst);

    if (sel == 4) {
        /* VF2IN.fmt */
        mipsInVF2INfmt(buffer, i_inst);
        return;
    }
    if (sel == 5) {
        /* VI2F.fmt */
        mipsInVI2Ffmt(buffer, i_inst);
        return;
    }
    if (rt & 0x60 == 0x60) {
        /* VCST.fmt */
        mipsInVCSTfmt(buffer, i_inst);
        return;
    }
    switch (rt) {
        case 0x01: /* VABS.fmt */
            mipsInVABSfmt(buffer, i_inst);
            break;
        case 0x02: /* VNEG.fmt */
            mipsInVNEGfmt(buffer, i_inst);
            break;
        case 0x03: /* VIDT.fmt */
            mipsInVIDTfmt(buffer, i_inst);
            break;
        case 0x06: /* VZERO.fmt */
            mipsInVZEROfmt(buffer, i_inst);
            break;
        case 0x07: /* VONE.fmt */
            mipsInVONEfmt(buffer, i_inst);
            break;
        case 0x10: /* VRCP.fmt */
            mipsInVRCPfmt(buffer, i_inst);
            break;
        case 0x11: /* VRSQ.fmt */
            mipsInVRSQfmt(buffer, i_inst);
            break;
        case 0x12: /* VSIN.fmt */
            mipsInVSINfmt(buffer, i_inst);
            break;
        case 0x13: /* VCOS.fmt */
            mipsInVCOSfmt(buffer, i_inst);
            break;
        case 0x14: /* VEXP2.fmt */
            mipsInVEXP2fmt(buffer, i_inst);
            break;
        case 0x15: /* VLOG2.fmt */
            mipsInVLOG2fmt(buffer, i_inst);
            break;
        case 0x16: /* VSQRT.fmt */
            mipsInVSQRTfmt(buffer, i_inst);
            break;
        case 0x17: /* VASIN.fmt */
            mipsInVASINfmt(buffer, i_inst);
            break;
        case 0x18: /* VNRCP.fmt */
            mipsInVRCPfmt(buffer, i_inst);
            break;
        case 0x1A: /* VNSIN.fmt */
            mipsInVNSINfmt(buffer, i_inst);
            break;
        case 0x1C: /* VREXP2.fmt */
            mipsInVREXP2fmt(buffer, i_inst);
            break;
        case 0x3C: /* VI2UCQ */
            mipsInVI2UCQ(buffer, i_inst);
            break;
        case 0x3F: /* VI2S.fmt */
            mipsInVI2Sfmt(buffer, i_inst);
            break;
        case 0x4A: /* VSGN.fmt */
            mipsInVSGNfmt(buffer, i_inst);
            break;
        default:
            sprintf(buffer, "???");
    }
}

// vi:cin:et:ts=4 sw=4

