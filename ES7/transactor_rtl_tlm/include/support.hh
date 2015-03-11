#ifndef SUPPORT_H
#define SUPPORT_H

#include "systemc.h"
#include <stdint.h>

sc_dt::sc_lv<64> doubleToLogicVector(double d);

double logicVectorToDouble(sc_dt::sc_lv<64> v);

#endif
