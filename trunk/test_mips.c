#include "mips.h"
#include <stdio.h>

int main(int argc, char **argv) {
    char buffer[80];
    mipsDecode(buffer, 0x4600C705, 0x08804000);
    printf("%s\n", buffer);
    mipsDecode(buffer, 0x00853020, 0x08804000);
    printf("%s\n", buffer);
    mipsDecode(buffer, 0x46262080, 0x08804000);
    printf("%s\n", buffer);
    mipsDecode(buffer, 0x21490003, 0x08804000);
    printf("%s\n", buffer);
    mipsDecode(buffer, 0x27BDFFF0, 0x08804000);
    printf("%s\n", buffer);
    mipsDecode(buffer, 0x014b4821, 0x08804000);
    printf("%s\n", buffer);
    
    MipsInstruction inst;
    inst.value = 0x4600C705;

}

