all: mlp

mlp:
	gcc -o mlp mlp.c mat.c

data_gen:
	gcc -o data_gen data_gen.c

ifeq ($(wildcard /bin/uname),)
    rm = del /q
else
    rm = rm -f
endif

clean:
	$(rm) *.exe


run:
	./mlp.exe data\xnor_data.csv