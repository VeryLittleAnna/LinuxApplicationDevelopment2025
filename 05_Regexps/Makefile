.RECIPEPREFIX = :
CC = cc
CFLAGS = -Wall 
GENERATES = esub *_out.txt

all:	esub

esub:	esub.c
:	$(CC) $< $(CFLAGS) -o $@

clean:
:	rm -f $(GENERATES)

test:	esub
:	@echo "Running tests...\n"

:	@echo "1"
:	@./esub "cat" "dog" "black cat on roof" > test_out.txt
:	@echo "black cat on roof" | sed -E 's/cat/dog/' > sed_out.txt
:	@diff test_out.txt sed_out.txt && echo "OK" || echo "WA"

:	@echo "2"
:	@./esub "([0-9]{2})/([0-9]{2})/([0-9]{4})" "\\3-\\2-\\1" "Date:	20/10/2025" > test_out.txt
:	@echo "Date:	20/10/2025" | sed -E 's/([0-9]{2})\/([0-9]{2})\/([0-9]{4})/\3-\2-\1/' > sed_out.txt
:	@diff test_out.txt sed_out.txt && echo "OK" || echo "WA"

:	@echo "3"
:	@./esub "slash" "back\\\\slash" "path with slash" > test_out.txt
:	@echo "path with slash" | sed -E 's/slash/back\\slash/' > sed_out.txt
:	@diff test_out.txt sed_out.txt && echo "OK" || echo "WA"

:	@echo "4"
:	@./esub "missing" "found" "text without pattern" > test_out.txt
:	@echo "text without pattern" | sed -E 's/missing/found/' > sed_out.txt
:	@diff test_out.txt sed_out.txt && echo "OK" || echo "WA"

:	@echo "5"
:	@./esub "(Alice) (Bob)" "\\2, \\1" "Alice Bob" > test_out.txt
:	@echo "Alice Bob" | sed -E 's/(Alice) (Bob)/\2, \1/' > sed_out.txt
:	@diff test_out.txt sed_out.txt && echo "OK" || echo "WA"

:	@echo "6"
:	@./esub "(([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3})\\.([0-9]{1,3}))" "IP:\\2.\\3.\\4.\\5" "Server:	192.168.1.1" > test_out.txt
:	@echo "Server:	192.168.1.1" | sed -E 's/(([0-9]{1,3})\.([0-9]{1,3})\.([0-9]{1,3})\.([0-9]{1,3}))/IP:\2.\3.\4.\5/' > sed_out.txt
:	@diff test_out.txt sed_out.txt && echo "OK" || echo "WA"

:	@echo "8"
:	@@./esub "([A-Z])([a-z])-([A-Z])([a-z])-([A-Z])([a-z])-([A-Z])([a-z])-([A-Z])([a-z])-([A-Z])([a-z])-([A-Z])([a-z])-([A-Z])([a-z])-([A-Z])([a-z])" "\\1\\2|\\3\\4|\\5\\6|\\7\\8|\\9\\10|\\11\\12|\\13\\14|\\15\\16|\\17\\18" "Aa-Bb-Cc-Dd-Ee-Ff-Gg-Hh-Ii" > test_out.txt
:	@@echo "Aa-Bb-Cc-Dd-Ee-Ff-Gg-Hh-Ii" | sed -E 's/([A-Z])([a-z])-([A-Z])([a-z])-([A-Z])([a-z])-([A-Z])([a-z])-([A-Z])([a-z])-([A-Z])([a-z])-([A-Z])([a-z])-([A-Z])([a-z])-([A-Z])([a-z])/\1\2|\3\4|\5\6|\7\8|\9\10|\11\12|\13\14|\15\16|\17\18/' > sed_out.txt
:	@diff test_out.txt sed_out.txt && echo "OK" || echo "WA"

:	@rm -f test_out.txt sed_out.txt
:	@echo "\nTests completed"

test-color:	esub
:	@echo "Testing colored output\n"

:	@echo "1"
:	@./esub -c "([0-9]{2})/([0-9]{2})/([0-9]{4})" "\\3-\\2-\\1" "20/10/2025"
:	@echo ""

:	@echo "2"
:	@./esub -c "(Alice) (Bob)" "\\2, \\1" "Alice Bob"
:	@echo ""

:	@echo "3"
:	@./esub -c "([A-Z])([a-z])-([A-Z])([a-z])-([A-Z])([a-z])-([A-Z])([a-z])-([A-Z])([a-z])-([A-Z])([a-z])-([A-Z])([a-z])-([A-Z])([a-z])-([A-Z])([a-z])" "\\1\\2|\\3\\4|\\5\\6|\\7\\8|\\9\\10|\\11\\12|\\13\\14|\\15\\16|\\17\\18" "Aa-Bb-Cc-Dd-Ee-Ff-Gg-Hh-Ii"
:	@echo ""

:	@echo "\nColor tests completed"

test-errors: esub
:	@echo "Testing errors\n"

:	@echo "1"
:	@./esub "[0-9" "replace" "test data" 2> test_err.txt || true
:	@grep -q "Regex error:" test_err.txt && echo "OK" || echo "WA" || true

:	@echo "2"
:	@./esub "(a)(b)" "\\1\\2\\5" "ab text" > test_out.txt 2> test_err.txt || true
:	@grep -q "Regex error:" test_err.txt && echo "OK" || echo "WA"

:	@rm -f test_err.txt test_out.txt
:	@echo "\nTests completed"

check:	test test-color test-errors
:	@echo "\n\nALL TESTS COMPLETED"

.PHONY:	all clean test test-color test-errors test-stress check