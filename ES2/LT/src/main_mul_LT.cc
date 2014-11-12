#include "mul_LT.hh"
#include "mul_LT_testbench.hh"

class mul_top : public sc_module
{

private:

  mul_LT  m_target;

  mul_LT_testbench m_initiator;


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
