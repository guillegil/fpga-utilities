----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 04/02/2019 01:10:22 PM
-- Design Name: 
-- Module Name: serializer - structural
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

entity manchester_encoder is
    generic(
        PRE: STD_LOGIC := '0';
        DATA_WIDTH: NATURAL := 8;
        PREAMBLE_WIDTH: NATURAL := 8
    );
    port(
        clk:        in STD_LOGIC;
        start_tx:   in STD_LOGIC;
        data_in:    in STD_LOGIC_VECTOR(DATA_WIDTH - 1 downto 0);
        data_out:   out STD_LOGIC;
        busy:       out STD_LOGIC
    );
end manchester_encoder;

architecture structural of manchester_encoder is

    type STATES is (IDLE, TX0, SEND_DATA, SEND_STOP);
    signal CURRENT_STATE, NEXT_STATE: STATES := IDLE;
    
    constant START_WORD: STD_LOGIC_VECTOR(PREAMBLE_WIDTH - 1 downto 0) := x"00";
    
    
    signal FRAME: STD_LOGIC_VECTOR(PREAMBLE_WIDTH + DATA_WIDTH - 1 downto 0) := (others => '0');
    signal DOUT: STD_LOGIC := '0';
    signal ENABLE: STD_LOGIC := '0';
    
    
begin
    
    process(clk, start_tx) is
        variable COUNT: NATURAL range 0 to (PREAMBLE_WIDTH + DATA_WIDTH) := 0;
    begin
        if(clk'event and clk = '1') then
            case CURRENT_STATE is
            when IDLE =>
                    if(start_tx = '1') then
                        NEXT_STATE <= TX0;
                    end if;
            when TX0 =>
                    if(start_tx = '0') then
                        NEXT_STATE <= SEND_DATA;
                        busy <= '1';
                        FRAME <= data_in & START_WORD;
                        DOUT <= FRAME(0);
                        ENABLE <= '1';
                    end if;
            when SEND_DATA =>
                if(COUNT = PREAMBLE_WIDTH + DATA_WIDTH - 1) then
                    NEXT_STATE <= SEND_STOP;
                    COUNT := 0;
                    ENABLE <= '0';
                else
                    COUNT := COUNT + 1;
                    DOUT <= FRAME(COUNT);
                end if;
                
            when SEND_STOP =>
                if(COUNT = PREAMBLE_WIDTH + DATA_WIDTH) then
                    COUNT := 0;
                    NEXT_STATE <= IDLE;
                    busy <= '0';
                else
                    COUNT := COUNT + 1;
                end if;
                
                ENABLE <= '0';
                DOUT <= '0';
            when others => NEXT_STATE <= IDLE;
            end case;
        end if;
    end process;
    
    
    CURRENT_STATE <= NEXT_STATE;
    data_out <= ((DOUT xor clk) and ENABLE) when PRE = '0' else ((not(DOUT) xor clk) nand ENABLE);
                  

end structural;
