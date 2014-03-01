#---------------------------------------------------------------------------------
# Clear the implicit built in rules
#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
ifeq ($(strip $(PSL1GHT)),)
$(error "Please set PSL1GHT in your environment. export PSL1GHT=<path>")
endif

#---------------------------------------------------------------------------------
#  TITLE, APPID, CONTENTID, ICON0 SFOXML before ppu_rules.
#---------------------------------------------------------------------------------
TC_ADD		:=	`date +%d%H%M`
ICON0		:=	ICON0.PNG
ICON1		:=	ICON1.PAM
PIC1		:=	PIC1.PNG
SFOXML		:=	sfo.xml

# usage:  make BUILD_STEALTH=yes
ifndef BUILD_STEALTH
TITLE		:=	GameSonic ODE Manager v01.20
APPID		:=	GAMESONICODMA
else
TITLE		:=	LEMMINGS Trial Version
APPID		:=	NPUA80034
endif
CONTENTID	:=	UP0001-$(APPID)_00-0000000000000000
PKGFILES	:=	release

GENPS3ISO_PATH  ?=	genps3iso -s
SCETOOL_PATH	?=	scetool

SCETOOL_FLAGS	?=	--sce-type=SELF --compress-data=FALSE --self-type=APP --key-revision=0004 --self-fw-version=0003004100000000 \
					--self-app-version=0001000000000000 --self-auth-id=1010000001000003 --self-vendor-id=01000002
					
include $(PSL1GHT)/ppu_rules

# aditional scetool flags (--self-ctrl-flags, --self-cap-flags...)
SCETOOL_FLAGS	+=	--self-ctrl-flags 0000000000000000000000000000000000000000000000000000000000000000
SCETOOL_FLAGS	+=	--self-cap-flags 00000000000000000000000000000000000000000000003b0000000100040000

#---------------------------------------------------------------------------------
# TARGET is the name of the output
# BUILD is the directory where object files & intermediate files will be placed
# SOURCES is a list of directories containing source code
# INCLUDES is a list of directories containing extra header files
#---------------------------------------------------------------------------------
TARGET		:=	$(notdir $(CURDIR))
BUILD		:=	build
SOURCES		:=	source source/ftp
DATA		:=	datas
SHADERS		:=	shaders
INCLUDES	:=	include include/ftp

#---------------------------------------------------------------------------------
# any extra libraries we wish to link with the project
#---------------------------------------------------------------------------------
LIBS		:=	 -lfreetype -lz -ltiny3d -lnetctl -lnet -lsysfs -lpngdec -ljpgdec -lsimdmath -lgcm_sys -lio -lsysutil -lrt -llv2 -lsysmodule


#---------------------------------------------------------------------------------
# options for code generation
#---------------------------------------------------------------------------------

CFLAGS		=	-O2 -Wall -mcpu=cell --std=gnu99 $(MACHDEP) $(INCLUDE)
CFLAGS		+=  `$(PORTLIBS)/bin/freetype-config --cflags`
CFLAGS		+= -D__MKDEF_MANAGER_DIR__="\"$(APPID)\"" -D__MKDEF_MANAGER_FULLDIR__="\"dev_usb000/$(APPID)\""
CFLAGS		+=  -DUSE_MEMCPY_SYSCALL
CFLAGS		+=  -DUSE_DISC_CALLBACK


CXXFLAGS	=	$(CFLAGS)


LDFLAGS		=	$(MACHDEP) -Wl,-Map,$(notdir $@).map

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBDIRS	:=

#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export OUTPUT	:=	$(CURDIR)/$(TARGET)

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
					$(foreach dir,$(DATA),$(CURDIR)/$(dir)) \
					$(foreach dir,$(SHADERS),$(CURDIR)/$(dir))

export DEPSDIR	:=	$(CURDIR)/$(BUILD)

export BUILDDIR	:=	$(CURDIR)/$(BUILD)

#---------------------------------------------------------------------------------
# automatically build a list of object files for our project
#---------------------------------------------------------------------------------
CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
sFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.S)))
BINFILES	:= $(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.bin)))
VCGFILES	:=	$(foreach dir,$(SHADERS),$(notdir $(wildcard $(dir)/*.vcg)))
FCGFILES	:=	$(foreach dir,$(SHADERS),$(notdir $(wildcard $(dir)/*.fcg)))

VPOFILES	:=	$(VCGFILES:.vcg=.vpo)
FPOFILES	:=	$(FCGFILES:.fcg=.fpo)

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
	export LD	:=	$(CXX)
else
	export LD	:=	$(CXX)
endif

export OFILES	:=	$(addsuffix .o,$(BINFILES)) \
					$(addsuffix .o,$(VPOFILES)) \
					$(addsuffix .o,$(FPOFILES)) \
					$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) \
					$(sFILES:.s=.o) $(SFILES:.S=.o)

#---------------------------------------------------------------------------------
# build a list of include paths
#---------------------------------------------------------------------------------
export INCLUDE	:=	$(foreach dir,$(INCLUDES), -I$(CURDIR)/$(dir)) \
					$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
					$(LIBPSL1GHT_INC) \
					-I$(CURDIR)/$(BUILD) -I$(PORTLIBS)/include -I$(PORTLIBS)/modules

#---------------------------------------------------------------------------------
# build a list of library paths
#---------------------------------------------------------------------------------
export LIBPATHS	:=	$(foreach dir,$(LIBDIRS),-L$(dir)/lib) \
					$(LIBPSL1GHT_LIB) -L$(PORTLIBS)/lib

export OUTPUT	:=	$(CURDIR)/$(TARGET)
.PHONY: $(BUILD) clean


#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(OUTPUT).elf $(OUTPUT).self EBOOT.BIN $(TARGET).iso build_iso/PS3_GAME/USRDIR/EBOOT.BIN 
	

#---------------------------------------------------------------------------------
run:
	ps3load $(OUTPUT).self

#---------------------------------------------------------------------------------
pkg:	$(BUILD) $(OUTPUT).pkg

#---------------------------------------------------------------------------------

pkg2: $(BUILD)

	$(VERB) echo building pkg ... $(notdir $@)
	$(VERB) mkdir -p $(BUILDDIR)/pkg2/USRDIR
	$(VERB) cp $(ICON0) $(BUILDDIR)/pkg2/ICON0.PNG
	$(VERB) cp $(ICON1) $(BUILDDIR)/pkg2/ICON1.PAM
	$(VERB) cp $(PIC1) $(BUILDDIR)/pkg2/PIC1.PNG
	$(SELF_NPDRM) $(SCETOOL_FLAGS) --np-content-id=$(CONTENTID) --encrypt $(BUILDDIR)/$(TARGET).elf $(BUILDDIR)/pkg2/USRDIR/EBOOT.BIN
	$(VERB) $(SFO) --title "$(TITLE)" --appid "$(APPID)" -f $(SFOXML) $(BUILDDIR)/pkg2/PARAM.SFO
	$(VERB) if [ -n "$(PKGFILES)" -a -d "$(PKGFILES)" ]; then cp -rf $(PKGFILES)/* $(BUILDDIR)/pkg2/; fi
	$(VERB) $(PKG) --contentid $(CONTENTID) $(BUILDDIR)/pkg2/ $(TARGET)_animated.pkg >> /dev/null
#---------------------------------------------------------------------------------

iso: $(BUILD)

	$(SCETOOL_PATH) $(SCETOOL_FLAGS) --encrypt $(BUILDDIR)/$(TARGET).elf $(CURDIR)/build_iso/PS3_GAME/USRDIR/EBOOT.BIN
	$(GENPS3ISO_PATH) $(CURDIR)/build_iso manager.iso

#---------------------------------------------------------------------------------


npdrm: $(BUILD)
	@$(SELF_NPDRM) $(SCETOOL_FLAGS) --np-content-id=$(CONTENTID) --encrypt $(BUILDDIR)/$(basename $(notdir $(OUTPUT))).elf $(BUILDDIR)/../EBOOT.BIN

#---------------------------------------------------------------------------------

else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT).self: $(OUTPUT).elf
$(OUTPUT).elf:	$(OFILES)

#---------------------------------------------------------------------------------
# This rule links in binary data with the .bin extension
#---------------------------------------------------------------------------------
%.bin.o	:	%.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
%.vpo.o	:	%.vpo
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

#---------------------------------------------------------------------------------
%.fpo.o	:	%.fpo
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)

-include $(DEPENDS)

#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------
