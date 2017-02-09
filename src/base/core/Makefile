CXXFLAGS += -I$(SRC_DIR)
CXXFLAGS += -std=c++11 -Wall -Werror -Wno-sign-compare -Wno-deprecated -g -c -o

CXX=g++

CPP_SOURCES :=	\
	atomicops-internals-x86.cc \
	callback_internal.cc	\
	ref_counted.cc \
	ref_counted_memory.cc \

CPP_OBJECTS:=$(CPP_SOURCES:.cc=.o)

STATIC_LIB=libcore.a

LIBS += $(STATIC_LIB)

all: $(CPP_OBJECTS) $(LIBS)

$(STATIC_LIB): $(CPP_OBJECTS)
	@echo "  [LINK] $@"
	@ar rs $(STATIC_LIB) $(CPP_OBJECTS)

.cc.o:
	@echo "  [CXX]  $@"
	@$(CXX) $(CXXFLAGS) $@ $<

clean:
	@rm -fr $(CPP_OBJECTS)
	@rm -fr $(LIBS)
