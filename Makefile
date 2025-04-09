OS_NAME := $(shell uname -s)

ifeq ($(OS_NAME),Linux)
  OPEN = xdg-open
  CHECK_LIBS += -lcheck -lsubunit -lrt
  CURSES_LIB = -lncurses
  CHECK_INC = 
endif

ifeq ($(OS_NAME),Darwin)
  CHECK_LIBS += -lcheck
  CURSES_LIB = -lncurses
  CHECK_INC = -I/usr/local/include
  LDFLAGS += -L/usr/local/lib
  OPEN = open
endif

CC = gcc

CFLAGS = -Wall -Wextra -Werror -std=c11 -I.
LDFLAGS += $(CURSES_LIB) -L./build/lib -l_game
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
CHECK_LIBS += -lpthread -lm

PREFIX ?= $(CURDIR)
DESTDIR ?= 

PREFIX ?= $(HOME)/.local
DESTDIR ?=
BINDIR = $(DESTDIR)$(PREFIX)/bin

LIB_SRC_DIR = ./collision_simulator
T_ETRIS_SRC_DIR = $(LIB_SRC_DIR)/t_e_t_r_i_s
GUI_DIR = ./gui/cli
OBJ_DIR = ./build/obj
LIB_DIR = ./build/lib
TEST_DIR = ./test


LIB_SOURCES = $(LIB_SRC_DIR)/specification.c $(T_ETRIS_SRC_DIR)/game_logic.c 
GUI_SOURCES = $(GUI_DIR)/main.c $(GUI_DIR)/input.c $(GUI_DIR)/renderer.c
TEST_SRC = $(wildcard $(TEST_DIR)/*.c) 

LIB_OBJECTS = $(patsubst $(LIB_SRC_DIR)/%.c,$(OBJ_DIR)/collision_simulator/%.o,$(LIB_SOURCES))
GUI_OBJECTS = $(patsubst $(GUI_DIR)/%.c,$(OBJ_DIR)/gui/cli/%.o,$(GUI_SOURCES))
LIB_NAME = lib_game.a  

all: game

game: $(LIB_DIR)/$(LIB_NAME) $(GUI_OBJECTS)
	$(CC) $(GUI_OBJECTS) -o $@ $(LDFLAGS)
	rm -rf ./build

$(LIB_DIR)/$(LIB_NAME): $(LIB_OBJECTS)
	@mkdir -p $(LIB_DIR)
	ar rcs $@ $^

$(OBJ_DIR)/collision_simulator/%.o: $(LIB_SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/gui/cli/%.o: $(GUI_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) $< -o $@

install: all
	@echo "Installing to $(BINDIR)"
	mkdir -p $(BINDIR)
	install -m 755 game $(BINDIR)
	rm -rf ./build game

uninstall:
	@echo "Removing from $(BINDIR)"
	rm -f $(BINDIR)/game
	rmdir --ignore-fail-on-non-empty $(BINDIR) || true

clean:
	rm -rf ./build game
	rm -f $(TEST_DIR)/*.gcno $(TEST_DIR)/*.gcda 
	rm -f *.gcno *.gcda coverage_filtered.info
	rm -f *.o unit_tests
	rm -f $(OBJ_DIR)/*.o coverage.info 
	rm -rf ./coverage_report game_dist.tar.gz
	rm -rf ./html ./latex

dvi: clean
	@echo "Generating DVI documentation..."
	doxygen Doxyfile 
	$(OPEN) html/index.html

dist:
	tar czvf game_dist.tar.gz \
		./collision_simulator \
		./gui \
		./test \
		./Doxyfile \
		./Makefile \
		./ncurses.supp \
		./README.md 

test: $(LIB_DIR)/$(LIB_NAME) 
	$(CC) $(CFLAGS) $(CHECK_INC) $(TEST_SRC) $(GCOV_FLAGS) \
	$(LIB_SOURCES) -o unit_tests \
	$(CHECK_LIBS) -L$(LIB_DIR) -l_game $(LDFLAGS)
	./unit_tests

gcov_report: test
	@echo "Generating coverage report..."
	lcov --capture --directory . --output-file coverage.info
	lcov --remove coverage.info '*/test/*' -o coverage_filtered.info
	genhtml coverage_filtered.info --output-directory ./coverage_report
	@$(OPEN) ./coverage_report/index.html || echo "Could not open report"

valgrind_game: clean all
	valgrind \
    --leak-check=full \
    --track-origins=yes \
    --show-leak-kinds=all \
    --suppressions=ncurses.supp \
    --trace-children=yes \
    --log-file=val_game.txt \
	-s \
    ./game

valgrind_tests: clean test
	valgrind \
    --leak-check=full \
    --track-origins=yes \
    --show-leak-kinds=all \
    --suppressions=ncurses.supp \
    --trace-children=yes \
	-s \
    ./unit_tests

cl:
	clang-format -i  $(LIB_SRC_DIR)/*.c  $(LIB_SRC_DIR)/*.h 
	clang-format -i  $(TETRIS_SRC_DIR)/*.c  $(TETRIS_SRC_DIR)/*.h 
	clang-format -i  $(GUI_DIR)/*.c  $(GUI_DIR)/*.h 
	clang-format -i  $(TEST_DIR)/*.c  $(TEST_DIR)/*.h 

cpp: 
	cppcheck --enable=all --force --error-exitcode=1 --std=c11 --suppress=missingIncludeSystem --inconclusive --language=c --check-level=exhaustive --verbose ../src

.PHONY: all install uninstall clean dvi dist test gcov_report valgrind_game cl cpp

