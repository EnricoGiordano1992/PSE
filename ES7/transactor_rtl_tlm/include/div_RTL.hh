#ifndef MUL_H
#define MUL_H

#include "systemc.h"
#include "support.hh"

SC_MODULE(divModule) {
public:
	sc_in<sc_logic> start;
	sc_out<sc_logic> done;
	//sc_in < sc_logic > input_rdy;
	sc_in<sc_lv<64> > op1in;
	sc_in<sc_lv<64> > op2in;
	sc_out<sc_lv<64> > result;
	sc_in<sc_logic> clk;
	sc_in<sc_logic> rst;

	 int flagInt;
	 int flagExt;
	 int flagTmp;

	typedef enum {
		Reset_ST,
		ST_0,
		ST_1,
		ST_2,
		ST_3,
		ST_4,
		ST_5,
		ST_6,
		ST_7,
		ST_8,
	} STATES;

	sc_signal<STATES> STATE, NEXT_STATE;

	SC_HAS_PROCESS(divModule);
public:
	divModule(const sc_module_name& module_name) :
			sc_module(module_name), start("start"), done("done"), op1in(
					"op1in"), op2in("op2in"), result("result"), clk("clk"), rst(
					"rst") {
		SC_METHOD(div_proc_fsm);
		sensitive << clk << rst;
		SC_METHOD(div_proc_dp);
		sensitive << start << op1in << op2in << STATE;

	}

private:
	void div_proc_dp();
	void div_proc_fsm();
};

#endif
