bin_PROGRAMS = rhasher
rhasher_SOURCES = rhasher.c

AM_CPPFLAGS = $(RHASH_CFLAGS)

if HAVE_READLINE
rhasher_CPPFLAGS = -DUSE_READLINE
rhasher_LDADD = $(LIBREADLINE) $(RHASH_LIBS)
else
rhasher_CPPFLAGS = -DUSE_READLINE=0
rhasher_LDADD = $(RHASH_LIBS)
endif

TESTS = test_basic.sh
EXTRA_DIST = $(TESTS)

check_SCRIPTS = test_basic.sh
test_basic.sh: Makefile
	echo '#!/bin/sh' > test_basic.sh
	echo 'cat > test_cmd.txt << "EOF"' >> test_basic.sh
	echo 'MD5 "hello"' >> test_basic.sh  
	echo 'EOF' >> test_basic.sh
	echo 'output=$$(./rhasher < test_cmd.txt)' >> test_basic.sh
	echo 'result=$$(echo "$$output" | grep -E "^[0-9a-f]{32}$$")' >> test_basic.sh
	echo 'expected="5d41402abc4b2a76b9719d911017c592"' >> test_basic.sh
	echo 'if [ "$$result" = "$$expected" ]; then' >> test_basic.sh
	echo '  echo "PASS: MD5 test"; exit 0' >> test_basic.sh
	echo 'else' >> test_basic.sh
	echo '  echo "FAIL: expected $$expected, got $$result"' >> test_basic.sh
	echo '  echo "Full output was:"' >> test_basic.sh
	echo '  echo "$$output"' >> test_basic.sh
	echo '  exit 1' >> test_basic.sh
	echo 'fi' >> test_basic.sh
	echo 'rm -f test_cmd.txt' >> test_basic.sh
	chmod +x test_basic.sh

clean-local:
	rm -f test_basic.sh test_cmd.txt

maintainer-clean-local:
	rm -rf autom4te.cache configure~
	rm -rf .deps
	rm -f aclocal.m4 compile config.h* configure depcomp install-sh Makefile.in missing stamp-h1 test-driver

distclean-local: maintainer-clean-local