# Disassembler #
The Disassembler is a real-time MIPS32 instruction decoder and disassembler for Cheat Engine of Five.  The Disassembler views current memory as MIPS32 Assembly Language.

## Appearance ##
The Disassembler Applet is comprised of a Disassembler Panel with a Status Line underneath.

### Disassembler Panel ###
The Disassembler Panel is a Table with three columns.  The Columns, from left to right are: the address, the value and the comment column.  The Address column always shows the "real" address that is being viewed.  The Value column shows the memory value that is being viewed.  Depending on several different criteria, the Comment column will show a variety of information.  If the row is a MIPS 32 instruction, the comment column will show an assembly language version of the instruction.  If the row contains a pointer, the comment column will show the word "Pointer" and possibly more information.

### Status Line ###
Directly beneath the Disassembler Panel is a status line.  This status line will occasionally show information about the area of memory being viewed.  This information includes the memory segment and any applicable labels for the address being viewed.

## Controls ##
The Disassembler uses the following controls.

### D-Pad Up/Down ###
The D-Pad is used to select the previous or next line in the Disassembler.  While the User is editing an Address or Value, the D-pad is used to increment or decrement the selected digit.

### D-Pad Left/Right ###
The D-pad is used to select the Address or Value column in the Disassembler.  On lines containing **branch** instructions or **jump** instructions, pressing right while in the Value column will cause the Disassembler to jump to the destination of the instruction.  After jumping, pressing Left in the Address column will cause the Disassembler to jump back to the last address.  There is a stack of 10 addresses kept as history.  While the User is editing and Address or Value, the D-pad is used to select the previous or next digit in the Address or Value.

### Cross Button ###
The Cross Button is used to start editing the value of the currently selected row.  Pressing the Cross button will cause a Hex Key Pad to pop up allowing the value to be changed.

### Circle Button ###
The Circle Button is used to exit CEFive.  While the user is editing, the Circle Button is used to cancel the Address or Value being edited and return it to its original value.

### Square Button ###
The Square Button can be used in conjunction with D-Pad Up and Down to act as a Page selector.  Holding the Square Button while pressing the D-Pad Up, will cause the Disassembler to scroll one page up.  The same applies for down.

### Triangle Button ###
The Triangle button will pop up a [Hex Keypad](HexKeypad.md) allowing the user to enter
a new address to navigate to.

### Left Trigger Button ###
The Left Trigger Button is used to pop up an Applet Menu allowing the user to select a different applet to invoke.  While the User is editing, the Left Trigger Button has no effect.

## Display Format ##
### Address Column ###
The Address Column is a 32-bit unsigned integer expressed as Hexadecimal(base 16) with the character sequence '0x' appearing at the beginning of the address.  The Address displayed is the kernel address with the user bit masked off (0x40000000).

### Value Column ###
The Value Column is a 32-bit unsigned integer expressed as Hexadecimal(base 16).  The Value column contains the value at the Address specified in the Address column.  The Value is not altered in any other way than to be presented in Big-Endian format.

### Code Column ###
The Code Column is a text representation of the Value column when decoded as a MIPS32 Instruction.  The text that is shown is an assembly language interpretation of the 32-bit unsigned integer value.  The notation used to express MIPS32 assembly language is the same as the notation used in the MIPS Programmers Reference, and follows the specification as closely as possible.
For instructions that are not able to be decoded as MIPS32 assembly language, the mnemonic **???** is shown instead.