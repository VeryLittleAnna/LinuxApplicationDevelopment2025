.RECIPEPREFIX=:
SRC = main.c
BINARIES = original patch1 patch2 patch3
PATCHES = patch1.patch patch2.patch patch3.patch
.PHONY: all run clean

SEED = 12345
CHARS = '\#.'
SIZE = 6

all: $(BINARIES)

original: $(SRC)
:	gcc -o $@ $<

patch1.c: $(SRC) patch1.patch
:	patch -o $@ $(SRC) patch1.patch

patch2.c: patch1.c patch2.patch
:	patch -o $@ patch1.c patch2.patch

patch3.c: patch2.c patch3.patch
:	patch -o $@ patch2.c patch3.patch

patch1: patch1.c
:	gcc -o $@ $<

patch2: patch2.c
:	gcc -o $@ $<

patch3: patch3.c
:	gcc -o $@ $<

run: $(BINARIES)
:	@echo "         === Original ==="
:	./original
:	@echo ""
:	@echo "         === Run patch1 (parameter - size) ==="
:	./patch1 $(SIZE)
:	@echo ""
:	@echo "         === Run patch2 (<path><wall> size) ==="
:	./patch2 $(CHARS) $(SIZE)
:	@echo ""
:	@echo "=== Run patch3 (seed <path><wall> + size) ==="
:	./patch3 $(SEED) $(CHARS) $(SIZE)

clean:
:	rm -f $(BINARIES) patch1.c patch2.c patch3.c *.orig

