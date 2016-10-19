PROJECT := VpkPacker
PROJECT_TITLE := VpkPacker
PROJECT_TITLEID := VPKPACKER

CFLAGS := 
CFLAGS += -Wl,-q -Wall -o3
CFLAGS += -DIOAPI_NO_64
CXXFLAGS := -std=c++11 $(CFLAGS)
ASFLAGS := $(CFLAGS)

LIBS := -lSceSysmodule_stub -lSceCtrl_stub
LIBS += -lScePgf_stub -lvita2d
LIBS += -lSceDisplay_stub -lSceGxm_stub
#LIBS += -lSceCommonDialog_stub -lfreetype
LIBS += -lSceAppUtil_stub
LIBS += -lz -lm -lc

RESOURCE := --add sce_sys/livearea/contents/template.xml=sce_sys/livearea/contents/template.xml
RESOURCE += --add sce_sys/icon0.png=sce_sys/icon0.png
RESOURCE += --add sce_sys/livearea/contents/bg0.png=sce_sys/livearea/contents/bg0.png
RESOURCE += --add sce_sys/livearea/contents/startup.png=sce_sys/livearea/contents/startup.png

PSVITAIP := 192.168.0.13

PHONY := all package clean
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

PREFIX  = arm-vita-eabi
CC := $(PREFIX)-gcc
CXX := $(PREFIX)-g++

PACKVPK := vita-pack-vpk
MAKEEBOOT := vita-make-fself
MAKESFO := vita-mksfoex
STRIP := $(PREFIX)-strip
CREATE := vita-elf-create

SRC_C := $(call rwildcard, src/, *.c)
SRC_CPP := $(call rwildcard, src/, *.cpp)

OBJ_DIRS := $(addprefix out/, $(dir $(SRC_C:src/%.c=%.o))) $(addprefix out/, $(dir $(SRC_CPP:src/%.cpp=%.o)))
OBJS := $(addprefix out/, $(SRC_C:src/%.c=%.o)) $(addprefix out/, $(SRC_CPP:src/%.cpp=%.o))

all: package

package: $(PROJECT).vpk send

$(PROJECT).vpk: eboot.bin param.sfo
	$(PACKVPK) -s param.sfo -b eboot.bin $(RESOURCE) $(PROJECT).vpk
#	7z a -tzip ./$(PROJECT).vpk -r .\sce_sys\* .\eboot.bin
	
eboot.bin: $(PROJECT).velf
	$(MAKEEBOOT) $(PROJECT).velf eboot.bin

param.sfo:
	$(MAKESFO) -s TITLE_ID="$(PROJECT_TITLEID)" "$(PROJECT_TITLE)" param.sfo

$(PROJECT).velf: $(PROJECT).elf
	$(STRIP) -g $<
	$(CREATE) $< $@

$(PROJECT).elf: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

$(OBJ_DIRS):
	mkdir -p $@

out/%.o : src/%.cpp | $(OBJ_DIRS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

out/%.o : src/%.c | $(OBJ_DIRS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(PROJECT).velf $(PROJECT).elf $(PROJECT).vpk param.sfo eboot.bin $(OBJS)
	rm -r $(abspath $(OBJ_DIRS))

vpksend: $(PROJECT).vpk
	curl -T $(PROJECT).vpk ftp://$(PSVITAIP):1337/ux0:_test/
	@echo "Sent."

send: eboot.bin
	curl -T eboot.bin ftp://$(PSVITAIP):1337/ux0:/app/$(PROJECT_TITLEID)/
	@echo "Sent."
