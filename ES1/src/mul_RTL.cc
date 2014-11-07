#include "mul_RTL.hh"

void mul_RTL :: elaborate_mul_FSM(void){

	//variabili interne
	static sc_lv<11> exp1, exp2;
	static sc_lv<64> exp_tot;

	static sc_lv<128> mantissa1, mantissa2;
	static sc_lv<128> mantissa_tot;

	static sc_lv<1>  sign1, sign2, sign_tot;
	static sc_uint<6> counter;

	sc_lv<64> result_tot;
	static sc_biguint<128> buffer;



	if (reset.read() == 0){
		STATUS = SR;
	}
	else if (clk.read() == 1) {


		STATUS = NEXT_STATUS;

		switch(STATUS){
		//allo stato di reset non fa niente
		case SR:
			out_result.write(0);
			result_isready.write(0);
			end_mantissa.write(false);

			break;

			//preparazione del sistema
		case S0:
			out_result.write(0);
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

			mantissa1[52] = '1';
			mantissa2[52] = '1';

			sign1 = number_a.read().range(63,63);
			sign2 = number_b.read().range(63,63);

			counter = 0;
			mantissa_tot = "0";

			cout << "mantissa1:" << mantissa1.range(52,0) << endl;
			cout << "mantissa2:" << mantissa1.range(52,0) << endl;

			break;

			//sommo gli esponenti
		case S1:
			exp_tot = static_cast< sc_int<11> >( exp1 ) + static_cast< sc_int<11> >( exp2 );

			break;

			//polarizzo il risultato
		case S2:
			exp_tot = static_cast< sc_int<11> >( exp_tot ) - 1023;

			break;

			//prodotto tra mantisse: controllo se devo fare altri passi
		case S3:
			if(counter <= 52){
				cout << counter << " continuo a moltiplicare" << endl;
				end_mantissa.write(false);
			}
			else
				end_mantissa.write(true);

			break;

			//prodotto tra mantisse: eseguo l'algoritmo
		case S31:

			if(mantissa2[counter] == '1') {

//				buffer.range(counter + 52, counter) = static_cast< sc_biguint< 128 > > (mantissa_tot).range(counter + 52, counter) + static_cast< sc_uint<53> >(mantissa1).range(52,0);
//				buffer = static_cast< sc_biguint< 128 > > (mantissa_tot) << 1;
//				buffer = buffer + static_cast< sc_uint<53> >(mantissa1).range(52,0);

				buffer = static_cast< sc_biguint< 128 > > (mantissa_tot) + (static_cast< sc_biguint<128> >(mantissa1) << (counter));

				mantissa_tot = static_cast< sc_lv< 128 > > (buffer);

				cout << "moltiplico=>" << mantissa_tot.range(127,0) << endl;

			}

			++counter;

			break;

			//normalizzo il risultato: 2 controlli
		case S4:


//			if(static_cast< sc_biguint< 128 > > (mantissa_tot).range(52,0) != 0){
				//controllo che non ci siano numeri dopo il 53-esimo bit
				if(static_cast< sc_biguint< 128 > > (mantissa_tot).range(127,53) != 0){


					mantissa_tot = mantissa_tot >> 1;
					static_cast< sc_uint<11> >( exp_tot )++;
					cout << "shift a destra=>" << mantissa_tot.range(127,0) << endl;

				}

				else if(mantissa_tot[52] != '1'){
					mantissa_tot  = mantissa_tot << 1;
					exp_tot = static_cast< sc_uint<11> >( exp_tot )--;
					cout << "shift a sinistra=>" << mantissa_tot.range(127,0) << endl;

				}
//			}
			break;

			//overflow?
		case S5:
			if(static_cast< sc_uint<11> >( exp_tot ) == 0 || static_cast< sc_biguint<128> >( mantissa_tot ) == 0)
				error.write(true);

			break;

			//?
		case S6:
			//arrotondare (ma è stato gia' fatto, il bit di precisione e' gia' andato perso)
			break;

			//devo normalizzare ancora?
		case S7:
			if(((static_cast< sc_biguint<128> > (mantissa_tot).range(127,53) == 0) && (mantissa_tot[52] == '1')))
				normalizzato.write( true );
			else {
				if((static_cast< sc_biguint<128> > (mantissa_tot).range(127,53) == 0) && (static_cast< sc_biguint< 128 > > (mantissa_tot).range(52,0)) == 0)
					normalizzato.write( true );
				else
					normalizzato.write( false );
			}
			break;

			//calcolo del segno
		case S8:
			sign_tot = sign1 ^ sign2;

			break;

		case S9:

			result_tot.range(63,63) = sign_tot ;
			result_tot.range(62,52) = exp_tot ;
			result_tot.range(51,0) = mantissa_tot.range(51,0) ;
//			result_tot[51] = '1';

			out_result.write(result_tot);
			result_isready.write(1);

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
		if(end_mantissa.read())
			NEXT_STATUS = S4;
		else
			NEXT_STATUS = S31;

		break;

	case S31:
		NEXT_STATUS = S3;
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
