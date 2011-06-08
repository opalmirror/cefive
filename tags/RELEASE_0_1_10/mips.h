/* mips.h
 *  MIPS32 Instruction Decoding
 * Author: Sir Gee of Five
 * 
 */

#ifndef MIPS_H
#define MIPS_H

#include <stdio.h>
#include <pspkerneltypes.h>

#define S 0
#define T 1
#define D 2

#define MIPSFMT_S "S"
#define MIPSFMT_D "D"
#define MIPSFMT_PS "PS"

#ifdef	__cplusplus
extern "C" {
#endif

    /** Decode a value as a MIPS32 instruction into the specified string.
     * 
     * @param buffer Pointer to a string to hold the assembly language.
     * @param i_inst The value to decode.
     * @param i_addr The address of the value.
     */
    void mipsDecode(char *buffer, unsigned int i_inst, unsigned int i_addr);
    
    /** Return a SceUInt32 containing the destination address of a branch
     * instruction.
     * 
     * @param i_inst The value of the instruction to decode.
     * @param i_addr The address of the instruction.
     * @return A SceUInt32 containing the address or 0 is returned.
     */
    SceUInt32 mipsGetBranchDestination(unsigned int i_inst, unsigned int i_addr);
    
    /** Return the function of a MIPS32 instruction.
     * 
     * @param i_inst The value of the instruction to decode.
     * @return An unsigned char containing the function is returned.
     */
    unsigned char mipsGetFunction(unsigned int i_inst);
    
    /** Return the Op Code of a MIPS32 instruction.
     * 
     * @param i_inst The value of the instruction to decode.
     * @return An unsigned char containing the Op Code is returned.
     */
    unsigned char mipsGetOpCode(unsigned int i_inst);
    
    /** Return a SceUInt32 containing the destination address of a jump
     * instruction.
     * 
     * @param i_inst The value of the instruction to decode.
     * @param i_addr The address of the instruction to decode.
     * @return A SceUInt32 containing the address or 0 is returned.
     */
    SceUInt32 mipsGetJumpDestination(unsigned int i_inst, unsigned int i_addr);

#ifdef	__cplusplus
}
#endif

#endif
