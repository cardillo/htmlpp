.DEFAULT: all
.SUFFIXES:

SHELL:=/bin/bash
SRCDIR:=$(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
BLDDIR:=$(CURDIR)/bld
SOURCES=$(filter-out %/example.cpp,$(wildcard $(SRCDIR)/test/*.cpp))
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
	@rm -rf $(BLDDIR)/networking-ts-impl-master $(BLDDIR)/example*
	@rmdir $(BLDDIR)

.PHONY: lint
lint:
	./test/cpplint.py \
	    --extensions=hpp \
	    --filter=-legal/copyright \
	    $(SRCDIR)/include/*.hpp

.PHONY: example
example: $(BLDDIR)/example

$(BLDDIR)/example: CXXFLAGS += -I$(BLDDIR)/networking-ts-impl-master/include
$(BLDDIR)/example: $(BLDDIR)/example.o
	$(LINK.cpp) $(OUTPUT_OPTION) $^

$(BLDDIR)/example.o: | $(BLDDIR)/networking-ts-impl-master/

$(BLDDIR)/networking-ts-impl-master/:
	@curl -sSL https://github.com/chriskohlhoff/networking-ts-impl/archive/refs/heads/master.zip | tar -xC $(BLDDIR) -f-
