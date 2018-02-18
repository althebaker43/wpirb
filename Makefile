
include include.mk

CXXFLAGS += -g -Wall -Werror

MODULES= \
	FieldControlSystem \
	IterativeRobot \
	Timer \
	Joystick \
	LiveWindow \
	SmartDashboard \
	Command \
	CommandGroup \
	Subsystem \
	Scheduler \
	RedBot \
	IOBuffer \
	Main
OBJS=$(MODULES:%=%.o)
LIB=libwpirb.a

LDFLAGS += -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt

TEST_MODULES= \
	TestUtils \
	TestIterativeRobot \
	TestRedBot \
	TestIOBuffer
TEST_OBJS=$(TEST_MODULES:%=%.o)
TEST_RUNNER=runTests

DEPENDS = $(MODULES:%=%.d) $(TEST_MODULES:%=%.d)

COMPONENT_DIR = component
COMPONENT_LIB = $(COMPONENT_DIR)/libcomponent.a
CPPFLAGS += -I$(COMPONENT_DIR)

REDBOTCOMPONENTS_DIR = redBotComponents
REDBOTCOMPONENTS_LIB = $(REDBOTCOMPONENTS_DIR)/libredbotcomponents.a
CPPFLAGS += -I$(REDBOTCOMPONENTS_DIR)

ARDUINO_DIR = arduino

RESIDUE= \
	$(LIB) \
	$(OBJS) \
	$(TEST_OBJS) \
	$(TEST_RUNNER) \
	$(DEPENDS)


.PHONY : all
all : tags TAGS test

.PHONY : test
test : $(TEST_RUNNER)
	./$(TEST_RUNNER) -v

.PHONY : test_all
test_all : test
	$(MAKE) -C $(REDBOTCOMPONENTS_DIR) test
	$(MAKE) -C $(ARDUINO_DIR) test

$(TEST_RUNNER) : $(LIB) $(COMPONENT_LIB) $(REDBOTCOMPONENTS_LIB) $(TEST_OBJS) AllTests.cpp
	$(CXX) \
	    $(CPPFLAGS) \
	    $(CXXFLAGS) \
	    -o $@ \
	    AllTests.cpp $(TEST_OBJS) \
	    -L. -lwpirb -L$(COMPONENT_DIR) -lcomponent -L$(REDBOTCOMPONENTS_DIR) -lredbotcomponents $(LDFLAGS)

$(LIB) : $(OBJS)
	ar r $@ $^

$(COMPONENT_LIB) :
	$(MAKE) -C $(COMPONENT_DIR) lib

$(REDBOTCOMPONENTS_LIB) : $(COMPONENT_LIB)
	$(MAKE) -C $(REDBOTCOMPONENTS_DIR) lib

-include $(DEPENDS)

$(DEPENDS) : %.d : %.cpp
	$(CXX) $(CPPFLAGS) -MM $< > $@

tags : $(MODULES:%=%.cpp) $(MODULES:%=%.h)
	ctags $(MODULES:%=%.cpp) $(MODULES:%=%.h)

TAGS : $(MODULES:%=%.cpp) $(MODULES:%=%.h)
	etags -l c++ $(MODULES:%=%.cpp) $(MODULES:%=%.h)

.PHONY : clean
clean :
	$(MAKE) -C $(COMPONENT_DIR) clean
	$(MAKE) -C $(REDBOTCOMPONENTS_DIR) clean
	$(MAKE) -C $(ARDUINO_DIR) clean
	rm -rf $(RESIDUE)
