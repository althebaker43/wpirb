
include ../include.mk

CPPFLAGS += -I..
CXXFLAGS += -g

RESIDUE = \
	joystickTest


joystickTest : joystickTest.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< -L.. -lwpirb $(LDFLAGS)

.PHONY : clean
clean :
	rm -rf $(RESIDUE)
