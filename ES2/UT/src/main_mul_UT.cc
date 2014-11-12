#include "mul_UT.hh"
#include "mul_UT_testbench.hh"

class mul_top : public sc_module
{

private:

  mul_UT  m_target;

  mul_UT_testbench m_initiator;


public:

  mul_top(sc_module_name name)
    : sc_module(name)
    , m_target("target")
    , m_initiator("initiator")
  {
    m_initiator.initiator_socket(m_target.target_socket);
  }

};

int main(int argc, char* argv[])
{

  mul_top top("top");

  sc_start();

  return 0;

}
