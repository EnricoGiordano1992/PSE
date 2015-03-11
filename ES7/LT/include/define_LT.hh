#ifndef __define_LT_h__
#define __define_LT_h__

#include <systemc.h>

struct iostruct {
  double numero_a;
  double numero_b;
  double risultato;
};

#define ADDRESS_TYPE int
#define DATA_TYPE iostruct

#endif
