CXXFLAGS = -O2 -g -Wall -fmessage-length=0 -std=c++11

LIBS =

SRCDIR = src
OBJDIR = obj
BINDIR = bin

TARGET =$(BINDIR)/DemoCpp11

SRCS = DemoCpp11.cpp \
smart_pointers.cpp \
type_support.cpp \
range_based_loops.cpp \
initialization.cpp \
pitfalls.cpp

VPATH= $(SRCDIR)

all:	$(TARGET)

$(OBJDIR)/%.o : %.cpp $(OBJDIR)
	$(CXX) -c $(CXXFLAGS) $< -o $@
$(OBJDIR):
	mkdir -p $(OBJDIR)

OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))
$(TARGET):	$(OBJS) $(BINDIR)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)
$(BINDIR):
	mkdir -p $(BINDIR)


clean:
	rm -f $(OBJS) $(TARGET)
	rmdir $(OBJDIR) $(BINDIR)
