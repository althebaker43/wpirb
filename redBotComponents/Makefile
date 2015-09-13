
include ../include.mk

COMPONENT_ROOT = ../component

CPPFLAGS += \
	-I../ \
	-I$(COMPONENT_ROOT)

MODULES = \
	RedBotComponent \
	RobotDrive \
	DigitalOutput \
	DigitalInput \
	AnalogInput \
	ConfigurableInterface \
	RedBotPacket \
	XMLElement
OBJS = $(MODULES:%=%.o)
LIB = libredbotcomponents.a

LDFLAGS += \
	-L. -lredbotcomponents \
	-L$(COMPONENT_ROOT) -lcomponent \
	-L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt

TEST_MODULES = \
	TestPackets \
	TestComponents \
	TestUtils
TEST_OBJS = $(TEST_MODULES:%=%.o)
TEST_RUNNER = runTests

DEPENDS = $(MODULES:%=%.d) $(TEST_MODULES:%=%.d)

RESIDUE = \
	$(LIB) \
	$(OBJS) \
	$(TEST_OBJS) \
	$(TEST_RUNNER) \
	$(DEPENDS)


.PHONY : all
all : test

.PHONY : test
test  : $(TEST_RUNNER)
	./$(TEST_RUNNER)

$(TEST_RUNNER) : $(LIB) $(TEST_OBJS)
	$(CXX) \
	    $(CPPFLAGS) \
	    $(CXXFLAGS) \
	    -o $@ \
	    AllTests.cpp $(TEST_OBJS) \
	    $(LDFLAGS)

.PHONY : lib
lib : $(LIB)

$(LIB) : $(OBJS)
	ar ru $@ $^

-include $(DEPENDS)

$(DEPENDS) : %.d : %.cpp
	$(CXX) $(CPPFLAGS) -MM $< > $@

.PHONY : clean
clean :
	rm -rf $(RESIDUE)
