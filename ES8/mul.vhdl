PACKAGE mul_pack IS
  CONSTANT SIZE : INTEGER := 64;
  -- definisco un tipo che corrisponde agli stati della macchina
  TYPE FSM_ST IS (SR, S0, S0I, S1, S2, S3, S31, S4, S5, S7, S8, S9);  
    
  END mul_pack;
  
  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE WORK.mul_pack.ALL;
  use ieee.numeric_std.all;

  --definisco l'entità MUL
  entity mul is
    port (
      clock, reset   : in  bit;
      isready: in std_logic;
      number_a: in std_logic_vector (SIZE-1 DOWNTO 0);
      number_b: in std_logic_vector (SIZE-1 DOWNTO 0);

      result_isready: out bit;
      out_result: out std_logic_vector (SIZE-1 DOWNTO 0)
    );
    
end mul;


  --definisco l'architettura di MUL
  architecture mul of mul is

  
  --definisco i segnali
  
  signal status: FSM_ST;
  signal next_status: FSM_ST;
  signal error: std_logic;
  signal normalizzato: std_logic;
  signal end_mantissa: std_logic;

   
--inizio a descrivere il comportamento della mia architettura
 begin 
   -- quello che in systemC era mul_RTL
   process(status, isready, number_a, number_b)

  --scrivo le variabili che mi servono
  variable exp1: std_logic_vector (SIZE-1 DOWNTO 0);
  variable exp2: std_logic_vector (SIZE-1 DOWNTO 0);
  variable exp_tot: std_logic_vector (SIZE-1 DOWNTO 0);
  
  variable mantissa1: std_logic_vector (127 DOWNTO 0);
  variable mantissa2: std_logic_vector (127 DOWNTO 0);
  variable mantissa_tot: std_logic_vector (127 DOWNTO 0); 
  
  variable sign1: std_logic;
  variable sign2: std_logic;

  variable sign_tot: std_logic;
  
  variable result_tot: std_logic_vector (SIZE-1 DOWNTO 0);
  variable buff: std_logic_vector (127 DOWNTO 0);

  variable vcl_number_a: std_logic_vector(SIZE-1 DOWNTO 0);
  variable vcl_number_b: std_logic_vector(SIZE-1 DOWNTO 0);
  variable vcl_result: std_logic_vector(SIZE-1 DOWNTO 0);
  variable counter: unsigned (32 downto 0);

     begin
       case status is
	      when SR =>
	        for i in 0 to 63 loop 
	           out_result(i) <= '0';
          end loop;
	        for i in 0 to 127 loop 
	           mantissa_tot(i) := '0';
          end loop;
	        for i in 0 to 127 loop 
	           mantissa1(i) := '0';
          end loop;
	        for i in 0 to 127 loop 
	           mantissa2(i) := '0';
          end loop;
	        for i in 0 to 63 loop 
	           buff(i) := '0';
          end loop;
	        for i in 0 to SIZE-1 loop 
	           exp_tot(i) := '0';
          end loop;
	        for i in 0 to SIZE-1 loop 
	           exp1(i) := '0';
          end loop;
	        for i in 0 to SIZE-1 loop 
	           exp2(i) := '0';
          end loop;
          
          counter := to_unsigned(0, 33);
          

	      when S0 =>
	        for i in 0 to 63 loop 
	           out_result(i) <= '0';
          end loop;
	        
	      when S0I =>
	        vcl_number_a := number_a;
	        vcl_number_b := number_b;
	        
	        exp1(10 downto 0) := number_a(62 downto 52);
	        exp2(10 downto 0) := number_b(62 downto 52);
	       
	        mantissa1(51 downto 0) := number_a(51 downto 0); 
	        mantissa2(51 downto 0) := number_b(51 downto 0);
	        
	        mantissa1(52) := '1';
	        mantissa2(52) := '1';
	        
	        sign1 := number_a(63);
	        sign2 := number_b(63);
	        
	        counter := to_unsigned(0, 33);
	        
	      WHEN S1 =>
	        --magic cast
	        exp_tot := std_logic_vector((unsigned(exp1)) + (unsigned(exp2)));
	        
	        
	      WHEN S2 =>
	        exp_tot := std_logic_vector((unsigned(exp_tot) - 1023));
        
        
	      WHEN S3 =>
	        if (counter <= 52) then
	          end_mantissa <= '0';
	        
	        else
	          end_mantissa <= '1';
	        end if;
	        
	      WHEN S31 =>
	        if(mantissa2(to_integer(counter)) = '1') then
	          buff := std_logic_vector((unsigned(mantissa_tot) + (unsigned(mantissa1) sll to_integer(unsigned(counter)))));
	         
	          mantissa_tot := buff;
	          
	        end if;
	        
	        counter := counter + 1;
	        
	      WHEN S4 =>
	        if(unsigned(mantissa_tot(127 downto 105)) /= 0) then
	          mantissa_tot := std_logic_vector(unsigned(mantissa_tot) srl 1);
	          buff := std_logic_vector(to_unsigned(to_integer(unsigned(exp_tot(63 downto 0))) + 1, 128));
	          exp_tot := buff(63 downto 0);
	          
	        elsif(mantissa_tot(104) /= '1') then
	          mantissa_tot := std_logic_vector(unsigned(mantissa_tot) sll 1);
	          buff(63 downto 0) := std_logic_vector(unsigned(exp_tot(63 downto 0)) - 1);
	          exp_tot := buff(63 downto 0);
	          
	        end if;
        
		  WHEN S5 =>
		    if(unsigned(exp_tot) = 0 or unsigned(mantissa_tot) = 0) then
		      error <= '1';
			end if;

		          		          
		  WHEN S7 =>
		    if(unsigned(mantissa_tot(127 downto 105)) = 0 and mantissa_tot(104) = '1') then
		      normalizzato <= '1';
		        
		    else
		        if(unsigned(mantissa_tot(127 downto 105)) = 0 and unsigned(mantissa_tot(104 downto 0)) = 0) then
		          normalizzato <= '1';
		        else
		          normalizzato <= '0';
		        end if;
		    end if;
		      
		  WHEN S8 =>
		    sign_tot := sign1 and sign2;
		      
		  WHEN S9 =>
		    result_tot(63) := sign_tot;
		    result_tot(62 downto 52) := exp_tot(10 downto 0);
		    result_tot(51 downto 0) := mantissa_tot(103 downto 52);  --prima era downto 52
		    out_result <= result_tot;
		    result_isready <= '1';
	          
  
       end case;
     end process;
    
    -- quello che in systemC era elaborate_mul_RTL
    process(clock, reset)
      begin
        if reset = '0' then
          status <= SR;
          
        elsif clock'event and clock='1' then
          status <= next_status;
          
          case next_status is
            when SR =>
                next_status <= S0;            	 
              
            when S0 =>
              if(isready = '1') then
                next_status <= S0I;
              else
                next_status <= S0;
              end if;
              
            when S0I =>
                next_status <= S1;
                
            when S1 =>
                next_status <= S2;

            when S2 =>
                next_status <= S3;

            when S3 =>
                if(end_mantissa = '1') then
                  next_status <= S4;
                else
                  next_status <= S31;                
                end if;
                  
            when S31 =>
                next_status <= S3;

            when S4 =>
              if(normalizzato = '1') then
                  next_status <= S8;
              else
                next_status <= S5;
              end if;

            when S5 =>
                if(error = '1') then
                  next_status <= SR;
                else
                  next_status <= S7;
                end if;
            
                  
            when S7 =>
              if(normalizzato = '1') then
                  next_status <= S8;
              else
                  next_status <= S4;
              end if;
                  
            when S8 =>
                next_status <= S9;

            when S9 =>
                next_status <= SR;

              
            end case;
          end if;
        end process;
        
end mul;
  
  
  
      