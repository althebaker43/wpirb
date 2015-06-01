
include ../../include.mk

CPPFLAGS += -I../..

RESIDUE= \
	runRobot

runRobot : Robot.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o runRobot $(SOURCE) -lwpirb -L../.. $(LDFLAGS)

.PHONY : clean
clean :
	rm -rf $(RESIDUE)
