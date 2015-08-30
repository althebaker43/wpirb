
include include.mk

CXXFLAGS += -g -Wall -Werror

MODULES= \
	FieldControlSystem \
	IterativeRobot \
	RobotDrive \
	DigitalOutput \
	DigitalInput \
	AnalogInput \
	Timer \
	Joystick \
	LiveWindow \
	RedBot \
	IOBuffer \
	Packet \
	RedBotPacket \
	PinConfigPacket \
	Component \
	Main
OBJS=$(MODULES:%=%.o)
LIB=libwpirb.a

LDFLAGS += -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt

TEST_MODULES= \
	TestUtils \
	TestIterativeRobot \
	TestRedBot \
	TestPackets \
	TestComponents \
	TestIOBuffer
TEST_OBJS=$(TEST_MODULES:%=%.o)
TEST_RUNNER=runTests

DEPENDS = $(MODULES:%=%.d) $(TEST_MODULES:%=%.d)

RESIDUE= \
	$(LIB) \
	$(OBJS) \
	$(TEST_OBJS) \
	$(TEST_RUNNER) \
	$(DEPENDS)


.PHONY : all
all : tags test

.PHONY : test
test : $(TEST_RUNNER)
	./$(TEST_RUNNER)

$(TEST_RUNNER) : $(LIB) $(TEST_OBJS)
	$(CXX) \
	    $(CPPFLAGS) \
	    $(CXXFLAGS) \
	    -o $@ \
	    AllTests.cpp $(TEST_OBJS) libwpirb.a \
	    $(LDFLAGS)

$(LIB) : $(OBJS)
	ar r $@ $^

-include $(DEPENDS)

$(DEPENDS) : %.d : %.cpp
	$(CXX) $(CPPFLAGS) -MM $< > $@

tags : $(MODULES:%=%.cpp) $(MODULES:%=%.h)
	ctags $(MODULES:%=%.cpp) $(MODULES:%=%.h)

.PHONY : clean
clean :
	rm -rf $(RESIDUE)
