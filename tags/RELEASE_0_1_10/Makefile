TARGET = CEFive
OBJS =  addresscolumn.o \
	block.o \
	appletconfig.o \
	appletmenu.o \
	appletmenuconfig.o \
	asciicolumn.o \
	bytecolumn.o \
	cefiveconfig.o \
	cefiveui.o \
	cheat.o \
	cheateditor.o \
	cheatengine.o \
	cheatpanel.o \
	colorconfig.o \
	crt0_prx.o \
	cursorpos.o \
	dimension.o \
	disassembler.o \
	dwordcolumn.o \
	dwordeditor.o \
	gameinfo.o \
	geelog.o \
	ggame.o \
	glabel.o \
	glabelmap.o \
	gsegmap.o \
	gsegment.o \
	gstubmap.o \
	hexeditor.o \
	hexpad.o \
	jumpstack.o \
	memviewpanel.o \
	menumodel.o \
	mips.o \
	modstub.o \
	module.o \
	niteprio.o \
	optionspanel.o \
	panelconfig.o \
	queryview.o \
	resultmodel.o \
	resultview.o \
	searchapplet.o \
	searchengine.o \
	searchpanel.o

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
