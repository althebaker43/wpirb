
include ../include.mk

MODULES = \
	Component \
	Packet
OBJS = $(MODULES:%=%.o)
LIB = libcomponent.a

DEPENDS = $(MODULES:%=%.d)

RESIDUE = \
	$(LIB) \
	$(OBJS) \
	$(DEPENDS)


.PHONY : all
all : $(LIB)

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
