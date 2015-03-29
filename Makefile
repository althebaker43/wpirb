
CPPFLAGS= \
	-Wall \
	-Werror

MODULES= \
	IterativeRobot \
	RobotDrive \
	Joystick \
	LiveWindow
OBJS=$(MODULES:%=%.o)
LIB=libwpirb.a

CPPUTEST_HOME = $(HOME)/Development/tdd/cpputest-build
CPPFLAGS += -I$(CPPUTEST_HOME)/include
CXXFLAGS += -include  $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorNewMacros.h
LD_LIBRARIES += -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt
TEST_MODULES= \
	TestIterativeRobot
TEST_OBJS=$(TEST_MODULES:%=%.o)
TEST_RUNNER=runTests

RESIDUE= \
	$(LIB) \
	$(OBJS) \
	$(TEST_OBJS) \
	$(TEST_RUNNER)


.PHONY : test
test : $(TEST_RUNNER)
	./$(TEST_RUNNER)

$(TEST_RUNNER) : $(LIB) $(TEST_OBJS)
	$(CXX) \
	    $(CPPFLAGS) \
	    $(CXXFLAGS) \
	    -o $@ \
	    AllTests.cpp $(TEST_OBJS) libwpirb.a \
	    $(LD_LIBRARIES)

$(LIB) : $(OBJS)
	ar r $@ $^

.PHONY : clean
clean :
	rm -rf $(RESIDUE)
