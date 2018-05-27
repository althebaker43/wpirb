
WPIRB_DIR = ..
COMPONENT_DIR = $(WPIRB_DIR)/component
REDBOTCOMPONENTS_DIR = $(WPIRB_DIR)/redBotComponents

LDFLAGS += \
	-L$(WPIRB_DIR) -lwpirb \
	-L$(REDBOTCOMPONENTS_DIR) -lredbotcomponents \
	-L$(COMPONENT_DIR) -lcomponent

include ../include.mk

MODULES= \
	WPIRBRobot
OBJS=$(MODULES:%=%.o)

TEST_MODULES= \
	TestWPIRBRobot \
	Arduino \
	RedBot
TEST_OBJS=$(TEST_MODULES:%=%.o)
TEST_RUNNER=runTests

DEPENDS = $(MODULES:%=%.d) $(TEST_MODULES:%=%.d)

CPPFLAGS += -I$(WPIRB_DIR) -I$(REDBOTCOMPONENTS_DIR) -I$(COMPONENT_DIR) -DRB=rb
CXXFLAGS = -g -Wall -Werror
LDFLAGS += -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt

ARDUINO_ROOT = $(HOME)/Development/redbot/arduino-1.6.3

RESIDUE= \
	$(LIB) \
	$(OBJS) \
	$(TEST_OBJS) \
	$(TEST_RUNNER) \
	$(DEPENDS)


.PHONY : test
test : $(TEST_RUNNER)
	./$(TEST_RUNNER)

$(TEST_RUNNER) : $(OBJS) $(TEST_OBJS)
	$(CXX) \
	    $(CPPFLAGS) \
	    $(CXXFLAGS) \
	    -o $@ \
	    AllTests.cpp $(OBJS) $(TEST_OBJS) \
	    $(LDFLAGS)

-include $(DEPENDS)

$(DEPENDS) : %.d : %.cpp
	$(CXX) $(CPPFLAGS) -MM $< > $@

.PHONY : update_arduino
update_arduino :
	cp -u $(MODULES:%=%.cpp) $(MODULES:%=%.h) $(ARDUINO_ROOT)/libraries/WPIRBRobot/

.PHONY : clean
clean :
	rm -rf $(RESIDUE)
