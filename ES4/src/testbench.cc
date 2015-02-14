#include "testbench.h"

#include <iostream>
#include <fstream>
using namespace std;

testbench::testbench(sc_module_name name_)
: sc_module(name_)
{
	SC_THREAD(run);
	sensitive << clk.pos();
	SC_THREAD(clk_gen);
}

void testbench::clk_gen()
{
	while( true )
	{
		clk.write( sc_dt::SC_LOGIC_1 );
		wait(PERIOD / 2, sc_core::SC_MS);
		clk.write( sc_dt::SC_LOGIC_0 );
		wait(PERIOD / 2, sc_core::SC_MS);
	}
}

void testbench::run()
{

    std::fstream myfile("inputs.txt", std::ios_base::in);

    double a;

    cout << "reading file" << endl;
    while (myfile >> a)
    {
        cout << "send " << a << "to Controller" << endl;
        number.write(a);
        wait();
    }

    sc_stop();

}


