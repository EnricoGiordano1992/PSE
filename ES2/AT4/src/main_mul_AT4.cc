#include "mul_AT4.hh"
#include "mul_AT4_testbench.hh"

class mul_top : public sc_module
{

private:

  mul_AT4  m_target;

  mul_AT4_testbench m_initiator;


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
