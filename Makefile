PROGNAME := agi3d
#release or debug
buildtype := release
BIN_WXCONFIG :=$(HOME)/homebrew/bin/wx-config
	
CXX := g++
CPPFLAGS += 
LDFLAGS +=
LBFGSLIB = -llbfgs
WXLIBS = `$(BIN_WXCONFIG) --libs --gl-libs`
LIBS += -lpng15 $(FRAMEWORK) -lpthread -llapack -lblas $(LBFGSLIB) $(WXLIBS)
WXCONFIG = `$(BIN_WXCONFIG) --cxxflags`
INCLUDES := -Ivendor
DEFS := $(INCLUDES) $(WXCONFIG) 
FRAMEWORK = -framework OpenGL
FRAMEWORK += -framework GLUT
FRAMEWORK += -framework Accelerate

VPATH = src 
SRCS = DrawPanel.cpp SubPanel.cpp MainFrame.cpp AppearanceWindow.cpp Main.cpp calcLayout.cpp constraintsolver3d.cpp constraintsolver2d.cpp

CXXFLAGS := -Wall -Wextra
ifeq ($(buildtype),release)
  CXXFLAGS += -O3 -DNDEBUG
else ifeq ($(buildtype),debug)
  CXXFLAGS += -O0 -g
else
  $(error buildtype must be release, debug, profile or coverage)
endif

OUTDIR := build/$(buildtype)

PROG := $(OUTDIR)/$(PROGNAME)
OBJS := $(SRCS:%.cpp=$(OUTDIR)/%.o)
DEPS := $(SRCS:%.cpp=$(OUTDIR)/%.d)

.PHONY: install clean distclean

all: $(PROG)

-include $(DEPS)

$(PROG): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

$(OUTDIR)/%.o:%.cpp
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(DEFS) -o $@ -c -MMD -MP -MF $(@:%.o=%.d) $<

clean:
	rm -rf $(OUTDIR)

distclean:
	rm -rf build


