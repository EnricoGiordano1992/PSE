
#include "mul_RTL_testbench.hh"
#include "mul_RTL.hh"

int sc_main(int argc, char **argv)
{
  sc_signal< sc_logic >         clock;
  
  sc_signal < bool >        reset_signal;
  sc_signal<sc_lv<64> >   p_In_data1;
  sc_signal<sc_lv<64> >   p_In_data2;
  sc_signal<sc_uint<1> >    p_In_enable;
  sc_signal<sc_uint<1> >    p_Out_enable;
  sc_signal<sc_lv<64> >   p_Out_data1;
  sc_signal<sc_lv<64> >   p_Out_data2;
  sc_signal<sc_lv<64> >   p_result;
  
  mul_RTL_testbench        i_src_RTL("src_RTL");             // source module
  mul_RTL                  i_mul_RTL("mul_RTL");    		// root RTL module

  i_mul_RTL.clk(clock);
  i_mul_RTL.reset(reset_signal);
  i_mul_RTL.isready(p_Out_enable);
  i_mul_RTL.number_a(p_In_data1);
  i_mul_RTL.number_b(p_In_data2);
  i_mul_RTL.out_result(p_result);
  i_mul_RTL.result_isready(p_In_enable);
  
  i_src_RTL.clk(clock);
  i_src_RTL.isready(p_Out_enable);
  i_src_RTL.number_a(p_In_data1);
  i_src_RTL.number_b(p_In_data2);
  i_src_RTL.result_isready(p_In_enable);
  i_src_RTL.result(p_result);
  i_src_RTL.reset( reset_signal );

  sc_trace_file * fp = sc_create_vcd_trace_file("wave");
  sc_trace(fp, clock, "clock");
  sc_trace(fp, reset_signal, "reset");
  sc_trace(fp, p_In_enable, "number_ready");
  sc_trace(fp, p_Out_enable, "result_ready");
  sc_trace(fp, p_In_data1, "number1");
  sc_trace(fp, p_In_data2, "number2");
  sc_trace(fp, p_result, "result");

  
  sc_start();

  return 0;
};

