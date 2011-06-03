#include "mips.h"

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

static void decodeBSHFL(char *, unsigned int, unsigned int);
static void decodeCOP0(char *, unsigned int, unsigned int);
static void decodeCOP1(char *, unsigned int, unsigned int);
static void decodeCOP1D(char *, unsigned int, unsigned int);
static void decodeCOP1L(char *, unsigned int, unsigned int);
static void decodeCOP1PS(char *, unsigned int, unsigned int);
static void decodeCOP1S(char *, unsigned int, unsigned int);
static void decodeCOP1W(char *, unsigned int, unsigned int);
static void decodeCOP1X(char *, unsigned int, unsigned int);
static void decodeCOP2(char *, unsigned int, unsigned int);
static void decodeREGIMM(char *, unsigned int, unsigned int);
static void decodeSPECIAL(char *, unsigned int, unsigned int);
static void decodeSPECIAL2(char *, unsigned int, unsigned int);
static void decodeSPECIAL3(char *, unsigned int, unsigned int);
static void decodeVCOP(char* buffer, unsigned int i_inst, unsigned int i_addr);
static char* mipsGetFormat(int fmt);
static short mipsGetImmediate(unsigned int i_inst);
static int mipsGetInstrIndex(unsigned int i_inst);
static unsigned char mipsGetOperand0(unsigned int i_inst);
static unsigned char mipsGetOperand1(unsigned int i_inst);
static unsigned char mipsGetOperand2(unsigned int i_inst);
static unsigned char mipsGetOperand3(unsigned int i_inst);
static void mipsType0(char *, const char *);
static void mipsTypeFdFs(char *, const char *, int, int);
static void mipsTypeRtOffsetBase(char *, const char *, int, short, int);
static void mipsTypeVrd(char* buffer, const char* mnem, int iRd);
static void mipsTypeVrdA(char* buffer, const char* mnem, int iRd, int iA);
static void mipsTypeVrdVrs(char* buffer, const char* mnem, int iRd, int iRs);
static void mipsTypeVrdVrsScale(char* buffer, const char* mnem,
        int iRd, int iRs, int scale);
static void mipsTypeVrdVrsVrt(char* buffer, const char* mnem,
        int iRd, int iRs, int iRt);

static void in_brs_offset(char* buffer, const char* mnem, unsigned int inst,
        unsigned int address);
static void in_brs_rt_offset(char* buffer, const char* mnem, unsigned int inst,
        unsigned int address);
static void in_fd_fr_fs_ft(char* buffer, const char* mnem, unsigned int inst);
static void in_fd_fs(char* buffer, const char* mnem, unsigned int inst);
static void in_fd_fs_ft(char* buffer, const char* mnem, unsigned int inst);
static void in_fd_fs_ft_rs(char* buffer, const char* mnem, unsigned int inst);
static void in_fd_index_base(char* buffer, const char* mnem, unsigned int inst);
static void in_fmt_fd_fs(char* buffer, const char* mnem, unsigned int inst);
static void in_fmt_fd_fs_ft(char* buffer, const char* mnem, unsigned int inst);
static void in_fmt_fd_fs_rt(char* buffer, const char* mnem, unsigned int inst);
static void in_fs_index_base(char* buffer, const char* mnem, unsigned int inst);
static void in_ft_offset_base(char* buffer, const char* mnem, 
        unsigned int inst);
static void in_rd_rs(char* buffer, const char* mnem, unsigned int i_inst);
static void in_rd_rs_rt(char* buffer, const char* mnem, unsigned int inst);
static void in_rd_rt(char* buffer, const char* mnem, unsigned int inst);
static void in_rd_rt_rs(char* buffer, const char* mnem, unsigned int inst);
static void in_rd_rt_sa(char* buffer, const char* mnem, unsigned int inst);
static void in_rs(char* buffer, const char* mnem, unsigned int inst);
static void in_rs_imm(char* buffer, const char* mnem, unsigned int inst);
static void in_rs_rt(char* buffer, const char* mnem, unsigned int inst);
static void in_rs_rt_imm(char* buffer, const char* mnem, unsigned int inst);
static void in_rt_fs(char* buffer, const char* mnem, unsigned int inst);
static void in_rt_imm(char* buffer, const char* mnem, unsigned int inst);
static void in_rt_offset_base(char* buffer, const char* mnem, 
        unsigned int inst);
static void in_rt_rd(char* buffer, const char* mnem, unsigned int inst);
static void in_rt_rs_imm(char* buffer, const char* mnem, unsigned int inst);

static void re_cc_fr_fr(char *, const char *, int, int, int);
static void re_cc_target(char *, const char *, int, unsigned int);
static void re_fr_fr(char *, const char *, int, int);
static void re_fr_fr_cc(char *, const char *, int, int, int);
static void re_fr_fr_fr(char *, const char *, int, int, int);
static void re_fr_fr_fr_fr(char *buffer, const char *mnem, int iFd, int iFr, int iFs, int iFt);
static void re_fr_fr_fr_gr(char *, const char *, int, int, int, int);
static void re_fr_fr_gr(char *, const char *, int, int, int);
static void re_fr_index_base(char *buffer, const char *mnem, int iFs, int iIndex, int iBase);
static void re_fr_offset_base(char *, const char *, int, int, short);
static void re_gr(char *, const char *, int);
static void re_gr_fr(char *, const char *, int, int);
static void re_gr_gr(char *, const char *, int, int);
static void re_gr_gr_cc(char *, const char *, int, int, int);
static void re_gr_gr_gr(char *, const char *, int, int, int);
static void re_gr_gr_imm(char *, const char *, int, int, short);
static void re_gr_imm(char *, const char *, int, short);
static void re_gr_gr_pos_size(char *, const char *, int, int, int, int);
static void re_gr_gr_sa(char *, const char *, int, int, int);
static void re_gr_gr_sel(char* buffer, const char* mnem, 
        int i_rt, int i_rd, int i_sel);
static void re_gr_gr_target(char *, const char *, int, int, unsigned int);
static void re_gr_offset_base(char *, const char *, int, int, short);
static void re_gr_target(char *, const char *, int, unsigned int);
static void re_offset_base(char *, const char *, short, int);
static void re_op_offset_base(char *, const char *, int, int, short);
static void re_target(char *, const char *, unsigned int);

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

static void in_brs_offset(char* buffer, const char* mnem, unsigned int inst,
        unsigned int address) {
    int rs = (int)mipsGetOperand0(inst);
    short offset = mipsGetImmediate(inst);
    unsigned int dest = (address + 4 + (offset << 2)) & 0x0FFFFFFF;
    re_gr_target(buffer, mnem, rs, dest);
}

static void in_brs_rt_offset(char* buffer, const char* mnem, unsigned int inst,
        unsigned int address)
{
    int rs = (int)mipsGetOperand0(inst);
    int rt = (int)mipsGetOperand1(inst);
    short offset = mipsGetImmediate(inst);
    unsigned int dest = (address & 0x0FFFFFFF) + 4 + (offset << 2);
    re_gr_gr_target(buffer, mnem, rs, rt, dest);
}

static void in_fd_fr_fs_ft(char* buffer, const char* mnem, unsigned int inst) {
    int fd = (int)mipsGetOperand0(inst);
    int fr = (int)mipsGetOperand0(inst);
    int fs = (int)mipsGetOperand0(inst);
    int ft = (int)mipsGetOperand0(inst);
    int fmt = (inst & 0x7);
    char fmnem[10];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(fmnem, "%s.%s", mnem, sfmt);
    re_fr_fr_fr_fr(buffer, fmnem, fd, fr, fs, ft);
}

static void in_fd_fs(char* buffer, const char* mnem, unsigned int inst) {
    int fs = (int)mipsGetOperand2(inst);
    int fd = (int)mipsGetOperand3(inst);
    re_fr_fr(buffer, mnem, fd, fs);
}

static void in_fd_fs_ft(char* buffer, const char* mnem, unsigned int inst) {
    int ft = (int)mipsGetOperand0(inst);
    int fs = (int)mipsGetOperand1(inst);
    int fd = (int)mipsGetOperand2(inst);
    re_fr_fr_fr(buffer, mnem, fd, fs, ft);
}

static void in_fd_fs_ft_rs(char* buffer, const char* mnem, unsigned int inst) {
    int rs = (int)mipsGetOperand0(inst);
    int ft = (int)mipsGetOperand1(inst);
    int fs = (int)mipsGetOperand2(inst);
    int fd = (int)mipsGetOperand3(inst);
    re_fr_fr_fr_gr(buffer, mnem, fd, fs, ft, rs);
}

static void in_fd_index_base(char* buffer, const char* mnem, unsigned int inst) 
{
    int base = (int)mipsGetOperand0(inst);
    int index = (int)mipsGetOperand1(inst);
    int fd = (int)mipsGetOperand3(inst);
    re_fr_index_base(buffer, mnem, fd, index, base);
}

static void in_fmt_fd_fs(char* buffer, const char* mnem, unsigned int inst) {
    int fmt = (int)mipsGetOperand0(inst);
    int fs = (int)mipsGetOperand2(inst);
    int fd = (int)mipsGetOperand3(inst);
    char* sfmt = mipsGetFormat(fmt);
    char fmnem[9];
    sprintf(fmnem, "%s.%s", mnem, sfmt);
    re_fr_fr(buffer, fmnem, fd, fs);
}

static void in_fmt_fd_fs_ft(char* buffer, const char* mnem, unsigned int inst) {
    int fmt = (int)mipsGetOperand0(inst);
    int ft = (int)mipsGetOperand1(inst);
    int fs = (int)mipsGetOperand2(inst);
    int fd = (int)mipsGetOperand3(inst);
    char fmnem[9];
    char *sfmt = mipsGetFormat(fmt);
    sprintf(fmnem, "%s.%s", mnem, sfmt);
    re_fr_fr_fr(buffer, fmnem, fd, fs, ft);
}

static void in_fmt_fd_fs_rt(char* buffer, const char* mnem, unsigned int inst) {
    int fmt = (int)mipsGetOperand0(inst);
    int rt = (int)mipsGetOperand1(inst);
    int fs = (int)mipsGetOperand2(inst);
    int fd = (int)mipsGetOperand3(inst);
    char fmnem[10];
    char* sfmt = mipsGetFormat(fmt);
    sprintf(fmnem, "%s.%s", mnem, sfmt);
    re_fr_fr_gr(buffer, fmnem, fd, fs, rt);
}

static void in_fs_index_base(char* buffer, const char* mnem, unsigned int inst) 
{
    int base = (int)mipsGetOperand0(inst);
    int index = (int)mipsGetOperand1(inst);
    int fs = (int)mipsGetOperand2(inst);
    re_fr_index_base(buffer, mnem, fs, index, base);
}

static void in_ft_offset_base(char* buffer, const char* mnem, unsigned int inst)
{
    int base = (int)mipsGetOperand0(inst);
    int ft = (int)mipsGetOperand1(inst);
    short offset = mipsGetImmediate(inst);
    re_fr_offset_base(buffer, mnem, base, ft, offset);
}

static void in_rd_rs(char* buffer, const char* mnem, unsigned int i_inst) {
    int rs = (int)mipsGetOperand0(i_inst);
    int rd = (int)mipsGetOperand2(i_inst);
    re_gr_gr(buffer, mnem, rd, rs);
}

static void in_rd_rs_rt(char* buffer, const char* mnem, unsigned int inst) {
    int rs = (int)mipsGetOperand0(inst);
    int rt = (int)mipsGetOperand1(inst);
    int rd = (int)mipsGetOperand2(inst);
    re_gr_gr_gr(buffer, mnem, rd, rs, rt);
}

static void in_rd_rt(char* buffer, const char* mnem, unsigned int inst) {
    int rt = (int)mipsGetOperand1(inst);
    int rd = (int)mipsGetOperand2(inst);
    re_gr_gr(buffer, mnem, rd, rt);
}

static void in_rd_rt_rs(char* buffer, const char* mnem, unsigned int inst) {
    int rs = (int)mipsGetOperand0(inst);
    int rt = (int)mipsGetOperand1(inst);
    int rd = (int)mipsGetOperand2(inst);
    re_gr_gr_gr(buffer, mnem, rd, rs, rt);
}

static void in_rd_rt_sa(char* buffer, const char* mnem, unsigned int inst) {
    int rt = (int)mipsGetOperand1(inst);
    int rd = (int)mipsGetOperand2(inst);
    int sa = (int)mipsGetOperand3(inst);
    re_gr_gr_sa(buffer, mnem, rd, rt, sa);
}

static void in_rs(char* buffer, const char* mnem, unsigned int inst) {
    int rs = (int)mipsGetOperand0(inst);
    re_gr(buffer, mnem, rs);
}

static void in_rs_imm(char* buffer, const char* mnem, unsigned int inst) {
    int rs = (int)mipsGetOperand0(inst);
    short imm = mipsGetImmediate(inst);
    re_gr_imm(buffer, mnem, rs, imm);
}

static void in_rs_rt(char* buffer, const char* mnem, unsigned int inst) {
    int rs = (int)mipsGetOperand0(inst);
    int rt = (int)mipsGetOperand1(inst);
    re_gr_gr(buffer, mnem, rs, rt);
}

static void in_rs_rt_imm(char* buffer, const char* mnem, unsigned int inst) {
    int rs = (int)mipsGetOperand0(inst);
    int rt = (int)mipsGetOperand1(inst);
    short imm = mipsGetImmediate(inst);
    re_gr_gr_imm(buffer, mnem, rs, rt, imm);
}

static void in_rt_fs(char* buffer, const char* mnem, unsigned int inst) {
    int rt = (int)mipsGetOperand1(inst);
    int fs = (int)mipsGetOperand2(inst);
    re_gr_fr(buffer, mnem, rt, fs);
}

static void in_rt_imm(char* buffer, const char* mnem, unsigned int inst) {
    int rt = (int)mipsGetOperand1(inst);
    short imm = mipsGetImmediate(inst);
    re_gr_imm(buffer, mnem, rt, imm);
}

static void in_rt_offset_base(char* buffer, const char* mnem, unsigned int inst)
{
    int base = (int)mipsGetOperand0(inst);
    int rt = (int)mipsGetOperand1(inst);
    short offset = mipsGetImmediate(inst);
    re_gr_offset_base(buffer, mnem, base, rt, offset);
}

static void in_rt_rd(char* buffer, const char* mnem, unsigned int inst) {
    int rt = (int)mipsGetOperand1(inst);
    int rd = (int)mipsGetOperand2(inst);
    re_gr_gr(buffer, mnem, rt, rd);
}

static void in_rt_rs_imm(char* buffer, const char* mnem, unsigned int inst) {
    int rs = (int)mipsGetOperand0(inst);
    int rt = (int)mipsGetOperand1(inst);
    unsigned short imm = mipsGetUnsignedImmediate(inst);
    re_gr_gr_imm(buffer, mnem, rs, rt, imm);
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
        re_target(buffer, mnem, dest);
    } else {
        re_cc_target(buffer, mnem, cc, dest);
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
        re_target(buffer, mnem, dest);
    } else {
        re_cc_target(buffer, mnem, cc, dest);
    }
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
    re_op_offset_base(buffer, mnem, i_base, i_op, offset);
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
        re_cc_fr_fr(buffer, mnem, cc, fs, ft);
    }
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
    re_gr_gr_pos_size(buffer, "ext", rt, rs, pos, size);
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
    re_gr_gr_pos_size(buffer, "ins", rt, rs, pos, size);
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
    re_target(buffer, mnem, dest);
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
    re_target(buffer, mnem, dest);
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
        re_gr(buffer, mnem, rs);
    } else {
        re_gr_gr(buffer, mnem, rd, rs);
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
    re_gr(buffer, mnem, rs);
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
        re_gr_gr(buffer, mnem, rt, rd);
    } else {
        //mipsType7(buffer, mnem, rt, rd, sel);
        re_gr_gr_sel(buffer, mnem, rt, rd, sel);
    }
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
        re_gr_gr(buffer, mnem, rt, rd);
    } else {
        re_gr_gr_sa(buffer, mnem, rt, rd, sel);
    }
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
        re_gr(buffer, "di", rt);
    } else {
        re_gr(buffer, "ei", rt);
    }
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
        re_gr_gr_cc(buffer, "movt", rd, rs, cc);
    } else {
        re_gr_gr_cc(buffer, "movf", rd, rs, cc);
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
    re_fr_fr_cc(buffer, mnem, fd, fs, cc);
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
        re_gr_gr(buffer, mnem, rt, rd);
    } else {
        //mipsType7(buffer, mnem, rt, rd, sel);
        re_gr_gr_sel(buffer, mnem, rt, rd, sel);
    }
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
    re_op_offset_base(buffer, mnem, base, hint, offset);
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
    re_op_offset_base(buffer, mnem, base, hint, index);
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
    re_target(buffer, mnem, code);
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
    re_gr_gr_sa(buffer, "sll", rd, rt, sa);
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
        re_gr_gr_sa(buffer, "rotr", i_rd, i_rt, i_sa);
    } else {
        re_gr_gr_sa(buffer, "srl", i_rd, i_rt, i_sa);
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
        re_gr_gr_gr(buffer, "rotrv", rd, rs, rt);
    } else {
        re_gr_gr_gr(buffer, "srlv", rd, rs, rt);
    }
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
    re_offset_base(buffer, mnem, offset, base);
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

/* mipsType0
 *  Render a Type 0 instruction.
 *  [MNEM]
 */
static void mipsType0(char *buffer, const char *mnem) {
    sprintf(buffer, "%s", mnem);
}

/** Render a Type 'CC, FPR, FPR' instruction.
 * [MNEM] cc, fs, ft
 * 
 * @param buffer string acting as the destination.
 * @param mnem string containing the mnemonic.
 * @param cc int containing the condition code.
 * @param i_fs int containing the source FP register.
 * @param i_ft int containing the target FP register.
 */
static void re_cc_fr_fr(char *buffer, const char *mnem, int cc, int i_fs, 
        int i_ft) {
    const char *fs = mipsFpRegNames[i_fs];
    const char *ft = mipsFpRegNames[i_ft];
    sprintf(buffer, "%s %d, $%s, $%s", mnem, cc, fs, ft);
}

/* mipsTypeCcDest
 *  Render a Type 'cc, dest' instruction.
 *  [MNEM] cc, dest
 */
static void re_cc_target(char *buffer, const char *mnem, int cc, 
        unsigned int dest) {
    sprintf(buffer, "%s %d, 0x%08X", mnem, cc, dest);
}

/* mipsType13
 *  Render a Type 13 instruction.
 *  [MNEM] fd, fs
 */
static void re_fr_fr(char *buffer, const char *mnem, int i_fd, int i_fs) {
    const char *fd = mipsFpRegNames[i_fd];
    const char *fs = mipsFpRegNames[i_fs];
    sprintf(buffer, "%s $%s, $%s", mnem, fd, fs);
}

/* mipsTypeFdFsCc
 *  Render a Type 'fd, fs, cc' instruction.
 *  [MNEM] fd, fs, cc
 */
static void re_fr_fr_cc(char *buffer, const char *mnem, int i_fd, int i_fs, int i_cc) {
    const char *fd = mipsFpRegNames[i_fd];
    const char *fs = mipsFpRegNames[i_fs];
    sprintf(buffer, "%s $%s, $%s, %d", mnem, fd, fs, i_cc);
}

/* mipsTypeFdFsFt
 *  Render a Type 'fd, fs, ft' instruction.
 *  [MNEM] fd, fs, ft
 */
static void re_fr_fr_fr(char *buffer, const char *mnem, int iFd, int iFs, int iFt) {
    const char *fd = mipsFpRegNames[iFd];
    const char *fs = mipsFpRegNames[iFs];
    const char *ft = mipsFpRegNames[iFt];
    sprintf(buffer, "%s $%s, $%s, $%s", mnem, fd, fs, ft);
}

/* mipsTypeFdFrFsFt
 *  Render a Type 'fd, fr, fs, ft' instruction.
 *  [MNEM] fd, fr, fs, ft
 */
static void re_fr_fr_fr_fr(char *buffer, const char *mnem, int iFd, int iFr, 
        int iFs, int iFt) {
    const char *fd = mipsFpRegNames[iFd];
    const char *fr = mipsFpRegNames[iFr];
    const char *fs = mipsFpRegNames[iFs];
    const char *ft = mipsFpRegNames[iFt];
    sprintf(buffer, "%s $%s, $%s, $%s, $%s", mnem, fd, fr, fs, ft);
}

/* mipsTypeFdFsFtRs
 *  Render a Type 'fd, fs, ft, rs' instruction.
 *  [MNEM] fd, fs, ft, rs
 */
static void re_fr_fr_fr_gr(char *buffer, const char *mnem, int iFd, int iFs, int iFt, int iRs) {
    const char *fd = mipsFpRegNames[iFd];
    const char *fs = mipsFpRegNames[iFs];
    const char *ft = mipsFpRegNames[iFt];
    const char *rs = mipsRegisterArray[iRs];
    sprintf(buffer, "%s $%s, $%s, $%s, $%s", mnem, fd, fs, ft, rs);
}

/* mipsTypeFdFsRt
 *  Render a Type 'fd, fs, rt' instruction.
 *  [MNEM] fd, fs, rt
 */
static void re_fr_fr_gr(char *buffer, const char *mnem, int iFd, int iFs, int iRt) {
    const char *fd = mipsFpRegNames[iFd];
    const char *fs = mipsFpRegNames[iFs];
    const char *rt = mipsRegisterArray[iRt];
    sprintf(buffer, "%s $%s, $%s, $%s", mnem, fd, fs, rt);
}

/* mipsTypeFsIndexBase
 *  Render a Type 'fs, index(base)' instruction.
 *  [MNEM] fs, index(base)
 */
static void re_fr_index_base(char *buffer, const char *mnem, int iFs, int iIndex, int iBase) {
    const char *fs = mipsFpRegNames[iFs];
    const char *index = mipsRegisterArray[iIndex];
    const char *base = mipsRegisterArray[iBase];
    sprintf(buffer, "%s $%s, $%s($%s)", mnem, fs, index, base);
}

/** Render a FPR, offset(GPR) type instruction.
 * [MNEM] FPR, offset(GPR)
 * 
 * @param buffer string to act as the destination.
 * @param mnem string containing the mnemonic.
 * @param base int containing the base register.
 * @param ft int containing the target FP register.
 * @param offset short containing the offset.
 */
static void re_fr_offset_base(char *buffer, const char *mnem, int base, 
        int ft, short offset) {
    const char *sBase = mipsRegisterArray[base];
    const char *sTgt = mipsFpRegNames[ft];
    sprintf(buffer, "%s $%s, 0x%X($%s)", mnem, sTgt, offset, sBase);
}

/* mipsType8
 *  Render a Type 8 instruction.
 *  [MNEM] rs
 */
static void re_gr(char *buffer, const char *mnem, int i_rs) {
    const char *rs = mipsRegisterArray[i_rs];
    sprintf(buffer, "%s $%s", mnem, rs);
}

/** Render a GPR, FPR type instruction.
 * [MNEM] GPR, FPR
 * 
 * @param buffer string to act as the destination.
 * @param mnem string containing the mnemonic.
 * @param i_rt int containing the target GP register.
 * @param i_fs int containing the source FP register.
 */
static void re_gr_fr(char *buffer, const char *mnem, int i_rt, int i_fs) {
    const char *rt = mipsRegisterArray[i_rt];
    const char *fs = mipsFpRegNames[i_fs];
    sprintf(buffer, "%s $%s, $%s", mnem, rt, fs);
}

/** Render a GPR, GPR type instruction.
 * [MNEM] GPR, GPR
 * 
 * @param buffer string to act as the destination.
 * @param mnem string containing the mnemonic.
 * @param i_rs int containing the source GP register.
 * @param i_rt int containing the target GP register.
 */
static void re_gr_gr(char *buffer, const char *mnem, int i_rs, int i_rt) {
    const char *rs = mipsRegisterArray[i_rs];
    const char *rt = mipsRegisterArray[i_rt];
    sprintf(buffer, "%s $%s, $%s", mnem, rs, rt);
}

/* mipsType9
 *  Render a Type 9 instruction.
 *  [MNEM] rd, rs, cc
 */
static void re_gr_gr_cc(char *buffer, const char *mnem, int i_rd, int i_rs, int i_cc) {
    const char *rd = mipsRegisterArray[i_rd];
    const char *rs = mipsRegisterArray[i_rs];
    sprintf(buffer, "%s $%s, $%s, 0x%X", mnem, rd, rs, i_cc);
}

/* mipsType4
 *  Render a Type 4 instruction.
 *  [MNEM] rd, rs, rt
 */
static void re_gr_gr_gr(char *buffer, const char *mnem, int i_rd, int i_rs, 
        int i_rt) {
    const char *rd = mipsRegisterArray[i_rd];
    const char *rs = mipsRegisterArray[i_rs];
    const char *rt = mipsRegisterArray[i_rt];
    sprintf(buffer, "%s $%s, $%s, $%s", mnem, rd, rs, rt);
}

/** Render a GPR, GPR, immediate type instruction.
 * [MNEM] rs, rt, immediate
 * 
 * @param buffer string to act as the destination.
 * @param mnem string containing the mnemonic.
 * @param i_rs int containing the source GP register.
 * @param i_rt int containing the target GP register.
 * @param imm short containing the immediate value.
 */
static void re_gr_gr_imm(char *buffer, const char *mnem, int i_rs, int i_rt,
        short imm) {
    const char *rs = mipsRegisterArray[i_rs];
    const char *rt = mipsRegisterArray[i_rt];
    sprintf(buffer, "%s $%s, $%s, 0x%04X", mnem, rs, rt, (unsigned short)imm);
}

/* mipsTypeRtRsPosSize
 *  Render a Type 'rt, rs, pos, size' instruction.
 *  [MNEM] rt, rs, pos, size
 */
static void re_gr_gr_pos_size(char *buffer, const char *mnem, int iRt, int iRs, int iPos, int iSize) {
    const char *rt = mipsRegisterArray[iRt];
    const char *rs = mipsRegisterArray[iRs];
    sprintf(buffer, "%s $%s, $%s, %d, %d", mnem, rt, rs, iPos, iSize);
}

/* mipsType7
 *  Render a Type 7 instruction.
 *  [MNEM] rd, rt, sa
 */
static void re_gr_gr_sa(char *buffer, const char *mnem, int i_rd, int i_rt, 
        int sa) {
    const char *rd = mipsRegisterArray[i_rd];
    const char *rt = mipsRegisterArray[i_rt];
    sprintf(buffer, "%s $%s, $%s, 0x%X", mnem, rd, rt, sa);
}

/* mipsTypeRtRdSel
 *  Render a Type 'rt, rd, sel' instruction.
 *  [MNEM] rt, rd, sel
 */
static void re_gr_gr_sel(char* buffer, const char* mnem, 
        int i_rt, int i_rd, int i_sel) {
    const char *rt = mipsRegisterArray[i_rt];
    const char *rd = mipsRegisterArray[i_rd];
    const char *sel = mipsControlRegister[i_sel];
    sprintf(buffer, "%s $%s, $%s, $%s", mnem, rt, rd, sel);
}

/* mipsType11
 *  Render a Type 11 instruction.
 *  [MNEM] rs, rt, target
 */
static void re_gr_gr_target(char *buffer, const char *mnem, int i_rs, int i_rt,
        unsigned int target) {
    const char *rs = mipsRegisterArray[i_rs];
    const char *rt = mipsRegisterArray[i_rt];
    sprintf(buffer, "%s $%s, $%s, 0x%08X", mnem, rs, rt, target);
}

/* mipsType5
 *  Render a Type5 instruction.
 *  [MNEM] rs, immediate
 */
static void re_gr_imm(char *buffer, const char *mnem, int i_rs, short n_imm) {
    const char *rs = mipsRegisterArray[i_rs];
    sprintf(buffer, "%s $%s, 0x%X", mnem, rs, (unsigned short)n_imm);
}

/** Render a GPR, offset(base) type instruction.
 * [MNEM] rt, offset(base)
 * 
 * @param buffer string to act as the destination.
 * @param mnem string containing the mnemonic.
 * @param i_base int containing the base register.
 * @param i_rt int containing the target register.
 * @param offset short containing the offset.
 */
static void re_gr_offset_base(char *buffer, const char *mnem, int i_base, 
        int i_rt, short offset) {
    const char *base = mipsRegisterArray[i_base];
    const char *rt = mipsRegisterArray[i_rt];
    sprintf(buffer, "%s $%s, 0x%X($%s)", mnem, rt, offset, base);
}

/* mipsType12
 *  Render a Type 12 instruction.
 *  [MNEM] rs, target
 */
static void re_gr_target(char *buffer, const char *mnem, int i_rs, 
        unsigned int target) {
    const char *rs = mipsRegisterArray[i_rs];
    sprintf(buffer, "%s $%s, 0x%08X", mnem, rs, target);
}

/* mipsType6
 *  Render a Type6 instruction.
 *  [MNEM] offset(base)
 */
static void re_offset_base(char *buffer, const char *mnem, short offset, 
        int i_base) {
    const char *base = mipsRegisterArray[i_base];
    sprintf(buffer, "%s 0x%X($%s)", mnem, offset, base);
}

/** Render an op, offset(base) type instruction.
 * [MNEM] op, offset(base)
 * 
 * @param buffer string to act as the destination.
 * @param mnem string containing the mnemonic.
 * @param i_base int containing the base register.
 * @param i_op int containing the op.
 * @param offset short containing the offset.
 */
static void re_op_offset_base(char *buffer, const char *mnem, int i_base, 
        int i_op, short offset) {
    const char *base = mipsRegisterArray[i_base];
    sprintf(buffer, "%s 0x%X, 0x%X($%s)", mnem, i_op, offset, base);
}

/* mipsType10
 *  Render a Type 10 instruction.
 *  [MNEM] target
 */
static void re_target(char *buffer, const char *mnem, unsigned int target) {
    sprintf(buffer, "%s 0x%08X", mnem, target);
}

/* mipsTypeFdFs
 *  Render a Type 'fd, fs' instruction.
 *  [MNEM] fd, fs
 */
static void mipsTypeFdFs(char *buffer, const char *mnem, int i_fd, int i_fs) {
    const char *fd = mipsFpRegNames[i_fd];
    const char *fs = mipsFpRegNames[i_fs];
    sprintf(buffer, "%s $%s, $%s", mnem, fd, fs);
}

/* mipsTypeRtOffsetBase
 *  Render a Type 'rt, offset(base)' instruction.
 *  [MNEM] rt, offset(base)
 */
static void mipsTypeRtOffsetBase(char *buffer, const char *mnem, int i_rt, short offset, int i_base) {
    const char *base = mipsRegisterArray[i_base];
    const char *rt = mipsRegisterArray[i_rt];
    sprintf(buffer, "%s $%s, 0x%X($%s)", mnem, rt, offset, base);
}

static void mipsTypeVrd(char* buffer, const char* mnem, int iRd) {
    sprintf(buffer, "%s $v%d", mnem, iRd);
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
    sprintf(buffer, "%s $v%d, %s", mnem, iRd, sCn);
}

static void mipsTypeVrdVrs(char* buffer, const char* mnem, int iRd, int iRs) {
    sprintf(buffer, "%s $v%d, $v%d", mnem, iRd, iRs);
}

static void mipsTypeVrdVrsScale(char* buffer, const char* mnem,
        int iRd, int iRs, int scale) {
    sprintf(buffer, "%s $v%d, $v%d, %d", mnem, iRd, iRs, scale);
}

static void mipsTypeVrdVrsVrt(char* buffer, const char* mnem,
        int iRd, int iRs, int iRt) {
    sprintf(buffer, "%s $v%d, $v%d, $v%d", mnem, iRd, iRs, iRt);
}

void mipsDecode(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    unsigned char uc_opcode;

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
        case 0x04: /* BEQ rs, rt, offset - Branch on Equal */
            in_brs_rt_offset(buffer, "beq", i_inst, i_addr);
            break;
        case 0x05: /* BNE rs, rt, offset - Branch on Not Equal */
            in_brs_rt_offset(buffer, "bne", i_inst, i_addr);
            break;
        case 0x06: /* BLEZ rs, offset - Branch on Less Than or Equal to Zero */
            in_brs_offset(buffer, "blez", i_inst, i_addr);
            break;
        case 0x07: /* BGTZ rs, offset - Branch on Greater Than Zero */
            in_brs_offset(buffer, "bgtz", i_inst, i_addr);
            break;
        case 0x08: /* ADDI rt, rs, immediate - Add Immediate */
            in_rt_rs_imm(buffer, "addi", i_inst);
            break;
        case 0x09: /* ADDIU rt, rs, immediate - Add Immediate Unsigned Word  */
            in_rt_rs_imm(buffer, "addiu", i_inst);
            break;
        case 0x0A: /* SLTI rs, rt, immediate - Set on Less Than Immediate */
            in_rs_rt_imm(buffer, "slti", i_inst);
            break;
        case 0x0B: /* SLTIU rs, rt, immediate - 
                    * Set on Less Than Immediate Unsigned */
            in_rs_rt_imm(buffer, "sltiu", i_inst);
            break;
        case 0x0C: /* ANDI rt, rs, immediate - AND Immediate */
            in_rt_rs_imm(buffer, "andi", i_inst);
            break;
        case 0x0D: /* ORI rs, rt, immediate - OR Immediate */
            in_rs_rt_imm(buffer, "ori", i_inst);
            break;
        case 0x0E: /* XORI rs, rt, immediate - XOR Immediate */
            in_rs_rt_imm(buffer, "xori", i_inst);
            break;
        case 0x0F: /* LUI rt, immediate - Load Upper Immediate */
            in_rt_imm(buffer, "lui", i_inst);
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
        case 0x14: /* BEQL rs, rt, offset - Branch on Equal Likely */
            in_brs_rt_offset(buffer, "beql", i_inst, i_addr);
            break;
        case 0x15: /* BNEL rs, rt, offset - Branch on Not Equal Likely */
            in_brs_rt_offset(buffer, "bnel", i_inst, i_addr);
            break;
        case 0x16: /* BLEZL rs, offset - 
                    * Branch on Less Than or Equal to Zero Likely */
            in_brs_offset(buffer, "blezl", i_inst, i_addr);
            break;
        case 0x17: /* BGTZL rs, offset - Branch on Greater Than Zero Likely */
            in_brs_offset(buffer, "bgtzl", i_inst, i_addr);
            break;
        case 0x1C: /* *SPECIAL2 */
            decodeSPECIAL2(buffer, i_inst, i_addr);
            break;
        case 0x1F: /* *SPECIAL3 */
            decodeSPECIAL3(buffer, i_inst, i_addr);
            break;
        case 0x20: /* LB rt, offset(base) - Load Byte */
            in_rt_offset_base(buffer, "lb", i_inst);
            break;
        case 0x21: /* LH rt, offset(base) - Load Halfword */
            in_rt_offset_base(buffer, "lh", i_inst);
            break;
        case 0x22: /* LWL rt, offset(base) - Load Word Left */
            in_rt_offset_base(buffer, "lwl", i_inst);
            break;
        case 0x23: /* LW rt, offset(base) - Load Word */
            in_rt_offset_base(buffer, "lw", i_inst);
            break;
        case 0x24: /* LBU rt, offset(base) - Load Byte Unsigned */
            in_rt_offset_base(buffer, "lbu", i_inst);
            break;
        case 0x25: /* LHU rt, offset(base) - Load Halfword Unsigned */
            in_rt_offset_base(buffer, "lhu", i_inst);
            break;
        case 0x26: /* LWR rt, offset(base) - Load Word Right */
            in_rt_offset_base(buffer, "lwr", i_inst);
            break;
        case 0x28: /* SB rt, offset(base) - Store Byte */
            in_rt_offset_base(buffer, "sb", i_inst);
            break;
        case 0x29: /* SH rt, offset(base) - Store Halfword */
            in_rt_offset_base(buffer, "sh", i_inst);
            break;
        case 0x2A: /* SWL rt, offset(base) - Store Word Left */
            in_rt_offset_base(buffer, "swl", i_inst);
            break;
        case 0x2B: /* SW rt, offset(base) - Store Word */
            in_rt_offset_base(buffer, "sw", i_inst);
            break;
        case 0x2E: /* SWR rt, offset(base) - Store Word Right */
            in_rt_offset_base(buffer, "swr", i_inst);
            break;
        case 0x2F: /* CACHE */
            mipsInCACHE(buffer, i_inst);
            break;
        case 0x30: /* LL rt, offset(base) - Load Linked Word */
            in_rt_offset_base(buffer, "ll", i_inst);
            break;
        case 0x31: /* LWC1 ft, offset(base) - Load Word to Floating Point */
            in_ft_offset_base(buffer, "lwc1", i_inst);
            break;
        case 0x32: /* LWC2 rt, offset(base) - Load Word to Coprocessor 2 */
            in_rt_offset_base(buffer, "lwc2", i_inst);
            break;
        case 0x33: /* PREF */
            mipsInPREF(buffer, i_inst);
            break;
        case 0x34: /* *VCOP */
            decodeVCOP(buffer, i_inst, i_addr);
            break;
        case 0x35: /* LDC1 ft, offset(base) - 
                    * Load Doubleword to Floating Point */
            in_ft_offset_base(buffer, "ldc1", i_inst);
            break;
        case 0x36: /* LV.Q rt, offset(base) - 
                    * LoadVector.Quadword Relative to Address */
            in_rt_offset_base(buffer, "lv.q", i_inst);
            break;
        case 0x38: /* SC rt, offset(base) - Store Conditional Word */
            in_rt_offset_base(buffer, "sc", i_inst);
            break;
        case 0x39: /* SWC1 ft, offset(base) - Store Word from Floating Point */
            in_ft_offset_base(buffer, "swc1", i_inst);
            break;
        case 0x3A: /* SWC2 rt, offset(base) - Store Word from Coprocessor 2 */
            in_rt_offset_base(buffer, "swc2", i_inst);
            break;
        case 0x3D: /* SDC1 ft, offset(base) - 
                    * Store Doubleword from Floating Point */
            in_ft_offset_base(buffer, "sdc1", i_inst);
            break;
        case 0x3E: /* SV.Q rt, offset(base) - 
                    * StoreVector.Quadword Relative to Address */
            in_rt_offset_base(buffer, "sv.q", i_inst);
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
        case 0x02: /* WSBH rd, rt - Word Swap Bytes Within Halfwords */
            in_rd_rt(buffer, "wsbh", i_inst);
            break;
        case 0x10: /* SEB rd, rt - Sign-Extend Byte */
            in_rd_rt(buffer, "seb", i_inst);
            break;
        case 0x18: /* SEH rd, rt - Sign-Extend Halfword */
            in_rd_rt(buffer, "seh", i_inst);
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
            case 0x01: /* TLBR - Read Indexed TLB Entry */
                mipsType0(buffer, "tlbr");
                break;
            case 0x02: /* TLBWI - Write Indexed TLB Entry */
                mipsType0(buffer, "tlbwi");
                break;
            case 0x06: /* TLBWR - Write Random TLB Entry */
                mipsType0(buffer, "tlbwr");
                break;
            case 0x08: /* TLBP - Probe TLB for Matching Entry */
                mipsType0(buffer, "tlbp");
                break;
            case 0x18: /* ERET - Exception Return */
                mipsType0(buffer, "eret");
                break;
            case 0x1F: /* DERET - Debug Exception Return */
                mipsType0(buffer, "deret");
                break;
            case 0x20: /* WAIT - Enter StandBy Mode */
                mipsType0(buffer, "wait");
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
            case 0x0A: /* RDPGPRrd, rt - Read GPR from Previous Shadow Set */
                in_rd_rt(buffer, "rdpgpr", i_inst);
                break;
            case 0x0B: /* *MFMC0 */
                mipsInMFMC0(buffer, i_inst);
                break;
            case 0x0E: /* WRPGPR rd, rt - Write to GPR in Previous Shadow Set */
                in_rd_rt(buffer, "wrpgpr", i_inst);
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
        case 0x00: /* MFC1 rt, fs - Move Word From Floating Point */
            in_rt_fs(buffer, "mfc1", i_inst);
            break;
        case 0x02: /* CFC1 rt, fs - Move Control Word From Floating Point */
            in_rt_fs(buffer, "cfc1", i_inst);
            break;
        case 0x03: /* MFHC1 rt, fs - 
                    * Move Word From High Half of Floating Point Register */
            in_rt_fs(buffer, "mfhc1", i_inst);
            break;
        case 0x04: /* MTC1 rt, fs - Move Word to Floating Point */
            in_rt_fs(buffer, "mtc1", i_inst);
            break;
        case 0x06: /* CTC1 rt, fs - Move Control Word to Floating Point */
            in_rt_fs(buffer, "ctc1", i_inst);
            break;
        case 0x07: /* MTHC1 rt, fs - 
                    * Move Word to High Half of Floating Point Register */
            in_rt_fs(buffer, "mthc1", i_inst);
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
        case 0x00: /* ADD.D fd, fs, ft - Floating Point Add (Double) */
            in_fmt_fd_fs_ft(buffer, "add", i_inst);
            break;
        case 0x01: /* SUB.D fd, fs, ft - Floating Point Subtract (Double) */
            in_fmt_fd_fs_ft(buffer, "sub", i_inst);
            break;
        case 0x02: /* MUL.D fd, fs, ft - Floating Point Multiply (Double) */
            in_fmt_fd_fs_ft(buffer, "mul", i_inst);
            break;
        case 0x03: /* DIV.D fd, fs, ft - Floating Point Divide (Double) */
            in_fmt_fd_fs_ft(buffer, "div", i_inst);
            break;
        case 0x04: /* SQRT.D fd, fs - Floating Point Square Root (Double) */
            in_fmt_fd_fs(buffer, "sqrt", i_inst);
            break;
        case 0x05: /* ABS.D fd, fs - Floating Point Absolute Value (Double) */
            in_fmt_fd_fs(buffer, "abs", i_inst);
            break;
        case 0x06: /* MOV.D fd, fs - Floating Point Move (Double) */
            in_fmt_fd_fs(buffer, "mov", i_inst);
            break;
        case 0x07: /* NEG.D fd, fs - Floating Point Negate (Double) */
            in_fmt_fd_fs(buffer, "neg", i_inst);
            break;
        case 0x08: /* ROUND.L.D fd, fs - 
                    * Floating Point Round to Long Fixed Point (Double) */
            in_fmt_fd_fs(buffer, "round.l", i_inst);
            break;
        case 0x09: /* TRUNC.L.D fd, fs - 
                    * Floating Point Truncate to Long Fixed Point (Double) */
            in_fmt_fd_fs(buffer, "trunc.l", i_inst);
            break;
        case 0x0A: /* CEIL.L.D fd, fs - 
                    * Floating Point Ceiling Convert to Long Fixed Point */
            in_fmt_fd_fs(buffer, "ceil.l", i_inst);
            break;
        case 0x0B: /* FLOOR.L.D fd, fs - 
                    * Floating Point Floor Convert to Long Fixed Point */
            in_fmt_fd_fs(buffer, "floor.l", i_inst);
            break;
        case 0x0C: /* ROUND.W.D fd, fs - 
                    * Floating Point Round to Word Fixed Point */
            in_fmt_fd_fs(buffer, "round.w", i_inst);
            break;
        case 0x0D: /* TRUNC.W.D fd, fs - 
                    * Floating Point Truncate to Word Fixed Point */
            in_fmt_fd_fs(buffer, "trunc.w", i_inst);
            break;
        case 0x0E: /* CEIL.W.D fd, fs - 
                    * Floating Point Ceiling Convert to Word Fixed Point */
            in_fmt_fd_fs(buffer, "ceil.w", i_inst);
            break;
        case 0x0F: /* FLOOR.W.D fd, fs - 
                    * Floating Point Floor Convert to Word Fixed Point */
            in_fmt_fd_fs(buffer, "floor.w", i_inst);
            break;
        case 0x11: /* MOVCF.D */
            mipsInMOVCFfmt(buffer, i_inst);
            break;
        case 0x12: /* MOVZ.D fd, fs, rt - 
                    * Floating Point Move Conditional on Zero */
            in_fmt_fd_fs_rt(buffer, "movz", i_inst);
            break;
        case 0x13: /* MOVN.D fd, fs, rt - 
                    * Floating Point Move Conditional on Not Zero */
            in_fmt_fd_fs_rt(buffer, "movn", i_inst);
            break;
        case 0x15: /* RECIP.D fd, fs - Reciprocal Approximation */
            in_fmt_fd_fs(buffer, "recip", i_inst);
            break;
        case 0x16: /* RSQRT.D fd, fs - Reciprocal Square Root Approximation */
            in_fmt_fd_fs(buffer, "rsqrt", i_inst);
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
        case 0x21: /* CVT.S.D fd, fs - 
                    * Floating Point Convert to Single Floating Point */
            in_fmt_fd_fs(buffer, "cvt.s", i_inst);
            break;
        case 0x24: /* CVT.W.D fd, fs - 
                    * Floating Point Convert to Word Fixed Point */
            in_fmt_fd_fs(buffer, "cvt.w", i_inst);
            break;
        case 0x25: /* CVT.L.D fd, fs - 
                    * Floating Point Convert to Long Fixed Point */
            in_fmt_fd_fs(buffer, "cvt.l", i_inst);
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
        case 0x21: /* CVT.S.L fd, fs - 
                    * Floating Point Convert to Single Floating Point */
            in_fmt_fd_fs(buffer, "cvt.s", i_inst);
            break;
        case 0x22: /* CVT.D.L fd, fs - 
                    * Floating Point Convert to Double Floating Point */
            in_fmt_fd_fs(buffer, "cvt.d", i_inst);
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
        case 0x00: /* ADD.PS fd, fs, ft - Floating Point Add */
            in_fmt_fd_fs_ft(buffer, "add", i_inst);
            break;
        case 0x01: /* SUB.PS fd, fs, ft - Floating Point Subtract */
            in_fmt_fd_fs_ft(buffer, "sub", i_inst);
            break;
        case 0x02: /* MUL.PS fd, fs, ft - Floating Point Multiply */
            in_fmt_fd_fs_ft(buffer, "mul", i_inst);
            break;
        case 0x05: /* ABS.PS fd, fs - 
                    * Floating Point Absolute Value (Paired Single) */
            in_fmt_fd_fs(buffer, "abs", i_inst);
            break;
        case 0x06: /* MOV.PS fd, fs - Floating Point Move (Paired Single) */
            in_fmt_fd_fs(buffer, "mov", i_inst);
            break;
        case 0x07: /* NEG.PS fd, fs - Floating Point Negate (Paired Single) */
            in_fmt_fd_fs(buffer, "neg", i_inst);
            break;
        case 0x11: /* MOVCF.PS */
            mipsInMOVCFfmt(buffer, i_inst);
            break;
        case 0x12: /* MOVZ.PS fd, fs, rt - 
                    * Floating Point Move Conditional on Zero */
            in_fmt_fd_fs_rt(buffer, "movz", i_inst);
            break;
        case 0x13: /* MOVN.PS fd, fs, rt - 
                    * Floating Point Move Conditional on Not Zero */
            in_fmt_fd_fs_rt(buffer, "movn", i_inst);
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
        case 0x20: /* CVT.S.PU fd, fs - 
                    * Floating Point Convert Pair Upper to Single Floating Point
                    */
            in_fd_fs(buffer, "cvt.s.pu", i_inst);
            break;
        case 0x24: /* CVT.PW.PS */
            sprintf(buffer, "CVT.PW.PS");
            break;
        case 0x28: /* CVT.S.PL fd, fs - 
                    * Floating Point Convert Pair Lower to Single Floating Point
                    */
            in_fd_fs(buffer, "cvt.s.pl", i_inst);
            break;
        case 0x2C: /* PLL.PS fd, fs, ft - Pair Lower Lower */
            in_fd_fs_ft(buffer, "pll.ps", i_inst);
            break;
        case 0x2D: /* PLU.PS fd, fs, ft - Pair Lower Upper */
            in_fd_fs_ft(buffer, "plu.ps", i_inst);
            break;
        case 0x2E: /* PUL.PS fd, fs, ft - Pair Upper Lower */
            in_fd_fs_ft(buffer, "pul.ps", i_inst);
            break;
        case 0x2F: /* PUU.PS fd, fs, ft - Pair Upper Upper */
            in_fd_fs_ft(buffer, "puu.ps", i_inst);
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
        case 0x00: /* ADD.S fd, fs, ft - Floating Point Add (Single) */
            in_fmt_fd_fs_ft(buffer, "add", i_inst);
            break;
        case 0x01: /* SUB.S fd, fs, ft - Floating Point Subtract (Single) */
            in_fmt_fd_fs_ft(buffer, "sub", i_inst);
            break;
        case 0x02: /* MUL.S fd, fs, ft - Floating Point Multiply (Single) */
            in_fmt_fd_fs_ft(buffer, "mul", i_inst);
            break;
        case 0x03: /* DIV.S fd, fs, ft - Floating Point Divide (Single) */
            in_fmt_fd_fs_ft(buffer, "div", i_inst);
            break;
        case 0x04: /* SQRT.S fd, fs - Floating Point Square Root (Single) */
            in_fmt_fd_fs(buffer, "sqrt", i_inst);
            break;
        case 0x05: /* ABS.S fd, fs - Floating Point Absolute Value (Single) */
            in_fmt_fd_fs(buffer, "abs", i_inst);
            break;
        case 0x06: /* MOV.S fd, fs - Floating Point Move (Single) */
            in_fmt_fd_fs(buffer, "mov", i_inst);
            break;
        case 0x07: /* NEG.S fd, fs - Floating Point Negate (Single) */
            in_fmt_fd_fs(buffer, "neg", i_inst);
            break;
        case 0x08: /* ROUND.L.S fd, fs - 
                    * Floating Point Round to Long Fixed Point (Single) */
            in_fmt_fd_fs(buffer, "round.l", i_inst);
            break;
        case 0x09: /* TRUNC.L.S fd, fs - 
                    * Floating Point Truncate to Long Fixed Point (Single) */
            in_fmt_fd_fs(buffer, "trunc.l", i_inst);
            break;
        case 0x0A: /* CEIL.L.S fd, fs - 
                    * Floating Point Ceiling Convert to Long Fixed Point */
            in_fmt_fd_fs(buffer, "ceil.l", i_inst);
            break;
        case 0x0B: /* FLOOR.L.S fd, fs - 
                    * Floating Point Floor Convert to Long Fixed Point */
            in_fmt_fd_fs(buffer, "floor.l", i_inst);
            break;
        case 0x0C: /* ROUND.W.S fd, fs - 
                    * Floating Point Round to Word Fixed Point */
            in_fmt_fd_fs(buffer, "round.w", i_inst);
            break;
        case 0x0D: /* TRUNC.W.S fd, fs - 
                    * Floating Point Truncate to Word Fixed Point */
            in_fmt_fd_fs(buffer, "trunc.w", i_inst);
            break;
        case 0x0E: /* CEIL.W.S fd, fs - 
                    * Floating Point Ceiling Convert to Word Fixed Point */
            in_fmt_fd_fs(buffer, "ceil.w", i_inst);
            break;
        case 0x0F: /* FLOOR.W.S fd, fs - 
                    * Floating Point Floor Convert to Word Fixed Point */
            in_fmt_fd_fs(buffer, "floor.w", i_inst);
            break;
        case 0x11: /* MOVCF.S */
            mipsInMOVCFfmt(buffer, i_inst);
            break;
        case 0x12: /* MOVZ.S fd, fs, rt - 
                    * Floating Point Move Conditional on Zero */
            in_fmt_fd_fs_rt(buffer, "movz", i_inst);
            break;
        case 0x13: /* MOVN.S fd, fs, rt - 
                    * Floating Point Move Conditional on Not Zero */
            in_fmt_fd_fs_rt(buffer, "movn", i_inst);
            break;
        case 0x15: /* RECIP.S fd, fs - Reciprocal Approximation */
            in_fmt_fd_fs(buffer, "recip", i_inst);
            break;
        case 0x16: /* RSQRT.S fd, fs - Reciprocal Square Root Approximation */
            in_fmt_fd_fs(buffer, "rsqrt", i_inst);
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
        case 0x21: /* CVT.D.S fd, fs - 
                    * Floating Point Convert to Double Floating Point */
            in_fmt_fd_fs(buffer, "cvt.d", i_inst);
            break;
        case 0x24: /* CVT.W.S fd, fs - 
                    * Floating Point Convert to Word Fixed Point */
            in_fmt_fd_fs(buffer, "cvt.w", i_inst);
            break;
        case 0x25: /* CVT.L.S fd, fs - 
                    * Floating Point Convert to Long Fixed Point */
            in_fmt_fd_fs(buffer, "cvt.l", i_inst);
            break;
        case 0x26: /* CVT.PS.S fd, fs, ft - 
                    * Floating Point Convert Pair to Paired Single */
            in_fd_fs_ft(buffer, "cvt.ps.s", i_inst);
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
        case 0x20: /* CVT.S.W fd, fs - 
                    * Floating Point Convert to Single Floating Point */
            in_fmt_fd_fs(buffer, "cvt.s", i_inst);
            break;
        case 0x21: /* CVT.D.W fd, fs - 
                    * Floating Point Convert to Double Floating Point */
            in_fmt_fd_fs(buffer, "cvt.d", i_inst);
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
        case 0x02: /* CFC2 rt, fs - Move Control Word From Floating Point */
            in_rt_fs(buffer, "cfc2", i_inst);
            break;
        case 0x03: /* MFHC2 rt, rd - 
                    * Move Word From High Half of Coprocessor 2 Register */
            in_rt_rd(buffer, "mfhc2", i_inst);
            break;
        case 0x04: /* MTC2 rt, rd - Move Word to Coprocessor 2 */
            in_rt_rd(buffer, "mtc2", i_inst);
            break;
        case 0x06: /* CTC2 rt, rd - Move Control Word to Coprocessor 2 */
            in_rt_rd(buffer, "ctc2", i_inst);
            break;
        case 0x07: /* MTHC2 rt, rd - 
                    * Move Word to High Half of Coprocessor 2 Register */
            in_rt_rd(buffer, "mthc2", i_inst);
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
        case 0x00: /* LWXC1 fd, index(base) - 
                    * Load Word Indexed to Floating Point */
            in_fd_index_base(buffer, "lwxc1", i_inst);
            break;
        case 0x01: /* LDXC1 fd, index(base) - 
                    * Load Doubleword Indexed to Floating Point */
            in_fd_index_base(buffer, "ldxc1", i_inst);
            break;
        case 0x05: /* LUXC1 fd, index(base) - 
                    * Load Doubleword Indexed Unaligned to Floating Point */
            in_fd_index_base(buffer, "luxc1", i_inst);
            break;
        case 0x08: /* SWXC1 fs, index(base) - 
                    * Store Word Indexed from Floating Point */
            in_fs_index_base(buffer, "swxc1", i_inst);
            break;
        case 0x09: /* SDXC1 fs, index(base) - 
                    * Store Doubleword Indexed from Floating Point */
            in_fs_index_base(buffer, "sdxc1", i_inst);
            break;
        case 0x0D: /* SUXC1 fs, index(base) - 
                    * Store Doubleword Indexed Unaligned from Floating Point */
            in_fs_index_base(buffer, "suxc1", i_inst);
            break;
        case 0x0F: /* PREFX */
            mipsInPREFX(buffer, i_inst);
            break;
        case 0x1E: /* ALNV.PS fd, fs, ft, rs - Floating Point Align Variable */
            in_fd_fs_ft_rs(buffer, "alnv.ps", i_inst);
            break;
        case 0x20: /* MADD.S fd, fr, fs, ft - Floating Point Multiply Add */
            in_fd_fr_fs_ft(buffer, "madd", i_inst);
            break;
        case 0x21: /* MADD.D fd, fr, fs, ft - Floating Point Multiply Add*/
            in_fd_fr_fs_ft(buffer, "madd", i_inst);
            break;
        case 0x26: /* MADD.PS fd, fr, fs, ft - Floating Point Multiply Add */
            in_fd_fr_fs_ft(buffer, "madd", i_inst);
            break;
        case 0x28: /* MSUB.S fd, fr, fs, ft - Floating Point Multiply Subtract 
                    */
            in_fd_fr_fs_ft(buffer, "msub", i_inst);
            break;
        case 0x29: /* MSUB.D fd, fr, fs, ft - Floating Point Multiply Subtract 
                    */
            in_fd_fr_fs_ft(buffer, "msub", i_inst);
            break;
        case 0x2E: /* MSUB.PS fd, fr, fs, ft - Floating Point Multiply Subtract
                    */
            in_fd_fr_fs_ft(buffer, "msub", i_inst);
            break;
        case 0x30: /* NMADD.S fd, fr, fs, ft - 
                    * Floating Point Negative Multiply Add */
            in_fd_fr_fs_ft(buffer, "nmadd", i_inst);
            break;
        case 0x31: /* NMADD.D fd, fr, fs, ft - 
                    * Floating Point Negative Multiply Add */
            in_fd_fr_fs_ft(buffer, "nmadd", i_inst);
            break;
        case 0x36: /* NMADD.PS fd, fr, fs, ft - 
                    * Floating Point Negative Multiply Add */
            in_fd_fr_fs_ft(buffer, "nmadd", i_inst);
            break;
        case 0x38: /* NMSUB.S fd. fr. fs. ft - 
                    * Floating Point Negative Multiply Subtract */
            in_fd_fr_fs_ft(buffer, "nmsub", i_inst);
            break;
        case 0x39: /* NMSUB.D fd. fr. fs. ft - 
                    * Floating Point Negative Multiply Subtract */
            in_fd_fr_fs_ft(buffer, "nmsub", i_inst);
            break;
        case 0x3E: /* NMSUB.PS fd. fr. fs. ft - 
                    * Floating Point Negative Multiply Subtract */
            in_fd_fr_fs_ft(buffer, "nmsub", i_inst);
            break;
        default:
            sprintf(buffer, "???");
    }
}

static void decodeREGIMM(char *buffer, unsigned int i_inst,
        unsigned int i_addr) {
    int rt = (int)mipsGetOperand1(i_inst);
    switch (rt) {
        case 0x00: /* BLTZ rs, offset - Branch on Less Than Zero */
            in_brs_offset(buffer, "bltz", i_inst, i_addr);
            break;
        case 0x01: /* BGEZ rs, offset - 
                    * Branch on Greater Than or Equal to Zero */
            in_brs_offset(buffer, "bgez", i_inst, i_addr);
            break;
        case 0x02: /* BLTZL rs, offset - Branch on Less Than Zero Likely */
            in_brs_offset(buffer, "bltzl", i_inst, i_addr);
            break;
        case 0x03: /* BGEZL rs, offset - 
                    * Branch on Greater Than or Equal to Zero Likely */
            in_brs_offset(buffer, "bgezl", i_inst, i_addr);
            break;
        case 0x08: /* TGEI rs, immediate - Trap if Greater or Equal Immediate */
            in_rs_imm(buffer, "tgei", i_inst);
            break;
        case 0x09: /* TGEIU rs, immediate - 
                    * Trap if Greater or Equal Immediate Unsigned */
            in_rs_imm(buffer, "tgeiu", i_inst);
            break;
        case 0x0A: /* TLTI rs, immediate - Trap if Less Than Immediate */
            in_rs_imm(buffer, "tlti", i_inst);
            break;
        case 0x0B: /* TLTIU rs, immediate - 
                    * Trap if Less Than Immediate Unsigned */
            in_rs_imm(buffer, "tltiu", i_inst);
            break;
        case 0x0C: /* TEQI rs, immediate - Trap if Equal Immediate */
            in_rs_imm(buffer, "tegi", i_inst);
            break;
        case 0x0E: /* TNEI rs, immediate - Trap if Not Equal Immediate */
            in_rs_imm(buffer, "tnei", i_inst);
            break;
        case 0x10: /* BLTZAL rs, offset - Branch on Less Than Zero and Link */
            in_brs_offset(buffer, "bltzal", i_inst, i_addr);
            break;
        case 0x11: /* BGEZAL rs, offset - 
                    * Branch on Greater Than or Equal to Zero and Link */
            in_brs_offset(buffer, "bgezal", i_inst, i_addr);
            break;
        case 0x12: /* BLTZALL rs, offset - 
                    * Branch on Less Than Zero and Link Likely */
            in_brs_offset(buffer, "bltzall", i_inst, i_addr);
            break;
        case 0x13: /* BGEZALL rs, offset - 
                    * Branch on Greater Than or Equal to Zero and Link Likely */
            in_brs_offset(buffer, "bgezall", i_inst, i_addr);
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
        case 0x03: /* SRA rd, rt, sa - Shift Word Right Arithmetic */
            in_rd_rt_sa(buffer, "sra", i_inst);
            break;
        case 0x04: /* SLLV rd, rt, rs - Shift Word Left Logical Variable */
            in_rd_rt_rs(buffer, "sllv", i_inst);
            break;
        case 0x06: /* *SRLV */
            mipsInSRLV(buffer, i_inst);
            break;
        case 0x07: /* SRAV rd, rt, rs - Shift Word Right Arithmetic Variable */
            in_rd_rt_rs(buffer, "srav", i_inst);
            break;
        case 0x08: /* JR */
            mipsInJR(buffer, i_inst);
            break;
        case 0x09: /* JALR */
            mipsInJALR(buffer, i_inst);
            break;
        case 0x0A: /* MOVZ rd, rs, rt - Move Conditional on Zero */
            in_rd_rs_rt(buffer, "movz", i_inst);
            break;
        case 0x0B: /* MOVN rd, rs, rt - Move Conditional on Not Zero */
            in_rd_rs_rt(buffer, "movn", i_inst);
            break;
        case 0x0C: /* SYSCALL - System Call */
            mipsType0(buffer, "syscall");
            break;
        case 0x0D: /* BREAK - Breakpoint */
            mipsType0(buffer, "break");
            break;
        case 0x0F: /* SYNC - Synchronize Shared Memory */
            mipsType0(buffer, "sync");
            break;
        case 0x10: /* MFHI rs - Move From HI Register */
            in_rs(buffer, "mfhi", i_inst);
            break;
        case 0x11: /* MTHI rs - Move to HI Register */
            in_rs(buffer, "mthi", i_inst);
            break;
        case 0x12: /* MFLO rs - Move From LO Register */
            in_rs(buffer, "mflo", i_inst);
            break;
        case 0x13: /* MTLO rs - Move to LO Register */
            in_rs(buffer, "mtlo", i_inst);
            break;
        case 0x18: /* MULT rs, rt - Multiply Word */
            in_rs_rt(buffer, "mult", i_inst);
            break;
        case 0x19: /* MULTU rs, rt - Multiply Unsigned Word */
            in_rs_rt(buffer, "multu", i_inst);
            break;
        case 0x1A: /* DIV rs, rt - Divide Word */
            in_rs_rt(buffer, "div", i_inst);
            break;
        case 0x1B: /* DIVU rs, rt - Divide Unsigned Word */
            in_rs_rt(buffer, "divu", i_inst);
            break;
        case 0x20: /* ADD rd, rs, rt - Add Word */
            in_rd_rs_rt(buffer, "add", i_inst);
            break;
        case 0x21: /* ADDU rd, rs, rt - Add Unsigned Word */
            in_rd_rs_rt(buffer, "addu", i_inst);
            break;
        case 0x22: /* SUB rd, rs, rt - Subtract Word */
            in_rd_rs_rt(buffer, "sub", i_inst);
            break;
        case 0x23: /* SUBU rd, rs, rt - Subtract Unsigned Word */
            in_rd_rs_rt(buffer, "subu", i_inst);
            break;
        case 0x24: /* AND rd, rs, rt - And */
            in_rd_rs_rt(buffer, "and", i_inst);
            break;
        case 0x25: /* OR rd, rs, rt - Or */
            in_rd_rs_rt(buffer, "or", i_inst);
            break;
        case 0x26: /* XOR rd, rs, rt - Exclusive Or */
            in_rd_rs_rt(buffer, "xor", i_inst);
            break;
        case 0x27: /* NOR rd, rs, rt - Not Or */
            in_rd_rs_rt(buffer, "nor", i_inst);
            break;
        case 0x2A: /* SLT rd, rs, rt - Set on Less Than */
            in_rd_rs_rt(buffer, "slt", i_inst);
            break;
        case 0x2B: /* SLTU rd, rs, rt - Set on Less Than Unsigned */
            in_rd_rs_rt(buffer, "sltu", i_inst);
            break;
        case 0x30: /* TGE rs, rt - Trap if Greater or Equal */
            in_rs_rt(buffer, "tge", i_inst);
            break;
        case 0x31: /* TGEU rs, rt - Trap if Greater or Equal Unsigned */
            in_rs_rt(buffer, "tgeu", i_inst);
            break;
        case 0x32: /* TLT rs, rt - Trap if Less Than */
            in_rs_rt(buffer, "tlt", i_inst);
            break;
        case 0x33: /* TLTU rs, rt - Trap if Less Than Unsigned */
            in_rs_rt(buffer, "tltu", i_inst);
            break;
        case 0x34: /* TEQ rs, rt - Trap if Equal */
            in_rs_rt(buffer, "teq", i_inst);
            break;
        case 0x36: /* TNE rs, rt - Trap if Not Equal */
            in_rs_rt(buffer, "tne", i_inst);
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
        case 0x00: /* MADD rs, rt - Multiply and Add Word to Hi, Lo */
            in_rs_rt(buffer, "madd", i_inst);
            break;
        case 0x01: /* MADDU rs, rt - Multiply and Add Unsigned Word to Hi, Lo */
            in_rs_rt(buffer, "maddu", i_inst);
            break;
        case 0x02: /* MUL rd, rs, rt - Multiply Word to GPR */
            in_rd_rs_rt(buffer, "mul", i_inst);
            break;
        case 0x04: /* MSUB rs, rt - Multiply and Subtract Word to Hi, Lo */
            in_rs_rt(buffer, "msub", i_inst);
            break;
        case 0x05: /* MSUBU rs, rt - 
                    * Multiply and Subtract Unsigned Word to Hi, Lo */
            in_rs_rt(buffer, "msubu", i_inst);
            break;
        case 0x20: /* CLZ rd, rs - Count Leading Zeros in Word */
            in_rd_rs(buffer, "clz", i_inst);
            break;
        case 0x21: /* CLO rd, rs - Count Leading Ones in Word */
            in_rd_rs(buffer, "clo", i_inst);
            break;
        case 0x24: /* MFIC rt, rd - Allegrex MFIC */
            in_rt_rd(buffer, "mfic", i_inst);
            break;
        case 0x26: /* MTIC rt, rd - Allegrex MTIC */
            in_rt_rd(buffer, "mtic", i_inst);
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
        case 0x3B: /* RDHWR rt, rd - Read Hardware Register */
            in_rt_rd(buffer, "rdhwr", i_inst);
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

