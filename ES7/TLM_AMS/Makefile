.SUFFIXES:
.SUFFIXES: .cc .cpp .o .x
TARGET_ARCH = linux

CC     = g++
OPT    =
DEBUG  = 
OTHER  = -Wall -O3

# Use this CFLAGS definition if you need to see debug output
# CFLAGS = -c -g -Wno-deprecated -Wall -D_DEBUG
CFLAGS = -c -g -Wno-deprecated -Wall -std=c++11

MODULE = run
SRCS = \
../src/testbench_TLM.cc \
../src/tlm_2_rtl.cc \
../src/rtl_2_tdf.cc \
../src/error.cc \
../src/controller.cc \
../src/plant.cc \
../src/system.cc \
../src/main.cc 

OBJS = $(SRCS:../src/%.cc=%.o)

include Makefile.defs
