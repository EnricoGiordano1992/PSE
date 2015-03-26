#ifndef mul_RTL_H
#define mul_RTL_H

#include "systemc.h"

SC_MODULE(mul_RTL){

	//flag di input per sapere se sono pronti i numeri da moltiplicare
	sc_in<sc_uint<1> >    isready;

	//numeri di input da moltiplicare
	sc_in<sc_lv<64> >   	number_a;
	sc_in<sc_lv<64> >  	number_b;

	//flag di output per sapere quando e' pronto l'output
	sc_out<sc_uint<1> >   result_isready;

	sc_out<sc_lv<64> >  		out_result;


	sc_in<bool>           		reset;
	sc_in< sc_dt::sc_logic > 	clk;

	typedef enum {SR, S0, S0I, S1, S2, S3, S31, S4, S5, S6, S7, S8, S9} STATES;

	sc_signal<STATES> STATUS, NEXT_STATUS;

	sc_signal<sc_lv<64> >   	vcl_number_a;
	sc_signal<sc_lv<64> >  		vcl_number_b;
	sc_signal<sc_lv<64> >  		vcl_result;
	sc_signal<bool>				end_mantissa;

	//overflow?
	sc_signal<bool>				error;
	//normalizzato?
	sc_signal<bool >		 normalizzato;


	/////////////////////

	void elaborate_mul(void);
	void elaborate_mul_FSM(void);

	SC_CTOR(mul_RTL){
		SC_METHOD(elaborate_mul_FSM);
		sensitive << reset.neg();
		sensitive << clk.pos();



		SC_METHOD(elaborate_mul);
		sensitive << STATUS << isready << number_a << number_b;
	};
};

#endif
