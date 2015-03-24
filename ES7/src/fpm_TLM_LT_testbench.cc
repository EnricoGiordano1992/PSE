#include "../include/fpm_TLM_LT_testbench.hh"
#include <iostream>
#include <iomanip>

void mul_LT_testbench::invalidate_direct_mem_ptr(uint64 start_range, uint64 end_range)
{
  
}

tlm::tlm_sync_enum mul_LT_testbench::nb_transport_bw(tlm::tlm_generic_payload &  trans, tlm::tlm_phase &  phase, sc_time &  t)
{
  return tlm::TLM_COMPLETED;
}

void mul_LT_testbench::run()
{

  sc_time local_time = m_qk.get_local_time();

  // First transaction (initialization)
  iostruct div_packet;
  tlm::tlm_generic_payload payload;	
  
  for (int i = 1; i <= 10; i++){

     
    div_packet.op1 = ((rand() % 201) - 100) * 0.1;
    div_packet.op2 = ((rand() % 201) - 100) * 0.1;
    
    payload.set_data_ptr((unsigned char*) &div_packet);
    payload.set_address(0);
    payload.set_write();
    
    local_time = m_qk.get_local_time();
    
    // start write transaction
    initiator_socket->b_transport(payload, local_time);

    // start read transaction
    payload.set_read();
    initiator_socket->b_transport(payload, local_time);
    if(payload.get_response_status() == tlm::TLM_OK_RESPONSE){

      risultati_fpm.push_back(div_packet.result);
      risultati_ams.push_back(div_packet.result2);
      cout<<endl;
    }

    m_qk.set(local_time);
    if (m_qk.need_sync()) {
      m_qk.sync();
    }
    

  }

  cout << "stampo il vettore di risultati:" << endl;
  cout << "(formato: riferimenti \t valori calcolati \t variazione %)" << endl;
  for(int i=0; i<risultati_fpm.size()-1; i++){

    double percentuale = (risultati_fpm.at(i) - risultati_ams.at(i+1)) / risultati_fpm.at(i) * 100;
    cout << risultati_fpm.at(i) << "\t" << risultati_ams.at(i+1) << "\t" << percentuale << endl;
  }


  sc_stop();
  
}





mul_LT_testbench::mul_LT_testbench(sc_module_name name)
  : sc_module(name)
{

  initiator_socket(*this);

  SC_THREAD(run);

  m_qk.set_global_quantum(sc_time(500, SC_NS));
  m_qk.reset();

}
