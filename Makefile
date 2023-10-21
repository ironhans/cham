CC := gcc

DBG_DIR := debug
RLS_DIR := release
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
TEST_DIR := tests

EXE := $(BIN_DIR)/cham
SRC := $(shell find src -name '*.c')
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

TEST_EXE := $(BIN_DIR)/run-tests
TEST_SRC := $(filter-out src/$(EXE).c, $(SRC) $(wildcard $(TEST_DIR)/*.c))
VAL := $(wildcard $(TEST_DIR)/valgrind-out.txt*)

CPPFLAGS := -I/home/$(USER)/.local/include -MMD -MP
CFLAGS   := -Wall -pedantic -Wextra -Werror -x c
LDFLAGS  := -Wl,-rpath=/home/$(USER)/.local/lib -L/home/$(USER)/.local/lib
LDLIBS   := -lm

.PHONY: all clean run-tests run-valgrind

all: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	mkdir -p `dirname $@`
	$(CC) $(CPPFLAGS) $(CFLAGS) -O2 -c $< -o $@

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
	rm -rfv $(OBJ_DIR)
	rm -rfv $(BIN_DIR)
	rm -rfv $(VAL)
	rm -rfv "$(TEST_DIR)/test_file"

-include $(OBJ:.o=.d)
