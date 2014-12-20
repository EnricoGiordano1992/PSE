/*
 * controller.h
 *
 *  Created on: Dec 2, 2014
 *      Author: enrico
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <systemc-ams.h>

#include <systemc.h>



SCA_TDF_MODULE (controller )
{

	sca_tdf::sca_in<double> r_input;
	sca_tdf::sca_in<double> y_input;

	sca_tdf::sca_out<double> k_out;

	double r_calc;
	double y_calc;

	double err_calc;
	double err_calc_pre;

	double k_calc;
	double k_calc_pre;

	void set_attributes ();
	// [] rate,tstep,delay
	void initialize();
	// [] state
	void processing ();
	// ! behavior


	SCA_CTOR(controller)
	{
	}

};

#endif /* CONTROLLER_H_ */
