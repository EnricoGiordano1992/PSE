#ifndef __define_LT_h__
#define __define_LT_h__

#include <systemc.h>

struct iostruct {
	double op1;
	double op2;
	double result;
	double result2;
};

#define ADDRESS_TYPE int
#define DATA_TYPE iostruct

#endif
