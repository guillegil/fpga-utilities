----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 03/31/2019 08:41:02 PM
-- Design Name: 
-- Module Name: manchester_encoder - structural
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
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity manchester_encoder is
    port(
        clk2x:      in STD_LOGIC;
        rst:        in STD_LOGIC;
        start_tx:   in STD_LOGIC;
        data_in:    in STD_LOGIC_VECTOR(7 downto 0);
        data_out:   out STD_LOGIC;
        busy:       out STD_LOGIC 
    );
end manchester_encoder;

architecture structural of manchester_encoder is

    constant START_WORD: STD_LOGIC_VECTOR(15 downto 0) := x"5555";   -- "0101010101010101"
    constant STOP_WORD: STD_LOGIC_VECTOR(15 downto 0) := x"AAAA";    -- "1010101010101010"
    
    type STATES is (IDLE, SEND_START, SEND_DATA, WAIT1, SEND_STOP);
    signal CURRENT_STATE, NEXT_STATE: STATES := IDLE;
    
    
    signal DIN: STD_LOGIC_VECTOR(7 downto 0) := (others => '0');
    signal MANCHESTER_WORD: STD_LOGIC_VECTOR(15 downto 0) := (others => '0');
    signal DOUT, BSY: STD_LOGIC;
    
begin

    process(clk2x, rst) is  
        variable COUNT: NATURAL range 0 to 15 := 0;
    begin
        if(rst = '0') then
        
        else if(clk2x'event and clk2x ='1') then
            case CURRENT_STATE is
            when IDLE =>
                if(start_tx = '1') then
                    NEXT_STATE <= SEND_START;
                    DIN <= data_in;
                    BSY <= '1';
                    DOUT <= START_WORD(0);
                else
                    BSY <= '0';
                    DOUT <= '0';
                end if;
           
                
            when SEND_START =>
                if(COUNT = 15) then
                    COUNT := 0;
                    NEXT_STATE <= SEND_DATA;
                    DOUT <= MANCHESTER_WORD(0);    
                else
                    if(count < 8) then
                        MANCHESTER_WORD(2*COUNT) <= not(DIN(COUNT));
                        MANCHESTER_WORD(2*COUNT + 1) <= DIN(COUNT);
                    end if;
                    
                    COUNT := COUNT + 1;
                    DOUT <= START_WORD(COUNT);
                end if;                
            when WAIT1 => 
                DOUT <= MANCHESTER_WORD(0);    
                NEXT_STATE <= SEND_DATA;
            when SEND_DATA =>
                if(COUNT = 15) then
                    COUNT := 0;
                    NEXT_STATE <= SEND_STOP;
                    DOUT <= '0';
                    BSY <= '1';
                else
                    COUNT := COUNT + 1;
                    DOUT <= MANCHESTER_WORD(COUNT);
                end if;
            when SEND_STOP => 
                if(COUNT = 15) then
                    COUNT := 0;
                    NEXT_STATE <= IDLE;
                else
                    COUNT := COUNT + 1;
                end if;
                
                DOUT <= '0';
            end case;
        end if;
        end if;
    end process;

    CURRENT_STATE <= NEXT_STATE;
    data_out <= DOUT;
    busy <= BSY;

end structural;
