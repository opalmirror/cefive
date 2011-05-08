TARGET = CEFive
OBJS =  block.o \
	cheat.o \
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
	disassembler.o \
	hexeditor.o \
	searchengine.o \
	searchpanel.o \
	cheatpanel.o \
	cheateditor.o \
	cheatengine.o \
	appletmenu.o \
	appletconfig.o \
	panelconfig.o \
	gameinfo.o \
	optionspanel.o \
	cefiveui.o \
	colorconfig.o \
	cefiveconfig.o \
	geelog.o \
	dmodelrow.o \
	dasmmodel.o \
	dasmtable.o \
	dasmconfig.o \
	gdasm.o \
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

dmodelrow.o: dmodelrow.c dmodelrow.h
	$(CC) $(CFLAGS) -c dmodelrow.c

dasmmodel.o: dasmmodel.c dasmmodel.h dmodelrow.o
	$(CC) $(CFLAGS) -c dasmmodel.c

cursorpos.o: cursorpos.c cursorpos.h
	$(CC) $(CFLAGS) -c cursorpos.c

dimension.o: dimension.c dimension.h
	$(CC) $(CFLAGS) -c dimension.c

block.o: block.c block.h
	$(CC) $(CFLAGS) -c block.c

cheat.o: cheat.c cheat.h
	$(CC) $(CFLAGS) -c cheat.c

colorconfig.o: colorconfig.c colorconfig.h
	$(CC) $(CFLAGS) -c colorconfig.c

panelconfig.o: colorconfig.o cursorpos.o dimension.o panelconfig.c panelconfig.h
	$(CC) $(CFLAGS) -c panelconfig.c

dasmtable.o: dasmtable.c dasmtable.h dasmmodel.o cursorpos.o panelconfig.o
	$(CC) $(CFLAGS) -c dasmtable.c

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

appletmenu.o: colorconfig.o appletmenu.c appletmenu.h
	$(CC) $(CFLAGS) -c appletmenu.c

cefiveconfig.o: appletconfig.o cefiveconfig.c cefiveconfig.h
	$(CC) $(CFLAGS) -c cefiveconfig.c

cheateditor.o: colorconfig.o dwordeditor.o cheatengine.o cheat.o block.o cheateditor.c cheateditor.h
	$(CC) $(CFLAGS) -c cheateditor.c

cheatengine.o: cefiveconfig.o cheat.o block.o cheatengine.c cheatengine.h
	$(CC) $(CFLAGS) -c cheatengine.c

cheatpanel.o: cheatengine.o colorconfig.o cheat.o block.o cheatpanel.c cheatpanel.h
	$(CC) $(CFLAGS) -c cheatpanel.c

geelog.o: geelog.c geelog.h
	$(CC) $(CFLAGS) -c geelog.c
	
disassembler.o: mips.o dwordeditor.o addresscolumn.o dwordcolumn.o textcolumn.o colorconfig.o appletconfig.o disassembler.c disassembler.h
	$(CC) $(CFLAGS) -c disassembler.c

dasmconfig.o: dasmconfig.c dasmconfig.h appletconfig.o colorconfig.o
	$(CC) $(CFLAGS) -c dasmconfig.c
	
gdasm.o: gdasm.c gdasm.h dasmconfig.o geelog.o cursorpos.o colorconfig.o dasmtable.o appletconfig.o
	$(CC) $(CFLAGS) -c gdasm.c

crt_prx.o: cheat.o block.o cheatengine.o cefiveui.o cefiveconfig.o searchengine.o geelog.o crt_prx.c crt_prx.h
	$(CC) $(CFLAGS) -c crt_prx.o
