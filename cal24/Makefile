CC = gcc
CFLAGS = -Wall -Wextra -g -I utils
DEPS = *.h
UTILS = utils/rational.o utils/expression.o
OBJ_MAIN = src/cal24.o $(UTILS)
OBJ_CHECK = src/cal24check.o $(UTILS)
OBJ_CHECK_TEST = src/cal24test.o $(UTILS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: cal24 cal24check cal24test testall

cal24: $(OBJ_MAIN)
	$(CC) -o $@ $^ $(CFLAGS)

cal24check: $(OBJ_CHECK)
	$(CC) -o $@ $^ $(CFLAGS)

cal24test: $(OBJ_CHECK_TEST)
	$(CC) -o $@ $^ $(CFLAGS)

testall: utils/testall.o
	$(CC) -o testall utils/testall.o $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o cal24 cal24check cal24test testall