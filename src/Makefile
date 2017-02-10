CXXFLAGS += -I.
CXXFLAGS += -std=c++11 -Wall -Werror -Wno-char-subscripts -Wno-sign-compare -Wno-deprecated -g -c -o

CXX=g++

CPP_SOURCES :=	\
	base/core/atomicops-internals-x86.cc	\
	base/core/bits.cc \
	base/core/callback_internal.cc \
	base/core/cpu.cc \
	base/core/int128.cc \
	base/core/once.cc \
	base/core/ref_counted.cc \
	base/core/ref_counted_memory.cc \
	base/core/spinlock_internal.cc \
	base/core/spinlock.cc \
	base/core/sysinfo.cc \
	base/hash/city.cc \
	base/hash/hash.cc \
	base/hash/jenkins.cc \
	base/math/mathlimits.cc \
	base/strings/ascii_ctype.cc \
	base/strings/charset.cc \
	base/strings/escaping.cc \
	base/strings/human_readable.cc \
	base/strings/join.cc \
	base/strings/memutil.cc \
	base/strings/numbers.cc \
	base/strings/serialize.cc \
	base/strings/split.cc \
	base/strings/strcat.cc \
	base/strings/stringpiece.cc \
	base/strings/stringprintf.cc \
	base/strings/strip.cc \
	base/strings/strtoint.cc \
	base/strings/substitute.cc \
	base/strings/util.cc \
	base/time/walltime.cc \
	base/threading/thread_collision_warner.cc \

CPP_OBJECTS:=$(CPP_SOURCES:.cc=.o)

CC=gcc
CFLAGS += -I. -Wall -Werror -std=c99 -g -c -o
C_SOURCES := base/utf/rune.c
C_OBJECTS := $(C_SOURCES:.c=.o)

STATIC_LIB=libcore.a

LIBS += $(STATIC_LIB)

all: $(C_OBJECTS) $(CPP_OBJECTS) $(LIBS)

$(STATIC_LIB): $(CPP_OBJECTS)
	@echo "  [LINK] $@"
	@ar rs $(STATIC_LIB) $(CPP_OBJECTS) $(C_OBJECTS)

.cc.o:
	@echo "  [CXX]  $@"
	@$(CXX) $(CXXFLAGS) $@ $<

.c.o:
	@echo "  [CC]   $@"
	@$(CC) $(CFLAGS) $@ $<

clean:
	@rm -fr $(CPP_OBJECTS)
	@rm -fr $(LIBS)
