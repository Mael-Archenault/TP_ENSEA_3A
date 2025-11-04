library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

use work.CHECK_PKG.all;

entity tb_Nbits_register is
end entity tb_Nbits_register;

architecture Behavioral of tb_Nbits_register is

    constant N : integer := 8;  -- Number of bits for the register

    signal clk   : STD_LOGIC := '0';
    signal reset : STD_LOGIC := '0';
    signal d     : STD_LOGIC_VECTOR(N-1 downto 0) := (others => '0');
    signal q     : STD_LOGIC_VECTOR(N-1 downto 0);

    component Nbits_register
        generic (
            N : integer := 8
        );
        port (
            clk   : in  STD_LOGIC;
            reset : in  STD_LOGIC;
            d     : in  STD_LOGIC_VECTOR(N-1 downto 0);
            q     : out STD_LOGIC_VECTOR(N-1 downto 0)
        );
    end component Nbits_register;

begin
    u1: Nbits_register
        generic map (
            N => N
        )
        port map (
            clk   => clk,
            reset => reset,
            d     => d,
            q     => q
        );

    -- Clock generation
    clk_process : process
    begin
        while now < 200 ns loop
            clk <= '0';
            wait for 10 ns;
            clk <= '1';
            wait for 10 ns;
        end loop;
        wait;
    end process clk_process;

    -- Test sequence
    stim_process : process
    begin
        -- Initial reset
        reset <= '1';
        d<= "00000000";
        wait for 10 ns;
        reset <= '0';
        wait for 10 ns;
        wait for 8 ns;

        d <= "00001111";
        wait for 14 ns;
        d <= "11110000";
        wait for 20 ns;
        d <= "10101010";
        wait for 10 ns;
        d <= "01010101";
        wait for 10 ns;
        d <= "11111111";
        wait;
    end process stim_process;  
end architecture Behavioral; 

