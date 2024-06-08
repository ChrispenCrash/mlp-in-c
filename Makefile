all: mlp

mlp:
	gcc -o mlp mlp.c funcs.c

ifeq ($(wildcard /bin/uname),)
    rm = del /q
else
    rm = rm -f
endif

clean:
	$(rm) *.exe