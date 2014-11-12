#include "mul_UT_testbench.hh"

void mul_UT_testbench::invalidate_direct_mem_ptr(uint64 start_range, uint64 end_range)
{
  
}

tlm::tlm_sync_enum mul_UT_testbench::nb_transport_bw(tlm::tlm_generic_payload &  trans, tlm::tlm_phase &  phase, sc_time &  t)
{
  return tlm::TLM_COMPLETED;
}


void mul_UT_testbench::run()
{

  sc_time local_time;
  // First transaction (initialization)
  iostruct mul_packet;
  tlm::tlm_generic_payload payload;	

  cout<<"calcolo la moltiplicazione tra double!"<<endl;

    mul_packet.numero_a = 2.5;
    mul_packet.numero_b = 1.3;
    cout<<"\t "<<mul_packet.numero_a << " * " << mul_packet.numero_b << " = ";
    payload.set_data_ptr((unsigned char*) &mul_packet);
    payload.set_address(0);
    payload.set_write();
    
    // start write transaction
    initiator_socket->b_transport(payload, local_time);

    // start read transaction
    if(payload.get_response_status() == tlm::TLM_OK_RESPONSE){
      cout << mul_packet.risultato << endl;

  }
  sc_stop();
  
}


mul_UT_testbench::mul_UT_testbench(sc_module_name name)
  : sc_module(name)
{

  initiator_socket(*this);

  SC_THREAD(run);

}
