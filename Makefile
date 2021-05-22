# A makefile to wrap execution of the local copy of SCons

SCONS = python tools/scons/scons.py

.PHONY: clean

all: test_runner

test_runner:
	$(SCONS)

test: test_runner
	./bin/test_runner

clean:
	$(SCONS) -c

distclean:
	$(SCONS) -c
	rm -rf build bin
