----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 04/01/2019 01:53:34 AM
-- Design Name: 
-- Module Name: med_tb - test_bench
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

entity med_tb is end;

architecture test_bench of med_tb is

    component manchester_encoder is
        port(
            clk2x:      in STD_LOGIC;
            rst:        in STD_LOGIC;
            start_tx:   in STD_LOGIC;
            data_in:    in STD_LOGIC_VECTOR(7 downto 0);
            data_out:   out STD_LOGIC;
            busy:       out STD_LOGIC 
        );
    end component;
    
    component manchester_decoder is
        port(
            clk16x:          in STD_LOGIC;
            rst:            in STD_LOGIC;
            data_in:        in STD_LOGIC;
            data_out:       out STD_LOGIC_VECTOR(7 downto 0);
            data_ready:     out STD_LOGIC
        );
    end component;

    signal CLK2X: STD_LOGIC := '0';
    signal CLK16X: STD_LOGIC := '1';
    
    signal RST: STD_LOGIC := '1';
    signal START_TX: STD_LOGIC := '1';
    signal DATA_IN: STD_LOGIC_VECTOR(7 downto 0) := x"96";
    signal DATA_DEC_OUT: STD_LOGIC_VECTOR(7 downto 0) := (others => '0');
    signal DATA_EN_OUT: STD_LOGIC := '0';
    signal BUSY: STD_LOGIC := '0';
    
    signal DATA_READY: STD_LOGIC := '0';


    type test_record is record
        start_tx : std_logic ;
        din : std_logic_vector (7 downto 0) ;
    end record ;

    type test_array is array(positive range<>) of test_record ;
    
    constant test_vectors : test_array := (
        ('1',x"FF"),
        ('1',x"14"),
        ('1',x"00"),
        ('1',x"01"),
        ('1',x"F3"),
        ('1',x"00"),
        ('1',x"56"),
        ('1',x"33"),
        ('1',x"12"),
        ('1',x"45"),
        ('1',x"79")
    );
    
begin

    ME: manchester_encoder port map(CLK2X, RST, START_TX, DATA_IN, DATA_EN_OUT, BUSY);
    MD: manchester_decoder port map(CLK16X, RST, DATA_EN_OUT, DATA_DEC_OUT, DATA_READY);

    process
    begin
    
        CLK2X <= '1';
        wait for 50us;
        CLK2X <= '0';
        wait for 50us;
        CLK2X <= '1';
        
    end process;


    process
    begin
    
        CLK16X <= '0';
        wait for 3125ns;
        CLK16X <= '1';
        wait for 3125ns;
        CLK16X <= '0';
        
    end process;


    process
        	variable vector : test_record ;
        	variable B: STD_LOGIC := '0';
    begin
        for index in test_vectors'range loop
            vector := test_vectors(index);
            
            DATA_IN <= vector.din;
            START_TX <= vector.start_tx;
            
            wait for 400ms;
            START_TX <= '0';
            
            wait until BUSY = '0';
            
            --- wait for 50ns;
            
        end loop;
        wait;
    end process;
end test_bench;
