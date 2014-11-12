#include "mul_AT4.hh"

mul_AT4::mul_AT4(sc_module_name name_)
  : sc_module(name_)
  , target_socket("target_socket")
  , pending_transaction(NULL)
{

  target_socket(*this);

  SC_THREAD(IOPROCESS);

}

void mul_AT4::b_transport(tlm::tlm_generic_payload& trans, sc_time& t)
{

}

bool mul_AT4::get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data)
{
  return false;
}


tlm::tlm_sync_enum mul_AT4::nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_time& t)
{
  // If there is already a pending transaction, we refuse to
  // serve the request (note that when we return TLM_COMPLETED
  // we do not need to update the phase).
  if (pending_transaction != NULL) {
    trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return tlm::TLM_COMPLETED;
  }

  // Phase must be BEGIN_REQ, otherwise we set an
  // error response and complete the transaction.
  if (phase != tlm::BEGIN_REQ) {
    trans.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return tlm::TLM_COMPLETED;
  }

  pending_transaction = &trans;
  ioDataStruct = *((iostruct*) trans.get_data_ptr());

  phase = tlm::END_REQ;
  //cout << name()<<" sending notify" << endl;
  io_event.notify();
  
  return tlm::TLM_UPDATED;
  
}

unsigned int mul_AT4::transport_dbg(tlm::tlm_generic_payload& trans)
{
  return 0;
}

void mul_AT4::IOPROCESS()
{

  sc_time timing_annotation;

  while (true) {
    wait(io_event);

    // When an io_event is notified, it means we must send the response back
    // to the initiator. We wait 100ns to model the delay required to
    // process the request.

    wait(100, SC_NS);    

    if (pending_transaction->is_write()) {
      mul_function();
    }
    pending_transaction->set_response_status(tlm::TLM_OK_RESPONSE);

    *((iostruct*) pending_transaction->get_data_ptr()) = ioDataStruct;

    tlm::tlm_phase phase = tlm::BEGIN_RESP;

    target_socket->nb_transport_bw(*pending_transaction, phase, timing_annotation);

    pending_transaction = NULL;

  }

}

// FUNCTIONALITY SIDE:
//****************
void mul_AT4:: mul_function()
{
  ioDataStruct.risultato = ioDataStruct.numero_a * ioDataStruct.numero_b;

}

void mul_AT4:: end_of_elaboration()
{

}

void mul_AT4:: reset()
{

}
