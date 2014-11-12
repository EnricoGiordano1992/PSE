#include "mul_UT.hh"


mul_UT::mul_UT(sc_module_name name_)
  : sc_module(name_)
  , target_socket("target_socket")
  , pending_transaction(NULL)
{

  target_socket(*this);

}

void mul_UT::b_transport(tlm::tlm_generic_payload& trans, sc_time& t)
{

  ioDataStruct = *((iostruct*) trans.get_data_ptr());

  if (trans.is_write()) {
    mul_function();
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
    ioDataStruct.risultato = tmp_result;
    *((iostruct*) trans.get_data_ptr()) = ioDataStruct;
  } 
  else if (trans.is_read()){
    ioDataStruct.risultato = tmp_result;
    *((iostruct*) trans.get_data_ptr()) = ioDataStruct;
  }

}

bool mul_UT::get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data)
{
  return false;
}

tlm::tlm_sync_enum mul_UT::nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_time& t)
{
  return tlm::TLM_COMPLETED;
}

unsigned int mul_UT::transport_dbg(tlm::tlm_generic_payload& trans)
{
  return 0;
}



void mul_UT:: mul_function()
{
  tmp_result = ioDataStruct.numero_a * ioDataStruct.numero_b;
}

// Initialization:
void mul_UT:: end_of_elaboration()
{

}

void mul_UT:: reset()
{

}
