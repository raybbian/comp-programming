# Resolve the library root from this Makefile's own location so the repo
# works wherever it is checked out.
LIB_DIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

# Bare `make` (e.g. from editor compile commands) builds main.cpp;
# override with `make FILE=other.cpp`.
FILE ?= main.cpp

# `submit` pipes g++ into sed; without pipefail a preprocessor failure is
# masked by sed's exit status and a truncated submit.cpp gets written.
SHELL := /bin/bash
.SHELLFLAGS := -o pipefail -c

CXX := g++
CXXFLAGS := -std=c++23 -fsanitize=address -DLOCAL -Wall -Wextra -Wno-sign-conversion -O2 -g -I$(LIB_DIR)
CXXPPFLAGS := -std=c++23 -E -P -DPREPROCESS -I$(LIB_DIR)

.DEFAULT_GOAL := compile

OUTPUT_FILE := a.out
SUBMIT_FILE := submit.cpp

compile: submit
	$(CXX) $(CXXFLAGS) $(FILE) -o $(OUTPUT_FILE)

copy:
	cat $(SUBMIT_FILE) | wl-copy

submit: 
	(echo -e "#include <bits/stdc++.h>\n#include <ext/pb_ds/assoc_container.hpp>\n#include <ext/pb_ds/tree_policy.hpp>"; $(CXX) $(CXXPPFLAGS) $(FILE) | sed '/^ *$$/d') > $(SUBMIT_FILE)

new: 
	@read -p "File name: " file_name; \
	cp "$(LIB_DIR)/template.cpp" "$$file_name" && echo "New file '$$file_name' created."

clean:
	rm -f $(OUTPUT_FILE) && rm -f $(SUBMIT_FILE)

.PHONY: compile new clean submit copy

