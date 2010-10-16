TARGET = nitePR
OBJS =  block.o \
	cheat.o \
	mips.o \
	addresscolumn.o \
	asciicolumn.o \
	bytecolumn.o \
	textcolumn.o \
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
	crt0_prx.o \
	exports.o \
	module.o

# Define to build this as a prx (instead of a static elf)
BUILD_PRX=1
# Define the name of our custom exports (minus the .exp extension)
PRX_EXPORTS=exports.exp

USE_KERNEL_LIBS = 1
USE_KERNEL_LIBC = 1

INCDIR = 
CFLAGS = -O2 -G0 -w -msingle-float -g
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LIBS = -lpspchnnlsv -lpsputility -lpspdebug -lpspge_driver -lpspwlan -lpspumd
LDFLAGS = -nostdlib  -nodefaultlibs -g

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

block.o: block.c block.h
	$(CC) $(CFLAGS) -c block.c

cheat.o: cheat.c cheat.h
	$(CC) $(CFLAGS) -c cheat.c

colorconfig.o: colorconfig.c colorconfig.h
	$(CC) $(CFLAGS) -c colorconfig.c

panelconfig.o: colorconfig.o panelconfig.c panelconfig.h cursorpos.h dimension.h
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

togglecolumn.o: colorconfig.o togglecolumn.c togglecolumn.h
	$(CC) $(CFLAGS) -c togglecolumn.c

dwordeditor.o: colorconfig.o dwordeditor.c dwordeditor.h
	$(CC) $(CFLAGS) -c dwordeditor.c

appletmenu.o: colorconfig.o appletmenu.c appletmenu.h
	$(CC) $(CFLAGS) -c appletmenu.c

cefiveconfig.o: colorconfig.o cefiveconfig.c cefiveconfig.h
	$(CC) $(CFLAGS) -c cefiveconfig.c

cheateditor.o: colorconfig.o dwordeditor.o cheatengine.o cheat.o block.o cheateditor.c cheateditor.h
	$(CC) $(CFLAGS) -c cheateditor.c

cheatengine.o: cefiveconfig.o cheat.o block.o cheatengine.c cheatengine.h
	$(CC) $(CFLAGS) -c cheatengine.c

cheatpanel.o: cheatengine.o colorconfig.o cheat.o block.o cheatpanel.c cheatpanel.h
	$(CC) $(CFLAGS) -c cheatpanel.c
