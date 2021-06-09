.DEFAULT: all
.SUFFIXES:

SHELL:=/bin/bash
SRCDIR:=$(patsubst %/,%,$(dir $(lastword $(MAKEFILE_LIST))))
BLDDIR:=$(CURDIR)/bld
SOURCES=$(filter-out %/example.cpp,$(wildcard $(SRCDIR)/test/*.cpp))
OBJECTS=$(patsubst $(SRCDIR)/test/%.cpp,$(BLDDIR)/%.o,$(SOURCES))
CXXFLAGS=-std=c++14 -fprofile-arcs -ftest-coverage -I$(SRCDIR)/include

.PHONY: all
all: $(BLDDIR)/test

.PHONY: test
test: $(BLDDIR)/test
	$(BLDDIR)/test
	@gcov -t -o bld include/html.hpp 2>/dev/null | \
		./test/gcovfilter.py -i .hpp > bld/html.hpp.gcov

$(BLDDIR)/test: $(OBJECTS)
	$(LINK.cpp) $(OUTPUT_OPTION) $^ -pthread -lgtest

$(BLDDIR)/%.o: $(SRCDIR)/test/%.cpp $(SRCDIR)/include/html.hpp | $(BLDDIR)/
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<
    
$(BLDDIR)/:
	@mkdir -p $(BLDDIR)

.PHONY: clean
clean:
	@rm -f $(BLDDIR)/test $(OBJECTS)
	@rm -rf $(BLDDIR)/networking-ts-impl-master $(BLDDIR)/example*
	@rm -rf $(BLDDIR)/*.gcov $(BLDDIR)/*.gcda $(BLDDIR)/*.gcno
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
