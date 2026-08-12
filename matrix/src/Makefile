
CC = gcc 
CFLAGS = -Wall -Werror -Wextra -std=c11
LDFLAGS = -lm -lpthread -lcheck
COV_FLAGS = -fprofile-arcs -ftest-coverage
AR = ar
ARFLAGS = rcs

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    LDFLAGS += -lpthread -lcheck -lm -lsubunit
endif

SRC = s21_matrix.c
OBJS = $(SRC:.c=.o)

TEST_SRC = tests/s21_tests.c
TEST_OBJ = $(TEST_SRC:.c=.o)
TEST_EXEC = test_matrix

.PHONY: all clean s21_matrix.a test gcov_report

all: s21_matrix.a

s21_matrix.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_EXEC): CFLAGS += $(COV_FLAGS)
$(TEST_EXEC): LDFLAGS += $(COV_FLAGS)
$(TEST_EXEC): $(OBJS) $(TEST_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test: $(TEST_EXEC)
	./$(TEST_EXEC)


gcov_report: clean $(TEST_EXEC)
	./$(TEST_EXEC)
	@mkdir -p ./report
	lcov --capture --directory . --output-file report/coverage.info --rc lcov_branch_coverage=1
	lcov --remove report/coverage.info '*/tests/*' --output-file report/coverage_filtered.info
	genhtml report/coverage.info --output-directory report
	@xdg-open report/index.html || open report/index.html

leaks: $(TEST_EXEC)
	leaks --atExit -- ./$(TEST_EXEC)

clang:
	cp ../materials/linters/.clang-format ./
	clang-format -i *.c tests/*.c *.h tests/*.h
	
clean:
	rm -rf $(OBJS) *.o tests/*.o s21_matrix.a tests/report tests/*.gcda ./tests/*.gcno ./report *.gcda *.gcno ./test_matrix
