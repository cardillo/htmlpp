.DEFAULT: all
.SUFFIXES:

SHELL:=/bin/bash
SRCDIR:=$(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
BLDDIR:=$(CURDIR)/bld
SOURCES=$(wildcard $(SRCDIR)/test/*.cpp)
OBJECTS=$(patsubst $(SRCDIR)/test/%.cpp,$(BLDDIR)/%.o,$(SOURCES))
CXXFLAGS=-std=c++14 -I$(SRCDIR)/include

.PHONY: all
all: $(BLDDIR)/test
	$(BLDDIR)/test

$(BLDDIR)/test: $(OBJECTS)
	$(LINK.cpp) $(OUTPUT_OPTION) $^ -lgtest

$(BLDDIR)/%.o: $(SRCDIR)/test/%.cpp $(SRCDIR)/include/html.hpp | $(BLDDIR)/
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<
    
$(BLDDIR)/:
	@mkdir -p $(BLDDIR)

.PHONY: clean
clean:
	@rm -f $(BLDDIR)/test $(OBJECTS)
	@rmdir $(BLDDIR)

.PHONY: lint
lint:
	./test/cpplint.py \
	    --extensions=hpp \
	    --filter=-legal/copyright \
	    $(SRCDIR)/include/*.hpp
