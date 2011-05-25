TARGET = CEFive
OBJS =  block.o \
	blockmodel.o \
	cheat.o \
	cheatmodel.o \
	mips.o \
	cursorpos.o \
	dimension.o \
	addresscolumn.o \
	asciicolumn.o \
	bytecolumn.o \
	textcolumn.o \
	dasmrow.o \
	togglecolumn.o \
	dwordcolumn.o \
	dwordeditor.o \
	memviewpanel.o \
	disassembler.o \
	hexeditor.o \
	searchengine.o \
	searchpanel.o \
	cheatpanel.o \
	cheateditor.o \
	cheatengine.o \
	niteprio.o \
	menumodel.o \
	appletmenuconfig.o \
	appletmenu.o \
	appletconfig.o \
	panelconfig.o \
	gameinfo.o \
	optionspanel.o \
	cefiveui.o \
	colorconfig.o \
	cefiveconfig.o \
	geelog.o \
	crt0_prx.o \
	module.o

# Define to build this as a prx (instead of a static elf)
BUILD_PRX=1
# Define the name of our custom exports (minus the .exp extension)
PRX_EXPORTS=exports.exp

USE_KERNEL_LIBS = 1
USE_KERNEL_LIBC = 1
#USE_PSPSDK_LIBC = 1

INCDIR = 
CFLAGS = -O2 -G0 -w -msingle-float -g
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LIBS = -lpspdebug -lpspge_driver
LDFLAGS = -nostartfiles -mno-crt0 -g

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

mips.o: mips.c mips.h
	$(CC) $(CFLAGS) -c mips.c

cursorpos.o: cursorpos.c cursorpos.h
	$(CC) $(CFLAGS) -c cursorpos.c

dimension.o: dimension.c dimension.h
	$(CC) $(CFLAGS) -c dimension.c

block.o: block.c block.h
	$(CC) $(CFLAGS) -c block.c

blockmodel.o: blockmodel.c blockmodel.h block.o
	$(CC) $(CFLAGS) -c blockmodel.c

cheat.o: cheat.c cheat.h
	$(CC) $(CFLAGS) -c cheat.c

cheatmodel.o: cheatmodel.c cheatmodel.h cheat.o
	$(CC) $(CFLAGS) -c cheatmodel.c

colorconfig.o: colorconfig.c colorconfig.h
	$(CC) $(CFLAGS) -c colorconfig.c

panelconfig.o: colorconfig.o cursorpos.o dimension.o panelconfig.c panelconfig.h
	$(CC) $(CFLAGS) -c panelconfig.c

appletconfig.o: panelconfig.o colorconfig.o appletconfig.c appletconfig.h
	$(CC) $(CFLAGS) -c appletconfig.c

addresscolumn.o: colorconfig.o addresscolumn.c addresscolumn.h
	$(CC) $(CFLAGS) -c addresscolumn.c

asciicolumn.o: colorconfig.o asciicolumn.c asciicolumn.h
	$(CC) $(CFLAGS) -c asciicolumn.c

bytecolumn.o: colorconfig.o bytecolumn.c bytecolumn.h
	$(CC) $(CFLAGS) -c bytecolumn.c

dwordcolumn.o: colorconfig.o dwordcolumn.c dwordcolumn.h
	$(CC) $(CFLAGS) -c dwordcolumn.c

textcolumn.o: colorconfig.o textcolumn.c textcolumn.h
	$(CC) $(CFLAGS) -c textcolumn.c

dasmrow.o: addresscolumn.o dwordcolumn.o textcolumn.o dasmrow.c dasmrow.h
	$(CC) $(CFLAGS) -c dasmrow.c

togglecolumn.o: colorconfig.o togglecolumn.c togglecolumn.h
	$(CC) $(CFLAGS) -c togglecolumn.c

dwordeditor.o: colorconfig.o dwordeditor.c dwordeditor.h
	$(CC) $(CFLAGS) -c dwordeditor.c

menumodel.o: menumodel.c menumodel.h
	$(CC) $(CFLAGS) -c menumodel.c

appletmenuconfig.o: appletmenuconfig.c appletmenuconfig.h colorconfig.o cursorpos.o dimension.o
	$(CC) $(CFLAGS) -c appletmenuconfig.c

appletmenu.o: appletmenu.c appletmenu.h appletmenuconfig.o menumodel.o
	$(CC) $(CFLAGS) -c appletmenu.c

cefiveconfig.o: appletconfig.o cefiveconfig.c cefiveconfig.h
	$(CC) $(CFLAGS) -c cefiveconfig.c

cheateditor.o: colorconfig.o dwordeditor.o cheatengine.o cheat.o block.o cheateditor.c cheateditor.h
	$(CC) $(CFLAGS) -c cheateditor.c

cheatengine.o: cheatengine.c cheatengine.h blockmodel.o cheatmodel.o
	$(CC) $(CFLAGS) -c cheatengine.c

niteprio.o: niteprio.c niteprio.h cheatengine.o geelog.o
	$(CC) $(CFLAGS) -c niteprio.c

cheatpanel.o: cheatengine.o colorconfig.o cheat.o block.o cheatpanel.c cheatpanel.h
	$(CC) $(CFLAGS) -c cheatpanel.c

geelog.o: geelog.c geelog.h
	$(CC) $(CFLAGS) -c geelog.c

memviewpanel.o: memviewpanel.c memviewpanel.h
	$(CC) $(CFLAGS) -c memviewpanel.c
	
disassembler.o: mips.o dwordeditor.o addresscolumn.o dwordcolumn.o textcolumn.o colorconfig.o appletconfig.o disassembler.c disassembler.h
	$(CC) $(CFLAGS) -c disassembler.c

crt_prx.o: cheat.o block.o cheatengine.o cefiveui.o cefiveconfig.o searchengine.o geelog.o crt_prx.c crt_prx.h
	$(CC) $(CFLAGS) -c crt_prx.o
