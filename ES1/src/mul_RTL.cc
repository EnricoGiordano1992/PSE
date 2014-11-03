#include "mul_RTL.hh"

void mul_RTL :: elaborate_mul_FSM(void){

	//variabili interne
	static sc_lv<11> exp1, exp2, exp_tot;
	static sc_lv<53> mantissa1, mantissa2;
	static sc_lv<64> mantissa_tot;
	static sc_lv<1>  sign1, sign2, sign_tot;

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

			exp1 = (number_a.read().range(62,52));
			exp2 = (number_b.read().range(62, 52));

			mantissa1 = (number_a.read().range(51,0));
			mantissa2 = (number_b.read().range(51,0));

			sign1 = number_a.read().range(63,63);
			sign2 = number_b.read().range(63,63);

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

			cout << mantissa1.range(52,0) << endl;
			cout << mantissa2.range(52,0) << endl;
			cout << mantissa_tot.range(63,0) << endl;

			break;

			//normalizzo il risultato
		case S4:
			if(mantissa_tot[0] != '1'){
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

			result_tot.range(63,63) = sign_tot ;
			result_tot.range(62,52) = exp_tot ;
			result_tot.range(51,0) = mantissa_tot ;

			vcl_result.write(result_tot);

			break;

		}
	}
}


void mul_RTL :: elaborate_mul(void){


	NEXT_STATUS = STATUS;

	switch(STATUS){
	case SR:
		NEXT_STATUS = S0;
		break;

	case S0:
		if (isready.read() == 1){
			NEXT_STATUS = S0I;
		} else {
			NEXT_STATUS = S0;
		}
		break;

	case S0I:
		NEXT_STATUS = S1;
		break;

	case S1:
		NEXT_STATUS = S2;
		break;

	case S2:
		NEXT_STATUS = S3;
		break;

	case S3:
		NEXT_STATUS = S4;
		break;

	case S4:
		NEXT_STATUS = S5;
		break;

	case S5:
		if(error.read())
			NEXT_STATUS = SR;
		else
			NEXT_STATUS = S6;
		break;

	case S6:
		NEXT_STATUS = S7;
		break;

	case S7:
		if(normalizzato.read())
			NEXT_STATUS = S8;
		else
			NEXT_STATUS = S4;
		break;

	case S8:
		NEXT_STATUS = S9;
		break;

	case S9:
		NEXT_STATUS = SR;
		break;


	}

}
