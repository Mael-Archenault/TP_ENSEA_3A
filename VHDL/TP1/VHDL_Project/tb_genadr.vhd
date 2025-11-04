
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

entity tb_genadr is
end entity tb_genadr;


architecture arch of tb_genadr is

    component genadr
        generic (
            M : integer := 5
        );
        port (
            reset : in STD_LOGIC;
            clk : in STD_LOGIC;
            incread : in STD_LOGIC;
            incwrite : in STD_LOGIC;
            selread : in STD_LOGIC;
            adrg: out STD_LOGIC_VECTOR (M-1 downto 0)
        );
    end component genadr;

    signal clk    : std_logic := '0';
    signal reset  : std_logic := '0';
    signal incread : std_logic := '0';
    signal incwrite: std_logic := '0';
    signal selread : std_logic := '0';
    signal adrg   : std_logic_vector(4 downto 0);
begin

    u1: genadr
        generic map (
            M => 5
        )
        port map (
            reset  => reset,
            clk    => clk,
            incread => incread,
            incwrite=> incwrite,
            selread => selread,
            adrg   => adrg
        );

    process
    begin
        -- Clock generation
        while now < 1000 ns loop
            clk <= '0';
            wait for 5 ns;
            clk <= '1';
            wait for 5 ns;
        end loop;
        wait;
    end process;
    process
    begin
        -- Test sequence
        reset <= '1';
        wait for 10 ns;
        reset <= '0';
        selread <= '1';
        wait for 40 ns;
        incread <= '1';
        wait for 40 ns;
        incread <= '0';

        selread <= '0';
        wait for 40 ns;
        incwrite <= '1';
        wait for 20 ns;

        selread <= '1';
        incwrite <= '0';
        wait for 40 ns;


        reset <= '1';
        wait for 10 ns;
        reset <= '0';
        incwrite <= '0';
        
        wait;
        

    end process;


end arch ; -- arch