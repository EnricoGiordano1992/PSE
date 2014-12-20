/*
 * p_plant.h
 *
 *  Created on: Dec 20, 2014
 *      Author: enrico
 */

#ifndef P_PLANT_H_
#define P_PLANT_H_

#include <systemc-ams.h>

#include <systemc.h>



SCA_TDF_MODULE (p_plant )
{

	sca_tdf::sca_in<double> k_input;

	sca_tdf::sca_out<double> y_out;

	//funzione di trasferimento
	sca_tdf::sca_ltf_nd ltf_nd;

	sca_util::sca_vector<double> num, den; // numerator and denominator coefficients
	double h0; // DC gain

	void set_attributes ();
	// [] rate,tstep,delay
	void initialize();
	// [] state
	void processing ();
	// ! behavior


	SCA_CTOR(p_plant)
	{
	}

};

#endif /* P_PLANT_H_ */
