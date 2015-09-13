
WPIRB_DIR = ..
COMPONENT_DIR = $(WPIRB_DIR)/component
REDBOTCOMPONENTS_DIR = $(WPIRB_DIR)/redBotComponents

LDFLAGS += \
	-L$(WPIRB_DIR) -lwpirb \
	-L$(REDBOTCOMPONENTS_DIR) -lredbotcomponents \
	-L$(COMPONENT_DIR) -lcomponent

include ../include.mk


CPPFLAGS += \
	-I$(WPIRB_DIR) \
	-I$(COMPONENT_DIR) \
	-I$(REDBOTCOMPONENTS_DIR)
CXXFLAGS += -g

RESIDUE = \
	joystickTest


joystickTest : joystickTest.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $< -L.. -lwpirb $(LDFLAGS)

.PHONY : clean
clean :
	rm -rf $(RESIDUE)
