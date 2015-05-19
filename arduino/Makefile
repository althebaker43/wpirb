
include ../include.mk

MODULES= \
	WPIRBRobot \
	Arduino
OBJS=$(MODULES:%=%.o)

TEST_MODULES= \
	TestWPIRBRobot
TEST_OBJS=$(TEST_MODULES:%=%.o)
TEST_RUNNER=runTests

CPPFLAGS += -I$(CPPUTEST_HOME)/include
CXXFLAGS = -g -Wall -Werror
LD_LIBRARIES += -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt

RESIDUE= \
	$(LIB) \
	$(OBJS) \
	$(TEST_OBJS) \
	$(TEST_RUNNER)


.PHONY : test
test : $(TEST_RUNNER)
	./$(TEST_RUNNER)

$(TEST_RUNNER) : $(OBJS) $(TEST_OBJS)
	$(CXX) \
	    $(CPPFLAGS) \
	    $(CXXFLAGS) \
	    -o $@ \
	    AllTests.cpp $(OBJS) $(TEST_OBJS) \
	    $(LD_LIBRARIES)

.PHONY : clean
clean :
	rm -rf $(RESIDUE)
