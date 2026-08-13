Q ?= @
CC = arm-none-eabi-gcc
BUILD_DIR = output
BUILD_DIR_DEVICE = output/device
BUILD_DIR_SIMULATOR = output/simulator
CC_SIMULATOR = gcc
CXX_SIMULATOR = g++
CFLAGS_SIMULATOR = -std=c99
CFLAGS_SIMULATOR += -Os -Wall
CFLAGS_SIMULATOR += -ggdb -fPIC
LDFLAGS_SIMULATOR = -shared -Wl,-z,noexecstack -static-libgcc

# Epsilon simulator libraries
EPSILON_LIBS = sim/libs/eadk.a sim/libs/ion.a sim/libs/sdl.a sim/libs/kandinsky.a sim/libs/escher.a sim/libs/poincare.a sim/libs/python.a sim/libs/omg.a sim/libs/liba_bridge.a

define object_for_dir
$(addprefix $(1)/,$(addsuffix .o,$(basename $(2))))
endef
NWLINK = npm_config_loglevel=silent npx --yes --quiet -- nwlink@0.0.19
LINK_GC = 1
LTO = 1
EXTERNAL_DATA ?= assets/input.bin

define object_for
$(addprefix $(BUILD_DIR)/,$(addsuffix .o,$(basename $(1))))
endef

src = $(addprefix src/,\
  libs/storage.c \
  main.c \
)

# Platform selection: set PLATFORM=simulator when building/running on host simulator
PLATFORM ?= device

ifeq ($(PLATFORM),simulator)
# Add simulator-only flags and sources
CFLAGS_SIMULATOR += -DSIMULATOR_HOST
src_simulator = $(src) \
	src/sim/sim_eadk.c
else
src_simulator = $(src)
endif

CFLAGS_DEVICE = -std=c99
CFLAGS_DEVICE += $(shell $(NWLINK) eadk-cflags-device)
CFLAGS_DEVICE += -Os -Wall
CFLAGS_DEVICE += -ggdb
LDFLAGS = -Wl,--relocatable
LDFLAGS += -nostartfiles
LDFLAGS += --specs=nano.specs
# LDFLAGS += --specs=nosys.specs # Alternatively, use full-fledged newlib

ifeq ($(LINK_GC),1)
CFLAGS_DEVICE += -fdata-sections -ffunction-sections
LDFLAGS += -Wl,-e,main -Wl,-u,eadk_app_name -Wl,-u,eadk_app_icon -Wl,-u,eadk_api_level
LDFLAGS += -Wl,--gc-sections
endif

ifeq ($(LTO),1)
CFLAGS_DEVICE += -flto -fno-fat-lto-objects
CFLAGS_DEVICE += -fwhole-program
CFLAGS_DEVICE += -fvisibility=internal
LDFLAGS += -flinker-output=nolto-rel
endif

.PHONY: build
# Choose build target depending on PLATFORM (simulator or device)
ifeq ($(PLATFORM),simulator)
BUILD_TARGET := $(BUILD_DIR_SIMULATOR)/app.nwb
else
BUILD_TARGET := $(BUILD_DIR_DEVICE)/app.nwa
endif
build: $(BUILD_TARGET)

.PHONY: check
check: $(BUILD_DIR_DEVICE)/app.bin


.PHONY: run
# Run the appropriate target depending on PLATFORM
run: build
	@echo "RUN ($(PLATFORM))"
	$(Q) if [ "$(PLATFORM)" = "simulator" ]; then \
		if [ -s $(EXTERNAL_DATA) ]; then \
			./sim/epsilon.bin --nwb $(BUILD_DIR_SIMULATOR)/app.nwb --nwb-external-data $(EXTERNAL_DATA); \
		else \
			./sim/epsilon.bin --nwb $(BUILD_DIR_SIMULATOR)/app.nwb; \
		fi; \
	else \
		if [ -s $(EXTERNAL_DATA) ]; then \
			$(NWLINK) install-nwa --external-data $(EXTERNAL_DATA) $(BUILD_DIR_DEVICE)/app.nwa; \
		else \
			$(NWLINK) install-nwa $(BUILD_DIR_DEVICE)/app.nwa; \
		fi; \
	fi

$(BUILD_DIR_DEVICE)/%.bin: $(BUILD_DIR_DEVICE)/%.nwa $(EXTERNAL_DATA)
	@echo "BIN     $@"
	$(Q) if [ -s $(EXTERNAL_DATA) ]; then \
		$(NWLINK) nwa-bin --external-data $(EXTERNAL_DATA) $< $@; \
	else \
		$(NWLINK) nwa-bin $< $@; \
	fi

$(BUILD_DIR_DEVICE)/%.elf: $(BUILD_DIR_DEVICE)/%.nwa $(EXTERNAL_DATA)
	@echo "ELF     $@"
	$(Q) if [ -s $(EXTERNAL_DATA) ]; then \
		$(NWLINK) nwa-elf --external-data $(EXTERNAL_DATA) $< $@; \
	else \
		$(NWLINK) nwa-elf $< $@; \
	fi

$(BUILD_DIR_DEVICE)/app.nwa: $(call object_for_dir,$(BUILD_DIR_DEVICE),$(src)) $(BUILD_DIR_DEVICE)/icon.o
	@echo "LD      $@"
	$(Q) $(CC) $(CFLAGS_DEVICE) $(LDFLAGS) $^ -o $@

# Simulator build: produce a shared library for the simulator
$(BUILD_DIR_SIMULATOR)/app.nwb: $(call object_for_dir,$(BUILD_DIR_SIMULATOR),$(src_simulator))
	@echo "LDSIMULATOR  $@"
	$(Q) $(CXX_SIMULATOR) $(CFLAGS_SIMULATOR) $(LDFLAGS_SIMULATOR) $(call object_for_dir,$(BUILD_DIR_SIMULATOR),$(src_simulator)) -o $@

$(BUILD_DIR_DEVICE)/src/libs/storage.o: src/libs/storage.c | $(BUILD_DIR_DEVICE)
	@echo "CC      $^"
	$(Q) mkdir -p $(dir $@)
	$(Q) $(CC) $(CFLAGS_DEVICE) -w -c $^ -o $@

$(BUILD_DIR_SIMULATOR)/src/libs/storage.o: src/libs/storage.c | $(BUILD_DIR_SIMULATOR)
	@echo "CCSIMULATOR  $^"
	$(Q) mkdir -p $(dir $@)
	$(Q) $(CC_SIMULATOR) $(CFLAGS_SIMULATOR) -w -c $^ -o $@

$(addprefix $(BUILD_DIR_DEVICE)/,%.o): %.c | $(BUILD_DIR_DEVICE)
	@echo "CC      $^"
	$(Q) mkdir -p $(dir $@)
	$(Q) $(CC) $(CFLAGS_DEVICE) -c $^ -o $@

$(addprefix $(BUILD_DIR_SIMULATOR)/,%.o): %.c | $(BUILD_DIR_SIMULATOR)
	@echo "CCSIMULATOR  $^"
	$(Q) mkdir -p $(dir $@)
	$(Q) $(CC_SIMULATOR) $(CFLAGS_SIMULATOR) -c $^ -o $@


$(BUILD_DIR_DEVICE)/icon.o: assets/icon.png
	@echo "ICON    $<"
	$(Q) $(NWLINK) png-icon-o $< $@

.PRECIOUS: $(BUILD_DIR_DEVICE) $(BUILD_DIR_SIMULATOR)
$(BUILD_DIR_DEVICE):
	$(Q) mkdir -p $@/src

$(BUILD_DIR_SIMULATOR):
	$(Q) mkdir -p $@/src

.PHONY: clean
clean:
	@echo "CLEAN"
	$(Q) if [ "$(origin PLATFORM)" = "file" ]; then \
		rm -rf $(BUILD_DIR_DEVICE) $(BUILD_DIR_SIMULATOR); \
	elif [ "$(PLATFORM)" = "simulator" ]; then \
		rm -rf $(BUILD_DIR_SIMULATOR); \
	else \
		rm -rf $(BUILD_DIR_DEVICE); \
	fi
