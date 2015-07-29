/*
 * p_plant.cc
 *
 *
 */

#include "p_plant.h"

// costruttore
// [] rate,tstep,delay
void p_plant::set_attributes (){

	/* ??? perchè qui è commentato è negli altri file no ???*/

	//set_timestep( 20.0, sc_core::SC_MS); 	// module time step assignment of a of 10 ms
	//y_out.set_delay(1.0); 			// set delay of port out to 2 samples

}

// inizializzazioni
// [] state
void p_plant::initialize(){

	// costruiamo la funzione di trasferimento
	num(0) = 1.0;
	den(0) = 0;
	den(1) = 13;
	den(2) = 1;

}

// operazioni eseguite
// ! behavior
void p_plant::processing (){

	//cout << "AAA - p_plant ha generato il numero: " << ltf_nd( num, den, k_input.read(), 1 ) << endl;
	y_out.write( ltf_nd( num, den, k_input.read(), 1 ) );
	y_to_transactor.write( static_cast<double>(ltf_nd( num, den, k_input.read(), 1 )) );

	// ho messo h0 = 1, ma non so bene perché funzioni

}
