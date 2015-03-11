#include "../inc/rtl_2_tdf.hh"

rtl_2_tdf::rtl_2_tdf(sc_core::sc_module_name nm) :
        r_rtl("r_rtl"), y_tdf("y_tdf"), r_tdf("r_tdf"), y_rtl("y_rtl") {

}

void rtl_2_tdf::set_attributes() {
 
    /*y_tdf.set_delay(1.0);
    r_rtl.set_delay(1.0);*/
    set_timestep( 20.0, sc_core::SC_MS); 
}

void rtl_2_tdf::initialize(){
	
	/*r_tdf.initialize(0.0);
	y_rtl.initialize(0.0);*/
	
}
void rtl_2_tdf::processing() {
    std::cout<<"[ RTL_2_TDF ] READ :"<<r_rtl.read()<< std::endl;
    // passing r from tb--->target---->rtl2_tdf---->error--->controller---->plant
    r_tdf.write( r_rtl.read() );
    std::cout<<"[ RTL_2_TDF ] WRITE :"<<y_tdf.read()<< std::endl;
    //passing y from plant---->rtl_2_tdf---->target---->tb
    y_rtl.write( y_tdf.read() );
}
