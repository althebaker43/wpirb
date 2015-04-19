
CXXFLAGS=-I../..
LDFLAGS=-lwpirb -L../..

RESIDUE= \
	runRobot

runRobot : Robot.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o runRobot Robot.cpp $(LDFLAGS)

.PHONY : clean
clean :
	rm -rf $(RESIDUE)
