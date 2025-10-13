.RECIPEPREFIX=:
.PHONY: clean all test
СС = cc
CFLAGS = -Wall -O0 -g
GDB_FLOWS = flow1.gdb flow2.gdb
GDB_OUT = flow1.out flow2.out
GENERATES = range flow1.out flow2.out
REFERENCES = reference1.txt reference2.txt

range: range.c
:	$(CC) $< $(CFLAGS) -o $@

all: range

clean:
:	rm -f $(GENERATES) $(GDB_OUT)

%.out: range %.gdb
:	gdb --batch --quiet --command=$*.gdb ./range | grep '^@@@' > $@

test: range $(GDB_OUT) 
:	@i=1; \
:	for ref in $(REFERENCES); do \
:		out=flow$$i.out; \
:		if diff -u "$$ref" "$$out"; then echo "$${out%.out}: OK"; else echo "$${out%.out}: FAIL"; \
:		fi; \
:		i=$$((i+1)); \
:	done; \
:	echo "$$((i-1)) tests completed!"