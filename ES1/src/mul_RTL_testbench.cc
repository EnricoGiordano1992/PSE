#include "mul_RTL_testbench.hh"
#include "support.hh"

mul_RTL_testbench::mul_RTL_testbench(sc_module_name name_)
: sc_module(name_)
{

	SC_THREAD(run);
	sensitive << clk.pos();

	SC_THREAD(clk_gen);
}

void mul_RTL_testbench::clk_gen()
{
	while( true )
	{
		clk.write( sc_dt::SC_LOGIC_1 );
		wait(PERIOD / 2, sc_core::SC_NS);
		clk.write( sc_dt::SC_LOGIC_0 );
		wait(PERIOD / 2, sc_core::SC_NS);
	}
}

void mul_RTL_testbench::run()
{

	double numero1, numero2, new_result = 0;

	cout<<"Calcolo la moltiplicazione tra due double!"<<endl;

	numero1 = 1.23;
	numero2 = 1.3;

	cout << "\t " << numero1 << " * " << numero2 << endl;

	reset.write(true);
	number_a.write(doubleToLogicVector(numero1));
	number_b.write(doubleToLogicVector(numero2));

	isready.write(1);
	wait();

	while(result_isready.read() != 1) wait();
	new_result=logicVectorToDouble(result.read());
	cout << "\t = " << result << endl;


	reset.write(0);
	isready.write(0);
	number_a.write(0);
	number_b.write(0);

	sc_stop();

}


