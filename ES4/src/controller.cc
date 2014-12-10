/*
 * controller.cc
 *
 *  Created on: Dec 2, 2014
 *      Author: enrico
 */

#include "controller.h"

// ! Constructor
void controller::set_attributes (){

	set_timestep( 20.0, sc_core::SC_MS); 	// module time step assignment of a of 10 ms
	out.set_delay(1); 						// set delay of port out to 2 samples

}
// [] rate,tstep,delay
void controller::initialize(){

	r_calc = r_input.read();
	y_calc = y_input.read();
	k_calc_pre = 0;
	err_calc_pre = 0;

}
// [] state
void controller::processing (){

	err_calc = r_calc - y_calc;
	k_calc = k_calc_pre + 100 + (err_calc - err_calc_pre) + get_timestep() * err_calc;

	k_calc_pre = k_calc;
	err_calc_pre = err_calc;

	k_out.write(k_calc);

}
// ! behavior
void controller::ac_processing (){

}


// [] ac-behavior

