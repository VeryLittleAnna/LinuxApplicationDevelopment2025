.RECIPEPREFIX=:
.PHONY: clean all distclean
СС = cc
CFLAGS = -Wall -fPIC
LDFLAGS = -L.
GENERATES = prog prog-a prog-so README
TRASH = *.o *.so *.a *~ o.* outfile*
TEST_ENV = LD_LIBRARY_PATH=.

%.o: %.c
:	$(CC) $< $(CFLAGS) -c -o $@

prog: prog.o fun.o const.o
:	$(CC) -o $@ $^

liboutput_static.a: fun.o const.o
:	ar rcs $@ $^

liboutput.so: fun.o const.o
:	$(CC) -shared -o $@ $^

prog-a: prog.o liboutput_static.a
:	$(CC) -static -o $@ prog.o $(LDFLAGS) -loutput_static

prog-so: prog.o liboutput.so
:	$(CC) -o $@ prog.o $(LDFLAGS) -loutput

fun.o: outlib.h

all: prog prog-a prog-so README

README: prog
:	./$< 2> $@

clean:
:	rm -f $(TRASH)

distclean: clean
:	rm -rf $(GENERATES)

test: prog prog-a prog-so
:	./prog > outfile0 2>&1
:	./prog-a > outfile0-a 2>&1
:	$(TEST_ENV) ./prog-so > outfile0-so 2>&1
:	cmp outfile0 outfile0-a
:	cmp outfile0-a outfile0-so
#
:	./prog "Hello" >outfile1 2>&1
:	./prog-a "Hello" >outfile1-a 2>&1
:	$(TEST_ENV) ./prog-so "Hello" >outfile1-so 2>&1
:	cmp outfile1 outfile1-a
:	cmp outfile1-a outfile1-so
#
:	./prog "Yet" "another" "test" >outfile3 2>&1
:	./prog-a "Yet" "another" "test" >outfile3-a 2>&1
:	$(TEST_ENV) ./prog-so "Yet" "another" "test" >outfile3-so 2>&1
:	cmp outfile3 outfile3-a
:	cmp outfile3-a outfile3-so