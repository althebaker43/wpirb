
WPIRB_DIR = ../..
COMPONENT_DIR = $(WPIRB_DIR)/component
REDBOTCOMPONENTS_DIR = $(WPIRB_DIR)/redBotComponents

LDFLAGS += \
	-L$(WPIRB_DIR) -lwpirb \
	-L$(REDBOTCOMPONENTS_DIR) -lredbotcomponents \
	-L$(COMPONENT_DIR) -lcomponent -ldl -pthread

include ../../include.mk

CPPFLAGS += \
	-I$(WPIRB_DIR) \
	-I$(COMPONENT_DIR) \
	-I$(REDBOTCOMPONENTS_DIR)

RESIDUE= \
	runRobot

runRobot : Robot.cpp
	$(CXX) \
	    $(CPPFLAGS) \
	    $(CXXFLAGS) \
	    -o runRobot \
	    $(SOURCE) \
	    $(LDFLAGS)

.PHONY : clean
clean :
	rm -rf $(RESIDUE)
