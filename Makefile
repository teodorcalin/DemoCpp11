CXXFLAGS = -g -Wall -fmessage-length=0 -std=c++14

LIBS =

SRCDIR = src
OBJDIR = obj
BINDIR = bin

TARGET =$(BINDIR)/DemoCpp11

SRCS = DemoCpp11.cpp \
constexpr.cpp \
bit_manipulation.cpp \
scoped_enum.cpp \
smart_pointers.cpp \
type_support.cpp \
range_based_loops.cpp \
initialization.cpp \
default_and_deleted_functions.cpp \
lambdas.cpp \
pitfalls.cpp \
casts.cpp \
rvalues.cpp

VPATH= $(SRCDIR)

all:	$(TARGET)

$(OBJDIR)/%.o : %.cpp | $(OBJDIR)
	$(CXX) -c $(CXXFLAGS) $< -o $@
$(OBJDIR):
	mkdir -p $(OBJDIR)

OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))
$(TARGET):	$(OBJS) | $(BINDIR)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)
$(BINDIR):
	mkdir -p $(BINDIR)


clean:
	rm -f $(OBJS) $(TARGET)
	rmdir $(OBJDIR) $(BINDIR)
