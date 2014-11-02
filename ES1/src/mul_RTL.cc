#include "mul_RTL.hh"

void mul_RTL :: elaborate_mul_FSM(void){

	sc_lv<11> exp1;
	sc_lv<53> mantissa1;
	sc_lv<1>  sign1;

	sc_lv<11> exp2;
	sc_lv<53> mantissa2;
	sc_lv<1>  sign2;

	sc_lv<11> exp_tot;
	sc_lv<11> mantissa_tot;
	sc_lv<1>  sign_tot;

	sc_lv<64> result_tot;

	if (reset.read() == 0){
		STATUS = SR;
	}
	else if (clk.read() == 1) {


		STATUS = NEXT_STATUS;

		switch(STATUS){
		//allo stato di reset non fa niente
		case SR:
			result.write(0);
			result_isready.write(0);

			break;

			//preparazione del sistema
		case S0:
			result.write(0);
			result_isready.write(0);
			vcl_number_a.write(0);
			vcl_number_b.write(0);
			vcl_result.write(0);

			break;

			//leggo i numeri
		case S0I:
			vcl_number_a.write(number_a.read());
			vcl_number_b.write(number_b.read());

			exp1 = (number_a.read().range(1,12));
			exp2 = (number_b.read().range(1,12));

			mantissa1 = (number_a.read().range(13,63));
			mantissa2 = (number_b.read().range(13,63));

			sign1 = number_a.read().range(0,1);
			sign2 = number_b.read().range(0,1);

			break;

			//sommo gli esponenti
		case S1:
			exp_tot = static_cast< sc_uint<11> >( exp1 ) + static_cast< sc_uint<11> >( exp2 );

			break;

			//polarizzo il risultato
		case S2:
			exp_tot = static_cast< sc_uint<11> >( exp_tot ) - 1023;

			break;

			//prodotto tra mantisse
		case S3:
			mantissa_tot = static_cast< sc_uint<53> >( mantissa1 ) * static_cast< sc_uint<53> >( mantissa2 );

			break;

			//normalizzo il risultato
		case S4:
			if(mantissa_tot[53] != '1'){
				mantissa_tot << 1;
				exp_tot = static_cast< sc_uint<11> >( exp_tot )++;
			}

			break;

			//overflow?
		case S5:
			if(static_cast< sc_uint<11> >( exp_tot ) == 0 || static_cast< sc_uint<53> >( mantissa_tot ) == 0)
				error.write(true);

			break;

			//?
		case S6:

			break;

			//devo normalizzare ancora?
		case S7:
			if(mantissa_tot[0] == '1')
				normalizzato.write( true );
			else
				normalizzato.write( false );

			break;

			//calcolo del segno
		case S8:
			sign_tot = sign1 ^ sign2;

			break;

		case S9:

			result_tot.range(0,1) = sign_tot ;
			result_tot.range(1,12) = exp_tot ;
			result_tot.range(13,63) = mantissa_tot ;

			vcl_result.write(result_tot);

			break;

		}
	}
}


void mul_RTL :: elaborate_mul(void){

	/*
  NEXT_STATUS = STATUS;

  switch(STATUS){
    case Reset_ST:
      NEXT_STATUS = ST_0;
      break;

    case ST_0:
      if (number_isready.read() == 1){
        NEXT_STATUS = ST_1;
      } else {
        NEXT_STATUS = ST_0;
      }
      break;

    case ST_1:
      NEXT_STATUS = ST_2;
      break;

    case ST_2:
      if (Counter.read() < 16){
        NEXT_STATUS = ST_3;
      } else {
        NEXT_STATUS = ST_4;
      }
      break;

    case ST_3:
      NEXT_STATUS = ST_2;
      break;

    case ST_4:
      NEXT_STATUS = ST_0;
      break;
  }
	 */
}
