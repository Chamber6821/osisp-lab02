# https://stackoverflow.com/a/18258352/13830772
rwildcard = $(filter-out \ ,$(foreach pattern,$(2),$(wildcard $(1)/$(pattern)))$(foreach child,$(wildcard $(1)/*),$(call rwildcard,$(child),$(2))))
# https://stackoverflow.com/a/7324640/13830772
eq = $(and $(findstring $(1),$(2)),$(findstring $(2),$(1)))

TARGET ?= parent
BUILD_DIR = build

DEBUG_SUFFIX = $(if $(call eq,$(MODE),debug),-debug)
CFLAGS = -W -Wall -Wextra -Werror -pedantic -std=c11 -Isrc/main/ $(if $(call eq,$(MODE),debug),-ggdb)
CC = gcc $(CFLAGS)
SOURCES = $(call rwildcard,src/main,*.c)
SOURCE_STUB = $(BUILD_DIR)/stub.c
MAIN_OBJ = $(BUILD_DIR)/main-part$(DEBUG_SUFFIX).o
MAINS = $(wildcard src/cmd/*.c)
TARGETS = $(foreach main,$(MAINS),$(patsubst %.c,%,$(notdir $(main))))
EXECUTABLES = $(foreach target,$(TARGETS),$(call exec,$(target)))
exec = $(BUILD_DIR)/$(1)$(DEBUG_SUFFIX)

all: app

.PHONY: run
run: $(EXECUTABLES)
	@$(call exec,$(TARGET))

.PHONY: app
app: $(EXECUTABLES)

$(EXECUTABLES): $(call exec,%): src/cmd/%.c $(MAIN_OBJ)
	$(CC) $< $(MAIN_OBJ) -o $@

$(MAIN_OBJ): $(SOURCES) $(SOURCE_STUB)
	$(CC) -c $(SOURCES) $(SOURCE_STUB) -o $@

$(SOURCE_STUB): | $(BUILD_DIR)
	echo "typedef int make_iso_compilers_happy;" > $@

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
