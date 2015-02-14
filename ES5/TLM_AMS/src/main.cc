#include "../inc/system.hh"

#define TRACE_PLANT
#define TABULAR_FILE "plant.dat"
int sc_main( int ac, char *av[] )
{
	sc_core::sc_set_time_resolution(1.0, sc_core::SC_FS);
	
	System system("system");
	
	#ifdef TRACE_PLANT
		//AMS PLANT TRACE
		
		//sca_util::sca_trace_file * atf = sca_util::sca_create_vcd_trace_file("ams.vcd");
		sca_util::sca_trace_file* atf = sca_util::sca_create_tabular_trace_file(TABULAR_FILE);
		sca_util::sca_trace(atf, system.r   , "r");
		sca_util::sca_trace(atf, system.y   , "y");
		
	#endif
	
	
	sc_start( );
	return 0;
}
