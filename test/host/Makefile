# Host-side tests for Arduino library code.
# Compiles non-Arduino-specific code with the system C++ compiler so we can
# unit-test it on a normal laptop. Run: `make` (or `make run`).

CXX      ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O0 -g \
            -I../../src -DUNIT_TEST

SOURCES = ../../src/DebouncedButton.cpp \
          test_DebouncedButton.cpp \
          test_RingBuffer.cpp \
          main.cpp

OUT = ./.bin/tests

.PHONY: all run clean

all: $(OUT)

$(OUT): $(SOURCES)
	@mkdir -p .bin
	$(CXX) $(CXXFLAGS) -o $@ $^

run: $(OUT)
	$(OUT)

clean:
	rm -rf .bin
