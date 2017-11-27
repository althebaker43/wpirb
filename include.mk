
CPPUTEST_HOME = $(HOME)/Development/tdd/cpputest-3.6
CPPFLAGS += -I$(CPPUTEST_HOME)/include
#CXXFLAGS += -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorNewMacros.h

SDL_ROOT = /home/allen/Development/first/sdl/SDL2-build
CPPFLAGS += -I$(SDL_ROOT)/include
LDFLAGS += -L$(SDL_ROOT)/lib -lSDL2

CPPFLAGS += -std=c++14

WPIUTIL_ROOT = /home/allen/Development/first/wpiutil
CPPFLAGS += -I$(WPIUTIL_ROOT)/src/main/native/include
LDFLAGS += -L$(WPIUTIL_ROOT)/build/libs/wpiutil/shared -lwpiutil
export LD_LIBRARY_PATH := $(WPIUTIL_ROOT)/build/libs/wpiutil/shared:$(LD_LIBRARY_PATH)

NTCORE_ROOT = /home/allen/Development/first/ntcore
CPPFLAGS += -I$(NTCORE_ROOT)/src/main/native/include
LDFLAGS += -L$(NTCORE_ROOT)/build/libs/ntcore/shared -lntcore
export LD_LIBRARY_PATH := $(NTCORE_ROOT)/build/libs/ntcore/shared:$(LD_LIBRARY_PATH)
