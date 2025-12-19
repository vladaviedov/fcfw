export PWD=$(shell pwd)
export BUILD=$(PWD)/build
# export VERSION='"$(shell git describe --tags --dirty)"'
export VERSION="0.1"

export TOOLCHAIN_ROOT=$${HOME}/.arduino15/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7

export CC=$(TOOLCHAIN_ROOT)/bin/avr-gcc
export CFLAGS=-std=c99 \
	-I$(BUILD)/include \
	-I$(TOOLCHAIN_ROOT)/avr/include \
	-DFCFW_VERSION=$(VERSION) \
	-D__AVR_ATtiny1634__
export CFLAGS_RELEASE=-O2 -w
export CFLAGS_DEBUG=-Wall -Wextra -g -DDEBUG=1
export LDFLAGS=

export OBJ_DIR=
export TASK=

BUILD_MK=$(PWD)/build.mk

# Build tasks
.PHONY: release
release: TASK=release
release: CFLAGS+=$(CFLAGS_RELEASE)
release: OBJ_DIR=$(BUILD)/obj
release:
	$(MAKE) -f $(BUILD_MK)

.PHONY: debug
debug: TASK=debug
debug: CFLAGS+=$(CFLAGS_DEBUG)
debug: OBJ_DIR=$(BUILD)/objd
debug:
	$(MAKE) -f $(BUILD_MK)

.PHONY: install
install:
	$(MAKE) -f $(BUILD_MK) install

.PHONY: clean
clean:
	rm -rf $(BUILD)

# Compile flags
.PHONY: dev
dev:
	@printf "%s\n" $(CFLAGS) $(CFLAGS_DEBUG) > compile_flags.txt

# Formatting
FORMAT=clang-format
FORMAT_CHECK_FLAGS=--dry-run --Werror
FORMAT_FIX_FLAGS=-i

FORMAT_FILES=$(shell find src -type f -name '*.c' -o -name '*.h')

.PHONY: checkformat
checkformat:
	$(FORMAT) $(FORMAT_CHECK_FLAGS) $(FORMAT_FILES)

.PHONY: format
format:
	$(FORMAT) $(FORMAT_FIX_FLAGS) $(FORMAT_FILES)
