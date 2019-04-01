----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 03/31/2019 10:57:15 PM
-- Design Name: 
-- Module Name: synq_detector - structural
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity data_decoder is
    port(
        clk:            in STD_LOGIC;
        data_in:        in STD_LOGIC;
        ce:             in STD_LOGIC;
        data_out:       out STD_LOGIC_VECTOR(7 downto 0);
        data_ready:     out STD_LOGIC
    );
end data_decoder;

architecture structural of data_decoder is

    signal DRDY: STD_LOGIC := '0';
    signal DATA: STD_LOGIC := '0';
    
    signal MANCHESTER_WORD: STD_LOGIC_VECTOR(15 downto 0) := (others => '0');
    signal COUNT: NATURAL range 0 to 15 := 0;
begin

    process(clk, ce, data_in) is
    begin
        if(ce = '1') then
            if(clk'event and clk = '0') then
                if(COUNT = 15) then
                    COUNT <= 0;
                    DRDY <= '1';
                else
                    COUNT <= COUNT + 1;
                    DRDY <= '0';
                end if;  
                
                MANCHESTER_WORD(COUNT) <= data_in;
            end if;
        else
            DRDY <= '0';
        end if;
    end process;
    
    data_out(0) <= MANCHESTER_WORD(1);
    data_out(1) <= MANCHESTER_WORD(3);
    data_out(2) <= MANCHESTER_WORD(5);
    data_out(3) <= MANCHESTER_WORD(7);
    data_out(4) <= MANCHESTER_WORD(9);
    data_out(5) <= MANCHESTER_WORD(11);
    data_out(6) <= MANCHESTER_WORD(13);
    data_out(7) <= MANCHESTER_WORD(15);
    
    
    data_ready <= DRDY;
    
end structural;
