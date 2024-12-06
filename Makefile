CXX := g++
CXXFLAGS := -std=c++23 -DLOCAL -Wall -Wextra -Wno-sign-conversion -O2

.DEFAULT_GOAL := compile

OUTPUT_FILE := a.out
SUBMIT_FILE := submit.cpp

compile: submit
	$(CXX) $(CXXFLAGS) $(FILE) -o $(OUTPUT_FILE)

new: 
	@read -p "File name: " file_name; \
	cp "template.cpp" "$$file_name" && echo "New file '$$file_name' created."

submit: 
	(echo "#include <bits/stdc++.h>"; $(CXX) -std=c++23 -E -P -DPREPROCESS $(FILE) | sed '/^ *$$/d') > $(SUBMIT_FILE)

clean:
	rm -f $(OUTPUT_FILE)

.PHONY: compile new clean submit

