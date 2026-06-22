CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
LDFLAGS = -lm

all: mlp

mlp: mlp.c mat.c mat.h
	$(CC) $(CFLAGS) -o mlp mlp.c mat.c $(LDFLAGS)

test_matrix_io: tests/test_matrix_io.c mat.c mat.h
	$(CC) $(CFLAGS) -o tests/test_matrix_io tests/test_matrix_io.c mat.c $(LDFLAGS)

test: test_matrix_io
	./tests/test_matrix_io
	python3 tests/test_generate_data.py
	python3 tests/test_prepare_data.py

ifeq ($(wildcard /bin/uname),)
    rm = del /q
else
    rm = rm -f
endif

clean:
	$(rm) mlp tests/test_matrix_io tests/tmp_matrix.bin


run:
	./mlp data/prepared/x_train.bin