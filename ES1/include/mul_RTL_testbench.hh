#ifndef mul_RTL_TESTBENCH_H
#define mul_RTL_TESTBENCH_H

#define PERIOD 10

#include <systemc.h>

class mul_RTL_testbench : public sc_core::sc_module
{

private:

  void run();
  void clk_gen();

public:
  
//numeri di input da moltiplicare
  sc_in<sc_lv<64> >   	number_a;
  sc_in<sc_lv<64> >  	number_b;

//flag di output per sapere quando e' pronto l'output
  sc_out<sc_uint<1> >   result_isready;

  sc_out<sc_lv<64> >  		result;
  sc_in<bool>           	reset;
  sc_in< sc_dt::sc_logic > 	clk;

  typedef enum {SR, S0, S1, S2, S3, S4, S5, S6, S7, S8, S9} STATES;

  sc_signal<STATES> STATUS, NEXT_STATUS;

  sc_signal<sc_lv<64> >   	vcl_number_a;
  sc_signal<sc_lv<64> >  	vcl_number_b;
  sc_signal<sc_lv<64> >  	vcl_result;


  SC_HAS_PROCESS(mul_RTL_testbench);
  mul_RTL_testbench(sc_module_name name);

};

#endif
