PROGNAME := agi3d
#release or debug
buildtype := release
CXX := g++
CPPFLAGS += -std=c++11 -stdlib=libc++
#CPPFLAGS +=
LDFLAGS +=
LBFGSLIB = -llbfgs
WXLIBS = `wx-config --libs --gl-libs`
FRAMEWORK = -framework OpenGL
FRAMEWORK += -framework GLUT
FRAMEWORK += -framework Accelerate
FRAMEWORK += -framework IOKit
FRAMEWORK += -framework Cocoa
LIBS += -lpng15 $(FRAMEWORK) -lglfw3 -lpthread -llapack -lblas $(LBFGSLIB) $(WXLIBS)
WXCONFIG = `wx-config --cxxflags`
INCLUDES := -I../vendor
DEFS := $(INCLUDES) $(WXCONFIG) 

SRCS = constraintsolver3d.cpp constraintsolver2d.cpp Enums.cpp Observer.cpp Observable.cpp UserDefault.cpp Graph.cpp Configuration.cpp MenuBar.cpp MenuBarController.cpp GraphicController.cpp  ControlPanelController.cpp AppDelegete.cpp GraphicPanel.cpp ControlPanel.cpp Frame.cpp AppearanceWindow.cpp App.cpp

#CXXFLAGS := -Wall -Wextra
CXXFLAGS := -Wall -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function
ifeq ($(buildtype),release)
  CXXFLAGS += -O3 -DNDEBUG
else ifeq ($(buildtype),debug)
  CXXFLAGS += -O0 -g
else
  $(error buildtype must be release, debug, profile or coverage)
endif

OUTDIR := ../build/$(buildtype)

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


