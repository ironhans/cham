CC := gcc

CPPFLAGS := -I/home/$(USER)/.local/include -MMD -MP
CFLAGS   := -Wall -pedantic -Wextra -Werror -x c
LDFLAGS  := -Wl,-rpath=/home/$(USER)/.local/lib -L/home/$(USER)/.local/lib
LDLIBS   := -lm

MODE 	 := debug
SRC_DIR  := src
BIN_DIR  := bin
TEST_DIR := tests
ifeq ($(MODE),release)
	OBJ_DIR := release
	CFLAGS += -O3 -DNDEBUG
else
	OBJ_DIR := debug
	CFLAGS += -O0 -ggdb3
endif

EXE := $(BIN_DIR)/cham
SRC := $(shell find src -name '*.c')
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

TEST_EXE := $(BIN_DIR)/run-tests
TEST_SRC := $(filter-out src/$(EXE).c, $(SRC) $(wildcard $(TEST_DIR)/*.c))
VAL := $(wildcard $(TEST_DIR)/valgrind-out.txt*)

.PHONY: all clean run-tests run-valgrind

all: $(EXE)
	echo ${OBJ}
	echo ${OBJ_DIR}

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	mkdir -p `dirname $@`
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR) $(DBG_DIR) $(RLS_DIR):
	mkdir -p $@

run-tests: $(TEST_EXE)
	./$<

run-valgrind: $(TEST_EXE)
	valgrind --leak-check=full \
    	--show-leak-kinds=all \
    	--track-origins=yes \
    	--verbose \
    	--log-file="$(TEST_DIR)/valgrind-out.txt" \
    	./$<
	cat "$(TEST_DIR)/valgrind-out.txt"

$(TEST_EXE): $(BIN_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -ggdb3 $(LDFLAGS) $(LDLIBS) $(TEST_SRC) -o $@

clean:
	rm -rfv debug
	rm -rfv release
	rm -rfv $(BIN_DIR)
	rm -rfv $(VAL)
	rm -rfv "$(TEST_DIR)/test_file"

-include $(OBJ:.o=.d)
