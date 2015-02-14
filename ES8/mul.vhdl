PACKAGE mul_pack IS
  CONSTANT SIZE : INTEGER := 64;
  -- definisco un tipo che corrisponde agli stati della macchina
  TYPE FSM_ST IS (SR, S0, S0I, S1, S2, S3, S31, S4, S5, S6, S7, S8, S9);  
    
  END mul_pack;
  
  LIBRARY IEEE;
  USE IEEE.std_logic_1164.ALL;
  USE WORK.mul_pack.ALL;
  USE IEEE.NUMERIC_BIT.ALL;
  
  
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
  
  variable result_tot: std_logic_vector (SIZE-1 DOWNTO 0);
  variable buff: std_logic_vector (127 DOWNTO 0);

  variable vcl_number_a: std_logic_vector(SIZE-1 DOWNTO 0);
  variable vcl_number_b: std_logic_vector(SIZE-1 DOWNTO 0);
  variable vcl_result: std_logic_vector(SIZE-1 DOWNTO 0);
  variable end_mantissa: std_logic;
  variable error: std_logic;
  variable normalizzato: std_logic;
  variable counter: unsigned (32 downto 0);

     begin
       case status is
	      when SR =>
	        out_result <= '0';

	      when S0 =>
	        out_result <= '0';
	        
	      when S0I =>
	        vcl_number_a := number_a;
	        vcl_number_b := number_b;
	        
	        exp1 := number_a(62 downto 52);
	        exp2 := number_b(62 downto 52);
	       
	        mantissa1 := number_a(51 downto 0); 
	        mantissa2 := number_b(51 downto 0);
	        
	        mantissa1(52) := '1';
	        mantissa2(52) := '1';
	        
	        sign1 := number_a(63);
	        sign2 := number_b(63);
	        
	        counter := 0;
	        mantissa_tot := "0";
	        
	        
	      WHEN S1 =>
	        exp_tot := exp1 + exp2;
	        
	        
	      WHEN S2 =>
	        exp_tot := exp_tot - 1023;
        
        
	      WHEN S3 =>
	        if (counter <= 52) then
	          end_mantissa := '0';
	        
	        else
	          end_mantissa := '1';
	        end if;
	        
	      WHEN S31 =>
	        if(mantissa2(counter) = '1') then
	          buff := mantissa_tot + (mantissa1 sll counter);
	         
	          mantissa_tot := buff;
	          
	        end if;
	        
	        counter := counter + 1;
	        
	      WHEN S4 =>
	        if(mantissa_tot(127 downto 105) /= 0) then
	          mantissa_tot := mantissa_tot srl 1;
	          buff := exp_tot(63 downto 0) + 1;
	          exp_tot := buff;
	          
	        elsif(mantissa_tot(104) /= '1') then
	          mantissa_tot := mantissa_tot sll 1;
	          buff := exp_tot(63 downto 0) - 1;
	          exp_tot := buff;
	          
	        end if;
        
		  WHEN S5 =>
		    if(exp_tot = 0 or mantissa_tot = 0) then
		      error <= '1';
			end if;
		          
	      WHEN S6 =>
	        nop;
		          
		  WHEN S7 =>
		    if(mantissa_tot(127 downto 105) = 0 and mantissa_tot(104) = '1') then
		      normalizzato <= '1';
		        
		    else
		        if(mantissa_tot(127 downto 105) = 0 and mantissa_tot(104 downto 0) = 0) then
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
		    result_tot(51 downto 0) := mantissa_tot(104 downto 52);
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
              out_result <= "0000000000000000000000000000000000000000000000000000000000000000";
            end case;
          end if;
        end process;
        
end mul;
  
  
  
      