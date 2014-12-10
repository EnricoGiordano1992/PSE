/*
 * main.cpp
 *
 *  Created on: Dec 2, 2014
 *      Author: enrico
 */

#include <systemc-ams>
#include <systemc>

//----------LSF Modules----------

//----------ELN Modules----------

//----------TDF Modules----------

int sc_main(int argc, char* argv[]) {
    sc_core::sc_set_time_resolution(1.0, sc_core::SC_FS);

    //--------------------Local Ports Here--------------------

    //---------------------Modules Here-----------------------

    //-----------------Interconnections Here------------------
    //-----ELN Modules-----

    //-----TDF Modules-----


    //-----LSF Modules-----

    //-----------------------Traces Here----------------------

    //-----------------------Start Here-----------------------

    //--------------------Close Traces Here-------------------

    return 0;
}
