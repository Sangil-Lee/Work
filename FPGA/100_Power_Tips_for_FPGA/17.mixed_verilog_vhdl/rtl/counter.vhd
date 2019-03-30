-------------------------------------------------------------------------------
-- Copyright (C) 2009 OutputLogic.com 
-- This source file may be used and distributed without restriction 
-- provided that this copyright statement is not removed from the file 
-- and that any derivative work contains the original copyright notice 
-- and the associated disclaimer. 
-- 
-- THIS SOURCE FILE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS 
-- OR IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED	
-- WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. 
-------------------------------------------------------------------------------
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity lfsr_counter is 
  port ( 
    ce , rst, clk : in std_logic;
    lfsr_done : out std_logic);
end lfsr_counter;

architecture imp_lfsr_counter of lfsr_counter is	
  signal lfsr: std_logic_vector (10 downto 0);	
  signal d0, lfsr_equal: std_logic;	
begin 

  d0 <= lfsr(10) xnor lfsr(8) ;

  process(lfsr) begin 
    if(lfsr = x"359") then 
      lfsr_equal <= '1';
    else 
      lfsr_equal <= '0';
    end if;
  end process; 

    process (clk,rst) begin 
      if (rst = '1') then 
        lfsr <= b"00000000000";
        lfsr_done <= '0'; 
      elsif (clk'EVENT and clk = '1') then 
        lfsr_done <= lfsr_equal; 
        if (ce = '1') then 
          if(lfsr_equal = '1') then 
            lfsr <= b"00000000000";
       	  else 
            lfsr <= lfsr(9 downto 0) & d0; 
       	  end if; 
        end if; 
      end if; 
    end process; 
end architecture imp_lfsr_counter; 