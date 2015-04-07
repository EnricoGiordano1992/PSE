#include <systemc.h>
#include <systemc-ams.h>
#include <tlm.h>
#include "../include/transactor_RTL_TLM.hh"
#include "../include/fpm_TLM_LT_testbench.hh"
#include "../include/fpm_RTL.hh"
#include "controller.h"
#include "controller_err.h"
#include "p_plant.h"

#define PERIOD 10

SC_MODULE(main_RTL)
{
  
  // CLOCK SIGNAL
  sc_signal< sc_dt::sc_logic > clock;

  void clk_gen()
  {
    while( true )
    {
      clock.write( sc_dt::SC_LOGIC_1 );
      wait( PERIOD / 2, sc_core::SC_MS );
      clock.write( sc_dt::SC_LOGIC_0 );
      wait( PERIOD / 2, sc_core::SC_MS );
    }
  }

  // RESET SIGNAL
  sc_signal < sc_logic >        reset_signal;
  
  // OTHER SIGNALS
  sc_signal<sc_logic >  	number_is_rdy;
  sc_signal<sc_lv<64> > 	data1;
  sc_signal<sc_lv<64> > 	data2;
  sc_signal<sc_logic >  	result_is_rdy;
  sc_signal<sc_lv<64> > 	ris;
  sc_signal<double> temp;
  sc_signal<double> temp2;


      //sc_signal<double> r_in;
    sca_tdf::sca_signal<double> k_t;
    sca_tdf::sca_signal<double> y_out;
    sca_tdf::sca_signal<double> errore;

    //---------------------Modules Here-----------------------

  //Definition   
  mul_LT_testbench		i_src_LT;	       // source module
  root_RTL_transactor		i_root_RTL_transactor; // root transactor  
  divModule			i_root_RTL;    // root RTL module
  controller controller_istance;
  controller_err controller_err_istance;
  p_plant physical_plant_istance;

    
  //Construct
  SC_CTOR(main_RTL)
    : i_src_LT("i_src_LT")
    , i_root_RTL_transactor("i_root_RTL_transactor")
    , i_root_RTL("i_root_RTL")
    , controller_istance("controller")
    , controller_err_istance("controller_err")
    , physical_plant_istance("physical_plant")
  {
      
    SC_THREAD( clk_gen );

    // TLM testbench <-> Transactor binding (TLM interface)
    i_src_LT.initiator_socket(i_root_RTL_transactor.target_socket);
		
    // RTL design <-> Transactor binding (RTL interfaces)
    i_root_RTL.clk(clock);
    i_root_RTL.rst(reset_signal);
    i_root_RTL.start(number_is_rdy);
    i_root_RTL.op1in(data1);
    i_root_RTL.op2in(data2);
    i_root_RTL.result(ris);
    i_root_RTL.done(result_is_rdy);
 

    i_root_RTL_transactor.clk(clock);
    i_root_RTL_transactor.reset_to_RTL(reset_signal);
    i_root_RTL_transactor.p_In_enable(result_is_rdy);
    i_root_RTL_transactor.p_In_data(ris);
    i_root_RTL_transactor.p_Out_enable(number_is_rdy);
    i_root_RTL_transactor.p_Out_data1(data1);
    i_root_RTL_transactor.p_Out_data2(data2);




    // colleghiamo RTL a AMS
    i_root_RTL.result_to_ams(temp);
    controller_err_istance.r_input(temp);
    



    //-----TDF Modules-----

    //controller_err_istance.r_input(r_in);
    controller_err_istance.y_input(y_out);
    controller_istance.k_out(k_t);

    controller_err_istance.err_out(errore);
    controller_istance.err_in(errore);   

    physical_plant_istance.k_input(k_t);
    physical_plant_istance.y_out(y_out);

    // colleghiamo AMS al transattore per il risultato di ritorno
    physical_plant_istance.y_to_transactor(temp2);
    i_root_RTL_transactor.y_from_ams(temp2);
	
  }



};

int sc_main(int argc, char **argv)
{
  sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
  main_RTL* topl;

  topl = new main_RTL("topl");

  sc_start();

  return(0);

};

