
#include "root_RTL_testbench.hh"
#include "root_RTL.hh"

int sc_main(int argc, char **argv)
{
  sc_signal< sc_logic >         clock;
  
  sc_signal < bool >        reset_signal;
  sc_signal<sc_uint<32> >   p_In_data;
  sc_signal<sc_uint<1> >    p_In_enable;
  sc_signal<sc_uint<1> >    p_Out_enable;
  sc_signal<sc_uint<32> >   p_Out_data;
  
  root_RTL_testbench        i_src_RTL("src_RTL");             // source module
  root_RTL                  i_root_RTL("root_RTL");    // root RTL module

  i_root_RTL.clk(clock);
  i_root_RTL.reset(reset_signal);
  i_root_RTL.number_isready(p_In_enable);
  i_root_RTL.number_port(p_In_data);
  i_root_RTL.result_port(p_Out_data);
  i_root_RTL.result_isready(p_Out_enable);
  
  i_src_RTL.clk(clock);
  i_src_RTL.p_In_enable(p_Out_enable);
  i_src_RTL.p_In_data(p_Out_data);
  i_src_RTL.p_Out_enable(p_In_enable);
  i_src_RTL.p_Out_data(p_In_data);
  i_src_RTL.reset_to_RTL( reset_signal );
  
  sc_start();
  
  return 0;
};

