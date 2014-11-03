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

	//flag di input per sapere se sono pronti i numeri da moltiplicare
	sc_out<sc_uint<1> >    isready;

	//numeri di input da moltiplicare
	sc_out<sc_lv<64> >   	number_a;
	sc_out<sc_lv<64> >  	number_b;

	//flag di output per sapere quando e' pronto l'output
	sc_in<sc_uint<1> >   result_isready;

	sc_in<sc_lv<64> >  		result;
	sc_out<bool>           	reset;
	sc_out< sc_logic > 	clk;


	SC_HAS_PROCESS(mul_RTL_testbench);
	mul_RTL_testbench(sc_module_name name);

};

#endif
