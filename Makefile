# https://stackoverflow.com/a/18258352/13830772
rwildcard = $(filter-out \ ,$(foreach pattern,$(2),$(wildcard $(1)/$(pattern)))$(foreach child,$(wildcard $(1)/*),$(call rwildcard,$(child),$(2))))
# https://stackoverflow.com/a/7324640/13830772
eq = $(and $(findstring $(1),$(2)),$(findstring $(2),$(1)))

BUILD_DIR = build
EXECUTABLE = $(BUILD_DIR)/main$(if $(call eq,$(MODE),debug),-debug)
WF ?= 
WD ?= .

CFLAGS = -W -Wall -Wextra -Werror -pedantic -std=c11 $(if $(call eq,$(MODE),debug),-ggdb)
CC = gcc $(CFLAGS)
SOURCES = $(call rwildcard,src,*.c)

all: app

.PHONY: run
run: $(EXECUTABLE)
	@cd $(WD) && $(EXECUTABLE) $(WF)

.PHONY: app
app: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES) $(BUILD_DIR)
	$(CC) $(SOURCES) -o $(EXECUTABLE)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

clean:
	rm -r $(BUILD_DIR)
