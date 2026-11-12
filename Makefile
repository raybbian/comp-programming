# Resolve the library root from this Makefile's own location so the repo
# works wherever it is checked out.
LIB_DIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

# Bare `make` builds main.cpp; override with `make FILE=other.cpp`.
FILE ?= main.cpp
SRC := $(dir $(FILE))
NAME := $(basename $(notdir $(FILE)))

# Everything generated lands in out/, which is gitignored.
OUT := out

# `make stress TESTS=500 SEED=100 TL=2` runs seeds 100..599, killing a brute
# force or solution that runs longer than TL seconds.
TESTS ?= 1000
SEED ?= 1
TL ?= 10

# The submission pipes g++ into sed; without pipefail a preprocessor failure is
# masked by sed's exit status.
SHELL := /bin/bash
.SHELLFLAGS := -o pipefail -c

CXX := g++
CXXFLAGS = -std=c++23 -fsanitize=address -Wall -Wextra -Wno-sign-conversion -O2 -g -I$(LIB_DIR) -MMD -MP -MF $@.d

# out/a.out is FILE built with dbg() on, out/submit.cpp its single-file
# submission.
compile: $(OUT)/$(NAME).out
	@ln -f $< $(OUT)/a.out
	@(echo -e "#include <bits/stdc++.h>\n#include <ext/pb_ds/assoc_container.hpp>\n#include <ext/pb_ds/tree_policy.hpp>"; $(CXX) -std=c++23 -E -P -DPREPROCESS -I$(LIB_DIR) $(FILE) | sed '/^ *$$/d') > $(OUT)/submit.cpp

# Binaries are named after their source and rebuilt only when it or a header it
# includes changes. .bin drops -DLOCAL so dbg() cannot pollute a stress run.
$(OUT)/%.out: $(SRC)%.cpp
	@mkdir -p $(OUT)
	$(CXX) $(CXXFLAGS) -DLOCAL $< -o $@

$(OUT)/%.bin: $(SRC)%.cpp
	@mkdir -p $(OUT)
	$(CXX) $(CXXFLAGS) $< -o $@

-include $(wildcard $(OUT)/*.d)

# Brute force and generator live next to the solution FILE: x.cpp, x.brute.cpp,
# x.gen.cpp. The generator is run with the seed as its argument.
bins: $(OUT)/$(NAME).bin $(OUT)/$(NAME).brute.bin $(OUT)/$(NAME).gen.bin

# Stops at the first crash, timeout or mismatch, leaving
# out/{input,expected,actual} behind; `out/a.out < out/input` replays it.
stress: bins
	@cd $(OUT); for ((i = $(SEED); i < $(SEED) + $(TESTS); i++)); do \
	    rm -f expected actual; \
	    ./$(NAME).gen.bin $$i > input && \
	    timeout $(TL) ./$(NAME).brute.bin < input > expected && \
	    timeout $(TL) ./$(NAME).bin < input > actual && \
	    diff -bB expected actual > /dev/null || \
	    { echo "test $$i failed (exit $$?)"; head -n 40 input expected actual; exit 1; }; \
	done; echo "$(TESTS) tests passed"

new:
	@read -p "File name: " f; cp "$(LIB_DIR)/templates/template.cpp" "$$f"

copy:
	wl-copy < $(OUT)/submit.cpp

# Also removes brute forces and generators in the repo root, never solutions.
clean:
	rm -rf $(OUT) $(LIB_DIR)/*.brute.cpp $(LIB_DIR)/*.gen.cpp

.PHONY: compile bins stress new copy clean
