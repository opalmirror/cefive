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

static void de_bshfl(char *, unsigned int, unsigned int);
static void de_cop0(char *, unsigned int, unsigned int);
static void de_cop1(char *, unsigned int, unsigned int);
static void de_cop1d(char *, unsigned int, unsigned int);
static void de_cop1l(char *, unsigned int, unsigned int);
static void de_cop1ps(char *, unsigned int, unsigned int);
static void de_cop1s(char *, unsigned int, unsigned int);
static void de_cop1w(char *, unsigned int, unsigned int);
static void de_cop1x(char *, unsigned int, unsigned int);
static void de_cop2(char *, unsigned int, unsigned int);
static void de_regimm(char *, unsigned int, unsigned int);
static void de_special(char *, unsigned int, unsigned int);
static void de_special2(char *, unsigned int, unsigned int);
static void de_special3(char *, unsigned int, unsigned int);
static void de_vcop(char* buffer, unsigned int i_inst, unsigned int i_addr);
static void de_vcop2(char* buffer, const SceUInt32 inst, const SceUInt32 addr);
static void de_vcop3(char* buffer, const SceUInt32 inst, const SceUInt32 addr);
static void de_vcop4(char* buffer, const SceUInt32 inst, const SceUInt32 addr);
static void de_vcop5(char* buffer, const SceUInt32 inst, const SceUInt32 addr);

static char* get_format(int fmt);
static short get_imm(unsigned int i_inst);
static int get_instr_index(unsigned int i_inst);
static unsigned char get_operand0(unsigned int i_inst);
static unsigned char get_operand1(unsigned int i_inst);
static unsigned char get_operand2(unsigned int i_inst);
static unsigned char get_operand3(unsigned int i_inst);
static unsigned short get_uimm(unsigned int i_inst);
static char* get_vformat(int f, int F);
static int get_voperand0(unsigned int i_inst);
static int get_voperand1(unsigned int i_inst);
static int get_voperand2(unsigned int i_inst);
static int get_voperand3(unsigned int i_inst);
static int get_voperand4(unsigned int i_inst);
static int get_voperand5(unsigned int i_inst);

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
static void in_rt_rd_sel(char* buffer, const char* mnem, unsigned int inst);
static void in_rt_rs_imm(char* buffer, const char* mnem, unsigned int inst);
static void in_rt_rs_pos_size(char* buffer, const char* mnem, 
        unsigned int inst);
static void in_target(char* buffer, const char* mnem, unsigned int inst, 
        unsigned int address);
static void in_vrd(char* buffer, const char* mnem, unsigned int inst);
static void in_vrd_a(char* buffer, const char* mnem, unsigned int inst);
static void in_vrd_vrs(char* buffer, const char* mnem, unsigned int inst);
static void in_vrd_vrs_scale(char* buffer, const char* mnem, unsigned int inst);
static void in_vrd_vrs_vrt(char* buffer, const char* mnem, unsigned int inst);
static void in_vrs_vrd(char* buffer, const char* mnem, unsigned int inst);

static char* get_format(int fmt);
static short get_imm(unsigned int i_inst);
static int get_instr_index(unsigned int i_inst);
static unsigned char get_operand0(unsigned int i_inst);
static unsigned char get_operand1(unsigned int i_inst);
static unsigned char get_operand2(unsigned int i_inst);
static unsigned char get_operand3(unsigned int i_inst);
static void mipsInBC1(char *buffer, unsigned int i_inst, unsigned int i_addr);
static void mipsInBC2(char *buffer, unsigned int i_inst, unsigned int i_addr);
static void mipsInCACHE(char *buffer, unsigned int i_inst);
static void mipsInCcondfmt(char *buffer, unsigned int i_inst);
static void mipsInJALR(char *buffer, unsigned int i_inst);
static void mipsInMFMC0(char *buffer, unsigned int i_inst);
static void mipsInMOVC1(char *buffer, unsigned int i_inst);
static void mipsInMOVCFfmt(char *buffer, unsigned int i_inst);
static void mipsInMTC0(char *buffer, unsigned int i_inst);
static void mipsInPREF(char *buffer, unsigned int i_inst);
static void mipsInPREFX(char *buffer, unsigned int i_inst);
static void mipsInSDBBP(char *buffer, unsigned int i_inst);
static void mipsInSLL(char *buffer, unsigned int i_inst);
static void mipsInSRL(char *buffer, unsigned int i_inst);
static void mipsInSRLV(char *buffer, unsigned int i_inst);
static void mipsInSYNCI(char *buffer, unsigned int i_instr);

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
static void re_mnem(char *, const char *);
static void re_offset_base(char *, const char *, short, int);
static void re_op_offset_base(char *, const char *, int, int, short);
static void re_target(char *, const char *, unsigned int);
static void re_vr(char* buffer, const char* mnem, int iRd);
static void re_vr_a(char* buffer, const char* mnem, int iRd, int iA);
static void re_vr_vr(char* buffer, const char* mnem, int iRd, int iRs);
static void re_vr_vr_scale(char* buffer, const char* mnem,
        int iRd, int iRs, int scale);
static void re_vr_vr_vr(char* buffer, const char* mnem,
        int iRd, int iRs, int iRt);

SceUInt32 mipsGetBranchDestination(unsigned int i_inst, unsigned int i_addr) {
    SceUInt32 dest = 0;
    int offset = (int)get_imm(i_inst);
    dest = i_addr + 4 + (offset << 2);
    return dest;
}

unsigned char mipsGetFunction(unsigned int i_inst) {
    unsigned char uc_func;
    uc_func = (unsigned char)(i_inst & 0x3F);
    return uc_func;
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

static void de_bshfl(char *buffer, unsigned int i_inst, 
        unsigned int i_addr) {
    int sa = (int)get_operand3(i_inst);
    switch (sa) {
        case 0x02: /* WSBH rd, rt - Word Swap Bytes Within Halfwords */
            in_rd_rt(buffer, "WSBH", i_inst);
            break;
        case 0x10: /* SEB rd, rt - Sign-Extend Byte */
            in_rd_rt(buffer, "SEB", i_inst);
            break;
        case 0x18: /* SEH rd, rt - Sign-Extend Halfword */
            in_rd_rt(buffer, "SEH", i_inst);
            break;
        default:
            sprintf(buffer, "???");
            break;
    }
}

static void de_cop0(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    int rs = (int)get_operand0(i_inst);
    int func = (int)mipsGetFunction(i_inst);
    if (rs > 0x10) {
        switch (func) {
            case 0x01: /* TLBR - Read Indexed TLB Entry */
                re_mnem(buffer, "TLBR");
                break;
            case 0x02: /* TLBWI - Write Indexed TLB Entry */
                re_mnem(buffer, "TLBWI");
                break;
            case 0x06: /* TLBWR - Write Random TLB Entry */
                re_mnem(buffer, "TLBWR");
                break;
            case 0x08: /* TLBP - Probe TLB for Matching Entry */
                re_mnem(buffer, "TLBP");
                break;
            case 0x18: /* ERET - Exception Return */
                re_mnem(buffer, "ERET");
                break;
            case 0x1F: /* DERET - Debug Exception Return */
                re_mnem(buffer, "DERET");
                break;
            case 0x20: /* WAIT - Enter StandBy Mode */
                re_mnem(buffer, "WAIT");
                break;
            default:
                sprintf(buffer, "???");
                break;
        }
    } else {
        switch (rs) {
            case 0x00: /* MFC0 rt, rd[, sel] - Move from Coprocessor 0 */
                in_rt_rd_sel(buffer, "MFC0", i_inst);
                break;
            case 0x04: /* MTC0 */
                mipsInMTC0(buffer, i_inst);
                break;
            case 0x0A: /* RDPGPR rd, rt - Read GPR from Previous Shadow Set */
                in_rd_rt(buffer, "RDPGPR", i_inst);
                break;
            case 0x0B: /* *MFMC0 */
                mipsInMFMC0(buffer, i_inst);
                break;
            case 0x0E: /* WRPGPR rd, rt - Write to GPR in Previous Shadow Set */
                in_rd_rt(buffer, "WRPGPR", i_inst);
                break;
            default:
                sprintf(buffer, "???");
                break;
        }
    }
}

static void de_cop1(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    int rs = (int)get_operand0(i_inst);
    switch (rs) {
        case 0x00: /* MFC1 rt, fs - Move Word From Floating Point */
            in_rt_fs(buffer, "MFC1", i_inst);
            break;
        case 0x02: /* CFC1 rt, fs - Move Control Word From Floating Point */
            in_rt_fs(buffer, "CFC1", i_inst);
            break;
        case 0x03: /* MFHC1 rt, fs - 
                    * Move Word From High Half of Floating Point Register */
            in_rt_fs(buffer, "MFHC1", i_inst);
            break;
        case 0x04: /* MTC1 rt, fs - Move Word to Floating Point */
            in_rt_fs(buffer, "MTC1", i_inst);
            break;
        case 0x06: /* CTC1 rt, fs - Move Control Word to Floating Point */
            in_rt_fs(buffer, "CTC1", i_inst);
            break;
        case 0x07: /* MTHC1 rt, fs - 
                    * Move Word to High Half of Floating Point Register */
            in_rt_fs(buffer, "MTHC1", i_inst);
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
            de_cop1s(buffer, i_inst, i_addr);
            break;
        case 0x11: /* *D */
            de_cop1d(buffer, i_inst, i_addr);
            break;
        case 0x14: /* *W */
            de_cop1w(buffer, i_inst, i_addr);
            break;
        case 0x15: /* *L */
            de_cop1l(buffer, i_inst, i_addr);
            break;
        case 0x16: /* *PS */
            de_cop1ps(buffer, i_inst, i_addr);
            break;
        default:
            sprintf(buffer, "???");
    }
}

static void de_cop1d(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    int func = (int)mipsGetFunction(i_inst);
    switch (func) {
        case 0x00: /* ADD.D fd, fs, ft - Floating Point Add (Double) */
            in_fmt_fd_fs_ft(buffer, "ADD", i_inst);
            break;
        case 0x01: /* SUB.D fd, fs, ft - Floating Point Subtract (Double) */
            in_fmt_fd_fs_ft(buffer, "SUB", i_inst);
            break;
        case 0x02: /* MUL.D fd, fs, ft - Floating Point Multiply (Double) */
            in_fmt_fd_fs_ft(buffer, "MUL", i_inst);
            break;
        case 0x03: /* DIV.D fd, fs, ft - Floating Point Divide (Double) */
            in_fmt_fd_fs_ft(buffer, "DIV", i_inst);
            break;
        case 0x04: /* SQRT.D fd, fs - Floating Point Square Root (Double) */
            in_fmt_fd_fs(buffer, "SQRT", i_inst);
            break;
        case 0x05: /* ABS.D fd, fs - Floating Point Absolute Value (Double) */
            in_fmt_fd_fs(buffer, "ABS", i_inst);
            break;
        case 0x06: /* MOV.D fd, fs - Floating Point Move (Double) */
            in_fmt_fd_fs(buffer, "MOV", i_inst);
            break;
        case 0x07: /* NEG.D fd, fs - Floating Point Negate (Double) */
            in_fmt_fd_fs(buffer, "NEG", i_inst);
            break;
        case 0x08: /* ROUND.L.D fd, fs - 
                    * Floating Point Round to Long Fixed Point (Double) */
            in_fmt_fd_fs(buffer, "ROUND.L", i_inst);
            break;
        case 0x09: /* TRUNC.L.D fd, fs - 
                    * Floating Point Truncate to Long Fixed Point (Double) */
            in_fmt_fd_fs(buffer, "TRUNC.L", i_inst);
            break;
        case 0x0A: /* CEIL.L.D fd, fs - 
                    * Floating Point Ceiling Convert to Long Fixed Point */
            in_fmt_fd_fs(buffer, "CEIL.L", i_inst);
            break;
        case 0x0B: /* FLOOR.L.D fd, fs - 
                    * Floating Point Floor Convert to Long Fixed Point */
            in_fmt_fd_fs(buffer, "FLOOR.L", i_inst);
            break;
        case 0x0C: /* ROUND.W.D fd, fs - 
                    * Floating Point Round to Word Fixed Point */
            in_fmt_fd_fs(buffer, "ROUND.W", i_inst);
            break;
        case 0x0D: /* TRUNC.W.D fd, fs - 
                    * Floating Point Truncate to Word Fixed Point */
            in_fmt_fd_fs(buffer, "TRUNC.W", i_inst);
            break;
        case 0x0E: /* CEIL.W.D fd, fs - 
                    * Floating Point Ceiling Convert to Word Fixed Point */
            in_fmt_fd_fs(buffer, "CEIL.W", i_inst);
            break;
        case 0x0F: /* FLOOR.W.D fd, fs - 
                    * Floating Point Floor Convert to Word Fixed Point */
            in_fmt_fd_fs(buffer, "FLOOR.W", i_inst);
            break;
        case 0x11: /* MOVCF.D */
            mipsInMOVCFfmt(buffer, i_inst);
            break;
        case 0x12: /* MOVZ.D fd, fs, rt - 
                    * Floating Point Move Conditional on Zero */
            in_fmt_fd_fs_rt(buffer, "MOVZ", i_inst);
            break;
        case 0x13: /* MOVN.D fd, fs, rt - 
                    * Floating Point Move Conditional on Not Zero */
            in_fmt_fd_fs_rt(buffer, "MOVN", i_inst);
            break;
        case 0x15: /* RECIP.D fd, fs - Reciprocal Approximation */
            in_fmt_fd_fs(buffer, "RECIP", i_inst);
            break;
        case 0x16: /* RSQRT.D fd, fs - Reciprocal Square Root Approximation */
            in_fmt_fd_fs(buffer, "RSQRT", i_inst);
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
            in_fmt_fd_fs(buffer, "CVT.S", i_inst);
            break;
        case 0x24: /* CVT.W.D fd, fs - 
                    * Floating Point Convert to Word Fixed Point */
            in_fmt_fd_fs(buffer, "CVT.W", i_inst);
            break;
        case 0x25: /* CVT.L.D fd, fs - 
                    * Floating Point Convert to Long Fixed Point */
            in_fmt_fd_fs(buffer, "CVT.L", i_inst);
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

static void de_cop1l(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    int func = (int)mipsGetFunction(i_inst);
    switch (func) {
        case 0x21: /* CVT.S.L fd, fs - 
                    * Floating Point Convert to Single Floating Point */
            in_fmt_fd_fs(buffer, "CVT.S", i_inst);
            break;
        case 0x22: /* CVT.D.L fd, fs - 
                    * Floating Point Convert to Double Floating Point */
            in_fmt_fd_fs(buffer, "CVT.D", i_inst);
            break;
        case 0x26: /* CVT.PS.PW */
            sprintf(buffer, "CVT.PS.PW");
            break;
        default:
            sprintf(buffer, "???");
    }
}

static void de_cop1ps(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    int func = (int)mipsGetFunction(i_inst);
    switch (func) {
        case 0x00: /* ADD.PS fd, fs, ft - Floating Point Add */
            in_fmt_fd_fs_ft(buffer, "ADD", i_inst);
            break;
        case 0x01: /* SUB.PS fd, fs, ft - Floating Point Subtract */
            in_fmt_fd_fs_ft(buffer, "SUB", i_inst);
            break;
        case 0x02: /* MUL.PS fd, fs, ft - Floating Point Multiply */
            in_fmt_fd_fs_ft(buffer, "MUL", i_inst);
            break;
        case 0x05: /* ABS.PS fd, fs - 
                    * Floating Point Absolute Value (Paired Single) */
            in_fmt_fd_fs(buffer, "ABS", i_inst);
            break;
        case 0x06: /* MOV.PS fd, fs - Floating Point Move (Paired Single) */
            in_fmt_fd_fs(buffer, "MOV", i_inst);
            break;
        case 0x07: /* NEG.PS fd, fs - Floating Point Negate (Paired Single) */
            in_fmt_fd_fs(buffer, "NEG", i_inst);
            break;
        case 0x11: /* MOVCF.PS */
            mipsInMOVCFfmt(buffer, i_inst);
            break;
        case 0x12: /* MOVZ.PS fd, fs, rt - 
                    * Floating Point Move Conditional on Zero */
            in_fmt_fd_fs_rt(buffer, "MOVZ", i_inst);
            break;
        case 0x13: /* MOVN.PS fd, fs, rt - 
                    * Floating Point Move Conditional on Not Zero */
            in_fmt_fd_fs_rt(buffer, "MOVN", i_inst);
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
            in_fd_fs(buffer, "CVT.S.PU", i_inst);
            break;
        case 0x24: /* CVT.PW.PS */
            sprintf(buffer, "CVT.PW.PS");
            break;
        case 0x28: /* CVT.S.PL fd, fs - 
                    * Floating Point Convert Pair Lower to Single Floating Point
                    */
            in_fd_fs(buffer, "CVT.S.PL", i_inst);
            break;
        case 0x2C: /* PLL.PS fd, fs, ft - Pair Lower Lower */
            in_fd_fs_ft(buffer, "PLL.PS", i_inst);
            break;
        case 0x2D: /* PLU.PS fd, fs, ft - Pair Lower Upper */
            in_fd_fs_ft(buffer, "PLU.PS", i_inst);
            break;
        case 0x2E: /* PUL.PS fd, fs, ft - Pair Upper Lower */
            in_fd_fs_ft(buffer, "PUL.PS", i_inst);
            break;
        case 0x2F: /* PUU.PS fd, fs, ft - Pair Upper Upper */
            in_fd_fs_ft(buffer, "PUU.PS", i_inst);
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

static void de_cop1s(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    int func = (int)mipsGetFunction(i_inst);
    switch (func) {
        case 0x00: /* ADD.S fd, fs, ft - Floating Point Add (Single) */
            in_fmt_fd_fs_ft(buffer, "ADD", i_inst);
            break;
        case 0x01: /* SUB.S fd, fs, ft - Floating Point Subtract (Single) */
            in_fmt_fd_fs_ft(buffer, "SUB", i_inst);
            break;
        case 0x02: /* MUL.S fd, fs, ft - Floating Point Multiply (Single) */
            in_fmt_fd_fs_ft(buffer, "MUL", i_inst);
            break;
        case 0x03: /* DIV.S fd, fs, ft - Floating Point Divide (Single) */
            in_fmt_fd_fs_ft(buffer, "DIV", i_inst);
            break;
        case 0x04: /* SQRT.S fd, fs - Floating Point Square Root (Single) */
            in_fmt_fd_fs(buffer, "SQRT", i_inst);
            break;
        case 0x05: /* ABS.S fd, fs - Floating Point Absolute Value (Single) */
            in_fmt_fd_fs(buffer, "ABS", i_inst);
            break;
        case 0x06: /* MOV.S fd, fs - Floating Point Move (Single) */
            in_fmt_fd_fs(buffer, "MOV", i_inst);
            break;
        case 0x07: /* NEG.S fd, fs - Floating Point Negate (Single) */
            in_fmt_fd_fs(buffer, "NEG", i_inst);
            break;
        case 0x08: /* ROUND.L.S fd, fs - 
                    * Floating Point Round to Long Fixed Point (Single) */
            in_fmt_fd_fs(buffer, "ROUND.L", i_inst);
            break;
        case 0x09: /* TRUNC.L.S fd, fs - 
                    * Floating Point Truncate to Long Fixed Point (Single) */
            in_fmt_fd_fs(buffer, "TRUNC.L", i_inst);
            break;
        case 0x0A: /* CEIL.L.S fd, fs - 
                    * Floating Point Ceiling Convert to Long Fixed Point */
            in_fmt_fd_fs(buffer, "CEIL.L", i_inst);
            break;
        case 0x0B: /* FLOOR.L.S fd, fs - 
                    * Floating Point Floor Convert to Long Fixed Point */
            in_fmt_fd_fs(buffer, "FLOOR.L", i_inst);
            break;
        case 0x0C: /* ROUND.W.S fd, fs - 
                    * Floating Point Round to Word Fixed Point */
            in_fmt_fd_fs(buffer, "ROUND.W", i_inst);
            break;
        case 0x0D: /* TRUNC.W.S fd, fs - 
                    * Floating Point Truncate to Word Fixed Point */
            in_fmt_fd_fs(buffer, "TRUNC.W", i_inst);
            break;
        case 0x0E: /* CEIL.W.S fd, fs - 
                    * Floating Point Ceiling Convert to Word Fixed Point */
            in_fmt_fd_fs(buffer, "CEIL.W", i_inst);
            break;
        case 0x0F: /* FLOOR.W.S fd, fs - 
                    * Floating Point Floor Convert to Word Fixed Point */
            in_fmt_fd_fs(buffer, "FLOOR.W", i_inst);
            break;
        case 0x11: /* MOVCF.S */
            mipsInMOVCFfmt(buffer, i_inst);
            break;
        case 0x12: /* MOVZ.S fd, fs, rt - 
                    * Floating Point Move Conditional on Zero */
            in_fmt_fd_fs_rt(buffer, "MOVZ", i_inst);
            break;
        case 0x13: /* MOVN.S fd, fs, rt - 
                    * Floating Point Move Conditional on Not Zero */
            in_fmt_fd_fs_rt(buffer, "MOVN", i_inst);
            break;
        case 0x15: /* RECIP.S fd, fs - Reciprocal Approximation */
            in_fmt_fd_fs(buffer, "RECIP", i_inst);
            break;
        case 0x16: /* RSQRT.S fd, fs - Reciprocal Square Root Approximation */
            in_fmt_fd_fs(buffer, "RSQRT", i_inst);
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
            in_fmt_fd_fs(buffer, "CVT.D", i_inst);
            break;
        case 0x24: /* CVT.W.S fd, fs - 
                    * Floating Point Convert to Word Fixed Point */
            in_fmt_fd_fs(buffer, "CVT.W", i_inst);
            break;
        case 0x25: /* CVT.L.S fd, fs - 
                    * Floating Point Convert to Long Fixed Point */
            in_fmt_fd_fs(buffer, "CVT.L", i_inst);
            break;
        case 0x26: /* CVT.PS.S fd, fs, ft - 
                    * Floating Point Convert Pair to Paired Single */
            in_fd_fs_ft(buffer, "CVT.PS.S", i_inst);
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

static void de_cop1w(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    int func = (int)mipsGetFunction(i_inst);
    switch (func) {
        case 0x20: /* CVT.S.W fd, fs - 
                    * Floating Point Convert to Single Floating Point */
            in_fmt_fd_fs(buffer, "CVT.S", i_inst);
            break;
        case 0x21: /* CVT.D.W fd, fs - 
                    * Floating Point Convert to Double Floating Point */
            in_fmt_fd_fs(buffer, "CVT.D", i_inst);
            break;
        case 0x26: /* CVT.PS.PW */
            sprintf(buffer, "CVT.PS.PW");
            break;
        default:
            sprintf(buffer, "???");
    }
}

static void de_cop2(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    int rs = (int)get_operand0(i_inst);
    switch (rs) {
        case 0x00: /* MFC2 rt, rd[, sel] - Move from Coprocessor 2 */
            in_rt_rd_sel(buffer, "MFC2", i_inst);
            break;
        case 0x02: /* CFC2 rt, fs - Move Control Word From Floating Point */
            in_rt_fs(buffer, "CFC2", i_inst);
            break;
        case 0x03: /* MFHC2 rt, rd - 
                    * Move Word From High Half of Coprocessor 2 Register */
            in_rt_rd(buffer, "MFHC2", i_inst);
            break;
        case 0x04: /* MTC2 rt, rd - Move Word to Coprocessor 2 */
            in_rt_rd(buffer, "MTC2", i_inst);
            break;
        case 0x06: /* CTC2 rt, rd - Move Control Word to Coprocessor 2 */
            in_rt_rd(buffer, "CTC2", i_inst);
            break;
        case 0x07: /* MTHC2 rt, rd - 
                    * Move Word to High Half of Coprocessor 2 Register */
            in_rt_rd(buffer, "MTHC2", i_inst);
            break;
        case 0x08: /* BC2 */
            mipsInBC2(buffer, i_inst, i_addr);
            break;
        default:
            sprintf(buffer, "???");
    }
}

static void de_cop1x(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    int func = (int)mipsGetFunction(i_inst);
    switch (func) {
        case 0x00: /* LWXC1 fd, index(base) - 
                    * Load Word Indexed to Floating Point */
            in_fd_index_base(buffer, "LWXC1", i_inst);
            break;
        case 0x01: /* LDXC1 fd, index(base) - 
                    * Load Doubleword Indexed to Floating Point */
            in_fd_index_base(buffer, "LDXC1", i_inst);
            break;
        case 0x05: /* LUXC1 fd, index(base) - 
                    * Load Doubleword Indexed Unaligned to Floating Point */
            in_fd_index_base(buffer, "LUXC1", i_inst);
            break;
        case 0x08: /* SWXC1 fs, index(base) - 
                    * Store Word Indexed from Floating Point */
            in_fs_index_base(buffer, "SWXC1", i_inst);
            break;
        case 0x09: /* SDXC1 fs, index(base) - 
                    * Store Doubleword Indexed from Floating Point */
            in_fs_index_base(buffer, "SDXC1", i_inst);
            break;
        case 0x0D: /* SUXC1 fs, index(base) - 
                    * Store Doubleword Indexed Unaligned from Floating Point */
            in_fs_index_base(buffer, "SUXC1", i_inst);
            break;
        case 0x0F: /* PREFX */
            mipsInPREFX(buffer, i_inst);
            break;
        case 0x1E: /* ALNV.PS fd, fs, ft, rs - Floating Point Align Variable */
            in_fd_fs_ft_rs(buffer, "ALNV.PS", i_inst);
            break;
        case 0x20: /* MADD.S fd, fr, fs, ft - Floating Point Multiply Add */
            in_fd_fr_fs_ft(buffer, "MADD", i_inst);
            break;
        case 0x21: /* MADD.D fd, fr, fs, ft - Floating Point Multiply Add*/
            in_fd_fr_fs_ft(buffer, "MADD", i_inst);
            break;
        case 0x26: /* MADD.PS fd, fr, fs, ft - Floating Point Multiply Add */
            in_fd_fr_fs_ft(buffer, "MADD", i_inst);
            break;
        case 0x28: /* MSUB.S fd, fr, fs, ft - Floating Point Multiply Subtract 
                    */
            in_fd_fr_fs_ft(buffer, "MSUB", i_inst);
            break;
        case 0x29: /* MSUB.D fd, fr, fs, ft - Floating Point Multiply Subtract 
                    */
            in_fd_fr_fs_ft(buffer, "MSUB", i_inst);
            break;
        case 0x2E: /* MSUB.PS fd, fr, fs, ft - Floating Point Multiply Subtract
                    */
            in_fd_fr_fs_ft(buffer, "MSUB", i_inst);
            break;
        case 0x30: /* NMADD.S fd, fr, fs, ft - 
                    * Floating Point Negative Multiply Add */
            in_fd_fr_fs_ft(buffer, "NMADD", i_inst);
            break;
        case 0x31: /* NMADD.D fd, fr, fs, ft - 
                    * Floating Point Negative Multiply Add */
            in_fd_fr_fs_ft(buffer, "NMADD", i_inst);
            break;
        case 0x36: /* NMADD.PS fd, fr, fs, ft - 
                    * Floating Point Negative Multiply Add */
            in_fd_fr_fs_ft(buffer, "NMADD", i_inst);
            break;
        case 0x38: /* NMSUB.S fd. fr. fs. ft - 
                    * Floating Point Negative Multiply Subtract */
            in_fd_fr_fs_ft(buffer, "NMSUB", i_inst);
            break;
        case 0x39: /* NMSUB.D fd. fr. fs. ft - 
                    * Floating Point Negative Multiply Subtract */
            in_fd_fr_fs_ft(buffer, "NMSUB", i_inst);
            break;
        case 0x3E: /* NMSUB.PS fd. fr. fs. ft - 
                    * Floating Point Negative Multiply Subtract */
            in_fd_fr_fs_ft(buffer, "NMSUB", i_inst);
            break;
        default:
            sprintf(buffer, "???");
    }
}

static void de_regimm(char *buffer, unsigned int i_inst,
        unsigned int i_addr) {
    int rt = (int)get_operand1(i_inst);
    switch (rt) {
        case 0x00: /* BLTZ rs, offset - Branch on Less Than Zero */
            in_brs_offset(buffer, "BLTZ", i_inst, i_addr);
            break;
        case 0x01: /* BGEZ rs, offset - 
                    * Branch on Greater Than or Equal to Zero */
            in_brs_offset(buffer, "BGEZ", i_inst, i_addr);
            break;
        case 0x02: /* BLTZL rs, offset - Branch on Less Than Zero Likely */
            in_brs_offset(buffer, "BLTZL", i_inst, i_addr);
            break;
        case 0x03: /* BGEZL rs, offset - 
                    * Branch on Greater Than or Equal to Zero Likely */
            in_brs_offset(buffer, "BGEZL", i_inst, i_addr);
            break;
        case 0x08: /* TGEI rs, immediate - Trap if Greater or Equal Immediate */
            in_rs_imm(buffer, "TGEI", i_inst);
            break;
        case 0x09: /* TGEIU rs, immediate - 
                    * Trap if Greater or Equal Immediate Unsigned */
            in_rs_imm(buffer, "TGEIU", i_inst);
            break;
        case 0x0A: /* TLTI rs, immediate - Trap if Less Than Immediate */
            in_rs_imm(buffer, "TLTI", i_inst);
            break;
        case 0x0B: /* TLTIU rs, immediate - 
                    * Trap if Less Than Immediate Unsigned */
            in_rs_imm(buffer, "TLTIU", i_inst);
            break;
        case 0x0C: /* TEQI rs, immediate - Trap if Equal Immediate */
            in_rs_imm(buffer, "TEGI", i_inst);
            break;
        case 0x0E: /* TNEI rs, immediate - Trap if Not Equal Immediate */
            in_rs_imm(buffer, "TNEI", i_inst);
            break;
        case 0x10: /* BLTZAL rs, offset - Branch on Less Than Zero and Link */
            in_brs_offset(buffer, "BLTZAL", i_inst, i_addr);
            break;
        case 0x11: /* BGEZAL rs, offset - 
                    * Branch on Greater Than or Equal to Zero and Link */
            in_brs_offset(buffer, "BGEZAL", i_inst, i_addr);
            break;
        case 0x12: /* BLTZALL rs, offset - 
                    * Branch on Less Than Zero and Link Likely */
            in_brs_offset(buffer, "BLTZALL", i_inst, i_addr);
            break;
        case 0x13: /* BGEZALL rs, offset - 
                    * Branch on Greater Than or Equal to Zero and Link Likely */
            in_brs_offset(buffer, "BGEZALL", i_inst, i_addr);
            break;
        case 0x1F: /* SYNCI */
            mipsInSYNCI(buffer, i_inst);
            break;
        default:
            sprintf(buffer, "???");
            break;
    }
}

static void de_special(char *buffer, unsigned int i_inst,
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
            in_rd_rt_sa(buffer, "SRA", i_inst);
            break;
        case 0x04: /* SLLV rd, rt, rs - Shift Word Left Logical Variable */
            in_rd_rt_rs(buffer, "SLLV", i_inst);
            break;
        case 0x06: /* *SRLV */
            mipsInSRLV(buffer, i_inst);
            break;
        case 0x07: /* SRAV rd, rt, rs - Shift Word Right Arithmetic Variable */
            in_rd_rt_rs(buffer, "SRAV", i_inst);
            break;
        case 0x08: /* JR rs - Jump Register */
            in_rs(buffer, "JR", i_inst);
            break;
        case 0x09: /* JALR */
            mipsInJALR(buffer, i_inst);
            break;
        case 0x0A: /* MOVZ rd, rs, rt - Move Conditional on Zero */
            in_rd_rs_rt(buffer, "MOVZ", i_inst);
            break;
        case 0x0B: /* MOVN rd, rs, rt - Move Conditional on Not Zero */
            in_rd_rs_rt(buffer, "MOVN", i_inst);
            break;
        case 0x0C: /* SYSCALL - System Call */
            re_mnem(buffer, "SYSCALL");
            break;
        case 0x0D: /* BREAK - Breakpoint */
            re_mnem(buffer, "BREAK");
            break;
        case 0x0F: /* SYNC - Synchronize Shared Memory */
            re_mnem(buffer, "SYNC");
            break;
        case 0x10: /* MFHI rs - Move From HI Register */
            in_rs(buffer, "MFHI", i_inst);
            break;
        case 0x11: /* MTHI rs - Move to HI Register */
            in_rs(buffer, "MTHI", i_inst);
            break;
        case 0x12: /* MFLO rs - Move From LO Register */
            in_rs(buffer, "MFLO", i_inst);
            break;
        case 0x13: /* MTLO rs - Move to LO Register */
            in_rs(buffer, "MTLO", i_inst);
            break;
        case 0x18: /* MULT rs, rt - Multiply Word */
            in_rs_rt(buffer, "MULT", i_inst);
            break;
        case 0x19: /* MULTU rs, rt - Multiply Unsigned Word */
            in_rs_rt(buffer, "MULTU", i_inst);
            break;
        case 0x1A: /* DIV rs, rt - Divide Word */
            in_rs_rt(buffer, "DIV", i_inst);
            break;
        case 0x1B: /* DIVU rs, rt - Divide Unsigned Word */
            in_rs_rt(buffer, "DIVU", i_inst);
            break;
        case 0x20: /* ADD rd, rs, rt - Add Word */
            in_rd_rs_rt(buffer, "ADD", i_inst);
            break;
        case 0x21: /* ADDU rd, rs, rt - Add Unsigned Word */
            in_rd_rs_rt(buffer, "ADDU", i_inst);
            break;
        case 0x22: /* SUB rd, rs, rt - Subtract Word */
            in_rd_rs_rt(buffer, "SUB", i_inst);
            break;
        case 0x23: /* SUBU rd, rs, rt - Subtract Unsigned Word */
            in_rd_rs_rt(buffer, "SUBU", i_inst);
            break;
        case 0x24: /* AND rd, rs, rt - And */
            in_rd_rs_rt(buffer, "AND", i_inst);
            break;
        case 0x25: /* OR rd, rs, rt - Or */
            in_rd_rs_rt(buffer, "OR", i_inst);
            break;
        case 0x26: /* XOR rd, rs, rt - Exclusive Or */
            in_rd_rs_rt(buffer, "XOR", i_inst);
            break;
        case 0x27: /* NOR rd, rs, rt - Not Or */
            in_rd_rs_rt(buffer, "NOR", i_inst);
            break;
        case 0x2A: /* SLT rd, rs, rt - Set on Less Than */
            in_rd_rs_rt(buffer, "SLT", i_inst);
            break;
        case 0x2B: /* SLTU rd, rs, rt - Set on Less Than Unsigned */
            in_rd_rs_rt(buffer, "SLTU", i_inst);
            break;
        case 0x30: /* TGE rs, rt - Trap if Greater or Equal */
            in_rs_rt(buffer, "TGE", i_inst);
            break;
        case 0x31: /* TGEU rs, rt - Trap if Greater or Equal Unsigned */
            in_rs_rt(buffer, "TGEU", i_inst);
            break;
        case 0x32: /* TLT rs, rt - Trap if Less Than */
            in_rs_rt(buffer, "TLT", i_inst);
            break;
        case 0x33: /* TLTU rs, rt - Trap if Less Than Unsigned */
            in_rs_rt(buffer, "TLTU", i_inst);
            break;
        case 0x34: /* TEQ rs, rt - Trap if Equal */
            in_rs_rt(buffer, "TEQ", i_inst);
            break;
        case 0x36: /* TNE rs, rt - Trap if Not Equal */
            in_rs_rt(buffer, "TNE", i_inst);
            break;
        default:
            sprintf(buffer, "???");
            break;
    }
}

static void de_special2(char *buffer, unsigned int i_inst,
        unsigned int i_addr) {
    int func = (int)mipsGetFunction(i_inst);
    switch (func) {
        case 0x00: /* MADD rs, rt - Multiply and Add Word to Hi, Lo */
            in_rs_rt(buffer, "MADD", i_inst);
            break;
        case 0x01: /* MADDU rs, rt - Multiply and Add Unsigned Word to Hi, Lo */
            in_rs_rt(buffer, "MADDU", i_inst);
            break;
        case 0x02: /* MUL rd, rs, rt - Multiply Word to GPR */
            in_rd_rs_rt(buffer, "MUL", i_inst);
            break;
        case 0x04: /* MSUB rs, rt - Multiply and Subtract Word to Hi, Lo */
            in_rs_rt(buffer, "MSUB", i_inst);
            break;
        case 0x05: /* MSUBU rs, rt - 
                    * Multiply and Subtract Unsigned Word to Hi, Lo */
            in_rs_rt(buffer, "MSUBU", i_inst);
            break;
        case 0x20: /* CLZ rd, rs - Count Leading Zeros in Word */
            in_rd_rs(buffer, "CLZ", i_inst);
            break;
        case 0x21: /* CLO rd, rs - Count Leading Ones in Word */
            in_rd_rs(buffer, "CLO", i_inst);
            break;
        case 0x24: /* MFIC rt, rd - Allegrex MFIC */
            in_rt_rd(buffer, "MFIC", i_inst);
            break;
        case 0x26: /* MTIC rt, rd - Allegrex MTIC */
            in_rt_rd(buffer, "MTIC", i_inst);
            break;
        case 0x3F: /* SDBBP */
            mipsInSDBBP(buffer, i_inst);
            break;
        default:
            sprintf(buffer, "???");
            break;
    }
}

static void de_special3(char *buffer, unsigned int i_inst,
        unsigned int i_addr) {
    int func = (int)mipsGetFunction(i_inst);
    switch (func) {
        case 0x00: /* EXT rt, rs, pos, size - Extract Bit Field */
            in_rt_rs_pos_size(buffer, "EXT", i_inst);
            break;
        case 0x04: /* INS rt, rs, pos, size - Insert Bit Field */
            in_rt_rs_pos_size(buffer, "INS", i_inst);
            break;
        case 0x20: /* *BSHFL */
            de_bshfl(buffer, i_inst, i_addr);
            break;
        case 0x3B: /* RDHWR rt, rd - Read Hardware Register */
            in_rt_rd(buffer, "RDHWR", i_inst);
            break;
        default:
            sprintf(buffer, "???");
            break;
    }
}

static void de_vcop(char* buffer, unsigned int i_inst, unsigned int i_addr) {
    int sel = get_voperand0(i_inst);
    int rt = get_voperand1(i_inst);

    if (sel == 4) {
        /* VF2IN.fmt rd, rs - Vector Float to Integer - Round Near */
        in_vrd_vrs_scale(buffer, "VF2IN", i_inst);
        return;
    }
    if (sel == 5) {
        /* VI2F.fmt rd, rs, scale - Vector Integer to Float */
        in_vrd_vrs_scale(buffer, "VI2F", i_inst);
        return;
    }
    if (rt & 0x60 == 0x60) {
        /* VCST.fmt rd, a - Vector Store Constant (Single/Pair/Triple/Quad) */
        in_vrd_a(buffer, "VCST", i_inst);
        return;
    }
    switch (rt) {
        case 0x01: /* VABS.fmt rd, rs - 
                    * Vector Absolute Value (Single/Pair/Triple/Quad) */
            in_vrd_vrs(buffer, "VABS", i_inst);
            break;
        case 0x02: /* VNEG.fmt rd, rs - 
                    * Vector Negate (Single/Pair/Triple/Quad) */
            in_vrd_vrs(buffer, "VNEG", i_inst);
            break;
        case 0x03: /* VIDT.fmt rd - 
                    * Vector Load Identity (Pair/Triple/Quad) */
            in_vrd(buffer, "VIDT", i_inst);
            break;
        case 0x06: /* VZERO.fmt rd - 
                    * Set Vector Zero (Single/Pair/Triple/Quad) */
            in_vrd(buffer, "VZERO", i_inst);
            break;
        case 0x07: /* VONE.fmt rd - 
                    * Set Vector One (Single/Pair/Triple/Quad) */
            in_vrd(buffer, "VONE", i_inst);
            break;
        case 0x10: /* VRCP.fmt rs, rd - 
                    * Vector Reciprocal (Single/Pair/Triple/Quad) */
            in_vrs_vrd(buffer, "VRCP", i_inst);
            break;
        case 0x11: /* VRSQ.fmt rs, rd - 
                    * Vector Reciprocal Square Root (Single/Pair/Triple/Quad) */
            in_vrs_vrd(buffer, "VRSQ", i_inst);
            break;
        case 0x12: /* VSIN.fmt rs, rd - 
                    * Vector Sin (Single/Pair/Triple/Quad) */
            in_vrs_vrd(buffer, "VSIN", i_inst);
            break;
        case 0x13: /* VCOS.fmt rs, rd - 
                    * Vector Cosin (Single/Pair/Triple/Quad) */
            in_vrs_vrd(buffer, "VCOS", i_inst);
            break;
        case 0x14: /* VEXP2.fmt rs, rd - 
                    * Vector Exp2 (Single/Pair/Triple/Quad) */
            in_vrs_vrd(buffer, "VEXP2", i_inst);
            break;
        case 0x15: /* VLOG2.fmt rs, rd - 
                    * Vector Log2 (Single/Pair/Triple/Quad) */
            in_vrs_vrd(buffer, "VLOG2", i_inst);
            break;
        case 0x16: /* VSQRT.fmt rs, rd - 
                    * Vector Square Root (Single/Pair/Triple/Quad) */
            in_vrs_vrd(buffer, "VSQRT", i_inst);
            break;
        case 0x17: /* VASIN.fmt rs, rd - 
                    * Vector Arc Sin (Single/Pair/Triple/Quad) */
            in_vrs_vrd(buffer, "VASIN", i_inst);
            break;
        case 0x18: /* VNRCP.fmt rs, rd - 
                    * Vector Negative Reciprocal (Single/Pair/Triple/Quad) */
            in_vrs_vrd(buffer, "VNRCP", i_inst);
            break;
        case 0x1A: /* VNSIN.fmt rs, rd - 
                    * Vector Negative Sin (Single/Pair/Triple/Quad) */
            in_vrs_vrd(buffer, "VNSIN", i_inst);
            break;
        case 0x1C: /* VREXP2.fmt rs, rd - 
                    * Vector Reciprocal Exp2 (Single/Pair/Triple/Quad) */
            in_vrs_vrd(buffer, "VREXP2", i_inst);
            break;
        case 0x3C: /* VI2UC.Q rd, rs - 
                    * Vector Integer to Unsigned Char Quad */
            in_vrd_vrs(buffer, "VI2UC.Q", i_inst);
            break;
        case 0x3F: /* VI2S.fmt rd, rs - 
                    * Vector Integer to Short (Pair/Quad) */
            in_vrd_vrs(buffer, "VI2S", i_inst);
            break;
        case 0x4A: /* VSGN.fmt rd, rs - 
                    * Vector Sign (Single/Pair/Triple/Quad) */
            in_vrd_vrs(buffer, "VSGN", i_inst);
            break;
        default:
            sprintf(buffer, "???");
    }
}

static void de_vcop2(char* buffer, const SceUInt32 inst, const SceUInt32 addr) {
    int func = get_voperand0(inst);
    switch (func) {
        case 0x0: /* VADD.fmt rd, rs, rt - 
                    * Vector Add (Single/Pair/Triple/Quad) */
            in_vrd_vrs_vrt(buffer, "VADD", inst);
            break;
        case 0x7: /* VDIV.fmt rd, rs, rt - 
                    * Vector Divide (Single/Pair/Triple/Quad) */
            in_vrd_vrs_vrt(buffer, "VDIV", inst);
            break;
        default:
            sprintf(buffer, "???");
    }
}

static void de_vcop3(char* buffer, const SceUInt32 inst, const SceUInt32 addr) {
    int func = get_voperand0(inst);
    switch (func) {
        case 0x0: /* VMUL.fmt rd, rs, rt - 
                   * Vector Multiply (Single/Pair/Triple/Quad) */
            in_vrd_vrs_vrt(buffer, "VMUL", inst);
            break;
        case 0x1: /* VDOT.fmt rd, rs, rt - 
                   * Vector Dot Product (Single/Pair/Triple/Quad) */
            in_vrd_vrs_vrt(buffer, "VDOT", inst);
            break;
        case 0x4: /* VDHP.fmt rd, rs, rt - 
                   * Vector Homogenous Dot Product (Single/Pair/Triple/Quad) */
            in_vrd_vrs_vrt(buffer, "VDHP", inst);
            break;
        default:
            sprintf(buffer, "???");
    }
}

static void de_vcop4(char* buffer, const SceUInt32 inst, const SceUInt32 addr) {
    int func = get_voperand0(inst);
    switch (func) {
        case 0x0: /* VSUB.fmt rd, rs, rt - 
                   * Vector Subtract (Single/Pair/Triple/Quad) */
            in_vrd_vrs_vrt(buffer, "VSUB", inst);
            break;
        default:
            sprintf(buffer, "???");
    }
}

static void de_vcop5(char* buffer, const SceUInt32 inst, const SceUInt32 addr) {
    int func = get_voperand0(inst);
    switch (func) {
        case 0x2: /* VMIN.fmt rd, rs, rt - 
                   * Vector Minimum Value (Single/Pair/Triple/Quad) */
            in_vrd_vrs_vrt(buffer, "VMIN", inst);
            break;
        case 0x3: /* VMAX.fmt rd, rs, rt - 
                   * Vector Maximum Value (Single/Pair/Triple/Quad) */
            in_vrd_vrs_vrt(buffer, "VMAX", inst);
            break;
        default:
            sprintf(buffer, "???");
    }
}

static char* get_format(int fmt) {
    if (fmt == 0x10) {
        return "S";
    }
    if (fmt == 0x11) {
        return "D";
    }
    if (fmt == 0x14) {
        return "W";
    }
    if (fmt == 0x15) {
        return "L";
    }
    if (fmt == 0x16) {
        return "PS";
    }
    return "?";
}

static short get_imm(unsigned int i_inst) {
    short un_imm;
    un_imm = (short)(i_inst & 0xFFFF);
    return un_imm;
}

static int get_instr_index(unsigned int i_inst) {
    unsigned int index = 0;
    index = i_inst & 0x03FFFFFF;
    return index;
}

static unsigned char get_operand0(unsigned int i_inst) {
    unsigned char uc_oper;
    uc_oper = (unsigned char)(((i_inst & 0x03E00000) >> 21) & 0x1F);
    return uc_oper;
}

static unsigned char get_operand1(unsigned int i_inst) {
    unsigned char uc_oper;
    uc_oper = (unsigned char)(((i_inst & 0x001F0000) >> 16) & 0x1F);
    return uc_oper;
}

static unsigned char get_operand2(unsigned int i_inst) {
    unsigned char uc_oper;
    uc_oper = (unsigned char)(((i_inst & 0x0000F800) >> 11) & 0x1F);
    return uc_oper;
}

static unsigned char get_operand3(unsigned int i_inst) {
    unsigned char uc_oper;
    uc_oper = (unsigned char)(((i_inst & 0x000007C0) >> 6) & 0x1F);
    return uc_oper;
}

static unsigned short get_uimm(unsigned int i_inst) {
    unsigned short un_val;
    un_val = (unsigned short)(i_inst & 0x0000FFFF);
    return un_val;
}

static char* get_vformat(int f, int F) {
    if (f == 0 && F == 0) {
        return "S";
    }
    if (f == 0 && F == 1) {
        return "P";
    }
    if (f == 1 && F == 0) {
        return "T";
    }
    if (f == 1 && F == 1) {
        return "Q";
    }
    return "?";
}

/* 0000 0011 1000 0000  0000 0000 0000 0000 */
static int get_voperand0(unsigned int i_inst) {
    int val = (i_inst & 0x03800000) >> 23;
    return val;
}

static int get_voperand1(unsigned int i_inst) {
    int val = (i_inst & 0x007F0000) >> 16;
    return val;
}

static int get_voperand2(unsigned int i_inst) {
    int val = (i_inst & 0x00008000) >> 15;
    return val;
}

static int get_voperand3(unsigned int i_inst) {
    int val = (i_inst & 0x00007F00) >> 8;
    return val;
}

static int get_voperand4(unsigned int i_inst) {
    int val = (i_inst & 0x00000080) >> 7;
    return val;
}

static int get_voperand5(unsigned int i_inst) {
    int val = i_inst & 0x0000007F;
    return val;
}

static void in_brs_offset(char* buffer, const char* mnem, unsigned int inst,
        unsigned int address) {
    int rs = (int)get_operand0(inst);
    short offset = get_imm(inst);
    unsigned int dest = (address + 4 + (offset << 2)) & 0x0FFFFFFF;
    re_gr_target(buffer, mnem, rs, dest);
}

static void in_brs_rt_offset(char* buffer, const char* mnem, unsigned int inst,
        unsigned int address)
{
    int rs = (int)get_operand0(inst);
    int rt = (int)get_operand1(inst);
    short offset = get_imm(inst);
    unsigned int dest = (address & 0x0FFFFFFF) + 4 + (offset << 2);
    re_gr_gr_target(buffer, mnem, rs, rt, dest);
}

static void in_fd_fr_fs_ft(char* buffer, const char* mnem, unsigned int inst) {
    int fd = (int)get_operand0(inst);
    int fr = (int)get_operand0(inst);
    int fs = (int)get_operand0(inst);
    int ft = (int)get_operand0(inst);
    int fmt = (inst & 0x7);
    char fmnem[10];
    char *sfmt = get_format(fmt);
    sprintf(fmnem, "%s.%s", mnem, sfmt);
    re_fr_fr_fr_fr(buffer, fmnem, fd, fr, fs, ft);
}

static void in_fd_fs(char* buffer, const char* mnem, unsigned int inst) {
    int fs = (int)get_operand2(inst);
    int fd = (int)get_operand3(inst);
    re_fr_fr(buffer, mnem, fd, fs);
}

static void in_fd_fs_ft(char* buffer, const char* mnem, unsigned int inst) {
    int ft = (int)get_operand0(inst);
    int fs = (int)get_operand1(inst);
    int fd = (int)get_operand2(inst);
    re_fr_fr_fr(buffer, mnem, fd, fs, ft);
}

static void in_fd_fs_ft_rs(char* buffer, const char* mnem, unsigned int inst) {
    int rs = (int)get_operand0(inst);
    int ft = (int)get_operand1(inst);
    int fs = (int)get_operand2(inst);
    int fd = (int)get_operand3(inst);
    re_fr_fr_fr_gr(buffer, mnem, fd, fs, ft, rs);
}

static void in_fd_index_base(char* buffer, const char* mnem, unsigned int inst) 
{
    int base = (int)get_operand0(inst);
    int index = (int)get_operand1(inst);
    int fd = (int)get_operand3(inst);
    re_fr_index_base(buffer, mnem, fd, index, base);
}

static void in_fmt_fd_fs(char* buffer, const char* mnem, unsigned int inst) {
    int fmt = (int)get_operand0(inst);
    int fs = (int)get_operand2(inst);
    int fd = (int)get_operand3(inst);
    char* sfmt = get_format(fmt);
    char fmnem[9];
    sprintf(fmnem, "%s.%s", mnem, sfmt);
    re_fr_fr(buffer, fmnem, fd, fs);
}

static void in_fmt_fd_fs_ft(char* buffer, const char* mnem, unsigned int inst) {
    int fmt = (int)get_operand0(inst);
    int ft = (int)get_operand1(inst);
    int fs = (int)get_operand2(inst);
    int fd = (int)get_operand3(inst);
    char fmnem[9];
    char *sfmt = get_format(fmt);
    sprintf(fmnem, "%s.%s", mnem, sfmt);
    re_fr_fr_fr(buffer, fmnem, fd, fs, ft);
}

static void in_fmt_fd_fs_rt(char* buffer, const char* mnem, unsigned int inst) {
    int fmt = (int)get_operand0(inst);
    int rt = (int)get_operand1(inst);
    int fs = (int)get_operand2(inst);
    int fd = (int)get_operand3(inst);
    char fmnem[10];
    char* sfmt = get_format(fmt);
    sprintf(fmnem, "%s.%s", mnem, sfmt);
    re_fr_fr_gr(buffer, fmnem, fd, fs, rt);
}

static void in_fs_index_base(char* buffer, const char* mnem, unsigned int inst) 
{
    int base = (int)get_operand0(inst);
    int index = (int)get_operand1(inst);
    int fs = (int)get_operand2(inst);
    re_fr_index_base(buffer, mnem, fs, index, base);
}

static void in_ft_offset_base(char* buffer, const char* mnem, unsigned int inst)
{
    int base = (int)get_operand0(inst);
    int ft = (int)get_operand1(inst);
    short offset = get_imm(inst);
    re_fr_offset_base(buffer, mnem, base, ft, offset);
}

static void in_rd_rs(char* buffer, const char* mnem, unsigned int i_inst) {
    int rs = (int)get_operand0(i_inst);
    int rd = (int)get_operand2(i_inst);
    re_gr_gr(buffer, mnem, rd, rs);
}

static void in_rd_rs_rt(char* buffer, const char* mnem, unsigned int inst) {
    int rs = (int)get_operand0(inst);
    int rt = (int)get_operand1(inst);
    int rd = (int)get_operand2(inst);
    re_gr_gr_gr(buffer, mnem, rd, rs, rt);
}

static void in_rd_rt(char* buffer, const char* mnem, unsigned int inst) {
    int rt = (int)get_operand1(inst);
    int rd = (int)get_operand2(inst);
    re_gr_gr(buffer, mnem, rd, rt);
}

static void in_rd_rt_rs(char* buffer, const char* mnem, unsigned int inst) {
    int rs = (int)get_operand0(inst);
    int rt = (int)get_operand1(inst);
    int rd = (int)get_operand2(inst);
    re_gr_gr_gr(buffer, mnem, rd, rs, rt);
}

static void in_rd_rt_sa(char* buffer, const char* mnem, unsigned int inst) {
    int rt = (int)get_operand1(inst);
    int rd = (int)get_operand2(inst);
    int sa = (int)get_operand3(inst);
    re_gr_gr_sa(buffer, mnem, rd, rt, sa);
}

static void in_rs(char* buffer, const char* mnem, unsigned int inst) {
    int rs = (int)get_operand0(inst);
    re_gr(buffer, mnem, rs);
}

static void in_rs_imm(char* buffer, const char* mnem, unsigned int inst) {
    int rs = (int)get_operand0(inst);
    short imm = get_imm(inst);
    re_gr_imm(buffer, mnem, rs, imm);
}

static void in_rs_rt(char* buffer, const char* mnem, unsigned int inst) {
    int rs = (int)get_operand0(inst);
    int rt = (int)get_operand1(inst);
    re_gr_gr(buffer, mnem, rs, rt);
}

static void in_rs_rt_imm(char* buffer, const char* mnem, unsigned int inst) {
    int rs = (int)get_operand0(inst);
    int rt = (int)get_operand1(inst);
    short imm = get_imm(inst);
    re_gr_gr_imm(buffer, mnem, rs, rt, imm);
}

static void in_rt_fs(char* buffer, const char* mnem, unsigned int inst) {
    int rt = (int)get_operand1(inst);
    int fs = (int)get_operand2(inst);
    re_gr_fr(buffer, mnem, rt, fs);
}

static void in_rt_imm(char* buffer, const char* mnem, unsigned int inst) {
    int rt = (int)get_operand1(inst);
    short imm = get_imm(inst);
    re_gr_imm(buffer, mnem, rt, imm);
}

static void in_rt_offset_base(char* buffer, const char* mnem, unsigned int inst)
{
    int base = (int)get_operand0(inst);
    int rt = (int)get_operand1(inst);
    short offset = get_imm(inst);
    re_gr_offset_base(buffer, mnem, base, rt, offset);
}

static void in_rt_rd(char* buffer, const char* mnem, unsigned int inst) {
    int rt = (int)get_operand1(inst);
    int rd = (int)get_operand2(inst);
    re_gr_gr(buffer, mnem, rt, rd);
}

static void in_rt_rd_sel(char* buffer, const char* mnem, unsigned int inst) {
    int rt = (int)get_operand1(inst);
    int rd = (int)get_operand2(inst);
    int sel = inst & 0x00000007;
    if (sel == 0) {
        re_gr_gr(buffer, mnem, rt, rd);
    } else {
        re_gr_gr_sel(buffer, mnem, rt, rd, sel);
    }
}

static void in_rt_rs_imm(char* buffer, const char* mnem, unsigned int inst) {
    int rs = (int)get_operand0(inst);
    int rt = (int)get_operand1(inst);
    unsigned short imm = get_uimm(inst);
    re_gr_gr_imm(buffer, mnem, rs, rt, imm);
}

static void in_rt_rs_pos_size(char* buffer, const char* mnem, unsigned int inst)
{
    int rs = (int)get_operand0(inst);
    int rt = (int)get_operand1(inst);
    int size = (int)get_operand2(inst) + 1;
    int pos = (int)get_operand3(inst);
    re_gr_gr_pos_size(buffer, mnem, rt, rs, pos, size);
}

static void in_target(char* buffer, const char* mnem, unsigned int inst, 
        unsigned int address) {
    unsigned int instr_index = ((inst & 0x03FFFFFF) << 2);
    unsigned int dest = ((address + 4) & 0x0C000000) | instr_index;
    re_target(buffer, mnem, dest);
}

static void in_vrd(char* buffer, const char* mnem, unsigned int inst) {
    int f = get_voperand2(inst);
    int F = get_voperand4(inst);
    int rd = get_voperand5(inst);
    char fmnem[10];
    char* sfmt = get_vformat(f, F);
    sprintf(fmnem, "%s.%s", mnem, sfmt);
    re_vr(buffer, fmnem, rd);
}

static void in_vrd_a(char* buffer, const char* mnem, unsigned int inst) {
    int op1 = get_voperand1(inst);
    int a = op1 & 0x1F;
    int f = get_voperand2(inst);
    int F = get_voperand4(inst);
    int rd = get_voperand5(inst);
    char fmnem[10];
    char *sfmt = get_vformat(f, F);
    sprintf(fmnem, "%s.%s", mnem, sfmt);
    re_vr_a(buffer, fmnem, rd, a);
}

static void in_vrd_vrs(char* buffer, const char* mnem, unsigned int inst) {
    int f = get_voperand2(inst);
    int rs = get_voperand3(inst);
    int F = get_voperand4(inst);
    int rd = get_voperand5(inst);
    char fmnem[10];
    char *sfmt = get_vformat(f, F);
    sprintf(fmnem, "%s.%s", mnem, sfmt);
    re_vr_vr(buffer, fmnem, rd, rs);
}

static void in_vrd_vrs_scale(char* buffer, const char* mnem, unsigned int inst)
{
    int scale = get_voperand1(inst);
    int f = get_voperand2(inst);
    int rs = get_voperand3(inst);
    int F = get_voperand4(inst);
    int rd = get_voperand5(inst);
    char fmnem[10];
    char *sfmt = get_vformat(f, F);
    sprintf(fmnem, "%s.%s", mnem, sfmt);
    re_vr_vr_scale(buffer, fmnem, rd, rs, scale);
}

static void in_vrd_vrs_vrt(char* buffer, const char* mnem, unsigned int inst) {
    int rt = get_voperand1(inst);
    int f = get_voperand2(inst);
    int rs = get_voperand3(inst);
    int F = get_voperand4(inst);
    int rd = get_voperand5(inst);
    char fmnem[10];
    char *sfmt = get_vformat(f, F);
    sprintf(fmnem, "%s.%s", mnem, sfmt);
    re_vr_vr_vr(buffer, fmnem, rd, rs, rt);
}

static void in_vrs_vrd(char* buffer, const char* mnem, unsigned int inst) {
    int f = get_voperand2(inst);
    int rs = get_voperand3(inst);
    int F = get_voperand4(inst);
    int rd = get_voperand5(inst);
    char fmnem[10];
    char *sfmt = get_vformat(f, F);
    sprintf(fmnem, "%s.%s", mnem, sfmt);
    re_vr_vr(buffer, fmnem, rs, rd);
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
    int op1 = (int)get_operand1(i_inst);
    int offset = (int)get_imm(i_inst);
    int cc = (op1 & 0x1C) >> 2;
    int nd = (op1 & 2) >> 1;
    int tf = op1 & 1;
    unsigned int dest = (i_addr & 0x0FFFFFFF) + 4 + (offset << 2);
    if (tf == 0) {
        if (nd == 0) {
            mnem = "BC1F";
        } else {
            mnem = "BC1FL";
        }
    } else {
        if (nd == 0) {
            mnem = "BC1T";
        } else {
            mnem = "BC1TL";
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
    int op1 = (int)get_operand1(i_inst);
    int offset = (int)get_imm(i_inst);
    int cc = (op1 & 0x1C) >> 2;
    int nd = (op1 & 2) >> 1;
    int tf = op1 & 1;
    unsigned int dest = (i_addr & 0x0FFFFFFF) + 4 + (offset << 2);
    if (tf == 0) {
        if (nd == 0) {
            mnem = "BC2F";
        } else {
            mnem = "BC2FL";
        }
    } else {
        if (nd == 0) {
            mnem = "BC2T";
        } else {
            mnem = "BC2TL";
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
    const char *mnem = "CACHE";
    int i_base = (int)get_operand0(i_inst);
    int i_op = (int)get_operand1(i_inst);
    short offset = get_imm(i_inst);
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
    int fmt = (int)get_operand0(i_inst);
    int ft = (int)get_operand1(i_inst);
    int fs = (int)get_operand2(i_inst);
    int op3 = (int)get_operand3(i_inst);
    int op4 = (int)mipsGetFunction(i_inst);
    int cc = (op3 & 0x1C) >> 2;
    int a = (op3 & 1);
    int cond = op4 & 0xF;
    char *sfmt;
    char *scond;
    char mnem[9];
    
    switch (cond) {
        case 0:
            scond = "F";
            break;
        case 1:
            scond = "UN";
            break;
        case 2:
            scond = "EQ";
            break;
        case 3:
            scond = "UEQ";
            break;
        case 4:
            scond = "OLT";
            break;
        case 5:
            scond = "ULT";
            break;
        case 6:
            scond = "OLE";
            break;
        case 7:
            scond = "ULE";
            break;
        case 8:
            scond = "SF";
            break;
        case 9:
            scond = "NGLE";
            break;
        case 0xA:
            scond = "SEQ";
            break;
        case 0xB:
            scond = "NGL";
            break;
        case 0xC:
            scond = "LT";
            break;
        case 0xD:
            scond = "NGE";
            break;
        case 0xE:
            scond = "LE";
            break;
        case 0xF:
            scond = "NGT";
            break;
    }
    sfmt = get_format(fmt);
    sprintf(mnem, "C.%s.%s", scond, sfmt);
    if (cc == 0) {
        re_fr_fr(buffer, mnem, fs, ft);
    } else {
        re_cc_fr_fr(buffer, mnem, cc, fs, ft);
    }
}

/* mipsInJALR
 *  Jump and Link Register
 *  JALR rs (rd = 31 implied)
 *  JALR rd, rs
 *  [000000][-----][00000][-----][-----][001001]
 * [SPECIAL][rs   ][0    ][rd   ][hint ][JALR  ]
 */
static void mipsInJALR(char *buffer, unsigned int i_inst) {
    const char *mnem = "JALR";
    int rs = (int)get_operand0(i_inst);
    int rd = (int)get_operand2(i_inst);
    if (rd == 31) {
        re_gr(buffer, mnem, rs);
    } else {
        re_gr_gr(buffer, mnem, rd, rs);
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
    int rt = (int)get_operand1(i_inst);
    int sc = (i_inst & 0x20) >> 5;
    if (sc == 0) {
        re_gr(buffer, "DI", rt);
    } else {
        re_gr(buffer, "EI", rt);
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
    int rs = (int)get_operand0(i_inst);
    int o1 = (int)get_operand1(i_inst);
    int cc = (o1 & 0x1C) >> 2;
    int tf = (o1 & 0x01);
    int rd = (int)get_operand2(i_inst);
    if (tf == 1) {
        re_gr_gr_cc(buffer, "MOVT", rd, rs, cc);
    } else {
        re_gr_gr_cc(buffer, "MOVF", rd, rs, cc);
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
    const char *mn = "MOV";
    char *cond;
    char *format;
    char mnem[10];
    int fmt = (int)get_operand0(i_inst);
    int op1 = (int)get_operand1(i_inst);
    int fs = (int)get_operand2(i_inst);
    int fd = (int)get_operand3(i_inst);
    int cc = (op1 & 0x1C) >> 2;
    int tf = op1 & 1;
    format = get_format(fmt);
    if (tf == 1) {
        cond = "T";
    } else {
        cond = "F";
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
    const char *mnem = "MTC0";
    int rt = (int)get_operand1(i_inst);
    int rd = (int)get_operand2(i_inst);
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
    const char *mnem = "PREF";
    int base = (int)get_operand0(i_inst);
    int hint = (int)get_operand1(i_inst);
    short offset = get_imm(i_inst);
    re_op_offset_base(buffer, mnem, base, hint, offset);
}

/* mipsInPREFX
 *  Prefetch Indexed
 *  PREFX hint, index(base)
 *  [010011][-----][-----][-----][00000][001111]
 *  [COP1X ][base ][index][hint ][0    ][PREFX ]
 */
static void mipsInPREFX(char *buffer, unsigned int i_inst) {
    const char *mnem = "PREFX";
    int base = (int)get_operand0(i_inst);
    int index = (int)get_operand1(i_inst);
    int hint = (int)get_operand2(i_inst);
    re_op_offset_base(buffer, mnem, base, hint, index);
}

/* mipsInSDBBP
 *  Software Debug Breakpoint
 *  SDBBP code
 *  [011100]  [--------------------][111111]
 *  [SPECIAL2][code                ][SDBBP ]
 */
static void mipsInSDBBP(char *buffer, unsigned int i_inst) {
    const char *mnem = "SDBBP";
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
    int rt = (int)get_operand1(i_inst);
    int rd = (int)get_operand2(i_inst);
    int sa = (int)get_operand3(i_inst);
    if (rt == 0 && rd == 0) {
        switch(sa) {
            case 0:
                re_mnem(buffer, "NOP");
                break;
            case 1:
                re_mnem(buffer, "SSNOP");
                break;
            case 3:
                re_mnem(buffer, "EHB");
                break;
        }
        return;
    }
    re_gr_gr_sa(buffer, "SLL", rd, rt, sa);
}

/* mipsInSRL
 *  Shift Word Right Logical
 *  SRL rd, rt, sa
 *  ROTR rd, rt, sa
 *  [000000][0000][0][-----][-----][-----][000010]
 * [SPECIAL][0   ][R][rt   ][rd   ][sa   ][SRL   ]
 */
static void mipsInSRL(char *buffer, unsigned int i_inst) {
    int i_rt = (int)get_operand1(i_inst);
    int i_rd = (int)get_operand2(i_inst);
    int i_sa = (int)get_operand3(i_inst);
    int o0 = (int)get_operand0(i_inst);
    int r = (o0 & 0x1);
    if (r == 1) {
        re_gr_gr_sa(buffer, "ROTR", i_rd, i_rt, i_sa);
    } else {
        re_gr_gr_sa(buffer, "SRL", i_rd, i_rt, i_sa);
    }
}

/* mipsInSRLV
 *  Shift Word Right Logical Variable
 *  SRLV rd, rt, rs
 *  [000000][-----][-----][-----][0000][0][000110]
 * [SPECIAL][rs   ][rt   ][rd   ][0   ][R][SRLV  ]
 */
static void mipsInSRLV(char *buffer, unsigned int i_inst) {
    int rs = (int)get_operand0(i_inst);
    int rt = (int)get_operand1(i_inst);
    int rd = (int)get_operand2(i_inst);
    int o3 = (int)get_operand3(i_inst);
    int r = o3 & 1;
    if (r == 1) {
        re_gr_gr_gr(buffer, "ROTRV", rd, rs, rt);
    } else {
        re_gr_gr_gr(buffer, "SRLV", rd, rs, rt);
    }
}

/* mipsInSYNCI
 *  Syncronize Caches to Make Instruction Writes Effective
 *  SYNCI offset(base)
 *  [000001][-----][11111][----------------]
 *  [REGIMM][base ][SYNCI][offset          ]
 */
static void mipsInSYNCI(char *buffer, unsigned int i_instr) {
    const char *mnem = "SYNCI";
    int base = (int)get_operand0(i_instr);
    short offset = get_imm(i_instr);
    re_offset_base(buffer, mnem, offset, base);
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

/** Render a Type 'CC, DEST' instruction.
 * [MNEM] cc, dest
 * 
 * @param buffer string acting as the destination.
 * @param mnem string containing the mnemonic.
 * @param cc int containing the condition code.
 * @param dest unsigned int containing the destination address.
 */
static void re_cc_target(char *buffer, const char *mnem, int cc, 
        unsigned int dest) {
    sprintf(buffer, "%s %d, 0x%08X", mnem, cc, dest);
}

/** Render a Type 'FPR, FPR' instruction.
 * [MNEM] fd, fs
 * 
 * @param buffer string acting as the destination.
 * @param mnem string containing the mnemonic.
 * @param i_fd int indicating the destination FP register.
 * @param i_fs int indicating the source FP register.
 */
static void re_fr_fr(char *buffer, const char *mnem, int i_fd, int i_fs) {
    const char *fd = mipsFpRegNames[i_fd];
    const char *fs = mipsFpRegNames[i_fs];
    sprintf(buffer, "%s $%s, $%s", mnem, fd, fs);
}

/** Render a Type 'FPR, FPR, CC' instruction.
 * [MNEM] fd, fs, cc
 * 
 * @param buffer string acting as the destination.
 * @param mnem string containing the mnemonic.
 * @param i_fd int indicating the destination FP register.
 * @param i_fs int indicating the source FP register.
 * @param i_cc int indicating the condition code.
 */
static void re_fr_fr_cc(char *buffer, const char *mnem, int i_fd, int i_fs, int i_cc) {
    const char *fd = mipsFpRegNames[i_fd];
    const char *fs = mipsFpRegNames[i_fs];
    sprintf(buffer, "%s $%s, $%s, %d", mnem, fd, fs, i_cc);
}

/** Render a Type 'FPR, FPR, FPR' instruction.
 * [MNEM] fd, fs, ft
 * 
 * @param buffer string acting as the destination.
 * @param mnem string containing the mnemonic.
 * @param iFd int indicating the destination FP register.
 * @param iFs int indicating the source FP register.
 * @param iFt int indicating the target FP register.
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

/** Render a Type 'GPR' instruction.
 * [MNEM] rs
 * 
 * @param buffer string acting as the destination.
 * @param mnem string containing the mnemonic.
 * @param i_rs int indicating the source GP register.
 */
static void re_gr(char *buffer, const char *mnem, int i_rs) {
    const char *rs = mipsRegisterArray[i_rs];
    sprintf(buffer, "%s $%s", mnem, rs);
}

/** Render a Type 'GPR, FPR' instruction.
 * [MNEM] rt, fs
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

/** Render a Type 'GPR, GPR' instruction.
 * [MNEM] rs, rt
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

/** Render a Type 'GPR, GPR, CC' instruction.
 * [MNEM] rd, rs, cc
 * 
 * @param buffer string acting as the destination.
 * @param mnem string containing the mnemonic.
 * @param i_rd int indicating the destination GP register.
 * @param i_rs int indicating the source GP register.
 * @param i_cc int indicating the condition code.
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

/* mipsType0
 *  Render a Type 0 instruction.
 *  [MNEM]
 */
static void re_mnem(char *buffer, const char *mnem) {
    sprintf(buffer, "%s", mnem);
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

static void re_vr(char* buffer, const char* mnem, int iRd) {
    sprintf(buffer, "%s $v%d", mnem, iRd);
}

static void re_vr_a(char* buffer, const char* mnem, int iRd, int iA) {
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

static void re_vr_vr(char* buffer, const char* mnem, int iRd, int iRs) {
    sprintf(buffer, "%s $v%d, $v%d", mnem, iRd, iRs);
}

static void re_vr_vr_scale(char* buffer, const char* mnem,
        int iRd, int iRs, int scale) {
    sprintf(buffer, "%s $v%d, $v%d, %d", mnem, iRd, iRs, scale);
}

static void re_vr_vr_vr(char* buffer, const char* mnem,
        int iRd, int iRs, int iRt) {
    sprintf(buffer, "%s $v%d, $v%d, $v%d", mnem, iRd, iRs, iRt);
}

void mipsDecode(char *buffer, unsigned int i_inst, unsigned int i_addr) {
    unsigned char uc_opcode;

    uc_opcode = mipsGetOpCode(i_inst);
    switch (uc_opcode) {
        case 0: /* SPECIAL */
            de_special(buffer, i_inst, i_addr);
            break;
        case 1: /* REGIMM */
            de_regimm(buffer, i_inst, i_addr);
            break;
        case 0x02: /* J target - Jump */
            in_target(buffer, "J", i_inst, i_addr);
            break;
        case 0x03: /* JAL target - Jump and Link */
            in_target(buffer, "JAL", i_inst, i_addr);
            break;
        case 0x04: /* BEQ rs, rt, offset - Branch on Equal */
            in_brs_rt_offset(buffer, "BEQ", i_inst, i_addr);
            break;
        case 0x05: /* BNE rs, rt, offset - Branch on Not Equal */
            in_brs_rt_offset(buffer, "BNE", i_inst, i_addr);
            break;
        case 0x06: /* BLEZ rs, offset - Branch on Less Than or Equal to Zero */
            in_brs_offset(buffer, "BLEZ", i_inst, i_addr);
            break;
        case 0x07: /* BGTZ rs, offset - Branch on Greater Than Zero */
            in_brs_offset(buffer, "BGTZ", i_inst, i_addr);
            break;
        case 0x08: /* ADDI rt, rs, immediate - Add Immediate */
            in_rt_rs_imm(buffer, "ADDI", i_inst);
            break;
        case 0x09: /* ADDIU rt, rs, immediate - Add Immediate Unsigned Word  */
            in_rt_rs_imm(buffer, "ADDIU", i_inst);
            break;
        case 0x0A: /* SLTI rs, rt, immediate - Set on Less Than Immediate */
            in_rs_rt_imm(buffer, "SLTI", i_inst);
            break;
        case 0x0B: /* SLTIU rs, rt, immediate - 
                    * Set on Less Than Immediate Unsigned */
            in_rs_rt_imm(buffer, "SLTIU", i_inst);
            break;
        case 0x0C: /* ANDI rt, rs, immediate - AND Immediate */
            in_rt_rs_imm(buffer, "ANDI", i_inst);
            break;
        case 0x0D: /* ORI rs, rt, immediate - OR Immediate */
            in_rs_rt_imm(buffer, "ORI", i_inst);
            break;
        case 0x0E: /* XORI rs, rt, immediate - XOR Immediate */
            in_rs_rt_imm(buffer, "XORI", i_inst);
            break;
        case 0x0F: /* LUI rt, immediate - Load Upper Immediate */
            in_rt_imm(buffer, "LUI", i_inst);
            break;
        case 0x10: /* *COP0 */
            de_cop0(buffer, i_inst, i_addr);
            break;
        case 0x11: /* *COP1 */
            de_cop1(buffer, i_inst, i_addr);
            break;
        case 0x12: /* *COP2 */
            de_cop2(buffer, i_inst, i_addr);
            break;
        case 0x13: /* *COP1X */
            de_cop1x(buffer, i_inst, i_addr);
            break;
        case 0x14: /* BEQL rs, rt, offset - Branch on Equal Likely */
            in_brs_rt_offset(buffer, "BEQL", i_inst, i_addr);
            break;
        case 0x15: /* BNEL rs, rt, offset - Branch on Not Equal Likely */
            in_brs_rt_offset(buffer, "BNEL", i_inst, i_addr);
            break;
        case 0x16: /* BLEZL rs, offset - 
                    * Branch on Less Than or Equal to Zero Likely */
            in_brs_offset(buffer, "BLEZL", i_inst, i_addr);
            break;
        case 0x17: /* BGTZL rs, offset - Branch on Greater Than Zero Likely */
            in_brs_offset(buffer, "BGTZL", i_inst, i_addr);
            break;
        case 0x18: /* VCOP Instruction-2 */
            de_vcop2(buffer, i_inst, i_addr);
            break;
        case 0x19: /* VCOP Instruction-3 */
            de_vcop3(buffer, i_inst, i_addr);
            break;
        case 0x1A: /* VCOP Instruction-4 */
            de_vcop4(buffer, i_inst, i_addr);
            break;
        case 0x1B: /* VCOP Instruction-5 */
            de_vcop5(buffer, i_inst, i_addr);
            break;
        case 0x1C: /* *SPECIAL2 */
            de_special2(buffer, i_inst, i_addr);
            break;
        case 0x1F: /* *SPECIAL3 */
            de_special3(buffer, i_inst, i_addr);
            break;
        case 0x20: /* LB rt, offset(base) - Load Byte */
            in_rt_offset_base(buffer, "LB", i_inst);
            break;
        case 0x21: /* LH rt, offset(base) - Load Halfword */
            in_rt_offset_base(buffer, "LH", i_inst);
            break;
        case 0x22: /* LWL rt, offset(base) - Load Word Left */
            in_rt_offset_base(buffer, "LWL", i_inst);
            break;
        case 0x23: /* LW rt, offset(base) - Load Word */
            in_rt_offset_base(buffer, "LW", i_inst);
            break;
        case 0x24: /* LBU rt, offset(base) - Load Byte Unsigned */
            in_rt_offset_base(buffer, "LBU", i_inst);
            break;
        case 0x25: /* LHU rt, offset(base) - Load Halfword Unsigned */
            in_rt_offset_base(buffer, "LHU", i_inst);
            break;
        case 0x26: /* LWR rt, offset(base) - Load Word Right */
            in_rt_offset_base(buffer, "LWR", i_inst);
            break;
        case 0x28: /* SB rt, offset(base) - Store Byte */
            in_rt_offset_base(buffer, "SB", i_inst);
            break;
        case 0x29: /* SH rt, offset(base) - Store Halfword */
            in_rt_offset_base(buffer, "SH", i_inst);
            break;
        case 0x2A: /* SWL rt, offset(base) - Store Word Left */
            in_rt_offset_base(buffer, "SWL", i_inst);
            break;
        case 0x2B: /* SW rt, offset(base) - Store Word */
            in_rt_offset_base(buffer, "SW", i_inst);
            break;
        case 0x2E: /* SWR rt, offset(base) - Store Word Right */
            in_rt_offset_base(buffer, "SWR", i_inst);
            break;
        case 0x2F: /* CACHE */
            mipsInCACHE(buffer, i_inst);
            break;
        case 0x30: /* LL rt, offset(base) - Load Linked Word */
            in_rt_offset_base(buffer, "LL", i_inst);
            break;
        case 0x31: /* LWC1 ft, offset(base) - Load Word to Floating Point */
            in_ft_offset_base(buffer, "LWC1", i_inst);
            break;
        case 0x32: /* LWC2 rt, offset(base) - Load Word to Coprocessor 2 */
            in_rt_offset_base(buffer, "LWC2", i_inst);
            break;
        case 0x33: /* PREF */
            mipsInPREF(buffer, i_inst);
            break;
        case 0x34: /* *VCOP */
            de_vcop(buffer, i_inst, i_addr);
            break;
        case 0x35: /* LDC1 ft, offset(base) - 
                    * Load Doubleword to Floating Point */
            in_ft_offset_base(buffer, "LDC1", i_inst);
            break;
        case 0x36: /* LV.Q rt, offset(base) - 
                    * LoadVector.Quadword Relative to Address */
            in_rt_offset_base(buffer, "LV.Q", i_inst);
            break;
        case 0x38: /* SC rt, offset(base) - Store Conditional Word */
            in_rt_offset_base(buffer, "SC", i_inst);
            break;
        case 0x39: /* SWC1 ft, offset(base) - Store Word from Floating Point */
            in_ft_offset_base(buffer, "SWC1", i_inst);
            break;
        case 0x3A: /* SWC2 rt, offset(base) - Store Word from Coprocessor 2 */
            in_rt_offset_base(buffer, "SWC2", i_inst);
            break;
        case 0x3D: /* SDC1 ft, offset(base) - 
                    * Store Doubleword from Floating Point */
            in_ft_offset_base(buffer, "SDC1", i_inst);
            break;
        case 0x3E: /* SV.Q rt, offset(base) - 
                    * StoreVector.Quadword Relative to Address */
            in_rt_offset_base(buffer, "SV.Q", i_inst);
            break;
        default:
            sprintf(buffer, "???");
            break;
    }
}
