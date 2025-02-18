LIB_DIR = /home/rayb/Projects/comp-programming
CXX := g++
CXXFLAGS := -std=c++23 -DLOCAL -Wall -Wextra -Wno-sign-conversion -O2 -I$(LIB_DIR)
CXXPPFLAGS := -std=c++23 -E -P -DPREPROCESS -I$(LIB_DIR)

.DEFAULT_GOAL := compile

OUTPUT_FILE := a.out
SUBMIT_FILE := submit.cpp

compile: submit
	$(CXX) $(CXXFLAGS) $(FILE) -o $(OUTPUT_FILE)

copy:
	cat $(SUBMIT_FILE) | wl-copy

submit: 
	(echo "#include <bits/stdc++.h>"; $(CXX) $(CXXPPFLAGS) $(FILE) | sed '/^ *$$/d') > $(SUBMIT_FILE)

new: 
	@read -p "File name: " file_name; \
	cp "$(LIB_DIR)/template.cpp" "$$file_name" && echo "New file '$$file_name' created."

clean:
	rm -f $(OUTPUT_FILE) && rm -f $(SUBMIT_FILE)

.PHONY: compile new clean submit copy

