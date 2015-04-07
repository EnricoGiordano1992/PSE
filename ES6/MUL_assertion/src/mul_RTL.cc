#include "mul_RTL.hh"

void mul_RTL :: elaborate_mul_FSM(void){

	//variabili interne
	static sc_lv<64> exp1, exp2;
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

			cout << "numero 1: " << number_a.read() << endl;
			cout << "numero 2: " << number_b.read() << endl;
			cout << "mantissa1:" << mantissa1.range(52,0) << endl;
			cout << "mantissa2:" << mantissa1.range(52,0) << endl;
			cout << "exp1: " << exp1 << endl;
			cout << "exp2: " << exp2 << endl;

			break;

			//sommo gli esponenti
		case S1:
			exp_tot = static_cast< sc_int<64> >( exp1 ) + static_cast< sc_int<64> >( exp2 );

			break;

			//polarizzo il risultato
		case S2:
			cout << "exp tot:" << static_cast< sc_int<64> >( exp_tot ) << endl;
			exp_tot = static_cast< sc_int<64> >( exp_tot ) - 1023;
			cout << "exp tot normalizzato:" << static_cast< sc_int<64> >( exp_tot ) << endl;

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


			//controllo che non ci siano numeri dopo il 104-esimo bit
			if(static_cast< sc_biguint< 128 > > (mantissa_tot).range(127,105) != 0){


				mantissa_tot = mantissa_tot >> 1;
				buffer = static_cast< sc_int<64> >( exp_tot ).range(63,0) + 1;
				exp_tot = static_cast< sc_lv<64> > (buffer);
				cout << "exp: " << static_cast< sc_int<64> >( exp_tot ) << endl;
				cout << "shift a destra=>" << mantissa_tot.range(127,0) << endl;

			}

			else if(mantissa_tot[104] != '1'){
				mantissa_tot  = mantissa_tot << 1;
				buffer = static_cast< sc_int<64> >( exp_tot ).range(63,0) - 1;
				exp_tot = static_cast< sc_lv<64> > (buffer);
				cout << "exp: " << static_cast< sc_int<64> >( exp_tot ) << endl;
				cout << "shift a sinistra=>" << mantissa_tot.range(127,0) << endl;

			}
			break;

			//overflow?
		case S5:
			if(static_cast< sc_uint<64> >( exp_tot ) == 0 || static_cast< sc_biguint<128> >( mantissa_tot ) == 0)
				error.write(true);

			break;

			//?
		case S6:
			//arrotonda
			break;

			//devo normalizzare ancora?
		case S7:
			if(((static_cast< sc_biguint<128> > (mantissa_tot).range(127,105) == 0) && (mantissa_tot[104] == '1')))
				normalizzato.write( true );
			else {
				if((static_cast< sc_biguint<128> > (mantissa_tot).range(127,105) == 0) && ((static_cast< sc_biguint< 128 > > (mantissa_tot).range(104,0)) == 0)){
					normalizzato.write( true ); }
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
			result_tot.range(62,52) = exp_tot.range(10,0) ;
			//			mantissa_tot = mantissa_tot << 1;

			result_tot.range(51,0) = mantissa_tot.range(104,52) ;
			//			result_tot[52] = '1';


			cout << "res: " << result_tot.range(63,0) << endl;
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



// CHECKERS

void mul_RTL :: property1(void){
  // if number_isready == 1 then result_isready == 1 
  // in less than 100 clock cycles
  
  int count = 0;
  bool true_property = false;

  while(true){
    if (number_isready.read() == 1){
      while((count < 100)&(!true_property)){
      wait();
      if (result_isready.read() == 1)
	true_property = true;
      }
      
      if (true_property)
	cout<<"\033[34m"<<"[CHECK: ] property 1 is true"<<"\033[0m"<<endl;
      else {
	cout<<"\033[34m"<<"[CHECK: ] property 1 is false"<<"\033[0m"<<endl;
	sc_stop();
      }
      count = 0;
      true_property = false;
    }
    wait();
  }
}

void mul_RTL :: property2(void){
  // if number_isready == 0 then result_port == 0 and result_isready == 0
  
  while(true){
    if (number_isready.read() == 0){
      if ((result_port.read() == 0)&&(result_isready.read() == 0))
	cout<<"\033[35m"<<"[CHECK: ] property 2 is true"<<"\033[0m"<<endl;
      else{
	cout<<"\033[35m"<<"[CHECK: ] property 2 is false"<<"\033[0m"<<endl;
	sc_stop();
      }
    }
    wait();
  }
}
  
void mul_RTL :: property3(void){
  // if Counter < 16 then STATUS = ST4

  while(true){
    if (Counter.read() < 16){
      if (STATUS.read() == ST_4)       
	cout<<"\033[32m"<<"[CHECK: ] property 3 is true"<<"\033[0m"<<endl;
      else{
	cout<<"\033[32m"<<"[CHECK: ] property 3 is false"<<"\033[0m"<<endl;
	sc_stop();
      }
    }
    wait();
  }


}
