
CPPUTEST_HOME = $(HOME)/Development/tdd/cpputest-3.6
CPPFLAGS += -I$(CPPUTEST_HOME)/include
#CXXFLAGS += -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorNewMacros.h

SDL_ROOT = /home/allen/Development/first/sdl/SDL2-build
CPPFLAGS += -I$(SDL_ROOT)/include
LDFLAGS += -L$(SDL_ROOT)/lib -lSDL2
