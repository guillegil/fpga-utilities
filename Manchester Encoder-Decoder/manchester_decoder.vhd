----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 03/31/2019 10:17:37 PM
-- Design Name: 
-- Module Name: manchester_decoder - structural
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

entity manchester_decoder is
    port(
        clk16x:          in STD_LOGIC;
        rst:            in STD_LOGIC;
        data_in:        in STD_LOGIC;
        data_out:       out STD_LOGIC_VECTOR(7 downto 0);
        data_ready:     out STD_LOGIC
    );
end manchester_decoder;

architecture structural of manchester_decoder is

    type STATES is (IDLE, PREPARE, START, DATA, WAIT_READY, STOP);
    signal CURRENT_STATE, NEXT_STATE : STATES := IDLE;

    constant DIVIDE_FACTOR: STD_LOGIC_VECTOR(4 downto 0) := "01000";

    signal SYNQ1, SYNQ2: STD_LOGIC := '0';
    signal COUNT: NATURAL range 0 to 15 := 0;
    signal PULSE_COUNT: NATURAL range 0 to 15 := 0;
    
    component data_decoder is
        port(
            clk:            in STD_LOGIC;
            data_in:        in STD_LOGIC;
            ce:             in STD_LOGIC;
            data_out:       out STD_LOGIC_VECTOR(7 downto 0);
            data_ready:     out STD_LOGIC
        );
    end component;
    
    
    signal CE, CLK2X, DRDY: STD_LOGIC := '0';
    
begin

    DATA_DEC: data_decoder 
        port map(clk => CLK2X, data_in => data_in, ce => CE, data_out => data_out, data_ready => DRDY);

        
    process(clk16x, data_in, rst, DRDY) is
    begin
        if(rst = '0') then
        else if(clk16x'event and clk16x = '1') then
            case CURRENT_STATE is
            when IDLE => 
                if(SYNQ1 /= SYNQ2) then
                    NEXT_STATE <= PREPARE;
                end if;
                
                SYNQ2 <= SYNQ1;
                SYNQ1 <= data_in;
            when PREPARE =>
                if(COUNT = 7) then
                    COUNT <= 0;
                    NEXT_STATE <= START;
                    SYNQ2 <= SYNQ1;
                else
                    COUNT <= COUNT + 1;
                end if;
            when START => 
                if(PULSE_COUNT = 15) then
                    NEXT_STATE <= WAIT_READY;
                    PULSE_COUNT <= 0;
                    COUNT <= 0;
                    SYNQ1 <= '0';
                    CE <= '1';
                    CLK2X <= '1';
                    data_ready <= '0';
                else
                    if(COUNT = 15) then
                        COUNT <= 0;
                        if(SYNQ1 /= SYNQ2) then
                            SYNQ2 <= SYNQ1;
                            PULSE_COUNT <= PULSE_COUNT + 1;
                        else
                            NEXT_STATE <= IDLE;
                            PULSE_COUNT <= 0;
                            SYNQ1 <= '0';
                            SYNQ2 <= '0';
                        end if;
                    else
                        COUNT <= COUNT + 1;
                        SYNQ1 <= data_in;
                    end if;   
                end if;      
            when DATA =>
                NEXT_STATE <= WAIT_READY;
                CLK2X <= '1';
            when WAIT_READY =>
                if(DRDY = '1') then
                    data_ready <= '1';
                    NEXT_STATE <= STOP;
                    CE <= '0';
                    CLK2X <= '0';
                else
                    CE <= '1';
                    if(COUNT = 7) then
                        CLK2X <= not CLK2X;
                        COUNT <= 0;
                    else
                        COUNT <= COUNT + 1;
                    end if;
                end if;
            when STOP =>
                if(data_in = '1') then
                    NEXT_STATE <= PREPARE;
                else
                    if(COUNT = 7) then
                        COUNT <= 0;
                        NEXT_STATE <= IDLE;
                    else
                        COUNT <= COUNT + 1;
                    end if;
                end if;
                
            when others => NEXT_STATE <= IDLE;    
            end case;
        end if;
        end if;
    end process;

    
    CURRENT_STATE <= NEXT_STATE;
end structural;
