/*
 * main.cpp
 *
 *  Created on: Dec 2, 2014
 *      Author: enrico
 */

#include <systemc-ams.h>
#include <systemc.h>
#include "controller.h"
#include "p_plant.h"

//----------LSF Modules----------

//----------ELN Modules----------

//----------TDF Modules----------

int sc_main(int argc, char* argv[]) {
    sc_core::sc_set_time_resolution(1.0, sc_core::SC_FS);

    //--------------------Local Ports Here--------------------
    sca_tdf::sca_signal<double> r_in;
    sca_tdf::sca_signal<double> e_t;
    sca_tdf::sca_signal<double> k_t;
    sca_tdf::sca_signal<double> y_out;

    //---------------------Modules Here-----------------------

    //-----------------Interconnections Here------------------
    //-----ELN Modules-----

    //-----TDF Modules-----
    controller controller_istance("controller");
    controller_istance.r_input(r_in);
    controller_istance.y_input(y_out);
    controller_istance.k_out(k_t);


    p_plant	physical_plant_istance("physical_plant");
    physical_plant_istance.k_input(k_t);
    physical_plant_istance.y_out(y_out);


    //-----LSF Modules-----

    //-----------------------Traces Here----------------------

    //-----------------------Start Here-----------------------
    sc_start( 5000, sc_core::SC_SEC );
    //--------------------Close Traces Here-------------------

    return 0;
}
