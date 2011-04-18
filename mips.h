/* mips.h
 *  MIPS32 Instruction Decoding
 * Author: Sir Gee of Five
 * 
 */

#ifndef _MIPS_H_
#define _MIPS_H_

#include <pspkerneltypes.h>

#ifdef	__cplusplus
extern "C" {
#endif
    
#define S 0
#define T 1
#define D 2

#define MIPSFMT_S "S"
#define MIPSFMT_D "D"
#define MIPSFMT_PS "PS"

typedef enum _EOperandType {
    MOT_NONE,
    MOT_GPREG,
    MOT_FPREG,
    MOT_OFFSET,
    MOT_CC,
    MOT_TARGET,
    MOT_IMMEDIATE,
    MOT_INSTRINDEX,
    MOT_SA
} EOperandType;

typedef struct _MipsOperand {
    EOperandType operandtype;
    char *name;
    int value;
} MipsOperand;

typedef struct _MipsInstruction {
    unsigned int value;
    int opcode;
    char *mnemonic;
    int operand_count;
    MipsOperand operands[6];
    char *description;
} MipsInstruction;

static void mipsAddOperand(MipsInstruction *prIns, EOperandType otype, char *name, int value);
void mipsDecode(char *,unsigned int, unsigned int);
SceUInt32 mipsGetBranchDestination(unsigned int i_inst, unsigned int i_addr);
unsigned char mipsGetFunction(unsigned int i_inst);
unsigned char mipsGetOpCode(unsigned int);
SceUInt32 mipsGetJumpDestination(unsigned int i_inst, unsigned int i_addr);

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

static void mipsInADD(char *, unsigned int);
static void mipsInADDI(char *, unsigned int);
static void mipsInADDIU(char *, unsigned int);
static void mipsInADDU(char *, unsigned int);
static void mipsInALNVPS(char *, unsigned int);
static void mipsInAND(char *, unsigned int);
static void mipsInANDI(char *, unsigned int);

#ifdef	__cplusplus
}
#endif

#endif
// vi:cin:et:ts=4 sw=4
