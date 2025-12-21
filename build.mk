BUILD_DIRS=$(BUILD) \
		   $(BUILD)/include \
		   $(BUILD)/bin \
		   $(OBJ_DIR)

ELF_TARGET=$(BUILD)/bin/fcfw.elf
HEX_TARGET=$(BUILD)/bin/fcfw.hex

SUBDIRS=$(shell cd $(PWD)/src && find * -type d)
MKSUBDIRS=$(addprefix $(OBJ_DIR)/, $(SUBDIRS))
SRCS=$(shell cd $(PWD)/src && find * -type f -name '*.c')
OBJS=$(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

.PHONY: build
build: $(BUILD_DIRS) $(HEX_TARGET)

# Templates
define make_build_dir
$(1):
	mkdir -p $$@
endef

define mk_subdir
$(OBJ_DIR)/$(1): $(BUILD)
	mkdir -p $$@
endef

define compile_subdir
$(OBJ_DIR)/$(1)%.o: $(PWD)/src/$(1)%.c $(LIBUTILS) $(LIBNRL) $(MKSUBDIRS)
	$$(CC) $$(CFLAGS) -c -o $$@ $$<
endef

# Build directory rules
$(foreach build_dir, $(BUILD_DIRS), \
	$(eval $(call make_build_dir,$(build_dir))))

$(HEX_TARGET): $(ELF_TARGET)
	$(OBJCOPY) -O ihex $^ $@

$(ELF_TARGET): $(BUILD) $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

# Build root
$(eval $(call compile_subdir,))

# Build subdirectories
$(foreach subdir, $(SUBDIRS), $(eval $(call mk_subdir,$(subdir))))
$(foreach subdir, $(SUBDIRS), $(eval $(call compile_subdir,$(subdir))))
