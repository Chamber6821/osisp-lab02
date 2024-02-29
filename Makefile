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
OBJECTS = $(foreach source,$(SOURCES),$(call object,$(source)))
MAINS = $(wildcard src/cmd/*.c)
EXECUTABLES = $(foreach main,$(MAINS),$(call executable,$(main)))

executable = $(BUILD_DIR)/$(patsubst %.c,%,$(notdir $(1)))$(DEBUG_SUFFIX)
object = $(BUILD_DIR)/$(patsubst %.c,%.o,$(1))
dependencies = $(CC) -MM $(1) -MT $(2)

$(foreach source,$(SOURCES),$(eval $(shell $(call dependencies,$(source),$(call object,$(source))))))
$(foreach main,$(MAINS),$(eval $(shell $(call dependencies,$(main),$(call executable,$(main))))))

all: app

.PHONY: run
run: $(EXECUTABLES)
	@$(BUILD_DIR)/$(TARGET)

.PHONY: app
app: $(EXECUTABLES)

$(EXECUTABLES): $(call executable,%): src/cmd/%.c $(OBJECTS)
	mkdir -p $(dir $@)
	$(CC) $< $(OBJECTS) -o $@

$(OBJECTS): $(call object,%.c): %.c
	mkdir -p $(dir $@)
	$(CC) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
