#ifndef _BLOCK_H
#define _BLOCK_H

#define BLOCK_SUCCESS           (0)
#define BLOCK_FAILURE           (-1)
#define BLOCK_MEMORY            (-2)

#define BLOCK_FALSE             (0)
#define BLOCK_TRUE              (1)

//Block flags
#define BLOCK_FLAG_DMA  	(1<<4) // 00010000 0x10
#define BLOCK_FLAG_FREEZE       (1<<5) // 00100000 0x20
//#define BLOCK_FLAG_DWORD	(3<<6) // 11000000 0xC0 00000100
#define BLOCK_FLAG_DWORD        ((unsigned char) 0x04)
//#define BLOCK_FLAG_UWORD	(2<<6) // 10000000 0x80 00001000
#define BLOCK_FLAG_UWORD        ((unsigned char) 0x08)
//#define BLOCK_FLAG_WORD		(1<<6) // 01000000 0x40 00000010
#define BLOCK_FLAG_WORD         ((unsigned char) 0x02)
//#define BLOCK_FLAG_BYTE		(0<<6) // 00000000 0x00 00000001
#define BLOCK_FLAG_BYTE         ((unsigned char) 0x01)

typedef struct _Block{
    unsigned char flags;
    unsigned int address;
    unsigned int stdVal;
    unsigned int hakVal;
}Block;

int block_clear_byte(Block* prBlock);
int block_clear_dma(Block* prBlock);
int block_clear_dword(Block* prBlock);
int block_clear_freeze(Block* prBlock);
int block_clear_qword(Block* prBlock);
int block_clear_word(Block* prBlock);
unsigned int block_get_address(Block* prBlock);
unsigned int block_get_original(Block* prBlock);
unsigned int block_get_patch(Block* prBlock);
int block_init(Block* prBlock, unsigned int address, unsigned int value,
        unsigned char flags);
int block_is_byte(Block* prBlock);
int block_is_dma(Block* prBlock);
int block_is_dword(Block* prBlock);
int block_is_freeze(Block* prBlock);
int block_is_qword(Block* prBlock);
int block_is_word(Block* prBlock);
int block_set_address(Block* prBlock, unsigned int addr);
int block_set_byte(Block* prBlock);
int block_set_dma(Block* prBlock);
int block_set_dword(Block* prBlock);
int block_set_freeze(Block* prBlock);
int block_set_original(Block* prBlock, unsigned int orig);
int block_set_patch(Block* prBlock, unsigned int patch);
int block_set_qword(Block* prBlock);
int block_set_word(Block* prBlock);

#endif /* _BLOCK_H */
