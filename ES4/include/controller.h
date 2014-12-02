/*
 * controller.h
 *
 *  Created on: Dec 2, 2014
 *      Author: enrico
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <systemc-ams.h>

SCA_TDF_MODULE (my_tdf_module )
{

	sca_tdf::sca_in<double> in;
	sca_tdf::sca_out<double> out;
	SCA_CTOR(my_tdf_module) {}
	// ! Constructor
	void set_attributes ();
	// [] rate,tstep,delay
	void initialize();
	// [] state
	void processing ();
	// ! behavior
	void ac_processing ();
	// [] ac-behavior
};

#endif /* CONTROLLER_H_ */
