SYSTEMC=$$SYSTEMC_HOME
SYSTEMC_AMS=$$SCAMS_HOME


#SYSC_LIB=$(SYSTEMC)/lib-linux64
#SYSC_AMS_LIB=$(SYSTEMC_AMS)/lib-linux64
SYSTEMC=/usr/local/systemc-2.3.1
SYSTEMC_AMS=/home/enrico/systemc-ams-2.0_beta1

SYSC_LIB=$(SYSTEMC)/lib-linux64
SYSC_AMS_LIB=$(SYSTEMC_AMS)/lib-linux

INCDIR= -I$(SYSTEMC)/include -I$(SYSTEMC_AMS)/include -I./inc/
LIBDIR= -L$(SYSC_LIB) -L$(SYSC_AMS_LIB)
LIBS= -lsystemc-ams -lsystemc


EXECUTABLE=./bin/es4.x

CXX=g++ -O3
CFLAGS= -c
LD=g++

all: $(info $(INCDIR)) $(info $(LIBDIR)) $(EXECUTABLE)

clean:
	@echo Cleaning
	@rm -f obj/*.o *~
	@rm -f $(EXECUTABLE)

$(EXECUTABLE): obj/main.o obj/controller.o obj/p_plant.o obj/global.o
	@ $(LD) -o $@ $(INCDIR) $(LIBDIR) obj/*.o $(LIBS)

obj/controller.o: src/controller.cc obj/global.o
	@ $(CXX) $(CFLAGS) -o $@ $(INCDIR) $(LIBDIR) $(LIBS) $<

obj/main.o: src/main.cc obj/global.o 
	@ $(CXX) $(CFLAGS) -o $@ $(INCDIR) $(LIBDIR) $(LIBS) $<

obj/p_plant.o: src/p_plant.cc obj/global.o
	@ $(CXX) $(CFLAGS) -o $@ $(INCDIR) $(LIBDIR) $(LIBS) $<

obj/global.o: src/global.cc
	@ $(CXX) $(CFLAGS) -o $@ $(INCDIR) $(LIBDIR) $(LIBS) $<



.PHONY: all clean	

