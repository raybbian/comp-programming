# Resolve the library root from this Makefile's own location so the repo
# works wherever it is checked out.
LIB_DIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

# Bare `make` (e.g. from editor compile commands) builds main.cpp;
# override with `make FILE=other.cpp`.
FILE ?= main.cpp

# Brute force and generator live next to the solution: x.cpp, x.brute.cpp,
# x.gen.cpp. The generator is run as `gen.out <seed>`.
STEM := $(patsubst %.brute,%,$(patsubst %.gen,%,$(basename $(FILE))))
BRUTE_FILE ?= $(STEM).brute.cpp
GEN_FILE ?= $(STEM).gen.cpp

# `submit` pipes g++ into sed; without pipefail a preprocessor failure is
# masked by sed's exit status and a truncated submit.cpp gets written.
SHELL := /bin/bash
.SHELLFLAGS := -o pipefail -c

CXX := g++
CXXFLAGS := -std=c++23 -fsanitize=address -DLOCAL -Wall -Wextra -Wno-sign-conversion -O2 -g -I$(LIB_DIR)
CXXPPFLAGS := -std=c++23 -E -P -DPREPROCESS -I$(LIB_DIR)
# Stress binaries drop -DLOCAL so dbg() output cannot pollute stdout.
STRESS_FLAGS := $(filter-out -DLOCAL,$(CXXFLAGS))

.DEFAULT_GOAL := compile

OUTPUT_FILE := a.out
SUBMIT_FILE := submit.cpp
SOL_BIN := sol.out
BRUTE_BIN := brute.out
GEN_BIN := gen.out

# `make stress TESTS=500 SEED=100 TL=2` runs seeds 100..599, killing a brute
# force or solution that runs longer than TL seconds.
STRESS_DIR := stress
TESTS ?= 100
SEED ?= 1
TL ?= 10

compile: submit
	$(CXX) $(CXXFLAGS) $(FILE) -o $(OUTPUT_FILE)

copy:
	cat $(SUBMIT_FILE) | wl-copy

submit: 
	(echo -e "#include <bits/stdc++.h>\n#include <ext/pb_ds/assoc_container.hpp>\n#include <ext/pb_ds/tree_policy.hpp>"; $(CXX) $(CXXPPFLAGS) $(FILE) | sed '/^ *$$/d') > $(SUBMIT_FILE)

new: 
	@read -p "File name: " file_name; \
	cp "$(LIB_DIR)/template.cpp" "$$file_name" && echo "New file '$$file_name' created."

sol:
	$(CXX) $(STRESS_FLAGS) $(FILE) -o $(SOL_BIN)

brute:
	$(CXX) $(STRESS_FLAGS) $(BRUTE_FILE) -o $(BRUTE_BIN)

gen:
	$(CXX) $(STRESS_FLAGS) $(GEN_FILE) -o $(GEN_BIN)

# For each seed: gen -> input, brute -> expected, solution -> actual. Stops at
# the first crash or mismatch, leaving stress/{seed,input,expected,actual}
# behind; `a.out < stress/input` replays it with dbg() enabled. On success
# stress/ is removed.
stress: sol brute gen
	@rm -rf $(STRESS_DIR) && mkdir -p $(STRESS_DIR); \
	in=$(STRESS_DIR)/input; exp=$(STRESS_DIR)/expected; act=$(STRESS_DIR)/actual; \
	show() { echo "--- $$2 ($$1)"; head -n 40 "$$1"; \
	         [ "$$(wc -l < "$$1")" -gt 40 ] && echo "... (truncated)"; true; }; \
	for ((i = $(SEED); i < $(SEED) + $(TESTS); i++)); do \
	    echo $$i > $(STRESS_DIR)/seed; rm -f $$exp $$act; \
	    ./$(GEN_BIN) $$i > $$in || { echo "test $$i: generator failed"; exit 1; }; \
	    timeout $(TL)s ./$(BRUTE_BIN) < $$in > $$exp; code=$$?; \
	    if [ $$code -ne 0 ]; then \
	        [ $$code -eq 124 ] && echo "test $$i: brute force timed out ($(TL)s)" \
	                           || echo "test $$i: brute force exited with code $$code"; \
	        show $$in input; exit 1; \
	    fi; \
	    timeout $(TL)s ./$(SOL_BIN) < $$in > $$act; code=$$?; \
	    if [ $$code -ne 0 ]; then \
	        [ $$code -eq 124 ] && echo "test $$i: solution timed out ($(TL)s)" \
	                           || echo "test $$i: solution exited with code $$code"; \
	        show $$in input; \
	        echo "--- replay: ./$(OUTPUT_FILE) < $$in"; exit 1; \
	    fi; \
	    if ! diff -bB $$exp $$act > /dev/null; then \
	        echo "test $$i: wrong answer"; \
	        show $$in input; show $$exp expected; show $$act actual; \
	        echo "--- replay: ./$(OUTPUT_FILE) < $$in"; exit 1; \
	    fi; \
	done; \
	echo "stress: $(TESTS) tests passed (seeds $(SEED)..$$((i - 1)))"; \
	rm -rf $(STRESS_DIR)

# Brute forces and generators in the repo root. Solutions are never removed.
WORK_FILES := $(wildcard $(LIB_DIR)/*.brute.cpp $(LIB_DIR)/*.gen.cpp)

clean:
	rm -f $(OUTPUT_FILE) $(SUBMIT_FILE) $(SOL_BIN) $(BRUTE_BIN) $(GEN_BIN) $(WORK_FILES) && rm -rf $(STRESS_DIR)

.PHONY: compile new clean submit copy sol brute gen stress
