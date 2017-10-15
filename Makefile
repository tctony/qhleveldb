SOURCES = \
  util/status.cc \
  util/env.cc \
	util/env_posix.cc \
  util/arena.cc \
	table/table.cc

TESTS = \
  util/env_test \
  util/arena_test \
	db/skiplist_test

PROGRNAMES := $(notdir $(TESTS))

CC = cc
CXX = c++
CFLAGS += -I. -I./include
CXXFLAGS += -I. -I./include -std=c++0x
LDFLAGS +=
LIBS +=

OUT_DIR = build

STATIC_OUTDIR = $(OUT_DIR)/static

STATIC_PROGRAMS := $(addprefix $(STATIC_OUTDIR)/, $(PROGRNAMES))

STATIC_LIBOBJECTS := $(addprefix $(STATIC_OUTDIR)/, $(SOURCES:.cc=.o))

STATIC_TESTOBJS := $(addprefix $(STATIC_OUTDIR)/, $(addsuffix .o, $(TESTS)))

TESTUTIL := $(STATIC_OUTDIR)/util/testutil.o
TESTHARNESS := $(STATIC_OUTDIR)/util/testharness.o $(TESTUTIL)

STATIC_ALLOBJS := $(STATIC_LIBOBJECTS) $(STATIC_TESTOBJS) $(TESTHARNESS)

default: all

all: $(STATIC_OUTDIR)/libqhleveldb.a $(STATIC_PROGRAMS)

clean:
	-rm -rf $(OUT_DIR)

test: $(STATIC_PROGRAMS)
	for t in $(notdir $(STATIC_PROGRAMS)); do echo "***** Running $$t"; $(STATIC_OUTDIR)/$$t || exit 1; done

$(OUT_DIR):
	mkdir $@

$(STATIC_OUTDIR): | $(OUT_DIR)
	mkdir $@

$(STATIC_OUTDIR)/db: | $(STATIC_OUTDIR)
	mkdir $@

$(STATIC_OUTDIR)/util: | $(STATIC_OUTDIR)
	mkdir $@

$(STATIC_OUTDIR)/table: | $(STATIC_OUTDIR)
	mkdir $@

.PHONY: STATIC_OBJDIRS
STATIC_OBJDIRS: \
	$(STATIC_OUTDIR)/db \
	$(STATIC_OUTDIR)/util \
	$(STATIC_OUTDIR)/table

$(STATIC_ALLOBJS): | STATIC_OBJDIRS

$(STATIC_OUTDIR)/libqhleveldb.a: $(STATIC_LIBOBJECTS)
	rm -rf $@
	$(AR) -rs $@ $(STATIC_LIBOBJECTS)

$(STATIC_OUTDIR)/env_test: util/env_test.cc $(STATIC_LIBOBJECTS) $(TESTHARNESS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) util/env_test.cc $(STATIC_LIBOBJECTS) $(TESTHARNESS) -o $@ $(LIBS)

$(STATIC_OUTDIR)/arena_test: util/arena_test.cc $(STATIC_LIBOBJECTS) $(TESTHARNESS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) util/arena_test.cc $(STATIC_LIBOBJECTS) $(TESTHARNESS) -o $@ $(LIBS)

$(STATIC_OUTDIR)/skiplist_test: db/skiplist_test.cc $(STATIC_LIBOBJECTS) $(TESTHARNESS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) db/skiplist_test.cc $(STATIC_LIBOBJECTS) $(TESTHARNESS) -o $@ $(LIBS)

$(STATIC_OUTDIR)/%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(STATIC_OUTDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
