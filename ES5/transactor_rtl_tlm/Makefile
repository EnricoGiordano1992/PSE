# Some notes:
#	In order to run correctly this Makefile, you have to set this variables:
SYSTEMC_DIR=$$SYSTEMC_HOME
SYSTEMC_INC = $(SYSTEMC_DIR)/include
SYSTEMC_LIB = $(SYSTEMC_DIR)/lib-linux64
#
#	So, you need to install both SystemC and SystemC-AMS.
#	Good Luck!

CXX        = g++
LD         = g++
RM         = rm -rf
CXXFLAGS   = -c -ansi

EXECUTABLE=./bin/es7.x

INCS =	\
		-I $(SYSTEMC_INC)\
	-I./include/\
	-I.

LIBS =	-L $(SYSTEMC_LIB) \
	 	-lsystemc \
	 	-lm \

OBJS =     ./obj/mul_LT_testbench.o \
	   ./obj/div_RTL.o \
	   ./obj/root_RTL_transactor.o \
	   ./obj/support.o \
	   ./obj/main_root_RTL.o

all: $(EXECUTABLE)

install: all

$(EXECUTABLE): $(OBJS)
	@echo Linking $@
	@$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)
	
#------------------------------------------------------------
#---------------------------Main-----------------------------
obj/main_root_RTL.o: src/main_root_RTL.cc
	@echo Compiling $@
	@$(CXX) $(DEBUG) $(CXXFLAGS) $(INCS) -o $@ $<

#------------------------------------------------------------
#--------------------TDF Modules-----------------------------


obj/mul_LT_testbench.o: src/mul_LT_testbench.cc
	@echo Compiling $@
	@$(CXX) $(DEBUG) $(CXXFLAGS) $(INCS) -o $@ $<

obj/div_RTL.o: src/div_RTL.cc
	@echo Compiling $@
	@$(CXX) $(DEBUG) $(CXXFLAGS) $(INCS) -o $@ $<

obj/root_RTL_transactor.o: src/root_RTL_transactor.cc
	@echo Compiling $@
	@$(CXX) $(DEBUG) $(CXXFLAGS) $(INCS) -o $@ $<

obj/support.o: src/support.cc
	@echo Compiling $@
	@$(CXX) $(DEBUG) $(CXXFLAGS) $(INCS) -o $@ $<



clean:
	@echo "Cleaning..."
	@rm -rf obj/*.o
	@rm -rf *.x
	@rm -rf *.vcd
	@rm -rf $(EXECUTABLE)
	@echo "Done!"

.PHONY: all clean install
